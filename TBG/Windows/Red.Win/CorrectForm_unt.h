//---------------------------------------------------------------------------

#ifndef CorrectForm_untH
#define CorrectForm_untH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TCorrectForm : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TGroupBox *GroupBox2;
   TGroupBox *GroupBox3;
   TCheckBox *CheckBox1;
   TGroupBox *GroupBox4;
	TCheckBox *cbxLineStyle;
	TCheckBox *cbxLineColor;
	TCheckBox *cbxLineThick;
	TCheckBox *cbxFillStyle;
	TCheckBox *cbxFillColor;
	TCheckBox *cbxTextStyle;
	TCheckBox *cbxTextColor;
   TCheckBox *CheckBox9;
	TCheckBox *cbxText;
   TCheckBox *CheckBox11;
	TCheckBox *cbxTextSize;
   void __fastcall CheckBoxClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TCorrectForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCorrectForm *CorrectForm;
//---------------------------------------------------------------------------
#endif
