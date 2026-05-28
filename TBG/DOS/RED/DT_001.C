/* dt_001.c ---------------------------------------------------------------*/
/****************************************************************************
               Демонстрационно-тестовая программа
                функций интерфейса с CGM-файлами.

                      Borland C++ 2.0

              (C) Павельев П.И.,1994 (20 сентября)
****************************************************************************/

#include "cgm.h"
/* при отсутствии "cgm_doc.h" закоментировать нижнюю строку  */
#include "cgm_doc.h" /* тестирование прототипов в документации */

#include "dt.h"

#include <alloc.h>
#include <conio.h>
#include <string.h>


/*-------------------------------------------------------------------------*/
static OBJECT *pic_null=NULL;
static OBJECT *pic_bad=(OBJECT *)1000;
/*-------------------------------------------------------------------------*/
void test1(void)/* первая группа тестов */
{
 char *ntf="test1";
 long mem_InitCGM;
 long mem_CloseCGM;
 int i;

 NameTestFunc = ntf;

 {/* Проверки InitCGM-CloseCGM */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки InitCGM-CloseCGM",SHOW_DIR);

 cur_num_type_test = specific_err;/* специфические ошибки */
 /* повторная инициализация */
 test_func("1",InitCGM()<0);
 /* освобождение памяти парой InitCGM-CloseCGM */
 CloseCGM();          /* завершение работы с CGM */
 mem_InitCGM = coreleft();
 InitCGM();           /* инициализация работы с CGM */
 CloseCGM();          /* завершение работы с CGM */
 mem_CloseCGM = coreleft();
 InitCGM();           /* инициализация работы с CGM */
 test_func("2",mem_InitCGM == mem_CloseCGM);
 /* освобождение памяти парой InitCGM-CloseCGM */
 }/* Проверки InitCGM-CloseCGM */

 {/* Генерация CGM-файлов */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Генерация CGM-файлов",SHOW_DIR);

 cur_num_type_test = check_work;/* автоматические проверки */
 for(i=0;i<max_NameTbgFile;i++)
 {
  char s_tbg[80];
  char s_cgm[80];
  OBJECT *p=NULL;
/*  long start_mem;*/

  strcpy(s_tbg,NameTbgFile[i]);/* добавляем расширение */
  strcat(s_tbg,".tbg");        /* добавляем расширение */
  strcpy(s_cgm,NameTbgFile[i]);/* добавляем расширение */
  strcat(s_cgm,".cgm");        /* добавляем расширение */

/*  start_mem = coreleft();/* память до загрузки картинки */*/
  test_func("1",LoadTBGFile(&p,s_tbg)==0);/* загружаем */
  test_func("2",SavePicCGM(p,s_cgm,s_cgm)==0);/* сохраняем */
  test_func("3",DelPic(&p)==0);/* удаляем */
  {
  char s[80];
  strcpy(s,s_cgm);
  strcat(s," 4");
/*  test_func(s,start_mem == coreleft());/* память после удаления */*/
  }
  test_func("5",LoadFile(&p,s_cgm)==0);/* загружаем то,что получили */
  test_func("6",DelPic(&p)==0);/* удаляем */
  {
  strcat(s_cgm," 7");
/*  test_func(s_cgm,start_mem == coreleft());/* память после удаления */*/
  }
 }/* for */
 }/* Генерация CGM-файлов */
/* return ;*/

 {/* Тестирование CGM-библиотек */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Тестирование CGM-библиотек",SHOW_DIR);

 cur_num_type_test = check_work;/* автоматические проверки */
 /* удаляем возможно уже имеемые библиотеки */
 unlink(name_lib1);
 unlink(name_lib2);
 unlink(name_lib3);
 unlink(name_lib4);

 for(i=0;i<max_NameTbgFile;i++)
 {
  char s_cgm[80];
  OBJECT *p=NULL,*p1=NULL,*p2=NULL,*p3=NULL;

  strcpy(s_cgm,NameTbgFile[i]);/* добавляем расширение */
  strcat(s_cgm,".cgm");        /* добавляем расширение */

  test_func("1",LoadFile(&p,s_cgm)==0);/* загружаем */
  test_func("2",AddPicCGM   (p,name_lib1,s_cgm)==0);/* добавляем картинку */

  test_func("3",AddNewPicCGM(p,name_lib2,s_cgm)==0);/* добавляем картинку как новую */

  test_func("4",AddPicCGM   (p,name_lib3,s_cgm)==0);/* добавляем картинку */
  test_func("5",AddPicCGM   (p,name_lib3,s_cgm)==0);/* добавляем картинку */

  test_func("6",AddPicCGM   (p,name_lib4,s_cgm)==0);/* добавляем картинку */
  test_func("7",AddPicCGM   (p,name_lib4,s_cgm)==0);/* добавляем картинку */
  test_func("8",AddPicCGM   (p,name_lib4,s_cgm)==0);/* добавляем картинку */
  /* все помеченные на удаление с заданным именем */
  test_func("9",DelPicCGMFile(name_lib4,s_cgm,DEL_MARK_ALL_PNAME)==0);

  test_func("10",DelPic(&p)==0);/* удаляем */

  test_func("11",LoadFilePic(&p1,name_lib1,s_cgm)==0);/* загружаем */
  test_func("12",LoadFilePic(&p2,name_lib2,s_cgm)==0);/* загружаем */
  test_func("13",LoadFilePic(&p3,name_lib3,s_cgm)==0);/* загружаем */

  if(i == 0)
  {
   test_func("14",strcmp(GetTextObjNum(p1,0,0),"начало")==0);
   test_func("15",strcmp(GetTextObjNum(p2,0,0),"начало")==0);
   test_func("16",strcmp(GetTextObjNum(p3,0,0),"начало")==0);
  }/* if */

  if(i == max_NameTbgFile-1)
  {
   test_func("17",strcmp(GetTextObjNum(p1,255,255),"конец")==0);
   test_func("18",strcmp(GetTextObjNum(p2,255,255),"конец")==0);
   test_func("19",strcmp(GetTextObjNum(p3,255,255),"конец")==0);
  }/* if */

  if((CheckObjNum(p1,127,127))&&
     (CheckObjNum(p2,127,127))&&
     (CheckObjNum(p3,127,127)))
  {
   test_func("20",strcmp(GetTextObjNum(p1,127,127),"середина")==0);
   test_func("21",strcmp(GetTextObjNum(p2,127,127),"середина")==0);
   test_func("22",strcmp(GetTextObjNum(p3,127,127),"середина")==0);
  }/* if */

  test_func("23",DelPic(&p1)==0);/* удаляем */
  test_func("24",DelPic(&p2)==0);/* удаляем */
  test_func("25",DelPic(&p3)==0);/* удаляем */

 }/* for */

 DelPicCGMFile(name_lib3,"",DEL_MARK_ALL);/* все помеченные картинки */
 test_func("26",
           (GetLenghtFile(name_lib1)==GetLenghtFile(name_lib2))&&
           (GetLenghtFile(name_lib2)==GetLenghtFile(name_lib3))&&
           (GetLenghtFile(name_lib3)==GetLenghtFile(name_lib4))
 );/* библиотеки равны */

 /* включение каталога картинок в CGM */
 InsertDirCGMFile(name_lib1);
 InsertDirCGMFile(name_lib2);
 InsertDirCGMFile(name_lib3);
 InsertDirCGMFile(name_lib4);
 test_func("27",
           (GetLenghtFile(name_lib1)==GetLenghtFile(name_lib2))&&
           (GetLenghtFile(name_lib2)==GetLenghtFile(name_lib3))&&
           (GetLenghtFile(name_lib3)==GetLenghtFile(name_lib4))
 );/* библиотеки равны */

 for(i=0;i<max_NameTbgFile;i++)
 {
  char s_cgm[80];
  OBJECT *p2=NULL,*p3=NULL,*p4=NULL;
  strcpy(s_cgm,NameTbgFile[i]);/* добавляем расширение */
  strcat(s_cgm,".cgm");        /* добавляем расширение */

  /* все картинки с заданным именем */
  test_func("28",DelPicCGMFile(name_lib2,s_cgm,DEL_ALL_PNAME)==0);
  test_func("29",DelPicCGMFile(name_lib3,s_cgm,DEL_ALL_PNAME)==0);
  test_func("30",DelPicCGMFile(name_lib4,s_cgm,DEL_ALL_PNAME)==0);

  /* загружаем то чего нет */
  test_func("31",LoadFilePic(&p2,name_lib2,s_cgm));
  test_func("32",LoadFilePic(&p3,name_lib3,s_cgm));
  test_func("33",LoadFilePic(&p4,name_lib4,s_cgm));
 }/* for */

 /* нет файла после удаления всех картинок из него */
 test_func("34",GetLenghtFile(name_lib2)==-1L);
 test_func("35",GetLenghtFile(name_lib3)==-1L);
 test_func("36",GetLenghtFile(name_lib4)==-1L);

 }/* Тестирование CGM-библиотек */

 {/* Проверки SetLibCGM,... */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки SetLibCGM,...",SHOW_DIR);
 cur_num_type_test = specific_err;  /* специфические ошибки */
 /* не существует CGM-библиотека */
 test_func("1",SetLibCGM("dt.ff"));

 cur_num_type_test = check_work;    /* проверка работы */
 test_func("2",DelPic(&dt_err)==0);
 test_func("3",DelPic(&dt_main)==0);
 SetLibCGM(name_lib1);

 unlink("dt_err.cgm"); /* нет отдельных файлов */
 unlink("dt_main.cgm");
 test_func("4",LoadFilePic(&dt_err,name_lib1,"dt_err.cgm")==0);
 test_func("5",LoadFilePic(&dt_main,name_lib1,"dt_main.cgm")==0);

 ShowObj(dt_main,0,SHOW_DIR);/* общее обрамление */
 ShowObj(dt_err, 0,SHOW_DIR);/* для индикации ошибок */
 UnSetLibCGM();
 {
  OBJECT *p=NULL;
 test_func("6",LoadFile(&p,"dt_err.cgm"));
 test_func("7",LoadFile(&p,"dt_main.cgm"));
 }
 SetLibCGM(name_lib1);

 }/* Проверки SetLibCGM,... */

 {/* Проверки LoadFile,LoadFilePic */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки LoadFile,LoadFilePic",SHOW_DIR);

 cur_num_type_test = common_err;    /* общие ошибки */

 /* не выполнена InitCGM */
 CloseCGM();
 test_func("1",LoadFile(&pic_null,"dt.ff"));
 InitCGM();
 /* занята память */
 test_func("2",LoadFile(&dt_main,"dt.ff"));
 /* не найдена картинка */
 test_func("3",LoadFile(&pic_null,"dt.ff"));

 UnSetLibCGM();
 /* файл не открывается */
 test_func("4",LoadFile(&pic_null,"dt.ff"));
 /* файл не в формате CGM */
 test_func("5",LoadFile(&pic_null,"dt.exe"));
 SetLibCGM(name_lib1);

 }/* Проверки LoadFile,LoadFilePic */

 {/* загрузка всех тестовых картинок */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Загрузка всех тестовых картинок...",SHOW_DIR);

 test_func("1",LoadFilePic(&dt1gp,ndt1gp,ndt1gp)==0);
 test_func("2",LoadFile   (&dtagp,ndtagp)==0);
 test_func("3",LoadFilePic(&dt1gp1le,ndt1gp1le,ndt1gp1le)==0);
 test_func("4",LoadFile   (&dtagpbe,ndtagpbe)==0);

 }/* загрузка всех тестовых картинок */

 {/* Проверки DelPic */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки DelPic",SHOW_DIR);
 cur_num_type_test = common_err;/* общие ошибки */

 test_func( "1",DelPic(&pic_null) != 0);/* нет картинки */
 test_func( "2",DelPic(&pic_bad) != 0);/* плохая картинка */

 }/* Проверки DelPic */

 {/* Проверки MovetoPic */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки MovetoPic",SHOW_DIR);

 cur_num_type_test = common_err;/* общие ошибки */

 test_func( "1",MovetoPic(pic_null,0,0)!=0);/* нет картинки */
 test_func( "2",MovetoPic(pic_bad,0,0)!=0);/* плохая картинка */

 }/* Проверки MovetoPic */

 {/* Проверки ShowAll,ShowObj... */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки ShowAll,ShowObj...",SHOW_DIR);

 cur_num_type_test = common_err;/* общие ошибки */

 test_func( "1",ShowAll(pic_null)!=0);/* нет картинки */
 test_func( "2",ShowAll(pic_bad)!=0);/* плохая картинка */

 test_func( "3",ShowObj(pic_null,0,SHOW)!=0);/* нет картинки */
 test_func( "4",ShowObj(pic_bad,0,SHOW)!=0);/* плохая картинка */
 test_func( "5",ShowObj(dt1gp,0,SHOW)!=0);/* не найден ЛЭ */
 test_func( "6",ShowObj(dtagp,0,SHOW)!=0);/* не найден ЛЭ */

 test_func( "7",ShowObjNum(pic_null,0,0,SHOW)!=0);/* нет картинки */
 test_func( "8",ShowObjNum(pic_bad,0,0,SHOW)!=0);/* плохая картинка */
 test_func( "9",ShowObjNum(dt1gp,0,0,SHOW)!=0);/* не найден ЛЭ */
 test_func("10",ShowObjNum(dtagp,0,0,SHOW)!=0);/* не найден ЛЭ */

 test_func("11",ShowObjColor(pic_null,0,15)!=0);/* нет картинки */
 test_func("12",ShowObjColor(pic_bad,0,15)!=0);/* плохая картинка */
 test_func("13",ShowObjColor(dt1gp,0,15)!=0);/* не найден ЛЭ */
 test_func("14",ShowObjColor(dtagp,0,15)!=0);/* не найден ЛЭ */

 test_func("15",ShowObjNumColor(pic_null,0,0,15)!=0);/* нет картинки */
 test_func("16",ShowObjNumColor(pic_bad,0,0,15)!=0);/* плохая картинка */
 test_func("17",ShowObjNumColor(dt1gp,0,0,15)!=0);/* не найден ЛЭ */
 test_func("18",ShowObjNumColor(dtagp,0,0,15)!=0);/* не найден ЛЭ */

 }/* Проверки ShowAll,ShowObj... */

 {/* Проверки SetColor... */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки SetColor...",SHOW_DIR);

 cur_num_type_test = common_err;/* общие ошибки */

 test_func( "1",SetColorObj(pic_null,0,15,SHOW)!=0);/* нет картинки */
 test_func( "2",SetColorObj(pic_bad,0,15,SHOW)!=0);/* плохая картинка */
 test_func( "3",SetColorObj(dt1gp,0,15,SHOW)!=0);/* не найден ЛЭ */
 test_func( "4",SetColorObj(dtagp,0,15,SHOW)!=0);/* не найден ЛЭ */

 test_func( "5",SetColorObjNum(pic_null,0,0,15,SHOW)!=0);/* нет картинки */
 test_func( "6",SetColorObjNum(pic_bad,0,0,15,SHOW)!=0);/* плохая картинка */
 test_func( "7",SetColorObjNum(dt1gp,0,0,15,SHOW)!=0);/* не найден ЛЭ */
 test_func( "8",SetColorObjNum(dtagp,0,0,15,SHOW)!=0);/* не найден ЛЭ */

 }/* Проверки SetColor... */

 {/* Проверки GetColor... */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки GetColor...",SHOW_DIR);
 cur_num_type_test = common_err;/* общие ошибки */

 test_func( "1",GetColorObjNum(pic_null,0,0) < 0);/* нет картинки */
 test_func( "2",GetColorObjNum(pic_bad,0,0) < 0);/* плохая картинка */
 test_func( "3",GetColorObjNum(dt1gp,0,0) < 0);/* не найден ЛЭ */
 test_func( "4",GetColorObjNum(dtagp,0,0) < 0);/* не найден ЛЭ */

 }/* Проверки GetColor... */

 {/* Проверки GetSize... */
 int xmin,ymin,xmax,ymax;
 VP vp;
 WC wc;

 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки GetSize...",SHOW_DIR);
 cur_num_type_test = common_err;/* общие ошибки */

 test_func( "1",GetSizeObjNum(pic_null,0,0,&xmin,&ymin,&xmax,&ymax) < 0);/* нет картинки */
 test_func( "2",GetSizeObjNum(pic_bad,0,0,&xmin,&ymin,&xmax,&ymax) < 0);/* плохая картинка */
 test_func( "3",GetSizeObjNum(dt1gp,0,0,&xmin,&ymin,&xmax,&ymax) < 0);/* не найден ЛЭ */
 test_func( "4",GetSizeObjNum(dtagp,0,0,&xmin,&ymin,&xmax,&ymax) < 0);/* не найден ЛЭ */

 test_func( "5",GetSizeObjNumVP(pic_null,0,0,&vp) < 0);/* нет картинки */
 test_func( "6",GetSizeObjNumVP(pic_bad,0,0,&vp) < 0);/* плохая картинка */
 test_func( "7",GetSizeObjNumVP(dt1gp,0,0,&vp) < 0);/* не найден ЛЭ */
 test_func( "8",GetSizeObjNumVP(dtagp,0,0,&vp) < 0);/* не найден ЛЭ */

 test_func( "9",GetSizeObjNumWC(pic_null,0,0,&wc) < 0);/* нет картинки */
 test_func("10",GetSizeObjNumWC(pic_bad,0,0,&wc) < 0);/* плохая картинка */
 test_func("11",GetSizeObjNumWC(dt1gp,0,0,&wc) < 0);/* не найден ЛЭ */
 test_func("12",GetSizeObjNumWC(dtagp,0,0,&wc) < 0);/* не найден ЛЭ */

 }/* Проверки GetSize... */

 {/* Проверки GetPos... */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки GetPos...",SHOW_DIR);
 cur_num_type_test = common_err;/* общие ошибки */

 test_func( "1",GetPosMouseObj(pic_null,0,0) < 0);/* нет картинки */
 test_func( "2",GetPosMouseObj(pic_bad,0,0) < 0);/* плохая картинка */
 test_func( "3",GetPosMouseObj(dt1gp,0,0) < 0);/* не найден ЛЭ */
 test_func( "4",GetPosMouseObj(dtagp,0,0) < 0);/* не найден ЛЭ */

 test_func( "5",GetPosObj(pic_null,0,0,0) < 0);/* нет картинки */
 test_func( "6",GetPosObj(pic_bad,0,0,0) < 0);/* плохая картинка */
 test_func( "7",GetPosObj(dt1gp,0,0,0) < 0);/* не найден ЛЭ */
 test_func( "8",GetPosObj(dtagp,0,0,0) < 0);/* не найден ЛЭ */

 }/* Проверки GetPos... */

 {/* Проверки GetMin...Max... */
 int max,min;

 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки GetMin...Max...",SHOW_DIR);
 cur_num_type_test = common_err;/* общие ошибки */

 test_func( "1",GetMaxObj(pic_null,0,&max) < 0);/* нет картинки */
 test_func( "2",GetMaxObj(pic_bad,0,&max) < 0);/* плохая картинка */
 test_func( "3",GetMaxObj(dt1gp,0,&max) < 0);/* не найден ЛЭ */
 test_func( "4",GetMaxObj(dtagp,0,&max) < 0);/* не найден ЛЭ */

 test_func( "5",GetMinObj(pic_null,0,&min) < 0);/* нет картинки */
 test_func( "6",GetMinObj(pic_bad,0,&min) < 0);/* плохая картинка */
 test_func( "7",GetMinObj(dt1gp,0,&min) < 0);/* не найден ЛЭ */
 test_func( "8",GetMinObj(dtagp,0,&min) < 0);/* не найден ЛЭ */

 test_func( "9",GetObjMin(pic_null,0) < 0);/* нет картинки */
 test_func("10",GetObjMin(pic_bad,0) < 0);/* плохая картинка */
 test_func("11",GetObjMin(dt1gp,0) < 0);/* не найден ЛЭ */
 test_func("12",GetObjMin(dtagp,0) < 0);/* не найден ЛЭ */

 test_func("13",GetObjMax(pic_null,0) < 0);/* нет картинки */
 test_func("14",GetObjMax(pic_bad,0) < 0);/* плохая картинка */
 test_func("15",GetObjMax(dt1gp,0) < 0);/* не найден ЛЭ */
 test_func("16",GetObjMax(dtagp,0) < 0);/* не найден ЛЭ */

 }/* Проверки GetMin...Max... */

 {/* Проверки OutText... */

 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки OutText...",SHOW_DIR);
 cur_num_type_test = common_err;/* общие ошибки */

 test_func( "1",OutTextObjNum(pic_null,0,0,15,"",SHOW_DIR) < 0);/* нет картинки */
 test_func( "2",OutTextObjNum(pic_bad,0,0,15,"",SHOW_DIR) < 0);/* плохая картинка */
 test_func( "3",OutTextObjNum(dt1gp,0,0,15,"",SHOW_DIR) < 0);/* не найден ЛЭ */
 test_func( "4",OutTextObjNum(dtagp,0,0,15,"",SHOW_DIR) < 0);/* не найден ЛЭ */
 cur_num_type_test = specific_err;  /* специфические ошибки */
 test_func( "5",OutTextObjNum(dt_err,255,1,15,"",SHOW_DIR) < 0);/* это не текст */

 cur_num_type_test = common_err;/* общие ошибки */
 test_func( "6",GetTextObjNum(pic_null,0,0) == NULL);/* нет картинки */
 test_func( "7",GetTextObjNum(pic_bad,0,0) == NULL);/* плохая картинка */
 test_func( "8",GetTextObjNum(dt1gp,0,0) == NULL);/* не найден ЛЭ */
 test_func( "9",GetTextObjNum(dtagp,0,0) == NULL);/* не найден ЛЭ */
 cur_num_type_test = specific_err;  /* специфические ошибки */
 test_func("10",GetTextObjNum(dt_err,255,1) == NULL);/* это не текст */

 cur_num_type_test = common_err;/* общие ошибки */
 test_func("11",StrlenObjNum(pic_null,0,0) < 0);/* нет картинки */
 test_func("12",StrlenObjNum(pic_bad,0,0) < 0);/* плохая картинка */
 test_func("13",StrlenObjNum(dt1gp,0,0) < 0);/* не найден ЛЭ */
 test_func("14",StrlenObjNum(dtagp,0,0) < 0);/* не найден ЛЭ */
 cur_num_type_test = specific_err;  /* специфические ошибки */
 test_func("15",StrlenObjNum(dt_err,255,1) < 0);/* это не текст */

 }/* Проверки OutText... */

 {/* Проверки SetPic...Get... */
 WC wcw;
 VP vp;

 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки SetPic...Get...",SHOW_DIR);
 cur_num_type_test = common_err;/* общие ошибки */

 test_func( "1",SetPicWCW(pic_null,&wcw) < 0);/* нет картинки */
 test_func( "2",SetPicWCW(pic_bad,&wcw) < 0);/* плохая картинка */

 test_func( "4",SetPicVP(pic_null,&vp) < 0);/* нет картинки */
 test_func( "5",SetPicVP(pic_bad,&vp) < 0);/* плохая картинка */

 test_func( "6",GetPicWCW(pic_null,&wcw) < 0);/* нет картинки */
 test_func( "7",GetPicWCW(pic_bad,&wcw) < 0);/* плохая картинка */

 test_func( "8",GetPicVP(pic_null,&vp) < 0);/* нет картинки */
 test_func( "9",GetPicVP(pic_bad,&vp) < 0);/* плохая картинка */

 }/* Проверки SetPic...Get... */

 {/* Проверки Check... */

 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Проверки Check...",SHOW_DIR);
 /* проверки ошибок Check... соответствуют проверке работоспособности */
 cur_num_type_test = check_work;     /* проверка работы */

 test_func( "1",CheckObj(pic_null,0) == 0);/* нет картинки */
 test_func( "2",CheckObj(pic_bad,0) == 0);/* плохая картинка */
 test_func( "3",CheckObj(dt1gp,0) == 0);/* не найден ЛЭ */
 test_func( "4",CheckObj(dtagp,0) == 0);/* не найден ЛЭ */
 test_func( "5",CheckObj(dt_main,0) != 0);/* есть такой ЛЭ */

 test_func( "6",CheckObjNum(pic_null,0,0) == 0);/* нет картинки */
 test_func( "7",CheckObjNum(pic_bad,0,0) == 0);/* плохая картинка */
 test_func( "8",CheckObjNum(dt1gp,0,0) == 0);/* не найден ЛЭ */
 test_func( "9",CheckObjNum(dtagp,0,0) == 0);/* не найден ЛЭ */
 test_func("10",CheckObjNum(dt_main,0,0) != 0);/* есть такой ЛЭ */

 }/* Проверки Check... */


 OutTextObjNum(dt_err,4,0,GetColorObjNum(dt_err,255,1),
 "Выполнено:",SHOW_DIR);

 ShowObjColor(dt_err,1,GetColorObjNum(dt_err,3,0));/* Ok */
 OutTextObjNum(dt_main,1,1,GetColorObjNum(dt_main,1,255),
 "Закончены тесты функций.Нажмите любую клавишу...",SHOW_DIR);
 getch();

}/*  test1 */
/*-------------------------------------------------------------------------*/
/* end dt_001.c -----------------------------------------------------------*/
