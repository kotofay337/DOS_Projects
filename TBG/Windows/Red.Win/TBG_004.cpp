/* TBG_004.CPP
*
*   TBG v7.0
*   25-09-2001 8:33 am msk
*
*/

#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#include "Screen.h"
#endif

#include "tbg.hpp"
#include "tbg_tc.h"

// --- Rectangle --------------------------------------------------------------
// ----------------------------------------------------------------------------
bool TRect2p::Load(char *rbuf){
   bool ret;
   char Name[32];
   double sX, sY;
   char *s, *tok = " ";

   ret = sscanf(rbuf, "%s %u 0x%x %u",
                   &Name,
                   &Width,
                   &Color,
                   &npts) == 4 ? true : false;

   s = strtok(rbuf, tok);
   for(unsigned j = 0;j < 4;j++)
      s = strtok(NULL, tok);

   for(unsigned i = 0;ret == true && i < npts;i++){
      ret = sscanf(s, " %lf", &sX) == 1 ? true : false;
      s = strtok(NULL, tok);
      if(ret){
         ret = sscanf(s, " %lf", &sY) == 1 ? true : false;
         s = strtok(NULL, tok);
      }/* if */
      if(ret){
         pts[i].moveto(sX, sY);
      }/* if */
   }/* for */

   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void TRect2p::Save(FILE *f){

   fprintf(f, "FrameRectangle");
   fprintf(f, " %u", Width);
   fprintf(f, " 0x%x", Color);
   fprintf(f, " %u", npts);
   for(unsigned i = 0;i < npts;i++){
      fprintf(f, " %lf", pts[i].getx());
      fprintf(f, " %lf", pts[i].gety());
   }/* for */
   fprintf(f, "\n");
   SaveLE(f);
};
#endif
// ----------------------------------------------------------------------------
void TRect2p::Show(TCanvas *cvs){
   unsigned i;
   cvs->Pen->Color = Color;
   cvs->Pen->Width = Width;
   cvs->Pen->Style = psSolid;

   if(wp == NULL)
      wp = new TPoint[npts + 1];

   for(i = 0;i < npts;i++){
     wp[i].x = pts[i].getx() * SizeScreenX + 0.5;
     wp[i].y = pts[i].gety() * SizeScreenY + 0.5;
   }/* for */
   wp[npts].x = pts[0].getx() * SizeScreenX + 0.5;
   wp[npts].y = pts[0].gety() * SizeScreenY + 0.5;

   cvs->Polyline(wp, npts);
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
bool TRect2p::Hit(POINT p){
   bool ret = false;
   int sX, sY, eX, eY;
   unsigned i;

   for(i = 0;i < npts - 1;i++){
     sX = pts[i].getx() * SizeScreenX + 0.5;
     sY = pts[i].gety() * SizeScreenY + 0.5;
     eX = pts[i+1].getx() * SizeScreenX + 0.5;
     eY = pts[i+1].gety() * SizeScreenY + 0.5;
     ret = Cross(sX, sY, eX, eY, p.x, p.y, Width > 0 ? Width : 1) == 1 ? true : false;
     if(ret){
        break;
     }/* if */
   }/* for */
   if(!ret){
     sX = pts[i].getx() * SizeScreenX + 0.5;
     sY = pts[i].gety() * SizeScreenY + 0.5;
     eX = pts[0].getx() * SizeScreenX + 0.5;
     eY = pts[0].gety() * SizeScreenY + 0.5;
     ret = Cross(sX, sY, eX, eY, p.x, p.y, Width > 0 ? Width : 1) == 1 ? true : false;
   }/* if */
   return ret;
}// TRect2p::Hit
// ----------------------------------------------------------------------------
bool TRect2p::Select(POINT p, TShiftState Shift){
   bool hit = Hit(p);

   if(Shift.Contains(ssShift)){
      if(hit){
         SetSelect(!GetSelect());
      }/* if */
   }/* if */
   else{
      if((hit) && (GetSelect() == true)){
         SetSelect(false);
      }/* if */
      else{
         SetSelect(hit);
      }/* elif */
   }/* elif */
   return GetSelect();
}// TRect2p::Select
// ----------------------------------------------------------------------------
bool TRect2p::Select(TRect r, TShiftState Shift){
   int x, y;
   unsigned i;
   bool h = true;

   for(i = 0;(i < npts) && h;i++){
      x = pts[i].getx() * SizeScreenX + 0.5;
      y = pts[i].gety() * SizeScreenY + 0.5;
      h = ((x >= r.left) && (x <= r.right) &&
           (y >= r.top)  && (y <= r.bottom));
   }/* for */

   if(h){
      SetSelect(true);
   }/* if */
   else{
      if(!Shift.Contains(ssShift)){
         SetSelect(false);
      }/* if */
   }/* elif */

   return GetSelect();
}// TRect2p::Select
// ----------------------------------------------------------------------------
void TRect2p::ShowSelect(TCanvas *cvs, SSMode m){
   int X, Y;

   if((GetSelect() == true) && (m != SSSelect) && (m != SSIndicate)){
      for(unsigned i = 0;i < npts;i++){
        X = pts[i].getx() * SizeScreenX + 0.5;
        Y = pts[i].gety() * SizeScreenY + 0.5;
        ShowCoordSelector(cvs, X, Y);
      }/* for */
   }/* if */

   if(m == SSSelect){
      X = pts[0].getx() * SizeScreenX + 0.5;
      Y = pts[0].gety() * SizeScreenY + 0.5;
      ShowSelector(cvs, X, Y, GetSelect());
   }/* if */

   if(m == SSIndicate){
     cvs->Pen->Mode = pmXor;
     Show(cvs);
     cvs->Pen->Mode = pmCopy;
   }/* if */
}/* ShowSelect */
// ----------------------------------------------------------------------------
int TRect2p::MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y){
   if(Creation()){
      // ГП создаётся
      if (Shift.Contains(ssLeft)){
         if(Op == Nop){
            Op = Start;
            SetStartXY(X, Y);
            SetEndXY(X, Y);
            D(printf("\n Rect: Start created from: %d, %d", X, Y));
         }/* if */
         else{
            Op = Nop;
            return -1;
         }// elif
      }/* if */

      if (Shift.Contains(ssRight)){
         if(Op == Start){
            Op = Nop;
            D(printf("\n Rect: Stop created to: %d, %d", X, Y));
            return 1;
         }/* if */
      }/* if */
   }/* if */
   return 0;
};
// ----------------------------------------------------------------------------
int TRect2p::MouseMove(TObject *Sender, TShiftState Shift, int X, int Y){
   if(Creation()){
      // ГП создаётся
      if(Op == Start){
        //TCanvas *Canvas = ((TForm*)Sender)->Canvas;
        //Canvas->Pen->Mode = pmNotXor;
        //Show(Canvas);
        SetEndXY(X, Y);
        //Show(Canvas);
        //Canvas->Pen->Mode = pmCopy;
        D(printf("\n Rect: Continued Create to: %d, %d", X, Y));
      }/* if */
   }/* if */
   return 0;
};
#endif
// ----------------------------------------------------------------------------
TRect2p::TRect2p(){  
          npts = 4;
          pts = new TPointf[npts];
          wp = NULL;
          Type = tFrameRect;
          SetStartXY(0.0, 0.0);
          SetEndXY(0.0, 0.0);
          Color = (TColor)0;
          Width = 1;
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

TRect2p::TRect2p(double ssX, double ssY,
      double seX, double seY,
      TColor sColor, unsigned char sWidth){
          npts = 4;
          pts = new TPointf[npts];
          wp = NULL;
         Type = tFrameRect;
         SetStartXY(ssX, ssY);
         SetEndXY(seX, seY);
         Color = sColor;
         Width = sWidth == 0 ? 1 : sWidth;
         Smoothing(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

TRect2p::~TRect2p(){  
   if(wp) 
      delete[] wp; 
   delete[] pts;
};

 AnsiString TRect2p::GetName(void){return "прямоугольник";};

void TRect2p::SetStartXY(int ssX, int ssY){
   pts[0].moveto(ssX / SizeScreenX, ssY / SizeScreenY);

   pts[1].moveto(pts[1].getx(), ssY / SizeScreenY);
   pts[3].moveto(ssX / SizeScreenX, pts[3].gety());
};
void TRect2p::SetEndXY(int seX, int seY){
   pts[2].moveto(seX / SizeScreenX, seY / SizeScreenY);

   pts[1].moveto(seX / SizeScreenX, pts[1].gety());
   pts[3].moveto(pts[3].getx(),     seY / SizeScreenY);
};

void TRect2p::SetStartXY(double ssX, double ssY){
   pts[0].moveto(ssX, ssY);
   pts[1].moveto(pts[1].getx(), ssY);
   pts[3].moveto(ssX, pts[3].gety());
};

void TRect2p::SetEndXY(double seX, double seY){
   pts[2].moveto(seX, seY);
   pts[1].moveto(seX, pts[1].gety());
   pts[3].moveto(pts[3].getx(), seY);
};

double TRect2p::GetStartX(void){ return pts[0].getx(); };
double TRect2p::GetStartY(void){ return pts[0].gety(); };

double TRect2p::GetEndX(void){ return pts[npts - 1].getx(); };
double TRect2p::GetEndY(void){ return pts[npts - 1].gety(); };

#ifdef _EDITOR_

int TRect2p::GetCoordCnt(void){return npts;};
   
TPointf *TRect2p::GetCoordPtr(int i){return (i >= 0) && (i < GetCoordCnt()) ? pts + i : NULL;};

void TRect2p::MirrorX(double ax){
   for(unsigned i = 0;i < npts;i++){
      pts[i].mirrorx(ax);
   }/* for */
};
   
void TRect2p::MirrorY(double ay){
   for(unsigned i = 0;i < npts;i++){
      pts[i].mirrory(ay);
   }/* for */
};
   
TRect2p& TRect2p::operator=(TRect2p& R){
   Type = R.Type;
   for(unsigned i = 0;i < npts;i++){
      pts[i] = R.pts[i];
   }/* for */
   Color = R.Color;
   Width = R.Width;
   TBaseGP::Smoothing(R.Smoothable());
   return *this;
};

TBaseGP* TRect2p::Copy(CopyGEMode m){
   TRect2p *l = new TRect2p;
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

int TRect2p::RotateTo(TPoint cp, double ang){
   Op = Start;
   TPointf c;
   c.moveto(cp.x / SizeScreenX, cp.y / SizeScreenY);
   for(unsigned i = 0;i < npts;i++){
      pts[i].rotate(ang, c);
   }/* for */
   Op = Stop;
   return 0;
};

int TRect2p::Zoom(double Zk){
   for(unsigned i = 0;i < npts;i++){
      pts[i].zoom(Zk);
   }/* for */
   return 0;
};
   
int TRect2p::MoveTo(double toX, double toY, bool m){  
   Op = Start;
   for(unsigned i = 0;i < npts;i++){
      pts[i].moveto(pts[i].getx() - toX, pts[i].gety() - toY);
   }/* for */
   Op = Stop;
   return 0;
};
#endif
void TRect2p::Smoothing(bool to){
   TBaseGP::Smoothing(to && (Width == 1) && ((unsigned long)Color == 0UL || ((unsigned long)Color & 0x00FFFFFF) == 0x005a5a5a));
};

 