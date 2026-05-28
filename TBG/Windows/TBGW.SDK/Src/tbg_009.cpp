/* TBG_009.CPP
*
*   TBG v7.0
*   13-05-2002 21:43 msk
*
*/

#include <math.h>

#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#include "Screen.h"
#include "TextBGI_unt.h"
#endif

#include "tbg.hpp"
#include "tbg_tc.h"
#pragma hdrstop

// --- Текст BGI ------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool __fastcall TBGIText::Load(char *rbuf){
   bool ret = false;

   char brkused, quoted, token[1024];
   int i = 0,next = 0;

   Color = clBlack;

   // Read my name
   parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
         case kwFGColor:{
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               ReadValue(token, rHEX, &Color);
            }
            break;
         case kwText:{
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               Text = token;
            }
            break;
         case kwDataBGIText:{
               char t[32];
               int nx = 0;
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rSHORTINT, &BGIFont);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rSHORTINT, &Direction);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rSHORTINT, &Size);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rSHORTINT, &HorJustify);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rSHORTINT, &VerJustify);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rSHORTINT, &Sda);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rSHORTINT, &SSX);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rSHORTINT, &SSY);
               {
                  short int v;
                  parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
                  ret |= ReadValue(t, rSHORTINT, &v);
                  Mltx = v;
                  parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
                  ret |= ReadValue(t, rSHORTINT, &v);
                  Mlty = v;
               }
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rDOUBLE, &Angle);
            }
            break;
      }// switch
   }// while

   ret |= TBaseGP::Load(rbuf);

   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TBGIText::Save(FILE *f){
   fprintf(f, "BGIText");
   fprintf(f, " -text \"%s\"", pStr);
   if(Color != clBlack) fprintf(f, " -fgc %x", Color);
   TBaseGP::SaveAsBase(f);
   fprintf(f, " -dataBGI \"%d", BGIFont);
   fprintf(f, " %hd", Direction);
   fprintf(f, " %hd", Size);
   fprintf(f, " %hd", HorJustify);
   fprintf(f, " %hd", VerJustify);
   fprintf(f, " %hd", Sda);
   fprintf(f, " %hd", SSX);
   fprintf(f, " %hd", SSY);
   fprintf(f, " %d", Mltx);
   fprintf(f, " %d", Mlty);
   fprintf(f, " %lf\"", Angle);
};
// ----------------------------------------------------------------------------
#endif
void __fastcall TBGIText::SetText(char *t){
   if(pStr)
      delete pStr;
   lenStr = strlen(t) + 1;
   pStr = new char[lenStr];
   if(pStr)
      strcpy(pStr, t);
}
// ----------------------------------------------------------------------------
char* __fastcall TBGIText::GetText(void){
   return pStr;
}
// ----------------------------------------------------------------------------
void __fastcall TBGIText::SetFont(int ifont){
   BGIFont = ifont;
}
// ----------------------------------------------------------------------------
void __fastcall TBGIText::SetDirection(int dir){
   Direction = dir;
}
// ----------------------------------------------------------------------------
void __fastcall TBGIText::SetSize(short int size){
   Size= size;
}
// ----------------------------------------------------------------------------
void __fastcall TBGIText::SetAngle(double ang){
   Angle = ang;
}
// ----------------------------------------------------------------------------
void __fastcall TBGIText::SetJustify(short int hj, short int vj){
   HorJustify = hj;
   VerJustify = vj;
}
// ----------------------------------------------------------------------------
void __fastcall TBGIText::SetExt(short int sda, unsigned char mltx, unsigned char mlty){
   Sda = sda;
   Mltx = mltx;
   Mlty = mlty;
}
// ----------------------------------------------------------------------------
void __fastcall TBGIText::SetSS(short int sx, short int sy){
   SSX = sx;
   SSY = sy;
}
// ----------------------------------------------------------------------------
void __fastcall TBGIText::Show(TWorkPlace *wp){
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;
   TCanvas *cvs = wp->getCanvas();
   cvs->Pen->Color = Color;
   cvs->Pen->Width = 1;
//   cvs->Pen->Width = (Width / ((640 + 480.) / 2.)) * ((SizeScreenX + SizeScreenY) / 2.);
   cvs->Pen->Style = psSolid;

   toScreen(wp);

   SetTextJustify(HorJustify, VerJustify);
   ::SetAngle(Angle);
   if(BGIFont == DEFAULT_FONT){
      SetTextStyle(TRIPLEX_FONT, Direction, 4);
   }/* if */
   else{
      SetTextStyle(BGIFont, Direction, Size);

      double mw=(double)Sda / ((double)32760);
      double mxn, myn;

      mxn = (short int)(((double)(Mltx))* mw *
            ((double)wp->getMaxX()) / ((double)SSX) + 0.5);
      myn = (short int)(((double)(Mlty))* mw *
            ((double)wp->getMaxY()) / ((double)SSY) + 0.5);
      if(mxn > wp->getMaxX())
         mxn = wp->getMaxX();
      if(myn > wp->getMaxY())
         myn = wp->getMaxY();
      SetUserCharSize(mxn,10,myn,10);
   }// elif

   SetHDCFont(cvs->Handle);
   OutTextCol(getps(0).x, getps(0).y, pStr);
};
// ----------------------------------------------------------------------------
bool __fastcall TBGIText::Hit(TWorkPlace *wp, TPoint p){
   toScreen(wp);

   SetTextJustify(HorJustify, VerJustify);
   ::SetAngle(Angle);
   if(BGIFont == DEFAULT_FONT){
      SetTextStyle(TRIPLEX_FONT, Direction, 4);
   }/* if */
   else{
      SetTextStyle(BGIFont, Direction, 0);

      double mw=(double)Sda / ((double)32760);
      double mxn, myn;

      mxn = (short int)(((double)(Mltx))* mw *
            ((double)wp->getMaxX()) / ((double)SSX) + 0.5);
      myn = (short int)(((double)(Mlty))* mw *
            ((double)wp->getMaxY()) / ((double)SSY) + 0.5);
      if(mxn > wp->getMaxX())
         mxn = wp->getMaxX();
      if(myn > wp->getMaxY())
         myn = wp->getMaxY();
      SetUserCharSize(mxn,10,myn,10);
   }// elif

   TCanvas *cvs = wp->getCanvas();
   cvs->Pen->Color = clRed;
   cvs->Pen->Width = 1;
   cvs->Pen->Style = psSolid;

   TRect r;
   int th = 0, tw = 0;

   switch(Direction){
      case HORIZ_DIR:
         th = TextHeight(pStr);
         tw = TextWidth(pStr);
         break;
      case VERT_DIR:
         tw = TextHeight(pStr);
         th = TextWidth(pStr);
         break;
   }/* switch */

   r = TRect(getps(0).x, getps(0).y, getps(0).x + tw, getps(0).y + th);
   OffsetRect(r, HorJustify == CENTER_TEXT ? -tw / 2 :
                 HorJustify == RIGHT_TEXT  ? -tw     : 0,
                 VerJustify == CENTER_TEXT ? -th / 2:
                 VerJustify == BOTTOM_TEXT ? -th     : 0);
   if(Angle){
      const int NUMP = 5;
      bool ret = false;

      TPoint pp[NUMP];
      pp[0] = TPoint(r.left, r.top);
      pp[1] = TPoint(r.right, r.top);
      pp[2] = TPoint(r.right, r.bottom);
      pp[3] = TPoint(r.left, r.bottom);
      pp[4] = TPoint(r.left, r.top);

      TCoordSpace cs;
      cs.setPoints(pp, NUMP);
      cs.toAbsolute(wp->getMaxX(), wp->getMaxY());
      cs.RotateTo(getpf(0), Angle);
      cs.toScreen(wp);

      HRGN hRgn = CreatePolygonRgn(cs.getps(), NUMP, ALTERNATE);
      RECT rbox;
      GetRgnBox(hRgn, &rbox);
      if(PtInRect(rbox, p))
         ret = PtInRegion(hRgn, p.x, p.y);
      DeleteObject(hRgn);
      //cvs->Polyline(cs.getps(), NUMP - 1);
      return ret;
   }// if
   //cvs->FrameRect(r);
   return PtInRect(r, p);
}// TBGIText::Hit
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
bool __fastcall TBGIText::Select(TWorkPlace *wp, TRect r, TShiftState Shift){

   int sx, sy;

   toScreen(wp);
   sx = getps(0).x;
   sy = getps(0).y;

   if((sx >= r.left) && (sx <= r.right)  &&
      (sy >= r.top)  && (sy <= r.bottom) ){
      SetSelect(true);
   }/* if */
   else{
      if(!Shift.Contains(ssShift)){
         SetSelect(false);
      }/* if */
   }/* elif */

   return GetSelect();
}// TBGIText::Select
// ----------------------------------------------------------------------------
int __fastcall TBGIText::MouseDown(TWorkPlace *wp, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y){

   if(Editing()){
      switch(pc){
        case cMove:
           // переместить ГП
           if (Shift.Contains(ssLeft)){
             MoveTo(X / wp->getMaxX(), Y / wp->getMaxY());
             D(printf("\n TextBGI: Start Moved from: %d, %d", X, Y));
           }/* if */
           else
              return 1;
           break;
        case cZoom:
           // увеличить ГП
           break;
        case cEdit:
           // изменить какие-то свойства ГП
           break;
      }/* switch */
   }// if

   if(Creation()){
      // ГП создаётся
      if (Shift.Contains(ssLeft)){
         if(Op == Nop){
            Op = Start;
            SetStartXY(X / wp->getMaxX(), Y / wp->getMaxY());
            D(printf("\n TextBGI: Start created from: %d, %d", X, Y));
         }/* if */
         else{
            Op = Nop;
            return -1;
         }// elif
      }/* if */

      if (Shift.Contains(ssRight)){
         if(Op == Start){
            Op = Nop;
            D(printf("\n TextBGI: Stop created to: %d, %d", X, Y));
            return 1;
         }/* if */
      }/* if */
   }/* if */
   return 0;
};
// ----------------------------------------------------------------------------
int __fastcall TBGIText::MouseMove(TWorkPlace *wp, TObject *Sender, TShiftState Shift, int X, int Y){

   if(Editing()){
      switch(pc){
        case cMove:
           if (Shift.Contains(ssLeft)){
             MoveTo(X / wp->getMaxX(), Y / wp->getMaxY());
             D(printf("\n BGIText: Continued Moved to: %d, %d", X, Y));
           }/* if */
           break;
        case cZoom:
           // увеличить ГП
           break;
        case cEdit:
           // изменить какие-то свойства ГП
           break;
      }/* switch */
   }// if

   if(Creation()){
      // ГП создаётся
      if(Op == Start){
        if (Shift.Contains(ssLeft)){
           SetStartXY(X / wp->getMaxX(), Y / wp->getMaxY());
        }/* if */
        if (Shift.Contains(ssAlt)){
           Angle = dAngleText = getpf(0).angleto(TPointf(X / wp->getMaxX(), Y / wp->getMaxY()));
        }/* if */
        D(printf("\n BGIText: Continued Create to: %d, %d", X, Y));
      }/* if */
   }/* if */
   return 0;
};
#endif
// ----------------------------------------------------------------------------
__fastcall TBGIText::TBGIText(){
          newSize(1);
          SetType(tBGIText);
          SetStartXY(0.0, 0.0);
          Color = (TColor)0;
          pStr = NULL;
          lenStr = 0;
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

TBGIText::~TBGIText(){
   delete pStr;
};

__fastcall TBGIText::TBGIText(double ssX, double ssY,
      TColor sColor){
         newSize(1);
         SetType(tBGIText);
         SetStartXY(ssX, ssY);
         Color = sColor;
         pStr = NULL;
         lenStr = 0;
         Smoothing(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

AnsiString __fastcall TBGIText::GetName(void){return "текст BGI";};

#ifdef _EDITOR_

TBGIText& __fastcall TBGIText::operator=(TBGIText& R){
   if(this != &R){
      newSize(R.getSize());
      for(int i = 0;i < getSize();i++)
         getpf(i) = R.getpf(i);
      SetType(R.GetType());
      Color = R.Color;
      SetFont(R.BGIFont);
      SetDirection(R.Direction);
      SetJustify(R.HorJustify, R.VerJustify);
      SetSize(R.Size);
      SetExt(R.Sda, R.Mltx, R.Mlty);
      SetSS(R.SSX, R.SSY);
      SetAngle(R.Angle);
      SetText(R.GetText());
   }/* if */
   return *this;
};

TBaseGP* __fastcall TBGIText::Copy(CopyGEMode m){
   TBGIText *l = new TBGIText;
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

int __fastcall TBGIText::RotateTo(TPointf cp, double ang){
   Op = Start;
   TBaseGP::RotateTo(cp, ang);
   Angle += ang;
   Op = Stop;
   return 0;
};

int __fastcall TBGIText::Zoom(double Zk){
   SetStartXY(GetStartX() * Zk, GetStartY() * Zk);
   SSX = SSX / Zk + 0.5;
   SSY = SSY / Zk + 0.5;
   return 0;
};

#endif



