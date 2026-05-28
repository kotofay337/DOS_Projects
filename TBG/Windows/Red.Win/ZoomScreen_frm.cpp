//---------------------------------------------------------------------------

#include <vcl.h>
#include <time.h>

#include "Screen.h"
#include "ToolBar.h"
#include "PropDlg1.h"
#include "ZoomScreen_frm.h"

#include "tbg.hpp"
#include "TBG_Editor.hpp"
#include "tbg_tc.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TZoomScreen *ZoomScreen;

#define XSIZERULER 20
#define YSIZERULER 20

static int sRulers = 0;
/* ----------------------------------------------------------------------- */
void TZoomScreen::ZoomOn(int xzoom){
   XZoom = xzoom;
   Zoom = 1;

   RECT r = GetClientRect();
	HRGN hRgn = CreateEllipticRgnIndirect(&r);
	SetWindowRgn(ZoomScreen->Handle, hRgn, TRUE);
	DeleteObject(hRgn);

   ZoomScreen->Visible = true;
   Timer1->Enabled = true;
}/* ZoomOn */
/* ----------------------------------------------------------------------- */
void TZoomScreen::ZoomOff(void){
   Timer1->Enabled = false;
   ZoomScreen->Visible = false;
   XZoom = 2;
   Zoom = 0;
}/* ZoomOff */
/* ----------------------------------------------------------------------- */
void TZoomScreen::ZoomDisplay (void){
   static int x_mouse_old, y_mouse_old, xp, yp;
   TPoint mp = Mouse->CursorPos;
   int x_mouse = mp.x;
   int y_mouse = mp.y;

   if((y_mouse != y_mouse_old) || (x_mouse != x_mouse_old)){
  //    clock_t end, start = clock();
      y_mouse_old = y_mouse;
      x_mouse_old = x_mouse;
      HDC hDC = GetDC(NULL);
      if(Zoom){
         int XS = ClientWidth / XZoom, YS = ClientHeight / XZoom;
         for(yp = 0;yp < YS;yp++){
            for(xp = 0;xp < XS;xp++){
               Canvas->Brush->Style = bsSolid;// Screen
               Canvas->Brush->Color = GetPixel(hDC, xp + (x_mouse - XS / 2), yp + (y_mouse - YS / 2));
//               Canvas->Brush->Color = cvs_dst->Pixels[xp + (x_mouse - XS / 2)][yp + (y_mouse - YS / 2)];
               Canvas->FillRect(TRect(xp * XZoom, yp * XZoom,
                   xp * XZoom + XZoom, yp * XZoom + XZoom));
            }/* for */
         }/* for */
         Canvas->Pen->Mode = pmNotXor;
         Canvas->Pen->Color = clBlack;
         Canvas->Pen->Width = XZoom;
         Line(Canvas->Handle, ClientWidth / 2, 0, ClientWidth / 2, ClientHeight);
         Line(Canvas->Handle, 0, ClientHeight / 2, ClientWidth, ClientHeight / 2);
         Canvas->Pen->Mode = pmCopy;
      }/* if */
      ReleaseDC(NULL, hDC);
//      end = clock();
   }/* if */
}/* ZoomDisplay */
/* ----------------------------------------------------------------------- */
/*
void HorizRuler(void){
   int maxx = getmaxx();
   int maxy = YSIZERULER;
   int x;
   setcolor(EGA_DARKGRAY);
   setlinestyle(SOLID_LINE, 0, 1);
   setfillstyle(SOLID_FILL, EGA_LIGHTGRAY);
   settextjustify(CENTER_TEXT, TOP_TEXT);
   settextstyle(SMALL_FONT, HORIZ_DIR, 2);
   mouse_off();
   bar(0, 0, maxx, maxy);
   for(x = 0;x < maxx;x++){
      if(x % 20 == 0){
         char s[16];
         line(x, 0, x, maxy - 10);
         sprintf(s, "%d", x);
         outtextxy(x, maxy - 9, s);
      }/* if */
      else{
         if(x % 10 == 0){
            line(x, 0, x, maxy / 3);
         }/* if */
         else{
            if(x % 2 == 0){
               line(x, 0, x, maxy / 4);
            }/* if */
         }/* elif */
      }/* elif */
   }/* for */
   setwritemode(XOR_PUT);
   setcolor(EGA_YELLOW);
   line(x_mouse, 0, x_mouse, YSIZERULER - 10);
   setwritemode(COPY_PUT);
   mouse_on();
}/* HorizRuler */
/****************************************************************************/
void VertRuler(void){
   int maxy = getmaxy();
   int maxx = XSIZERULER;
   int y;
   setcolor(EGA_DARKGRAY);
   setlinestyle(SOLID_LINE, 0, 1);
   setfillstyle(SOLID_FILL, EGA_LIGHTGRAY);
   settextjustify(LEFT_TEXT, CENTER_TEXT);
   settextstyle(SMALL_FONT, HORIZ_DIR, 2);
   mouse_off();
   bar(0, 0, maxx + 3, maxy);
   for(y = 0;y < maxy;y++){
      if(y % 20 == 0){
         char s[16];
         line(0, y, maxx - 10, y);
         sprintf(s, "%d", y);
         outtextxy(maxx - 8, y, s);
      }/* if */
      else{
         if(y % 10 == 0){
            line(0, y, maxx / 3, y);
         }/* if */
         else{
            if(y % 2 == 0){
               line(0, y, maxx / 4, y);
            }/* if */
         }/* elif */
      }/* elif */
   }/* for */
   setwritemode(XOR_PUT);
   setcolor(EGA_YELLOW);
   line(0, y_mouse, XSIZERULER - 10, y_mouse);
   setwritemode(COPY_PUT);
   mouse_on();
}/* VertRuler */
/* ----------------------------------------------------------------------- */
void Rulers(void){
   if(sRulers == 0){
      return;
   }/* if */
   HorizRuler();
   VertRuler();
}/* Rulers */
*/
/* ----------------------------------------------------------------------- */
__fastcall TZoomScreen::TZoomScreen(TComponent* Owner)
   : TForm(Owner)
{
   Zoom = 0;
   XZoom = 4;
   MoveWindow = false;
}
//---------------------------------------------------------------------------
void __fastcall TZoomScreen::Timer1Timer(TObject *Sender)
{
   ZoomDisplay();
}
//---------------------------------------------------------------------------
void __fastcall TZoomScreen::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(Button == mbLeft){
      MoveWindow = true;
      WX = X;
      WY = Y;
      Timer1->Enabled = false;
   }// if
}
//---------------------------------------------------------------------------
void __fastcall TZoomScreen::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if(MoveWindow){
      TPoint mp = Mouse->CursorPos;
      Left = mp.x - WX;
      Top = mp.y - WY;
   }// if
}
//---------------------------------------------------------------------------

void __fastcall TZoomScreen::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(Button == mbLeft){
      MoveWindow = false;
      Timer1->Enabled = true;
   }// if
}
//---------------------------------------------------------------------------

