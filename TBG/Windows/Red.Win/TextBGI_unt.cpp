//---------------------------------------------------------------------------

#include <vcl.h>
#include <Registry.hpp>

#include "Screen.h"
#include "ToolBar.h"
#include "PropDlg1.h"

#include "tbg.hpp"
#include "TBG_Editor.hpp"

#include "TextBGI_unt.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTextBGI *TextBGI;
//---------------------------------------------------------------------------
__fastcall TTextBGI::TTextBGI(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TTextBGI::TextBGIEditKeyPress(TObject *Sender, char &Key)
{
   switch(Key){
      case VK_RETURN:
         strcpy(sTextBGI, TextBGIEdit->Text.c_str());
         TextBGI->Visible = false;
         if(Painter && Painter->PictureTBG && Painter->EditGP && Painter->EditGP->GetType() == tBGIText){
            Painter->Repaint();
            Painter->EditGP = NULL;
         }/* if */
         break;
   }/* switch */   
}
//---------------------------------------------------------------------------

void __fastcall TTextBGI::TextBGIEditChange(TObject *Sender)
{

   strcpy(sTextBGI, TextBGIEdit->Text.c_str());
   if(Painter && Painter->PictureTBG && Painter->EditGP && Painter->EditGP->GetType() == tBGIText){
      CharToOem(sTextBGI, sTextBGI);
      ((TBGIText *)Painter->EditGP)->SetText(sTextBGI);
      OemToChar(sTextBGI, sTextBGI);
      Painter->Repaint();
   }/* if */

}
//---------------------------------------------------------------------------

void __fastcall TTextBGI::UpDown1Click(TObject *Sender, TUDBtnType Button)
{

   dAngleText += Button == Comctrls::btNext ? 0.1 : -0.1;
   if(dAngleText > 360.)
      dAngleText = 0.;
   if(dAngleText < 0.)
      dAngleText = 360.;

   if(Painter && Painter->PictureTBG && Painter->EditGP && Painter->EditGP->GetType() == tBGIText){
//      ((TBGIText *)Painter->EditGP)->SetAngle(dAngleText);
      Painter->Repaint();
   }/* if */

   Edit1->Text = dAngleText;
}
//---------------------------------------------------------------------------
void __fastcall TTextBGI::UpDown2Click(TObject *Sender, TUDBtnType Button)
{
   SizeFntBGI += Button == Comctrls::btNext ? 1 : -1;
   if(SizeFntBGI > 10)
      SizeFntBGI = 0;
   if(SizeFntBGI < 0)
      SizeFntBGI = 10;

   if(Painter && Painter->PictureTBG && Painter->EditGP && Painter->EditGP->GetType() == tBGIText){
//      ((TBGIText *)Painter->EditGP)->SetAngle(dAngleText);
      Painter->Repaint();
   }/* if */

   Edit2->Text = SizeFntBGI;
}
//---------------------------------------------------------------------------
void __fastcall TTextBGI::FormActivate(TObject *Sender)
{
   Edit1->Text = dAngleText;
   Edit2->Text = SizeFntBGI;
}
//---------------------------------------------------------------------------

void __fastcall TTextBGI::Edit1KeyPress(TObject *Sender, char &Key)
{
   switch(Key){
      case VK_RETURN:
         try {
            dAngleText = Edit1->Text.ToDouble();
         }
         catch (Exception &exception){
            Application->MessageBox("Недопустимый угол поворота текста.\n"
                                    "Введите число с десятичной запятой.", "Ошибка", MB_ICONWARNING);
         }
         if(dAngleText > 360.)
            dAngleText = 0.;
         if(dAngleText < 0.)
            dAngleText = 360.;

         if(Painter && Painter->PictureTBG && Painter->EditGP && Painter->EditGP->GetType() == tBGIText){
//            ((TBGIText *)Painter->EditGP)->SetAngle(dAngleText);
            Painter->Repaint();
         }/* if */

         Edit1->Text = dAngleText;
         break;
   }/* switch */
}
//---------------------------------------------------------------------------


void __fastcall TTextBGI::Edit2KeyPress(TObject *Sender, char &Key)
{
   switch(Key){
      case VK_RETURN:
         try {
            SizeFntBGI = Edit2->Text.ToInt();
         }
         catch (Exception &exception){
            Application->MessageBox("Недопустимый размер текста BGI.\n"
                                    "Введите число от 0 до 10.", "Ошибка", MB_ICONWARNING);
         }

         if(SizeFntBGI > 10)
            SizeFntBGI = 0;
         if(SizeFntBGI < 0)
            SizeFntBGI = 10;

         if(Painter && Painter->PictureTBG && Painter->EditGP && Painter->EditGP->GetType() == tBGIText){
//            ((TBGIText *)Painter->EditGP)->SetAngle(dAngleText);
            Painter->Repaint();
         }/* if */

         Edit2->Text = SizeFntBGI;
         break;
   }/* switch */
}
//---------------------------------------------------------------------------

