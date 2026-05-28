//---------------------------------------------------------------------------

#ifndef ScreenH
#define ScreenH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>

#include <ComCtrls.hpp>
#include <OleCtnrs.hpp>

#include "tbg.hpp"
#include <CustomizeDlg.hpp>
#include <ActnCtrls.hpp>
#include <ActnMan.hpp>
#include <ToolWin.hpp>
#pragma hdrstop
//---------------------------------------------------------------------------
class TPainter : public TForm
{
__published:	// IDE-managed Components
        TPopupMenu *ppmScreen;
        TMenuItem *ss640_480;
        TMenuItem *ss800_600;
        TMenuItem *ss1024_768;
        TMenuItem *ssFullScreen;
        TMenuItem *ssWindow;
        TMenuItem *ssFrame;
   TMenuItem *N1;
   TMenuItem *miPrint;
   TMenuItem *miClosePicture;
   TTimer *Timer1;
   TMenuItem *ssZoomDisplay;
   TTimer *Timer2;
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall ss640_480Click(TObject *Sender);
        void __fastcall ssFullScreenClick(TObject *Sender);
        void __fastcall ssWindowClick(TObject *Sender);
        void __fastcall ss800_600Click(TObject *Sender);
        void __fastcall ss1024_768Click(TObject *Sender);
        void __fastcall ssFrameClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
   void __fastcall miPrintClick(TObject *Sender);
   void __fastcall miClosePictureClick(TObject *Sender);
   void __fastcall Timer1Timer(TObject *Sender);
   void __fastcall ssZoomDisplayClick(TObject *Sender);
   void __fastcall Timer2Timer(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TPainter(TComponent* Owner);

      TPictureTBG *PictureTBG;
      unsigned EditGPCount;
      TBaseGP *EditGP, *SaveGP;
      int IndexLastMacro;

      bool RcSel, 
           ShowSelected,
           PntCenterRotateFlag;

      TRect SelRect;
      TPoint MovePntSel;

      TPoint PntCenterRotate; 
      double AngleRotate;

      LinkList<TPointf*> lstpts;
      TPointf *EditPoint;

      TCursor SaveCursor;

};
//---------------------------------------------------------------------------
extern PACKAGE TPainter *Painter;
//---------------------------------------------------------------------------
#endif
