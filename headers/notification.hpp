#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP
#include "wx/wx.h"
#include "wx/minifram.h"
#include "wx/timer.h"

class notification :
	public wxMiniFrame
{
	private:
		wxWindow *parent;
		int display_width, display_height;
		int x, y;
		int timeout;
		wxTimer timer;
		wxStaticText *info;
		wxSizer *sizer;
		wxPanel *panel;

	public:
		notification(wxWindow *, wxWindowID, wxString, int);
		bool Show(wxString);
		void timer_end(wxTimerEvent&);
		void left_down(wxMouseEvent&);
	private:
		void calc_position_();
		DECLARE_EVENT_TABLE();
};

#endif
