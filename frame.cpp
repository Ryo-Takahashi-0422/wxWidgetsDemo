#include "Frame.h"

//基底クラスと同じ引数を持つコンストラクタ（定義）
myFrame::myFrame
(
    wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name

) : wxFrame(parent, id, title, pos, size, style, name)//基底クラスへの委譲コンストラクタ
{
    //メニューアイテムに割り当てるIDを列挙型を宣言しておく
    enum ID_MENU
    {
        ID_MENU_EXIT,
    };

    //メニューの構築
    menu_file = new wxMenu;
    menu_file->Append(ID_MENU_EXIT, "終了\tCtrl+Q");

    //メニューバーの構築とメニューバーにメニューをセット
    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "ファイル(&F)");

    //このWindowにメニューバーをセット
    SetMenuBar(menu_bar);

    //メニューの終了を押したらプログラム終了
    Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
            Destroy();
    }, ID_MENU_EXIT);

    //ステータスバー
    CreateStatusBar();
    SetStatusText("ステータスバーはここ");

    //パネル（親widgetはこのthisつまりmyFrame）
    panel = new wxPanel(this, wxID_ANY);

    //テキストコントロール（親widgetはpanel）
    textCtrl = new wxTextCtrl(panel, wxID_ANY);

    //ボタン（親widgetはpanel）
    button_uc = new wxButton(panel, wxID_ANY, "UpperCase");
    button_lc = new wxButton(panel, wxID_ANY, "LowerCase");

    //サイザー（垂直方向に詰める）
    auto vsizer = new wxBoxSizer(wxVERTICAL);

    //サイザー（水平方向に詰める）
    auto hsizer = new wxBoxSizer(wxHORIZONTAL);

    //レイアウトを整える
    panel->SetSizer(vsizer);

    vsizer->Add(textCtrl, 0, wxALIGN_CENTER);
    vsizer->Add(hsizer, 0, wxALIGN_CENTER);
    hsizer->Add(button_uc, 0, wxALIGN_CENTER);
    hsizer->Add(button_lc, 0, wxALIGN_CENTER);

    //UpperCaseボタンを押したら、TextCtrlの文字列をUpperCaseする
    button_uc->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
        {
            //ダイアログ（親widgetはこのthisつまりmyFrame）
            wxMessageDialog dialog(this, "テキストをUpperCaseしますか？", "タイトル", wxCANCEL);

            //ダイアログを表示して、OKボタンを押したらテキストの変換を実施する
            if (dialog.ShowModal() == wxID_OK)
            {
                auto text = textCtrl->GetValue();
                text.UpperCase();
                textCtrl->SetValue(text);
            }
    });

    //LowerCaseボタンを押したら、TextCtrlの文字列をLowerCaseする
    button_lc->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
        {
            //ダイアログ（親widgetはこのthisつまりmyFrame）
            wxMessageDialog dialog(this, "テキストをlowerCaseしますか？", "タイトル", wxCANCEL);

            //ダイアログを表示して、OKボタンを押したらテキストの変換を実施する    
            if (dialog.ShowModal() == wxID_OK)
            {
                auto text = textCtrl->GetValue();
                text.LowerCase();
                textCtrl->SetValue(text);
            }
    });
}