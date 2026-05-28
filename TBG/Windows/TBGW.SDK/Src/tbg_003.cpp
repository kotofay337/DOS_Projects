/* TBG_003.CPP
*
*   TBG v7.0
*   9-05-2002 11:25 msk
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
bool __fastcall TFillPolyRect::Load(char *rbuf){
   bool ret = false;

   ColorFill = clWhite;
   StyleFill = bsSolid;

   char brkused, quoted, token[32];
   int i = 0,next = 0;

   // Read my name
   parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
         case kwBGColor:{
               TColor Color;
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               if(ReadValue(token, rHEX, &Color))
                  ColorFill = Color;
            }
            break;
         case kwFillStyle:{
               TBrushStyle BStyle;
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               if(ReadValue(token, rHEX, &BStyle))
                  StyleFill = BStyle;
            }
            break;
      }// switch
   }// while
   ret = TPolyRect::Load(rbuf);
   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TFillPolyRect::Save(FILE *f){
   fprintf(f, "FillPolyRectangle");
   if(ColorFill  != clWhite) fprintf(f, " -bgc %x", ColorFill);
   if(StyleFill  != bsSolid) fprintf(f, " -fs %x", StyleFill);
   TPolyRect::SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
bool __fastcall TFillPolyRect::Hit(TWorkPlace *wp, TPoint p){
   bool ret = false;
   TCanvas *cvs = wp->getCanvas();
   HDC hDC = cvs->Handle;
   toScreen(wp);
   HRGN hRgn = CreatePolygonRgn(getps(), getSize(), ALTERNATE);
//   SelectClipRgn(hDC, hRgn);
//   ret = PtVisible(hDC, p.x, p.y);
   ret = PtInRegion(hRgn, p.x, p.y);
//   SelectClipRgn(hDC, NULL);
   DeleteObject(hRgn);
   return ret;
}// TPolyRect::Hit
// ----------------------------------------------------------------------------
void __fastcall TFillPolyRect::Show(TWorkPlace *wp){
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;
   TCanvas *cvs = wp->getCanvas();
   cvs->Pen->Color = GetColor();
   cvs->Pen->Width = GetWidthLine();
   cvs->Pen->Style = psSolid;
   cvs->Brush->Color = ColorFill;
   cvs->Brush->Style = StyleFill;

   getpf(getMaxInd()) = getpf(0);
   toScreen(wp);

   cvs->Polygon(getps(), getMaxInd());
   if(GetWidthLine() == 1 && wp->getEditProcessing() == false)
      WuPolyline(cvs, getps(), getMaxInd());
};
// ----------------------------------------------------------------------------
__fastcall TFillPolyRect::TFillPolyRect():TPolyRect(){
          SetType(tFillPolyRect);
          ColorFill = clWhite;
          StyleFill = bsSolid;
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TFillPolyRect::TFillPolyRect(double ssX, double ssY,
     double seX, double seY,
     TColor sColor, TColor sColorFill, TBrushStyle sStyleFill, unsigned sWidth):TPolyRect(ssX, ssY, seX, seY, sColor, sWidth){
         SetType(tFillPolyRect);
         ColorFill = sColorFill;
         StyleFill = sStyleFill;
         Smoothing(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

TFillPolyRect::~TFillPolyRect(){
};

AnsiString __fastcall TFillPolyRect::GetName(void){return "4-х угольник заполненный";};

#ifdef _EDITOR_

TFillPolyRect& __fastcall TFillPolyRect::operator=(TFillPolyRect& R){
   SetType(R.GetType());
   *(TPolyRect*)this = (TPolyRect&)R;
   ColorFill = R.ColorFill;
   StyleFill = R.StyleFill;
   return *this;
};

TBaseGP* __fastcall TFillPolyRect::Copy(CopyGEMode m){
   TFillPolyRect *l = new TFillPolyRect;
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



