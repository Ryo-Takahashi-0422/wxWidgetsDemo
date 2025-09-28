#include "App.h"
#include "Frame.h"

//真のmain関数（エントリポイント）を生成するマクロ
wxIMPLEMENT_APP(myApp);

//事実上のmain関数
bool myApp::OnInit()
{
    //「カスタマイズされた」ウィンドウを生成
    auto frame = new myFrame(nullptr, wxID_ANY, "ウィンドウのタイトル");

    //ウィンドウを表示
    frame->Show();

    //trueでwxWidgetsのメインループへ、falseでプログラム即終了
    return true;
}