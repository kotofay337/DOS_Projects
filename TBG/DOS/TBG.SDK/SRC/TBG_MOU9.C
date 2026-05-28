/* tbg_mou9.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/


#include <graphics.h>
#include "tbg_tc.h" /* обьявления функций и данных */
/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990 */
/*--------------------------------------------------------------------------*/
void mouse_keyxy(short int *key_left,short int *key_right,short int *key_press,
                               short int *x_mouse,  short int *y_mouse)
{
union REGS inregs;

 if(MouseSet == OFF) {
    (*x_mouse)  =0;
    (*y_mouse)  =0;
    (*key_press)=0;
    (*key_left) =0;
    (*key_right)=0;
    return;// 'мыши' нет
 }// if

 inregs .x.ax=3;
 int86(0x33,&inregs,&inregs);
 (*x_mouse)  =inregs.x.cx;
 (*y_mouse)  =inregs.x.dx;
 (*key_press)=inregs.x.bx;

 inregs .x.ax=5;
 inregs .x.bx=0;/* 0-левая кнопка */
 int86(0x33,&inregs,&inregs);
 (*key_left)=inregs.x.bx;

 inregs .x.ax=5;
 inregs .x.bx=1;/* 1-правая кнопка */
 int86(0x33,&inregs,&inregs);
 (*key_right)=inregs.x.bx;

}/* mouse_keyxy */

/*--------------------------------------------------------------------------*/