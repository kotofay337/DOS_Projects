//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("tbgw.res");
USEFORM("Screen.cpp", Painter);
USEFORM("ToolBar.cpp", SpeedBar);
USEUNIT("tbg_001.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TPainter), &Painter);
                 Application->CreateForm(__classid(TSpeedBar), &SpeedBar);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
