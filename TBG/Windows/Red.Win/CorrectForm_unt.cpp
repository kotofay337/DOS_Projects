//---------------------------------------------------------------------------
#include <stdio.h>
#include <vcl.h>

#include "Screen.h"
#include "ToolBar.h"
#include "PropDlg1.h"

#include "tbg.hpp"
#include "TBG_Editor.hpp"
#include "TextBGI_unt.h"
#include "CorrectForm_unt.h"
#include "SelectBGIFontsForm.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCorrectForm *CorrectForm;
//---------------------------------------------------------------------------
__fastcall TCorrectForm::TCorrectForm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------



void __fastcall TCorrectForm::CheckBoxClick(TObject *Sender)
{
   switch(((TCheckBox*)Sender)->Tag){
      case 1:
         SpeedBar->sbCorrect->Down =
         SpeedBar->sbCorrect->Down && CheckBox1->Checked ? true : false;
         break;
   }/* switch */
}
//---------------------------------------------------------------------------


