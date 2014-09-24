#ifndef CHAT_NOTEBOOK_HPP
#define CHAT_NOTEBOOK_HPP
#include "wx/wx.h"
#include "wx/notebook.h"
#include "tamandua_textctrl.hpp"
#include "notification.hpp"
#include "tamandua.hpp"
#include <map>
#include <memory>

class chat_notebook :
	public wxNotebook
{
	private:
		struct tab_elements
		{
			tamandua::id_number_t group_id;
			wxString name;
			int tab_index;
			wxPanel *panel;
			wxBoxSizer *sizer;
			tamandua_textctrl *msgs;

			tab_elements(tamandua::id_number_t gr_id = 0, wxString n = wxEmptyString) :
				group_id(gr_id),
				name(n),
				tab_index(-1),
				panel(nullptr),
				sizer(nullptr),
				msgs(nullptr)
			{}
		};
		std::map<tamandua::id_number_t, tab_elements> tabs_;
		std::map<int, tamandua::id_number_t> groups_ids_;

		wxTextCtrl *msg_input;
		notification *newmsg_notify;

	public:
		chat_notebook(wxWindow *p, wxWindowID id, wxTextCtrl *m) :
			wxNotebook(p, id, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM),
			msg_input(m),
			newmsg_notify(new notification(wxTheApp->GetTopWindow(), wxID_ANY, wxT("New message"), 4500))
		{}

		void init_tabs();
		void new_tab(tamandua::id_number_t, wxString);
		void remove_tab(tamandua::id_number_t);
		tab_elements get_tab(tamandua::id_number_t);
		tamandua::id_number_t get_current_group_id();
		void add_message(std::pair<std::string, tamandua::message>);

		void next_tab();
		void prev_tab();

		void refresh_page_name(wxBookCtrlEvent &);

	private:
		void refresh_ids_();
		void mark_tab_unread_(tab_elements);

		DECLARE_EVENT_TABLE();
};

#endif
