/* tbg_smxy.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#ifndef __WIN32__
#include <graphics.h>
#endif
#include "tbg_tc.h" /* обьявления функций и данных */
/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/*--------------------------------------------------------------------------*/
void set_MaxXY(OBJECT **obj) /* установка параметров для вывода в окно */
{
#ifndef __WIN32__
 struct viewporttype viewinfo;

 getviewsettings(&viewinfo);

 // размер стороны видеопорта
 SX_vp = (double)(viewinfo.right - viewinfo.left);
 SY_vp = (double)(viewinfo.bottom - viewinfo.top);

 // размеры видеопортa: 0,0-MaxX_vp,MaxY_vp
 MaxX_vp = viewinfo.right - viewinfo.left;
 MaxY_vp = viewinfo.bottom - viewinfo.top;

// printf("              %d %d   %d  %d \r", MinX_vp, MinY_vp, MaxX_vp, MaxY_vp);
 /* размеры полного экрана */
 iMaxX = getmaxx(); dMaxX = (double)iMaxX;
 iMaxY = getmaxy(); dMaxY = (double)iMaxY;
//printf("dos: SX_vp: %lf, SY_vp: %lf, MaxX_vp: %d, MaxY_vp: %d, iMaxX: %d, iMaxY: %d, dMaxX: %lf, dMaxY: %lf\n",
  //      SX_vp, SY_vp, MaxX_vp, MaxY_vp, iMaxX, iMaxY, dMaxX, dMaxY);
#else

 RECT r;

 // размер стороны видеопорта
 SX_vp = (double)(((struct HTBG*)*(obj))->SIZEX - 1);
 SY_vp = (double)(((struct HTBG*)*(obj))->SIZEY - 1);

 // размеры видеопортa: 0,0-MaxX_vp,MaxY_vp
 MaxX_vp = ((struct HTBG*)*(obj))->SIZEX - 1;
 MaxY_vp = ((struct HTBG*)*(obj))->SIZEY - 1;

// printf("              %d %d   %d  %d \r", MinX_vp, MinY_vp, MaxX_vp, MaxY_vp);

 /* размеры полного экрана */
 GetClientRect(((struct HTBG*)*(obj))->hwnd, &r);
 iMaxX = r.right;  dMaxX = (double)iMaxX;
 iMaxY = r.bottom; dMaxY = (double)iMaxY;

//printf("win: SX_vp: %lf, SY_vp: %lf, MaxX_vp: %d, MaxY_vp: %d, iMaxX: %d, iMaxY: %d, dMaxX: %lf, dMaxY: %lf\n",
//             SX_vp, SY_vp, MaxX_vp, MaxY_vp, iMaxX, iMaxY, dMaxX, dMaxY);

#endif

}/* set_MaxXY */
/*--------------------------------------------------------------------------*/
