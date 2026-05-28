/* TBG_015.CPP
*
*   TBG v7.0
*   3-06-2003 19:11 msk
*
*/

#include <math.h>

#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#include "Screen.h"
#endif

#include "tbg.hpp"
#include "tbg_tc.h"
#pragma hdrstop

// ----------------------------------------------------------------------------
BOOL GradientRectangle(HDC hDC, int x0, int y0, int x1, int y1, COLORREF c0, COLORREF c1, int type)
{
	TRIVERTEX vert[4] = {
		{ x0, y0,  R16(c0), G16(c0), B16(c0), 0 },				//  0:c0         3:(c0+c1)/2
		{ x1, y1,  R16(c1), G16(c1), B16(c1), 0 },				//
		{ x0, y1,  R16(c0, c1), G16(c0, c1), B16(c0, c1), 0 },	//
		{ x1, y0,  R16(c0, c1), G16(c0, c1), B16(c0, c1), 0 }	//  2:(c0+c1)/2  1: c1
	};

	ULONG Index[] = { 0, 1, 2, 0, 1, 3};

	switch ( type )
	{
		case gfsLeftToRight:
			return GradientFill(hDC, vert, 2, Index, 1, GRADIENT_FILL_RECT_H);

		case gfsLeftTopToRightBottom:
			return GradientFill(hDC, vert, 4, Index, 2, GRADIENT_FILL_TRIANGLE);

		case gfsUpToDown:
			return GradientFill(hDC, vert, 2, Index, 1, GRADIENT_FILL_RECT_V);

		case gfsRightTopToLeftBottom:
			vert[0].x = x1;
			vert[3].x = x0;
			vert[1].x = x0;
			vert[2].x = x1;
			return GradientFill(hDC, vert, 4, Index, 2, GRADIENT_FILL_TRIANGLE);
	}

	return FALSE;
}
// ----------------------------------------------------------------------------
BOOL SymGradientRectangle(HDC hDC, int x0, int y0, int x1, int y1, COLORREF c0, COLORREF c1, int type)
{
	TRIVERTEX vert[] = {
		{ x0,        y0,  R16(c0), G16(c0), B16(c0), 0 },
		{ x1, (y0+y1)/2,  R16(c1), G16(c1), B16(c1), 0 },
		{ x0, (y0+y1)/2,  R16(c1), G16(c1), B16(c1), 0 },
		{ x1,        y1,  R16(c0), G16(c0), B16(c0), 0 },

		{ x1,		 y0,  R16(c1), G16(c1), B16(c1), 0 },
		{ x0,		 y1,  R16(c1), G16(c1), B16(c1), 0 },

		{ (x0+x1)/2, y1,  R16(c1), G16(c1), B16(c1), 0 },
		{ (x0+x1)/2, y0,  R16(c1), G16(c1), B16(c1), 0 }
	};

	switch ( type )
	{
		case gfsVertical:
		{
			ULONG Index[] = { 0, 6, 7, 3 };
			return GradientFill(hDC, vert, 8, Index, 2, GRADIENT_FILL_RECT_H);
		}

		case gfsDiagonal:
		{
			ULONG Index[] = { 0, 4, 5, 4, 5, 3 };
			return GradientFill(hDC, vert, 6, Index, 2, GRADIENT_FILL_TRIANGLE);
		}

		case gfsHorizontal:
		{
			ULONG Index[] = { 0, 1, 2, 3 };
			return GradientFill(hDC, vert, 4, Index, 2, GRADIENT_FILL_RECT_V);
		}

		case gfsBackDiagonal:
		{
			TRIVERTEX vert[4] = {
				{ x0, y0,  R16(c1), G16(c1), B16(c1), 0 },
				{ x1, y0,  R16(c0), G16(c0), B16(c0), 0 },
				{ x0, y1,  R16(c0), G16(c0), B16(c0), 0 },
				{ x1, y1,  R16(c1), G16(c1), B16(c1), 0 }
			};

			ULONG Index[] = { 0, 1, 3, 0, 3, 2 };

			return GradientFill(hDC, vert, 4, Index, 2, GRADIENT_FILL_TRIANGLE);
		}
	}

	return FALSE;
}
// ----------------------------------------------------------------------------
BOOL CornerGradientRectangle(HDC hDC, int x0, int y0, int x1, int y1, COLORREF c0, COLORREF c1, int corner)
{
   if((corner < 0) || (corner > 3))
      return FALSE;
      
	TRIVERTEX vert[] = {
		{ x0,        y0,  R16(c1), G16(c1), B16(c1), 0 },
		{ x1,        y0,  R16(c1), G16(c1), B16(c1), 0 },
		{ x1,		 y1,  R16(c1), G16(c1), B16(c1), 0 },
		{ x0,		 y1,  R16(c1), G16(c1), B16(c1), 0 }
	};

	vert[corner].Red   = R16(c0);
	vert[corner].Green = G16(c0);
	vert[corner].Blue  = B16(c0);

	ULONG Index[] = { corner, (corner+1)%4, (corner+2)%4, 
		              corner, (corner+3)%4, (corner+2)%4 };
	
	return GradientFill(hDC, vert, 4, Index, 2, GRADIENT_FILL_TRIANGLE);
}
// ----------------------------------------------------------------------------
BOOL CenterGradientRectangle(HDC hDC, int x0, int y0, int x1, int y1, COLORREF c0, COLORREF c1)
{
	TRIVERTEX vert[] = {
		{ x0,        y0,		R16(c1), G16(c1), B16(c1), 0 },
		{ x1,        y0,		R16(c1), G16(c1), B16(c1), 0 },
		{ x1,		 y1,		R16(c1), G16(c1), B16(c1), 0 },
		{ x0,		 y1,		R16(c1), G16(c1), B16(c1), 0 },
		{ (x0+x1)/2, (y0+y1)/2, R16(c0), G16(c0), B16(c0), 0 }
	};

	ULONG Index[] = { 0, 1, 4, 1, 2, 4, 2, 3, 4, 3, 0, 4 };

	return GradientFill(hDC, vert, 5, Index, 4, GRADIENT_FILL_TRIANGLE);
}
// ----------------------------------------------------------------------------
BOOL RadialGradientFill(HDC hDC, int x0, int y0, int x1, int y1, int r, COLORREF c0, COLORREF c1, int nPart)
{
	const double PI2 = M_PI * (double)2.0;

	TRIVERTEX * pVertex = new TRIVERTEX[nPart+1];
	ULONG     * pMesh   = new ULONG[(nPart+1)*3];

	pVertex[0].x     = x1;
	pVertex[0].y     = y1;
	pVertex[0].Red   = R16(c0);
	pVertex[0].Green = G16(c0);
	pVertex[0].Blue  = G16(c0);
	pVertex[0].Alpha = 0;

	for (int i=0; i<nPart; i++)
	{
		pVertex[i+1].x     = x0 + (int) (r * cos(PI2 * i / nPart));
		pVertex[i+1].y     = y0 + (int) (r * sin(PI2 * i / nPart));
		pVertex[i+1].Red   = R16(c1);
		pVertex[i+1].Green = G16(c1);
		pVertex[i+1].Blue  = B16(c1);
		pVertex[i+1].Alpha = 0;

		pMesh[i*3+0] = 0;
		pMesh[i*3+1] = i+1;
		pMesh[i*3+2] = (i+1) % nPart+1;
	}

	BOOL rslt = GradientFill(hDC, pVertex, nPart+1, pMesh, nPart, GRADIENT_FILL_TRIANGLE);

	delete [] pVertex;
	delete [] pMesh;

	return rslt;
}

// --- Rectangle --------------------------------------------------------------
// ----------------------------------------------------------------------------
bool __fastcall TGradientFillRectangle::Load(char *rbuf){
   bool ret = false;

   Color2 = clWhite;
   FillStyle = gfsLeftToRight;

   char brkused, quoted, token[32];
   int i = 0,next = 0;

   // Read my name
   parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
         case kwBGColor:{
               TColor Color;
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               if(ReadValue(token, rHEX, &Color))
                  Color2 = Color;
            }
            break;
         case kwFillStyle:{
               TBrushStyle BStyle;
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               if(ReadValue(token, rINT, &BStyle))
                  FillStyle = BStyle;
            }
            break;
      }// switch
   }// while
   ret = TLine::Load(rbuf);
   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TGradientFillRectangle::Save(FILE *f){
   fprintf(f, "GradientFillRectangle");
   if(Color2     != clWhite) fprintf(f, " -bgc %x", Color2);
   if(FillStyle  != gfsLeftToRight) fprintf(f, " -fs %d", FillStyle);
   TLine::SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
void __fastcall TGradientFillRectangle::Show(TWorkPlace *wp){
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;

   HDC hDC = wp->getCanvas()->Handle;
   toScreen(wp);

   switch(FillStyle){
      case gfsLeftToRight:
      case gfsLeftTopToRightBottom:
      case gfsUpToDown:
      case gfsRightTopToLeftBottom:
         GradientRectangle(hDC, getps(0).x, getps(0).y, getps(1).x, getps(1).y, GetColor(), Color2, FillStyle);
         break;
      case gfsVertical:
      case gfsDiagonal:
      case gfsHorizontal:
      case gfsBackDiagonal:
         SymGradientRectangle(hDC, getps(0).x, getps(0).y, getps(1).x, getps(1).y, GetColor(), Color2, FillStyle);
         break;
      case gfsLeftTopCorner:     // 8
      case gfsRightTopCorner:    // 9
      case gfsRightBottomCorner: // 10
      case gfsLeftBottomCorner:  // 11
         CornerGradientRectangle(hDC, getps(0).x, getps(0).y, getps(1).x, getps(1).y, GetColor(), Color2, FillStyle - gfsLeftTopCorner);
         break;
      case gfsCenter:
         CenterGradientRectangle(hDC, getps(0).x, getps(0).y, getps(1).x, getps(1).y, GetColor(), Color2);
         break;
   }// switch
};
// ----------------------------------------------------------------------------
bool __fastcall TGradientFillRectangle::Hit(TWorkPlace *wp, TPoint p){
   toScreen(wp);
   TRect r(
    min(getps(0).x, getps(1).x),
    min(getps(0).y, getps(1).y),
    max(getps(0).x, getps(1).x),
    max(getps(0).y, getps(1).y)
   );
   return PtInRect(r, p);
}// TGradientFillRectangle::Hit
// ----------------------------------------------------------------------------
__fastcall TGradientFillRectangle::TGradientFillRectangle():TLine(){
          SetType(tGradientFillRect);
          Color2 = clBlack;
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TGradientFillRectangle::TGradientFillRectangle(
      double ssX, double ssY,
      double seX, double seY,
      TColor sColor1, TColor sColor2,
      GradientFillStyle sFillStyle):TLine(ssX, ssY, seX, seY, sColor1, 1){
         SetType(tGradientFillRect);
         Color2 = sColor2;
         FillStyle = sFillStyle;
         Smoothing(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

TGradientFillRectangle::~TGradientFillRectangle(){  
};

AnsiString __fastcall TGradientFillRectangle::GetName(void){return "прямоугольник заполненный градиентом";};

#ifdef _EDITOR_

TGradientFillRectangle& __fastcall TGradientFillRectangle::operator=(TGradientFillRectangle& R){
   *(TLine*)this = (TLine&)R;
   SetType(R.GetType());
   Color2 = R.Color2;
   FillStyle = R.FillStyle;
   TBaseGP::Smoothing(R.Smoothable());
   return *this;
};

TBaseGP* __fastcall TGradientFillRectangle::Copy(CopyGEMode m){
   TGradientFillRectangle *l = new TGradientFillRectangle;
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

 