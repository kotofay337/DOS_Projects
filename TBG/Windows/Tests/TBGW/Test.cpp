//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

USEFORM("TestWTBG.cpp", TestForm);
//---------------------------------------------------------------------------

#pragma argsused
int main(int argc, char* argv[])
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TTestForm), &TestForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
 