#ifndef __POINTF_HPP__
#define __POINTF_HPP__

#define STRICT
#include <windows.h>
#include "gdiplus.h"
#pragma hdrstop
#include <vcl.h>

#define RAD ((double)57.29577951472)

// Тип координата с плавающей точкой
// TBG_000.CPP
class TPointf {
   double x;
   double y;
public:
   __fastcall TPointf();
   __fastcall TPointf(double sx, double sy);
   __fastcall TPointf(TPointf& p);
   bool __fastcall hit(TPointf p, double h);
   void __fastcall offset(double ox, double oy);
   void __fastcall moveto(double tox, double toy);
   void __fastcall zoom(double Zk);
   TPoint __fastcall toscreen(int, int);
   double __fastcall getx(void);
   double __fastcall gety(void);
   bool __fastcall operator==(const TPointf& p);
   bool __fastcall operator!=(const TPointf& p);
   const TPointf& __fastcall operator=(const TPointf& r);
   const TPointf& __fastcall operator+=(const TPointf& r);
   const TPointf& __fastcall operator-=(const TPointf& r);
   const TPointf& __fastcall operator=(const TPoint& r);
   TPointf __fastcall operator+(const TPointf& r);
   TPointf __fastcall operator-(const TPointf& r);
   void __fastcall rotate(double a, TPointf& c);
   double __fastcall angleto(TPointf& pto);
   void __fastcall mirrorx(double ax);
   void __fastcall mirrory(double ay);
};
#endif
