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
    // ���C���f�B�X�v���C�̃T�C�Y���擾
    wxDisplay display;
    wxRect screenSize = display.GetClientArea();

    int w = screenSize.GetWidth() * 0.5;  // ��30%
    int h = screenSize.GetHeight() * 0.5; // �c30%

    // �E�B���h�E�T�C�Y���Z�b�g�i�ʒu�͉�ʒ����Ɂj
    SetSize(wxSize(w, h));
    Centre();

    // ���j���[
    enum ID_MENU { ID_MENU_EXIT };
    menu_file = new wxMenu;
    menu_file->Append(ID_MENU_EXIT, "�I��\tCtrl+Q");

    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "�t�@�C��(&F)");
    SetMenuBar(menu_bar);

    Bind(wxEVT_MENU, &myFrame::OnExit, this, ID_MENU_EXIT);

    CreateStatusBar();
    SetStatusText("�X�e�[�^�X�o�[�͂���");

    // �p�l���ƃE�B�W�F�b�g
    panel = new wxPanel(this, wxID_ANY);
    textCtrl = new wxTextCtrl(panel, wxID_ANY);
    button_uc = new wxButton(panel, wxID_ANY, "UpperCase");
    button_lc = new wxButton(panel, wxID_ANY, "LowerCase");
    button_start = new wxButton(panel, wxID_ANY, "Start");
    button_stop = new wxButton(panel, wxID_ANY, "Stop");
    button_stop->Enable(false); // �ŏ��͒�~�{�^������
    listBox = new wxListBox(panel, wxID_ANY);

    // �T�C�U�[
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

    // ���l�̂ݓ��͂ł���悤�����o���f�[�^���쐬
    wxIntegerValidator<unsigned int> validatorInt(nullptr, wxNUM_VAL_ZERO_AS_BLANK);

    // �I�����ԓ��́i���A�b�j
    auto hsizer_endtime = new wxBoxSizer(wxHORIZONTAL);
    hsizer_endtime->Add(new wxStaticText(panel, wxID_ANY, "�I������ (�� : �b) "), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    text_end_min = new wxTextCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    text_end_sec = new wxTextCtrl(panel, wxID_ANY, "10", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    hsizer_endtime->Add(text_end_min, 0, wxRIGHT, 2);
    hsizer_endtime->Add(new wxStaticText(panel, wxID_ANY, ":"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 2);
    hsizer_endtime->Add(text_end_sec, 0);

    // �J�E���g�Ԋu���́i���A�b�j
    auto hsizer_interval = new wxBoxSizer(wxHORIZONTAL);
    hsizer_interval->Add(new wxStaticText(panel, wxID_ANY, "�J�E���g�Ԋu (�� : �b) "), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    text_interval_min = new wxTextCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    text_interval_sec = new wxTextCtrl(panel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    hsizer_interval->Add(text_interval_min, 0, wxRIGHT, 2);
    hsizer_interval->Add(new wxStaticText(panel, wxID_ANY, ":"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 2);
    hsizer_interval->Add(text_interval_sec, 0);

    // ���������C����vsizer�ɒǉ�
    vsizer->Add(hsizer_endtime, 0, wxALIGN_CENTER | wxALL, 5);
    vsizer->Add(hsizer_interval, 0, wxALIGN_CENTER | wxALL, 5);

    // �{�^���C�x���g�o�C���h
    button_uc->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        wxMessageDialog dialog(this, "�e�L�X�g��UpperCase���܂����H", "�^�C�g��", wxOK | wxCANCEL);
        if (dialog.ShowModal() == wxID_OK)
        {
            auto text = textCtrl->GetValue();
            text.UpperCase();
            textCtrl->SetValue(text);
        }
        });

    button_lc->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        wxMessageDialog dialog(this, "�e�L�X�g��LowerCase���܂����H", "�^�C�g��", wxOK | wxCANCEL);
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
        wxMessageBox("�I�����ԂƃJ�E���g�Ԋu�𐳂������͂��Ă��������B", "���̓G���[", wxICON_ERROR);
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
        listBox->Append("�v���J�n");
        while (running.load())
        {
            auto next = start + std::chrono::milliseconds(intervalMillis * (count + 1));
            // ���̎������I�������𒴂���Ȃ�I��������ݒ�
            if (next > endTime)
            {
                next = endTime;
            }

            // ���̎����܂ő҂�
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
                    listBox->Append("�v���I��");
                    StopTimer();  // ���C���X���b�h�̒�~�����Ăяo���͈��S�ȌĂяo���ɂ���
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
    if (endSec >= 60 || intSec >= 60) return false; // �b��60�����ɐ���

    endSeconds = static_cast<int>(endMin * 60 + endSec);
    intervalMillis = static_cast<int>((intMin * 60 + intSec) * 1000);

    if (intervalMillis <= 0) intervalMillis = 1000; // �f�t�H���g1�b

    return endSeconds > 0;
}