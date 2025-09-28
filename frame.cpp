#include "Frame.h"

//���N���X�Ɠ������������R���X�g���N�^�i��`�j
myFrame::myFrame
(
    wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name

) : wxFrame(parent, id, title, pos, size, style, name)//���N���X�ւ̈Ϗ��R���X�g���N�^
{
    //���j���[�A�C�e���Ɋ��蓖�Ă�ID��񋓌^��錾���Ă���
    enum ID_MENU
    {
        ID_MENU_EXIT,
    };

    //���j���[�̍\�z
    menu_file = new wxMenu;
    menu_file->Append(ID_MENU_EXIT, "�I��\tCtrl+Q");

    //���j���[�o�[�̍\�z�ƃ��j���[�o�[�Ƀ��j���[���Z�b�g
    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "�t�@�C��(&F)");

    //����Window�Ƀ��j���[�o�[���Z�b�g
    SetMenuBar(menu_bar);

    //���j���[�̏I������������v���O�����I��
    Bind(wxEVT_MENU, [=](wxCommandEvent& event)
    {
            Destroy();
    }, ID_MENU_EXIT);

    //�X�e�[�^�X�o�[
    CreateStatusBar();
    SetStatusText("�X�e�[�^�X�o�[�͂���");

    //�p�l���i�ewidget�͂���this�܂�myFrame�j
    panel = new wxPanel(this, wxID_ANY);

    //�e�L�X�g�R���g���[���i�ewidget��panel�j
    textCtrl = new wxTextCtrl(panel, wxID_ANY);

    //�{�^���i�ewidget��panel�j
    button_uc = new wxButton(panel, wxID_ANY, "UpperCase");
    button_lc = new wxButton(panel, wxID_ANY, "LowerCase");

    //�T�C�U�[�i���������ɋl�߂�j
    auto vsizer = new wxBoxSizer(wxVERTICAL);

    //�T�C�U�[�i���������ɋl�߂�j
    auto hsizer = new wxBoxSizer(wxHORIZONTAL);

    //���C�A�E�g�𐮂���
    panel->SetSizer(vsizer);

    vsizer->Add(textCtrl, 0, wxALIGN_CENTER);
    vsizer->Add(hsizer, 0, wxALIGN_CENTER);
    hsizer->Add(button_uc, 0, wxALIGN_CENTER);
    hsizer->Add(button_lc, 0, wxALIGN_CENTER);

    //UpperCase�{�^������������ATextCtrl�̕������UpperCase����
    button_uc->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
        {
            //�_�C�A���O�i�ewidget�͂���this�܂�myFrame�j
            wxMessageDialog dialog(this, "�e�L�X�g��UpperCase���܂����H", "�^�C�g��", wxCANCEL);

            //�_�C�A���O��\�����āAOK�{�^������������e�L�X�g�̕ϊ������{����
            if (dialog.ShowModal() == wxID_OK)
            {
                auto text = textCtrl->GetValue();
                text.UpperCase();
                textCtrl->SetValue(text);
            }
    });

    //LowerCase�{�^������������ATextCtrl�̕������LowerCase����
    button_lc->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event)
        {
            //�_�C�A���O�i�ewidget�͂���this�܂�myFrame�j
            wxMessageDialog dialog(this, "�e�L�X�g��lowerCase���܂����H", "�^�C�g��", wxCANCEL);

            //�_�C�A���O��\�����āAOK�{�^������������e�L�X�g�̕ϊ������{����    
            if (dialog.ShowModal() == wxID_OK)
            {
                auto text = textCtrl->GetValue();
                text.LowerCase();
                textCtrl->SetValue(text);
            }
    });
}