/* TBG_001.CPP
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

// --- Линия ------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool TLine::Load(char *rbuf){
   bool ret;
   char Name[32];
   double sX, sY, eX, eY;
   ret = sscanf(rbuf, "%s %lf %lf %lf %lf %u 0x%x",
                   &Name,
                   &sX,
                   &sY,
                   &eX,
                   &eY,
                   &Width,
                   &Color) == 7 ? true : false;
   pts[0].moveto(sX, sY);
   pts[1].moveto(eX, eY);
   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void TLine::Save(FILE *f){

   fprintf(f, "Line");
   fprintf(f, " %lf", pts[0].getx());
   fprintf(f, " %lf", pts[0].gety());
   fprintf(f, " %lf", pts[1].getx());
   fprintf(f, " %lf", pts[1].gety());
   fprintf(f, " %u", Width);
   fprintf(f, " 0x%x\n", Color);

   SaveLE(f);
};
#endif
// ----------------------------------------------------------------------------
void TLine::Show(TCanvas *cvs){
   cvs->Pen->Color = Color;
   cvs->Pen->Width = Width;
//   cvs->Pen->Width = (Width / ((640 + 480.) / 2.)) * ((SizeScreenX + SizeScreenY) / 2.);
   cvs->Pen->Style = psSolid;
   int sX, sY, eX, eY;
   sX = pts[0].getx() * SizeScreenX + 0.5;
   sY = pts[0].gety() * SizeScreenY + 0.5;
   eX = pts[1].getx() * SizeScreenX + 0.5;
   eY = pts[1].gety() * SizeScreenY + 0.5;
   
   if(Smoothable()){
      DrawWuLine(cvs, sX, sY, eX, eY, 255, 255, 8, (unsigned long)Color);
   }
   else{

//      cvs->MoveTo(sX * SizeScreenX + 0.5, sY * SizeScreenY + 0.5);
//      cvs->LineTo(eX * SizeScreenX + 0.5, eY * SizeScreenY + 0.5);

      Line(cvs->Handle, sX, sY, eX, eY);

/*
      PIPELINE ppl;
      ppl.xs = sX * SizeScreenX + 0.5;
      ppl.ys = sY * SizeScreenY + 0.5;
      ppl.xe = eX * SizeScreenX + 0.5;
      ppl.ye = eY * SizeScreenY + 0.5;
      ppl.DO = 1;
      ppl.O = 2;
      ppl.S = 10;
      ppl.C1 = 0;
      ppl.C2 = 15;
      pipeline(cvs->Handle, &ppl, -2, SOLID_LINE, NORM_WIDTH);
*/
   }//elif
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
/*
*   Cross
*
*   Вычисляет лежит ли точка(_x2, _y2) на отрезке заданном(_x0, _y0, _x1, _y1)
*   в пределах толщины точки заданной _z, если лежит то возвращает "1"
*   иначе "0".
*
*   Мат, обеспечение: Войнов А.
*   Кодирование: Чумак А.
*
*   11-FEB-2002
*/
int Cross(int _x0, int _y0,
          int _x1, int _y1,
          int _x2, int _y2, int _z){

   float x0, y0, x1, y1, x2, y2, z, k, a;

   if( (_x2 >= min(_x0, _x1)) && (_y2 >= min(_y0, _y1)) &&
       (_x2 <= max(_x0, _x1)) && (_y2 <= max(_y0, _y1)) ){

      x0 = _x0; y0 = _y0;
      x1 = _x1; y1 = _y1;
      x2 = _x2; y2 = _y2;
      z  = _z;

      if((x0 != x1) && (y0 != y1)){
         k = (y0 - y1)/(x0 - x1);
         a = y0 - k * x0;
         if(k != 0){
            float t1 = k * x2 + a;
            if((((t1 - z) <= y2) &&
                ((t1 + z) >= y2))){
               return 1;
            }/*if  */
            else{
               float t2 = (y2 - a) / k;
               if(((t2 - z) <= x2) &&
                  ((t2 + z) >= x2) ){
                  return 1;
               }/*if  */
               else{
                  return 0;
               }/* elif */
            }/* elif */
         }/* if */
      }/* if */

      if((x0 != x1) && (y0 == y1)){
        if( ((y0 - z) <= y2) &&
            ((y0 + z) >= y2) ){
           return 1;
        }/* if */
        else{
           return 0;
        }/* elif */
      }/* if */

      if((x0 == x1) && (y0 != y1)){
        if(((x0 - z) <= x2) &&
           ((x0 + z) >= x2) ){
           return 1;
        }/* if */
        else{
           return 0;
        }/* elif */
      }/* if */

   }/* if */
   return 0;
}/* Cross */
// ----------------------------------------------------------------------------
bool TLine::Hit(POINT p){
   int sX, sY, eX, eY;
   sX = pts[0].getx() * SizeScreenX + 0.5;
   sY = pts[0].gety() * SizeScreenY + 0.5;
   eX = pts[1].getx() * SizeScreenX + 0.5;
   eY = pts[1].gety() * SizeScreenY + 0.5;
   return Cross(sX, sY, eX, eY,
                p.x, p.y, Width > 0 ? Width : 1) == 1 ? true : false;
}// TLine::Hit
// ----------------------------------------------------------------------------
bool TLine::Select(POINT p, TShiftState Shift){
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
}// TLine::Select
// ----------------------------------------------------------------------------
bool TLine::Select(TRect r, TShiftState Shift){
   int sx, sy, ex, ey;
   sx = pts[0].getx() * SizeScreenX + 0.5;
   sy = pts[0].gety() * SizeScreenY + 0.5;
   ex = pts[1].getx() * SizeScreenX + 0.5;
   ey = pts[1].gety() * SizeScreenY + 0.5;

   if((sx >= r.left) && (sx <= r.right)  &&
      (sy >= r.top)  && (sy <= r.bottom) && 
      (ex >= r.left) && (ex <= r.right)  &&
      (ey >= r.top)  && (ey <= r.bottom) ){
      SetSelect(true);
   }/* if */
   else{
      if(!Shift.Contains(ssShift)){
         SetSelect(false);
      }/* if */
   }/* elif */

   return GetSelect();
}// TLine::Select
// ----------------------------------------------------------------------------
void TLine::ShowSelect(TCanvas *cvs, SSMode m){
   int sX, sY, eX, eY;
   sX = pts[0].getx() * SizeScreenX + 0.5;
   sY = pts[0].gety() * SizeScreenY + 0.5;
   eX = pts[1].getx() * SizeScreenX + 0.5;
   eY = pts[1].gety() * SizeScreenY + 0.5;

   if((GetSelect() == true) && (m != SSSelect) && (m != SSIndicate)){
      int x, y;

      cvs->Pen->Color = clBlack;
      cvs->Pen->Width = 1;
      cvs->Brush->Color = clLime;
      cvs->Brush->Style = bsSolid;
      
      x = sX;
      y = sY;
      cvs->Ellipse(x - 4, y - 4, x + 4, y + 4);
   
      x = eX;
      y = eY;
      cvs->Ellipse(x - 4, y - 4, x + 4, y + 4);
   }/* if */

   if(m == SSSelect){
      int x, y;

      cvs->Pen->Color = clTeal;
      cvs->Pen->Width = 1;
      cvs->Brush->Color = GetSelect() ? (TColor)0x008270D6 : (TColor)0x0060D6FC;
      cvs->Brush->Style = bsSolid;
      
      x = sX;
      y = sY;

      Windows::TPoint points[4];
      points[0] = Point(x, y);
      points[1] = Point(x - 6, y - 6);
      points[2] = Point(x + 6, y - 6);
      points[3] = Point(x, y);
      cvs->Polygon(points, 3);
/*
      cvs->Rectangle(x - 4, y - 4, x + 4, y + 4);
   
      x = eX * SizeScreenX + 0.5;
      y = eY * SizeScreenY + 0.5;
      cvs->Rectangle(x - 4, y - 4, x + 4, y + 4);
*/      
   }/* if */

   if(m == SSIndicate){
     cvs->Pen->Mode = pmXor;
     Show(cvs);
     cvs->Pen->Mode = pmCopy;
   }/* if */
}/* ShowSelect */
// ----------------------------------------------------------------------------
int TLine::MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y){

   if(Editing()){
      switch(pc){
        case cMove:
           // переместить ГП
           if (Shift.Contains(ssLeft)){
             MoveTo(X / SizeScreenX, Y / SizeScreenY);
             D(printf("\n Line: Start Moved from: %d, %d", X, Y));
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
            SetStartXY(X, Y);
            SetEndXY(X, Y);
            D(printf("\n Line: Start created from: %d, %d", X, Y));
         }/* if */
         else{
            Op = Nop;
            return -1;
         }// elif
      }/* if */

      if (Shift.Contains(ssRight)){
         if(Op == Start){
            Op = Nop;
            D(printf("\n Line: Stop created to: %d, %d", X, Y));
            return 1;
         }/* if */
      }/* if */
   }/* if */
   return 0;
};
// ----------------------------------------------------------------------------
int TLine::MouseMove(TObject *Sender, TShiftState Shift, int X, int Y){

   if(Editing()){
      switch(pc){
        case cMove:
           if (Shift.Contains(ssLeft)){
             MoveTo(X / SizeScreenX, Y / SizeScreenY);
             D(printf("\n Line: Continued Moved to: %d, %d", X, Y));
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
        //TCanvas *Canvas = ((TForm*)Sender)->Canvas;
        //Canvas->Pen->Mode = pmNotXor;
        //Show(Canvas);
        SetEndXY(X, Y);
        //Show(Canvas);
        //Canvas->Pen->Mode = pmCopy;
        D(printf("\n Line: Continued Create to: %d, %d", X, Y));
      }/* if */
   }/* if */
   return 0;
};
#endif
// ----------------------------------------------------------------------------
TLine::TLine(){  Type = tLine;
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

TLine::TLine(double ssX, double ssY,
      double seX, double seY,
      TColor sColor, unsigned char sWidth){
         Type = tLine;
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

 AnsiString TLine::GetName(void){return "отрезок";};

 void TLine::SetStartXY(int ssX, int ssY){
   pts[0].moveto(ssX / SizeScreenX, ssY / SizeScreenY);
};
 void TLine::SetEndXY(int seX, int seY){
   pts[1].moveto(seX / SizeScreenX, seY / SizeScreenY);
};

 void TLine::SetStartXY(double ssX, double ssY){
   pts[0].moveto(ssX, ssY);
};

 void TLine::SetEndXY(double seX, double seY){
   pts[1].moveto(seX, seY);
};

 double TLine::GetStartX(void){ return pts[0].getx(); };
 double TLine::GetStartY(void){ return pts[0].gety(); };

 double TLine::GetEndX(void){ return pts[1].getx(); };
 double TLine::GetEndY(void){ return pts[1].gety(); };

#ifdef _EDITOR_

 int TLine::GetCoordCnt(void){return sizeof pts / sizeof pts[0];};
   
 TPointf *TLine::GetCoordPtr(int i){return (i >= 0) && (i < GetCoordCnt()) ? &pts[i] : NULL;};

 void TLine::SwapCoord(void){
   pts[0].swapxy(pts[1]);
};

 void TLine::MirrorX(double ax){
   int numpts = GetCoordCnt();
   for(int i = 0;i < numpts;i++){
      pts[i].mirrorx(ax);
   }/* for */
};
   
 void TLine::MirrorY(double ay){
   int numpts = GetCoordCnt();
   for(int i = 0;i < numpts;i++){
      pts[i].mirrory(ay);
   }/* for */
};
   
 void TLine::SwapCoordX(void){
   pts[0].swapx(pts[1]);
};

 void TLine::SwapCoordY(void){
   pts[0].swapy(pts[1]);
};

TLine& TLine::operator=(TLine& R){
   Type = R.Type;
   pts[0] = R.pts[0];
   pts[1] = R.pts[1];
   Color = R.Color;
   Width = R.Width;
   TBaseGP::Smoothing(R.Smoothable());
   return *this;
};

TBaseGP* TLine::Copy(CopyGEMode m){
   TLine *l = new TLine;
   *l = *this;
   switch(m){
      case cmWithLogic:
         this->CopyLE(l);
         break;
      case cmMirror:
         l->SwapCoord();
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

int TLine::RotateTo(TPoint cp, double ang){
   Op = Start;
   TPointf c;
   c.moveto(cp.x / SizeScreenX, cp.y / SizeScreenY);
   pts[0].rotate(ang, c);
   pts[1].rotate(ang, c);
   Op = Stop;
   return 0;
};

int TLine::Zoom(double Zk){
   SetStartXY(GetStartX() * Zk, GetStartY() * Zk);
   SetEndXY(GetEndX() * Zk, GetEndY() * Zk);
   return 0;
};
   
int TLine::MoveTo(double toX, double toY, bool m){  
   Op = Start;
   if(m){
      double dX = GetEndX() - GetStartX(),
             dY = GetEndY() - GetStartY();
      SetStartXY(toX, toY);
      SetEndXY(toX + dX, toY + dY);
   }/* if */
   else{
      SetStartXY(GetStartX() - toX, GetStartY() - toY);
      SetEndXY  (GetEndX()   - toX, GetEndY()   - toY);
   }/* elif */
   Op = Stop;
   return 0;
};
#endif
void TLine::Smoothing(bool to){
   TBaseGP::Smoothing(to && (Width == 1) && ((unsigned long)Color == 0UL || ((unsigned long)Color & 0x00FFFFFF) == 0x005a5a5a));
};



 