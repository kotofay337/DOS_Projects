/* TBG_006.CPP
*
*   TBG v7.0
*   9-05-2002 11:25 msk
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
bool __fastcall TFillRectangle::Load(char *rbuf){
   bool ret = false;

   ColorFill = clWhite;
   StyleFill = bsSolid;

   char brkused, quoted, token[32];
   int i = 0,next = 0;

   // Read my name
   parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
         case kwFilter:{
               char t[64];
               int nx = 0;
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               for(int i = 0;i < 3;i++)
                  for(int j = 0;j < 3;j++){
                     parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
                     ret |= ReadValue(t, rINT, &m[i][j]);
                  }// for
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rINT, &w);
               parser(0,t,sizeof(t) - 1, token, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
               ret |= ReadValue(t, rINT, &a);
               Filter = true;
            }
            break;
         case kwBlurToGrayscale:
            BlurToGrayscale = true;
            break;
         case kwBlur:
            parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
            ReadValue(token, rINT, &Blur);
            break;
         case kwBGColor:{
               TColor Color;
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               if(ReadValue(token, rHEX, &Color))
                  ColorFill = Color;
            }
            break;
         case kwFillStyle:{
               TBrushStyle BStyle;
               parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
               if(ReadValue(token, rHEX, &BStyle))
                  StyleFill = BStyle;
            }
            break;
      }// switch
   }// while
   ret = TLine::Load(rbuf);
   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TFillRectangle::Save(FILE *f){
   fprintf(f, "FillRectangle");
   if((Blur == 0) && (Filter == false)){
      if(ColorFill  != clWhite) fprintf(f, " -bgc %x", ColorFill);
      if(StyleFill  != bsSolid) fprintf(f, " -fs %x", StyleFill);
   }// if
   else{
      if(Filter == false){
         fprintf(f, " -blur %u", Blur);
         if(BlurToGrayscale) fprintf(f, " -grayscale");
      }// if
      else{
         fprintf(f, " -filter \"");
         for(int i = 0;i < 3;i++)
            for(int j = 0;j < 3;j++)
               fprintf(f, " %d", m[i][j]);
         fprintf(f, "  %d %d\"", w, a);
      }// elif
   }// elif
   TLine::SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
void __fastcall TFillRectangle::SetBlur(int b){Blur = b;};
int __fastcall TFillRectangle::GetBlur(void){return Blur;};
void __fastcall TFillRectangle::SetBlurToGrayscale(bool gs){BlurToGrayscale = gs;};
bool __fastcall TFillRectangle::GetBlurToGrayscale(void){return BlurToGrayscale;};
// ----------------------------------------------------------------------------
void __fastcall TFillRectangle::Show(TWorkPlace *wp){
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;
   TCanvas *cvs = wp->getCanvas();
   toScreen(wp);

   if((Blur == 0) && (Filter == false)){
      cvs->Pen->Color = GetColor();
      cvs->Pen->Width = GetWidthLine();
      cvs->Pen->Style = psSolid;
      cvs->Brush->Color = ColorFill;
      cvs->Brush->Style = StyleFill;
      cvs->Rectangle(TRect(getps(0), getps(getMaxInd())));
   }// if
   else{
     //printf("\nBlur=%d, Filter=%d m={ ", Blur, Filter);
     //for(int i = 0;i < 3;i++)
     //   for(int j = 0;j < 3;j++)
     //      printf("[%d][%d]=%d ", i, j, m[i][j]);
     //printf("},  w=%d a=%d\n", w, a);

      Graphics::TBitmap *SrcBitmap = NULL;
      Graphics::TBitmap *DstBitmap = NULL;
      SrcBitmap = new Graphics::TBitmap;
      DstBitmap = new Graphics::TBitmap;
      if(SrcBitmap && DstBitmap){
         TRect   DestRect, SrcRect;
         DestRect = Rect(min(getps(0).x, getps(1).x), min(getps(0).y, getps(1).y),
                         max(getps(0).x, getps(1).x), max(getps(0).y, getps(1).y));
         SrcRect = Rect(0, 0, DestRect.Width(), DestRect.Height());
         SrcBitmap->PixelFormat = pf32bit;
         SrcBitmap->Width = DestRect.Width();
         SrcBitmap->Height = DestRect.Height();

         DstBitmap->PixelFormat = pf32bit;
         DstBitmap->Width = DestRect.Width();
         DstBitmap->Height = DestRect.Height();

         SrcBitmap->Canvas->CopyRect(SrcRect, cvs, DestRect);

         DWORD *SrcPtr00, *SrcPtr10, *SrcPtr20, *DstPtr;
         int r, g, b;
         BYTE R, G, B;
         DWORD p00, p01, p02;
         DWORD p10, p11, p12;
         DWORD p20, p21, p22;
          for (int y = 1; y < SrcBitmap->Height - 1; y++){
            SrcPtr00 = (DWORD *)SrcBitmap->ScanLine[y - 1];
            SrcPtr10 = (DWORD *)SrcBitmap->ScanLine[y    ];
            SrcPtr20 = (DWORD *)SrcBitmap->ScanLine[y + 1];
            DstPtr = (DWORD *)DstBitmap->ScanLine[y];
            for (int x = 1; x < SrcBitmap->Width - 1; x++){
              p00 = SrcPtr00[x - 1]; p01 = SrcPtr00[x]; p02 = SrcPtr00[x + 1];
              p10 = SrcPtr10[x - 1]; p11 = SrcPtr10[x]; p12 = SrcPtr10[x + 1];
              p20 = SrcPtr20[x - 1]; p21 = SrcPtr20[x]; p22 = SrcPtr20[x + 1];

              if(Filter){
                 r = (GetRValue(p00)*m[0][0] + GetRValue(p01)*m[0][1] + GetRValue(p02)*m[0][2] +
                      GetRValue(p10)*m[1][0] + GetRValue(p11)*m[1][1] + GetRValue(p12)*m[1][2] +
                      GetRValue(p20)*m[2][0] + GetRValue(p21)*m[2][1] + GetRValue(p22)*m[2][2]) / w + a;

                 g = (GetGValue(p00)*m[0][0] + GetGValue(p01)*m[0][1] + GetGValue(p02)*m[0][2] +
                      GetGValue(p10)*m[1][0] + GetGValue(p11)*m[1][1] + GetGValue(p12)*m[1][2] +
                      GetGValue(p20)*m[2][0] + GetGValue(p21)*m[2][1] + GetGValue(p22)*m[2][2]) / w + a;

                 b = (GetBValue(p00)*m[0][0] + GetBValue(p01)*m[0][1] + GetBValue(p02)*m[0][2] +
                      GetBValue(p10)*m[1][0] + GetBValue(p11)*m[1][1] + GetBValue(p12)*m[1][2] +
                      GetBValue(p20)*m[2][0] + GetBValue(p21)*m[2][1] + GetBValue(p22)*m[2][2]) / w + a;

                 R = NORMALIZE(r, 0, 255);
                 G = NORMALIZE(g, 0, 255);
                 B = NORMALIZE(b, 0, 255);

                 DstPtr[x] = RGB(R, G, B);
              }// if
              else{
                 r = (GetRValue(p00) + GetRValue(p01) + GetRValue(p02) +
                      GetRValue(p10) + GetRValue(p11) + GetRValue(p12) +
                      GetRValue(p20) + GetRValue(p21) + GetRValue(p22)) / 9;

                 g = (GetGValue(p00) + GetGValue(p01) + GetGValue(p02) +
                      GetGValue(p10) + GetGValue(p11) + GetGValue(p12) +
                      GetGValue(p20) + GetGValue(p21) + GetGValue(p22)) / 9;

                 b = (GetBValue(p00) + GetBValue(p01) + GetBValue(p02) +
                      GetBValue(p10) + GetBValue(p11) + GetBValue(p12) +
                      GetBValue(p20) + GetBValue(p21) + GetBValue(p22)) / 9;

                 R = NORMALIZE(r, 0, 255);
                 G = NORMALIZE(g, 0, 255);
                 B = NORMALIZE(b, 0, 255);

                 if(BlurToGrayscale){
                    BYTE gs = (BYTE)((R * 30 + G * 59 + B * 11 + 50) / 100);
                    DstPtr[x] = RGB(gs, gs, gs);
                 }// if
                 else{
                    DstPtr[x] = RGB(R, G, B);
                 }// if
              }// elif
            }// for
          }// for

         cvs->CopyRect(DestRect, DstBitmap->Canvas, SrcRect);
         delete SrcBitmap;
         delete DstBitmap;
      }// if
      else{
      }// elif
   }// elif
};
// ----------------------------------------------------------------------------
bool __fastcall TFillRectangle::Hit(TWorkPlace *wp, TPoint p){
   toScreen(wp);
   TRect r(
    min(getps(0).x, getps(1).x),
    min(getps(0).y, getps(1).y),
    max(getps(0).x, getps(1).x),
    max(getps(0).y, getps(1).y)
   );
   return PtInRect(r, p);
}// TFillRectangle::Hit
// ----------------------------------------------------------------------------
__fastcall TFillRectangle::TFillRectangle():TLine(){
          SetType(tFillRect);
          ColorFill = clBlack;
          StyleFill = bsSolid;
          Blur = 0;
          BlurToGrayscale = false;
          Filter = false;
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TFillRectangle::TFillRectangle(double ssX, double ssY,
     double seX, double seY,
     TColor sColor, TColor sColorFill, TBrushStyle sStyleFill, unsigned sWidth):TLine(ssX, ssY, seX, seY, sColor, sWidth){
         SetType(tFillRect);
         ColorFill = sColorFill;
         StyleFill = sStyleFill;
         Blur = 0;
         BlurToGrayscale = false;
         Filter = false;
         Smoothing(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

TFillRectangle::~TFillRectangle(){
/*
      if(SrcBitmap)
         delete SrcBitmap;
      if(DstBitmap)
         delete DstBitmap;
*/
};

AnsiString __fastcall TFillRectangle::GetName(void){return "прямоугольник заполненный";};

#ifdef _EDITOR_

TFillRectangle& __fastcall TFillRectangle::operator=(TFillRectangle& R){
   *(TLine*)this = (TLine&)R;
   SetType(R.GetType());
   ColorFill = R.ColorFill;
   StyleFill = R.StyleFill;
   Blur = R.Blur;
   BlurToGrayscale = R.BlurToGrayscale;

   Filter = R.Filter;
   for(int i = 0;i < 3;i++)
      for(int j = 0;j < 3;j++)
         m[i][j] = R.m[i][j];
   w = R.w;
   a = R.a;

   TBaseGP::Smoothing(R.Smoothable());
   return *this;
};

TBaseGP* __fastcall TFillRectangle::Copy(CopyGEMode m){
   TFillRectangle *l = new TFillRectangle;
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

