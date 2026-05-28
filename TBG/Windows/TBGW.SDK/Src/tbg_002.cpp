/* TBG_002.CPP
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
bool __fastcall TPolyRect::Load(char *rbuf){
   bool ret = false;
   char brkused, quoted, token[32];
   int i = 0,next = 0;

   Width = 1;
   Color = clBlack;

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
                  Color = C;
            }
            break;
      }// switch
   }// while

   ret = TBaseGP::Load(rbuf);

   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TPolyRect::SaveAsBase(FILE *f){
   if(Width != 1)       fprintf(f, " -width %u", Width);
   if(Color != clBlack) fprintf(f, " -fgc %x", Color);
   TBaseGP::SaveAsBase(f);
};
// ----------------------------------------------------------------------------
void __fastcall TPolyRect::Save(FILE *f){
   fprintf(f, "PolyFrameRectangle");
   SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
void __fastcall TPolyRect::Show(TWorkPlace *wp){

   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;

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

   getpf(getMaxInd()) = getpf(0);
   toScreen(wp);

   if(Width <= 1 && wp->getEditProcessing() == false){
      WuPolyline(cvs, getps(), getMaxInd());
   }
   else{
      cvs->Polyline(getps(), getMaxInd());
   }//elif

};
// ----------------------------------------------------------------------------
bool __fastcall TPolyRect::Hit(TWorkPlace *wp, TPoint p){
   bool ret = false;
   int i;

   toScreen(wp);
   for(i = 0;i < getSize() - 1;i++){
     ret = Cross(getps(i).x, getps(i).y, getps(i + 1).x, getps(i + 1).y, p.x, p.y, Width > 0 ? Width : 1) == 1 ? true : false;
     if(ret){
        break;
     }/* if */
   }/* for */
   return ret;
}// TPolyRect::Hit
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
int __fastcall TPolyRect::MouseDown(TWorkPlace *wp, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y){
   if(Creation()){
      // ГП создаётся
      if (Shift.Contains(ssLeft)){
         if(Op == Nop){
            Op = Start;
            SetStartXY(X / wp->getMaxX(), Y / wp->getMaxY());
            SetEndXY(X / wp->getMaxX(), Y / wp->getMaxY());
            D(printf("\n PolyRect: Start created from: %d, %d", X, Y));
         }/* if */
         else{
            Op = Nop;
            return -1;
         }// elif
      }/* if */

      if (Shift.Contains(ssRight)){
         if(Op == Start){
            Op = Nop;
            if (Shift.Contains(ssCtrl)){
               this->SetHideState(true);
            }/* if */
            D(printf("\n PolyRect: Stop created to: %d, %d", X, Y));
            return 1;
         }/* if */
      }/* if */
   }/* if */
   return 0;
};
// ----------------------------------------------------------------------------
int __fastcall TPolyRect::MouseMove(TWorkPlace *wp, TObject *Sender, TShiftState Shift, int X, int Y){
   if(Creation()){
      // ГП создаётся
      if(Op == Start){
        SetEndXY(X / wp->getMaxX(), Y / wp->getMaxY());
        D(printf("\n PolyRect: Continued Create to: %d, %d", X, Y));
      }/* if */
   }/* if */
   return 0;
};
#endif
// ----------------------------------------------------------------------------
__fastcall TPolyRect::TPolyRect(){
          newSize(5);
          SetType(tPolyRect);
          SetStartXY(0.0, 0.0);
          SetEndXY(0.0, 0.0);
          Color = (TColor)0;
          Width = 1;
          Smoothing(false);
          SetHideState(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TPolyRect::TPolyRect(double ssX, double ssY,
      double seX, double seY,
      TColor sColor, unsigned sWidth){
         newSize(5);
         SetType(tPolyRect);
         SetStartXY(ssX, ssY);
         SetEndXY(seX, seY);
         Color = sColor;
         Width = sWidth == 0 ? 1 : sWidth;
         Smoothing(false);
         SetHideState(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

TPolyRect::~TPolyRect(){
};

AnsiString __fastcall TPolyRect::GetName(void){return "4-х угольник";};


void __fastcall TPolyRect::SetStartXY(double ssX, double ssY){
   getpf(0).moveto(ssX, ssY);
   getpf(4).moveto(ssX, ssY);
   getpf(1).moveto(getpf(1).getx(), ssY);
   getpf(3).moveto(ssX, getpf(3).gety());
};

void __fastcall TPolyRect::SetEndXY(double seX, double seY){
   getpf(2).moveto(seX, seY);
   getpf(1).moveto(seX, getpf(1).gety());
   getpf(3).moveto(getpf(3).getx(), seY);
};

double __fastcall TPolyRect::GetStartX(void){ return getpf(0).getx(); };
double __fastcall TPolyRect::GetStartY(void){ return getpf(0).gety(); };

double __fastcall TPolyRect::GetEndX(void){ return getpf(2).getx(); };
double __fastcall TPolyRect::GetEndY(void){ return getpf(2).gety(); };

#ifdef _EDITOR_

TPolyRect& __fastcall TPolyRect::operator=(TPolyRect& R){
   SetType(R.GetType());
   newSize(getSize());
   setOffset(R.getOffset());
   for(int i = 0;i < getSize();i++)
      getpf(i) = R.getpf(i);
   Color = R.Color;
   Width = R.Width;
   TBaseGP::Smoothing(R.Smoothable());
   SetHideState(R.GetHideState());
   return *this;
};

TBaseGP* __fastcall TPolyRect::Copy(CopyGEMode m){
   TPolyRect *l = new TPolyRect;
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
void __fastcall TPolyRect::Smoothing(bool to){
   TBaseGP::Smoothing(to && (Width == 1));
};



