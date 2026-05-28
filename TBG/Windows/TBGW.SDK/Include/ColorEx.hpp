#ifndef __COLOREX_HPP__
#define __COLOREX_HPP__

#define STRICT
#include <windows.h>
#include "gdiplus.h"
#pragma hdrstop
#include <vcl.h>

//
// Color operation section
//

#define NORMALIZE(v, vmin, vmax) (max((vmin), min((v), (vmax))))

inline COLOR16 R16(COLORREF c) { return (COLOR16)(GetRValue(c)<<8); }
inline COLOR16 G16(COLORREF c) { return (COLOR16)(GetGValue(c)<<8); }
inline COLOR16 B16(COLORREF c) { return (COLOR16)(GetBValue(c)<<8); }

inline COLOR16 R16(COLORREF c0, COLORREF c1) { return (COLOR16)(((GetRValue(c0)+GetRValue(c1))/2)<<8); }
inline COLOR16 G16(COLORREF c0, COLORREF c1) { return (COLOR16)(((GetGValue(c0)+GetGValue(c1))/2)<<8); }
inline COLOR16 B16(COLORREF c0, COLORREF c1) { return (COLOR16)(((GetBValue(c0)+GetBValue(c1))/2)<<8); }

class TColorEx {

   enum Cvt{ noCvt, toHLS, toHSV };

   double Hue, Saturation,
          Value, Lighting,
          Red, Green, Blue;
   Cvt m;
protected:
   double __fastcall RGBValue(double H, double M1, double M2);
public:
   __fastcall TColorEx(void);
   __fastcall TColorEx(TColor);
   __fastcall TColorEx(const TColorEx&);

   void __fastcall RGBToHSV(void);
   void __fastcall RGBToHLS(void);
   void __fastcall HSVToRGB(void);
   void __fastcall HLSToRGB(void);

   TColor __fastcall toRGB(void);
   TColor __fastcall toGrayscale(void);

   const TColorEx& __fastcall operator=(const TColorEx& c);
   const TColorEx& __fastcall operator=(const TColor& c);

   inline double getHue       (void){ return Hue; };
   inline double getSaturation(void){ return Saturation; };
   inline double getValue     (void){ return Value; };
   inline double getLighting  (void){ return Lighting; };
   inline double getRed       (void){ return Red; };
   inline double getGreen     (void){ return Green; };
   inline double getBlue      (void){ return Blue; };
   inline void setHue       (double h){ Hue = NORMALIZE(h, 0.0, 360.0); };
   inline void setSaturation(double s){ Saturation = NORMALIZE(s, 0.0, 1.0); };
   inline void setValue     (double v){ Value = NORMALIZE(v, 0.0, 1.0); };
   inline void setLighting  (double l){ Lighting = NORMALIZE(l, 0.0, 1.0); };
   inline void setRed       (double r){ Red = NORMALIZE(r, 0.0, 1.0); };
   inline void setGreen     (double g){ Green = NORMALIZE(g, 0.0, 1.0); };
   inline void setBlue      (double b){ Blue = NORMALIZE(b, 0.0, 1.0); };
   inline void setRGB       (double r, double g, double b){setRed(r); setGreen(g); setBlue(b);};

   unsigned char __fastcall GetR(void);
   unsigned char __fastcall GetG(void);
   unsigned char __fastcall GetB(void);

   void __fastcall SetR(unsigned char r);
   void __fastcall SetG(unsigned char g);
   void __fastcall SetB(unsigned char b);
};
#endif // COLOREX