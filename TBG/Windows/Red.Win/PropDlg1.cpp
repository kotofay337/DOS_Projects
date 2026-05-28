//---------------------------------------------------------------------
#include <vcl.h>

#include "Screen.h"
#include "ToolBar.h"
#include "TBG_Editor.hpp"

#include "PropDlg1.h"
#include "tbg.hpp"
#pragma hdrstop
//---------------------------------------------------------------------
#pragma link "CGRID"
#pragma resource "*.dfm"
TPropDlg *PropDlg;
//---------------------------------------------------------------------
void ShapesInvalidate(TColor cp, TColor cf){
   if(PropDlg){
      PropDlg->Shape1->Brush->Color = cf;
      PropDlg->Shape2->Brush->Color = cf;
      PropDlg->Shape3->Brush->Color = cf;
      PropDlg->Shape4->Brush->Color = cf;
      PropDlg->Shape5->Brush->Color = cf;
      PropDlg->Shape6->Brush->Color = cf;
      PropDlg->Shape7->Brush->Color = cf;
      PropDlg->Shape8->Brush->Color = cf;

      PropDlg->Shape1->Pen->Color = cp;
      PropDlg->Shape2->Pen->Color = cp;
      PropDlg->Shape3->Pen->Color = cp;
      PropDlg->Shape4->Pen->Color = cp;
      PropDlg->Shape5->Pen->Color = cp;
      PropDlg->Shape6->Pen->Color = cp;
      PropDlg->Shape7->Pen->Color = cp;
      PropDlg->Shape8->Pen->Color = cp;
      {
         if(PropDlg->pic){
            TCanvas *cvs = PropDlg->GradientSelectPanel->Canvas;
            TPictureTBG *pic = PropDlg->pic;
            TBaseGP *gp = pic->FindFirstNo(0);
            while(gp){
              if(gp->GetType() == tGradientFillRect){
                 TGradientFillRectangle *r = (TGradientFillRectangle*)gp;
                 r->SetColor(cp);
                 r->SetColor2(cf);
              }// if
              gp = pic->FindNextNo(0);
            }/* while */
            pic->setSizeScreen(PropDlg->GradientSelectPanel->Width, PropDlg->GradientSelectPanel->Height);
            pic->setCanvas(cvs);
            pic->Show();
            PropDlg->GradientSelectPanel->Repaint();
         }// if
      }
   }/* if */
};
//---------------------------------------------------------------------

__fastcall TPropDlg::TPropDlg(TComponent* AOwner)
	: TForm(AOwner)
{
   ThickLineText->Text = WidthLine;
   pic = new TPictureTBG("GradientPanel.wTBG");
}
//---------------------------------------------------------------------

void __fastcall TPropDlg::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
   WidthLine += Button == Comctrls::btNext ? 1 : -1;

   if((WidthLine > 254) || (WidthLine < 1))
      WidthLine = 1;

   ThickLineText->Text = WidthLine;
   shpLineInvalidate();
}
//---------------------------------------------------------------------------

void __fastcall TPropDlg::ThickLineTextChange(TObject *Sender)
{
   WidthLine = atoi(ThickLineText->Text.c_str());

   if((WidthLine > 254) || (WidthLine < 1))
      WidthLine = 1;

   ThickLineText->Text = WidthLine;
   shpLineInvalidate();
}
//---------------------------------------------------------------------------

void __fastcall TPropDlg::OKBtnClick(TObject *Sender)
{
   PropDlg->Visible = false;
}
//---------------------------------------------------------------------------


void __fastcall TPropDlg::cbSelLEClick(TObject *Sender)
{
   if(cbSelLE->Checked){
      EditNo->Enabled = true;   
      EditNe->Enabled = true;   
   }/* if */
   else{
      EditNo->Enabled = false;   
      EditNe->Enabled = false;   
   }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TPropDlg::ShapesMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(Button == mbLeft){
      StyleFill = (TBrushStyle)((TShape*)Sender)->Tag;
      shpRectInvalidate();
   }/* if */   
}
//---------------------------------------------------------------------------

void __fastcall TPropDlg::FormPaint(TObject *Sender)
{
   ShapesInvalidate(ColorLine, ColorFill);
}
//---------------------------------------------------------------------------
void __fastcall TPropDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
   if(pic)
      pic->DeleteAllGP();	
}
//---------------------------------------------------------------------------


void __fastcall TPropDlg::GradientSelectPanelMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(pic){
      pic->setSizeScreen(GradientSelectPanel->Width, GradientSelectPanel->Height);
      TBaseGP *gp = pic->Hit(TPoint(X, Y));
      if(gp && (gp->GetType() == tGradientFillRect)){
         GradientStyleFill = ((TGradientFillRectangle*)gp)->GetGradientFillStyle();
         shpRectInvalidate();
      }// if
   }// if
}
//---------------------------------------------------------------------------



