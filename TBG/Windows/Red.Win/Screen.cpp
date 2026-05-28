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
TPainter *Painter;

//---------------------------------------------------------------------------
__fastcall TPainter::TPainter(TComponent* Owner)
        : TForm(Owner)
{
   BorderStyle = bsNone;

   PictureTBG = new TPictureTBG;
   if(PictureTBG){
      PictureTBG->setSizeScreen(ClientWidth, ClientHeight);
      PictureTBG->setCanvas(Canvas);
      PictureTBG->WorkPlace()->setShowHideState(SpeedBar->N1->Checked);
      PictureTBG->WorkPlace()->setEditProcessing(!SpeedBar->WuSmoothLine->Checked);
      EditGP = NULL;
      EditGPCount = 0;
      IndexLastMacro = -1;
      
      PntCenterRotateFlag = false;
      PntCenterRotate.x = PictureTBG->getMaxX() / 2;
      PntCenterRotate.y = PictureTBG->getMaxY() / 2;
      AngleRotate = 0.0;

      RcSel = false;
      ShowSelected = true;
      SpeedBar->FormActivate(Owner);

      SaveGP = NULL;
      SaveCursor = crNone;
      
      D(printf("\nnew Painter created"));
   }// if
}
//---------------------------------------------------------------------------

void __fastcall TPainter::FormMouseWheel(TObject *Sender,
      TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{
   if(!Active){
      SetFocus();
      return;
   }/* if */

   if(Creation()){
      D(printf("\ncreation"));
      if(Shift.Contains(ssRight)){
         OffsetY = WheelDelta > 0 ? OffsetY + 1 : OffsetY - 1;
      }/* if */
      else{
         if(Shift.Contains(ssLeft)){
            OffsetX = WheelDelta > 0 ? OffsetX + 1 : OffsetX - 1;
         }/* if */
      }/* elif */
      if(EditGP){
         D(printf("\n send GP: mouseMove"));
         Shift << ssLeft;
         EditGP->MouseMove(PictureTBG->WorkPlace(), Sender, Shift, OffsetX + MouseX, OffsetY + MouseY);
         PictureTBG->setChangeStatus(true);
         SpeedBar->FormActivate(Owner);
         PictureTBG->IsUpdate(true);
         Painter->Invalidate();
      }// if
   }/* if */

   if(Editing()){
      D(printf("\nediting"));
      // выбранные примитивы перемещать при нажатой левой кнопке мыши
      if(Shift.Contains(ssRight)){
         OffsetY = WheelDelta > 0 ? OffsetY + 1 : OffsetY - 1;
      }/* if */
      else{
         if(Shift.Contains(ssLeft)){
            OffsetX = WheelDelta > 0 ? OffsetX + 1 : OffsetX - 1;
         }/* if */
      }/* elif */
      switch(pc){
         case cMoveCrd:
            if(Shift.Contains(ssLeft)){
               if(EditPoint){
                  EditPoint->moveto((OffsetX + MouseX) / PictureTBG->getMaxX(), (OffsetY + MouseY) / PictureTBG->getMaxY());
                  PictureTBG->setChangeStatus(true);
                  SpeedBar->FormActivate(Owner);
                  PictureTBG->IsUpdate(true);
                  Repaint();
               }/* if */
            }/* if */
            break;
         case cMove:
            if(Shift.Contains(ssLeft)){
               TPoint to;
               to.x = 0;
               to.y = 0;
               if(Shift.Contains(ssRight)){
                  to.y = (WheelDelta > 0) ? 1 : -1;
               }/* if */
               else{
                  if(Shift.Contains(ssLeft)){
                     to.x = (WheelDelta > 0) ? 1 : -1;
                  }/* if */
               }/* elif */
               PictureTBG->MoveToSelectedGP(to, false);
               PictureTBG->setChangeStatus(true);
               SpeedBar->FormActivate(Owner);
               PictureTBG->IsUpdate(true);
               Painter->Invalidate();
            }/* if */
            break;
         case cRotate:
            if(Shift.Contains(ssLeft)){
               AngleRotate = (WheelDelta > 0 ? -0.1 : 0.1) * (Shift.Contains(ssShift) ? 10. : 1.);
               PictureTBG->RotateToSelectedGP();
               PictureTBG->setChangeStatus(true);
               SpeedBar->FormActivate(Owner);
               PictureTBG->IsUpdate(true);
               Painter->Invalidate();
            }/* if */
            break;
         case cZoom:
            if(Shift.Contains(ssShift)){
               PictureTBG->ZoomSelectedGP(WheelDelta > 0 ? 1.009 : .99 );
            }/* if */
            else{
               PictureTBG->ZoomSelectedGP(WheelDelta > 0 ? 1.0009 : .999 );
            }/* elif */
            PictureTBG->setChangeStatus(true);
            SpeedBar->FormActivate(Owner);
            PictureTBG->IsUpdate(true);
            Painter->Invalidate();
            break;
      }/* switch */
      return;
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TPainter::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   if(!Active)
      return;

   MouseX = X;
   MouseY = Y;
   TBaseGP *OneGP;

   if(Selectoring()){
      // режимы отображения окна редактирования
      if(Button == mbRight){
         // подсветка текущего ГП
         if(!Shift.Contains(ssShift)){
            OneGP = PictureTBG->GetGP();
            if(OneGP){
               PictureTBG->setCanvas(Canvas);
               OneGP->Show(PictureTBG->WorkPlace());
               return;
            }/* if */
         }/* if */
      }/* if */

      if(Button == mbLeft){
         if(RcSel){
            //RcSel = false;
            SelRect.right = X;
            SelRect.bottom  = Y;
            if(PictureTBG->Select(SelRect, Shift)){
               PictureTBG->IsUpdate(true);
               Repaint();
            }/* if */
         }/* if */
      }/* if */

   }/* if */

   if(Editing()){
      D(printf("\nediting"));

      switch(pc){
         case cMoveCrd:
            if(Button == mbLeft){
               Screen->Cursor = crSizeAll;
               SaveGP = NULL;
               EditPoint = NULL;
            }/* if */
         case cMove:
         case cRotate:
         case cZoom:
            if(Button == mbLeft){
               ShowSelected = true;
               PictureTBG->WorkPlace()->setEditProcessing(!SpeedBar->WuSmoothLine->Checked);
               SpeedBar->FormActivate(Owner);
            }/* if */
            break;
      }// switch
      PictureTBG->IsUpdate(true);
      Repaint();
      return;
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TPainter::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(!Active)
      return;

   TBaseGP *OneGP;

   MouseX = X;
   MouseY = Y;

   ppmScreen->AutoPopup = false;

   if(Selectoring()){
      D(printf("\nselectored"));

      // возможно это выбор рамкой
      if(Button == mbLeft){
         SelRect.left    = X;
         SelRect.top     = Y;
         if(RcSel){
            RcSel = false;
         }/* if */
         POINT p;
         p.x = X;
         p.y = Y;
         // выбрать попавшийся ГП
         OneGP = PictureTBG->Select(p, Shift);
         if(OneGP){
            AnsiString s;
            s = "Выбран ";
            if(OneGP->GetHideState()){
               s += "cкрытый ";
            }/* if */
            s += OneGP->GetName();
            s += OneGP->GetStringAllLogic();
            SpeedBar->StatusBar1->Panels->Items[0]->Text = s;
            PictureTBG->IsUpdate(true);
         }/* if */
      }/* if */

      // режимы отображения окна редактирования
      if(Button == mbRight){
         // подсветка текущего ГП
         if(!Shift.Contains(ssShift)){
            OneGP = PictureTBG->GetGP();
            if(OneGP){
               OneGP->ShowSelect(PictureTBG->WorkPlace(), SSIndicate);
               PictureTBG->IsUpdate(true);
            }/* if */
         }/* if */
         else{
            ppmScreen->AutoPopup = true;
         }/* elif */
      }/* if */
      PictureTBG->IsUpdate(true);
      Repaint();
      return;
   }/* if */

   if(Editing()){
      D(printf("\nediting"));

      switch(pc){
         case cMoveCrd:
            if(PictureTBG){
              TPointf h(X / PictureTBG->getMaxX(), Y / PictureTBG->getMaxY());
              if(Shift.Contains(ssLeft)){
                 SaveGP = PictureTBG->HitToPointGP(h);
                 if(SaveGP){
                    EditPoint = SaveGP->HitToPoint(h);
                    if(EditPoint){
                       Screen->Cursor = crNone;
                       ShowSelected = false;
                       PictureTBG->WorkPlace()->setEditProcessing(true);
                       PictureTBG->IsUpdate(true);
                       Repaint();
                    }// if
                 }// if
              }/* if */
            }// if
            break;
         // копирование элементов, их выбор
         case cCopy:{
              CopyGEMode m = cmWithLogic;
              if(SpeedBar->miCopyWLogic->Checked) m = cmWithLogic;
              if(SpeedBar->miCopyWOLogic->Checked) m = cmWithOutLogic;
              if(SpeedBar->miMirrorX->Checked) m = cmMirrorX;
              if(SpeedBar->miMirrorY->Checked) m = cmMirrorY;
              if(SpeedBar->miMirrorXY->Checked) m = cmMirror;
              if(PictureTBG->CopySelectedGP(m)){
                 pc = cMove;
              }/* if */
            }
            // break - отсутствует преднамеренно !!!!
         // начало перемещения
         case cZoom:
         case cMove:
            MovePntSel.x = X;
            MovePntSel.y = Y;
            ShowSelected = false;
            PictureTBG->WorkPlace()->setEditProcessing(true);
            break;
         // начало поворота
         case cRotate:
            ShowSelected = false;
            PictureTBG->WorkPlace()->setEditProcessing(true);
            PntCenterRotateFlag = true;
            break;
      }// switch
      PictureTBG->IsUpdate(true);
      Repaint();
      return;
   }/* if */

   if(Deleting()){
      D(printf("\ndeleting"));
      return;
   }/* if */

   if(Creation()){
      OffsetX = OffsetY = 0;
      D(printf("\ncreation"));
      switch(pc){
         // создание нового ГП

         // Линии
         case cLine:
            if(EditGP == NULL){
               EditGP = new TLine(0, 0, 0, 0, ColorLine, WidthLine);
               D(printf("\n line"));
            }// if
            break;

         case cPolyline:
            if(EditGP == NULL){
               EditGP = new TPolyLine(0, 0, 0, 0, ColorLine, WidthLine);
               D(printf("\n polyline"));
            }// if
            break;
         case cPipe:
            if(EditGP == NULL){
               EditGP = new TPipeLine(0, 0, 0, 0, ColorLine, ColorFill, 1, 1, 8, WidthLine);
               D(printf("\n pipeline"));
            }// if
            break;
         case cPolygon:
            if(EditGP == NULL){
               EditGP = new TPolygon(0, 0, 0, 0, ColorLine, ColorFill, StyleFill, WidthLine);
               D(printf("\n polygon"));
            }// if
            break;
         // Прямоугольники
         case cFrameRect:
            if(EditGP == NULL){
               EditGP = new TRectangle(0, 0, 0, 0, ColorLine, WidthLine);
               D(printf("\n rect"));
            }// if
            break;
         case cPolyRect:
            if(EditGP == NULL){
               EditGP = new TPolyRect(0, 0, 0, 0, ColorLine, WidthLine);
               D(printf("\n polyrect"));
            }// if
            break;
         case cFillFrameRect:
            if(EditGP == NULL){
               TFillRectangle *fr;
               fr = new TFillRectangle(0, 0, 0, 0, ColorLine, ColorFill, StyleFill, WidthLine);
               if(SpeedBar->miBlurToColor->Checked){
                  fr->SetBlur(1);
               }// if
               if(SpeedBar->miBlurToGrayscale->Checked){
                  fr->SetBlur(1);
                  fr->SetBlurToGrayscale(true);
               }// if
               EditGP = fr;
               D(printf("\n fillrect"));
            }// if
            break;
         case cGradientFillFrameRect:
            if(EditGP == NULL){
               EditGP = new TGradientFillRectangle(0, 0, 0, 0, ColorLine, ColorFill, GradientStyleFill);
               D(printf("\n gradientfillrect"));
            }// if
            break;
         case cPolyFillFrameRect:
            if(EditGP == NULL){
               EditGP = new TFillPolyRect(0, 0, 0, 0, ColorLine, ColorFill, StyleFill, WidthLine);
               D(printf("\n fillpolyrect"));
            }// if
            break;
         // Эллипсы
         case cArc:
            if(EditGP == NULL){
               TPointf p[4];
               EditGP = new TArc(p, ColorLine, WidthLine);
               D(printf("\n arc"));
            }// if
            break;
         case cFillArc:
            if(EditGP == NULL){
               TPointf p[4];
               EditGP = new TFillArc(p, ColorLine, ColorFill, StyleFill, WidthLine);
               D(printf("\n fillarc"));
            }// if
            break;
         // Текст
         case cTextBGI:
            if(EditGP == NULL){
               TBGIText *t;
               t = new TBGIText(0, 0, ColorText);
               if(t){
                  CharToOem(sTextBGI, sTextBGI);
                  t->SetText(sTextBGI);
                  OemToChar(sTextBGI, sTextBGI);
                  t->SetFont(iFntBGI);
                  t->SetDirection(HORIZ_DIR);
                  t->SetSize((short int)SizeFntBGI);
                  t->SetJustify(LEFT_TEXT, BOTTOM_TEXT);
                  t->SetExt(32760, 10, 10);
                  t->SetAngle(dAngleText);
                  t->SetSS(PictureTBG->getMaxX(), PictureTBG->getMaxY());
                  EditGP = t;
               D(printf("\n BGItext"));
               }// if
            }// if
            break;

         // Макропримитивы
         case cMacro:
            if(EditGP == NULL){
               unsigned Index, State;
               TTreeView* tv = MacroSelect->getSelectedMacro();
               TTreeNode* ns;
               if(tv && tv->Selected){
                  if(IndexLastMacro == -1){
                     IndexLastMacro = 0;
                  }// if

                  ns = tv->Selections[IndexLastMacro++];

                  if((unsigned)IndexLastMacro >= tv->SelectionCount){
                     IndexLastMacro = 0;
                  }// if

                  Index = mtv->SearchMacroAsIndex(ns);
                  State = 4;
                  EditGP = new TMacro(Index, State, TPointf(MouseX / PictureTBG->getMaxX(), MouseY / PictureTBG->getMaxY()));
                  D(printf("\n macro: %u %u", Index, State));
               }// if
            }// if
            break;

/*
         case cTextTrueType:
            if(EditGP == NULL){
               TTrueTypeText *t;
               t = new TTrueTypeText(0, 0, ColorText);
               if(t){
                  t->SetText(sTextBGI);
                  t->SetAngle(dAngleText);
                  EditGP = t;
               D(printf("\n TrueTypetext"));
               }// if
            }// if
            break;
*/
      }/* switch */

      if(EditGP){
         // новый примитив создан, редактируется положение
         switch(EditGP->MouseDown(PictureTBG->WorkPlace(), Sender, Button, Shift, OffsetX + MouseX,
                                                         OffsetY + MouseY)){
            case 1:
               // Adding New LE
               if(PropDlg && PropDlg->cbSelLE->Checked){
                  if(EditGP){
                     EditGP->InsertLE((unsigned short)atoi(PropDlg->EditNo->Text.c_str()), (unsigned short)atoi(PropDlg->EditNe->Text.c_str()));
                     SpeedBar->EditOBJ->Text = EditGP->GetNo();
                     SpeedBar->EditNUM->Text = EditGP->GetNe();
                     SpeedBar->sbCurrentLE->ShowHint = true;
                     SpeedBar->sbCurrentLE->Hint = EditGP->GetID();
                     SpeedBar->sbCurrentLE->Caption = EditGP->IndexCurrentLE();
                     SpeedBar->sbCurrentGE->Caption = EditPic->IndexCurrentGP();
                     SpeedBar->sbAllLE->Caption = EditGP->GetCount();
                  }/* if */
               }/* if */
               PictureTBG->InsertGP(EditGP);
               EditGP = NULL;
               D(printf("\n  GP added to list"));
               PictureTBG->WorkPlace()->setEditProcessing(!SpeedBar->WuSmoothLine->Checked);
               PictureTBG->setChangeStatus(true);
               PictureTBG->IsUpdate(true);
               SpeedBar->FormActivate(Owner);
               break;
            case -1:
               delete EditGP;
               EditGP = NULL;
               D(printf("\n  GP cancelled"));
               PictureTBG->WorkPlace()->setEditProcessing(!SpeedBar->WuSmoothLine->Checked);
               PictureTBG->IsUpdate(true);
               break;
            case 0: // новый примтив продолжает создаваться
               PictureTBG->WorkPlace()->setEditProcessing(true);
               break;
         }// switch
         Repaint();
      }// if
      return;
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TPainter::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{

   if(!Active){
      SetFocus();
      return;
   }/* if */

   MouseX = X;
   MouseY = Y;

   if(Selectoring()){
      D(printf("\nselectored"));
      POINT p;
      p.x = X;
      p.y = Y;

      // выбор ГП рамкой нажатой левой кнопкой мыши
      if(Shift.Contains(ssLeft)){
         // нет рамки
         // движущийся угол рамки
         SelRect.right   = X;
         SelRect.bottom  = Y;
         // нормализация прямоугольника выбора
         int x1 = SelRect.left;
         int x2 = SelRect.right;
         int y1 = SelRect.top;
         int y2 = SelRect.bottom;
         TRect sr(min(x1, x2), min(y1, y2), max(x1, x2), max(y1, y2));
         if(!RcSel){
            if((sr.Width() >= 1) && (sr.Height() >= 1)){
               RcSel = true;
            }/* if */
         }/* if */
         // есть рамка
         if(RcSel){
            // выбирает попавшие элементы: взводит флаг выбора
            AnsiString s = PictureTBG->Select(sr, Shift);
            s += " ГП выбрано";
            SpeedBar->StatusBar1->Panels->Items[0]->Text = s;
            PictureTBG->IsUpdate(true);
            Repaint();
         }/* if */
      }/* if */
      else{
         // отработать попадание в ГП
         TBaseGP *tgp = PictureTBG->Hit(p);
         AnsiString s = "";
         Hint = s;
         ShowHint = false;
         if(tgp){
            //s = "Попали в ";
            if(tgp->GetHideState()){
               s += "cкрытый ГП: ";
            }/* if */
            s += tgp->GetName();
            s += tgp->GetStringAllLogic();
            ShowHint = false;
            ShowHint = true;
         }/* if */
         Hint = s;
         SpeedBar->StatusBar1->Panels->Items[0]->Text = s;
      }/* elif */
      return;
   }/* if */

   if(Creation()){
      D(printf("\ncreation"));
      if(EditGP){
         D(printf("\n send GP: mouseMove"));
         EditGP->MouseMove(PictureTBG->WorkPlace(), Sender, Shift, OffsetX + MouseX, OffsetY + MouseY);
         Repaint();
      }// if
   }/* if */

   if(Editing()){
      D(printf("\nediting"));
      switch(pc){
         case cMoveCrd:
            if(PictureTBG){
              TPointf h(X / PictureTBG->getMaxX(), Y / PictureTBG->getMaxY());
              if(!Shift.Contains(ssLeft)){
                 SaveGP = PictureTBG->HitToPointGP(h);
                 if(Screen->Cursor != crSizeAll)
                    SaveCursor = Screen->Cursor;
                 if(SaveGP){
                    Screen->Cursor = crSizeAll;
                 }// if
                 else{
                    Screen->Cursor = SaveCursor;
                 }// if
              }/* if */
              else{
                 if(SaveGP){
                    if(EditPoint){
                      EditPoint->moveto(X / PictureTBG->getMaxX(), Y / PictureTBG->getMaxY());
                      PictureTBG->setChangeStatus(true);
                      PictureTBG->IsUpdate(true);
                      Repaint();
                    }// if
                 }// if
              }// elif
            }// if
            break;
         // выбранные примитивы перемещать при нажатой левой кнопке мыши
         case cZoom:
         case cMove:
            if(Shift.Contains(ssLeft)){
               TPoint to;
               to.x = MovePntSel.x - X;
               to.y = MovePntSel.y - Y;
               MovePntSel.x = X;
               MovePntSel.y = Y;
               PictureTBG->MoveToSelectedGP(to, false);
               Painter->Invalidate();
            }/* if */
            break;
         case cRotate:
            if(Shift.Contains(ssLeft)){
               PictureTBG->RotateToSelectedGP();
               Painter->Invalidate();
            }/* if */
            break;
      }/* switch */
      return;
   }/* if */
}
//---------------------------------------------------------------------------
void __fastcall TPainter::FormPaint(TObject *Sender)
{
   
   // CPU Speed & repaint time
   static bool cpuspeed = false;
   static unsigned cpuspeed10;
   __int64 tc, start, stop;

   asm rdtsc
   start = _EAX << 32 | _EDX;

   if(cpuspeed == false){
      Sleep(1000);  // 1 sec.
      asm rdtsc
      stop = _EAX << 32 | _EDX;
      if(stop >= start){
         cpuspeed10 = (unsigned)((stop - start) / (__int64)100000);
         cpuspeed = true;
      }// if
   }// if
   //-----------------
   
   if(PictureTBG){
      PictureTBG->setSizeScreen(ClientWidth, ClientHeight);
      PictureTBG->setCanvas(Canvas);
      PictureTBG->Show();
      PictureTBG->ShowSelect(ShowSelected ? SSNormal : SSNone);
   }/* if */

   
   if(PictureTBG){
      if(EditGP){
         TWorkPlace ewp(Canvas, true);
         ewp.setSizeScreen(ClientWidth, ClientHeight);
         EditGP->Show(&ewp);
      }// if
   }// if

   if(RcSel && PictureTBG){
      TCanvas *cvs = Canvas;//PictureTBG->WorkPlace()->getCanvas();
      cvs->Pen->Style = psSolid;
      cvs->Pen->Color = clBlack;
      cvs->Pen->Width = 1;
      cvs->Brush->Style = bsSolid;
      cvs->Brush->Color = clWhite;
      int x1 = SelRect.left;
      int x2 = SelRect.right;
      int y1 = SelRect.top;
      int y2 = SelRect.bottom;
      TRect sr(min(x1, x2), min(y1, y2), max(x1, x2), max(y1, y2));
      cvs->DrawFocusRect(sr);
   }/* if */

   if(PntCenterRotateFlag && RcSel && PictureTBG){
      TCanvas *cvs = Canvas;//PictureTBG->WorkPlace()->getCanvas();
      TRect r;

      PntCenterRotate.x = SelRect.Left + SelRect.Width() / 2;
      PntCenterRotate.y = SelRect.Top + SelRect.Height() / 2;

      cvs->Pen->Style = psSolid;
      cvs->Pen->Color = clBlack;
      cvs->Pen->Width = 1;
      cvs->Brush->Style = bsSolid;
      cvs->Brush->Color = clGreen;
      r.Left    = PntCenterRotate.x - 4;
      r.Top     = PntCenterRotate.y - 4;
      r.Right   = PntCenterRotate.x + 4;
      r.Bottom  = PntCenterRotate.y + 4;
      cvs->Ellipse(r);

      cvs->Pen->Width = 1;
      cvs->Brush->Style = bsClear;
      r.Left    = PntCenterRotate.x - 7;
      r.Top     = PntCenterRotate.y - 7;
      r.Right   = PntCenterRotate.x + 7;
      r.Bottom  = PntCenterRotate.y + 7;
      cvs->Ellipse(r);
   }/* if */

   // End of CPU-speed & repaint time
   asm rdtsc
   tc = _EAX << 32 | _EDX;
   if(tc >= start){
      stop = (tc - start) / (__int64)100000;
      unsigned stp = (unsigned)stop;
      printf("\rCPU speed: %3.3d.%1.1d, repaint %6.1f ms", cpuspeed10 / 10, cpuspeed10 % 10,
                                                           stp * 1000.0 / cpuspeed10);
   }// if
   //-----------------

}
//---------------------------------------------------------------------------
void __fastcall TPainter::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if(!Active)
      return;

   //D(printf("Press key : %d : modificators: %d\n", Key, Shift));

   switch(Key){
      case VK_F4:
         break;
      case VK_PRIOR: // PgUp
         if(PictureTBG){
            TBaseGP *gp = PictureTBG->GetGP();
            if(gp && gp->GetType() == tPipe){
               TPipeLine *gpppl = (TPipeLine*)gp;
               if(Shift.Contains(ssShift)){
                  gpppl->IncDO();
               }/* if */
               else{
                  gpppl->IncS();
               }/* elif */
            }/* if */
         }/* if */
         break;
      case VK_NEXT: // PgDn
         if(PictureTBG){
            TBaseGP *gp = PictureTBG->GetGP();
            if(gp && gp->GetType() == tPipe){
               TPipeLine *gpppl = (TPipeLine*)gp;
               if(Shift.Contains(ssShift)){
                  gpppl->DecDO();
               }/* if */
               else{
                  gpppl->DecS();
               }/* elif */
            }/* if */
         }/* if */
         break;
      case VK_SPACE:
         SpeedBar->Visible = !SpeedBar->Visible;
         break;
      case VK_ESCAPE:{
         static TCursor sc;
         if(Cursor != crNone)
            sc = Cursor;
         Cursor = Cursor == crNone ? sc : crNone;
         }
         break;
      case VK_F8:
         if(MacroSelect == NULL)
            Application->CreateForm(__classid(TMacroSelect), &MacroSelect);
         MacroSelect->Visible = true;
         break;
      case VK_F10:
         PictureTBG->IsUpdate(true);
         Repaint();
         return;
      case VK_F2:{
            if(PictureTBG->GetCount() == 0){
               Application->MessageBox("Нет примитивов!", "Сохранение файла", MB_OK | MB_ICONSTOP);
               break;
            }/* if */
            TCursor Save_Cursor = Screen->Cursor;
            Screen->Cursor = crHourGlass;
            try{
               PictureTBG->Save(Caption.c_str());
            }// try
            __finally{
              Screen->Cursor = Save_Cursor; // always restore the cursor
            }// finally
            SpeedBar->FormActivate(Owner);
         }
         return;
      case VK_F3:
         break;
      case VK_DELETE:
         if(PictureTBG){
            PictureTBG->DeleteCurrentGP();
            PictureTBG->setChangeStatus(true);
            SpeedBar->FormActivate(Owner);
            PictureTBG->IsUpdate(true);
            Repaint();
         }/* if */
         return;
      case VK_RETURN:{
         TShiftState Sh;
         Sh << ssLeft;
         FormMouseDown(Sender, mbLeft, Sh, MouseX, MouseY);
         FormMouseUp(Sender, mbLeft, Sh, MouseX, MouseY);
         }
         break;
      case VK_ADD:{
         TShiftState Sh;
         Sh << ssRight;
         FormMouseDown(Sender, mbRight, Sh, MouseX, MouseY);
         FormMouseUp(Sender, mbRight, Sh, MouseX, MouseY);
         }
         break;
      case VK_MULTIPLY:{
         static bool h;
         Shift << ssLeft;
         if(Shift.Contains(ssShift)) Shift << ssRight;
         TPoint cp = Mouse->CursorPos;
         FormMouseWheel(Sender, Shift, -1, cp, h);
         }
         break;
      case VK_DIVIDE:{
         static bool h;
         Shift << ssLeft;
         if(Shift.Contains(ssShift)) Shift << ssRight;
         TPoint cp = Mouse->CursorPos;
         FormMouseWheel(Sender, Shift, 1, cp, h);
         }
         break;
      case VK_HOME:
         if(PictureTBG && PictureTBG->GetGP()){
            static TBaseGP *gp;
            static i, imax;
            TPointf *gpp;
            TPoint p;
            if(gp != PictureTBG->GetGP()){
               gp = PictureTBG->GetGP();
               imax = gp->GetCoordCnt() - 1;
               i = 0;
            }/* if */
            else{
               i = i >= imax ? 0 : i + 1;
            }/* elif */
            gpp = gp->GetCoordPtr(i);
            p.x = gpp->getx() * PictureTBG->getMaxX() + 0.5;
            p.y = gpp->gety() * PictureTBG->getMaxY() + 0.5;
            p = ClientToScreen(p);
            Mouse->CursorPos = p;
         }/* if */
         break;
      case VK_UP:{
         TPoint p = Mouse->CursorPos;
         p.y -= Shift.Contains(ssShift) ? 10 : 1;
         Mouse->CursorPos = p;
         }
         break;
      case VK_DOWN:{
         TPoint p = Mouse->CursorPos;
         p.y += Shift.Contains(ssShift) ? 10 : 1;
         Mouse->CursorPos = p;
         }
         break;
      case VK_LEFT:{
         TPoint p = Mouse->CursorPos;
         p.x -= Shift.Contains(ssShift) ? 10 : 1;
         Mouse->CursorPos = p;
         }
         break;
      case VK_RIGHT:{
         TPoint p = Mouse->CursorPos;
         p.x += Shift.Contains(ssShift) ? 10 : 1;
         Mouse->CursorPos = p;
         }
         break;
   }/* switch */

   if(Shift.Contains(ssCtrl)){
      switch(Key){
         case VK_RETURN:
            if(EditGP == NULL){
               EditGP = new TReperPoint();
               if(EditGP){
                  EditGP->SetStartXY(MouseX / PictureTBG->getMaxX(), MouseY / PictureTBG->getMaxY());
                  if(PropDlg && PropDlg->cbSelLE->Checked){
                     EditGP->InsertLE((unsigned short)atoi(PropDlg->EditNo->Text.c_str()), (unsigned short)atoi(PropDlg->EditNe->Text.c_str()));
                     SpeedBar->EditOBJ->Text = EditGP->GetNo();
                     SpeedBar->EditNUM->Text = EditGP->GetNe();
                     SpeedBar->sbCurrentLE->ShowHint = true;
                     SpeedBar->sbCurrentLE->Hint = EditGP->GetID();
                     SpeedBar->sbCurrentLE->Caption = EditGP->IndexCurrentLE();
                     SpeedBar->sbCurrentGE->Caption = EditPic->IndexCurrentGP();
                     SpeedBar->sbAllLE->Caption = EditGP->GetCount();
                  }/* if */
                  PictureTBG->InsertGP(EditGP);
                  EditGP = NULL;
                  D(printf("\n  GP added to list"));
                  PictureTBG->setChangeStatus(true);
                  SpeedBar->FormActivate(Owner);
                  PictureTBG->IsUpdate(true);
                  Repaint();
               }// if
            }// if
            break;
         // Ctrl-A: Select All GE
         case 'a':
         case 'A':
            if(PictureTBG){
               PictureTBG->SelectAll();
               PictureTBG->IsUpdate(true);
               Repaint();
            }/* if */
            break;
         case VK_INSERT: // Put select GP into Clipboard
            {
            CopyGEMode m = cmWithLogic;
            if(SpeedBar->miCopyWLogic->Checked) m = cmWithLogic;
            if(SpeedBar->miCopyWOLogic->Checked) m = cmWithOutLogic;
            PictureTBG->CopySelectedGPIntoClipboard(m);
            }
            break;
         case 'p':
         case 'P':
         case 'з':
         case 'З':
            miPrintClick(Sender);
            break;
      }/* switch */
   }/* if */

   if(Shift.Contains(ssShift)){
      switch(Key){
         case VK_INSERT: // Get GP from Clipboard
            if(PictureTBG->CopyGPFromClipboard()){
               pc = cMove;
               PictureTBG->setChangeStatus(true);
               SpeedBar->FormActivate(Owner);
               PictureTBG->IsUpdate(true);
               Repaint();
            }/* if */
            break;
      }/* switch */
   }/* if */
}
//---------------------------------------------------------------------------


void __fastcall TPainter::FormClose(TObject *Sender, TCloseAction &Action)
{
   if(PictureTBG->getChangeStatus()){
      AnsiString sm;
      sm = "Файл \"";
      sm += Painter->Caption;
      sm += "\" был изменён.\nСохранить изменения?";
      if(Application->MessageBox(sm.c_str(), "Внимание!", MB_YESNO | MB_ICONWARNING) == IDYES){
         WORD Key = VK_F2;
         TShiftState Shift;
         FormKeyDown(Sender, Key, Shift);
      }// if
   }// if

   if(PictureTBG){
      PictureTBG->DeleteAllGP();
      EditPic = NULL;
   }// if

   if(EditGP){
      delete EditGP;
      EditGP = NULL;
   }// if

   D(printf("\nPainter closed"));

   Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TPainter::FormActivate(TObject *Sender)
{
   if(PictureTBG){
      EditPic = (TPictureTBG*)PictureTBG;
      PictureTBG->IsUpdate(true);
   }// if
   Painter = this;
   Painter->Invalidate();

   SpeedBar->FormActivate(Sender);

   if(pc == cZero)
      pc = cSelect;
}
//---------------------------------------------------------------------------

void __fastcall TPainter::FormDestroy(TObject *Sender)
{
   TCloseAction Act;
   FormClose(Owner, Act);
}
//---------------------------------------------------------------------------

void __fastcall TPainter::ss640_480Click(TObject *Sender)
{
   ClientWidth = 640 - 1;
   ClientHeight = 480 - 1;
   //Position = poScreenCenter;
   WindowState = wsNormal;
   PictureTBG->IsUpdate(true);
   Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TPainter::ss800_600Click(TObject *Sender)
{
   ClientWidth = 800 - 1;
   ClientHeight = 600 - 1;
   //Position = poScreenCenter;
   WindowState = wsNormal;
   PictureTBG->IsUpdate(true);
   Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TPainter::ss1024_768Click(TObject *Sender)
{
   ClientWidth = 1024 - 1;
   ClientHeight = 768 - 1;
   //Position = poScreenCenter;
   WindowState = wsNormal;
   PictureTBG->IsUpdate(true);
   Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TPainter::ssFullScreenClick(TObject *Sender)
{
   ClientWidth = Screen->Width;
   ClientHeight = Screen->Height;
   WindowState = wsMaximized;
   PictureTBG->IsUpdate(true);
   Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TPainter::ssWindowClick(TObject *Sender)
{
   ssWindow->Checked = true;
   ssFrame->Checked = false;
   BorderStyle = bsSizeable;
   //Position = poScreenCenter;
   PictureTBG->IsUpdate(true);
   Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TPainter::ssFrameClick(TObject *Sender)
{
   ssFrame->Checked = true;
   ssWindow->Checked = false;
   BorderStyle = bsNone;
   //Position = poScreenCenter;
   PictureTBG->IsUpdate(true);
   Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TPainter::FormResize(TObject *Sender)
{
 //  PictureTBG->WorkPlace()->setEditProcessing(true);
 //  PictureTBG->IsUpdate(true);
   Paint();
 //  PictureTBG->WorkPlace()->setEditProcessing(!SpeedBar->WuSmoothLine->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TPainter::miPrintClick(TObject *Sender)
{
  if(PictureTBG){
     SpeedBar->PrintDialog1->Options.Clear();
     if (SpeedBar->PrintDialog1->Execute()){
        TPrinter *Prntr = Printer();
        Prntr->Orientation = poLandscape;
        Prntr->BeginDoc();
        PictureTBG->setSizeScreen(Prntr->PageWidth, Prntr->PageHeight);
        PictureTBG->setCanvas(Prntr->Canvas);
        PictureTBG->Show();
        Prntr->EndDoc();
     }/* if */
  }/* if */
}
//---------------------------------------------------------------------------


void __fastcall TPainter::miClosePictureClick(TObject *Sender)
{
   Painter->Close();
}
//---------------------------------------------------------------------------

void __fastcall TPainter::Timer1Timer(TObject *Sender)
{
   if(PictureTBG){ // есть картинка
      PictureTBG->setSizeScreen(ClientWidth, ClientHeight);
      PictureTBG->setCanvas(Canvas);
      TBaseGP *gp = PictureTBG->GetGP();                     // взять текущий ГП
      if(gp && gp->GetType() == tPipe){                      // если он есть и он труба, то
         short obj = gp->GetNo();                            // взять его номер объекта,
         short num = gp->GetNe();                            // взять его номер элемента,
         if(obj >= 0 && num >= 0){                           // если они действующие то
            TBaseGP *gp = PictureTBG->FindFirstNe(obj, num); // начать поиск таких-же объектов+эл-тов
            while(gp){                                       // нашли!
               if(gp->GetType() == tPipe){                   // если это тоже труба
                  gp->Show(PictureTBG->WorkPlace());                          // показать её
                  PictureTBG->IsUpdate(true);
               }/* if */
               gp = PictureTBG->FindNextNe(obj, num);        // найти следующий такой-же объект+эл-т
            }/* while */
         }/* if */
         else{
            gp->Show(PictureTBG->WorkPlace());                                // просто показать, если ГП не пронумерован
         }/* elif */
         if(PictureTBG->IsUpdate()){
            Repaint();
         }// if
      }/* if */
   }/* if */
}
//---------------------------------------------------------------------------


void __fastcall TPainter::ssZoomDisplayClick(TObject *Sender)
{
   ssZoomDisplay->Checked = !ssZoomDisplay->Checked;
   if(ZoomScreen == NULL)
      Application->CreateForm(__classid(TZoomScreen), &ZoomScreen);
   ZoomScreen->Visible = ssZoomDisplay->Checked;
   if(ZoomScreen->Visible){
      ZoomScreen->Width = Screen->DesktopWidth / 5;
      ZoomScreen->Height = ZoomScreen->Width;
      ZoomScreen->Top = 0;
      ZoomScreen->Left = 0;
      ZoomScreen->ZoomOn(5);
   }// if
   else{
      ZoomScreen->ZoomOff();
   }// elif
}
//---------------------------------------------------------------------------


void __fastcall TPainter::Timer2Timer(TObject *Sender)
{
   if(vos.dwMajorVersion >= 5){
     if(AlphaBlendValue < 254){
        AlphaBlendValue += (255 - AlphaBlendValue) > 40 ? 40 : (255 - AlphaBlendValue);
        //SetLayeredWindowAttributes(Handle, RGB(0,0,1), al, LWA_ALPHA | LWA_COLORKEY);
     }// if
     else{
        Timer2->Enabled = false;
        AlphaBlend = false;
     }// elif
   }// if
   else{
      Timer2->Enabled = false;
   }// elif
}
//---------------------------------------------------------------------------


void __fastcall TPainter::FormShow(TObject *Sender)
{
   Timer2->Enabled = false;
   if(vos.dwMajorVersion >= ((DWORD)5)){
     AlphaBlend = true;
     Timer2->Enabled = true;
   }// if
}
//---------------------------------------------------------------------------





