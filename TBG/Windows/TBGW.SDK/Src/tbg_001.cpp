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

#include <math.h>
#include "tbg.hpp"
#include "tbg_tc.h"
#pragma hdrstop

/*
class KGPen
{
public:
	DWORD    m_cycle[4];
	LOGBRUSH m_logbrush;
	HPEN     m_hPen;

	KGPen()
	{
		m_cycle[0] = 20;
		m_cycle[1] = 15;
		m_cycle[2] = 10;
		m_cycle[3] =  5;

		m_logbrush.lbStyle = BS_SOLID;
		m_logbrush.lbColor = RGB(0, 0, 0xFF);
		m_logbrush.lbHatch = 0;

		m_hPen = NULL;
	}

	~KGPen(void)
	{
		if ( m_hPen )
			DeleteObject(m_hPen);
	}

	void SetPen(int style, int width);
	void DrawLine(HDC hDC, int x0, int y0, int x1, int y1);
};

void KGPen::SetPen(int style, int width)
{
	if ( m_hPen )
		DeleteObject(m_hPen);

	if ( ( style & PS_STYLE_MASK ) == PS_USERSTYLE )
		m_hPen = ExtCreatePen(PS_GEOMETRIC | style, width, & m_logbrush, 4, m_cycle);
	else
		m_hPen = ExtCreatePen(PS_GEOMETRIC | style, width, & m_logbrush, 0, NULL);
}

void KGPen::DrawLine(HDC hDC, int x0, int y0, int x1, int y1)
{
	if ( m_hPen )
	{
		SelectObject(hDC, m_hPen);
		SetBkColor(hDC, RGB(0, 0xFF, 0));
		SetBkMode(hDC, OPAQUE);

		// draw style line
		Line(hDC, x0, y0, x1, y1);

		SetBkColor(hDC, RGB(0xFF, 0xFF, 0xFF));

		SelectObject(hDC, GetStockObject(BLACK_PEN));
	}
}
*/
// --- Линия ------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool __fastcall TLine::Load(char *rbuf){
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
void __fastcall TLine::SaveAsBase(FILE *f){
   if(Width != 1)       fprintf(f, " -width %u", Width);
   if(Color != clBlack) fprintf(f, " -fgc %x", Color);
   TBaseGP::SaveAsBase(f);
};
// ----------------------------------------------------------------------------
void __fastcall TLine::Save(FILE *f){
   fprintf(f, "SimpleLine");
   SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
void __fastcall TLine::Show(TWorkPlace *wp){
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;

//   Gdiplus::Graphics *graphics = new Gdiplus::Graphics(wp->getCanvas()->Handle);
   Gdiplus::Graphics *graphics = wp->getGraphics();
   if(graphics){
      toScreen(wp);
      graphics->SetSmoothingMode(wp->getEditProcessing() ? Gdiplus::SmoothingModeHighSpeed : 
                                                           Gdiplus::SmoothingModeAntiAlias);
      Gdiplus::Pen pen(Gdiplus::Color(255, GetRValue(OutColor), GetGValue(OutColor), GetBValue(OutColor)));
      pen.SetWidth(Width);
      int x, y, xe, ye;
      x = getps(0).x;
      y = getps(0).y;
      xe = getps(1).x;
      ye = getps(1).y;
      graphics->DrawLine(&pen, x, y, xe, ye);
//      delete graphics;
   }// if
/*

   TCanvas *cvs = wp->getCanvas();
   cvs->Pen->Color = Color;
   cvs->Pen->Width = Width;
   cvs->Pen->Style = psSolid;
   toScreen(wp);

   if(Width <= 1 && wp->getEditProcessing() == false){
      WuLine(cvs, getps(0).x, getps(0).y, getps(1).x, getps(1).y, 255, 255, 8, (unsigned long)Color);
   }
   else{
      Line(cvs->Handle, getps(0).x, getps(0).y, getps(1).x, getps(1).y);
      //KGPen gpen;
		//gpen.m_logbrush.lbColor = RGB(0xFF,0,0);
		//gpen.m_logbrush.lbStyle = BS_HATCHED;
		//gpen.m_logbrush.lbHatch = HS_DIAGCROSS;
		//gpen.SetPen(PS_DASH | PS_ENDCAP_ROUND, Width);
		//gpen.DrawLine(cvs->Handle, getps(0).x, getps(0).y, getps(1).x, getps(1).y);
   }//elif
*/
};
// ----------------------------------------------------------------------------
void __fastcall TLine::ShowWithRotate(TWorkPlace* wp, TPointf p, double a){
   TPointf *sp = new TPointf[getSize()];
   if(sp){
      int sz = getSize();
      for(int i = 0;i < sz;i++)
         sp[i] = getpf(i);
      for(int i = 0;i < sz;i++)
         getpf(i).rotate(a, p);
      Show(wp);
      for(int i = 0;i < sz;i++)
         getpf(i) = sp[i];
      delete[] sp;
   }// if
};
// ----------------------------------------------------------------------------
void __fastcall TLine::ShowWithColor(TWorkPlace* wp, TColor cl){
   OutColor = cl;
   Show(wp);
};
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

int __fastcall Cross (int x1, int y1, int x2, int y2, int Xp, int Yp, int SelDepth) {
  float Xw, Yw;
  float Xc, Yc;
  float d, t, RS, RS2;

  Xw = x2 - x1;
  Yw = y2 - y1;
  RS = Xw * Xp + Yw * Yp;
  RS2 = RS - Xw * x1 - Yw * y1;
  t = Xw * Xw + Yw * Yw;
  if ( t != 0 ) t = RS2 / t; else t = RS2;
  Xc = t * Xw + x1;
  Yc = t * Yw + y1;
  if ( t <= 0 ) return 0;
  if ( t >= 1 ) return 0;
  d = sqrt ( ( Xc - Xp ) * ( Xc - Xp ) + ( Yc - Yp ) * ( Yc - Yp ) );
  return d < ((SelDepth + 4) / 2);
}

/*
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
*/
// ----------------------------------------------------------------------------
bool __fastcall TLine::Hit(TWorkPlace* wp, TPoint p){
   int sX, sY, eX, eY;
   toScreen(wp);
   sX = getps(0).x;
   sY = getps(0).y;
   eX = getps(getMaxInd()).x;
   eY = getps(getMaxInd()).y;
   return Cross(sX, sY, eX, eY,
                p.x, p.y, Width > 0 ? Width : 1) == 1 ? true : false;
}// TLine::Hit
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
int __fastcall TLine::MouseDown(TWorkPlace *wp, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y){

   if(Editing()){
      switch(pc){
        case cMove:
           // переместить ГП
           if (Shift.Contains(ssLeft)){
             MoveTo(X / wp->getMaxX(), Y / wp->getMaxY());
             Update = true;
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
            SetStartXY(X / wp->getMaxX(), Y / wp->getMaxY());
            SetEndXY(X / wp->getMaxX(), Y / wp->getMaxY());
            Update = true;
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
            Update = true;
            D(printf("\n Line: Stop created to: %d, %d", X, Y));
            return 1;
         }/* if */
      }/* if */
   }/* if */
   return 0;
};
// ----------------------------------------------------------------------------
int __fastcall TLine::MouseMove(TWorkPlace *wp, TObject *Sender, TShiftState Shift, int X, int Y){

   if(Editing()){
      switch(pc){
        case cMove:
           if (Shift.Contains(ssLeft)){
             MoveTo(X / wp->getMaxX(), Y / wp->getMaxY());
             Update = true;
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
        SetEndXY(X / wp->getMaxX(), Y / wp->getMaxY());
        Update = true;
        D(printf("\n Line: Continued Create to: %d, %d", X, Y));
      }/* if */
   }/* if */
   return 0;
};
#endif
// ----------------------------------------------------------------------------
__fastcall TLine::TLine(){
   SetType(tLine);
   newSize(2);
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

__fastcall TLine::TLine(double ssX, double ssY,
      double seX, double seY,
      TColor sColor, unsigned sWidth){
   newSize(2);
   SetType(tLine);
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

AnsiString __fastcall TLine::GetName(void){
   return "отрезок";
};

#ifdef _EDITOR_

TLine& __fastcall TLine::operator=(TLine& R){
   SetType(R.GetType());
   newSize(getSize());
   setOffset(R.getOffset());
   for(int i = 0;i < getSize();i++)
      getpf(i) = R.getpf(i);
   OutColor = Color = R.Color;
   Width = R.Width;
   TBaseGP::Smoothing(R.Smoothable());
   return *this;
};

TBaseGP* __fastcall TLine::Copy(CopyGEMode m){
   TLine *l = new TLine;
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
void __fastcall TLine::Smoothing(bool to){
//   TBaseGP::Smoothing(to && (Width == 1) && ((unsigned long)Color == 0UL || ((unsigned long)Color & 0x00FFFFFF) == 0x005a5a5a));
   TBaseGP::Smoothing(to && (Width == 1));
   Update = true;
};



