//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TestWTBG.h"

#include "tbg.hpp"
#include "tbg_tc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTestForm *TestForm;
//OBJECT **pic = NULL;
//---------------------------------------------------------------------------
__fastcall TTestForm::TTestForm(TComponent* Owner)
        : TForm(Owner)
{
   PictureTBG = NULL;

   char *TBGPath = getenv("TBGRESOURCESPATH");
   if(TBGPath == NULL)
      TBGPath = "C:\\TBG\\BIN\\";
   SetPathToBGIFont(TBGPath);
   mtv = new TMacroTreeView(TestForm);
   mtv->SetPathToMacro(TBGPath);
   mtv->LoadMacroTree("Macro.tree");

   Gdiplus::Status s;
   Gdiplus::GdiplusStartupInput input;
   Gdiplus::GdiplusStartupOutput output;

   s = Gdiplus::GdiplusStartup(&token, &input, &output);
   ;
}
//---------------------------------------------------------------------------
void __fastcall TTestForm::Button1Click(TObject *Sender)
{
  OpenDialog1->Filter = "TBG files (*.TBG)|*.TBG|Windows TBG files (*.WTBG)|*.WTBG";
   if(OpenDialog1->Execute()){

      TPictureTBG *NewPic = new TPictureTBG(OpenDialog1->FileName.c_str());

      if(NewPic){
         if(TestForm->PictureTBG){
            delete TestForm->PictureTBG;
         }// if
         TestForm->Caption = OpenDialog1->FileName;
         TestForm->PictureTBG = NewPic;
         NewPic->WorkPlace()->setEditProcessing(false);
         TestForm->Repaint();
      }/* if */

/*
     del_pic(&pic);
     SetPathToBGIFont("c:\\tbg\\bin");
     load_file(&pic, OpenDialog1->FileName.c_str());
*/
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TTestForm::FormPaint(TObject *Sender)
{
   if(PictureTBG){
      PictureTBG->setSizeScreen(ClientWidth, ClientHeight);
      PictureTBG->setCanvas(Canvas);
      PictureTBG->Show();
   }// if
}
//---------------------------------------------------------------------------
void __fastcall TTestForm::FormResize(TObject *Sender)
{
   Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TTestForm::FormDestroy(TObject *Sender)
{
   if(TestForm->PictureTBG){
      delete TestForm->PictureTBG;
   }// if
   if(mtv){
      mtv->DeleteAllPic();
      delete mtv;
   }// if
   Gdiplus::GdiplusShutdown(token);
}
//---------------------------------------------------------------------------

void __fastcall TTestForm::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   AnsiString HitStr = "...";
   if(PictureTBG && PictureTBG->Hit(TPoint(X, Y))){
      HitStr = "Попали";
   }// if
   if(PictureTBG){
      if(PictureTBG->pren(10, 100, TPoint(X, Y))){
         HitStr += " в: 10, 100";
         PictureTBG->setstate_obj_num(10, 100, 2, ::Show);
      }// if
      else{
         PictureTBG->setstate_obj_num(10, 100, 3, ::Show);
      }// elif
   }// if
   StatusBar1->Panels->Items[0]->Text = HitStr;
}
//---------------------------------------------------------------------------

void __fastcall TTestForm::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(PictureTBG){
      if(PictureTBG->pren(10, 100, TPoint(X, Y))){
         PictureTBG->setstate_obj_num(10, 100, Button == mbLeft ? 5 : 4, ::Show);
         PictureTBG->setstate_obj_num(10, 100, 2, ::Show);
      }// if
   }// if
}
//---------------------------------------------------------------------------

