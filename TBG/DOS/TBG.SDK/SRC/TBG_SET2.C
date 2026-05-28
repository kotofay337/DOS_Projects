/* tbg_set2.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/


#ifndef __WIN32__
#include <graphics.h>
#endif
#include "tbg_tc.h" /* обьявления функций и данных */
/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1993-95 */
/*--------------------------------------------------------------------------*/
void setpalette_tbg(void) /* установка "улучшеной" палитры */
{
#ifndef __WIN32__
 struct palettetype pal;

 pal.size = 15;
 pal.colors[ 0] = EGA_BLACK;
 pal.colors[ 1] = EGA_BLUE;
 pal.colors[ 2] = EGA_GREEN;
 pal.colors[ 3] = EGA_CYAN;
 pal.colors[ 4] = EGA_RED;
 pal.colors[ 5] = 38;// EGA_MAGENTA оранжевый GOLD
 pal.colors[ 6] = 14;// EGA_BROWN болото BOLOTO
 pal.colors[ 7] = EGA_LIGHTGRAY;
 pal.colors[ 8] = EGA_DARKGRAY;
 pal.colors[ 9] = 25;// EGA_LIGHTBLUE поярче
 pal.colors[10] = EGA_LIGHTGREEN;
 pal.colors[11] = 27;// EGA_LIGHTCYAN поярче
 pal.colors[12] = 36;// EGA_LIGHTRED  поярче
 pal.colors[13] = 39;// EGA_LIGHTMAGENTA пастельный PASTEL
 pal.colors[14] = 54;// EGA_YELLOW    поярче
 pal.colors[15] = EGA_WHITE;

 setallpalette(&pal);
#endif

}/* setpalette_tbg */
/*--------------------------------------------------------------------------*/