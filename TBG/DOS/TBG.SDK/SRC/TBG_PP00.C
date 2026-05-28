#ifdef __WIN32__
#include <windows.h>
#else
#include <graphics.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include "tbg_tc.h"
/* ----------------------------------------------------------------------- */
static POINT pt[512];
static short int cpt = 0;
/* ----------------------------------------------------------------------- */
#ifdef __WIN32__
void pipeline(HDC hdc, LPPIPELINE lppl, int color, int style, int thickness){
#else
void pipeline(LPPIPELINE lppl, int color){
#endif
   short int x,y,dx,dy,s1,s2,e,swap,i;
   short int x1,y1,x2,y2, I;
   x1 = lppl->xs;
   y1 = lppl->ys;
   x2 = lppl->xe;
   y2 = lppl->ye;
   if(color < 0){
      x = x1;
      y = y1;
      dx = abs(x2 - x1);
      dy = abs(y2 - y1);
      s1 = (x2 - x1) <  0 ? -1 :
           (x2 - x1) == 0 ?  0 : 1 ;
      s2 = (y2 - y1) <  0 ? -1 :
           (y2 - y1) == 0 ?  0 : 1 ;
      if(dy > dx){
         short int tmp = dx;
         dx = dy;
         dy = tmp;
         swap = 1;
      }/* if */
      else
         swap = 0;
      e = 2 * dy - dx;
      I = lppl->O;
      cpt = 0;
      pt[cpt].x = x;
      pt[cpt].y = y;
      cpt++;
      for(i = 1;i <= dx;i++){
         I++;
         if(I >= lppl->S){
            I = 0;
            pt[cpt].x = x;
            pt[cpt].y = y;
            cpt++;
            if(cpt >= (sizeof(pt) / sizeof(pt[0])))
               cpt--;
         }/* if */
         while(e >= 0){
            if(swap == 1)
               x += s1;
            else
               y += s2;
            e = e - 2 * dx;
         }/* while */
         if(swap == 1)
            y += s2;
         else
            x += s1;
         e = e + 2 * dy;
      }/* for */
      pt[cpt].x = x;
      pt[cpt].y = y;
      for(i = 0;i < cpt;i++){
#ifndef __WIN32__
         setcolor(i % 2 ? lppl->C1 : lppl->C2);
         line(pt[i].x, pt[i].y, pt[i+1].x, pt[i+1].y);
#else
         SelectObject(hdc,
          cEGA[i % 2 ? lppl->C1 : lppl->C2].hPen[(thickness == THICK_WIDTH) ? USERBIT_LINE : style]);
         Line(hdc, pt[i].x, pt[i].y, pt[i+1].x, pt[i+1].y);
#endif
      }/* for */
      if(color == -1){
          lppl->O -= lppl->DO;
          if(lppl->O < 0){
             short int t = lppl->C1;
             lppl->C1 = lppl->C2;
             lppl->C2 = t;
             lppl->O = lppl->S;
          }/* if */
      }/* if */
      else{
          lppl->O += lppl->DO;
          if(lppl->O >= lppl->S){
             int t = lppl->C1;
             lppl->C1 = lppl->C2;
             lppl->C2 = t;
             lppl->O = 0;
          }/* if */
      }/* elif */
      return;
   }/* if */
   else{
#ifndef __WIN32__
      setcolor(color);
      line(lppl->xs, lppl->ys, lppl->xe, lppl->ye);
#else
      SelectObject(hdc,
         cEGA[color].hPen[(thickness == THICK_WIDTH) ? USERBIT_LINE : style]);
      Line(hdc, x1, y1, x2, y2);
#endif
      return;
   }/* elif */
}/* pipe */
/* ----------------------------------------------------------------------- */
/*
void main (void){
   int gm, gd = DETECT, i, cx, cy;

   PIPELINE pl1;
   PIPELINE pl2;
   PIPELINE pl3;
   PIPELINE pl4;

   initgraph(&gd, &gm, "");

   cx = 400;
   cy = 400;
   pl1.S = cx / 10; pl1.C1 = EGA_WHITE; pl1.C2 = EGA_GREEN; pl1.O = 0; pl1.DO = 1;
   pl2.S = cx / 20; pl2.C1 = EGA_WHITE; pl2.C2 = EGA_GREEN; pl2.O = 0; pl2.DO = 1;
   pl3.S = cx / 30; pl3.C1 = EGA_WHITE; pl3.C2 = EGA_GREEN; pl3.O = 0; pl3.DO = 1;
   pl4.S = cx / 40; pl4.C1 = EGA_WHITE; pl4.C2 = EGA_GREEN; pl4.O = 0; pl4.DO = 1;

   pl1.xs = 0,  pl1.ys = 0,  pl1.xe = cx, pl1.ye = 0;
   pl2.xs = cx, pl2.ys = 0,  pl2.xe = cx, pl2.ye = cy;
   pl3.xs = cx, pl3.ys = cy, pl3.xe = 0,  pl3.ye = cx;
   pl4.xs = 0,  pl4.ys = cx, pl4.xe = 0,  pl4.ye = 0;

   while(1){

      pipeline(&pl1, -1);
      pipeline(&pl2, -1);
      pipeline(&pl3, -1);
      setlinestyle(SOLID_LINE, 0, 3);
      pipeline(&pl4, -2);
      setlinestyle(SOLID_LINE, 0, 1);

      if(kbhit()){
         pipeline(&pl1, EGA_BLACK);
         pipeline(&pl2, EGA_BLACK);
         pipeline(&pl3, EGA_BLACK);
         pipeline(&pl4, EGA_BLACK);
         switch(getch()){ // Norm
            case 27:
               closegraph();
               exit(0);
            case 'c':
               cleardevice();
               break;
            case 'z':
               pl1.C1++; pl1.C2 ++;
               pl2.C1++; pl2.C2 ++;
               pl3.C1++; pl3.C2 ++;
               pl4.C1++; pl4.C2 ++;
               break;
            case 'x':
               pl1.C1--; pl1.C2 --;
               pl2.C1--; pl2.C2 --;
               pl3.C1--; pl3.C2 --;
               pl4.C1--; pl4.C2 --;
               break;
            case '+':
               pl1.S++;
               pl2.S++;
               pl3.S++;
               pl4.S++;
               break;
            case '-':
               pl1.S--;
               pl2.S--;
               pl3.S--;
               pl4.S--;
               break;
            case 0:
               switch(getch()){ // Ext
                  case 0x48: // UP
                     cy--;
                     break;
                  case 0x50: // DOWN
                     cy++;
                     break;
                  case 0x4D: // RIGHT
                     cx++;
                     break;
                  case 0x4B: // LEFT
                     cx--;
                     break;
               }/* switch */
               break;
			}/* switch */
      }/* if */
   }/* while */
}/* main */
*/
/* ----------------------------------------------------------------------- */

