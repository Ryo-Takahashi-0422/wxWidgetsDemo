#include "Frame.h"


myFrame::myFrame(wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name)
    : wxFrame(parent, id, title, pos, size, style, name)
{
    // ���C���f�B�X�v���C�̃T�C�Y�擾�ƃE�B���h�E�T�C�Y�ݒ�i��ʂ�30%�j
    wxDisplay display;
    wxRect screenSize = display.GetClientArea();
    int w = screenSize.GetWidth() * 0.5;
    int h = screenSize.GetHeight() * 0.6;
    SetSize(wxSize(w, h));
    Centre();

    // ���j���[�쐬
    enum ID_MENU
    {
        ID_MENU_EXIT,
        ID_MENU_SEARCH_PORT
    };
    menu_file = new wxMenu;
    menu_file->Append(ID_MENU_SEARCH_PORT, "�|�[�g����\tCtrl+P");
    menu_file->Append(ID_MENU_EXIT, "�I��\tCtrl+Q");

    Bind(wxEVT_MENU, &myFrame::OnSearchPorts, this, ID_MENU_SEARCH_PORT);
    Bind(wxEVT_MENU, [=](wxCommandEvent&) { Destroy(); }, ID_MENU_EXIT);

    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "�t�@�C��(&F)");
    SetMenuBar(menu_bar);

    CreateStatusBar();
    SetStatusText("�X�e�[�^�X�o�[�͂���");

    // ���C���p�l���Ɗ�{�E�B�W�F�b�g
    panel = new wxPanel(this, wxID_ANY);
    //textCtrl = new wxTextCtrl(panel, wxID_ANY);
    //button_uc = new wxButton(panel, wxID_ANY, "UpperCase");
    //button_lc = new wxButton(panel, wxID_ANY, "LowerCase");
    button_start = new wxButton(panel, wxID_ANY, "Start");
    button_stop = new wxButton(panel, wxID_ANY, "Stop");
    button_stop->Enable(false);
    listBox = new wxListBox(panel, wxID_ANY);

    // �|�[�g�I��p�E�B�W�F�b�g�Ɛڑ��{�^��
    portChoice = new wxChoice(panel, wxID_ANY);
    portChoice->SetMinSize(wxSize(100, -1)); // ��100px�ɐݒ�
    connectButton = new wxButton(panel, wxID_ANY, "�ڑ�");
    connectButton->Enable(false);
    connectButton->Bind(wxEVT_BUTTON, &myFrame::OnConnectPort, this);

    auto hsizer_ports = new wxBoxSizer(wxHORIZONTAL);
    hsizer_ports->Add(new wxStaticText(panel, wxID_ANY, "�|�[�g�I��:"), 0,
        wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    hsizer_ports->Add(portChoice, 0, wxRIGHT, 10);
    hsizer_ports->Add(connectButton, 0);

    // �V���A���ʐM�ݒ�̃R���{�{�b�N�X���쐬
    baudChoice = new wxChoice(panel, wxID_ANY);
    baudChoice->Append("9600");
    baudChoice->Append("19200");
    baudChoice->Append("38400");
    baudChoice->Append("57600");
    baudChoice->Append("115200");
    baudChoice->SetSelection(0); // �f�t�H���g9600

    dataBitsChoice = new wxChoice(panel, wxID_ANY);
    dataBitsChoice->Append("5");
    dataBitsChoice->Append("6");
    dataBitsChoice->Append("7");
    dataBitsChoice->Append("8");
    dataBitsChoice->SetSelection(3); // �f�t�H���g8

    stopBitsChoice = new wxChoice(panel, wxID_ANY);
    stopBitsChoice->Append("1");
    stopBitsChoice->Append("2");
    stopBitsChoice->SetSelection(0); // �f�t�H���g1

    parityChoice = new wxChoice(panel, wxID_ANY);
    parityChoice->Append("None");
    parityChoice->Append("Odd");
    parityChoice->Append("Even");
    parityChoice->SetSelection(0); // �f�t�H���gNone

    // �I�����ԁi���A�b�j�e�L�X�g�{�b�N�X�i���l�̂݋��j
    wxIntegerValidator<unsigned int> validatorInt(nullptr, wxNUM_VAL_ZERO_AS_BLANK);

    auto hsizer_endtime = new wxBoxSizer(wxHORIZONTAL);
    hsizer_endtime->Add(new wxStaticText(panel, wxID_ANY, "�I������ (�� : �b) "), 0,
        wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    text_end_min = new wxTextCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    text_end_sec = new wxTextCtrl(panel, wxID_ANY, "10", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    hsizer_endtime->Add(text_end_min, 0, wxRIGHT, 2);
    hsizer_endtime->Add(new wxStaticText(panel, wxID_ANY, ":"), 0,
        wxALIGN_CENTER_VERTICAL | wxRIGHT, 2);
    hsizer_endtime->Add(text_end_sec, 0);

    // �J�E���g�Ԋu�i���A�b�j�e�L�X�g�{�b�N�X�i���l�̂݋��j
    auto hsizer_interval = new wxBoxSizer(wxHORIZONTAL);
    hsizer_interval->Add(new wxStaticText(panel, wxID_ANY, "�J�E���g�Ԋu (�� : �b) "), 0,
        wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    text_interval_min = new wxTextCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    text_interval_sec = new wxTextCtrl(panel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), 0, validatorInt);
    hsizer_interval->Add(text_interval_min, 0, wxRIGHT, 2);
    hsizer_interval->Add(new wxStaticText(panel, wxID_ANY, ":"), 0,
        wxALIGN_CENTER_VERTICAL | wxRIGHT, 2);
    hsizer_interval->Add(text_interval_sec, 0);

    // �V���A���ݒ�p�̃T�C�U�[�ɂ܂Ƃ߂�
    auto hsizer_serial_config = new wxBoxSizer(wxHORIZONTAL);
    hsizer_serial_config->Add(new wxStaticText(panel, wxID_ANY, "�{�[���[�g:"), 0,
        wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    hsizer_serial_config->Add(baudChoice, 0, wxRIGHT, 10);
    hsizer_serial_config->Add(new wxStaticText(panel, wxID_ANY, "�f�[�^�r�b�g:"), 0,
        wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    hsizer_serial_config->Add(dataBitsChoice, 0, wxRIGHT, 10);
    hsizer_serial_config->Add(new wxStaticText(panel, wxID_ANY, "�X�g�b�v�r�b�g:"), 0,
        wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    hsizer_serial_config->Add(stopBitsChoice, 0, wxRIGHT, 10);
    hsizer_serial_config->Add(new wxStaticText(panel, wxID_ANY, "�p���e�B:"), 0,
        wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    hsizer_serial_config->Add(parityChoice, 0);

    // ���C���̏c�����T�C�U�[
    auto vsizer = new wxBoxSizer(wxVERTICAL);
    auto hsizer1 = new wxBoxSizer(wxHORIZONTAL);
    auto hsizer2 = new wxBoxSizer(wxHORIZONTAL);

    panel->SetSizer(vsizer);

    // �|�[�g�I���Ɛڑ��{�^��
    vsizer->Add(hsizer_ports, 0, wxEXPAND | wxALL, 5);

    // �V���A���ʐM�ݒ�UI
    vsizer->Add(hsizer_serial_config, 0, wxEXPAND | wxALL, 5);

    // �I�����ԂƃJ�E���g�Ԋu���͗�
    vsizer->Add(hsizer_endtime, 0, wxEXPAND | wxALL, 5);
    vsizer->Add(hsizer_interval, 0, wxEXPAND | wxALL, 5);

    // �e�L�X�g�R���g���[��
    //vsizer->Add(textCtrl, 0, wxEXPAND | wxALL, 5);

    //// UpperCase/LowerCase �{�^��
    //hsizer1->Add(button_uc, 0, wxALL, 5);
    //hsizer1->Add(button_lc, 0, wxALL, 5);
    //vsizer->Add(hsizer1, 0, wxALIGN_CENTER);

    // Start/Stop �{�^��
    hsizer2->Add(button_start, 0, wxALL, 5);
    hsizer2->Add(button_stop, 0, wxALL, 5);
    vsizer->Add(hsizer2, 0, wxALIGN_CENTER);

    // ���X�g�{�b�N�X
    vsizer->Add(listBox, 1, wxEXPAND | wxALL, 10);

    //// �{�^���C�x���g�i�ȗ��\�j
    //button_uc->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
    //    wxMessageDialog dialog(this, "�e�L�X�g��UpperCase���܂����H", "�^�C�g��", wxOK | wxCANCEL);
    //    if (dialog.ShowModal() == wxID_OK)
    //    {
    //        auto text = textCtrl->GetValue();
    //        text.UpperCase();
    //        textCtrl->SetValue(text);
    //    }
    //});

    //button_lc->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
    //    wxMessageDialog dialog(this, "�e�L�X�g��LowerCase���܂����H", "�^�C�g��", wxOK | wxCANCEL);
    //    if (dialog.ShowModal() == wxID_OK)
    //    {
    //        auto text = textCtrl->GetValue();
    //        text.LowerCase();
    //        textCtrl->SetValue(text);
    //    }
    //});

    button_start->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { StartTimer(); });
    button_stop->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { StopTimer(); });
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

void myFrame::OnSearchPorts(wxCommandEvent&)
{
    portChoice->Clear();
    auto ports = EnumerateSerialPorts();

    for (const auto& port : ports)
        portChoice->Append(port);

    if (!ports.empty())
    {
        portChoice->SetSelection(0);
        connectButton->Enable(true);
    }
    else
    {
        wxMessageBox("���p�\�ȃV���A���|�[�g��������܂���B", "�|�[�g����", wxICON_INFORMATION);
        connectButton->Enable(false);
    }
}

std::vector<wxString> myFrame::EnumerateSerialPorts()
{
    std::vector<wxString> result;

    HDEVINFO deviceInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_COMPORT, nullptr, nullptr,
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (deviceInfo == INVALID_HANDLE_VALUE)
        return result;

    SP_DEVICE_INTERFACE_DATA devInterfaceData = {};
    devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInterfaces(deviceInfo, nullptr, &GUID_DEVINTERFACE_COMPORT, i, &devInterfaceData); ++i)
    {
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetail(deviceInfo, &devInterfaceData, nullptr, 0, &requiredSize, nullptr);

        std::vector<BYTE> buffer(requiredSize);
        auto* devDetail = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(buffer.data());
        devDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        if (SetupDiGetDeviceInterfaceDetail(deviceInfo, &devInterfaceData, devDetail, requiredSize, nullptr, nullptr))
        {
            wxString devicePath = devDetail->DevicePath;
            // COM�|�[�g���𒊏o
            size_t pos = devicePath.Find("COM");
            if (pos != wxNOT_FOUND)
            {
                wxString comPort;
                while (pos < devicePath.length() && wxIsalnum(devicePath[pos]))
                {
                    comPort += devicePath[pos++];
                }
                result.push_back(comPort);
            }
        }
    }

    SetupDiDestroyDeviceInfoList(deviceInfo);

    // COM�ԍ����ɕ��ёւ��i�Ⴂ���j
    std::sort(result.begin(), result.end(), [](const wxString& a, const wxString& b) {
        long anum = 0, bnum = 0;
        a.Mid(3).ToLong(&anum);
        b.Mid(3).ToLong(&bnum);
        return anum < bnum;
        });

    return result;
}

void myFrame::OnConnectPort(wxCommandEvent&)
{
    if (serialHandle != INVALID_HANDLE_VALUE)
    {
        wxMessageBox("���ɐڑ�����Ă��܂��B");
        return;
    }

    wxString portName = portChoice->GetStringSelection();
    if (portName.IsEmpty())
        return;

    wxString fullName = "\\\\.\\" + portName;

    serialHandle = CreateFile(fullName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0, nullptr,
        OPEN_EXISTING,
        0, nullptr);

    if (serialHandle == INVALID_HANDLE_VALUE)
    {
        wxMessageBox("�ڑ��Ɏ��s���܂����B", "�G���[", wxICON_ERROR);
        return;
    }

    // �ʐM�ݒ�
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(serialHandle, &dcbSerialParams)) {
        wxMessageBox("�ʐM��Ԃ̎擾�Ɏ��s", "�G���[", wxICON_ERROR);
        CloseHandle(serialHandle);
        serialHandle = INVALID_HANDLE_VALUE;
        return;
    }

    dcbSerialParams.BaudRate = wxAtoi(baudChoice->GetStringSelection());
    dcbSerialParams.ByteSize = wxAtoi(dataBitsChoice->GetStringSelection());

    wxString stopBitsStr = stopBitsChoice->GetStringSelection();
    if (stopBitsStr == "1")      dcbSerialParams.StopBits = ONESTOPBIT;
    else if (stopBitsStr == "2") dcbSerialParams.StopBits = TWOSTOPBITS;

    wxString parityStr = parityChoice->GetStringSelection();
    if (parityStr == "None")      dcbSerialParams.Parity = NOPARITY;
    else if (parityStr == "Odd")  dcbSerialParams.Parity = ODDPARITY;
    else if (parityStr == "Even") dcbSerialParams.Parity = EVENPARITY;

    if (!SetCommState(serialHandle, &dcbSerialParams)) {
        wxMessageBox("�ʐM�ݒ�̓K�p�Ɏ��s", "�G���[", wxICON_ERROR);
        CloseHandle(serialHandle);
        serialHandle = INVALID_HANDLE_VALUE;
        return;
    }

    wxMessageBox("�ڑ�����: " + portName, "�V���A���ڑ�", wxICON_INFORMATION);
}
