#include <math.h>
#include "Pointf.hpp"
#include <stdio.h>
// ----------------------------------------------------------------------------
// Реализация некоторых методов класса TPointf
// ----------------------------------------------------------------------------
__fastcall TPointf::TPointf(){x = y = 0.0;};
__fastcall TPointf::TPointf(double sx, double sy){
   x = sx; y = sy;
};
__fastcall TPointf::TPointf(TPointf& p){
   *this = p;
};
bool __fastcall TPointf::hit(TPointf p, double h){
   bool ret = false;
   if((p.getx() >= x - h) &&
      (p.gety() >= y - h) &&
      (p.getx() <= x + h) &&
      (p.gety() <= y + h) ){
      ret = true;
   }/* if */
   return ret;
};
void __fastcall TPointf::offset(double ox, double oy){
   x += ox;
   y += oy;
};
void __fastcall TPointf::moveto(double tox, double toy){
   x = tox;
   y = toy;
};
void __fastcall TPointf::zoom(double Zk){
   x *= Zk;
   y *= Zk;
};
TPoint __fastcall TPointf::toscreen(int maxx, int maxy){
   return TPoint(x * (double)maxx + 0.5, y * (double)maxy + 0.5);
};

double __fastcall TPointf::getx(void){return x;};
double __fastcall TPointf::gety(void){return y;};

TPointf __fastcall TPointf::operator+(const TPointf& r){
   return TPointf(x + r.x, y + r.y);
};

TPointf __fastcall TPointf::operator-(const TPointf& r){
   return TPointf(x - r.x, y - r.y);
};

const TPointf& __fastcall TPointf::operator+=(const TPointf& r){
   x += r.x;
   y += r.y;
   return *this;
};

const TPointf& __fastcall TPointf::operator-=(const TPointf& r){
   x -= r.x;
   y -= r.y;
   return *this;
};

bool __fastcall TPointf::operator==(const TPointf& p){
   return ((x == p.x) && (y == p.y));
};

bool __fastcall TPointf::operator!=(const TPointf& p){
   return ((x != p.x) || (y != p.y));
};

const TPointf& __fastcall TPointf::operator=(const TPointf& r){
   if(this != &r){
      x = r.x;
      y = r.y;
   }/* if */
   return *this;
};

const TPointf& __fastcall TPointf::operator=(const TPoint& r){
   x = r.x;
   y = r.y;
   return *this;
};

void __fastcall TPointf::rotate(double a, TPointf& c){
   double sina, cosa, tx, ty;
   a /= RAD;
   sina = sin(a);
   cosa = cos(a);
   tx = x - c.x;
   ty = y - c.y;
   x = ((tx * cosa) - (ty * sina)) + c.x;
   y = ((tx * sina) + (ty * cosa)) + c.y;
};

double __fastcall TPointf::angleto(TPointf& pto){ // azimuth to
   double x1, y1, x2, y2, a = 0.0;
   x1 = x;
   y1 = y;
   x2 = pto.x;
   y2 = pto.y;
   if(x2 - x1){
     a = atan((y2 - y1)/(x2 - x1)) * RAD;
     if((x2 - x1) < 0.0){
        a = 180.0 + a;
     }/* if */
   }/* if */
   if(a < 0.0) 
      a = 360.0 + a;
   return a;
};

void __fastcall TPointf::mirrorx(double ax){
   this->x = 2. * ax - this->x;
};

void __fastcall TPointf::mirrory(double ay){
   this->y = 2. * ay - this->y;
};
 