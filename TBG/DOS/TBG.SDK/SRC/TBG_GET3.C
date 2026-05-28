/* tbg_get3.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */
#ifndef __WIN32__
#include <graphics.h>
#endif
/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/*--------------------------------------------------------------------------*/
/* выяснение позиции "мыши" */
short int getpos_mouse_obj(OBJECT **pic,unsigned char numobj,unsigned char StartNum)
{
 short int i;
 short int max;
 short int xmin, ymin, xmax, ymax;
 short int x,y;

 error_tbg = ER_OK; /* нет ошибок */

  if(((struct HTBG*)*(pic))->ident != 0x5a5a)
  {/* нет картинки */
   error_tbg = ER_NOTPIC;
   return -1;
  }/* if */

  if(((struct HTBG*)*(pic))->Num_LE==-1)
  {/* нет логических элементов */
   error_tbg = ER_NOTLE;
   return -1;
  }/* if */

 getmax_obj(pic,numobj,&max);

 if(error_tbg)
 {/* нет обьекта */
   error_tbg = ER_NOTOBJ;
   return -1;
 }/* if */


 if(StartNum > max)
 {/* слишком большой стартовый номер */
   error_tbg = ER_STARTNUM;
   return -1;
 }/* if */

 mouse_getxy(&x,&y);/* берем координаты мыши */

 for(i=StartNum;i <= max;i++)/* max-1 */
 {

  getsize_obj_num(pic,numobj,i,&xmin,&ymin,&xmax,&ymax);
//  printf("[%d] {%d %d %d %d = %d %d}\n", i, xmin, ymin, xmax, ymax, x, y);
  if((x >= xmin)&&
     (x <= xmax)&&
     (y >= ymin)&&
     (y <= ymax)
    ) return i;/* принадлежит */

 }//for

 return -1;
}// getpos_mouse_obj
/*--------------------------------------------------------------------------*/