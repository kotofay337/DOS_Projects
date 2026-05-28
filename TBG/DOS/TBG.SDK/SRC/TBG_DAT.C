/* tbg_dat.c ----------------------------------------------------------------
                  ‘ункции интерфейса —и с TBG-файлами

                    √рафический пакет "“урЅо√рафика"
                      (C) ѕавельев ѕ.». 1990-95
---------------------------------------------------------------------------*/

#ifndef __WIN32__
#include <graphics.h>
#endif
#include "tbg_tc.h" /* обь€влени€ функций и данных */
/*--------------------------------------------------------------------------*/
char Tbl_Present=No; /* библитека есть */
struct HTBL   hl;
struct ITEM *pitDir;

FILE *fd, *fd_tmp;

char old_name[80];

unsigned short int iCountITEM=0;
unsigned short int iALLITEM  =0;

#ifndef __WIN32__
int  GraphDriver=DETECT,GraphMode;
#else
char CurrentTBLName[256];
#endif

char Show_tbg=OFF;/* флаг зан€тости TBG выводом на экран =свободно= */

short int size_xya = SIZE_XY; /* текущий размер окна             */
short int xa_min = 0,ya_min = 0;/* начало окна -абсолютный размер- */

double SX_vp;/* размер стороны видеопорта */
double SY_vp;/* размер стороны видеопорта */

/* размеры видеопорта */
short int MaxX_vp;
short int MaxY_vp;

/* размеры полного экрана */
short int    iMaxX;
short int    iMaxY;
double dMaxX;
double dMaxY;

char MouseSet = OFF;/* признак наличи€ 'мыши' */
short int lpress_lb;/* эмул€ци€ было нажатие левой кнопки 'мыша' */
short int lpress_rb;/* эмул€ци€ было нажатие пpавой кнопки 'мыша' */
short int cpress_lb;/* эмул€ци€ сейчас нажата лева€ кнопки 'мыша' */
short int cpress_rb;/* эмул€ци€ сейчас нажата пpава€ кнопки 'мыша' */

short int error_tbg=ER_OK;/* ошибка выполнени€ функции интерфейса */

/*--------------------------------------------------------------------------*/
