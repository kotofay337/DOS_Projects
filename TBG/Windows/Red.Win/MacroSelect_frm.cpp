//---------------------------------------------------------------------------

#include <vcl.h>
#include <Registry.hpp>
#include <Printers.hpp>
#include <math.h>


#include "Screen.h"
#include "ToolBar.h"
#include "PropDlg1.h"
#include "ZoomScreen_frm.h"
#include "MacroSelect_frm.h"

#include "tbg.hpp"
#include "TBG_Editor.hpp"
#include "tbg_tc.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMacroSelect *MacroSelect;
//---------------------------------------------------------------------------
__fastcall TMacroSelect::TMacroSelect(TComponent* Owner)
   : TForm(Owner)
{
   char *TBGPath = getenv("TBGRESOURCESPATH");
   if(TBGPath == NULL)
      TBGPath = "C:\\TBG\\BIN\\";
   mtv = new TMacroTreeView(NULL);
   mtv->SetPathToMacro(TBGPath);
   mtv->SetTreeView(MTree);
   mtv->LoadMacroTree("Macro.tree");
}
//---------------------------------------------------------------------------
TTreeView* TMacroSelect::getSelectedMacro(void){
   return MTree->Selected ? MTree : NULL;
};
//---------------------------------------------------------------------------
void __fastcall TMacroSelect::Button4Click(TObject *Sender)
{
   Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TMacroSelect::Button1Click(TObject *Sender)
{
//   TTreeNode* mn = mtv->SearchOnIndex(1U);
//   TTreeNode* s  = mtv->SearchState(mn, 4U);
//   mtv->ExecuteState(s, NULL);
}
//---------------------------------------------------------------------------


void __fastcall TMacroSelect::PrevScrEnableClick(TObject *Sender)
{
   PrevScr->Canvas->Pen->Color = clCream;
   PrevScr->Canvas->Pen->Style = psSolid;
   PrevScr->Canvas->Brush->Color = clCream;
   PrevScr->Canvas->Brush->Style = bsSolid;
   PrevScr->Canvas->Rectangle(0, 0, PrevScr->ClientWidth, PrevScr->ClientHeight);
   if(PrevScrEnable->Checked){
      MTreeChange(Sender, MTree->Selected);
   }// if
}
//---------------------------------------------------------------------------

void __fastcall TMacroSelect::FormDestroy(TObject *Sender)
{
   mtv->DeleteAllPic();
}
//---------------------------------------------------------------------------
void __fastcall TMacroSelect::MTreeChange(TObject *Sender, TTreeNode *Node)
{

   unsigned Index, State;
   if(Node && PrevScrEnable->Checked){
      Index = mtv->SearchMacroAsIndex(Node);
      State = mtv->SearchStateAsIndex(Node);
      if(State == 0U)
         State = 4U;

      TMacro m(Index, State, TPointf(0.0, 0.0));
      Graphics::TBitmap *Bitmap = new Graphics::TBitmap;
      Bitmap->PixelFormat = pf32bit;
      Bitmap->Width = PrevScr->ClientWidth;
      Bitmap->Height = PrevScr->ClientHeight;
      
      TWorkPlace wp(Bitmap->Canvas, true);
      wp.setSizeScreen(639, 479);
      wp.setEditProcessing(false);
      m.setOffset(TPointf(PrevScr->ClientWidth / 2. / 640., PrevScr->ClientHeight / 2. / 480.));
      m.Show(&wp);

      PrevScr->Canvas->Draw(0, 0, Bitmap);
      
      delete Bitmap;
   }// if
}
//---------------------------------------------------------------------------

