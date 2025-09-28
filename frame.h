//#pragma once
#include "resource.h"
#include <wx/wx.h>

class myFrame : public wxFrame
{
public:
    //基底クラスと同じ引数を持つコンストラクタ（宣言）
    myFrame
    (
        wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr
    );

private:
    wxMenuBar* menu_bar;
    wxMenu* menu_file;

    wxPanel* panel;
    wxTextCtrl* textCtrl;
    wxButton* button_uc;
    wxButton* button_lc;
};
