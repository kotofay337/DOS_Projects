//---------------------------------------------------------------------------

#ifndef About_frmH
#define About_frmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TAbout : public TForm
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TImage *Image1;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *Label5;
   TLabel *Label6;
   TButton *Button1;
   TLabel *Label7;
   void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAbout *About;
//---------------------------------------------------------------------------
#endif
