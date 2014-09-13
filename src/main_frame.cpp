#include "main_frame.hpp"
#include "wx/app.h"
#include "wx/mstream.h"
#include "tamandua/tamandua.hpp"
#include "tamandua_box.hpp"
#include "debug_gui.hpp"
#include <string>
#include <sstream>
#include <functional>

wxDECLARE_APP(gui_client);

main_frame::main_frame() :
	wxFrame(0, wxID_ANY, wxT("Tamandua GUI Client"), wxPoint(100,100), wxSize(600,800)),
	connected(false),
	verified(true)
{
	panel = new wxPanel(this);
	msg = new wxTextCtrl(panel, MSG_CTRL, wxEmptyString, wxPoint(0,0), wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);
	notebook = new chat_notebook(panel, CHAT_NOTEBOOK, msg);
	statusbar = new wxStatusBar(this, wxID_ANY);
	main_sizer = new wxBoxSizer(wxVERTICAL);
	connect_sizer = new wxBoxSizer(wxHORIZONTAL);
	connect_host = new wxTextCtrl(panel, CON_HOST_TEXT, wxT("localhost"), wxPoint(0,0), wxDefaultSize, wxTE_PROCESS_ENTER);
	connect_port = new wxTextCtrl(panel, CON_PORT_TEXT, wxT("5000"), wxPoint(0,0), wxDefaultSize, wxTE_PROCESS_ENTER);
	connect_button = new wxButton(panel, CON_BTN, wxT("Connect"));
	cols_sizer = new wxBoxSizer(wxHORIZONTAL);
	msg_sizer = new wxBoxSizer(wxVERTICAL);
	rooms_list = new wxListBox(panel, LISTBOX_ROOMS, wxPoint(0,0), wxDefaultSize, 0, NULL, wxLB_SINGLE | wxLB_SORT);
	participants_list = new wxListBox(panel, LISTBOX_PARTICIPANTS, wxPoint(0,0), wxDefaultSize, 0, NULL, wxLB_SINGLE | wxLB_SORT);
	statusbar->SetFieldsCount(2);

	panel->SetSizer(main_sizer);
	connect_sizer->Add(connect_host,1);
	connect_sizer->Add(connect_port,1);
	connect_sizer->Add(connect_button,1);
	main_sizer->Add(connect_sizer, 0, wxALL | wxEXPAND, 10);
	main_sizer->Add(cols_sizer, 3, wxBOTTOM | wxLEFT | wxRIGHT | wxEXPAND, 10);

	cols_sizer->Add(rooms_list, 2, wxALL | wxEXPAND, 5);
	cols_sizer->Add(msg_sizer, 6, wxALL | wxEXPAND, 5);
	cols_sizer->Add(participants_list, 2, wxALL | wxEXPAND, 5);
	msg_sizer->Add(notebook, 3, wxBOTTOM | wxLEFT | wxRIGHT | wxEXPAND, 5);
	msg_sizer->Add(msg, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 5);

	statusbar->SetStatusText(wxT("Not connected"));

	// menu bar
	menubar = new wxMenuBar;

	// TMD menu
	wxMenu *tmd_menu = new wxMenu;
	tmd_menu->Append(TMD_MENU_RLIST, wxT("Rooms list\tCtrl+R"));
	tmd_menu->Append(TMD_MENU_PLIST, wxT("Participants list\tCtrl+P"));

	menubar->Append(tmd_menu, wxT("TMD"));

	SetMenuBar(menubar);
	SetStatusBar(statusbar);
}

void main_frame::send_message(wxCommandEvent &event)
{
	last_msg_content = msg->GetValue();
	msg->Clear();
	std::string msg_body = std::string(last_msg_content.utf8_str());
	tamandua::id_number_t group = notebook->get_current_group_id();
	tb->client.send_message(msg_body, group);
}

void main_frame::connect(wxCommandEvent &event)
{
	std::string host(connect_host->GetValue().utf8_str());
	std::string port(connect_port->GetValue().utf8_str());

	tb = new tamandua_box;
	tb->client.add_event_handler(tamandua::event_type::connecting_succeeded,
		std::bind(&main_frame::connect_callback_, this, std::placeholders::_1));
	tb->client.add_event_handler(tamandua::event_type::connecting_failed,
		std::bind(&main_frame::connect_callback_, this, std::placeholders::_1));
	tb->client.add_event_handler(tamandua::event_type::participants_list_received,
		[this](tamandua::status) {
			set_plist();
		});
	tb->client.add_event_handler(tamandua::event_type::rooms_list_received,
		[this](tamandua::status) {
			set_rlist();
		});

	tb->client.get_socket().set_verify_callback([this](bool pv, boost::asio::ssl::verify_context &ctx)
	{
		char subject_name[256];
		X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
		X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
		Debug("X509: ", subject_name);
		if (pv)
		{
			if (verified)
				verified = true;
		} else
		{
			verified = false;
		}

		return pv;
	});
	
	tb->client.connect(host, port);
	
	if (verified)
		context_verified_true_();
	else
		context_verified_false_();

	tb->io_service_thread = std::thread([this]() {
		tb->io_service.run();
	});
	tb->reader_thread = std::thread([this]() {
		tamandua::client &cl = tb->client;
		bool local_running = true;
		do {
			auto msg_pair = cl.get_next_message();
			wxTheApp->GetTopWindow()->GetEventHandler()->CallAfter(std::bind(&chat_notebook::add_message, notebook, msg_pair));
			tb->running_lock.lock();
			local_running = tb->running;
			tb->running_lock.unlock();
		} while (local_running);
	});
}

void main_frame::disconnect(wxCommandEvent &event)
{
	if (tb->client.is_connected())
	{
		tb->client.disconnect();
		tb->turn_off();
		tb->io_service_thread.join();
		tb->reader_thread.join();
		disconnect_callback_(tamandua::ok);
	}
}

void main_frame::connect_callback_(tamandua::status st)
{
	if (st == tamandua::status::ok)
	{
		connected = true;
		connect_button->Unbind(wxEVT_BUTTON, &main_frame::connect, this);
		connect_button->Bind(wxEVT_BUTTON, &main_frame::disconnect, this);
		connect_button->SetLabel(wxT("Disconnect"));
		statusbar->SetStatusText(wxT("Connected"), 0);
	} else
	{
		statusbar->SetStatusText(wxT("Connecting failed!"), 0);
	}
}

void main_frame::disconnect_callback_(tamandua::status st)
{
	if (st == tamandua::status::ok)
	{
		connect_button->Unbind(wxEVT_BUTTON, &main_frame::disconnect, this);
		connect_button->Bind(wxEVT_BUTTON, &main_frame::connect, this);
		connect_button->SetLabel(wxString(wxT("Connect")));
		statusbar->SetStatusText(wxT("Disconnected"));
	}
}

void main_frame::key_page_up()
{
	notebook->next_tab();
}

void main_frame::key_page_down()
{
	notebook->prev_tab();
}

void main_frame::key_up()
{
	msg->SetValue(last_msg_content);
}

void main_frame::show_rlist(wxCommandEvent &event)
{
	set_rlist();
}

void main_frame::show_plist(wxCommandEvent &event)
{
	set_plist();
}

void main_frame::rooms_dbclicked(wxCommandEvent &event)
{
	int sel = rooms_list->GetSelection();
	wxString room = rooms_list->GetString(sel);
	std::stringstream stream;
	stream << "/room \"" << std::string(room.ToUTF8()) << "\"";
	std::string msg = stream.str();
	tb->client.send_message(msg);
}

void main_frame::set_rlist()
{
	rooms_list->Clear();
	for (auto p : tb->client.get_rooms_list())
	{
		rooms_list->Append(wxString::FromUTF8(p.second.data()));
	}
}

void main_frame::set_plist()
{
	participants_list->Clear();
	for (auto p : tb->client.get_participants_list())
	{
		participants_list->Append(wxString::FromUTF8(p.second.data()));
	}
}

void main_frame::context_verified_true_()
{
	Debug("Verification: true");
	statusbar->SetStatusText("Verified certificate!", 1);
}

void main_frame::context_verified_false_()
{
	Debug("Verification: false");
	statusbar->SetStatusText("Not verified certificate!", 1);
}

BEGIN_EVENT_TABLE(main_frame, wxFrame)
	EVT_TEXT_ENTER(MSG_CTRL, main_frame::send_message)
	EVT_BUTTON(CON_BTN, main_frame::connect)
	EVT_MENU(TMD_MENU_RLIST, main_frame::show_rlist)
	EVT_MENU(TMD_MENU_PLIST, main_frame::show_plist)
	EVT_LISTBOX_DCLICK(LISTBOX_ROOMS, main_frame::rooms_dbclicked)
END_EVENT_TABLE()
