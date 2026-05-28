/* TBG_005.CPP
*
*   TBG v7.0
*   08-05-2002 17:01 msk
*
*/

#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#include "Screen.h"
#endif

#include "tbg.hpp"
#include "tbg_tc.h"
#pragma hdrstop

// --- Rectangle --------------------------------------------------------------
// ----------------------------------------------------------------------------
bool __fastcall TRectangle::Load(char *rbuf){
   return TLine::Load(rbuf);
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TRectangle::Save(FILE *f){
   fprintf(f, "FrameRectangle");
   TLine::SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
void __fastcall TRectangle::Show(TWorkPlace *wp){
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;
   TCanvas *cvs = wp->getCanvas();
   cvs->Pen->Color = GetColor();
   cvs->Pen->Width = GetWidthLine();
   cvs->Pen->Style = psSolid;
   cvs->Brush->Style = bsClear;

   toScreen(wp);
   cvs->Rectangle(TRect(getps(0), getps(getMaxInd())));
};
// ----------------------------------------------------------------------------
bool __fastcall TRectangle::Hit(TWorkPlace *wp, TPoint p){
   bool ret = false;
   toScreen(wp);
   if(
      Cross(getps(0).x, getps(0).y, getps(1).x, getps(0).y, p.x, p.y, GetWidthLine() > 0 ? GetWidthLine() : 1) ||
      Cross(getps(1).x, getps(0).y, getps(1).x, getps(1).y, p.x, p.y, GetWidthLine() > 0 ? GetWidthLine() : 1) ||
      Cross(getps(1).x, getps(1).y, getps(0).x, getps(1).y, p.x, p.y, GetWidthLine() > 0 ? GetWidthLine() : 1) ||
      Cross(getps(0).x, getps(1).y, getps(0).x, getps(0).y, p.x, p.y, GetWidthLine() > 0 ? GetWidthLine() : 1))
      ret = true;
   return ret;
}// TRectangle::Hit
// ----------------------------------------------------------------------------
__fastcall TRectangle::TRectangle(){
          SetType(tFrameRect);
          SetStartXY(0.0, 0.0);
          SetEndXY(0.0, 0.0);
          SetColor((TColor)0);
          SetWidthLine(1);
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TRectangle::TRectangle(double ssX, double ssY,
      double seX, double seY,
      TColor sColor, unsigned sWidth):TLine(ssX, ssY, seX, seY, sColor, sWidth){
         SetType(tFrameRect);
         Smoothing(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

TRectangle::~TRectangle(){
};

AnsiString __fastcall TRectangle::GetName(void){return "прямоугольник";};

#ifdef _EDITOR_

TRectangle& __fastcall TRectangle::operator=(TRectangle& R){
   SetType(R.GetType());
   *(TLine*)this = (TLine&)R;
   TBaseGP::Smoothing(R.Smoothable());
   return *this;
};

TBaseGP* __fastcall TRectangle::Copy(CopyGEMode m){
   TRectangle *l = new TRectangle;
   *l = *this;
   switch(m){
      case cmWithLogic:
         this->CopyLE(l);
         break;
      case cmMirror:
         l->MirrorX(0.5);
         l->MirrorY(0.5);
         break;
      case cmMirrorX:
         l->MirrorX(0.5);
         break;
      case cmMirrorY:
         l->MirrorY(0.5);
         break;
   }/* switch */
   return l;
};
#endif

 