//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TestOLE.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

AnsiString Fname;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MNewClick(TObject *Sender)
{
   if(OleContainer1->InsertObjectDialog())
      Fname = "";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MCloseClick(TObject *Sender)
{
   OleContainer1->DestroyObject();   
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MSaveClick(TObject *Sender)
{
   if(Fname == ""){
      if(SaveDialog1->Execute()){
         Fname = SaveDialog1->FileName;   
      }
      else{
         return;
      }
   }
   OleContainer1->SaveToFile(ChangeFileExt(Fname, ".ole"));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MOpenClick(TObject *Sender)
{
  if(OpenDialog1->Execute()){
     OleContainer1->LoadFromFile(OpenDialog1->FileName);
     Fname = OpenDialog1->FileName;
     OleContainer1->Repaint();
  }
}
//---------------------------------------------------------------------------
