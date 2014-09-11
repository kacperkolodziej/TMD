#ifndef MAIN_FRAME_HPP
#define MAIN_FRAME_HPP
#include "wx/wx.h"
#include "wx/textctrl.h"
#include "chat_notebook.hpp"
#include "tamandua_textctrl.hpp"
#include "gui_client.hpp"
#include "tamandua_box.hpp"
#include <initializer_list>

class main_frame :
	public wxFrame
{
	private:
		wxPanel *panel;
		chat_notebook *notebook;
		wxTextCtrl *msg, *connect_host, *connect_port;
		wxButton *connect_button;
		wxBoxSizer *sizer;
		wxBoxSizer *connect_sizer;
		wxGridSizer *info_sizer;
		//wxMenuBar *menubar;
		wxStaticText *conn_lbl, *verify_lbl;

		bool connected;
		bool verified;
		tamandua_box *tb;
	public:
		enum {
			CHAT_NOTEBOOK = 1000,
			MSG_CTRL,
			PANEL,
			CON_HOST_TEXT,
			CON_PORT_TEXT,
			CON_BTN,
			FUNNY_BTN,
			TMD_MENU_RLIST,
			TMD_MENU_PLIST
		};

		main_frame();

		void connect(wxCommandEvent &);
		void send_message(wxCommandEvent &);
		void disconnect(wxCommandEvent &);
		void msgs_url(wxTextUrlEvent &);

		void connect_callback_(tamandua::status);
		void disconnect_callback_(tamandua::status);
		void message_received_callback_(tamandua::status);

	private:
		DECLARE_EVENT_TABLE();

		void message_sent_();
		void message_undelivered_();
		void context_verified_true_();
		void context_verified_false_();

};

#endif
