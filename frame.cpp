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

    int w = screenSize.GetWidth() * 0.5;  // 横30%
    int h = screenSize.GetHeight() * 0.5; // 縦30%

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

    // 数値のみ入力できるよう整数バリデータを作成
    wxIntegerValidator<unsigned int> validatorInt(nullptr, wxNUM_VAL_ZERO_AS_BLANK);

    // 終了時間入力（分、秒）
    auto hsizer_endtime = new wxBoxSizer(wxHORIZONTAL);
    hsizer_endtime->Add(new wxStaticText(panel, wxID_ANY, "終了時間 (分 : 秒) "), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    text_end_min = new wxTextCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    text_end_sec = new wxTextCtrl(panel, wxID_ANY, "10", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    hsizer_endtime->Add(text_end_min, 0, wxRIGHT, 2);
    hsizer_endtime->Add(new wxStaticText(panel, wxID_ANY, ":"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 2);
    hsizer_endtime->Add(text_end_sec, 0);

    // カウント間隔入力（分、秒）
    auto hsizer_interval = new wxBoxSizer(wxHORIZONTAL);
    hsizer_interval->Add(new wxStaticText(panel, wxID_ANY, "カウント間隔 (分 : 秒) "), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    text_interval_min = new wxTextCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    text_interval_sec = new wxTextCtrl(panel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    hsizer_interval->Add(text_interval_min, 0, wxRIGHT, 2);
    hsizer_interval->Add(new wxStaticText(panel, wxID_ANY, ":"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 2);
    hsizer_interval->Add(text_interval_sec, 0);

    // これらをメインのvsizerに追加
    vsizer->Add(hsizer_endtime, 0, wxALIGN_CENTER | wxALL, 5);
    vsizer->Add(hsizer_interval, 0, wxALIGN_CENTER | wxALL, 5);

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
    if (running.load()) return;

    int endSeconds = 0;
    int intervalMillis = 1000;

    if (!GetTimeSettings(endSeconds, intervalMillis))
    {
        wxMessageBox("終了時間とカウント間隔を正しく入力してください。", "入力エラー", wxICON_ERROR);
        return;
    }

    listBox->Clear();

    running = true;
    button_start->Enable(false);
    button_stop->Enable(true);

    timerThread = std::thread([this, endSeconds, intervalMillis]() {
        using clock = std::chrono::steady_clock;
        auto start = clock::now();
        auto endTime = start + std::chrono::seconds(endSeconds);

        int count = 0;
        listBox->Append("計測開始");
        while (running.load())
        {
            auto next = start + std::chrono::milliseconds(intervalMillis * (count + 1));
            // 次の時刻が終了時刻を超えるなら終了時刻を設定
            if (next > endTime)
            {
                next = endTime;
            }

            // 次の時刻まで待つ
            std::this_thread::sleep_until(next);
            if (!running.load()) break;

            auto now = clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
            int elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
            if (elapsed >= endSeconds)
            {
                wxTheApp->CallAfter([this, elapsed_ms]() {
                    int sec = elapsed_ms / 1000;
                    int ms = elapsed_ms % 1000;
                    listBox->Append(wxString::Format("Elapsed: %d.%03d seconds", sec, ms));
                    listBox->Append("計測終了");
                    StopTimer();  // メインスレッドの停止処理呼び出しは安全な呼び出しにする
                    });
                break;
            }            

            wxTheApp->CallAfter([this, elapsed_ms]() {
                int sec = elapsed_ms / 1000;
                int ms = elapsed_ms % 1000;
                listBox->Append(wxString::Format("Elapsed: %d.%03d seconds", sec, ms));
                listBox->EnsureVisible(listBox->GetCount() - 1);
            });

            ++count;
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

bool myFrame::GetTimeSettings(int& endSeconds, int& intervalMillis)
{
    long endMin = 0, endSec = 0, intMin = 0, intSec = 0;

    if (!text_end_min->GetValue().ToLong(&endMin)) return false;
    if (!text_end_sec->GetValue().ToLong(&endSec)) return false;
    if (!text_interval_min->GetValue().ToLong(&intMin)) return false;
    if (!text_interval_sec->GetValue().ToLong(&intSec)) return false;

    if (endMin < 0 || endSec < 0 || intMin < 0 || intSec < 0) return false;
    if (endSec >= 60 || intSec >= 60) return false; // 秒は60未満に制限

    endSeconds = static_cast<int>(endMin * 60 + endSec);
    intervalMillis = static_cast<int>((intMin * 60 + intSec) * 1000);

    if (intervalMillis <= 0) intervalMillis = 1000; // デフォルト1秒

    return endSeconds > 0;
}