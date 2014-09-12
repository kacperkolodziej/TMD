#include <thread>
#include <boost/asio.hpp>
#include "gui_client.hpp"
#include "main_frame.hpp"
#include "tamandua/tamandua.hpp"
#include "debug_gui.hpp"
#include <functional>
#include <chrono>

bool gui_client::OnInit()
{
	frame = new main_frame();
	frame->Show();
	return true;
}

int gui_client::FilterEvent(wxEvent &evt)
{
	if (evt.GetEventType() == wxEVT_KEY_DOWN)
	{
		return on_key_down(static_cast<wxKeyEvent&>(evt));
	}

	return -1;
}

int gui_client::on_key_down(wxKeyEvent &evt)
{
	if (evt.GetKeyCode() == WXK_PAGEUP)
		frame->key_page_up();

	else if (evt.GetKeyCode() == WXK_PAGEDOWN)
		frame->key_page_down();

	else if (evt.GetKeyCode() == WXK_UP)
		frame->key_up();
	
	else
		return -1;

	return 1;
}
