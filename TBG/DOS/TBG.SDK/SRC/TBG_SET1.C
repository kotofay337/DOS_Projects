/* tbg_set1.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#ifndef __WIN32__
#include <graphics.h>
#endif
#include "tbg_tc.h" /* обьявления функций и данных */
/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1993 */
/*--------------------------------------------------------------------------*/
void setviewport_obj_num(OBJECT **obj,unsigned char numobj,unsigned char num)
{
 short int X_port_max,X_port_min=0,
     Y_port_max,Y_port_min=0;
#ifndef __WIN32__
     X_port_max = getmaxx();
     Y_port_max = getmaxy();
#else
     X_port_max = ((struct HTBG*)*(obj))->SIZEX;
     Y_port_max = ((struct HTBG*)*(obj))->SIZEY;
#endif

 if(((struct HTBG*)*(obj))->ident == 0x5a5a)
 {
   getsize_obj_num(obj,numobj,num,
                   &X_port_min,&Y_port_min,
		   &X_port_max,&Y_port_max);
 }// if

#ifndef __WIN32__
 setviewport(X_port_min,Y_port_min,
             X_port_max,Y_port_max,1);
#endif

}// setviewport_obj_num
/*--------------------------------------------------------------------------*/