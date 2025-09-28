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
    // ���j���[�֘A
    wxMenuBar* menu_bar;
    wxMenu* menu_file;

    // �E�B�W�F�b�g
    wxPanel* panel;
    wxTextCtrl* textCtrl;
    wxButton* button_uc;
    wxButton* button_lc;
    wxButton* button_start;
    wxButton* button_stop;
    wxListBox* listBox;

    // �X���b�h����
    std::atomic<bool> running{ false };
    std::thread timerThread;

    // ���\�b�h
    void StartTimer();
    void StopTimer();

    // �C�x���g�o�C���h
    void OnExit(wxCommandEvent& event);
};
