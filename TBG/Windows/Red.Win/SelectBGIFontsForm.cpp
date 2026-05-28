//---------------------------------------------------------------------------

#include <vcl.h>
#include <Graphics.hpp>

#include "TBG_Editor.hpp"
#include "SelectBGIFontsForm.h"

#include "tbg_tc.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSelectBGI *SelectBGI;
//---------------------------------------------------------------------------
void OutLabelBGIFont(TCanvas *Canvas, int ifnt, char *sfnt){
   Canvas->Pen->Color = ColorText;
   SetTextStyle(ifnt, HORIZ_DIR, 4);
   SetUserCharSize(10, 10, 10, 10);
   SetAngle(0.0);
   SetHDCFont(Canvas->Handle);
   SetTextJustify(LEFT_TEXT,TOP_TEXT);
   OutTextXY(0, 0, sfnt);
}/* OutLabelBGIFont */
//---------------------------------------------------------------------------
__fastcall TSelectBGI::TSelectBGI(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSelectBGI::FormPaint(TObject *Sender)
{
   OutLabelBGIFont(Image1->Canvas,  TRIPLEX_FONT,     "Triplex");
   OutLabelBGIFont(Image2->Canvas,  SMALL_FONT,       "Small");
   OutLabelBGIFont(Image3->Canvas,  SANS_SERIF_FONT,  "Sans serif");
   OutLabelBGIFont(Image4->Canvas,  GOTHIC_FONT,      "Gothic");
   OutLabelBGIFont(Image5->Canvas,  SCRIPT_FONT,      "Script");
   OutLabelBGIFont(Image6->Canvas,  SIMPLEX_FONT,     "Simplex");
   OutLabelBGIFont(Image7->Canvas,  TRIPLEX_SCR_FONT, "Triplex SCR");
   OutLabelBGIFont(Image8->Canvas,  COMPLEX_FONT,     "Complex");
   OutLabelBGIFont(Image9->Canvas,  EUROPEAN_FONT,    "Euro");
   OutLabelBGIFont(Image10->Canvas, BOLD_FONT,        "Bold");
}
//---------------------------------------------------------------------------

void __fastcall TSelectBGI::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(Button == mbRight){
      SelectBGI->Visible = false;
   }/* if */   
}
//---------------------------------------------------------------------------

void __fastcall TSelectBGI::FormKeyPress(TObject *Sender, char &Key)
{
   if(Key == VK_ESCAPE){
      SelectBGI->Visible = false;
   }/* if */   
   
}
//---------------------------------------------------------------------------

void __fastcall TSelectBGI::rbFontBGIClick(TObject *Sender)
{
   iFntBGI = ((TRadioButton*)Sender)->Checked ? ((TRadioButton*)Sender)->Tag : 10;
}
//---------------------------------------------------------------------------

