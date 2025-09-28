#pragma once
#include <wx/wx.h>
#include <atomic>
#include <thread>
#include <wx/display.h>
#include <wx/valnum.h>
#include <wx/choice.h>
#include <wx/filename.h>
#include <wx/event.h>
#include <chrono>
#include <wx/msgdlg.h>
#include <initguid.h>
#include <devguid.h>
#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")

class myFrame : public wxFrame
{
public:
    myFrame(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr);

    ~myFrame();

private:
    // メニュー関連
    wxMenuBar* menu_bar;
    wxMenu* menu_file;

    // ウィジェット
    wxPanel* panel;
    //wxTextCtrl* textCtrl;
    //wxButton* button_uc;
    //wxButton* button_lc;
    wxButton* button_start;
    wxButton* button_stop;
    wxListBox* listBox;
    wxTextCtrl* text_end_min;
    wxTextCtrl* text_end_sec;
    wxTextCtrl* text_interval_min;
    wxTextCtrl* text_interval_sec;
    wxChoice* portChoice;
    wxButton* connectButton;
    wxChoice* baudChoice;
    wxChoice* dataBitsChoice;
    wxChoice* stopBitsChoice;
    wxChoice* parityChoice;


    // スレッド制御
    std::atomic<bool> running{ false };
    std::thread timerThread;

    HANDLE serialHandle = INVALID_HANDLE_VALUE;

    // 新規追加メソッド
    bool GetTimeSettings(int& endSeconds, int& intervalMillis);

    // メソッド
    void StartTimer();
    void StopTimer();

    // イベントバインド
    void OnExit(wxCommandEvent& event);

    void OnSearchPorts(wxCommandEvent& event);
    void OnConnectPort(wxCommandEvent& event);

    // メンバ変数
    std::vector<wxString> EnumerateSerialPorts();
};
