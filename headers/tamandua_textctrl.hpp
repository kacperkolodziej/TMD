#ifndef TAMANDUA_TEXTCTRL_HPP
#define TAMANDUA_TEXTCTRL_HPP
#include "wx/wx.h"
#include "wx/richtext/richtextctrl.h"
#include <mutex>

class tamandua_textctrl :
	public wxRichTextCtrl
{
	private:
		std::mutex write_lock_;
	public:
		tamandua_textctrl(wxWindow *win, wxWindowID id) :
			wxRichTextCtrl(win, id, wxEmptyString, wxPoint(0,0), wxDefaultSize, wxRE_MULTILINE | wxRE_READONLY)
		{}

		void add_message(wxString, wxString);
		void add_info(wxString);
		void add_error(wxString);
		void add_warning(wxString);
		void add_private_message(bool, wxString, wxString);
};

#endif
