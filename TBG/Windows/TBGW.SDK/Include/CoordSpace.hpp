#ifndef __COORDSPACE_HPP__
#define __COORDSPACE_HPP__

//
//  оординатное пространство
//
class TCoordSpace {
   TPointf  Offset;
   TPointf *pf;   // координаты от 0.0 до 1.0
   TPoint  *ps;  // координаты на экране
   int      numPoints; // количество
public:
   ~TCoordSpace();
   __fastcall TCoordSpace();
   __fastcall TCoordSpace(int);
   __fastcall TCoordSpace(POINT *pp, int);
   __fastcall TCoordSpace(const TCoordSpace&);

   void __fastcall Save(FILE *);
   bool __fastcall LoadAsBase(char *);
   bool __fastcall Load(char *);

   bool __fastcall newSize(int); // разместить
   bool __fastcall setPoints(POINT *pp, int);
   inline int getSize(void)const{return numPoints;}; // количество координат
   inline int getMaxInd(void)const{return numPoints - 1;}; // макс. индекс в массиве координат
   bool __fastcall toScreen(TPoint);      // пересчЄт в координаты на экране
   bool __fastcall toScreen(int, int);    // ----- " -----
   bool __fastcall toScreen(TWorkPlace*); // ----- " -----
   bool __fastcall toAbsolute(int, int);  // пересчЄт из экранных координат в абсолютные

   inline TPointf& getpf(int i)const{return pf[i];};  // получить по индексу
//   inline TPointf& operator[](int i){return pf[i];};
   inline TPoint& getps(int i){return ps[i];};  // получить по индексу
   inline TPoint* getps(void){return ps;};  // получить по индексу
   const TCoordSpace& __fastcall operator=(const TCoordSpace&);
   inline bool setOffset(TPointf p){
      if(Offset != p){
         Offset = p;
         return true;
      }// if
      return false;
   };
   inline TPointf getOffset(void){return Offset;};

   virtual void __fastcall SetStartXY(double ssX, double ssY);
   virtual void __fastcall SetEndXY(double seX, double seY);
   virtual double __fastcall GetStartX(void);
   virtual double __fastcall GetStartY(void);
   virtual double __fastcall GetEndX(void);
   virtual double __fastcall GetEndY(void);

   virtual int __fastcall GetCoordCnt(void);
   virtual TPointf* __fastcall GetCoordPtr(int i);
   virtual void __fastcall MirrorX(double ax);
   virtual void __fastcall MirrorY(double ay);

   virtual int __fastcall RotateTo(TPointf cp, double ang);
   virtual int __fastcall MoveTo(TPointf);
   virtual int __fastcall MoveTo(double, double);
   virtual int __fastcall Zoom(double Zk);

};
#endif // COORDSPACE
