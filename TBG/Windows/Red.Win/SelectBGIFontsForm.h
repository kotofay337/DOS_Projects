//---------------------------------------------------------------------------

#ifndef SelectBGIFontsFormH
#define SelectBGIFontsFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSelectBGI : public TForm
{
__published:	// IDE-managed Components
   TImage *Image1;
   TRadioButton *rbFontBGI1;
   TImage *Image2;
   TRadioButton *rbFontBGI2;
   TImage *Image3;
   TRadioButton *rbFontBGI3;
   TImage *Image4;
   TRadioButton *rbFontBGI4;
   TImage *Image5;
   TRadioButton *rbFontBGI5;
   TImage *Image6;
   TRadioButton *rbFontBGI;
   TImage *Image7;
   TRadioButton *rbFontBGI7;
   TImage *Image8;
   TRadioButton *rbFontBGI8;
   TImage *Image9;
   TRadioButton *rbFontBGI9;
   TImage *Image10;
   TRadioButton *rbFontBGI10;
   void __fastcall FormPaint(TObject *Sender);
   void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormKeyPress(TObject *Sender, char &Key);
   void __fastcall rbFontBGIClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TSelectBGI(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSelectBGI *SelectBGI;
//---------------------------------------------------------------------------
#endif
