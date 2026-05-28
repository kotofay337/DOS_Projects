#include "tbg.hpp"
// ----------------------------------------------------------------------------
__fastcall TCoordSpace::TCoordSpace(){
   numPoints = 0;
   pf = NULL;
   ps = NULL;
};
// ----------------------------------------------------------------------------
__fastcall TCoordSpace::TCoordSpace(int sz){
   numPoints = 0;
   pf = NULL;
   ps = NULL;
   newSize(sz);
};
// ----------------------------------------------------------------------------
__fastcall TCoordSpace::TCoordSpace(POINT *pp, int sz){
   numPoints = 0;
   pf = NULL;
   ps = NULL;
   if(pp && sz){
      newSize(sz);
      if(ps == NULL){
         ps = new TPoint[numPoints];
      }// if
      for(int i = 0;i < numPoints;i++){
         ps[i].x = pp[i].x;
         ps[i].y = pp[i].y;
      }// for
   }// if
};
// ----------------------------------------------------------------------------
__fastcall TCoordSpace::TCoordSpace(const TCoordSpace& s){
   numPoints = 0;
   pf = NULL;
   ps = NULL;
   *this = s;
};
// ----------------------------------------------------------------------------
TCoordSpace::~TCoordSpace(){
   if(pf){
      delete[] pf;
      pf = NULL;
   }// if
   if(ps){
      delete[] ps;
      ps = NULL;
   }// if
   numPoints = 0;
};
// ----------------------------------------------------------------------------
bool __fastcall TCoordSpace::newSize(int sz){ // разместить
   if((sz > 0) && (sz != numPoints)){
      TPointf *t = new TPointf[sz];
      if(t){
         if(pf){
            for(int i = 0;i < min(numPoints, sz);i++)
               t[i] = pf[i];
            delete[] pf;
         }// if
         pf = t;
         numPoints = sz;
         if(ps){
            delete[] ps;
            ps = NULL;
         }// if
         return true;
      }// if
   }// if
   return false;
};
// ----------------------------------------------------------------------------
void __fastcall TCoordSpace::Save(FILE *f){
   if(numPoints){
      fprintf(f, " -coord \"%u", getSize());
      for(int i = 0;i < getSize();i++){
         fprintf(f, " %lf", getpf(i).getx());
         fprintf(f, " %lf", getpf(i).gety());
      }/* for */
      fprintf(f, "\"");
   }// if
};
// ----------------------------------------------------------------------------
bool __fastcall TCoordSpace::LoadAsBase(char *rbuf){
   bool ret = false;
   char brkused, quoted;
   char t[32];
   int nx = 0;
   int nSize;

   parser(0,t,sizeof(t) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
   ret = ReadValue(t, rINT, &nSize);
   newSize(nSize);
   for(int i = 0; i < nSize ;i++){
       double sX, sY;
       parser(0,t,sizeof(t) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
       ret = ReadValue(t, rDOUBLE, &sX);
       parser(0,t,sizeof(t) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &nx, &quoted);
       ret = ReadValue(t, rDOUBLE, &sY);
       getpf(i).moveto(sX, sY);
   }// for
   return ret;
};
// ----------------------------------------------------------------------------
bool __fastcall TCoordSpace::Load(char *rbuf){
   bool ret = false;
   char brkused, quoted, token[9217]; // max 512 TPointf
   int next = 0;

   // Read my name
   parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
          case kwCoords:
            parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
            ret = LoadAsBase(token);
            break;
      }// switch
   }// while
   return ret;
};
// ----------------------------------------------------------------------------
const TCoordSpace& __fastcall TCoordSpace::operator=(const TCoordSpace& from){
   if(this != &from)
      if(newSize(from.getSize())){
         if(ps == NULL){
            ps = new TPoint[numPoints];
         }// if
         for(int i = 0;i < numPoints;i++)
            pf[i] = from.getpf(i);
         Offset = from.Offset;
      }// if
   return *this;
};
// ----------------------------------------------------------------------------
bool __fastcall TCoordSpace::toAbsolute(int ssx, int ssy){
   if(pf && numPoints && ssx & ssy && ps){
      for(int i = 0;i < numPoints;i++)
         pf[i].moveto(ps[i].x / (double)ssx, ps[i].y / (double)ssy);
      return true;
   }// if
   return false;
};
// ----------------------------------------------------------------------------
bool __fastcall TCoordSpace::setPoints(POINT *pp, int cnt){
   if(pp && cnt){
      if(cnt != numPoints){
         newSize(cnt);
         if(ps == NULL){
            ps = new TPoint[numPoints];
         }// if
      }// if
      for(int i = 0;i < numPoints;i++)
         ps[i] = pp[i];
   }// if
   return true;
};
// ----------------------------------------------------------------------------
bool __fastcall TCoordSpace::toScreen(int ssx, int ssy){
   if(pf && numPoints){
      if(ps == NULL){
         ps = new TPoint[numPoints];
      }// if
      if(ps){
         for(int i = 0;i < numPoints;i++)
            ps[i] = (pf[i] + Offset).toscreen(ssx, ssy);
         return true;
      }// if
   }// if
   return false;
};
// ----------------------------------------------------------------------------
bool __fastcall TCoordSpace::toScreen(TPoint p){
   return toScreen(p.x, p.y);
};
// ----------------------------------------------------------------------------
bool __fastcall TCoordSpace::toScreen(TWorkPlace* wp){
   return toScreen(wp->getMaxX(), wp->getMaxY());
};
// ----------------------------------------------------------------------------
void __fastcall TCoordSpace::SetStartXY(double ssX, double ssY){
   if(numPoints > 0){
      pf[0].moveto(ssX, ssY);
   }// if
};
// ----------------------------------------------------------------------------
void __fastcall TCoordSpace::SetEndXY(double seX, double seY){
   if(numPoints > 0){
      pf[numPoints - 1].moveto(seX, seY);
   }// if
};
// ----------------------------------------------------------------------------
double __fastcall TCoordSpace::GetStartX(void){
   if(numPoints > 0){
      return pf[0].getx();
   }// if
   return 0.0;
};
// ----------------------------------------------------------------------------
double __fastcall TCoordSpace::GetStartY(void){
   if(numPoints > 0){
      return pf[0].gety();
   }// if
   return 0.0;
};
// ----------------------------------------------------------------------------
double __fastcall TCoordSpace::GetEndX(void){
   if(numPoints > 0){
      return pf[numPoints - 1].getx();
   }// if
   return 0.0;
};
// ----------------------------------------------------------------------------
double __fastcall TCoordSpace::GetEndY(void){
   if(numPoints > 0){
      return pf[numPoints - 1].gety();
   }// if
   return 0.0;
};
// ----------------------------------------------------------------------------
int __fastcall TCoordSpace::GetCoordCnt(void){
   return numPoints;
};
// ----------------------------------------------------------------------------
TPointf* __fastcall TCoordSpace::GetCoordPtr(int i){
   return &getpf(i);
};
// ----------------------------------------------------------------------------
void __fastcall TCoordSpace::MirrorX(double ax){
   if(numPoints > 0)
      for(int i = 0;i < numPoints;i++)
         pf[i].mirrorx(ax);
};
// ----------------------------------------------------------------------------
void __fastcall TCoordSpace::MirrorY(double ay){
   if(numPoints > 0)
      for(int i = 0;i < numPoints;i++)
         pf[i].mirrory(ay);
};
// ----------------------------------------------------------------------------
int __fastcall TCoordSpace::RotateTo(TPointf cp, double ang){
   if(numPoints > 0){
      for(int i = 0;i < numPoints;i++)
         pf[i].rotate(ang, cp);
      return 1;
   }// if
   return 0;
};
// ----------------------------------------------------------------------------
int __fastcall TCoordSpace::Zoom(double Zk){
   if(numPoints > 0){
      for(int i = 0;i < numPoints;i++)
         pf[i].zoom(Zk);
      return 1;
   }// if
   return 0;
};
// ----------------------------------------------------------------------------
int __fastcall TCoordSpace::MoveTo(TPointf to){
   for(int i = 0;i < getSize();i++){
      getpf(i) -= to;
   }// for
   return 1;
};
// ----------------------------------------------------------------------------
int __fastcall TCoordSpace::MoveTo(double toX, double toY){
   return MoveTo(TPointf(toX, toY));
};
// ----------------------------------------------------------------------------

 