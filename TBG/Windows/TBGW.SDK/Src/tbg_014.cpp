/* TBG_014.CPP
*
*   TBG v7.0
*   14-05-2003 18:46 msk
*
*/

#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#include "Screen.h"
#endif

#include <math.h>
#include <Clipbrd.hpp>
#include "tbg.hpp"
#include "tbg_tc.h"
#pragma hdrstop


// --- FillArc ---------------------------------------------------------------
// ----------------------------------------------------------------------------
bool __fastcall TFillArc::Load(char *rbuf){
   bool ret = false;

   char brkused, quoted, token[32];
   int next = 0;

   ColorFill = clWhite;
   StyleFill = bsSolid;
   // Read my name
   parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
         case kwBGColor:
            parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
            ReadValue(token, rHEX, &ColorFill);
            break;
         case kwFillStyle:
            parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
            ReadValue(token, rHEX, &StyleFill);
            break;
      }// switch
   }// while
   ret = TArc::Load(rbuf);
   return ret;
};
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
void __fastcall TFillArc::Save(FILE *f){
   fprintf(f, "FillArc");
   if(ColorFill  != clWhite) fprintf(f, " -bgc %x", ColorFill);
   if(StyleFill  != bsSolid) fprintf(f, " -fs %x", StyleFill);
   TArc::SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
void __fastcall TFillArc::Show(TWorkPlace *wp){
/*
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;

   toScreen(wp);

   Gdiplus::Graphics *graphics = wp->getGraphics();

   if(graphics){
      graphics->SetSmoothingMode(wp->getEditProcessing() ? Gdiplus::SmoothingModeHighSpeed : 
                                                           Gdiplus::SmoothingModeAntiAlias);

      TColor c = GetColor();
      Gdiplus::Color c1(255, GetRValue(c), GetGValue(c), GetBValue(c));
      c = ColorFill;
      Gdiplus::Color c2(255, GetRValue(c), GetGValue(c), GetBValue(c));
      Gdiplus::Pen pen(c1);
      pen.SetWidth(GetWidth());
//      Gdiplus::Brush b;
      

      int x, y, xe, ye, w, h;
         
      x = min(getps(0).x, getps(1).x);
      y = min(getps(0).y, getps(1).y);
      xe = max(getps(1).x, getps(0).x);
      ye = max(getps(1).y, getps(0).y);
      w = max(x, xe) - min(x, xe) - 1;
      h = max(y, ye) - min(y, ye) - 1;

      if(getpf(2) == getpf(3)){
         graphics->FillEllipse(&pen, x, y, w, h);
      }// if
      else{
         int xc, yc;
         double startA, sweepA;
         
         xc = x + w / 2;
         yc = y + h / 2;

         TPointf cp(xc / wp->getMaxX() , yc / wp->getMaxY());
         
         startA = cp.angleto(getpf(3));
         sweepA = cp.angleto(getpf(2)) - startA;
         if(sweepA < 0.0) 
            sweepA += 360.0;
         graphics->FillPie(&pen, x, y, w, h, startA, sweepA);
      }// elif
  }// if
*/
/* Gradient path
    // Create a path that consists of a single ellipse.
   Gdiplus::GraphicsPath path;
   path.AddEllipse(x, y, w, h);

   // Use the path to construct a brush.
   Gdiplus::PathGradientBrush pthGrBrush(&path);

   // Set the center point to a location that is not the centroid of the path.
   pthGrBrush.SetCenterPoint(Gdiplus::Point(x + w / 2, y + h / 2));

   // Set the color at the center point to blue.
   pthGrBrush.SetCenterColor(c2);

   // Set the color along the entire boundary of the path to aqua.
   Gdiplus::Color colors[] = {c1};
   int count = 1;
   pthGrBrush.SetSurroundColors(colors, &count);

   graphics->FillEllipse(&pthGrBrush, x, y, w, h);
*/


///* Old game's

   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;

   TCanvas *cvs = wp->getCanvas();

   cvs->Pen->Color = GetColor();
   cvs->Pen->Width = GetWidth();
   cvs->Pen->Style = psSolid;
   cvs->Brush->Style = StyleFill;
   cvs->Brush->Color = ColorFill;

   toScreen(wp);

   if(getpf(2) == getpf(3)){
      cvs->Ellipse(
         getps(0).x, getps(0).y,
         getps(1).x, getps(1).y);
   }// if
   else{
      cvs->Pie(
         getps(0).x, getps(0).y,
         getps(1).x, getps(1).y,
         getps(2).x, getps(2).y,
         getps(3).x, getps(3).y);
   }// elif
//*/
};
// ----------------------------------------------------------------------------
/*
bool __fastcall TFillArc::Hit(TWorkPlace *wp, TPoint p){
   bool ret = false;
   TCanvas *cvs = wp->getCanvas();
   HDC hDC = cvs->Handle;
   toScreen(wp);
   HRGN hRgn = CreateEllipticRgn(getps(0).x, getps(0).y,
                                 getps(1).x, getps(1).y);
   RECT rbox;
   GetRgnBox(hRgn, &rbox);
   if(PtInRect(rbox, TPoint(p.x, p.y)))
      ret = PtInRegion(hRgn, p.x, p.y);
   DeleteObject(hRgn);
   return ret;
};
*/
// ----------------------------------------------------------------------------
__fastcall TFillArc::TFillArc():TArc(){
          ColorFill = clBlack;
       };

__fastcall TFillArc::TFillArc(TFillArc &s):TArc(s.GetCoordPtr(0), s.GetColor(), s.GetWidth()){
          ColorFill = s.ColorFill;
          StyleFill = s.StyleFill;
          Smoothing(s.Smoothable());
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TFillArc::TFillArc(TPointf *p,
      TColor sColor, TColor sColorFill, TBrushStyle sStyleFill, unsigned sWidth):TArc(p, sColor, sWidth){
         ColorFill = sColorFill;
         StyleFill = sStyleFill;
      };

TFillArc::~TFillArc(){
   TArc::~TArc();
};

AnsiString __fastcall TFillArc::GetName(void){return "сектор";};

#ifdef _EDITOR_

TFillArc& __fastcall TFillArc::operator=(TFillArc& R){
   ColorFill = R.ColorFill;
   StyleFill = R.StyleFill;
   *(TArc*)this = (TArc&)R;
   return *this;
};

TBaseGP* __fastcall TFillArc::Copy(CopyGEMode m){
   TFillArc *l = new TFillArc(*this);
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




