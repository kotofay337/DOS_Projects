//---------------------------------------------------------------------------

#ifndef TestWTBGH
#define TestWTBGH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>

#include "tbg.hpp"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TTestForm : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TOpenDialog *OpenDialog1;
	TStatusBar *StatusBar1;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
   void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
        __fastcall TTestForm(TComponent* Owner);
        TPictureTBG *PictureTBG;
        ULONG_PTR token;

};
//---------------------------------------------------------------------------
extern PACKAGE TTestForm *TestForm;
//---------------------------------------------------------------------------
#endif
