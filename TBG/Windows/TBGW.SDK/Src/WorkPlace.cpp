
#include "tbg.hpp"

__fastcall TWorkPlace::TWorkPlace(){
      maxX = 0; 
      maxY = 0; 

      ShowHideGP = true; 
      EditProcessed = false;

      Buf = new Graphics::TBitmap;
      Buf->PixelFormat = pf32bit;
      setSizeScreen(640, 480);

      if(Buf){
         BufCopy = false;
         CanvasBuf = Buf->Canvas;
         gs = new Gdiplus::Graphics(Buf->Canvas->Handle); 
         gsCopy = false; 
      }// if
      CanvasOut = NULL;
      Update = false;
};
// ----------------------------------------------------------------------------
__fastcall TWorkPlace::TWorkPlace(TCanvas *c, bool EditShow){
      maxX = 0; 
      maxY = 0; 

      ShowHideGP = true; 
      EditProcessed = EditShow ? true : false;

      if(!EditShow){
         Buf = new Graphics::TBitmap;
         Buf->PixelFormat = pf32bit;
         setSizeScreen(640, 480);

         if(Buf){
            BufCopy = false;
            CanvasBuf = Buf->Canvas;
            gs = new Gdiplus::Graphics(Buf->Canvas->Handle); 
            gsCopy = false; 
         }// if
         CanvasOut = c;
      }// if
      else{
         Buf = NULL;
         BufCopy = true;
         CanvasBuf = c;
         if(c){
            gs = new Gdiplus::Graphics(c->Handle); 
            gsCopy = false; 
            CanvasOut = c;
         }// if
      }// elif
      Update = false;
};
// ----------------------------------------------------------------------------
TWorkPlace::~TWorkPlace(){
   if(gs && (gsCopy == false)){
      delete gs;
      gs = NULL;
   }// if
   if(Buf && (BufCopy == false)){
      delete Buf;
      Buf = NULL;
   }// if
};
// ----------------------------------------------------------------------------
const TWorkPlace& __fastcall TWorkPlace::operator=(const TWorkPlace& R){
   this->~TWorkPlace();
   CanvasOut = R.CanvasOut;

   maxX = R.maxX;
   maxY = R.maxY;

   gs = R.gs;
   gsCopy = true;

   Buf = R.Buf;
   CanvasBuf = R.CanvasBuf;
   BufCopy = true;

   return *this;
};
// ----------------------------------------------------------------------------
void __fastcall TWorkPlace::setCanvasOut(TCanvas *c){
   if(c){
      CanvasOut = c;
   }// if
};
// ----------------------------------------------------------------------------
void __fastcall TWorkPlace::updateGraphics(void){
   if(!gsCopy){
      if(gs){
         delete gs;
      }// if
      gs = new Gdiplus::Graphics(getCanvas()->Handle);
   }// if
};
// ----------------------------------------------------------------------------
 