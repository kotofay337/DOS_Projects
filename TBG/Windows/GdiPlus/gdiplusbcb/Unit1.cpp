//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
int DemoGDIPlus(HDC hDC);

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormPaint(TObject *Sender)
{
  DemoGDIPlus(Canvas->Handle);
  Application->Terminate();
}
//---------------------------------------------------------------------------

