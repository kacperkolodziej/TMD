#ifndef MAIN_FRAME_HPP
#define MAIN_FRAME_HPP
#include "wx/wx.h"
#include "wx/textctrl.h"
#include "chat_notebook.hpp"
#include "tamandua_textctrl.hpp"
#include "gui_client.hpp"
#include "tamandua_box.hpp"
#include "list_frame.hpp"
#include <initializer_list>

class main_frame :
	public wxFrame
{
	private:
		wxPanel *panel;
		chat_notebook *notebook;
		wxTextCtrl *msg, *connect_host, *connect_port;
		wxStatusBar *statusbar;
		wxString last_msg_content;
		wxButton *connect_button;
		wxBoxSizer *main_sizer, *connect_sizer, *cols_sizer, *msg_sizer;
		wxGridSizer *info_sizer;
		wxListBox *rooms_list, *participants_list;
		wxMenuBar *menubar;

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
			TMD_MENU_PLIST,
			LISTBOX_ROOMS,
			LISTBOX_PARTICIPANTS
		};

		main_frame();

		void connect(wxCommandEvent &);
		void send_message(wxCommandEvent &);
		void disconnect(wxCommandEvent &);
		void connect_callback_(tamandua::status);
		void disconnect_callback_(tamandua::status);
		void reader_thread_function();
		
		void context_verified_true();
		void context_verified_false();
	
		void key_page_up();
		void key_page_down();
		void key_up();
		void rooms_dbclicked(wxCommandEvent &);
		void set_rlist();
		void set_plist();

	private:
		DECLARE_EVENT_TABLE();

};

#endif
