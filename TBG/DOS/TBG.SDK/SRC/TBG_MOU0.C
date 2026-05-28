/* tbg_mou0.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/


#include <graphics.h>
#include "tbg_tc.h" /* обьявления функций и данных */

int mouse_mode = ABSENT;

/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/*--------------------------------------------------------------------------*/
short int mouse_ini()
{
union REGS inregs;

 inregs.x.ax=0;
 int86(0x33,&inregs,&inregs);

// printf("\nMouse= %u \n",inregs.x.ax);
// getch();

 if((short int)inregs.x.ax < 0) MouseSet = ON; // 'мышь' есть
   else {                 MouseSet = OFF; // 'мыши' нет
/*			  printf("\nNot Mouse !\007");*/
 }// else if

 mouse_mode = ABSENT;

 return(inregs.x.ax);

}/* mouse_ini */
/* ----------------------------------------------------------------------- */
int gprintf(char *fmt, ...);
/*--------------------------------------------------------------------------*/
void mouse_draw(void){
   static short oldmx, oldmy, off_mode = 0;
          short mx, my;


#define __MOUSE__DRAW__()\
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


#define __MOUSE__CLEAR__()\
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

   if(mouse_mode == ABSENT)
      return;

   mouse_getxy(&mx, &my);

   if(mx > getmaxx()){
      mx = getmaxx();
      mouse_setxy(mx, my);
   }/* if */

   if(my > getmaxy()){
      my = getmaxy();
      mouse_setxy(mx, my);
   }/* if */

   setwritemode(XOR_PUT);

   switch(mouse_mode){
      case MOUSE_ON:
         // draw in NEW coord's
         if(++off_mode >= 0){
            __MOUSE__DRAW__();
            off_mode = 0;
            oldmx = mx;
            oldmy = my;
            mouse_mode = MOUSE_DRAW;
         }/* if */
         else{
            mouse_mode = MOUSE_NODRAW;
         }/* elif */
         break;
      case MOUSE_OFF:
         // clear on old coord's - mouse_off
         if(off_mode == 0)
            __MOUSE__CLEAR__();
         mouse_mode = MOUSE_NODRAW;
         off_mode--;
         break;
      case MOUSE_DRAW:
         // clear on old coord's
         // draw in NEW coord's
         if((oldmx != mx) || (oldmy != my)){
            __MOUSE__CLEAR__();
            __MOUSE__DRAW__();
            oldmx = mx;
            oldmy = my;
         }/* if */
         break;
      case MOUSE_NODRAW:
         // no draw
         break;
   }/* switch */

   setwritemode(COPY_PUT);

}/* mouse_draw */
/* ----------------------------------------------------------------------- */
