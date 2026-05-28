//---------------------------------------------------------------------------

#ifndef MacroSelect_frmH
#define MacroSelect_frmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMacroSelect : public TForm
{
__published:	// IDE-managed Components
   TTreeView *MTree;
   TButton *Button1;
   TButton *Button2;
   TButton *Button3;
	TCheckBox *PrevScrEnable;
   TButton *Button4;
   TButton *Button5;
	TImage *PrevScr;
   void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall PrevScrEnableClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall MTreeChange(TObject *Sender, TTreeNode *Node);
private:	// User declarations
public:		// User declarations
   __fastcall TMacroSelect(TComponent* Owner);
   TTreeView* getSelectedMacro(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TMacroSelect *MacroSelect;
//---------------------------------------------------------------------------
#endif
