/* ----------------------------------------------------------------------- */
/*

   v 0.01 prealpha

   Чумак А.И.

   (С) 1998-21-10 16:23.11

*/
/* ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <graphics.h>
/* ----------------------------------------------------------------------- */
static cx = 1, cy = 1;
static int SIZE_X = 8,
           SIZE_Y = 8;
/* ----------------------------------------------------------------------- */
int gprintf(char *fmt, ...){

   char buffer[256];
   char s[2];
   va_list argptr;
   int cnt, i;

   va_start(argptr, fmt);
   cnt = vsprintf(buffer, fmt, argptr);
   va_end(argptr);

   if(buffer[0] == '√'){// 251 decimal
      cx = getx();
      cy = gety();
      i = 1;
   }/* if */
   else{
      i = 0;
   }/* elif */

   settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
   setfillstyle(SOLID_FILL, 0);

   setcolor(7);

   s[1] = 0;
   for(;i < strlen(buffer);i++){
      s[0] = buffer[i];
      SIZE_X = textwidth(s);
      SIZE_Y = textheight(s);
      switch(s[0]){
         case '\b':
            cx -= textwidth("W");
            if(cx < 1)
               cx = 1;
            break;
         case '\t':
            cx += textwidth("W") * 8;
            if(cy > getmaxy() - textwidth("W"))
               cy = 1;
            break;
         case '\n':
            cy += SIZE_Y;
            if(cy > getmaxy() - textwidth("W"))
               cy = 1;
         case '\r':
            cx = 1;
            break;
         default:
            bar(cx, cy, cx + SIZE_X, cy + 1 + SIZE_Y);
            outtextxy(cx + 1, cy + 1 + SIZE_Y, s);
            cx += SIZE_X;
            if(cy > getmaxy() - textheight("W"))
               cy = 1;
            if(cx > getmaxx() - textwidth("W"))
               cx = 1;
            break;
      }/* switch */
   }/* for */
   return(cnt);
}/* gprintf */
/* ----------------------------------------------------------------------- */
