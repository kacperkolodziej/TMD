#ifndef LIST_FRAME_HPP
#define LIST_FRAME_HPP
#include "wx/wx.h"
#include "wx/listctrl.h"
#include "tamandua.hpp"
#include <map>
#include <string>

class list_frame :
	public wxFrame
{
	private:
		const std::map<tamandua::id_number_t, std::string> &data;
		wxPanel *panel;
		wxBoxSizer *sizer;
		wxListCtrl *list;
		wxButton *close_btn;

	public:
		list_frame(wxWindow *, wxWindowID, wxString, const std::map<tamandua::id_number_t, std::string> &);

	private:
		void update_data_();
};

#endif
