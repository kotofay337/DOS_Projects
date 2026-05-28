/* dt_000.c ---------------------------------------------------------------*/
/****************************************************************************
               Демонстрационно-тестовая программа
                функций интерфейса с CGM-файлами.

                      Borland C++ 2.0

              (C) Павельев П.И.,1994 (20 сентября)
****************************************************************************/

#include "cgm.h"
/* при отсутствии "cgm_doc.h" закоментировать нижнюю строку  */
#include "cgm_doc.h" /* тестирование прототипов в документации */

#include <conio.h>
#include <alloc.h>
#include <graphics.h>
#include <time.h>

#include "dt.h"

/*-------------------------------------------------------------------------*/
static int GD=EGA,GM=EGAHI;
static OBJECT *dt_titul;
static int pos;
/*
static OBJECT *dt_gp;
static OBJECT *dt_frm;

static OBJECT *dt_text;
static OBJECT *dt_text_tbg;
static OBJECT *dt_text_cgm;
*/
/*-------------------------------------------------------------------------*/
void main(void)
{
 long start_mem = coreleft();/* общий контроль распределения памяти */

 IndicErr=ON;/* индикация ошибок функций интерфейса */
 IndicErr=OFF;/* индикация ошибок функций интерфейса */

 initgraph(&GD,&GM,"");/* инициализация работы с BGI */
 InitCGM();            /* инициализация работы с CGM */
 mouse_ini();          /* инициализация работы с "мышью" */
 setpalette_tbg();     /* наша "улучшенная" палитра */

 LoadTBGFile(&dt_titul,"dt_titul.tbg");/* загружаем титульный лист */
 ShowAll(dt_titul);/* показываем весь титульный лист */

 LoadTBGFile(&dt_err,"dt_err.tbg");  /* для индикации ошибок */
 LoadTBGFile(&dt_main,"dt_main.tbg");/* общее обрамление */

 if(LoadFile(&dt_mode,ndt_mode_cgm)!=0)/* режим */
 {/* cgm-файла нет */
  LoadTBGFile(&dt_mode,ndt_mode_tbg);
 }/* if */


 while(!kbhit())
 {/* помигиваем на титульном листе */
  /* установка цвета с показом изменений */
  SetColorObjNum(dt_titul,1,0,(GetColorObjNum(dt_titul,1,0)+1)%16,SHOW);
  /* установка цвета без показа изменений */
  SetColorObjNum(dt_titul,1,0,GetColorObjNum(dt_titul,1,0),NO_SHOW);
  /* помигиваем на титульном листе */
 }/* while */
 getch();


 ShowObj(dt_main,0,SHOW_DIR);/* общее обрамление */

/* set_mode_dt();/* задает режимы тестирования */*/

 ShowObj(dt_err, 0,SHOW_DIR);/* для индикации ошибок */

 /* Тесты.Последовательность не менять !!! */
 all_test=0;
 test1();/* первая группа тестов */
 /* Тесты.Последовательность не менять !!! */

 DelPic(&dt_titul);/* удаление титульного листа */
 DelPic(&dt_err);  /* для индикации ошибок */
 DelPic(&dt_main); /* общее обрамление */


 SavePicCGM(dt_mode,ndt_mode_cgm,ndt_mode_cgm);/* сохраняем */
 DelPic(&dt_mode); /* режим */

 DelPic(&dt1gp);
 DelPic(&dtagp);
 DelPic(&dt1gp1le);
 DelPic(&dtagpbe);


 CloseCGM();  /* завершение работы с CGM */
 closegraph();/* завершение работы с BGI */

 if((start_mem-coreleft())!= 0L)
 {/* общий контроль распределения памяти */
  printf("\n%ld\n%ld",start_mem,coreleft());
  printf("\nОй-Ой-Ой !!!");
  getch();
  /* общий контроль распределения памяти */
 }/* if */

}/* main */
/*-------------------------------------------------------------------------*/
int test_func(char *mess_test,int what)/* индикация результатов теста */
{
 char s_err[100],s[20];

 all_test++;

 /* формируем сообщение о возможной ошибке пока не изменилось имя функции */
/*
 sprintf(s_err,"%sстрока теста='%s'.Функция(%s).",
 FuncName+1,mess_test,NameTestFunc);
*/
 sprintf(s_err,"Ошибка в строке теста='%s'.Тестовая функция '%s'.",
 mess_test,NameTestFunc);

 sprintf(s,"%d:%s",all_test,mess_test);
 OutTextObjNum(dt_err,3,0,GetColorObjNum(dt_err,255,1),s,SHOW_CHECK);

 /* индикация рядом со строкой (всю строку слишком долго) */
 ShowObjColor   (dt_err,5,GetColorObjNum(dt_err,255,1));
 ShowObjNumColor(dt_err,5,cur_num_type_test,random(16)+1);
/* ShowObjNumColor(dt_err,5,cur_num_type_test,LIGHTGREEN);*/

 if(what){}/* Ok */
 else
 {/* Error */
  ShowObj(dt_err,1,SHOW_DIR);
  ShowObjNumColor(dt_err,1,cur_num_type_test,GetColorObjNum(dt_err,2,0));
  /* индикация строки с сообщением об ошибке */
  OutTextObjNum(dt_err,2,0,GetColorObjNum(dt_err,255,1),s_err,SHOW_DIR);
  getch();
  ShowObjNumColor(dt_err,2,0,GetColorObjNum(dt_err,255,1));
  ShowObjNum(dt_err,1,cur_num_type_test,SHOW_DIR);
  /* Error */
 }
 return what;
}/* test_func */
/*-------------------------------------------------------------------------*/
/*GetLenghtFile - возвращает длину файла.*/
long GetLenghtFile(char *filename)
{
 FILE *f=NULL;
 long length;

 f=fopen(filename,"r");
 if(f==NULL) return -1L;
 fseek(f, 0L, SEEK_END);
 length = ftell(f);
 fclose(f);

 return length;
}/*  GetLenghtFile */
/*-------------------------------------------------------------------------*/
void set_mode_dt(void) /* задает режимы тестирования */
{

 ShowObj(dt_mode,0,SHOW_DIR);/* режим */
 SetColorObj(dt_mode,1,
             GetColorObjNum(dt_mode,6,GetObjMin(dt_mode,6)),
             SHOW_DIR);/* все в пассивный цвет */
 SetColorObjNum(dt_mode,1,pos,
             GetColorObjNum(dt_mode,6,GetObjMax(dt_mode,6)),
             SHOW_DIR);/* активная позиция */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Задание теста- Пробел.Закончить - Esc,Enter.",SHOW_DIR);
 while(1)
 {/* бесконечный цикл выбора тестов */
  int key;
  if(kbhit()==0) continue;
  key=getch();
  if(key==0)
  {
    key=getch();
    switch(key)
    {
     case 72:/* вверх */
             SetColorObjNum(dt_mode,1,pos,
                            GetColorObjNum(dt_mode,6,GetObjMin(dt_mode,6)),
                            SHOW_DIR);/* пассивная позиция */
             if((--pos) < GetObjMin(dt_mode,1))
             {
                pos = GetObjMax(dt_mode,1);
             }/* if */
             SetColorObjNum(dt_mode,1,pos,
                            GetColorObjNum(dt_mode,6,GetObjMax(dt_mode,6)),
                            SHOW_DIR);/* активная позиция */
     break;
     case 80:/* вниз */
             SetColorObjNum(dt_mode,1,pos,
                            GetColorObjNum(dt_mode,6,GetObjMin(dt_mode,6)),
                            SHOW_DIR);/* пассивная позиция */
             if((++pos) > GetObjMax(dt_mode,1))
             {
                pos = GetObjMin(dt_mode,1);
             }/* if */
             SetColorObjNum(dt_mode,1,pos,
                            GetColorObjNum(dt_mode,6,GetObjMax(dt_mode,6)),
                            SHOW_DIR);/* активная позиция */
     break;

    }/* switch */
  }/* if */

  if(key==32)
  {/* пробел */
   if(GetColorObjNum(dt_mode,2,pos) ==
      GetColorObjNum(dt_mode,5,GetObjMin(dt_mode,5))
     )
     {/* была пассивная позиция */
      SetColorObjNum(dt_mode,2,pos,
             GetColorObjNum(dt_mode,5,GetObjMax(dt_mode,5)),
             SHOW_DIR);/* активная позиция */
      /* была пассивная позиция */
     }/* thif */
     else
     {/* была активная позиция */
      SetColorObjNum(dt_mode,2,pos,
             GetColorObjNum(dt_mode,5,GetObjMin(dt_mode,5)),
             SHOW_DIR);/* пассивная позиция */
      /* была активная позиция */
     }/* elif */
   /* пробел */
  }/* if */

  if(key==13) /* Enter */
    break;/* закончить бесконечный цикл */
  if(key==27) /* Esc */
    break;/* закончить бесконечный цикл */
  /* бесконечный цикл выбора тестов */
 }/* while */

}/*  set_mode_dt */
/*-------------------------------------------------------------------------*/
/* end dt_000.c -----------------------------------------------------------*/
