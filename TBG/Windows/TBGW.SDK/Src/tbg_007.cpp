/* TBG_007.CPP
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
bool __fastcall TPolygon::Load(char *rbuf){
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
   ret = TPolyLine::Load(rbuf);
   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TPolygon::SaveAsBase(FILE *f){
   if(ColorFill  != clWhite) fprintf(f, " -bgc %x", ColorFill);
   if(StyleFill  != bsSolid) fprintf(f, " -fs %x", StyleFill);
   TPolyLine::SaveAsBase(f);
};
// ----------------------------------------------------------------------------
void __fastcall TPolygon::Save(FILE *f){
   fprintf(f, "Polygon");
   SaveAsBase(f);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
bool __fastcall TPolygon::Hit(TWorkPlace *wp, TPoint p){
   bool ret = false;
   TCanvas *cvs = wp->getCanvas();
   HDC hDC = cvs->Handle;

   toScreen(wp);
   HRGN hRgn = CreatePolygonRgn(getps(), getSize(), ALTERNATE);
   // PtInRegion
   //SelectClipRgn(cvs->Handle, hRgn);
   RECT rbox;
   GetRgnBox(hRgn, &rbox);
   if(PtInRect(rbox, TPoint(p.x, p.y)))
      ret = PtInRegion(hRgn, p.x, p.y);
   //SelectClipRgn(hDC, NULL);
   DeleteObject(hRgn);
   return ret;
}// TPolyLine::Hit
// ----------------------------------------------------------------------------
void __fastcall TPolygon::Show(TWorkPlace *wp){
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;
   TCanvas *cvs = wp->getCanvas();
   if(GetHideState() == true){
      cvs->Pen->Color = clDkGray;
      cvs->Pen->Width = 1;
      cvs->Brush->Color = clDkGray;
      cvs->Brush->Style = bsDiagCross;
   }// if
   else{
      cvs->Pen->Color = GetColor();
      cvs->Pen->Width = GetWidthLine();
      cvs->Brush->Color = ColorFill;
      cvs->Brush->Style = StyleFill;
   }// elif
   cvs->Pen->Style = psSolid;
   toScreen(wp);
   cvs->Polygon(getps(), getMaxInd());
   if((GetWidthLine() == 1) && (wp->getEditProcessing() == false)){
      WuPolyline(cvs, getps(), getMaxInd());
      WuLine(cvs, getps(getMaxInd()).x, getps(getMaxInd()).y, getps(0).x, getps(0).y, 255, 255, 8, (unsigned long)GetColor());
   }// if
};
// ----------------------------------------------------------------------------
__fastcall TPolygon::TPolygon():TPolyLine(){
          SetType(tPolygon);
          ColorFill = clBlack;
          StyleFill = bsSolid;
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TPolygon::TPolygon(TPolygon &s){
          SetType(tPolygon);
          //*this = s;
          Smoothing(s.Smoothable());
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TPolygon::TPolygon(double ssX, double ssY,
     double seX, double seY,
     TColor sColor, TColor sColorFill, TBrushStyle sStyleFill, unsigned sWidth):TPolyLine(ssX, ssY, seX, seY, sColor, sWidth){
         SetType(tPolygon);
         ColorFill = sColorFill;
         StyleFill = sStyleFill;
         Smoothing(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

TPolygon::~TPolygon(){
};

AnsiString __fastcall TPolygon::GetName(void){return "полигон";};

#ifdef _EDITOR_

TPolygon& __fastcall TPolygon::operator=(TPolygon& R){
   *(TPolyLine*)this = (TPolyLine&)R;
   SetType(R.GetType());
   ColorFill = R.ColorFill;
   StyleFill = R.StyleFill;
   TBaseGP::Smoothing(R.Smoothable());
   return *this;
};

TBaseGP* __fastcall TPolygon::Copy(CopyGEMode m){
   TPolygon *l = new TPolygon;
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


