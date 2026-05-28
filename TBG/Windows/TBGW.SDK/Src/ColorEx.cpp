#include <math.h>
#include "ColorEx.hpp"


__fastcall TColorEx::TColorEx(void){
   Hue = Saturation = Value = Lighting =
   Red = Green = Blue = 0.0;
   m = noCvt;
};
// ----------------------------------------------------------------------------
__fastcall TColorEx::TColorEx(TColor c){
   TColorEx();
   SetR(GetRValue(c));
   SetG(GetGValue(c));
   SetB(GetBValue(c));
};
// ----------------------------------------------------------------------------
__fastcall TColorEx::TColorEx(const TColorEx& cx){
};
// ----------------------------------------------------------------------------
void __fastcall TColorEx::RGBToHSV(void){
   double Temp = min(Red, min(Green, Blue));
   Value = max(Red, max(Green, Blue));

   if(Value == 0){
      Saturation = 0;
   }// if
   else{
      Saturation = (Value - Temp) / Value;
   }// elif

   if(Saturation == 0){
      Hue = 0;
   }// if
   else{
      double Cr = (Value - Red)   / (Value - Temp);
      double Cg = (Value - Green) / (Value - Temp);
      double Cb = (Value - Blue)  / (Value - Temp);

      if(Red == Value)
         Hue = Cb - Cg;
      if(Green == Value)
         Hue = 2. + Cr - Cb;
      if(Blue == Value)
         Hue = 4. + Cg - Cr;

      Hue *= 60.;
      if(Hue < 0.)
         Hue += 360.;
   }// elif
   m = toHSV;
};
// ----------------------------------------------------------------------------
void __fastcall TColorEx::HSVToRGB(void){
   if(m != toHSV)
      return;

   if(Saturation == 0){
      Red = Green = Blue = Value;
   }// if
   else{
      if(Hue == 360.){
         Hue = 0.;
      }// if
      else{
         Hue /= 60.;
      }// elif

      double V = Value;
      double I = floor(Hue);
      double F = Hue - I;
      double M = V * (1. - Saturation);
      double N = V * (1. - Saturation * F);
      double K = V * (1. - Saturation * (1. - F));

      if(I == 1.0)
         setRGB(V, K, M);
      if(I == 2.0)
         setRGB(N, V, M);
      if(I == 3.0)
         setRGB(M, V, K);
      if(I == 4.0)
         setRGB(M, N, V);
      if(I == 5.0)
         setRGB(K, M, V);
      if(I == 6.0)
         setRGB(V, M, N);
   }// elif
};
// ----------------------------------------------------------------------------
void __fastcall TColorEx::RGBToHLS(void){
   double M1 = max(Red, max(Green, Blue));
   double M2 = min(Red, min(Green, Blue));

   Lighting = (M1 + M2) / 2.;

   if(M1 == M2){
      Saturation = 0.;
      Hue = 0.0;
   }// if
   else{
      if(Lighting <= 0.5){
         Saturation = (M1 - M2) / (M1 + M2);
      }// if
      else{
         Saturation = (M1 - M2) / (2. - M1 - M2);
      }// elif

      double Cr = (M1 - Red)   / (M1 - M2);
      double Cg = (M1 - Green) / (M1 - M2);
      double Cb = (M1 - Blue)  / (M1 - M2);

      if(Red   == M1)
         Hue = Cb - Cg;
      if(Green == M1)
         Hue = 2. + Cr - Cb;
      if(Blue  == M1)
         Hue = 4. + Cg - Cr;

      Hue *= 60;
      if(Hue < 0.)
         Hue += 360.;
   }// elif
   m = toHLS;
};
// ----------------------------------------------------------------------------
double __fastcall TColorEx::RGBValue(double H, double M1, double M2){
   double retValue = 0.0;

   if(H <   0.)
      H += 360.;
   if(H > 360.)
      H -= 360.;

   if (H <   60.)
      retValue = M1 + (M2 - M1) * H / 60.;
   if((H >=  60.) && (H < 180.))
      retValue = M2;
   if((H >= 180.) && (H < 240.))
      retValue = M1 + (M2 - M1) * (240. - H) / 60.;
   if((H >= 240.) && (H < 360.))
      retValue = M1;

   return retValue;
};
// ----------------------------------------------------------------------------
void __fastcall TColorEx::HLSToRGB(void){
   double M1, M2;

   if(m != toHLS)
      return;

   if(Lighting <= 0.5){
      M2 = Lighting * (1. + Saturation);
   }// if
   else{
      M2 = Lighting + Saturation - Lighting * Saturation;
   }// elif
   M1 = 2. * Lighting - M2;

   if(Saturation == 0.0){
      setRGB(Lighting, Lighting, Lighting);
   }// if
   else{
      Red   = RGBValue(Hue + 120., M1, M2);
      Green = RGBValue(Hue, M1, M2);
      Blue  = RGBValue(Hue - 120., M1, M2);
   }// elif
};
// ----------------------------------------------------------------------------
TColor __fastcall TColorEx::toRGB(void){
   if(m == toHLS) HLSToRGB();
   if(m == toHSV) HSVToRGB();
   return (TColor)RGB(GetR(), GetG(), GetB());
};
// ----------------------------------------------------------------------------
TColor __fastcall TColorEx::toGrayscale(void){

   if(m == toHLS) HLSToRGB();
   if(m == toHSV) HSVToRGB();
   BYTE Gs = (BYTE)((GetR() * 30 + GetG() * 59 + GetB() * 11 + 50) / 100);

   return (TColor)RGB(Gs, Gs, Gs);

};
// ----------------------------------------------------------------------------
const TColorEx& __fastcall TColorEx::operator=(const TColorEx& c){
   if(this != &c){
      m = c.m;
      Hue = c.Hue;
      Saturation = c.Saturation;
      Value = c.Value;
      Lighting = c.Lighting;
      Red = c.Red;
      Green = c.Green;
      Blue = c.Blue;
   }// if
   return *this;
};
// ----------------------------------------------------------------------------
const TColorEx& __fastcall TColorEx::operator=(const TColor& c){
   SetR(GetRValue(c));
   SetG(GetGValue(c));
   SetB(GetBValue(c));
   m = noCvt;
   return *this;
};
// ----------------------------------------------------------------------------
unsigned char __fastcall TColorEx::GetR(void){ return getRed() * 255.0 + 0.5; };
// ----------------------------------------------------------------------------
unsigned char __fastcall TColorEx::GetG(void){ return getGreen() * 255.0 + 0.5; };
// ----------------------------------------------------------------------------
unsigned char __fastcall TColorEx::GetB(void){ return getBlue() * 255.0 + 0.5; };
// ----------------------------------------------------------------------------
void __fastcall TColorEx::SetR(unsigned char r){ setRed((double)r / 255.0); };
// ----------------------------------------------------------------------------
void __fastcall TColorEx::SetG(unsigned char g){ setGreen((double)g / 255.0); };
// ----------------------------------------------------------------------------
void __fastcall TColorEx::SetB(unsigned char b){ setBlue((double)b / 255.0); };

 