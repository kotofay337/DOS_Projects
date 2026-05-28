//---------------------------------------------------------------------------
      
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include <vcl.h>
#include <jpeg.hpp>
#include <Registry.hpp>
#include "About_frm.h"

#include "Screen.h"
#include "ToolBar.h"
#include "PropDlg1.h"
#include "LoadPreview.h"

#include "tbg.hpp"
#include "tbg_tc.h"
#include "TBG_Editor.hpp"
#include "TextBGI_unt.h"
#include "CorrectForm_unt.h"
#include "SelectBGIFontsForm.h"
#pragma hdrstop


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSpeedBar *SpeedBar;
//---------------------------------------------------------------------------
void shpLineInvalidate(void){
   TImage *img = SpeedBar->shpLine;
   TCanvas *cvs = img->Canvas;
   cvs->Pen->Color = ColorLine;
   cvs->Pen->Width = WidthLine;
   cvs->Brush->Color = (TColor)0x00F0FBFF;
   cvs->Brush->Style = bsSolid;
   TRect r;
   r.Left = 0;
   r.Top = 0;
   r.Right = img->Width;
   r.Bottom = img->Height;
   cvs->FillRect(r);
   cvs->MoveTo(1, img->Height / 2);
   cvs->LineTo(img->Width - 1, img->Height / 2);
   if(WidthLine >= img->Height - 2){
      AnsiString s;
      s = WidthLine;
      cvs->TextOut(img->Width / 2, 3, s);
   }/* if */
}/* shpLineInvalidate */
//---------------------------------------------------------------------------
void shpRectInvalidate(void){
   TImage *img = SpeedBar->shpRect;
   TCanvas *cvs = img->Canvas;
   cvs->Pen->Color = ColorLine;
   cvs->Pen->Width = WidthLine;
   TRect r;
   r.Left = 1;
   r.Top = 1;
   r.Right = img->Width - 1;
   r.Bottom = img->Height - 1;
   if(SpeedBar->miBarGradient->Checked && SpeedBar->miBar->Checked){
      HDC hDC = cvs->Handle;

      switch(GradientStyleFill){
         case gfsLeftToRight:
         case gfsLeftTopToRightBottom:
         case gfsUpToDown:
         case gfsRightTopToLeftBottom:
            GradientRectangle(hDC, 1, 1, img->Width - 1, img->Height - 1, ColorLine, ColorFill, GradientStyleFill);
            break;
         case gfsVertical:
         case gfsDiagonal:
         case gfsHorizontal:
         case gfsBackDiagonal:
            SymGradientRectangle(hDC, 1, 1, img->Width - 1, img->Height - 1, ColorLine, ColorFill, GradientStyleFill);
            break;
         case gfsLeftTopCorner:     // 8
         case gfsRightTopCorner:    // 9
         case gfsRightBottomCorner: // 10
         case gfsLeftBottomCorner:  // 11
            CornerGradientRectangle(hDC, 1, 1, img->Width - 1, img->Height - 1, ColorLine, ColorFill, GradientStyleFill - gfsLeftTopCorner);
            break;
         case gfsCenter:
            CenterGradientRectangle(hDC, 1, 1, img->Width - 1, img->Height - 1, ColorLine, ColorFill);
            break;
      }// switch

   }// if
   else{
      cvs->Brush->Color = (TColor)0x00F0FBFF;
      cvs->Brush->Style = bsSolid;
      cvs->Rectangle(r);
      cvs->Brush->Color = ColorFill;
      cvs->Brush->Style = StyleFill;
      cvs->Rectangle(r);
   }// elif
   img->Repaint();
}/* shpRectInvalidate */
//---------------------------------------------------------------------------
void shpTextInvalidate(void){
   TImage *img = SpeedBar->shpText;
   TCanvas *cvs = img->Canvas;
   cvs->Font->Color = ColorText;
   cvs->Font->Name = "Times New Roman";
   cvs->Font->Height = 22;
   cvs->Brush->Color = (TColor)0x00F0FBFF;
   cvs->Brush->Style = bsSolid;
   TRect r;
   r.Left = 0;
   r.Top = 0;
   r.Right = img->Width;
   r.Bottom = img->Height;
   cvs->FillRect(r);

   AnsiString s;
   s = "T";
   cvs->TextOut(3, 3, s);
}/* shpLineInvalidate */
//---------------------------------------------------------------------------
__fastcall TSpeedBar::TSpeedBar(TComponent* Owner)
        : TForm(Owner)
{

   Gdiplus::Status s;
   Gdiplus::GdiplusStartupInput input;
   Gdiplus::GdiplusStartupOutput output;

   s = Gdiplus::GdiplusStartup(&token, &input, &output);


   Timer1->Enabled = false;
   if(vos.dwMajorVersion >= ((DWORD)5)){
     //HWND m_hWnd = SpeedBar->Handle;
     //SetWindowLong(m_hWnd, GWL_EXSTYLE,
     //              GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
     //SetLayeredWindowAttributes(m_hWnd, RGB(0,0,1), 1, LWA_ALPHA | LWA_COLORKEY);
     AlphaBlendValue = 1;
     Timer1->Enabled = true;
   }// if

   SpeedBar->Left = 1;
   SpeedBar->Top = Screen->WorkAreaHeight - SpeedBar->Height - 1;
   pc = cZero;
   char *TBGPath = getenv("TBGRESOURCESPATH");
   if(TBGPath == NULL)
      TBGPath = "C:\\TBG\\BIN\\";
   SetPathToBGIFont(TBGPath);
   MPCSetPath(TBGPath);
/*
   TBGReg = new TRegistry;
   s = "\\Software\\TBG";

   TBGReg->RootKey = HKEY_CURRENT_USER;
   if(!TBGReg->KeyExists(s)){
      TBGReg->CreateKey(s);
      TBGReg->OpenKey(s, true);

      key = "Version";
      val = "7.0";
      TBGReg->WriteString(key, val);
     
      key = "CurrentPath";
      val = "";
      TBGReg->WriteString(key, val);
     
      key = "PosScreen";
      val = "0,0";
      TBGReg->WriteString(key, val);
     
      key = "SizeScreen";
      val = "800x600";
      TBGReg->WriteString(key, val);
     
      key = "ModeScreen";
      val = "Windowed";
      TBGReg->WriteString(key, val);
     
      key = "DeleteMode";
      TBGReg->WriteInteger(key, 2);
     
      TBGReg->CloseKey();
   }/* if */
   else{
      TBGReg->OpenKey(s, false);
      val = TBGReg->ReadString("SizeScreen");
      switch(TBGReg->ReadInteger("DeleteMode")){
         case 1:
            miDelAll->Checked = true;
            break;
         case 2:
            miDelCurrent->Checked = true;
            break;
         case 3:
            miDelSelected->Checked = true;
            break;
      }/* switch */

      switch(TBGReg->ReadInteger("CopyMode")){
         case 1:
            miCopyWLogic->Checked = true;
            break;
         case 2:
            miCopyWOLogic->Checked = true;
            break;
      }/* switch */
   }/* elif */
*/
   shpLineInvalidate();
   shpRectInvalidate();
   shpTextInvalidate();
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbQuitClick(TObject *Sender){
   UnloadAllFont();
   ClearClipboard();
   Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::sbLoadClick(TObject *Sender){
   static int IndexFile = 1;
   AnsiString cp;
   Application->CreateForm(__classid(TPainter), &Painter);

   EditPic = Painter->PictureTBG;

   EditPic->WorkPlace()->setShowHideState(N1->Checked);
//   EditPic->WorkPlace()->setEditProcessing(false);

   OpenDialog1->Filter = "TBG для DOS|*.TBG|TBG для Windows|*.wTBG|Все файлы TBG|*.wTBG;*.TBG;*.TBL;*.TBX";
   if(OpenDialog1->Execute()){
      D(printf("\nload file for editing: %s", OpenDialog1->FileName.c_str()));

      TCursor Save_Cursor = Screen->Cursor;
      Screen->Cursor = crHourGlass;
      try{
         EditPic->Load(OpenDialog1->FileName.c_str());
      }// try
      __finally{
        Screen->Cursor = Save_Cursor; // always restore the cursor
      }// finally

      Painter->Caption = OpenDialog1->FileName;
      Painter->ClientWidth = EditPic->GetSizeX();
      Painter->ClientHeight = EditPic->GetSizeY();
      Painter->WindowState = wsNormal;
      Painter->Repaint();
      FormActivate(Owner);
   }/* if */
   else{
      do{
         cp = GetCurrentDir();
         cp += "\\File ";
         cp += IndexFile++;
         cp += ".wTBG";
      }while(FileExists(cp));
      Painter->Caption = cp;
   }/* elif */

   if(Painter)
      Painter->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbSaveClick(TObject *Sender)
{
   char *sc = "Сохранение файла";

   if(!EditPic){
      Application->MessageBox("Нет рисунка!", sc, MB_OK | MB_ICONSTOP);
      return;
   }/* elif */

   SavePictureDialog1->Filter = "Файлы TBG для Windows(*.wTBG)|*.wTBG|Метафайл Windows(*.emf)|*.emf|Windows Bitmap(*.bmp)|*.bmp|JPEG Image(*.jpeg)|*.jpeg";
   SavePictureDialog1->FileName = Painter->Caption;
   if(SavePictureDialog1->Execute()){
      AnsiString exts[] = {
         ".wTBG",
         ".emf",
         ".bmp",
         ".jpeg"
      };
      AnsiString FileName = ChangeFileExt(SavePictureDialog1->FileName, exts[SavePictureDialog1->FilterIndex - 1]);
      AnsiString FileExt = ExtractFileExt(FileName);

      if(FileExists(FileName)){
         char sm[256];
         sprintf(sm, "Файл \"%s\" уже существует,\nзаменить его?", FileName.c_str());
         if(Application->MessageBox(sm, sc, MB_YESNO | MB_ICONWARNING) == IDCANCEL){
            return;
         }/* if */
      }/* if */

      TCursor Save_Cursor = Screen->Cursor;
      Screen->Cursor = crHourGlass;
      try{
         if(!FileExt.UpperCase().AnsiCompare(".WTBG")){
               EditPic->Save(FileName.c_str());
               Painter->Caption = FileName;
         }/* elif */

         if(!FileExt.UpperCase().AnsiCompare(".EMF")){
            TMetafile *pMetafile = new TMetafile;
            TMetafileCanvas *cvs = new TMetafileCanvas(pMetafile, 0);
            if(Painter->PictureTBG){
               Painter->PictureTBG->setSizeScreen(Painter->ClientWidth, Painter->ClientHeight);
               Painter->PictureTBG->setCanvas(cvs);
               Painter->PictureTBG->Show();
               delete cvs;
               pMetafile->SaveToFile(FileName);
            }/* if */
            else
               delete cvs;
            delete pMetafile;
         }//

         if(!FileExt.UpperCase().AnsiCompare(".BMP")){
            Graphics::TBitmap *Bitmap = new Graphics::TBitmap;
            if(Painter->PictureTBG){
               Bitmap->PixelFormat = pf32bit;
               Bitmap->Width = Painter->ClientWidth;
               Bitmap->Height = Painter->ClientHeight;
               Painter->PictureTBG->setSizeScreen(Painter->ClientWidth, Painter->ClientHeight);
               Painter->PictureTBG->setCanvas(Bitmap->Canvas);
               Painter->PictureTBG->WorkPlace()->setEditProcessing(false);
               Painter->PictureTBG->Show();
               Bitmap->SaveToFile(FileName);
            }/* if */
            delete Bitmap;
         }//

         if(!FileExt.UpperCase().AnsiCompare(".JPEG")){
            Graphics::TBitmap *Bitmap = new Graphics::TBitmap;
            if(Painter->PictureTBG){
               Bitmap->PixelFormat = pf32bit;
               Bitmap->Width = Painter->ClientWidth;
               Bitmap->Height = Painter->ClientHeight;
               Painter->PictureTBG->setSizeScreen(Painter->ClientWidth, Painter->ClientHeight);
               Painter->PictureTBG->setCanvas(Bitmap->Canvas);
               Painter->PictureTBG->WorkPlace()->setEditProcessing(false);
               Painter->PictureTBG->Show();
               {
                 TJPEGImage *jp = new TJPEGImage();
                 jp->CompressionQuality = 92;
                 try
                 {
                   jp->Assign(Bitmap);
                   jp->SaveToFile(FileName);
                 }
                 __finally
                 {
                   delete jp;
                 }
               }
            }/* if */
            delete Bitmap;
         }//

      }// try
      __finally{
        Screen->Cursor = Save_Cursor; // always restore the cursor
      }// finally
   }/* if */
}
//---------------------------------------------------------------------------


void __fastcall TSpeedBar::sbEllipseClick(TObject *Sender)
{
   Commands set = miEllipse->Checked ? cEllipse :
                  miArc->Checked     ? cArc     :
                  miFillArc->Checked ? cFillArc : cZero;
   pc = sbEllipse->Down ? set : cZero;

   if(pc != cZero){
      sbEllipsePlus->Visible = true;
      sbEllipseMinus->Visible = true;
   }
   else{
      sbEllipsePlus->Visible = false;
      sbEllipseMinus->Visible = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbLineClick(TObject *Sender)
{
   Commands set = miLineSimple->Checked  ? cLine      :
                  miLineStyled->Checked  ? cStyleLine :
                  miLinePoly->Checked    ? cPolyline  :
                  miLineBezier->Checked  ? cBezier    :
                  miLinePolygon->Checked ? cPolygon   :
                  miLinePipe->Checked    ? cPipe      : cZero;

   pc = sbLine->Down ? set : cZero;
   sbEllipsePlus->Visible = false;
   sbEllipseMinus->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbRectClick(TObject *Sender)
{
   Commands set = miRect2p->Checked ? cFrameRect :
                  miRect4p->Checked ? cPolyRect  : cZero;
   
   pc = sbRect->Down ? set : cZero;
   sbEllipsePlus->Visible = false;
   sbEllipseMinus->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbBarClick(TObject *Sender)
{
   Commands set = miBarGradient->Checked ? cGradientFillFrameRect :
                  miBar->Checked         ? cFillFrameRect :
                  miBarPoly->Checked     ? cPolyFillFrameRect : cZero;

   pc = sbBar->Down ? set : cZero;

   sbEllipsePlus->Visible = false;
   sbEllipseMinus->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbTextClick(TObject *Sender)
{
   Commands set = miTrueType->Checked? cTextTrueType :
                  miBGI->Checked     ? cTextBGI      : cZero;

   pc = sbText->Down ? set : cZero;

   sbEllipsePlus->Visible = false;
   sbEllipseMinus->Visible = false;

   if(pc == cTextBGI || 
      pc == cTextTrueType){
      if(!TextBGI){
         Application->CreateForm(__classid(TTextBGI), &TextBGI);
      }/* if */
      else{
        if(!TextBGI->Visible){
           TextBGI->Visible = true;
        }/* if */
      }/* if */
      TextBGI->TextBGIEdit->Text = sTextBGI;
   }/* if */
   else{
     TextBGI->Visible = false;
   }/* elif */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbMacroClick(TObject *Sender)
{
   pc = sbMacro->Down ? cMacro : cZero;
   if(Painter)
      Painter->IndexLastMacro = -1;
   sbEllipsePlus->Visible = false;
   sbEllipseMinus->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::sbMoveClick(TObject *Sender)
{
   pc = sbMove->Down ? (miMoved->Checked ? cMove : cRotate) : cZero;
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::TimeDateTimer(TObject *Sender)
{
   StatusBar1->Panels->Items[2]->Text =
      FormatDateTime("dddddd, dddd, hh:mm.ss", Now());
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbDelClick(TObject *Sender)
{
   if(EditPic){
      if(miDelSelected->Checked == true){
         EditPic->DeleteSelectedGP();
      }/* if */
      if(miDelCurrent->Checked == true){
         EditPic->DeleteCurrentGP();
      }/* if */
      if(miDelAll->Checked == true){
         EditPic->DeleteAllGP();
         miDelAll->Checked = false;
         miDelSelected->Checked = false;
         miDelCurrent->Checked = true;
      }/* if */

      Painter->Repaint();
      FormActivate(Owner);
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::FormActivate(TObject *Sender)
{
   if(EditPic){
      StatusBar1->Panels->Items[1]->Text = EditPic->getChangeStatus() ? "Не сохранён!" : "";
      sbAllGE->Caption = EditPic->GetCount();
      sbCurrentGE->Caption = EditPic->IndexCurrentGP();
      WuSmoothLine->Checked = !EditPic->WorkPlace()->getEditProcessing();
      TBaseGP *gp = EditPic->GetGP();
      if(gp){
         EditOBJ->Text = gp->GetNo();
         EditNUM->Text = gp->GetNe();
         sbCurrentLE->ShowHint = true;
         sbCurrentLE->Hint = gp->GetID();
         sbAllLE->Caption = gp->GetCount();
         sbCurrentLE->Caption = gp->IndexCurrentLE();
         eGEName->Text = gp->GetName();
         eFileName->Text = Painter->Caption;
      }/* if */
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::WuSmoothLineClick(TObject *Sender)
{
   WuSmoothLine->Checked = !WuSmoothLine->Checked;
   if(Painter && EditPic){
      EditPic->WorkPlace()->setEditProcessing(!WuSmoothLine->Checked);
      Painter->Invalidate();
      EditPic->setChangeStatus(true);
      FormActivate(NULL);
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::N1Click(TObject *Sender)
{
   N1->Checked = !N1->Checked;
   if(Painter){
      EditPic->WorkPlace()->setShowHideState(N1->Checked);
      Painter->Invalidate();
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::sbNUMMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(EditPic){
      if(Button == mbLeft){
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            int cNe = gp->GetNe(); 
            // no logic
            if(cNe == -1){
               return;
            }/* if */
            // dcr NumElm
            cNe--;
            if(cNe < 0)
               cNe = 255;
            gp->SetNe(cNe);
            EditNUM->Text = cNe;
            sbCurrentLE->ShowHint = true; 
            sbCurrentLE->Hint = gp->GetID(); 
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
         }/* if */
      }/* if */
      if(Button == mbRight){
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            int cNe = gp->GetNe(); 
            // no logic
            if(cNe == -1){
               return;
            }/* if */
            // inc NumElm
            cNe++;
            if(cNe > 255)
               cNe = 0;
            gp->SetNe(cNe);   
            EditNUM->Text = cNe;
            sbCurrentLE->ShowHint = true; 
            sbCurrentLE->Hint = gp->GetID(); 
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
         }/* if */
      }/* if */
   }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbCurrentLEMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(EditPic){
      if(Button == mbLeft){
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            gp->PrevLE();
            EditOBJ->Text = gp->GetNo();
            EditNUM->Text = gp->GetNe();
            sbCurrentLE->ShowHint = true;
            sbCurrentLE->Hint = gp->GetID();
            sbCurrentLE->Caption = gp->IndexCurrentLE();
         }/* if */
      }/* if */
      if(Button == mbRight){
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            gp->NextLE(); 
            EditOBJ->Text = gp->GetNo(); 
            EditNUM->Text = gp->GetNe(); 
            sbCurrentLE->ShowHint = true; 
            sbCurrentLE->Hint = gp->GetID(); 
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
         }/* if */
      }/* if */
   }/* if */
}
//---------------------------------------------------------------------------


void __fastcall TSpeedBar::sbAllLEMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(EditPic){
      if(Button == mbLeft){
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            gp->FirstLE(); 
            EditOBJ->Text = gp->GetNo(); 
            EditNUM->Text = gp->GetNe(); 
            sbCurrentLE->ShowHint = true; 
            sbCurrentLE->Hint = gp->GetID(); 
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
            sbAllLE->Caption = gp->GetCount();
         }/* if */
      }/* if */
      if(Button == mbRight){
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            gp->LastLE(); 
            EditOBJ->Text = gp->GetNo(); 
            EditNUM->Text = gp->GetNe(); 
            sbCurrentLE->ShowHint = true; 
            sbCurrentLE->Hint = gp->GetID(); 
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
            sbAllLE->Caption = gp->GetCount();
         }/* if */
      }/* if */
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::sbCurrentGEMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(EditPic){
      if(Button == mbLeft){
         EditPic->PrevGP();
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            EditOBJ->Text = gp->GetNo(); 
            EditNUM->Text = gp->GetNe(); 
            sbCurrentLE->ShowHint = true; 
            sbCurrentLE->Hint = gp->GetID(); 
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
            sbAllLE->Caption = gp->GetCount();
            sbCurrentGE->Caption = EditPic->IndexCurrentGP(); 
            eGEName->Text = gp->GetName();
         }/* if */
      }/* if */
      if(Button == mbRight){
         EditPic->NextGP();
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            EditOBJ->Text = gp->GetNo(); 
            EditNUM->Text = gp->GetNe(); 
            sbCurrentLE->ShowHint = true; 
            sbCurrentLE->Hint = gp->GetID();
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
            sbAllLE->Caption = gp->GetCount();
            sbCurrentGE->Caption = EditPic->IndexCurrentGP(); 
            eGEName->Text = gp->GetName();
         }/* if */
      }/* if */
      Painter->Repaint();
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::sbAllGEMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(EditPic){
      if(Button == mbLeft){
         EditPic->FirstGP();
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            EditOBJ->Text = gp->GetNo(); 
            EditNUM->Text = gp->GetNe(); 
            sbCurrentLE->ShowHint = true; 
            sbCurrentLE->Hint = gp->GetID();
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
            sbCurrentGE->Caption = EditPic->IndexCurrentGP(); 
            sbAllLE->Caption = gp->GetCount();
            eGEName->Text = gp->GetName();
         }/* if */
      }/* if */
      if(Button == mbRight){
         EditPic->LastGP();
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            EditOBJ->Text = gp->GetNo(); 
            EditNUM->Text = gp->GetNe(); 
            sbCurrentLE->ShowHint = true; 
            sbCurrentLE->Hint = gp->GetID(); 
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
            sbCurrentGE->Caption = EditPic->IndexCurrentGP(); 
            sbAllLE->Caption = gp->GetCount();
            eGEName->Text = gp->GetName();
         }/* if */
      }/* if */
      Painter->Repaint();
   }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::EditNUMMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(EditPic){
      if(Button == mbLeft){
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            int cNe = gp->GetNe();
            // no logic
            if(cNe == -1){
               return;
            }/* if */
            // dcr NumElm
            if(Shift.Contains(ssShift)){
               cNe -= 10;
            }/* if */
            else{
               cNe--;
            }/* elif */
            if(cNe < 0)
               cNe = 65534;
            gp->SetNe(cNe);
            EditNUM->Text = cNe;
            sbCurrentLE->ShowHint = true;
            sbCurrentLE->Hint = gp->GetID();
            sbCurrentLE->Caption = gp->IndexCurrentLE();
         }/* if */
      }/* if */
      if(Button == mbRight){
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            int cNe = gp->GetNe();
            // no logic
            if(cNe == -1){
               return;
            }/* if */
            // inc NumElm
            if(Shift.Contains(ssShift)){
               cNe += 10;
            }/* if */
            else{
               cNe++;
            }/* elif */
            if(cNe > 65534)
               cNe = 0;
            gp->SetNe(cNe);
            EditNUM->Text = cNe;
            sbCurrentLE->ShowHint = true;
            sbCurrentLE->Hint = gp->GetID(); 
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
         }/* if */
      }/* if */
   }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::EditOBJMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(EditPic){
      if(Button == mbLeft){
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            int cNo = gp->GetNo(); 

            // no logic
            if(cNo == -1){
               return;
            }/* if */
            // dcr NumObj
            if(Shift.Contains(ssShift)){
               cNo -= 10;
            }/* if */
            else{
               cNo--;
            }/* elif */
            if(cNo < 0)
               cNo = 65534;
            gp->SetNo(cNo);
            EditOBJ->Text = cNo;
            sbCurrentLE->ShowHint = true; 
            sbCurrentLE->Hint = gp->GetID(); 
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
         }/* if */
      }/* if */
      if(Button == mbRight){
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            int cNo = gp->GetNo(); 

            // no logic
            if(cNo == -1){
               return;
            }/* if */
            // inc NumObj
            if(Shift.Contains(ssShift)){
               cNo += 10;
            }/* if */
            else{
               cNo++;
            }/* elif */
            if(cNo > 65534)
               cNo = 0;
            gp->SetNo(cNo);   
            EditOBJ->Text = cNo;
            sbCurrentLE->ShowHint = true; 
            sbCurrentLE->Hint = gp->GetID(); 
            sbCurrentLE->Caption = gp->IndexCurrentLE();
         }/* if */
      }/* if */
   }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::EditOBJChange(TObject *Sender)
{
   if(EditPic){
      TBaseGP *gp = EditPic->GetGP();
      if(gp){
         unsigned short int cNo = atoi(EditOBJ->Text.c_str()); 

         if((int)cNo < 0)
            cNo = 65534;

         if(cNo > 65534)
            cNo = 0;

         gp->SetNo(cNo);   
         EditOBJ->Text = cNo;
      }/* if */
   }/* if */
   else{
      EditOBJ->Text = -1;
   }/* elif */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::EditNUMChange(TObject *Sender)
{
   if(EditPic){
      TBaseGP *gp = EditPic->GetGP();
      if(gp){
         unsigned short int cNe = atoi(EditNUM->Text.c_str()); 

         if((int)cNe < 0)
            cNe = 65534;

         if(cNe > 65534)
            cNe = 0;

         gp->SetNe(cNe);   
         EditNUM->Text = cNe;
      }/* if */
   }/* if */
   else{
      EditNUM->Text = -1;
   }/* elif */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbAddLEClick(TObject *Sender)
{
   if(EditPic){
      TBaseGP *gp = EditPic->GetGP();
      if(gp){
         gp->InsertLE(0, 0);   
         EditOBJ->Text = gp->GetNo();
         EditNUM->Text = gp->GetNe(); 
         sbCurrentLE->ShowHint = true; 
         sbCurrentLE->Hint = gp->GetID();
         sbCurrentLE->Caption = gp->IndexCurrentLE(); 
         sbCurrentGE->Caption = EditPic->IndexCurrentGP(); 
         sbAllLE->Caption = gp->GetCount();
      }/* if */
   }/* if */
}
//---------------------------------------------------------------------------


void __fastcall TSpeedBar::sbDelLEMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if((Button == mbLeft)&&(Shift.Contains(ssShift))){
      if(EditPic){
         TBaseGP *gp = EditPic->GetGP();
         if(gp){
            gp->DeleteLE();   
            EditOBJ->Text = gp->GetNo(); 
            EditNUM->Text = gp->GetNe(); 
            sbCurrentLE->ShowHint = true;
            sbCurrentLE->Hint = gp->GetID(); 
            sbCurrentLE->Caption = gp->IndexCurrentLE(); 
            sbCurrentGE->Caption = EditPic->IndexCurrentGP(); 
            sbAllLE->Caption = gp->GetCount();
         }/* if */
      }/* if */
   }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::miDelAllClick(TObject *Sender)
{
   sbDel->Hint = "Удалить " + miDelAll->Caption;
   miDelAll->Checked = true;
   miDelCurrent->Checked = false;
   miDelSelected->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::miDelCurrentClick(TObject *Sender)
{
   sbDel->Hint = "Удалить " + miDelCurrent->Caption;   
   miDelAll->Checked = false;
   miDelCurrent->Checked = true;
   miDelSelected->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::miDelSelectedClick(TObject *Sender)
{
   sbDel->Hint = "Удалить " + miDelSelected->Caption;   
   miDelAll->Checked = false;
   miDelCurrent->Checked = false;
   miDelSelected->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::miCopyWLogicClick(TObject *Sender)
{
   sbCopy->Hint = "Копировать " + miCopyWLogic->Caption;   
   miCopyWLogic->Checked  = true;
   miCopyWOLogic->Checked = false;
   miMirrorX->Checked     = false;
   miMirrorY->Checked     = false;
   miMirrorXY->Checked    = false;
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::miCopyWOLogicClick(TObject *Sender)
{
   sbCopy->Hint = "Копировать " + miCopyWOLogic->Caption;   
   miCopyWLogic->Checked  = false;
   miCopyWOLogic->Checked = true;
   miMirrorX->Checked     = false;
   miMirrorY->Checked     = false;
   miMirrorXY->Checked    = false;
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::miMirrorXClick(TObject *Sender)
{
   sbCopy->Hint = "Копировать " + miMirrorX->Caption;   
   miCopyWLogic->Checked  = false;
   miCopyWOLogic->Checked = false;
   miMirrorX->Checked     = true;
   miMirrorY->Checked     = false;
   miMirrorXY->Checked    = false;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::miMirrorYClick(TObject *Sender)
{
   sbCopy->Hint = "Копировать " + miMirrorY->Caption;   
   miCopyWLogic->Checked  = false;
   miCopyWOLogic->Checked = false;
   miMirrorX->Checked     = false;
   miMirrorY->Checked     = true;
   miMirrorXY->Checked    = false;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::miMirrorXYClick(TObject *Sender)
{
   sbCopy->Hint = "Копировать " + miMirrorXY->Caption;   
   miCopyWLogic->Checked  = false;
   miCopyWOLogic->Checked = false;
   miMirrorX->Checked     = false;
   miMirrorY->Checked     = false;
   miMirrorXY->Checked    = true;
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::FormDestroy(TObject *Sender)
{
   TBGReg = new TRegistry;
   s = "\\Software\\TBG";

   TBGReg->RootKey = HKEY_CURRENT_USER;
   TBGReg->OpenKey(s, true);

   int i = 0;

   if(miDelAll->Checked)
      i = 1;
   if(miDelCurrent->Checked)
      i = 2;
   if(miDelSelected->Checked)
      i = 3;
   TBGReg->WriteInteger("DeleteMode", i);

   if(miCopyWLogic->Checked)
      i = 1;
   if(miCopyWOLogic->Checked)
      i = 2;
   TBGReg->WriteInteger("CopyMode", i);

   TBGReg->CloseKey();

   Gdiplus::GdiplusShutdown(token);

}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::shpLineMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   if(Shift.Contains(ssRight)){
      TStrings *cs = new TStringList();
      cs->Add("ColorA=000000");
      cs->Add("ColorB=AA0000");
      cs->Add("ColorC=00AA00");
      cs->Add("ColorD=AAAA00");
      cs->Add("ColorE=0000AA");
      cs->Add("ColorF=00AAFF");
      cs->Add("ColorG=5FAAAA");
      cs->Add("ColorH=AAAAAA");
      cs->Add("ColorI=5A5A5A");
      cs->Add("ColorJ=FF0000");
      cs->Add("ColorK=00FF00");
      cs->Add("ColorL=FFFF00");
      cs->Add("ColorM=0000FF");
      cs->Add("ColorN=AAAAFF");
      cs->Add("ColorO=00FFFF");
      cs->Add("ColorP=FFFFFF");
      ColorDialog1->CustomColors = cs;
      ColorDialog1->Options << cdFullOpen;
      if(ColorDialog1->Execute())
         ColorLine = ColorDialog1->Color;
      delete cs;
      shpLineInvalidate();
   }/* if */

   if(Shift.Contains(ssLeft)){
      if(!PropDlg){
         Application->CreateForm(__classid(TPropDlg), &PropDlg);
      }/* if */
      else{
        if(!PropDlg->Visible){
           PropDlg->Visible = true;
        }/* if */
      }/* if */
      PropDlg->PageControl1->ActivePage = PropDlg->TabLineProp;
      shpLineInvalidate();
   }/* if */
   ShapesInvalidate(ColorLine, ColorFill);
}
//---------------------------------------------------------------------------



void __fastcall TSpeedBar::shpRectMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(Shift.Contains(ssRight)){
      TStrings *cs = new TStringList();
      cs->Add("ColorA=000000");
      cs->Add("ColorB=AA0000");
      cs->Add("ColorC=00AA00");
      cs->Add("ColorD=AAAA00");
      cs->Add("ColorE=0000AA");
      cs->Add("ColorF=00AAFF");
      cs->Add("ColorG=5FAAAA");
      cs->Add("ColorH=AAAAAA");
      cs->Add("ColorI=5A5A5A");
      cs->Add("ColorJ=FF0000");
      cs->Add("ColorK=00FF00");
      cs->Add("ColorL=FFFF00");
      cs->Add("ColorM=0000FF");
      cs->Add("ColorN=AAAAFF");
      cs->Add("ColorO=00FFFF");
      cs->Add("ColorP=FFFFFF");
      ColorDialog1->CustomColors = cs;
      ColorDialog1->Options << cdFullOpen;
      if(ColorDialog1->Execute())
         ColorFill = ColorDialog1->Color;
      delete cs;
      shpRectInvalidate();
   }/* if */

   if(Shift.Contains(ssLeft)){
      if(!PropDlg->Visible){
         PropDlg->Visible = true;
      }/* if */
      PropDlg->PageControl1->ActivePage = PropDlg->TabFillProp;
      shpRectInvalidate();
   }/* if */
   ShapesInvalidate(ColorLine, ColorFill);
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::shpTextMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(Shift.Contains(ssRight)){
      TStrings *cs = new TStringList();
      cs->Add("ColorA=000000");
      cs->Add("ColorB=AA0000");
      cs->Add("ColorC=00AA00");
      cs->Add("ColorD=AAAA00");
      cs->Add("ColorE=0000AA");
      cs->Add("ColorF=00AAFF");
      cs->Add("ColorG=5FAAAA");
      cs->Add("ColorH=AAAAAA");
      cs->Add("ColorI=5A5A5A");
      cs->Add("ColorJ=FF0000");
      cs->Add("ColorK=00FF00");
      cs->Add("ColorL=FFFF00");
      cs->Add("ColorM=0000FF");
      cs->Add("ColorN=AAAAFF");
      cs->Add("ColorO=00FFFF");
      cs->Add("ColorP=FFFFFF");
      ColorDialog1->CustomColors = cs;
      ColorDialog1->Options << cdFullOpen;
      if(ColorDialog1->Execute())
         ColorText = ColorDialog1->Color;
      delete cs;
      shpTextInvalidate();
   }/* if */

   if(Shift.Contains(ssLeft)){
      if(!PropDlg->Visible){
         PropDlg->Visible = true;
      }/* if */
      PropDlg->PageControl1->ActivePage = PropDlg->TabTextProp;
      shpTextInvalidate();
   }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbCopyClick(TObject *Sender)
{
   pc = sbCopy->Down ? cCopy : cZero;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbZoomClick(TObject *Sender)
{
   pc = sbZoom->Down ? cZoom : cZero;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbCorrectMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(Button == mbRight){
      if(!CorrectForm){
         Application->CreateForm(__classid(TCorrectForm), &CorrectForm);
      }/* if */
      else{
        if(!CorrectForm->Visible){
           CorrectForm->Visible = true;
        }/* if */
      }/* if */
   }/* if */
   if(Button == mbLeft){
      if(CorrectForm){
         sbCorrect->GroupIndex = CorrectForm->CheckBox1->Checked ? 10 : 0;
      }/* if */
   }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbFontSelectionMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(Button == mbRight){
   FontDialog1->Execute();
 }/* if */
 else{
      if(!SelectBGI){
         Application->CreateForm(__classid(TSelectBGI), &SelectBGI);
      }/* if */
      else{
        if(!SelectBGI->Visible){
           SelectBGI->Visible = true;
        }/* if */
      }/* if */
 }/* elif */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::miMovedClick(TObject *Sender)
{
   miMoved->Checked = true;
   miRotated->Checked = false;
   Painter->PntCenterRotateFlag = false;
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::miRotatedClick(TObject *Sender)
{
   miRotated->Checked = true;
   miMoved->Checked = false;
   Painter->PntCenterRotateFlag = true;
}
//---------------------------------------------------------------------------
static bool __fastcall CorrectGP(TBaseGP *gp){
  if(gp){
    switch(gp->GetType()){
       case tLine            :
       case tStyleLine       :
       case tPolyLine        :
       case tBezier          :
       case tPipe            :
       case tPolygon         :
       case tFrameRect       :
       case tRoundRect       :
       case tPolyRect        :
       case tFillRect        :
       case tFillPolyRect    :
       case tEllipse         :
       case tFillEllipse     :
       case tArc             :
       case tFillArc         :
          if(CorrectForm->cbxLineColor->Checked) gp->SetColor(ColorLine);
          if(CorrectForm->cbxLineThick->Checked) gp->SetWidthLine(WidthLine);
          if(CorrectForm->cbxFillColor->Checked) gp->SetFillColor(ColorFill);
          if(CorrectForm->cbxFillStyle->Checked) gp->SetFillStyle(StyleFill);
          break;
       case tGradientFillRect:{
             TGradientFillRectangle *gfr = (TGradientFillRectangle*)gp;
             if(CorrectForm->cbxFillStyle->Checked) gfr->SetGradientFillStyle(GradientStyleFill);
             if(CorrectForm->cbxFillColor->Checked) gfr->SetColor2(ColorLine);
             if(CorrectForm->cbxLineColor->Checked) gp->SetColor(ColorFill);
          }
          break;

       case tBGIText         :{
             TBGIText *t = (TBGIText*)gp;
             if(CorrectForm->cbxTextColor->Checked) t->SetColor(ColorText);
             if(CorrectForm->cbxTextSize->Checked) t->SetSize(SizeFntBGI);
             if(CorrectForm->cbxTextStyle->Checked) t->SetFont(iFntBGI);

             if(CorrectForm->cbxText->Checked){
                 if(!TextBGI){
                    Application->CreateForm(__classid(TTextBGI), &TextBGI);
                 }/* if */
                 if(TextBGI){
                   char s[256];
                   strcpy(s, t->GetText());
                   OemToChar(s, s);
                   TextBGI->TextBGIEdit->Text = s;
                   if(!TextBGI->Visible){
                      TextBGI->Visible = true;
                   }/* if */
                   Painter->EditGP = t;
                 }/* if */
             }// if
          }
          break;
       case tTrueTypeText    :
          break;

       case tMacro           :{
             TMacro* m = (TMacro*)gp;
          }
          break;
    }// switch
  }// if
}// CorrectGP
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::sbCorrectClick(TObject *Sender){
   if(sbCorrect->Down){
      if(CorrectForm->CheckBox1->Checked){
         pc = cMoveCrd;
      }// if
   }/* if */
   else{
      pc = cZero;
   }/* elif */

   if((pc != cMoveCrd) && EditPic->ExistGP()){
      EditPic->StoreCGP();
      EditPic->FirstGP();
      do{
         TBaseGP *gp = EditPic->GetGP();
         if(gp && gp->IsSelect)
            CorrectGP(gp);
      }while(EditPic->NextGP());
      EditPic->RestoreCGP();
      Painter->PictureTBG->setChangeStatus(true);
      Painter->PictureTBG->IsUpdate(true);
      Painter->Repaint();
   }// if
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::sbAboutClick(TObject *Sender)
{
   if(!About){
      Application->CreateForm(__classid(TAbout), &About);
      About->Visible = true;
   }/* if */
   else{
     if(!About->Visible){
        About->Visible = true;
     }/* if */
   }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::miBringTo(TObject *Sender)
{
  if(Painter && Painter->PictureTBG){
     switch(((TMenuItem*)Sender)->Tag){
        case 1: // 
           Painter->PictureTBG->BringSelectedGP(TPictureTBG::ToFront);
           break;
        case 2:
           Painter->PictureTBG->BringSelectedGP(TPictureTBG::ToBack);
           break;
        case 3:
           Painter->PictureTBG->BringSelectedGP(TPictureTBG::ToStepUp);
           break;
        case 4:
           Painter->PictureTBG->BringSelectedGP(TPictureTBG::ToStepDown);
           break;
     }/* switch */
     Painter->Invalidate();
  }/* if */
}
//---------------------------------------------------------------------------


void __fastcall TSpeedBar::sbAddLEMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(Shift.Contains(ssRight)){
      if(!PropDlg){
         Application->CreateForm(__classid(TPropDlg), &PropDlg);
      }/* if */
      else{
        if(!PropDlg->Visible){
           PropDlg->Visible = true;
        }/* if */
      }/* if */
      PropDlg->PageControl1->ActivePage = PropDlg->TabLogicProp;
   }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::OpenDialog1Show(TObject *Sender)
{
  if(!LoadPreview_frm){
     Application->CreateForm(__classid(TLoadPreview_frm), &LoadPreview_frm);
     LoadPreview_frm->Visible = true;
  }/* if */
  else{
    if(!LoadPreview_frm->Visible){
       LoadPreview_frm->Visible = true;
    }/* if */
  }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::OpenDialog1Close(TObject *Sender)
{
    if(LoadPreview_frm && LoadPreview_frm->Visible){
       LoadPreview_frm->Visible = false;
    }/* if */
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::OpenDialog1SelectionChange(TObject *Sender)
{
     
   if(LoadPreview_frm && LoadPreview_frm->Visible){
      TCursor Save_Cursor = Screen->Cursor;
      Screen->Cursor = crHourGlass;  
      try{
         LoadPreview_frm->pic.DeleteAllGP();
         LoadPreview_frm->pic.Load(OpenDialog1->FileName.c_str());
         LoadPreview_frm->Caption = OpenDialog1->FileName;
         LoadPreview_frm->Repaint();
      }// try
      __finally{
        Screen->Cursor = Save_Cursor; // always restore the cursor
      }// finally

   }/* if */

}
//---------------------------------------------------------------------------


void __fastcall TSpeedBar::OpenDialog1CanClose(TObject *Sender,
      bool &CanClose)
{
    if(LoadPreview_frm && LoadPreview_frm->Visible){
       LoadPreview_frm->Visible = false;
    }/* if */
}
//---------------------------------------------------------------------------


void __fastcall TSpeedBar::miLine(TObject *Sender)
{
   ((TMenuItem*)Sender)->Checked = true;

   if(sbLine->Down){
      Commands set = miLineSimple->Checked  ? cLine      :
                     miLineStyled->Checked  ? cStyleLine :
                     miLinePoly->Checked    ? cPolyline  :
                     miLineBezier->Checked  ? cBezier    :
                     miLinePolygon->Checked ? cPolygon   :
                     miLinePipe->Checked    ? cPipe      : cZero;
      pc = set;
   }/* if */

}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::miRectClick(TObject *Sender)
{
   ((TMenuItem*)Sender)->Checked = true;
   if(sbRect->Down){
      Commands set = miRect2p->Checked ? cFrameRect :
                     miRect4p->Checked ? cPolyRect  : cZero;
      pc = set;
   }/* if */
}
//---------------------------------------------------------------------------


void __fastcall TSpeedBar::miBarClick(TObject *Sender)
{
   ((TMenuItem*)Sender)->Checked = true;
   Commands set = pc;
   set = miBarGradient->Checked ? cGradientFillFrameRect :
         miBar->Checked         ? cFillFrameRect :
         miBarPoly->Checked     ? cPolyFillFrameRect : cZero;
   if(sbBar->Down){
      pc = set;
      shpRectInvalidate();
   }/* if */
   miBarGradient->Enabled = true;
   miBlur->Enabled = true;
   if(set == cPolyFillFrameRect){
      miBarGradient->Checked = false;
      miBarGradient->Enabled = false;
      miBlur->Enabled = false;
   }// if
}
//---------------------------------------------------------------------------


void __fastcall TSpeedBar::miSelectTypeFontsClick(TObject *Sender)
{
   ((TMenuItem*)Sender)->Checked = true;

   if(sbText->Down){
      Commands set = miBGI->Checked       ? cTextBGI      :
                     miTrueType->Checked  ? cTextTrueType : cZero;
      pc = set;
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TSpeedBar::miEllipseClick(TObject *Sender)
{
   ((TMenuItem*)Sender)->Checked = true;
   if(sbEllipse->Down){
      Commands set = miEllipse->Checked ? cEllipse :
                     miArc->Checked     ? cArc     :
                     miFillArc->Checked ? cFillArc : cZero;
      pc = set;
   }/* if */
}
//---------------------------------------------------------------------------


void __fastcall TSpeedBar::Timer1Timer(TObject *Sender)
{
   if(vos.dwMajorVersion >= 5){
     static unsigned int al = 10;
     if(al < 0xE0){
        al += 20;
        //SetLayeredWindowAttributes(SpeedBar->Handle, RGB(0,0,1), al, LWA_ALPHA | LWA_COLORKEY);
        AlphaBlendValue = al;
     }// if
     else{
        Timer1->Enabled = false;
     }// elif
   }// if
   else{
      Timer1->Enabled = false;
   }// elif

   if(Timer1->Enabled == false){
      if(ARGC > 1){
         do{
            Application->CreateForm(__classid(TPainter), &Painter);
            if(Painter){
               EditPic = Painter->PictureTBG;
               EditPic->WorkPlace()->setShowHideState(N1->Checked);
               EditPic->Load(ARGV[ARGC - 1]);

               Painter->Caption = ARGV[ARGC - 1];
               Painter->ClientWidth = EditPic->GetSizeX();
               Painter->ClientHeight = EditPic->GetSizeY();
               Painter->WindowState = wsNormal;
               Painter->BorderStyle = bsSizeable;
               Painter->ssWindow->Checked = true;
               Painter->Visible = true;
               Painter->Repaint();
            }// if
         }while(--ARGC > 1);
         FormActivate(Owner);
      }// if
   }// if
}
//---------------------------------------------------------------------------

void __fastcall TSpeedBar::miBarGradientClick(TObject *Sender)
{
   miBarGradient->Checked = !miBarGradient->Checked;
   if(miBarGradient->Checked){
      miBlurToColor->Checked = false;
      miBlurToGrayscale->Checked = false;
   }// if
   shpRectInvalidate();
}
//---------------------------------------------------------------------------


void __fastcall TSpeedBar::miBlurToClick(TObject *Sender)
{
   ((TMenuItem*)Sender)->Checked = true;
   miBarGradient->Checked = false;
   shpRectInvalidate();
}
//---------------------------------------------------------------------------


