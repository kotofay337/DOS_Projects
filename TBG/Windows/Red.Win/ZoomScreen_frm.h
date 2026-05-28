//---------------------------------------------------------------------------

#ifndef ZoomScreen_frmH
#define ZoomScreen_frmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TZoomScreen : public TForm
{
__published:	// IDE-managed Components
   TTimer *Timer1;
   void __fastcall Timer1Timer(TObject *Sender);
   void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
   void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
   bool MoveWindow;
   int WX, WY;
   int Zoom, XZoom;
   __fastcall TZoomScreen(TComponent* Owner);
   void ZoomDisplay(void);
   void ZoomOn(int xzoom);
   void ZoomOff(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TZoomScreen *ZoomScreen;
//---------------------------------------------------------------------------
#endif
