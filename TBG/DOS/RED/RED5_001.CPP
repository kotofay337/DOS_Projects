//#define krt
//#define chkp   // p
//#define red
/*==========================*/
/* 40-40 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bios.h>
#include <dos.h>
#include <time.h>
#include <dir.h>
#include <sys\stat.h>
#include <io.h>
#include <conio.h>
#include <alloc.h>
#include <graphics.h>
#include <keyboard.h>

#include "red5.h"

#ifdef krt
 char demomess[]={"ДемоВерсия:Полное сохранение 1-раз !!!"};
#else
 char demomess[]={""};
#endif

#ifdef krt
#else
#ifdef chkp

#define BIOS_SEG 0xffff

/* ----------------------------------------------------------------------- */
int tbk_cs(char *file_name)
{
   long baza;
   FILE *fp;
   char *chptrf;
//   struct ffblk ffblk;
   static unsigned char key[]={"gyhbdrf╨№╪№"};

   int ii,rez_buf;
   char *buf;
   unsigned sz_buf;

/*  chptr=getdta();*/
/*  printf("%Fp\n",chptr);*/

  if((*(key+0)=='g')&&
     (*(key+1)=='y')&&
     (*(key+2)=='h')&&
     (*(key+3)=='b'))
    {/* не было привязки */

/*  printf("Имя файла <%s> \n",file_name);*/
  if((chptrf=searchpath(file_name))==NULL){
    printf("НЕ найден файл <%s> \n",file_name);
    exit(0);
  }// if

  chmod(chptrf,S_IWRITE|S_IREAD);// чтение и запись в файл

/*  printf("Полное имя файла <%s> \n",chptrf);*/

  if((fp=fopen(chptrf,"r+b"))<=0) {exit(0);}
  else
  {
//    rewind(fp);
	sz_buf=32760;

	while((buf=(char *)malloc(sz_buf))==NULL) if((sz_buf-=20) < 20) break;

do{
	baza=ftell(fp);
	rez_buf=fread(buf,1,sz_buf,fp);
	for(ii=0;ii<rez_buf-4;ii++)
    {
	   if((*(buf+ii+0)==*(key+0))&&
	      (*(buf+ii+1)==*(key+1))&&
	      (*(buf+ii+2)==*(key+2))&&
	      (*(buf+ii+3)==*(key+3)))
       {/* нашли фрагмент */

      *(buf+ii+0)=~peekb(BIOS_SEG,5);
      *(buf+ii+1)=~peekb(BIOS_SEG,6);
      *(buf+ii+2)=~peekb(BIOS_SEG,8);
      *(buf+ii+3)=~peekb(BIOS_SEG,9);
      *(buf+ii+4)=~peekb(BIOS_SEG,11);
      *(buf+ii+5)=~peekb(BIOS_SEG,12);

      fseek(fp,baza,SEEK_SET);
      fwrite(buf,1,rez_buf,fp);
	  printf("Программа УСТАНОВЛЕНА.\r");
	  goto end;
     }// нашли фрагмент
	}// for
	baza=ftell(fp);
	if(rez_buf==10) rez_buf=0;
	fseek(fp,baza-10,SEEK_SET);

 }while(rez_buf!=0);
  printf("\007 Установки НЕТ !!!\n");
//  printf("\007");
  return !Ok;

 end:;
 // имитация установленности загруженной копии
 *(key+0)=~peekb(BIOS_SEG,5);
 *(key+1)=~peekb(BIOS_SEG,6);
 *(key+2)=~peekb(BIOS_SEG,8);
 *(key+3)=~peekb(BIOS_SEG,9);
 *(key+4)=~peekb(BIOS_SEG,11);
 *(key+5)=~peekb(BIOS_SEG,12);

 free(buf);

 fclose(fp);
// chmod(chptrf,S_IREAD);// только чтение
 return Ok;
 }// if открыли файл

 } // if не было привязки
 else
 {// была привязка

if( (peekb(BIOS_SEG, 5)==(char)(~*(key+0)&0x00ff))&&
    (peekb(BIOS_SEG, 6)==(char)(~*(key+1)&0x00ff))&&
    (peekb(BIOS_SEG, 8)==(char)(~*(key+2)&0x00ff))&&
    (peekb(BIOS_SEG, 9)==(char)(~*(key+3)&0x00ff))&&
    (peekb(BIOS_SEG,11)==(char)(~*(key+4)&0x00ff))&&
    (peekb(BIOS_SEG,12)==(char)(~*(key+5)&0x00ff)))
    {} /* Ok! Hа месте */
   else{
     printf("\rФайл <%s> не на месте.\n\007",chptrf);
     return !Ok;
   }// if else
  }// if была привязка
  return Ok;
} // tbk_cs

#endif /* chkp */
#endif // krt

/* ======================================================================= */
/* Для механиков прибабахи */
/* ----------------------------------------------------------------------- */

/* --------------- Изготовление труб -------------- */
static int  STEP  = 8; /* размер шага трубы */
static int  pnOBJ = -1; /* номер объекта */
static int  pnNUM = -1; /* номер элемента объекта */

static char step[1]; /* размер шага трубы */

enum{GROUP,INCREMENT,DELETE_SrcGE};
static char PIPE[3]={1,1,1}; /*  */

static void init_pn_pipe(OBJECT **pic)
{/* функция выполняющаяся при инициализации панели */
   char buf[10];

   sprintf(buf,"%u",STEP);
   outtext_obj_num(pic,18,1,getcolor_obj_num(pic,18,2),buf,SHOW);

   sprintf(buf,"%d",pnOBJ);
   outtext_obj_num(pic,28,1,getcolor_obj_num(pic,28,2),buf,SHOW);

   sprintf(buf,"%d",pnNUM);
   outtext_obj_num(pic,38,1,getcolor_obj_num(pic,38,2),buf,SHOW);

}/* init_pn_pipe */
/* ----------------------------------------------------------------------- */
static void funstep(OBJECT **pic)
{/* функция выполняющаяся при выборе позиции */
   char buf[10];

   mouse_off();

   sprintf(buf,"%u",STEP);
   STEP = atoi(EditString_obj_num(pic,18,1,buf,sizeof(buf)));
   outpic_obj_num(pic,18,2);
   sprintf(buf,"%u",STEP);
   outtext_obj_num(pic,18,1,getcolor_obj_num(pic,18,2),buf,SHOW);

   mouse_on();
}/* funstep */
/* ----------------------------------------------------------------------- */
static void funpnobj(OBJECT **pic)
{/* функция выполняющаяся при выборе позиции */
   char buf[10];

   mouse_off();

   sprintf(buf,"%d",pnOBJ);
   pnOBJ = atoi(EditString_obj_num(pic,28,1,buf,sizeof(buf)));
   outpic_obj_num(pic,28,2);
   sprintf(buf,"%d",pnOBJ);
   outtext_obj_num(pic,28,1,getcolor_obj_num(pic,28,2),buf,SHOW);

   mouse_on();
}/* funpnobj */
/* ----------------------------------------------------------------------- */
static void funpnnum(OBJECT **pic)
{/* функция выполняющаяся при выборе позиции */
   char buf[10];

   mouse_off();

   sprintf(buf,"%d",pnNUM);
   pnNUM = atoi(EditString_obj_num(pic,38,1,buf,sizeof(buf)));
   outpic_obj_num(pic,38,2);
   sprintf(buf,"%d",pnNUM);
   outtext_obj_num(pic,38,1,getcolor_obj_num(pic,38,2),buf,SHOW);

   mouse_on();
}/* funpnnum */
/* ----------------------------------------------------------------------- */
void add_NO (int cNO,int add_NO);
/* ----------------------------------------------------------------------- */
#pragma argsused
static void funadd(OBJECT **pic)
{/* функция выполняющаяся при выборе позиции */
 add_NO(pnOBJ,pnNUM); /* добавить те номера, которые там... */
}/* funadd */
/* ----------------------------------------------------------------------- */

FIELD m_pipe=
      { f_poli,                       // тип поля
        sizeof(PIPE)/sizeof(PIPE[0]), // размер данных
        1,                            // номер активной строки
        PIPE,                         // начало данных
        1,                            // номер обьекта в файле TBG
        SHOW_FLD,   // признак наличия индикации выбора позиции
        SHOW_FLD,   // признак наличия индикации указания позиции
        NULL        // функция выполняющаяся при выборе позиции
      };// описание поля

FIELD m_step=
      { f_poli,                       // тип поля
        sizeof(step)/sizeof(step[0]), // размер данных
        0,                            // номер активной строки
        step,                         // начало данных
        5,                            // номер обьекта в файле TBG
        SHOW_FLD,   // признак наличия индикации выбора позиции
        SHOW_FLD,   // признак наличия индикации указания позиции
        (void(*)(OBJECT**,...))funstep     // функция выполняющаяся при выборе позиции
      };// описание поля

FIELD m_pnobj=
      { f_poli,                       // тип поля
        sizeof(step)/sizeof(step[0]), // размер данных
        0,                            // номер активной строки
        step,                         // начало данных
        10,                           // номер обьекта в файле TBG
        SHOW_FLD,   // признак наличия индикации выбора позиции
        SHOW_FLD,   // признак наличия индикации указания позиции
        (void(*)(OBJECT**,...))funpnobj    // функция выполняющаяся при выборе позиции
      };// описание поля

FIELD m_pnnum=
      { f_poli,                       // тип поля
        sizeof(step)/sizeof(step[0]), // размер данных
        0,                            // номер активной строки
        step,                         // начало данных
        15,                           // номер обьекта в файле TBG
        SHOW_FLD,   // признак наличия индикации выбора позиции
        SHOW_FLD,   // признак наличия индикации указания позиции
        (void(*)(OBJECT**,...))funpnnum    // функция выполняющаяся при выборе позиции
      };// описание поля

FIELD m_pnadd=
      { f_poli,                       // тип поля
        sizeof(step)/sizeof(step[0]), // размер данных
        0,                            // номер активной строки
        step,                         // начало данных
        20,                           // номер обьекта в файле TBG
        SHOW_FLD,   // признак наличия индикации выбора позиции
        SHOW_FLD,   // признак наличия индикации указания позиции
        (void(*)(OBJECT**,...))funadd    // функция выполняющаяся при выборе позиции
      };// описание поля

FIELD *m_ipipe[]=
{&m_pipe,&m_step,&m_pnobj,&m_pnnum,&m_pnadd}; // описание полей панели

PANEL pn_pipe=
      { "pipe.tbg",          // имя файла TBG
        "tbg_col1.tbg",      // имя файла цветов TBG
        sizeof(m_ipipe)/sizeof(*m_ipipe),// число полей
        1,                       // номер активного поля
        m_ipipe,                 // начало описания полей
        init_pn_pipe
      };// панель Изготовление труб

/* ----------------------------------------------------------------------- */
void add_NO (int cNO,int add_NO)
{
   if(pic.GetPN() != -1){/* есть картинка */
      if(cNO == add_NO){/* добавление такого-же LE */
         Warning(" Нельзя добавить такой-же объект! ");
         return;
      }/* if */
      pic.StartPN(); /* в начало картинки */
      for(int i = 0;i <= pic.GetPN_TOP();i++){/* по всем GE */
         if(
            (ente == 0)|| /* рамки нет работаем по всем GE */
            ((ente == 2)&&(pic.GetSelect() == ON)) /* рамка есть и GE попал в нее */
           ){/* работаем с GE */
            delay(10);
            risp(pic.GetX(),pic.GetY()); /* рисуем стрелку-указатель */
            if(pic.GetMaxLE() >= 0){/* есть LE */
               pic.End_LLE(); /* в начало списка LE */
               for(;;){/* по всем LE */
                  int LN;
                  if(cNO == pic.Get_NO_CGE()){/* если есть такой объект */
                     LN = pic.Get_LN_CGE(); /* выбрать его LN */
                     if(pic.Check_NO_LN(add_NO,LN,NOSET) == NOT){/* нет добавляемого LE */
                        pic.AddLE(add_NO,LN); /* добавить */
                        pic.Check_NO_LN(cNO,LN,SET); /* установить на последний проверенный LN */
                      /* нет добавляемого LE */
                     }/* if */
                   /* если есть такой объект */
                  }/* if */
                  if(pic.GetCurLE() == -1) /* конец списка LN ? */
                     break;
                  pic.Pred(); /* следующий! */
                /* по всем LE */
               }/* for */
             /* есть LE */
            }/* if */
          /* работаем с GE */
         }/* if */
         pic.IncPN(); /* следующий GE! */
       /* по всем GE */
      }/* for */
    /* есть картинка */
   }/* if */
   else{
      Warning(" Картинка еще не нарисована. ");
   }/* elif */
}/* add_NO */
/* ----------------------------------------------------------------------- */
#define CalcXY {  t->x = x;\
                  t->y = y;\
                  t++;\
                  dp++;\
               }
/* ----------------------------------------------------------------------- */
/*
POINT *LINE2PIPE (POINT *l,int *count){
   int x,y,dx,dy,s1,s2,tmp,e,swap,i,p = 0;
   int x1,y1,x2,y2,dp;
   POINT *pipe,*t;
   x1 = l->x;
   y1 = l->y;
   x2 = (l+1)->x;
   y2 = (l+1)->y;
   x = x1;
   y = y1;
   dx = abs(x2 - x1);
   dy = abs(y2 - y1);
   s1 = (x2 - x1) <  0 ? -1 :
        (x2 - x1) == 0 ?  0 : 1 ;
   s2 = (y2 - y1) <  0 ? -1 :
        (y2 - y1) == 0 ?  0 : 1 ;
   if(dy > dx){
      tmp = dx;
      dx = dy;
      dy = tmp;
      swap = 1;
   }/* if */
   else
      swap = 0;
   e = 2 * dy - dx;
   dp = p = 0;
   dp++;
   for(i = 1;i <= dx;i++){
      if(p == STEP){
         dp++;
         p = 0;
      }/* if */
      else
         p++;
      while(e >= 0){
         if(swap == 1)
            x += s1;
         else
            y += s2;
         e = e - 2 * dx;
      }/* while */
      if(swap == 1)
         y += s2;
      else
         x += s1;
      e = e + 2 * dy;
   }/* for */
   dp++;
   t = pipe = (POINT*)calloc(dp,sizeof(POINT));
   if(t == NULL){
      Warning("Нехватка памяти для изготовления трубы.");
      return NULL;
   }/* if */
   x = x1;
   y = y1;
   e = 2 * dy - dx;
   dp = p = 0;
   CalcXY;
   for(i = 1;i <= dx;i++){
      if(p == STEP){
         CalcXY;
         p = 0;
      }/* if */
      else
         p++;
      while(e >= 0){
         if(swap == 1)
            x += s1;
         else
            y += s2;
         e = e - 2 * dx;
      }/* while */
      if(swap == 1)
         y += s2;
      else
         x += s1;
      e = e + 2 * dy;
   }/* for */
   CalcXY;
   *count = dp;
   return pipe;
}/* LINE2PIPE */
*/
/* - end --------- Изготовление труб -------------- */
/* ----------------------------------------------------------------------- */
/* - start ------- Макропримитивы ---------------------------------------- */
/* номера объектов */
#define TIndexObj 20
#define TStateObj 21
#define TUNameObj 30
#define ScreenObj 88
/* номера элем. объектов панели macro_m1.tbg */
#define posUp      0
#define posDown    1
static unsigned int Index = 1;
static unsigned int State = 4;
static char const * const no = "Нет макропримитива";
static char *UName = (char*)no;
static MPClass *tmpc = NULL;
static char chop[1];
/* ----------------------------------------------------------------------- */
/* in RED?_008.CPP */
int outpic_Display(OBJECT **pic);
/* ----------------------------------------------------------------------- */
#define GetUName(tpmpc) (tpmpc->uname)
#define GetPic(tpmpc) (tpmpc->pic)
/* ----------------------------------------------------------------------- */
int SearchState(MPClass *t, int State){
   SHead *tpS;
   if(t == NULL){
      return -1;
   }/* if */
   tpS = t->pShead;
   while(tpS){
      if(tpS->nState == State){
         return 1;
      }/* if */
      tpS = tpS->SHnext;
   }/* while */
   return 0;
}/* SearchState */
/* ----------------------------------------------------------------------- */
int LoadAndViewMacro(OBJECT **pic, unsigned int index){
   int ret;

   hide_tbl();
   if(index)
      tmpc = MPCLoad(index);
   else
      tmpc = NULL;
   restore_tbl();
   if(tmpc){
      UName = GetUName(tmpc);
      outpic_obj_num(pic, ScreenObj, 1);
      setviewport_obj_num(pic, ScreenObj, 0);
      offset_pic(GetPic(tmpc), 0, 0);
      outpic_Display(GetPic(tmpc));
      setviewport_obj_num(NULL, 0, 0);
      ret = Ok;
	}/* if */
   else{
      UName = (char*)no;
      outpic_obj_num(pic, ScreenObj, 1);
      ret = Fu;
   }/* elif */

   outtext_obj_num(pic, TUNameObj, 1, getcolor_obj_num(pic, TUNameObj, 2), UName, SHOW);

   return ret;
}/* LoadAndViewMacro */
/* ----------------------------------------------------------------------- */
static void init_pn_macro(OBJECT **pic)
{/* функция выполняющаяся при инициализации панели */
   char buf[6];

   Index = pck.mc_index;
   State = pck.mc_state;

   sprintf(buf,"%u",Index);
   outtext_obj_num(pic,TIndexObj,1,getcolor_obj_num(pic,TIndexObj,2),buf,SHOW);

   sprintf(buf,"%u",State);
   outtext_obj_num(pic,TStateObj,1,getcolor_obj_num(pic,TStateObj,2),buf,SHOW);

   MPCFree(tmpc);
   tmpc = NULL;

   LoadAndViewMacro(pic, Index);

}/* init_pn_macro */
/* ----------------------------------------------------------------------- */
static void funIndex(OBJECT **pic)
{/* функция выполняющаяся при выборе позиции */
   char buf[6];

   mouse_off();

   MPCFree(tmpc);
   tmpc = NULL;

   sprintf(buf,"%u",Index);
   Index = atoi(EditString_obj_num(pic,TIndexObj,1,buf,sizeof(buf)));
   outpic_obj_num(pic,TIndexObj,2);

   if(LoadAndViewMacro(pic, Index) == Ok){
	   pck.mc_index = Index;
   }/* if */

   sprintf(buf,"%u",Index);
   outtext_obj_num(pic,TIndexObj,1,getcolor_obj_num(pic,TIndexObj,2),buf,SHOW);

   mouse_on();
}/* funIndex */
/* ----------------------------------------------------------------------- */
static void funState(OBJECT **pic)
{/* функция выполняющаяся при выборе позиции */
   char buf[6];

   mouse_off();

   next:;
   switch(SearchState(tmpc, State)){
      case 1:
         break;
      case 0:
         State++;
         goto next;
      case -1:
         State = 0;
         break;
   }/* switch */

   sprintf(buf,"%u",State);
   State = atoi(EditString_obj_num(pic,TStateObj,1,buf,sizeof(buf)));

   next1:;
   switch(SearchState(tmpc, State)){
      case 1:
         break;
      case 0:
         State++;
         goto next1;
      case -1:
         State = 0;
         break;
   }/* switch */

   pck.mc_state = State;

   sprintf(buf,"%u",State);
   outpic_obj_num(pic,TStateObj,2);
   outtext_obj_num(pic,TStateObj,1,getcolor_obj_num(pic,TStateObj,2),buf,SHOW);

   mouse_on();
}/* funIndex */
/* ----------------------------------------------------------------------- */
static void fun_UpDown (OBJECT **pic, int pos){
   char buf[6];
   mouse_off();

   Index = atoi(gettext_obj_num(pic, TIndexObj, 1));

//   sprintf(buf,"%u",Index);
//   outtext_obj_num(pic,TIndexObj,1,getcolor_obj_num(pic,TIndexObj,2),buf,SHOW);

   if(pos == posUp){
      Index++;
   }/* if */
   if(pos == posDown){
      Index--;
   }/* if */

   MPCFree(tmpc);
   tmpc = NULL;

   sprintf(buf,"%u",Index);
   outtext_obj_num(pic,TIndexObj,1,getcolor_obj_num(pic,TIndexObj,2),buf,SHOW);

   if(LoadAndViewMacro(pic, Index) == Ok){
	   pck.mc_index = Index;
   }/* if */

   mouse_on();
}/* fun_UpDown */
/* ----------------------------------------------------------------------- */
static void fun_UpDownState (OBJECT **pic, int pos){
   char buf[6];
   mouse_off();

   State = atoi(gettext_obj_num(pic, TStateObj, 1));

   next:;
   if(pos == posUp){
      State++;
   }/* if */
   if(pos == posDown){
      State--;
   }/* if */

   switch(SearchState(tmpc, State)){
      case 1:
         break;
      case 0:
         goto next;
      case -1:
         State = 0;
         break;
   }/* switch */

   sprintf(buf,"%u",State);
   outtext_obj_num(pic,TStateObj,1,getcolor_obj_num(pic,TStateObj,2),buf,SHOW);

   pck.mc_state = State;

   mouse_on();
}/* fun_UpDownState */
/* ----------------------------------------------------------------------- */
static void fun_UText(OBJECT **pic)
{/* функция выполняющаяся при выборе позиции */
//   char *ps = NULL;
   mouse_off();
//   ps = EditString_obj_num(pic, TUNameObj, 1, UName, 255);
//   strcpy(UName, ps);
//   outpic_obj_num(pic, TUNameObj, 2);
   outtext_obj_num(pic, TUNameObj, 1, getcolor_obj_num(pic, TUNameObj, 2), UName, SHOW);
   mouse_on();
}/* funUText */
/* ----------------------------------------------------------------------- */
FIELD m_Index=
      { f_poli,                       // тип поля
        1, // размер данных
        0,                            // номер активной строки
        chop,                         // начало данных
        1,                           // номер обьекта в файле TBG
        SHOW_FLD,   // признак наличия индикации выбора позиции
        SHOW_FLD,   // признак наличия индикации указания позиции
        (void(*)(OBJECT**,...))funIndex  // функция выполняющаяся при выборе позиции
      };// описание поля
/* ----------------------------------------------------------------------- */
FIELD m_State=
      { f_poli,                       // тип поля
        1, // размер данных
        0,                            // номер активной строки
        chop,                         // начало данных
        15,                           // номер обьекта в файле TBG
        SHOW_FLD,   // признак наличия индикации выбора позиции
        SHOW_FLD,   // признак наличия индикации указания позиции
        (void(*)(OBJECT**,...))funState  // функция выполняющаяся при выборе позиции
      };// описание поля
/* ----------------------------------------------------------------------- */
FIELD m_UpDown=
      { f_poli,                       // тип поля
        1, // размер данных
        0,                            // номер активной строки
        chop,                         // начало данных
        5,                           // номер обьекта в файле TBG
        SHOW_FLD,   // признак наличия индикации выбора позиции
        SHOW_FLD,   // признак наличия индикации указания позиции
        (void(*)(OBJECT**,...))fun_UpDown  // функция выполняющаяся при выборе позиции
      };// описание поля
/*-------------------------------------------------------------------------*/
FIELD m_UpDownState=
      { f_poli,                       // тип поля
        1, // размер данных
        0,                            // номер активной строки
        chop,                         // начало данных
        25,                           // номер обьекта в файле TBG
        SHOW_FLD,   // признак наличия индикации выбора позиции
        SHOW_FLD,   // признак наличия индикации указания позиции
        (void(*)(OBJECT**,...))fun_UpDownState  // функция выполняющаяся при выборе позиции
      };// описание поля
/*-------------------------------------------------------------------------*/
FIELD m_UText=
      { f_poli,                       // тип поля
        1, // размер данных
        0,                            // номер активной строки
        chop,                         // начало данных
        10,                           // номер обьекта в файле TBG
        SHOW_FLD,   // признак наличия индикации выбора позиции
        SHOW_FLD,   // признак наличия индикации указания позиции
        (void(*)(OBJECT**,...))fun_UText    // функция выполняющаяся при выборе позиции
      };// описание поля
/*-------------------------------------------------------------------------*/
FIELD *m_iIndex[]=
{&m_Index, &m_UpDown, &m_UText, &m_State, &m_UpDownState}; // описание полей панели

PANEL pn_macro={
        "macro_m1.tbg",              // имя файла TBG
        "tbg_col1.tbg",          // имя файла цветов TBG
        sizeof(m_iIndex)/sizeof(*m_iIndex),// число полей
        1,                       // номер активного поля
        m_iIndex,                // начало описания полей
        init_pn_macro
      };
/* - end --------- Макропримитивы ---------------------------------------- */
/* ----------------------------------------------------------------------- */
int painter()
{
  short int xmin, ymin, xmax, ymax;//запросы координат
//  int xmin_m, ymin_m, xmax_m, ymax_m;//размер области 'под меню'
  char sm=OFF;//признак необходимости установки указателя в начало текущего ГЭ
/*  int x,y;//координаты репера */
 char mode_CtrlK=OFF;
 char mode_CtrlQ=OFF;
 int  num_rep;
 int  IndexMacro=0;
/* char mode_CtrlM=OFF;*/
 char PlayMacro=OFF;
 clock_t t;
/* char StartMacro=OFF,PlayMacro=OFF;*/


//  char checklab=-36;
//  char checkadr=-102;

//  char sum;


  /* получение символа указателя обьектов */
  setactivepage(1);
  settextstyle(0,0,1);settextjustify(0,0);setcolor(14);
  outtextxy(10,10,"\037");
  getimage(10,10-7,10+7,10,oper);
  setactivepage(0);
/*  printf("%o \n",43);getch();*/

/* место меню запоминаем */
getsize_obj_num_reset();
getsize_obj_num(menu_1,255,5,&xmin_m,&ymin_m,&xmax_m,&ymax_m);
menu=
(char *)calloc(imagesize(xmin_m,ymin_m,xmax_m,ymax_m),sizeof(char));/* выделяем память */
if(menu==NULL)
  {printf("Нет памяти для меню.'painter'");getch();return 0;};


getsize_obj_num_reset();
getsize_obj_num(menu_1,11,9,&xmin,&ymin,&xmax,&ymax);
mouse_setxy(xmax-4,ymax-4);/* мышь на загрузку */

refr_t();// перевывод всего изображения

do /*вечный цикл*/
{
/*
  for(i=0,sum=0;*(lab+i)!=0;i++) sum^=lab[i];
  if(sum != 56) return NORESTART;
*/
/*
  for(i=0,sum=0;*(adres+i)!=0;i++) sum^=adres[i];
//  printf("adrsum=%d\n",sum);
  if(sum!=checkadr) return NORESTART;
*/
#ifdef krt
#else
#ifdef chkp
 #ifdef red
//  if(tbk_cs("red.exe")== !Ok) return NORESTART;
 #else
//  if(tbk_cs("tbg.exe")== !Ok) return NORESTART;
 #endif
#endif/* chkp */
#endif // krt


 set_attrib();

 if(mn1_show==ON) oumenu();/* вывод меню,если надо */

 if(sm==ON) {sm=OFF;mouse_setxy(pic.GetX(),pic.GetY());};// if

  risp(pic.GetX(),pic.GetY());// рисуем стрелку указателя в начале ГЭ

 if(mouse_yes > 0) mouse_on();/* включение мыши,если мышь есть */
 setlinestyle(DOTTED_LINE,0,NORM_WIDTH);
 setcolor(LIGHTBLUE);


 clock_t t = clock();

 while(bioskey(1)==0)
 {

		 /* управление мышью */
	     if(mouse_yes > 0) {
           if(obrab_mouse() < 0)
              goto ctrl_alt;
        };
	     set_attrib();
	 if(mn1_show==ON) {// меню есть
	    // индикация необходимости сохранения результатов работы
	    if(pic.GetPN() < 0) SaveTest = OFF;// нет ГЭ

	    char curcol;
	    if(SaveTest == ON)  curcol=LIGHTRED;
	    if(SaveTest == OFF) curcol=DARKGRAY;
	    if(curcol != getcolor_obj_num(menu_1,11,2))
	      setcolor_obj_num(menu_1,11,2,curcol,SHOW);
	 }// if

  if(PlayMacro)
  { skankod=pck.KodMacro[IndexMacro++];
    if(skankod==0xffff)
    {
      PlayMacro=0;  /* end Macro */
      IndexMacro--;
    }/* thif */
    else           goto ctrl_alt;
  }/* if */

 // оживление труб
 {/* lockblock */
    if((clock() - t) > (CLK_TCK / 20)){
       pic.ShowPipe();
       t = clock();
    }/* if */
 }/* lockblock */

 mouse_draw();

 ZoomDisplay();

 }// while ждем нажатия

 mouse_getxy(&x_mouse,&y_mouse);/* берем координаты мыши */
 skankod=bioskey(0);

// printf("scan:%x  ss:%x  \r",skankod,peek(0,0x417));

/* printf("%d %d %d  %d   \r",mode_CtrlK,mode_CtrlQ,num_rep,skankod&255);*/

/* printf("%d   %d %d  \r",skankod&255,x_mouse,y_mouse);*/

ctrl_alt:;/* отображение для мыши */
/*
{/* bloc *//* Macro */
 int kod;

 kod=skankod&255;
 if(kod==13) mode_CtrlM=ON;
 if(mode_CtrlM) /* набоp макpо */
 {
  switch(kod)
  {
   case 27:/* Esc */
	mode_CtrlM=StartMacro=PlayMacro=0;
   break;
   case 'S':
   case 's':
	StartMacro=ON;IndexMacro=0;
   break;
   case 'P':
   case 'p':
	PlayMacro=ON; mode_CtrlM=StartMacro=IndexMacro=0;
/*	continue;/* не выполняем */*/
   break;
   case 'E':
   case 'e':
	mode_CtrlM=StartMacro=0;
	pck.KodMacro[IndexMacro]=0xffff;/* end Macro */
   break;
   default:
       if(StartMacro)
       {
	if((IndexMacro+1) < pck.MaxSizeKodMacro)
	{ pck.KodMacro[IndexMacro++]=skankod;
/*	  continue;/* не выполняем */*/
	}
	else
	{
	  pck.KodMacro[IndexMacro]=0xffff;/* end Macro */
	  mode_CtrlM=StartMacro=0;
	}/* elif */
       }/* if */
   break;
  }/* switch */
  continue;
 }/* if набоp макpо */
}/* bloc *//* Macro */
*/

{/* bloc *//* точки на экpане */
 int kod;
 num_rep=(skankod&255)-48;
 if((num_rep>=0)&&(num_rep<=9))
   {
    if(mode_CtrlK)
    {
     pck.reper[num_rep].x=x_mouse;// координаты репера
     pck.reper[num_rep].y=y_mouse;// координаты репера
     mode_CtrlK=OFF;
     continue;
    }/* if */
    if(mode_CtrlQ)
    {
     // координаты репера для 'мышки'
     mouse_setxy(pck.reper[num_rep].x,pck.reper[num_rep].y);
     mode_CtrlQ=OFF;
     continue;
    }/* if */
   }/* thif */
   else
   {mode_CtrlK=mode_CtrlQ=OFF;}/* elif */

 kod=skankod&255;
 if(kod==11) mode_CtrlK=ON;
 if(kod==17) mode_CtrlQ=ON;
}/* bloc *//* точки на экpане */




// printf("%d %d    \r",x_mouse,y_mouse);




  if(mouse_yes > 0) mouse_off();/* отключение мыши */

switch(skankod&255)
{/* эмуляция 'мыша' */
/*
  case 45:/* - левая */
       lpress_lb=1;
       cpress_lb=1;
  break;
*/
  case 43:/* + пpавая */
       lpress_rb=1;
       cpress_rb=1;
  break;
}/* switch */

// printf("scan:%x \r",skankod);

switch(skankod)
{
  case _DEL_:
	pic.DelGE();// удаление текущего ГЭ
	skankod= _F10_;// перевывод
  break;

  case _ESC_:
	if(pos_ms==41) {pic.Set_NO_CGE(0);break;}
	if(pos_ms==42) {pic.Set_LN_CGE(0);break;}
  break;

#define Rpr_OBJ    0
#define Rpr_St_NUM 1
#define Rpr_En_NUM 2

  case Alt_1: /* 1й репер на макропримитиве - начало */
     if(pic.GetTip() == TIP_esc){
         MPClass *tmpc = pic.GetMPClass();
         int x, y;
         pic.ShowCur();
         getsize_obj_num_reset();
         getsize_obj_num(tmpc->pic, Rpr_OBJ, Rpr_St_NUM, (short int*)&x, (short int*)&y, (short int*)&x, (short int*)&y);
         mouse_setxy(x, y);
     }/* if */
     break;

  case Alt_2: /* 2й репер на макропримитиве - конец */
     if(pic.GetTip() == TIP_esc){
         MPClass *tmpc = pic.GetMPClass();
         int x, y;
         pic.ShowCur();
         getsize_obj_num_reset();
         getsize_obj_num(tmpc->pic, Rpr_OBJ, Rpr_En_NUM, (short int*)&x, (short int*)&y, (short int*)&x, (short int*)&y);
         mouse_setxy(x, y);
     }/* if */
     break;

  case Alt_9: /*  */
     if(pic.GetTip() == TIP_txt){
        int sx, sy, ex, ey, tx, ty;
        int tobj, tnum;

        tobj = pic.Get_NO_CGE();
        tnum = pic.Get_LN_CGE();
        ty = TextHeightCol(pic.GetText());
        tx = TextWidthCol(pic.GetText());
        sx = pic.GetX() - 4;
        sy = pic.GetY() + 4;
        ex = pic.GetX() + tx + 4;
        ey = pic.GetY() - ty - 3;

        pic.CreateGE(TIP_bar); // фоновый прямоугольник
        pic.SetColor(EGA_DARKGRAY);
        pic.SetX(mmx(sx));
        pic.SetY(mmy(sy));
        pic.SetX2(mmx(ex - sx));
        pic.SetY2(mmy(ey - sy));
        pic.AddLE(tobj, tnum + 1);
        pic.AddLE(99, tobj);

        pic.CreateGE(TIP_line); // линия L
        pic.SetColor(EGA_LIGHTRED);
        pic.SetX(mmx(sx));
        pic.SetY(mmy(sy - 1));
        pic.SetX2(mmx(0));
        pic.SetY2(mmy(ey - sy + 2));
        pic.AddLE(tobj, tnum + 2);

        pic.CreateGE(TIP_line); // линия R
        pic.SetColor(EGA_LIGHTRED);
        pic.SetX(mmx(ex));
        pic.SetY(mmy(sy - 1));
        pic.SetX2(mmx(0));
        pic.SetY2(mmy(ey - sy + 2));
        pic.AddLE(tobj, tnum + 3);

        pic.CreateGE(TIP_line); // линия тени 1 гор.
        pic.SetColor(EGA_WHITE);
        pic.SetX(mmx(sx + 1));
        pic.SetY(mmy(sy + 1));
        pic.SetX2(mmx(ex - sx));
        pic.SetY2(mmy(0));

        pic.CreateGE(TIP_line); // линия тени 2 верт.
        pic.SetColor(EGA_WHITE);
        pic.SetX(mmx(ex + 1));
        pic.SetY(mmy(sy + 1));
        pic.SetX2(mmx(0));
        pic.SetY2(mmy(ey - sy));

        setcolor(EGA_LIGHTRED);
        rectangle(sx, sy, ex, ey);

        refr_t();

     }/* if */
     break;

  case _ctrl_ENTER_: /* создание реперной точки */
     if(pic.CreateGE(TIP_reper)) /* нет памяти для создания GE */
        break;
     pic.SetX(mmx(x_mouse));
     pic.SetY(mmy(y_mouse));
     break;
  case _ENTER_:
	if(pos_ms==41)
	  {
	   if(pic.GetMaxLE()<0) pic.AddLE(0,0);
	   mouse_off();
	   char buf_no[4];
	   sprintf(buf_no,"%u",pic.Get_NO_CGE());
	   pic.Set_NO_CGE(atoi(EditString_obj_num(menu_1,18,1,buf_no,sizeof(buf_no))));
	   outpic_obj_num(menu_1,18,2);
	   mouse_on();
	   break;
	  } // if

	if(pos_ms==42)
	  {
	   if(pic.GetMaxLE()<0) pic.AddLE(0,0);
	   mouse_off();
	   char buf_ln[4];
	   sprintf(buf_ln,"%u",pic.Get_LN_CGE());
	   pic.Set_LN_CGE(atoi(EditString_obj_num(menu_1,19,1,buf_ln,sizeof(buf_ln))));
	   outpic_obj_num(menu_1,19,2);
	   mouse_on();
	   break;
	  } // if
	 /* эмуляция 'мыша' */
	 lpress_lb=1;
	 cpress_lb=1;
  break;

  case _HOME_:
       sm=ON;//нужно установить мышь в начало текущего ГЭ
  break;

  case _alt_x_:
	if(SaveTest == ON){// есть изменения
	 ibell();
	 mouse_on();
	 if(panel_mode(&pn_me) == Fu) break;
	 switch(pck.Mode_exit)
	 {
	  case mode_exit_se:
	       outpic_obj_num(menu_mess,255,1);//стираем предыдущее пояснение
	       outpic_obj_num(menu_mess,mode_save_all_tbg,30);//новое
	       pic.SaveAll(pck.Fn_work_tbg,mode_save_all_tbg);
	       return NORESTART;/* выход в ДОС */
	  case mode_exit_nse:
	       pck.Mode_exit=mode_exit_se;
	       return NORESTART;/* выход в ДОС */
	  case mode_exit_ne:
	       pck.Mode_exit=mode_exit_se;
	  break;
	 }// switch
	}// if есть изменения
	else return NORESTART;/* выход в ДОС */
  break;
  case _SPACE_:/* управление выводом меню */
	     if(mn1_show==0) mn1_show=1;/* нужно выводить */
	       else       {mn1_show=0;mn1_out=0;};/* не нужно выводить */
//           printf("%d %d %d %d    \r",xmin_m,ymin_m,xmax_m,ymax_m);
		 switch(mn1_show)
	     { case 1:/* запоминаем место под меню */
//		      getimage(xmin_m,ymin_m,xmax_m,ymax_m,menu);
		   break;
	       case 0:
		      putimage(xmin_m,ymin_m,menu,COPY_PUT);
	   break;
	     }// switch
  break;

  case _F1_:
		help_f1();
  break;
  case _F2_:
	outpic_obj_num(menu_mess,255,1);//стираем предыдущее пояснение
	outpic_obj_num(menu_mess,mode_save_all_tbg,30);//новое
	pic.SaveAll(pck.Fn_work_tbg,mode_save_all_tbg);
  break;
  case _F5_:
     // копирование ЛЭ из текущего ГЭ, в ГЭ попавшие в РАМКУ
     pic.Add_LE_to_GE_in_ramca_from_current_GE();
     SaveTest = ON;
     break;
  case _shiftF5_:
     // удаление ЛЭ из ГЭ попавших в РАМКУ
     pic.Del_LE_to_GE_in_ramca();
     SaveTest = ON;
     break;
  case _F8_:
     mouse_on();
     panel_mode(&pn_macro);
     break;
  case _F9_:
     mouse_on();
     if(panel_mode(&pn_pipe) == Fu){
        break;
     }/* if */
     break;
  case _F7_:
/*
	  if((peek(0,0x417) & 3) == 0)
     {/* нет нажатых Shift-L или Shift-R */
        if(pic.GetTip() != TIP_line)
           break;

        POINT l[2]; /* координаты линии исходные */
        int dp = 0,i,cLN,cNO;
        POINT *p; /* указатель на массив точек линий */

        l[0].x = pic.GetX();
        l[0].y = pic.GetY();
        l[1].x = pic.GetX2();
        l[1].y = pic.GetY2();
        cNO = pic.Get_NO_CGE();
        cLN = pic.Get_LN_CGE();
        if(((GROUP)||(INCREMENT))&&((cNO == -1) || (cLN == -1))) /* нет логики */
           break;
        p = LINE2PIPE(l,&dp);
        if(p == NULL) /* труба не создана */
           break;
        if(PIPE[DELETE_SrcGE])
           pic.DelGE();
        for(i = 0;i < dp;i++)
        {/* добавляем отрезки трубы */
           if(pic.CreateGE(TIP_line)) /* нет памяти для создания GE */
              break;
           pic.SetX(mmx((p+i)->x));
           pic.SetY(mmy((p+i)->y));
           if(i+1 != dp){
              pic.SetX2(mmx((p+1+i)->x) - mmx((p+i)->x));
              pic.SetY2(mmy((p+1+i)->y) - mmy((p+i)->y));
           }/* if */
           else{
              pic.SetX2(mmx(l[1].x) - mmx((p+i)->x));
              pic.SetY2(mmy(l[1].y) - mmy((p+i)->y));
           }/* elif */
           if(PIPE[GROUP])
              pic.AddLE(cNO,(i % 3) + 1);
           if(PIPE[INCREMENT])
              pic.AddLE(cNO,(cLN+i) % 256);
           putpixel((p+i)->x,  (p+i)->y,  DARKGRAY);
           putpixel((p+1+i)->x,(p+1+i)->y,DARKGRAY);
           risp(pic.GetX(),pic.GetY());// рисуем стрелку указателя в начале ГЭ
         /* добавляем отрезки трубы */
        }/* for */
        free(p);

        if(farheapcheck() < 0)
        {
         closegraph();
         fprintf(stderr,"\nMemory allocation ERROR.\nExit program TBG 6.0\n");
         exit(-1);
        }/* if */
      /* нет нажатых Shift-L или Shift-R */
     }/* if */
*/
     break;
  case _a_:
  case _shift_a_:
   {
   static xzoom = 2;
	if((peek(0,0x417)&3)==0)/* нет нажатых Shift-L или Shift-R */
	  {ZoomOn(xzoom++);
      obrab_mouse();
     }
	  else
	  {ZoomOff();xzoom = 2;};/*if*/
   }
  break;

  case _shift_r_:
  sRulers = 0;
  refr_t();
  break;
  case _r_:
  sRulers = 1;
  Rulers();
  break;

  case _LEFT_:// точное перемещение мыши
	if((peek(0,0x417)&3)==0)/* нет нажатых Shift-L или Shift-R */
	  {mouse_setxy(x_mouse-1,y_mouse);}
	  else
	  {mouse_setxy(x_mouse-10,y_mouse);};/*if*/
  break;
  case _RIGHT_:
	if((peek(0,0x417)&3)==0)/* нет нажатых Shift-L или Shift-R */
	  {mouse_setxy(x_mouse+1,y_mouse);}
	  else
	  {mouse_setxy(x_mouse+10,y_mouse);};/*if*/
   break;
   case _UP_:
	if((peek(0,0x417)&3)==0)/* нет нажатых Shift-L или Shift-R */
	  {mouse_setxy(x_mouse,y_mouse-1);}
	  else
	  {mouse_setxy(x_mouse,y_mouse-10);};/*if*/
   break;
   case _DOWN_:
	if((peek(0,0x417)&3)==0)/* нет нажатых Shift-L или Shift-R */
	  {mouse_setxy(x_mouse,y_mouse+1);}
	  else
	  {mouse_setxy(x_mouse,y_mouse+10);};/*if*/
     break;
   case _PgUp_:
      if(pic.GetTip() == TIP_pipeline){
	      if((peek(0, 0x417) & 3) == 0){/* нет нажатых Shift-L или Shift-R */
            pic.SetS(+1);
		   }/* if */
	      else{
            pic.SetDO(+1);
		   };/* elif */
      }/* if */
      break;
   case _PgDn_:
      if(pic.GetTip() == TIP_pipeline){
	      if((peek(0, 0x417) & 3) == 0){/* нет нажатых Shift-L или Shift-R */
            pic.SetS(-1);
		   }/* if */
	      else{
            pic.SetDO(-1);
		   };/* elif */
      }/* if */
      break;
  default: break;
 };/*switch(skankod)*/

switch(skankod)
{
  case _F10_:// перевывод
	refr_t();
  break;/* перевывод */
}// switch(skankod)

/*
 switch(skankod)
 {
  case _q_:Multx++;
  break;
  case _a_:Multx--;
  break;
  case _w_:Multy++;
  break;
  case _s_:Multy--;
  break;
 }//
*/


/*
 switch(skankod)
 {/* переключение режимов */
  case _1_:
	    pck.ig_mode=1;/* CGA */
  break;
  case _2_: pck.ig_mode=2;/* EGA */
  break;
  case _3_: pck.ig_mode=3;/* VGA */
  break;
  case _4_: pck.ig_mode=4;/* VGA256 */
  break;
  case _5_: pck.ig_mode=5;/* EGA */
  break;
//  case _6_: pck.ig_mode=6;/* VGA */
//  break;
  case _7_: pck.ig_mode=max_ig_mode;/* DETECT */
  break;
 }/* switch */
*/

 if((num_rep>=1)&&(num_rep<=max_ig_mode))
 {
  pck.ig_mode=num_rep;
  num_rep=0;
  /* 1 CGA */
  /* 2 EGA */
  /* 3 VGA */
  /* 4 VGA256 */
  /* 5 EGA */
  /* 6 VGA */
  /* max_ig_mode DETECT */
 }/* if */

 if(pck.ig_mode!=0) {
    mn1_out=OFF;/* не выводилось меню */
    free(menu);/* освобождаем */
    return pck.ig_mode;
 }// if

 pos_ms_pred--;/* для обновления сообщений подсказки */

} while(1!=0);/*вечный цикл*/

}/* painter */
/****************************************************************************/
