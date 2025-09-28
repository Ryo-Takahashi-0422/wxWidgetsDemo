#pragma once
#include <wx/wx.h>
#include <atomic>
#include <thread>
#include <wx/display.h>

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
    wxTextCtrl* textCtrl;
    wxButton* button_uc;
    wxButton* button_lc;
    wxButton* button_start;
    wxButton* button_stop;
    wxListBox* listBox;

    // スレッド制御
    std::atomic<bool> running{ false };
    std::thread timerThread;

    // メソッド
    void StartTimer();
    void StopTimer();

    // イベントバインド
    void OnExit(wxCommandEvent& event);
};
