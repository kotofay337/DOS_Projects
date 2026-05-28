/* -----------------------------------------------------------------------
*
*  Config.C
*
*  Закладки и конфигурирование программы
*
*  (C) 2001 Чумак А. И.
*  ----------------------------------------------------------------------- */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <bios.h>
#include <time.h>
#include <graphics.h>
#include <math.h>
#include "tbg_tc.h"
#include "dbf.h"
#include "data.h"

#define DEBUG

/* ----------------------------------------------------------------------- */
int Tab_1 (void);
int Tab_2 (void);
int Tab_3 (void);
int Tab_4 (void);
int Tab_5 (void);
int Tab_6 (void);
int Tab_7 (void);


typedef
   enum{
      START,
      WAITCOMMAND,
      EXECUTE,
      STOPPROGRAM
   }SCFG;

static SCFG Step = START;

// список файлов картинок закладок
static char *pTab[] = {
"tbg\\shcfgtb0.tbg", // 0-я всегда фон
"tbg\\shcfgtb1.tbg", // 1 закладка
"tbg\\shcfgtb2.tbg",
"tbg\\shcfgtb3.tbg",
"tbg\\shcfgtb4.tbg",
"tbg\\shcfgtb5.tbg",
"tbg\\shcfgtb6.tbg",
"tbg\\shcfgtb7.tbg"
};

// список файлов картинок закладок
static char *pTabFilled[] = {
"",
"tbg\\shcfgtf1.tbg", // 1 закладка
"tbg\\shcfgtf2.tbg",
"tbg\\shcfgtf3.tbg",
"tbg\\shcfgtf4.tbg",
"tbg\\shcfgtf5.tbg",
"tbg\\shcfgtf6.tbg",
"tbg\\shcfgtf7.tbg"
};

#define TABS      (sizeof(pTab) / sizeof(pTab[0]))
#define OffsTabsV (32760 / 480 * (TABS - 2))
#define SizeOneLine (32760 / 480)

// массив указателей на картинки закладок
static OBJECT **pic[TABS] = {NULL, };
// кнопки активации закладок - строка символов
static char sKeyTabs[TABS + 1] = {'\0',};

// текущая закладка
static int cTab;
// указатель на картинку наполнения текущей закладки
static OBJECT **pf = NULL;

// таблица функций обработки закладок
static int (*pfTabs[])(void) = {
NULL, // 0 - не используется
NULL, // 1
Tab_2, // 2
NULL, // 3
NULL, // 4
NULL, // 5
NULL, // 6
Tab_7  // 7
};

/* ----------------------------------------------------------------------- */
void LoadAllPic (void){
   int i;
   for(i = 0;i < TABS;i++){
      load_file(&(pic[i]), pTab[i]);
      // в первой позиции строки находится символ активации
      sKeyTabs[i] = gettext_obj_num(pic[i], 1, 0)[0];
   }/* for */
}/* LoadAllPic */
/* ----------------------------------------------------------------------- */
void OutAllPic (int nTab){
   int i, j = 0;
   int OffsetTabVert = 0;

   // проверить диапазон
   if(nTab > TABS || nTab <= 0){
      return;
   }/* if */

   // выводим фон
   outpic_all(pic[0]);
   // выводим те что после выбранной, в обратном порядке
   for(i = nTab - 1;i > 0;i--){
      offset_pic(pic[i], 0, OffsetTabVert);
      outpic_all(pic[i]);
      OffsetTabVert += OffsTabsV;
      OffsetTabVert -= j * SizeOneLine;
      j++;
   }/* for */
   // выводим те что после нужного но выводим их с конца списка
   for(i = TABS - 1;i >= nTab;i--){
      offset_pic(pic[i], 0, OffsetTabVert);
      outpic_all(pic[i]);
      OffsetTabVert += OffsTabsV;
      OffsetTabVert -= j * SizeOneLine;
      j++;
   }/* for */
   // запомним текущий
   cTab = nTab;

}/* OutAllPic */
/* ----------------------------------------------------------------------- */
// удалить все картинки
void DelAllPic (void){
   int i;
   for(i = 0;i < TABS;i++){
      del_pic(&(pic[i]));
   }/* for */
}/* DelAllPic */
/* ----------------------------------------------------------------------- */
int Tab_2 (void){
   typedef
      enum{
         START,
         WAITCOMMAND,
         STOP,
      }STEP;

   static STEP S = START;

   if(S == START){
      // подсвечиваем название
      outpic_obj_num_color(pic[cTab], 1, 0, EGA_LIGHTGREEN);
      // загрузить наполнение текущей закладки
      del_pic(&pf);
      load_file(&pf, pTabFilled[cTab]);
      // показать
      outpic_all(pf);

      S = WAITCOMMAND;
      return 0;
   }/* if */

   if(S == WAITCOMMAND){
      if(Key == ESC){
         S = STOP;
      }/* if */
      if(ExtKey == 68){ // F10
         S = STOP;
      }/* if */
      return 0;
   }/* if */

   if(S == STOP){
      // подсвечиваем название
      outpic_obj_num(pic[cTab], 1, 0);
      del_pic(&pf);
      S = START;
      return 1;
   }/* if */

   return -1;
}/* Tab_2 */
/* ----------------------------------------------------------------------- */
void OutClock(void){
    static struct time oldtm;
    if((oldtm.ti_hund != tm.ti_hund) || (oldtm.ti_sec != tm.ti_sec) || (oldtm.ti_min != tm.ti_min) || (oldtm.ti_hour != tm.ti_hour)){
       oldtm = tm;
       // фон
       outpic_obj_num(pf, 33, 255);
       // часовая
       outpic_obj_num_with_rotate(pf, 33, 1, (double)((tm.ti_hour) * 15.0)
                                             + (tm.ti_min * .25)
                                             + (tm.ti_sec * .00666666666));
       // мин
       outpic_obj_num_with_rotate(pf, 33, 2, (double)((tm.ti_min) * 6.0)
                                             + (tm.ti_sec * .1));
       // сек
       outpic_obj_num_with_rotate(pf, 33, 3, (double)(tm.ti_sec) * 6.0
                                             + (tm.ti_hund * .06));
    }/* if */
}/* OutClock */
/* ----------------------------------------------------------------------- */
int Tab_7 (void){
   typedef
      enum{
         START,
         WAITCOMMAND,
         STOP,
      }STEP;

   static STEP S = START;
   static long t;
   static int CurDO, CurDN, CurDColor, Blink;
   static struct date td;

   if(S == START){
      // подсвечиваем название
      outpic_obj_num_color(pic[cTab], 1, 0, EGA_LIGHTGREEN);
      // загрузить наполнение текущей закладки
      del_pic(&pf);
      load_file(&pf, pTabFilled[cTab]);
      // показать
      outpic_all(pf);

      outtext_obj_num(pf, 1, 255, EGA_LIGHTGRAY, GetMonthName(&dt), NO_SHOW);
      // инициализировать календарь
      // тек. месяца
      {
         int o = 1, nc = 0, d;
         td = dt;
         dt.da_day = 1;
         for(dt.da_day = 1;dt.da_day <= DayInMonth(&dt);dt.da_day++){
            char s[16];
            nc = GetWeekDay(&dt);
            sprintf(s, "%2.1d", dt.da_day);
            setcolor_obj_num(pf, o, nc, GetDateType(&dt) == 'Р' ? EGA_GREEN : EGA_LIGHTRED, NO_SHOW);
            if(dt.da_day == td.da_day){
               CurDO = o;
               CurDN = nc;
               CurDColor = getcolor_obj_num(pf, o, nc);
            }/* if */
            outtext_obj_num(pf, o, nc, EGA_DARKGRAY, s, NO_SHOW);
            if(nc == 0){
               o++;
            }/* if */
         }/* for */
      }
      S = WAITCOMMAND;
      return 0;
   }/* if */

   if(S == WAITCOMMAND){
      OutClock();
      if(((bt - t) / CLK_TCK) >= 0.25F || bt == 0){
         t = bt;
         outpic_obj_num_color(pf, CurDO, CurDN, Blink == 0 ? CurDColor : EGA_YELLOW);
         Blink = !Blink;
      }/* if */
      if(Key == ESC){
         outpic_obj_num(pf, CurDO, CurDN);
         S = STOP;
      }/* if */
      if(ExtKey == 68){ // F10
         outpic_obj_num(pf, CurDO, CurDN);
         S = STOP;
      }/* if */
      return 0;
   }/* if */

   if(S == STOP){
      // подсвечиваем название
      outpic_obj_num(pic[cTab], 1, 0);
      del_pic(&pf);
      S = START;
      return 1;
   }/* if */

   return -1;
}/* Tab_7 */
/* ----------------------------------------------------------------------- */
// Конфигурирование программы при помощи "закладок" (Tab`s)
int ConfigProgram(void){

   if(Step == START){
      LoadAllPic();
      load_file(&p, "tbg\\shconfig.tbg");
      outpic_all(p);
      // вывести первую закладку
      OutAllPic(1);

      Step = WAITCOMMAND;
      return 0;
   }/* if */

   if(Step == WAITCOMMAND){
      OutTime(p);

      if(Key == ESC){
         Step = STOPPROGRAM;
         return 0;
      }/* if */

      if(Key && strchr(sKeyTabs, Key)){
         OutAllPic(Key - '0');
         Step = EXECUTE;
      }/* if */
      return 0;
   }/* if */

   if(Step == EXECUTE){
      OutTime(p);
      if(pfTabs[cTab]){
         switch(pfTabs[cTab]()){
            case 1:
               Step = WAITCOMMAND;
               break;
            case 0:
               break;
            case -1:
               break;
            default:
               break;
         }/* switch */
      }/* if */
      else{
         Step = WAITCOMMAND;
      }/* elif */
      return 0;
   }/* if */

   if(Step == STOPPROGRAM){
      del_pic(&p);
      DelAllPic();
      Step = START;
      return 1;
   }/* if */

   return 1;
}/* ConfigProgram */
/* ----------------------------------------------------------------------- */
