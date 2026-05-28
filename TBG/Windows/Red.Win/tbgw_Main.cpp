//---------------------------------------------------------------------------

#include <conio.h>
#include <vcl.h>
#include "librarian.hpp"
#include "Screen.h"
#include "TBG_Editor.hpp"
#include "MacroSelect_frm.h"
#include "PropDlg1.h"
#include "CorrectForm_unt.h"

#pragma hdrstop
USERES("tbgw.res");
USEFORM("Screen.cpp", Painter);
USEFORM("ToolBar.cpp", SpeedBar);
//---------------------------------------------------------------------------

WINAPI main(int argc, char *argv[], char env[])
{
        try
        {
                 vos.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                 GetVersionEx(&vos);
                 /*
                 char mess[1024];
                 memset(mess, 0, sizeof(mess));
                 sprintf(mess, "Version:\t %d.%d\nBild:\t %d\nPlatform:\t %s\nExtra Info:\t %s",
                         vos.dwMajorVersion,
                         vos.dwMinorVersion,
                         vos.dwBuildNumber,
                         vos.dwPlatformId == VER_PLATFORM_WIN32s ? "Windows 3.x" :
                         vos.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ? "Windows 9x" :
                         vos.dwPlatformId == VER_PLATFORM_WIN32_NT ? "Windows NT" : "...",
                         vos.szCSDVersion);
                 Application->MessageBox(mess, "OS Version Info", MB_OK);
                 */

                 ARGC = argc;
                 ARGV = argv;
                 ENV = env;

                 Application->Initialize();
                 Application->CreateForm(__classid(TSpeedBar), &SpeedBar);
                 Application->CreateForm(__classid(TCorrectForm), &CorrectForm);
                 Application->CreateForm(__classid(TMacroSelect), &MacroSelect);
                 Application->CreateForm(__classid(TPropDlg), &PropDlg);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
