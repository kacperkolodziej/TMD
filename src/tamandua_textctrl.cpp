#include "tamandua_textctrl.hpp"

#define Colour(r,g,b) BeginTextColour(wxColour(r,g,b))
#define EndColour() EndTextColour();
#define Nl() Newline();

void tamandua_textctrl::add_message(wxString author, wxString msg)
{
	write_lock_.lock();
	SetInsertionPointEnd();
	Colour(0x0b, 0x6c, 0xa1);
	BeginBold();
	WriteText(author);
	EndBold();
	EndColour();
	WriteText(": ");
	WriteText(msg);
	Nl();
	write_lock_.unlock();
}

void tamandua_textctrl::add_info(wxString info)
{
	write_lock_.lock();
	SetInsertionPointEnd();
	Colour(0x00, 0x99, 0);
	BeginBold();
	WriteText(info);
	EndBold();
	EndColour();
	Nl();
	write_lock_.unlock();
}

void tamandua_textctrl::add_error(wxString error)
{
	write_lock_.lock();
	SetInsertionPointEnd();
	Colour(0xcc, 0, 0);
	BeginItalic();
	WriteText(error);
	EndItalic();
	EndColour();
	Nl();
	write_lock_.unlock();
}

void tamandua_textctrl::add_warning(wxString warning)
{
	write_lock_.lock();
	SetInsertionPointEnd();
	Colour(0xed, 0x7b, 0x00);
	WriteText(warning);
	EndColour();
	Nl();
	write_lock_.unlock();
}

void tamandua_textctrl::add_private_message(bool mymsg, wxString author, wxString pm)
{
	write_lock_.lock();
	SetInsertionPointEnd();
	Colour(0x4e, 0x13, 0x08);
	if (mymsg)
		WriteText(wxT("<-"));
	else
		WriteText(wxT("->"));
	WriteText(author + wxT(": "));
	EndColour();
	WriteText(pm);
	Nl();
	write_lock_.unlock();
}
