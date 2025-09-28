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
    // ���j���[�֘A
    wxMenuBar* menu_bar;
    wxMenu* menu_file;

    // �E�B�W�F�b�g
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


    // �X���b�h����
    std::atomic<bool> running{ false };
    std::thread timerThread;

    HANDLE serialHandle = INVALID_HANDLE_VALUE;

    // �V�K�ǉ����\�b�h
    bool GetTimeSettings(int& endSeconds, int& intervalMillis);

    // ���\�b�h
    void StartTimer();
    void StopTimer();

    // �C�x���g�o�C���h
    void OnExit(wxCommandEvent& event);

    void OnSearchPorts(wxCommandEvent& event);
    void OnConnectPort(wxCommandEvent& event);

    // �����o�ϐ�
    std::vector<wxString> EnumerateSerialPorts();
};
