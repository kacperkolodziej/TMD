#include "tamandua_textctrl.hpp"

#define Colour(r,g,b) BeginTextColour(wxColour(r,g,b))
#define EndColour() EndTextColour();
#define Nl() Newline();

void tamandua_textctrl::add_message(wxString author, wxString msg)
{
	write_lock_.lock();
	Colour(0x0b, 0x6c, 0xa1);
	BeginBold();
	AppendText(author);
	EndBold();
	EndColour();
	AppendText(": ");
	AppendText(msg);
	Nl();
	write_lock_.unlock();
}

void tamandua_textctrl::add_info(wxString info)
{
	write_lock_.lock();
	Colour(0x00, 0x99, 0);
	BeginBold();
	AppendText(info);
	EndBold();
	EndColour();
	Nl();
	write_lock_.unlock();
}

void tamandua_textctrl::add_error(wxString error)
{
	write_lock_.lock();
	Colour(0xcc, 0, 0);
	BeginItalic();
	AppendText(error);
	EndItalic();
	EndColour();
	Nl();
	write_lock_.unlock();
}

void tamandua_textctrl::add_warning(wxString warning)
{
	write_lock_.lock();
	Colour(0xed, 0x7b, 0x00);
	AppendText(warning);
	EndColour();
	Nl();
	write_lock_.unlock();
}

void tamandua_textctrl::add_private_message(bool mymsg, wxString author, wxString pm)
{
	write_lock_.lock();
	Colour(0x4e, 0x13, 0x08);
	if (mymsg)
		AppendText(wxT("<-"));
	else
		AppendText(wxT("->"));
	AppendText(author + wxT(": "));
	EndColour();
	AppendText(pm);
	Nl();
	write_lock_.unlock();
}
