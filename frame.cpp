#include "Frame.h"
#include <chrono>
#include <wx/msgdlg.h>

//#ifdef _WIN32
//#include <windows.h>
//#endif

myFrame::myFrame(wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name)
    : wxFrame(parent, id, title, pos, size, style, name)
{
    // メインディスプレイのサイズを取得
    wxDisplay display;
    wxRect screenSize = display.GetClientArea();

    int w = screenSize.GetWidth() * 0.3;  // 横30%
    int h = screenSize.GetHeight() * 0.3; // 縦30%

    // ウィンドウサイズをセット（位置は画面中央に）
    SetSize(wxSize(w, h));
    Centre();

    // メニュー
    enum ID_MENU { ID_MENU_EXIT };
    menu_file = new wxMenu;
    menu_file->Append(ID_MENU_EXIT, "終了\tCtrl+Q");

    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "ファイル(&F)");
    SetMenuBar(menu_bar);

    Bind(wxEVT_MENU, &myFrame::OnExit, this, ID_MENU_EXIT);

    CreateStatusBar();
    SetStatusText("ステータスバーはここ");

    // パネルとウィジェット
    panel = new wxPanel(this, wxID_ANY);
    textCtrl = new wxTextCtrl(panel, wxID_ANY);
    button_uc = new wxButton(panel, wxID_ANY, "UpperCase");
    button_lc = new wxButton(panel, wxID_ANY, "LowerCase");
    button_start = new wxButton(panel, wxID_ANY, "Start");
    button_stop = new wxButton(panel, wxID_ANY, "Stop");
    button_stop->Enable(false); // 最初は停止ボタン無効
    listBox = new wxListBox(panel, wxID_ANY);

    // サイザー
    auto vsizer = new wxBoxSizer(wxVERTICAL);
    auto hsizer1 = new wxBoxSizer(wxHORIZONTAL); // UpperCase, LowerCase
    auto hsizer2 = new wxBoxSizer(wxHORIZONTAL); // Start, Stop

    panel->SetSizer(vsizer);
    vsizer->Add(textCtrl, 0, wxALIGN_CENTER | wxALL, 5);
    vsizer->Add(hsizer1, 0, wxALIGN_CENTER | wxALL, 5);
    vsizer->Add(hsizer2, 0, wxALIGN_CENTER | wxALL, 5);
    vsizer->Add(listBox, 1, wxEXPAND | wxALL, 10);

    hsizer1->Add(button_uc, 0, wxALIGN_CENTER | wxALL, 5);
    hsizer1->Add(button_lc, 0, wxALIGN_CENTER | wxALL, 5);

    hsizer2->Add(button_start, 0, wxALIGN_CENTER | wxALL, 5);
    hsizer2->Add(button_stop, 0, wxALIGN_CENTER | wxALL, 5);

    // ボタンイベントバインド
    button_uc->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        wxMessageDialog dialog(this, "テキストをUpperCaseしますか？", "タイトル", wxOK | wxCANCEL);
        if (dialog.ShowModal() == wxID_OK)
        {
            auto text = textCtrl->GetValue();
            text.UpperCase();
            textCtrl->SetValue(text);
        }
        });

    button_lc->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        wxMessageDialog dialog(this, "テキストをLowerCaseしますか？", "タイトル", wxOK | wxCANCEL);
        if (dialog.ShowModal() == wxID_OK)
        {
            auto text = textCtrl->GetValue();
            text.LowerCase();
            textCtrl->SetValue(text);
        }
        });

    button_start->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        StartTimer();
        });

    button_stop->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        StopTimer();
        });
}

myFrame::~myFrame()
{
    StopTimer();
}

void myFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void myFrame::StartTimer()
{
    if (running.load()) return; // 既に動作中

    listBox->Clear();

    running = true;
    button_start->Enable(false);
    button_stop->Enable(true);

    timerThread = std::thread([this]() {
        using clock = std::chrono::steady_clock;
        auto start = clock::now();

        int i = 0;
        while (running.load())
        {
            auto next = start + std::chrono::milliseconds(1000 * (i + 1)); // 1秒刻み
            std::this_thread::sleep_until(next);

            if (!running.load())
                break;

            auto now = clock::now();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

            int sec = elapsed_ms / 1000;
            int ms = elapsed_ms % 1000;

            wxTheApp->CallAfter([this, sec, ms]() {
                listBox->Append(wxString::Format("Elapsed: %d.%03d seconds", sec, ms));
                listBox->EnsureVisible(listBox->GetCount() - 1);
                });

            i++;
        }
        });
}

void myFrame::StopTimer()
{
    if (!running.load()) return;

    running = false;
    button_start->Enable(true);
    button_stop->Enable(false);

    if (timerThread.joinable())
        timerThread.join();
}
