#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <dir.h>
#include <conio.h>
#include <string.h>
#include <alloc.h>
#include <process.h>
#include <dos.h>
#include <io.h>
#include <bios.h>
#include <time.h>
#include <graphics.h>
#include <ctype.h>
/* ----------------------------------------------------------------------- */
static char *sep = "\x0E\n";
int TextWidthCol (char *txt){
   char *dup, *t;
   int smx = 0;

   // вычислить длины строк выбрать максимальную
   t = dup = strdup(txt);
   strtok(dup, sep);
   if(textwidth(t) >  smx){
      smx = textwidth(t);
   }/* if */
   while((t = strtok(NULL, sep)) != NULL){
      if(textwidth(t) >  smx){
         smx = textwidth(t);
      }/* if */
   }/* while */
   free(dup);
   return smx;
}/* TextWidthCol */
/* ----------------------------------------------------------------------- */
int TextHeightCol (char *txt){
   char *dup, *t;
   int smy = 0;

   // вычислить высоты строк и сложить их все
   t = dup = strdup(txt);
   strtok(dup, sep);
   if(textheight(t) >  smy){
      smy += textheight(t) + 3;
   }/* if */
   while((t = strtok(NULL, sep)) != NULL){
      smy += textheight(t) + 3;
   }/* while */
   free(dup);
   return smy;
}/* TextHeightCol */
/* ----------------------------------------------------------------------- */
void OutTextCol(int x, int y, char *txt){
   char *dup, *t, *tok;
   int ty;
   struct textsettingstype ti;

   gettextsettings(&ti);
   switch(ti.horiz){
      case CENTER_TEXT:
         x -= TextWidthCol(txt) / 2;
         break;
      case RIGHT_TEXT:
         x -= TextWidthCol(txt);
         break;
   }/* switch */

   switch(ti.vert){
      case CENTER_TEXT:
         y -= TextHeightCol(txt) / 2;
         break;
      case BOTTOM_TEXT:
         y -= TextHeightCol(txt);
         break;
   }/* switch */

   settextjustify(LEFT_TEXT, TOP_TEXT);
   t = dup = strdup(txt);
   strtok(dup, sep);
   moveto(x, y);
   ty = y;
   outtext(t);
   while((t = strtok(NULL, sep)) != NULL){
      if(ti.direction == HORIZ_DIR){
         moveto(x, ty += textheight(t) + 3);
      }/* if */
      else{
         moveto(x += textheight(t) + 3, ty);
      }/* elif */
      outtext(t);
   }/* while */
   free(dup);

}/* OutText */
/* ----------------------------------------------------------------------- */
/* draw an "x" at (x, y) */
void xat(int x, int y)
{
  line(x-4, y, x+4, y);
  line(x, y-4, x, y+4);
}
/* ----------------------------------------------------------------------- */
void main (void){
   /* horizontal text justification settings */
   char *hjust[] = { "LEFT_TEXT\n",
                     "CENTER_TEXT\n",
                     "RIGHT_TEXT\n"
                   };

   /* vertical text justification settings */
   char *vjust[] = { "LEFT_TEXT",
                     "CENTER_TEXT",
                     "RIGHT_TEXT"
                   };
   int gm, gd = DETECT;
   int x = 100, y = 100;
   char *s = "HelloHello\x0Eyybbgg\x0E!!!";
   int midx, midy, hj, vj;
   char msg[80];

   initgraph(&gd, &gm, "c:\\tbg\\bin");

   midx = getmaxx() / 2;
   midy = getmaxy() / 2;

   settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
   /* loop through text justifications */
   for (hj=LEFT_TEXT; hj<=RIGHT_TEXT; hj++)
      for (vj=LEFT_TEXT; vj<=RIGHT_TEXT; vj++)
      {
         cleardevice();
         /* set the text justification */
         settextjustify(hj, vj);

         /* create a message string */
         sprintf(msg, "%s%s", hjust[hj], vjust[vj]);

         /* create cross hairs on the screen */
         xat(midx, midy);

         /* output the message */
         OutTextCol(midx, midy, msg);
         getch();
      }


   closegraph();
}/* main */
/* ----------------------------------------------------------------------- */
