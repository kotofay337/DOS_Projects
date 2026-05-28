#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include "tbg_tc.h" /* обьявления функций и данных */

void cursor_draw(void);

static int cursor_mode = ABSENT;
static short oldmx, oldmy, off_mode = 0, cursor_tbg = OFF;
       short mx, my;

OBJECT **cursor_pic = NULL;
/* ----------------------------------------------------------------------- */
int absx(int crd){
   return (int)(0.5+((crd/(double)getmaxx())*SIZE_XY));
}/* ABSx */
/* ----------------------------------------------------------------------- */
int absy(int crd){
   return (int)(0.5+((crd/(double)getmaxy())*SIZE_XY));
}/* ABSy */
/* ----------------------------------------------------------------------- */
void cursor_ini(OBJECT *p){
   cursor_mode = PRESENT;
   mx = getmaxx() / 2;
   my = getmaxy() / 2;
   if(p != NULL){
      cursor_pic = p;
      cursor_tbg = ON;
   }/* if */
   else{
      cursor_pic = NULL;
      cursor_tbg = OFF;
   }/* elif */
}/* cursor_ini */
/* ----------------------------------------------------------------------- */
void cursor_reini(void){
   cursor_mode = ABSENT;
   cursor_tbg = OFF;
}/* cursor_ini */
/* ----------------------------------------------------------------------- */
void cursor_setxy(int x, int y){
   mx = x;
   my = y;
}/* cursor_ini */
/* ----------------------------------------------------------------------- */
void cursor_on(void){
   cursor_mode = CURSOR_ON;
   cursor_draw();
}/* cursor_on */
/* ----------------------------------------------------------------------- */
void cursor_off(void){
   cursor_mode = CURSOR_OFF;
   cursor_draw();
}/* cursor_off */
/* ----------------------------------------------------------------------- */
void cursor_draw(void){

#define __CURSOR__DRAW__()\
            if(cursor_tbg == ON)\
            {\
               offset_pic(cursor_pic, absx(mx), absy(my));\
               outpic_all(cursor_pic);\
            }\
            else\
            {\
               setlinestyle(SOLID_LINE, 0, 3);\
               setcolor(8);\
               line(mx - 8, my, mx + 8, my);\
               line(mx, my - 8, mx, my + 8);\
               setlinestyle(SOLID_LINE, 0, 1);\
               setcolor(7);\
               rectangle(mx - 1, my - 1, mx + 1, my + 1);\
               line(mx - 7, my, mx + 7, my);\
               line(mx, my - 7, mx, my + 7);\
            }


#define __CURSOR__CLEAR__()\
            if(cursor_tbg == ON)\
            {\
               offset_pic(cursor_pic, absx(oldmx), absy(oldmy));\
               outpic_all(cursor_pic);\
            }\
            else\
            {\
               setlinestyle(SOLID_LINE, 0, 3);\
               setcolor(8);\
               line(oldmx - 8, oldmy, oldmx + 8, oldmy);\
               line(oldmx, oldmy - 8, oldmx, oldmy + 8);\
               setlinestyle(SOLID_LINE, 0, 1);\
               setcolor(7);\
               rectangle(oldmx - 1, oldmy - 1, oldmx + 1, oldmy + 1);\
               line(oldmx - 7, oldmy, oldmx + 7, oldmy);\
               line(oldmx, oldmy - 7, oldmx, oldmy + 7);\
            }

   if(cursor_mode == ABSENT)
      return;

   if(mx > getmaxx()){
      mx = getmaxx();
   }/* if */

   if(my > getmaxy()){
      my = getmaxy();
   }/* if */

   setwritemode(XOR_PUT);

   switch(cursor_mode){
      case CURSOR_ON:
         // draw in NEW coord's
         if(++off_mode >= 0){
            __CURSOR__DRAW__();
            off_mode = 0;
            oldmx = mx;
            oldmy = my;
            cursor_mode = CURSOR_DRAW;
         }/* if */
         else{
            cursor_mode = CURSOR_NODRAW;
         }/* elif */
         break;
      case CURSOR_OFF:
         // clear on old coord's - mouse_off
         if(off_mode == 0)
            __CURSOR__CLEAR__();
         cursor_mode = CURSOR_NODRAW;
         off_mode--;
         break;
      case CURSOR_DRAW:
         // clear on old coord's
         // draw in NEW coord's
         if((oldmx != mx) || (oldmy != my)){
            __CURSOR__CLEAR__();
            __CURSOR__DRAW__();
            oldmx = mx;
            oldmy = my;
         }/* if */
         break;
      case CURSOR_NODRAW:
         // no draw
         break;
   }/* switch */

   setwritemode(COPY_PUT);

}/* cursor_draw */
/* ----------------------------------------------------------------------- */
#pragma argsused
void main(int argc, char *argv[]){
   int gm, gd = DETECT;
	short int x = 0, y = 0;
   OBJECT **pic = NULL, **fon;

   initgraph(&gd, &gm, "d:\\tbg\\bin");

   setpalette_tbg();

   load_file(&pic, "cursor.tbg");
   load_file(&fon, argv[1]);

   mouse_ini();

   cursor_ini(pic);

   outpic_all(fon);

   cursor_on();
   while(1){
      cursor_setxy(x, y);
      if(kbhit()){
         switch(getch()){
            case 'a':
               x--;
               break;
            case 'd':
               x++;
               break;
            case 'w':
               y--;
               break;
            case 'x':
               y++;
               break;
            case 033:
               goto go;
         }/* switch */
         mouse_setxy(x, y);
      }/* if */
      else{
         mouse_getxy(&x, &y);
      }/* elif */
      cursor_draw();
   }/* while */

go:;
   cursor_off();
   cursor_reini();
   del_pic(&pic);
   closegraph();

   return;
}/* main */
/* ----------------------------------------------------------------------- */
