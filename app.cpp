#include "App.h"
#include "Frame.h"

//�^��main�֐��i�G���g���|�C���g�j�𐶐�����}�N��
wxIMPLEMENT_APP(myApp);

//�������main�֐�
bool myApp::OnInit()
{
    //�u�J�X�^�}�C�Y���ꂽ�v�E�B���h�E�𐶐�
    auto frame = new myFrame(nullptr, wxID_ANY, "�E�B���h�E�̃^�C�g��");

    //�E�B���h�E��\��
    frame->Show();

    //true��wxWidgets�̃��C�����[�v�ցAfalse�Ńv���O�������I��
    return true;
}