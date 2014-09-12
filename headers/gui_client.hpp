#ifndef GUI_CLIENT_HPP
#define GUI_CLIENT_HPP
#include "wx/wx.h"
#include "main_frame.hpp"

class main_frame;

class gui_client :
	public wxApp
{
	private:
		main_frame *frame;
	public:
		virtual bool OnInit();
		int FilterEvent(wxEvent &);
		int on_key_down(wxKeyEvent &);
		main_frame * GetMainFrame()
		{
			return frame;
		}

};

#endif
