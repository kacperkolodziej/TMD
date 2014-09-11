#include "chat_notebook.hpp"
#include "debug_gui.hpp"
#include <utility>

void chat_notebook::new_group(tamandua::id_number_t group_id, wxString title)
{
	tab_elements tab;
	tab.panel = new wxPanel(this);
	tab.sizer = new wxBoxSizer(wxVERTICAL);
	tab.panel->SetSizer(tab.sizer);
	tab.msgs = new tamandua_textctrl(this, wxID_ANY);
	tab.sizer->Add(tab.msgs, 1, wxALL | wxEXPAND, 5);
	if (AddPage(tab.panel, title, true))
	{
		int sel = GetSelection();
		if (sel != wxNOT_FOUND)
		{
			tab.tab_index = sel;
			tabs_.insert(std::make_pair(group_id, tab));
		}
	} else
	{
		wxMessageBox(wxT("Couldn't add new tab!"));
	}
}

void chat_notebook::remove_group(tamandua::id_number_t group_id)
{
	auto it = tabs_.find(group_id);
	if (it == tabs_.end())
	{
		Debug("Couldn't find group with id: ", group_id);
		return;
	}

	tabs_.erase(it);
}

chat_notebook::tab_elements chat_notebook::get_group(tamandua::id_number_t group_id)
{
	auto it = tabs_.find(group_id);
	if (it == tabs_.end())
		return tab_elements();

	return (*it).second;
}

void chat_notebook::add_message(std::pair<std::string, tamandua::message> msg_pair)
{
	std::string &author = msg_pair.first;
	tamandua::message &msg = msg_pair.second;
	Debug("Received message of type: ", msg.header.type, ". Content: ", msg.body);

	tab_elements tab = get_group(msg.header.group);

	switch (msg.header.type)
	{
		case tamandua::message_type::standard_message:
			//tab.msgs->add_message(wxString::FromUTF8(author.data()), wxString::FromUTF8(msg.body.data()));
			break;

		case tamandua::message_type::group_enter_message:
			new_group(msg.header.group, msg.body);
			break;

		default:
			break;
	}
}
