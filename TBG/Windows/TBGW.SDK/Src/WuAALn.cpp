#include <vcl.h>

#include "tbg_tc.h"
#pragma hdrstop

// 2002.12.18
// Альфа-наложение добавлено мной: А.Ч.
//
static void __fastcall DrawPixel(TCanvas *Canvas, int x, int y, int bc, unsigned long rgb){
   TCanvas *c = Canvas;
   register int A, B = bc - 255;
   TColor s, d;
   register unsigned char r, g, b, sr, sg, sb;

   A = 255 - B;

   d = c->Pixels[x][y];

   r = (unsigned char) (d & 0xFF);
   g = (unsigned char)((d & 0xFF00) >> 8);
   b = (unsigned char)((d & 0xFF0000) >> 16);

   sr = (unsigned char) (rgb & 0xFF);
   sg = (unsigned char)((rgb & 0xFF00) >> 8);
   sb = (unsigned char)((rgb & 0xFF0000) >> 16);

   r = (sr * A + B * r) / 255;
   g = (sg * A + B * g) / 255;
   b = (sb * A + B * b) / 255;

   c->Pixels[x][y] = (TColor)RGB(r, g, b);
};

void __fastcall WuLine(TCanvas *Canvas, int X0, int Y0, int X1, int Y1, int BaseColor, int NumLevels,
   unsigned int IntensityBits, unsigned long rgb)
{
   register unsigned short int IntensityShift, ErrorAdj, ErrorAcc;
   register unsigned short int ErrorAccTemp, Weighting, WeightingComplementMask;
   register short int DeltaX, DeltaY, Temp, XDir;

   if (Y0 > Y1) {
      Temp = Y0; Y0 = Y1; Y1 = Temp;
      Temp = X0; X0 = X1; X1 = Temp;
   }
   DrawPixel(Canvas, X0, Y0, BaseColor, rgb);

   if ((DeltaX = X1 - X0) >= 0) {
      XDir = 1;
   } else {
      XDir = -1;
      DeltaX = -DeltaX;
   }
   if ((DeltaY = Y1 - Y0) == 0) {
      Line(Canvas->Handle, X0, Y0, X1, Y1);
      return;
   }
   if (DeltaX == 0) {
      Line(Canvas->Handle, X0, Y0, X1, Y1);
      return;
   }
   if (DeltaX == DeltaY) {
      Line(Canvas->Handle, X0, Y0, X1, Y1);
      return;
   }
   ErrorAcc = 0;
   IntensityShift = 16 - IntensityBits;
   WeightingComplementMask = NumLevels - 1;
   if (DeltaY > DeltaX) {
      ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
      while (--DeltaY) {
         ErrorAccTemp = ErrorAcc;
         ErrorAcc += ErrorAdj;
         if (ErrorAcc <= ErrorAccTemp) {
            X0 += XDir;
         }
         Y0++;
         Weighting = ErrorAcc >> IntensityShift;
         DrawPixel(Canvas, X0, Y0, BaseColor + Weighting, rgb);
         DrawPixel(Canvas, X0 + XDir, Y0,
               BaseColor + (Weighting ^ WeightingComplementMask), rgb);
      }
      DrawPixel(Canvas, X1, Y1, BaseColor, rgb);
      return;
   }
   ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
   while (--DeltaX) {
      ErrorAccTemp = ErrorAcc;
      ErrorAcc += ErrorAdj;
      if (ErrorAcc <= ErrorAccTemp) {
         Y0++;
      }
      X0 += XDir;
      Weighting = ErrorAcc >> IntensityShift;
      DrawPixel(Canvas, X0, Y0, BaseColor + Weighting, rgb);
      DrawPixel(Canvas, X0, Y0 + 1,
            BaseColor + (Weighting ^ WeightingComplementMask), rgb);
   }
   DrawPixel(Canvas, X1, Y1, BaseColor, rgb);
}
/* ----------------------------------------------------------------------- */
void __fastcall WuPolyline(TCanvas *cvs, TPoint *pts, int maxind){
   if(cvs && pts && maxind >= 1){
      for(int i = 0;i < maxind;i++)
         WuLine(cvs, pts[i].x, pts[i].y, pts[i + 1].x, pts[i + 1].y, 255, 255, 8, (unsigned long)cvs->Pen->Color);
   }// if
};
/* ----------------------------------------------------------------------- */
void __fastcall WuPolyDraw(TCanvas *cvs, POINT *pp, BYTE *pb, int cnt){
   int lmx, lmy;
   if(cvs && pb && pp && cnt)
      for(int i = 0;i < cnt;i++){
         switch(pb[i]){
            case PT_MOVETO:
               lmx = pp[i].x;
               lmy = pp[i].y;
               break;
            case PT_LINETO:
               WuLine(cvs, lmx, lmy, pp[i].x, pp[i].y, 255, 255, 8, (unsigned long)cvs->Pen->Color);
               lmx = pp[i].x;
               lmy = pp[i].y;
               break;
         }// switch
         if(pb[i] & PT_CLOSEFIGURE)
            WuLine(cvs, lmx, lmy, pp[i].x, pp[i].y, 255, 255, 8, (unsigned long)cvs->Pen->Color);
      }// for
}//
// ----------------------------------------------------------------------------

