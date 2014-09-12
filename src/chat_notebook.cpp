#include "chat_notebook.hpp"
#include "debug_gui.hpp"
#include <utility>

void chat_notebook::init_tabs()
{
	DeleteAllPages();
}

void chat_notebook::new_tab(tamandua::id_number_t group_id, wxString title)
{
	tab_elements tab(group_id);
	tab.panel = new wxPanel(this);
	tab.sizer = new wxBoxSizer(wxVERTICAL);
	tab.panel->SetSizer(tab.sizer);
	tab.msgs = new tamandua_textctrl(tab.panel, wxID_ANY);
	tab.sizer->Add(tab.msgs, 1, wxALL | wxEXPAND, 5);
	if (AddPage(tab.panel, title, true))
	{
		int sel = GetSelection();
		if (sel != wxNOT_FOUND)
		{
			tab.tab_index = sel;
			tabs_.insert(std::make_pair(group_id, tab));
			groups_ids_.insert(std::make_pair(sel, group_id));
			msg_input->SetFocus();
		}
	} else
	{
		wxMessageBox(wxT("Couldn't add new tab!"));
	}
}

void chat_notebook::remove_tab(tamandua::id_number_t group_id)
{
	auto it = tabs_.find(group_id);
	if (it == tabs_.end())
	{
		Debug("Couldn't find group with id: ", group_id);
		return;
	}
	DeletePage((*it).second.tab_index);	
	tabs_.erase(it);
	refresh_ids_();
}

chat_notebook::tab_elements chat_notebook::get_tab(tamandua::id_number_t group_id)
{
	auto it = tabs_.find(group_id);
	if (it == tabs_.end())
		return tab_elements();

	return (*it).second;
}

tamandua::id_number_t chat_notebook::get_current_group_id()
{
	int sel = GetSelection();
	auto it = groups_ids_.find(sel);
	return (*it).second;
}

void chat_notebook::add_message(std::pair<std::string, tamandua::message> msg_pair)
{
	std::string &author = msg_pair.first;
	tamandua::message &msg = msg_pair.second;
	Debug("Received message of type: ", msg.header.type, ". Content: ", msg.body);

	tab_elements tab = get_tab(msg.header.group);

	switch (msg.header.type)
	{
		case tamandua::message_type::standard_message:
			tab.msgs->add_message(wxString::FromUTF8(author.data()), wxString::FromUTF8(msg.body.data()));
			break;

		case tamandua::message_type::error_message:
			tab.msgs->add_error(wxString::FromUTF8(msg.body.data()));
			break;

		case tamandua::message_type::info_message:
			tab.msgs->add_info(wxString::FromUTF8(msg.body.data()));
			break;

		case tamandua::message_type::warning_message:
			tab.msgs->add_warning(wxString::FromUTF8(msg.body.data()));
			break;

		case tamandua::message_type::group_enter_message:
			new_tab(msg.header.group, msg.body);
			break;

		default:
			break;
	}
}

void chat_notebook::next_tab()
{
	int selection = GetSelection();
	size_t num = GetPageCount();
	SetSelection((selection + 1) % num);
	msg_input->SetFocus();
}

void chat_notebook::prev_tab()
{
	int selection = GetSelection();
	size_t num = GetPageCount();
	SetSelection((selection == 0) ? num-1 : (selection - 1) % num);
	msg_input->SetFocus();
}

void chat_notebook::refresh_ids_()
{
	for (auto t : tabs_)
	{
		t.second.tab_index = FindPage(t.second.panel);
	}
	groups_ids_.clear();
	for (auto p : tabs_)
	{
		groups_ids_.insert(std::make_pair(p.second.tab_index, p.second.group_id));
	}
}
