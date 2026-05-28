//---------------------------------------------------------------------------

#ifndef LoadPreviewH
#define LoadPreviewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "tbg.hpp"
//---------------------------------------------------------------------------
class TLoadPreview_frm : public TForm
{
__published:	// IDE-managed Components
   void __fastcall FormPaint(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TLoadPreview_frm(TComponent* Owner);

   TPictureTBG pic;
};
//---------------------------------------------------------------------------
extern PACKAGE TLoadPreview_frm *LoadPreview_frm;
//---------------------------------------------------------------------------
#endif
