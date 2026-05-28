/* TBG_008.CPP
*
*   TBG v7.0
*   13-05-2002 11:38 msk
*
*/

#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#include "Screen.h"
#endif

#include "tbg.hpp"
#include "tbg_tc.h"
#pragma hdrstop

// --- Pipeline ---------------------------------------------------------------
// ----------------------------------------------------------------------------
bool __fastcall TPipeLine::Load(char *rbuf){
   bool ret = false;

   char brkused, quoted, token[64];
   int i = 0,next = 0;

   // Read my name
   parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
         case kwDataPipe:{
               char t[32];
               int nx = 0;
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rSHORTINT, &DO);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rSHORTINT, &O);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rSHORTINT, &S);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rHEX, &Color1);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rHEX, &Color2);
            }
            break;
      }// switch
   }// while

   ret |= TPolyLine::Load(rbuf);

   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TPipeLine::Save(FILE *f){
   fprintf(f, "PipeLine");
   fprintf(f, " -datapipe \"");
   fprintf(f, " %hd", DO);
   fprintf(f, " %hd", O);
   fprintf(f, " %hd", S);
   fprintf(f, " %x", Color1);
   fprintf(f, " %x \"", Color2);
   TPolyLine::SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
void __fastcall TPipeLine::PipePaint(TCanvas *cvs, TPoint sp, TPoint ep, bool Mode, TColor color){
   static POINT pt[2048];
   static short int cpt = 0;
   short int x,y,dx,dy,s1,s2,e,swap,i;
   short int x1,y1,x2,y2, I;
   x1 = sp.x;
   y1 = sp.y;
   x2 = ep.x;
   y2 = ep.y;
   if(Mode){
      x = x1;
      y = y1;
      dx = abs(x2 - x1);
      dy = abs(y2 - y1);
      s1 = (x2 - x1) <  0 ? -1 :
           (x2 - x1) == 0 ?  0 : 1 ;
      s2 = (y2 - y1) <  0 ? -1 :
           (y2 - y1) == 0 ?  0 : 1 ;
      if(dy > dx){
         short int tmp = dx;
         dx = dy;
         dy = tmp;
         swap = 1;
      }/* if */
      else
         swap = 0;
      e = 2 * dy - dx;
      I = O;
      cpt = 0;
      pt[cpt].x = x;
      pt[cpt].y = y;
      cpt++;
      for(i = 1;i <= dx;i++){
         I++;
         if(I >= S){
            I = 0;
            pt[cpt].x = x;
            pt[cpt].y = y;
            cpt++;
            if(cpt >= (sizeof(pt) / sizeof(pt[0])))
               cpt--;
         }/* if */
         while(e >= 0){
            if(swap == 1)
               x += s1;
            else
               y += s2;
            e = e - 2 * dx;
         }/* while */
         if(swap == 1)
            y += s2;
         else
            x += s1;
         e = e + 2 * dy;
      }/* for */
      pt[cpt].x = x;
      pt[cpt].y = y;
      for(i = 0;i < cpt;i++){
         cvs->Pen->Color = i % 2 ? Color1 : Color2;
         cvs->Pen->Width = GetWidthLine();
         cvs->Pen->Style = psSolid;
         Line(cvs->Handle, pt[i].x, pt[i].y, pt[i+1].x, pt[i+1].y);
      }/* for */
      if(color == -1){
          O -= DO;
          if(O < 0){
             TColor t = Color1;
             Color1 = Color2;
             Color2 = t;
             O = S;
          }/* if */
      }/* if */
      else{
          O += DO;
          if(O >= S){
             TColor t = Color1;
             Color1 = Color2;
             Color2 = t;
             O = 0;
          }/* if */
      }/* elif */
      return;
   }/* if */
   else{
      cvs->Pen->Color = color;
      cvs->Pen->Width = GetWidthLine();
      cvs->Pen->Style = psSolid;
      Line(cvs->Handle, x1, y1, x2, y2);
      return;
   }/* elif */
}/* pipe */
// ----------------------------------------------------------------------------
void __fastcall TPipeLine::Show(TWorkPlace *wp){
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;
   TCanvas *cvs = wp->getCanvas();
   toScreen(wp);
   for(int i = 0;i < getMaxInd();i++){
      PipePaint(cvs, getps(i), getps(i + 1), Mode, GetColor());
   }/* for */
};
// ----------------------------------------------------------------------------
__fastcall TPipeLine::TPipeLine():TPolyLine(){
          SetType(tPipe);
          Color1 = clBlack;
          Color2 = clWhite;
          DO = 1;
          O = 1;
          S = 2;
          Mode = true;
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TPipeLine::TPipeLine(TPipeLine &s){
          SetType(tPipe);
          //*this = s;
          Smoothing(s.Smoothable());
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TPipeLine::TPipeLine(double ssX, double ssY,
      double seX, double seY,
      TColor sColor1, TColor sColor2,
      short int sDO,
      short int sO,
      short int sS,
      unsigned sWidth):TPolyLine(ssX, ssY, seX, seY, clBlack, sWidth){
         SetType(tPipe);
         Color1 = sColor1;
         Color2 = sColor2;
         DO = sDO;
         O = sO;
         S = sS;
         Mode = true;
         Smoothing(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

TPipeLine::~TPipeLine(){
};

AnsiString __fastcall TPipeLine::GetName(void){return "трубопровод";};

#ifdef _EDITOR_

TPipeLine& __fastcall TPipeLine::operator=(TPipeLine& R){
   *(TPolyLine*)this = (TPolyLine&)R;
   SetType(R.GetType());
   Color1 = R.Color1;
   Color2 = R.Color2;
   DO = R.DO;
   O = R.O;
   S = R.S;
   Mode = R.Mode;
   TBaseGP::Smoothing(R.Smoothable());
   return *this;
};

TBaseGP* __fastcall TPipeLine::Copy(CopyGEMode m){
   TPipeLine *l = new TPipeLine(*this);
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


 