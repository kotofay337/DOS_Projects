/* TBG_012.CPP
*
*   TBG v7.0
*   02-03-2003 13:51 msk
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

// --- TReperPoint ------------------------------------------------------------
// ----------------------------------------------------------------------------
bool __fastcall TReperPoint::Load(char *rbuf){
   return TBaseGP::Load(rbuf);
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TReperPoint::Save(FILE *f){
   fprintf(f, "ReperPoint");
   TBaseGP::SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
void __fastcall TReperPoint::Show(TWorkPlace *wp){
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
int __fastcall TReperPoint::MouseDown(TWorkPlace *wp, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y){
   if(Creation()){
      // ГП создаётся
      if (Shift.Contains(ssLeft)){
         if(Op == Nop){
            Op = Start;
            SetStartXY(X / wp->getMaxX(), Y / wp->getMaxY());
            D(printf("\n ReperPoint: Created to: %d, %d", X, Y));
            Op = Nop;
            return 1;
         }/* if */
      }/* if */
   }/* if */
   return 0;
};
// ----------------------------------------------------------------------------
int __fastcall TReperPoint::MouseMove(TWorkPlace *wp, TObject *Sender, TShiftState Shift, int X, int Y){
   return 0;
};
#endif
// ----------------------------------------------------------------------------
__fastcall TReperPoint::TReperPoint(){
          newSize(1);
          SetType(tReperPoint);
          SetStartXY(0.0, 0.0);
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TReperPoint::TReperPoint(TReperPoint &s){
          //*this = s;
          Smoothing(s.Smoothable());
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TReperPoint::TReperPoint(TPointf *p){
         newSize(1);
         SetType(tReperPoint);
         SetStartXY(p[0].getx(), p[0].gety());
         Smoothing(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

AnsiString __fastcall TReperPoint::GetName(void){return "реперная точка";};

#ifdef _EDITOR_

TReperPoint& __fastcall TReperPoint::operator=(TReperPoint& R){
   newSize(R.getSize());
   SetType(R.GetType());
   for(int i = 0;i < getSize();i++){
      getpf(i) = R.getpf(i);
   }/* for */
   TBaseGP::Smoothing(R.Smoothable());
   return *this;
};

TBaseGP* __fastcall TReperPoint::Copy(CopyGEMode m){
   TReperPoint *l = new TReperPoint(*this);
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



