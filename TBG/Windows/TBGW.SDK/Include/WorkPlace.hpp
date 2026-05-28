#ifndef __WORKPLACE_HPP__
#define __WORKPLACE_HPP__

//
// Рабочее место
//
class TWorkPlace {
   int                maxX,          // 
                      maxY;          // размеры изображения в пикселах
   bool               EditProcessed; // флаг редактирования изображения
   bool               ShowHideGP;    // флаг режима показа скрытых ГП

   TCanvas           *CanvasOut; // контекст вывода
   Graphics::TBitmap *Buf;       // буфер отображения
   Gdiplus::Graphics *gs;        // контекст GDI+
   bool               BufCopy;   // флаг копии буфера
   bool               gsCopy;    // флаг копии контекста GDI+
   TCanvas           *CanvasBuf; // контекст буфера отображения

   bool               Update; // флаг необходимости перевывода

public:
   __fastcall TWorkPlace();

   __fastcall TWorkPlace(TCanvas *c, bool EditShow = false);
   
   ~TWorkPlace();

   const TWorkPlace& __fastcall operator=(const TWorkPlace& r);

   inline void setSizeScreen(int x, int y){
      if(x != maxX)
         setMaxX(x);   
      if(y != maxY)
         setMaxY(y);   
   };
   inline void setMaxX(int x){
      maxX = x <= 0 ? 1 : x; 
      if(Buf)
         Buf->Width = maxX;
      Update = true;
   };
   inline void setMaxY(int y){
      maxY = y <= 0 ? 1 : y;
      if(Buf)
         Buf->Height = maxY;
      Update = true;
   };
   inline double getMaxX(void)const{
      return (double)maxX;
   };
   inline double getMaxY(void)const{
      return (double)maxY;
   };

   void __fastcall setCanvasOut(TCanvas*);

   inline TCanvas* getCanvasOut(void){
      return CanvasOut;
   };
   inline TCanvas* getCanvas(void){
      TCanvas *ret = CanvasBuf;
      if(Buf){
         ret = Buf->Canvas;
      }// if
      CanvasBuf = ret;
      return ret;
   };
   inline bool getBufCopy(void){
      return BufCopy;
   };
   inline Graphics::TBitmap* getBuf(void){
      return Buf;
   };

   void __fastcall updateGraphics(void);
   
   inline Gdiplus::Graphics* getGraphics(void){
      return gs;
   };
   inline void setEditProcessing(bool to){
      EditProcessed = to;
   };
   inline bool getEditProcessing(void){
      return EditProcessed;
   };
   inline bool setShowHideState(bool to){
      ShowHideGP = to;
      return to;
   };
   inline bool getShowHideState(void){
      return ShowHideGP;
   };
   inline void setUpdate(bool s){Update = s;};
   inline bool getUpdate(void){return Update;};
};
#endif

 