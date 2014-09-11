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
		};
		std::map<tamandua::id_number_t, tab_elements> tabs_;

	public:
		chat_notebook(wxWindow *p, wxWindowID id) :
			wxNotebook(p, id, wxDefaultPosition, wxDefaultSize, wxNB_LEFT)
		{}

		void new_group(tamandua::id_number_t, wxString);
		void remove_group(tamandua::id_number_t);
		tab_elements get_group(tamandua::id_number_t);
		void add_message(std::pair<std::string, tamandua::message>);


};

#endif
