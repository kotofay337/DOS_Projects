//---------------------------------------------------------------------------

#ifndef TextBGI_untH
#define TextBGI_untH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TTextBGI : public TForm
{
__published:	// IDE-managed Components
   TEdit *TextBGIEdit;
   TEdit *Edit1;
   TUpDown *UpDown1;
	TEdit *Edit2;
	TUpDown *UpDown2;
   void __fastcall TextBGIEditKeyPress(TObject *Sender, char &Key);
   void __fastcall TextBGIEditChange(TObject *Sender);
   void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
   void __fastcall FormActivate(TObject *Sender);
   void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
	void __fastcall UpDown2Click(TObject *Sender, TUDBtnType Button);
	void __fastcall Edit2KeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
   __fastcall TTextBGI(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTextBGI *TextBGI;
//---------------------------------------------------------------------------
#endif
