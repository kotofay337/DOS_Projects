//---------------------------------------------------------------------------

#ifndef TestOLEH
#define TestOLEH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <OleCtnrs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
   TOleContainer *OleContainer1;
   TMainMenu *MainMenu1;
   TOpenDialog *OpenDialog1;
   TSaveDialog *SaveDialog1;
   TMenuItem *N1;
   TMenuItem *MNew;
   TMenuItem *MOpen;
   TMenuItem *MSave;
   TMenuItem *MClose;
   void __fastcall MNewClick(TObject *Sender);
   void __fastcall MCloseClick(TObject *Sender);
   void __fastcall MSaveClick(TObject *Sender);
   void __fastcall MOpenClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
