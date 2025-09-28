#include <wx/wx.h>

//wxAppの派生クラスを定義してOnInit()をオーバーライド
class myApp : public wxApp
{
public:
    virtual bool OnInit() override;
};
