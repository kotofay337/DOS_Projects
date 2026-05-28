//---------------------------------------------------------------------------

#ifndef TestWTBGH
#define TestWTBGH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>

#include "tbg_tc.h"
//---------------------------------------------------------------------------
class TTestForm : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TOpenDialog *OpenDialog1;
   TCheckBox *CheckBoxSmooth;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TTestForm(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TTestForm *TestForm;
//---------------------------------------------------------------------------
#endif
