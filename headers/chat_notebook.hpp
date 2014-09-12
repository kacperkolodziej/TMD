#ifndef CHAT_NOTEBOOK_HPP
#define CHAT_NOTEBOOK_HPP
#include "wx/wx.h"
#include "wx/notebook.h"
#include "tamandua_textctrl.hpp"
#include "tamandua.hpp"
#include <map>
#include <memory>

class chat_notebook :
	public wxNotebook
{
	private:
		struct tab_elements
		{
			int tab_index;
			wxPanel *panel;
			wxBoxSizer *sizer;
			tamandua_textctrl *msgs;

			tab_elements() :
				tab_index(-1),
				panel(nullptr),
				sizer(nullptr),
				msgs(nullptr)
			{}
		};
		std::map<tamandua::id_number_t, tab_elements> tabs_;
		std::map<int, tamandua::id_number_t> groups_ids_;

	public:
		chat_notebook(wxWindow *p, wxWindowID id) :
			wxNotebook(p, id, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM)
		{}

		void new_group(tamandua::id_number_t, wxString);
		void remove_group(tamandua::id_number_t);
		tab_elements get_group(tamandua::id_number_t);
		tamandua::id_number_t get_current_group_id();
		void add_message(std::pair<std::string, tamandua::message>);

		void next_page();
		void prev_page();
};

#endif
