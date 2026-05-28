/* TBG_004.CPP
*
*   TBG v7.0
*   11-05-2002 22:06 msk
*
*/

#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#include "Screen.h"
#endif

#include "tbg.hpp"
#include "tbg_tc.h"
#pragma hdrstop

// --- Polyline ---------------------------------------------------------------
// ----------------------------------------------------------------------------
bool __fastcall TPolyLine::Load(char *rbuf){
   bool ret = false;

   char brkused, quoted, token[32];
   int i = 0,next = 0;

   Width = 1;
   OutColor = Color = clBlack;

   // Read my name
   parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
         case kwWidth:{
               int W;
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               if(ReadValue(token, rINT, &W))
                  Width = W;
            }
            break;
         case kwFGColor:{
               TColor C;
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               if(ReadValue(token, rHEX, &C))
                  OutColor = Color = C;
            }
            break;
      }// switch
   }// while

   ret = TBaseGP::Load(rbuf);

   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TPolyLine::SaveAsBase(FILE *f){
   if(Width != 1)       fprintf(f, " -width %u", Width);
   if(Color != clBlack) fprintf(f, " -fgc %x", Color);
   TBaseGP::SaveAsBase(f);
};
// ----------------------------------------------------------------------------
void __fastcall TPolyLine::Save(FILE *f){
   fprintf(f, "PolyLine");
   SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
void __fastcall TPolyLine::Show(TWorkPlace *wp){
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;

   Gdiplus::Graphics *graphics = wp->getGraphics();
   if(graphics){
      toScreen(wp);
      graphics->SetSmoothingMode(wp->getEditProcessing() ? Gdiplus::SmoothingModeHighSpeed : 
                                                           Gdiplus::SmoothingModeAntiAlias);
      Gdiplus::Pen pen(Gdiplus::Color(255, GetRValue(OutColor), GetGValue(OutColor), GetBValue(OutColor)));
      pen.SetWidth(Width);
      graphics->DrawLines(&pen, (Gdiplus::Point*)getps(), getSize());
   }// if
   Update = false;
/*
   TCanvas *cvs = wp->getCanvas();
   cvs->Pen->Style = psSolid;
   if(GetHideState() == true){
      cvs->Pen->Color = clDkGray;
      cvs->Pen->Width = 1;
   }// if
   else{
      cvs->Pen->Color = Color;
      cvs->Pen->Width = Width;
   }// elif

   toScreen(wp);
   if(Width == 1 && wp->getEditProcessing() == false)
      WuPolyline(cvs, getps(), getMaxInd());
   else
      cvs->Polyline(getps(), getMaxInd());
*/      
};
// ----------------------------------------------------------------------------
bool __fastcall TPolyLine::Hit(TWorkPlace *wp, TPoint p){
   bool ret = false;
   int i;

   toScreen(wp);
   for(i = 0;i < getSize();i++){
     ret = Cross(getps(i).x, getps(i).y, getps(i + 1).x, getps(i + 1).y, p.x, p.y, Width > 0 ? Width : 1) == 1 ? true : false;
     if(ret){
        break;
     }/* if */
   }/* for */
   return ret;
}// TPolyLine::Hit
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
int __fastcall TPolyLine::MouseDown(TWorkPlace *wp, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y){
   if(Creation()){
      // ГП создаётся
      if (Shift.Contains(ssLeft)){
         if(Op == Nop){
            Op = Start;
            SetStartXY(X / wp->getMaxX(), Y / wp->getMaxY());
            SetEndXY(X / wp->getMaxX(), Y / wp->getMaxY());
            Update = true;
            D(printf("\n Poly: Start created from: %d, %d", X, Y));
         }/* if */
         else{
            newSize(getSize() + 1);
            SetEndXY(X / wp->getMaxX(), Y / wp->getMaxY());
            Update = true;
            D(printf("\n Poly: add point"));
         }// elif
      }/* if */

      if (Shift.Contains(ssRight)){
         if(Op == Start){
            Op = Nop;
            if (Shift.Contains(ssCtrl)){
               this->SetHideState(true);
            }/* if */
            Update = true;
            D(printf("\n Poly: Stop created to: %d, %d", X, Y));
            return 1;
         }/* if */
      }/* if */
   }/* if */
   return 0;
};
// ----------------------------------------------------------------------------
int __fastcall TPolyLine::MouseMove(TWorkPlace *wp, TObject *Sender, TShiftState Shift, int X, int Y){
   if(Creation()){
      // ГП создаётся
      if(Op == Start){
        SetEndXY(X / wp->getMaxX(), Y / wp->getMaxY());
        Update = true;
        D(printf("\n Poly: Continued Create to: %d, %d", X, Y));
      }/* if */
   }/* if */
   return 0;
};
#endif
// ----------------------------------------------------------------------------
__fastcall TPolyLine::TPolyLine(){
          newSize(2);
          SetType(tPolyLine);
          SetStartXY(0.0, 0.0);
          SetEndXY(0.0, 0.0);
          OutColor = Color = (TColor)0;
          Width = 1;
          Smoothing(false);
          Update = true;
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TPolyLine::TPolyLine(TPolyLine &s){
          SetType(tPolyLine);
          newSize(2);
          Smoothing(s.Smoothable());
          Update = true;
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TPolyLine::TPolyLine(double ssX, double ssY,
      double seX, double seY,
      TColor sColor, unsigned sWidth){
         newSize(2);
         SetType(tPolyLine);
         SetStartXY(ssX, ssY);
         SetEndXY(seX, seY);
         OutColor = Color = sColor;
         Width = sWidth == 0 ? 1 : sWidth;
         Smoothing(false);
         Update = true;
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

TPolyLine::~TPolyLine(){
};

AnsiString __fastcall TPolyLine::GetName(void){return "ломаная";};

#ifdef _EDITOR_

int __fastcall TPolyLine::GetCoordCnt(void){return getSize();};

TPolyLine& __fastcall TPolyLine::operator=(TPolyLine& R){
   SetType(R.GetType());
   newSize(R.getSize());
   for(int i = 0;i < getSize();i++){
      getpf(i) = R.getpf(i);
   }/* for */
   OutColor = Color = R.Color;
   Width = R.Width;
   TBaseGP::Smoothing(R.Smoothable());
   Update = true;
   return *this;
};

TBaseGP* __fastcall TPolyLine::Copy(CopyGEMode m){
   TPolyLine *l = new TPolyLine(*this);
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
void __fastcall TPolyLine::Smoothing(bool to){
   TBaseGP::Smoothing(to && (Width == 1));
   Update = true;
};

