/* TBG_011.CPP
*
*   TBG v7.0
*   11-05-2002 22:06 msk
*
*/

#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#include "Screen.h"
#endif

#include <math.h>
#include "tbg.hpp"
#include "tbg_tc.h"
#pragma hdrstop

// --- Arc ---------------------------------------------------------------
// ----------------------------------------------------------------------------
bool __fastcall TArc::Load(char *rbuf){
   bool ret = false;

   char brkused, quoted, token[32];
   int i = 0,next = 0;

   Width = 1;
   Color = clBlack;
   Angle = 0.0;

   ret = TBaseGP::Load(rbuf);

   // Read my name
   parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
         case kwWidth:
            parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
            ReadValue(token, rINT, &Width);
            break;
         case kwFGColor:
            parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
            ReadValue(token, rHEX, &Color);
            break;
         case kwAngle:
            parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
            ReadValue(token, rDOUBLE, &Angle);
            if(Angle)
               CenterRotatePoint.moveto(getpf(0).getx() + (getpf(1).getx() - getpf(0).getx()) / 2.0,
                                        getpf(0).gety() + (getpf(1).gety() - getpf(0).gety()) / 2.0);
            break;
      }// switch
   }// while

   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
void __fastcall TArc::SaveAsBase(FILE *f){
   if(Width != 1)       fprintf(f, " -width %u", Width);
   if(Color != clBlack) fprintf(f, " -fgc %x", Color);
   if(Angle != 0.0)     fprintf(f, " -angle %lf", Angle);
   TBaseGP::SaveAsBase(f);
};
// ----------------------------------------------------------------------------
void __fastcall TArc::Save(FILE *f){
   fprintf(f, "Arc");
   SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
void __fastcall TArc::Show(TWorkPlace *wp){
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;
   toScreen(wp);
   Gdiplus::Graphics *graphics = wp->getGraphics();
   if(graphics){
      graphics->SetSmoothingMode(wp->getEditProcessing() ? Gdiplus::SmoothingModeHighSpeed : 
                                                           Gdiplus::SmoothingModeAntiAlias);
      Gdiplus::Pen pen(Gdiplus::Color(255, GetRValue(Color), GetGValue(Color), GetBValue(Color)));
      pen.SetWidth(Width);

      int x, y, xe, ye, w, h;
         
      x = min(getps(0).x, getps(1).x);
      y = min(getps(0).y, getps(1).y);
      xe = max(getps(1).x, getps(0).x);
      ye = max(getps(1).y, getps(0).y);
      w = max(x, xe) - min(x, xe) - 1;
      h = max(y, ye) - min(y, ye) - 1;

      if(getpf(2) == getpf(3)){
         graphics->DrawEllipse(&pen, x, y, w, h);
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
         graphics->DrawArc(&pen, x, y, w, h, startA, sweepA);
      }// elif
  }// if
      
/*
   }// if
   else{
      TCanvas *cvs = wp->getCanvas();
      cvs->Pen->Color = Color;
      cvs->Pen->Width = Width;
      cvs->Pen->Style = psSolid;
      cvs->Arc(
         getps(0).x, getps(0).y,
         getps(1).x, getps(1).y,
         getps(2).x, getps(2).y,
         getps(3).x, getps(3).y);
   }// elif
*/      
};
// ----------------------------------------------------------------------------
bool __fastcall TArc::Hit(TWorkPlace *wp, TPoint p){
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
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
void __fastcall TArc::ShowSelect(TWorkPlace *wp, SSMode m){
   int X, Y, Xc, Yc;
   TCanvas *cvs = wp->getCanvas();

   toScreen(wp);

   // Это отображается когда элемент текущий и выбран
   if(GetSelect() == true){

      TPointf pf[8];
      TPoint ps[8];

      pf[0] = getpf(0);
      pf[1].moveto(getpf(1).getx(), getpf(0).gety());
      pf[2] = getpf(1);
      pf[3].moveto(getpf(0).getx(), getpf(1).gety());
      pf[4] = getpf(0);

      // начало-конец дуги
      pf[5] = getpf(2);
      pf[6] = getpf(3);

      // центр 
      pf[7].moveto(getpf(0).getx() + (getpf(1).getx() - getpf(0).getx()) / 2.0,
                   getpf(0).gety() + (getpf(1).gety() - getpf(0).gety()) / 2.0);

      for(int i = 0;i < 8;i++)
         ps[i] = pf[i].toscreen(wp->getMaxX(), wp->getMaxY());

      cvs->Pen->Style = psDot;
      cvs->Pen->Width = 1;
      cvs->Pen->Color = clWhite;
      cvs->Brush->Color = clBlack;

      Xc = ps[7].x;
      Yc = ps[7].y;
      cvs->MoveTo(Xc, Yc);

      X = ps[5].x;
      Y = ps[5].y;
      cvs->LineTo(X, Y);

      cvs->MoveTo(Xc, Yc);

      X = ps[6].x;
      Y = ps[6].y;
      cvs->LineTo(X, Y);

      cvs->Polyline(ps, 4);
   }/* if */

   if((GetSelect() == true) && (m != SSSelect) && (m != SSIndicate)){
      for(int i = 0;i < getSize();i++){
        ShowCoordSelector(wp, getps(i).x, getps(i).y);
      }/* for */
   }/* if */

   if(m == SSSelect){
      X = getps(0).x;
      Y = getps(0).y;
      ShowSelector(cvs, X, Y, GetSelect());
   }/* if */

   if(m == SSIndicate){
     cvs->Pen->Mode = pmXor;
     Show(wp);
     cvs->Pen->Mode = pmCopy;
   }/* if */
}/* ShowSelect */
// ----------------------------------------------------------------------------
int __fastcall TArc::MouseDown(TWorkPlace *wp, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y){
   if(Creation()){
      // ГП создаётся
      if (Shift.Contains(ssLeft)){
         if(Op == Nop){
            Op = Start;
            SetStartXY(X / wp->getMaxX(), Y / wp->getMaxY());
            SetEndXY(X / wp->getMaxX(), Y / wp->getMaxY());
            SetPhiStart(X / wp->getMaxX(), Y / wp->getMaxY());
            SetPhiEnd(X / wp->getMaxX(), Y / wp->getMaxY());
            D(printf("\n Arc: Start created from: %d, %d", X, Y));
         }/* if */
         else{
            return -1;
         }// elif
      }/* if */

      if (Shift.Contains(ssRight)){
         if(Op == Start){
            Op = Nop;
            D(printf("\n Arc: Stop created to: %d, %d", X, Y));
            return 1;
         }/* if */
      }/* if */
   }/* if */
   return 0;
};
// ----------------------------------------------------------------------------
int __fastcall TArc::MouseMove(TWorkPlace *wp, TObject *Sender, TShiftState Shift, int X, int Y){
   if(Creation()){
      double MaxX = wp->getMaxX(),
             MaxY = wp->getMaxY();
      double dX = X / MaxX,
             dY = Y / MaxY;
      // ГП создаётся
      if(Op == Start){
        if (Shift.Contains(ssShift)){ // toCircle
           double DistX = getpf(1).getx() - getpf(0).getx();
           double Aspect = MaxX / MaxY;
           dY = getpf(0).gety() + DistX * Aspect;
        }/* if */

        SetEndXY(dX, dY);
        SetPhiStart(dX, dY);
        SetPhiEnd(dX, dY);

        if (Shift.Contains(ssCtrl)){ // from Center
        }/* if */

        D(printf("\n Arc: Continued Create to: %d, %d", X, Y));

        if (Shift.Contains(ssAlt)){
           double x1, y1, x2, y2, a;
           x1 = getpf(0).getx() + (getpf(1).getx() - getpf(0).getx()) / 2.0;
           y1 = getpf(0).gety() + (getpf(1).gety() - getpf(0).gety()) / 2.0;
           x2 = X / MaxX;
           y2 = Y / MaxY;
           if((x2 - x1)){
              a = atan((y2 - y1)/(x2 - x1)) * RAD;
              if((x2 - x1) < 0.0){
                 a = 180 + a;
              }/* if */
              Angle = a;
              if(Angle != 0.0)
                 CenterRotatePoint.moveto(x1, y1);
           }/* if */
        }/* if */
         /*
         if(Angle != 0.0){
            TPointf cp; // x1 + (x2 - x1) / 2
            cp.moveto(getpf(0).getx() + (getpf(1).getx() - getpf(0).getx()) / 2.0,
                      getpf(0).gety() + (getpf(1).gety() - getpf(0).gety()) / 2.0);
            RotateTo(cp, Angle);
         }// if
         */
      }/* if */
   }/* if */
   return 0;
};
#endif
// ----------------------------------------------------------------------------
__fastcall TArc::TArc(){
          newSize(4);
          SetType(tArc);
          SetStartXY(0.0, 0.0);
          SetEndXY(0.0, 0.0);
          SetPhiStart(0.0, 0.0);
          SetPhiEnd(0.0, 0.0);
          Color = (TColor)0;
          Width = 1;
          Angle = 0.0;
          cs = NULL;
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TArc::TArc(TArc &s){
//          *this = s;
          newSize(s.getSize());
          SetType(s.GetType());
          for(int i = 0;i < getSize();i++){
             getpf(i) = s.getpf(i);
          }/* for */
          Color = s.Color;
          Width = s.Width;
          Angle = s.Angle;
          cs = NULL;
          Smoothing(s.Smoothable());
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TArc::TArc(TPointf *p,
      TColor sColor, unsigned sWidth){
         newSize(4);
         SetType(tArc);
         SetStartXY(p[0].getx(), p[0].gety());
         SetEndXY(p[1].getx(), p[1].gety());
         SetPhiStart(p[2]);
         SetPhiEnd(p[3]);
         Color = sColor;
         Width = sWidth == 0 ? 1 : sWidth;
         Angle = 0.0;
         cs = NULL;
         Smoothing(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

TArc::~TArc(){
   if(cs)
      delete cs;
};

AnsiString __fastcall TArc::GetName(void){return "дуга";};

void __fastcall TArc::SetEndXY(double seX, double seY){
   getpf(1).moveto(seX, seY);
};

void __fastcall TArc::SetPhiStart(double PhiSx, double PhiSy){
   getpf(2).moveto(PhiSx, PhiSy);
};

void __fastcall TArc::SetPhiEnd(double PhiEx, double PhiEy){
   getpf(3).moveto(PhiEx, PhiEy);
};

void __fastcall TArc::SetPhiStart(TPointf &p){
   getpf(2) = p;
};

void __fastcall TArc::SetPhiEnd(TPointf &p){
   getpf(3) = p;
};

TPointf& __fastcall TArc::GetPhiStart(void){
   return getpf(2);
};

TPointf& __fastcall TArc::GetPhiEnd(void){
   return getpf(3);
};

#ifdef _EDITOR_

void __fastcall TArc::toCircle(void){
};

TArc& __fastcall TArc::operator=(TArc& R){
   newSize(R.getSize());
   SetType(R.GetType());
   for(int i = 0;i < getSize();i++){
      getpf(i) = R.getpf(i);
   }/* for */
   Color = R.Color;
   Width = R.Width;
   Angle = R.Angle;
   cs = NULL;
   TBaseGP::Smoothing(R.Smoothable());
   return *this;
};

TBaseGP* __fastcall TArc::Copy(CopyGEMode m){
   TArc *l = new TArc(*this);
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

 
 