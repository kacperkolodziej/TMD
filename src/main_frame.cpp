#include "main_frame.hpp"
#include "wx/app.h"
#include "wx/mstream.h"
#include "tamandua/tamandua.hpp"
#include "tamandua_box.hpp"
#include "debug_gui.hpp"
#include <string>
#include <functional>

wxDECLARE_APP(gui_client);

main_frame::main_frame() :
	wxFrame(0, wxID_ANY, wxT("Tamandua GUI Client"), wxPoint(100,100), wxSize(400,700)),
	connected(false),
	verified(true)
{
	panel = new wxPanel(this);
	msg = new wxTextCtrl(panel, MSG_CTRL, wxEmptyString, wxPoint(0,0), wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);
	notebook = new chat_notebook(panel, CHAT_NOTEBOOK, msg);
	sizer = new wxBoxSizer(wxVERTICAL);
	connect_sizer = new wxBoxSizer(wxHORIZONTAL);
	connect_host = new wxTextCtrl(panel, CON_HOST_TEXT, wxT("localhost"), wxPoint(0,0), wxDefaultSize, wxTE_PROCESS_ENTER);
	connect_port = new wxTextCtrl(panel, CON_PORT_TEXT, wxT("5000"), wxPoint(0,0), wxDefaultSize, wxTE_PROCESS_ENTER);
	connect_button = new wxButton(panel, CON_BTN, wxT("Connect"));
	info_sizer = new wxGridSizer(2, 5, 5);

	panel->SetSizer(sizer);
	connect_sizer->Add(connect_host,1);
	connect_sizer->Add(connect_port,1);
	connect_sizer->Add(connect_button,1);
	sizer->Add(connect_sizer, 0, wxALL | wxEXPAND, 10);
	sizer->Add(info_sizer, 0, wxBOTTOM | wxLEFT | wxRIGHT | wxEXPAND, 10);
	sizer->Add(notebook, 3, wxBOTTOM | wxLEFT | wxRIGHT | wxEXPAND, 10);
	sizer->Add(msg, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 10);

	conn_lbl = new wxStaticText(panel, wxID_ANY, wxT("Not connected"));
	verify_lbl = new wxStaticText(panel, wxID_ANY, wxEmptyString);
	info_sizer->Add(conn_lbl, 0, wxALL | wxEXPAND, 5);
	info_sizer->Add(verify_lbl, 0, wxALL | wxEXPAND, 5);

	// menu bar
	//menubar = new wxMenuBar;

	// TMD menu
	//wxMenu *tmd_menu = new wxMenu;
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
		conn_lbl->SetLabel(wxT("Connected"));
	} else
	{
		conn_lbl->SetLabel(wxT("Connecting failed!"));
	}
}

void main_frame::disconnect_callback_(tamandua::status st)
{
	if (st == tamandua::status::ok)
	{
		connect_button->Unbind(wxEVT_BUTTON, &main_frame::disconnect, this);
		connect_button->Bind(wxEVT_BUTTON, &main_frame::connect, this);
		connect_button->SetLabel(wxString(wxT("Connect")));
		conn_lbl->SetLabel(wxT("Disconnected"));
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

void main_frame::context_verified_true_()
{
	Debug("Verification: true");
	verify_lbl->SetLabel("Verified certificate!");
}

void main_frame::context_verified_false_()
{
	Debug("Verification: false");
	verify_lbl->SetLabel("Not verified certificate!");
}

BEGIN_EVENT_TABLE(main_frame, wxFrame)
	EVT_TEXT_ENTER(MSG_CTRL, main_frame::send_message)
	EVT_BUTTON(CON_BTN, main_frame::connect)
END_EVENT_TABLE()
