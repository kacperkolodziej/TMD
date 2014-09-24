#include "notification.hpp"
#include "debug_gui.hpp"

notification::notification(wxWindow *par, wxWindowID id, wxString title, int time) :
	wxMiniFrame(NULL, id, title, wxDefaultPosition, wxSize(300,100), wxSIMPLE_BORDER | wxFRAME_NO_TASKBAR),
	parent(par),
	display_width(0),
	display_height(0),
	x(0),
	y(0),
	timeout(time),
	timer(this)
{
	panel = new wxPanel(this);
	sizer = new wxBoxSizer(wxHORIZONTAL);
	info = new wxStaticText(panel, wxID_ANY, wxEmptyString, wxPoint(10,30), wxSize(280,80), wxALIGN_CENTRE_HORIZONTAL);
	sizer->Add(info, 1, wxALL | wxEXPAND, 20);
	panel->SetSizer(sizer);
	calc_position_();
}

bool notification::Show(wxString text)
{
	info->SetLabel(text);
	timer.StartOnce(timeout);
	return wxMiniFrame::Show(true);
}

void notification::timer_end(wxTimerEvent &event)
{
	wxMiniFrame::Show(false);
}

void notification::left_down(wxMouseEvent &event)
{
	wxMiniFrame::Show(false);
}

void notification::calc_position_()
{
	wxDisplaySize(&display_width, &display_height);
	wxSize s = GetSize();
	x = display_width - s.GetWidth();
	y = display_height - s.GetHeight();
	SetPosition(wxPoint(x, y));
}

BEGIN_EVENT_TABLE(notification, wxMiniFrame)
	EVT_TIMER(wxID_ANY, notification::timer_end)
	EVT_LEFT_DOWN(notification::left_down)
END_EVENT_TABLE()
