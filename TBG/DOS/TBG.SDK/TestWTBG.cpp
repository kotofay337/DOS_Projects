//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TestWTBG.h"

#include "tbg_tc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTestForm *TestForm;
OBJECT **pic = NULL;

TRect   rEGA;
TCanvas *gCanvas;
Graphics::TBitmap *gScreen;
//---------------------------------------------------------------------------
void ClrScr(){
   gCanvas->Brush->Style = bsSolid;
   gCanvas->Brush->Color = clBlack;
   gCanvas->FillRect(rEGA);
};
//---------------------------------------------------------------------------
__fastcall TTestForm::TTestForm(TComponent* Owner)
        : TForm(Owner)
{
   CheckBoxSmooth->Enabled = false;
   CreateEGATools();
   SetPathToBGIFont("c:\\tbg\\bin\\");
   MPCSetPath("c:\\tbg\\bin\\");

   rEGA = Rect(0,0,640,480);
   gScreen = new Graphics::TBitmap;
   gScreen->Width = 640;
   gScreen->Height = 480;
   gCanvas = gScreen->Canvas;

   ClrScr();
}
//---------------------------------------------------------------------------
void __fastcall TTestForm::Button1Click(TObject *Sender)
{
  OpenDialog1->Filter = "TBG files (*.TBG)|*.TBG|Windows TBG files (*.WTBG)|*.WTBG";
   if(OpenDialog1->Execute()){
     del_pic(&pic);
     load_file(&pic, OpenDialog1->FileName.c_str());
      if(pic){
         ClrScr();
         TBG_SetDC(pic, gCanvas->Handle);
         outpic_all(pic);
      }// if
     Repaint();
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TTestForm::FormPaint(TObject *Sender)
{
   Canvas->CopyRect(rEGA, gScreen->Canvas, rEGA);
}
//---------------------------------------------------------------------------

void __fastcall TTestForm::FormDestroy(TObject *Sender)
{
   DeleteEGATools();
   del_pic(&pic);
   delete gScreen;
}
//---------------------------------------------------------------------------




