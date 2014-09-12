#include "list_frame.hpp"
#include "wx/longlong.h"

list_frame::list_frame(wxWindow *parent, wxWindowID id, wxString title, const std::map<tamandua::id_number_t, std::string> &d) :
	wxFrame(parent, id, title),
	data(d)
{
	panel = new wxPanel(this);
	sizer = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(sizer);
	list = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	sizer->Add(list, 1, wxALL | wxEXPAND, 10);
	list->InsertColumn(0, wxT("ID"), wxLIST_FORMAT_RIGHT, wxLIST_AUTOSIZE);
	list->InsertColumn(1, wxT("Name"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);

	update_data_();
}

void list_frame::update_data_()
{
	list->DeleteAllItems();
	size_t pos = 1;
	for (auto p : data)
	{
		wxLongLong id(p.first);
		pos = list->InsertItem(pos, id.ToString());
		list->SetItem(pos, 1,  wxString::FromUTF8(p.second.data()));
		++pos;
	}
}
