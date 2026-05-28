/* tbg_mc00.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
                      (C) Чумак А.И.    1992-97
---------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------- */
/* Структура макропримитива в памяти
   mpc
  +---+
  | * |
  +-+-+
    |
    |
    |
    |                                                                        NULL
    |                                                           ICommand      A
    |                                                            +----------+ |
    |                                                          +>| next   *-+-+
    |                                                          | +----------|
    |                                                          | | type     |  char*
    |                                                          | +----------|  +------+
    |                                                          | | dc     *-+->||||||||
    |                                                          | +----------+  +------+              NULL
    |                                                          +---------------+                       A
    |                                                           ICommand       |          ICommand     |
    |                                                            +----------+  |          +----------+ |
    |                                                            | next   *-+--+          | next   *-+-+
    |                                                          +>+----------|           +>+----------|
    |                                                          | | type     | char*     | | type     |  char*
    |                                                          | +----------|  +------+ | +----------|  +-+
    |                                                          | | dc     *-+->|||||||| | | dc     *-+->|||
    |                                                   NULL   | +----------+  +------+ | +----------+  +-+
    |                                                    A     +---------------+        +--------------+
    |                                      ICommand      |       ICommand      |          ICommand     |
    |                                      +-----------+ |       +----------+  |          +----------+ |
    |                                    +>| next   *--+-+     +>| next   *-+--+        +>| next   *-+-+
    |                                    | +-----------|       | +----------|           | +----------|
    |                                    | | type      |       | | type     | char*     | | type     |  char*
    |                                    | +-----------|       | +----------|  +------+ | +----------|  +--+
    |                                    | | dc     *--+->NULL | | dc     *-+->|||||||| | | dc     *-+->||||
    |                                    | +-----------+       | +----------+  +------+ | +----------+  +--+
    |                           next     +-------------+       +--------------+         +--------------+
    |                         MPClass      SHead       |         SHead        |           SHead        |
    |                            A         +----------+|         +----------+ |           +----------+ |
    |                            |    +--->| next *---++-------->| next *---+-+---------->| next *---+-+->NULL
    |                            |    |    +----------||         +----------| |           +----------| |
    |                            |    |    | nState   ||         | nState   | |           | nState   | |
    |       MPClass              |    |    +----------||         +----------| |           +----------| |
    +----->+------------------+  |    |    | nCommand ||         | nCommand | |           | nCommand | |
           | Index            |  |    |    +----------||         +----------| |           +----------| |
           +------------------|  |    |    |          ||         |          | |           |          | |
           | next      *------+--+    |    +----------||         +----------| |           +----------| |
           +------------------|       |    |IChead*---++         |IChead*---+-+           |IChead*---+-+
           | pSHead    *------+-------+    +----------+          +----------+             +----------+
           +------------------|
           |   "M00001.TBG"   |
           +------------------|    OBJECT **
           |                  |    +--------------------------------+
           | *pic      *------+--->|*|*|*|*|*|*|*|   .  .  .  .   |*|
           +------------------|    +--------------------------------+
           | cb        *------+---+
           +------------------|   |
           | uname     *------+---+-+
           +------------------+   | |
                                  | |
           +----------------------+ | +-----------------------------------+
           |                        +>|Клапан проходной ...            |\0|
           |                          +-----------------------------------+
           |
           V MyFunc()
           +-----------------+
           |                 |
           | . . .           |
           |                 |
           | CODE            |
           |                 |
           | . . .           |
           |                 |
           +-----------------+

*/
/* ----------------------------------------------------------------------- */


#include "tbg_tc.h" /* обьявления функций и данных */
#include "ltool.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <alloc.h>

#ifdef _WIN32
int heapchecknode(void *p){
   return _USEDENTRY;
}
int heapchecknode(void *p);
#endif
/* ----------------------------------------------------------------------- */
typedef
   struct TBG_LIST {
      struct TBG_LIST *next;
      LPMPClass ToMPClass;
   }TBG_LIST, *LPTBG_LIST;

typedef
   struct TBG_HEADER {
      LPTBG_LIST head;
      LPTBG_LIST tail;
      LPTBG_LIST current;
   }TBG_HEADER, *LPTBG_HEADER;
/* ----------------------------------------------------------------------- */
static void execCI(MPClass *tmpc,TCommand tc, unsigned short int sd,
                   char *dc, unsigned short int st, LPMP mp);
/* ----------------------------------------------------------------------- */
static char *MPath = ".";
static MPClass *mpc = NULL;  /* голова списка классов макропримитивов */

FILE *mperr = NULL;
/* внешние переменные лексического анализатора */
long int vali;
double vald;
int comment = 0;
char buftxt[256];
int  maxbuftxt;
int *pnumln = NULL;
/* ----------------------------------------------------------------------- */
/* состояния автомата чтения файла MACROTBM.TXT */
typedef
   enum {
      aSTART,        /* начало - ещё ничего не прочитано */
      aMACRO,        /* прочитано ключевое слово(КС) "Макро" */
      aDESC,         /* пр. КС "Описание" */
      aPICT,         /* пр. КС "Картинка" */
      aSTATE,        /* пр. КС "Состояние" */
      aSTATEFROM,    /* пр. КС "Состояния"*/
      aTXTDESC,      /* пр. строка описания */
      aTXTPICT,      /* пр. имя файла картинки */
      aSEPMACRO,     /* пр. разделитель ":" */
      aSEPPICT,      /* пр. ":" */
      aSEPDESC,      /* пр. ":" */
      aSEPSTATE,     /* пр. ":" */
      aSEPSTATEFROM, /* пр. ":" */
      aSEPCOM,       /* пр. ":" */
      aNUMMACRO,     /* пр. номер макропримитива */
      aNUMSTATE,     /* пр. номер состояния */
      aNUMSTATEFROM, /* пр. номер макропримитива состояний */
      aCOM,          /* пр. команда интефейса TBG */
      aARGCOMTXT,    /* пр. аргумент типа ТЕКСТ */
      aARGCOMNUM,    /* пр. ------"------ целое число */
      aARGCOMFLT,    /* пр. ------"------ floatpoint число */
      aARGCOMNUMOBJ, /* пр. ------"------ номер объекта */
      aARGCOMNUMELM, /* пр. ------------- номер элемента объекта */
      aARGCOMCOLOR,  /* пр. ------------- код цвета */
      aARGCOMMODE,   /* пр. ------------- режим вывода */
      aNULL,         /* пропуск */
      aEOFSTOP,      /* автомат остановлен по причине конца файла */
      aERRORSTOP,    /* автомат остановлен с ошибкой */
      aSTOP          /* автомат остановлен */
   }AMA;

/* ----------------------------------------------------------------------- */
/* таблица команд */
COMMAND commands[] = {
   {"Макро"                        , MACRO                , 0                            , 1, { NUMBER}},
   {"Описание"                     , DESC                 , 0                            , 1, { TEXT}},
   {"Картинка"                     , PICT                 , 0                            , 1, { TEXT}},
   {"Состояние"                    , STATE                , 0                            , 1, { NUMBER}},
   {"Состояния"                    , STATEFROM            , 0                            , 1, { NUMBER}},
   {"сохранить_состояние"          , Csavestate           , 0                            , 0  },
   {"показать"                     , Coutpic_all          , 0                            , 0  },
   {"показать_объект"              , Coutpic_obj          , sizeof(Doutpic_obj)          , 1, { TOBJ}},
   {"показать_элемент"             , Coutpic_obj_num      , sizeof(Doutpic_obj_num)      , 2, { TOBJ, TNUM}},
   {"показать_элемент_вращением"   , Coutpic_obj_num_with_rotate    , sizeof(Doutpic_obj_num_with_rotate)    , 3, { TOBJ, TNUM, NUMF}},

//   {"установить_привязку"          , Csetreper_obj_num    , sizeof(Dsetreper_obj_num)    , 2, { TOBJ, TNUM}},
//   {"setreper"                     , Csetreper_obj_num    , sizeof(Dsetreper_obj_num)    , 2, { TOBJ, TNUM}},

   {"установить_цвет_элемента"     , Csetcolor_obj_num    , sizeof(Dsetcolor_obj_num)    , 4, { TOBJ, TNUM, COLORS, MODES}},
   {"установить_цвет_объекта"      , Csetcolor_obj        , sizeof(Dsetcolor_obj)        , 3, { TOBJ, COLORS, MODES}},
   {"установить_состояние_элемента", Csetstate_obj_num    , sizeof(Dsetstate_obj_num)    , 3, { TOBJ, TNUM, NUMBER}},
   {"установить_состояние_объекта" , Csetstate_obj        , sizeof(Dsetstate_obj)        , 2, { TOBJ, NUMBER}},
   {"установить_текст_объекта"     , Couttext_obj_num     , sizeof(Douttext_obj_num)     , 5, { TOBJ, TNUM, TEXT, COLORS, MODES}},
   {"исполнить_состояние"          , CexecState           , sizeof(DexecState)           , 1, { NUMBER}},
   {"показать_элемент_цветом"      , Coutpic_obj_num_color, sizeof(Doutpic_obj_num_color), 3, { TOBJ, TNUM, COLORS}},
   {"показать_объект_цветом"       , Coutpic_obj_color    , sizeof(Doutpic_obj_color)    , 2, { TOBJ, COLORS}},

   {"╠ръЁю"                        , MACRO                , 0                            , 1, { NUMBER}},
   {"╬яшёрэшх"                     , DESC                 , 0                            , 1, { TEXT}},
   {"╩рЁЄшэър"                     , PICT                 , 0                            , 1, { TEXT}},
   {"╤юёЄю эшх"                    , STATE                , 0                            , 1, { NUMBER}},
   {"╤юёЄю эш "                    , STATEFROM            , 0                            , 1, { NUMBER}},
   {"ёюїЁрэшЄ№_ёюёЄю эшх"          , Csavestate           , 0                            , 0  },
   {"яюърчрЄ№"                     , Coutpic_all          , 0                            , 0  },
   {"яюърчрЄ№_юс·хъЄ"              , Coutpic_obj          , sizeof(Doutpic_obj)          , 1, { TOBJ}},
   {"яюърчрЄ№_¤ыхьхэЄ"             , Coutpic_obj_num      , sizeof(Doutpic_obj_num)      , 2, { TOBJ, TNUM}},
   {"яюърчрЄ№_¤ыхьхэЄ_тЁр∙хэшхь"   , Coutpic_obj_num_with_rotate    , sizeof(Doutpic_obj_num_with_rotate)    , 3, { TOBJ, TNUM, NUMF}},
   {"єёЄрэютшЄ№_ЎтхЄ_¤ыхьхэЄр"     , Csetcolor_obj_num    , sizeof(Dsetcolor_obj_num)    , 4, { TOBJ, TNUM, COLORS, MODES}},
   {"єёЄрэютшЄ№_ЎтхЄ_юс·хъЄр"      , Csetcolor_obj        , sizeof(Dsetcolor_obj)        , 3, { TOBJ, COLORS, MODES}},
   {"єёЄрэютшЄ№_ёюёЄю эшх_¤ыхьхэЄр", Csetstate_obj_num    , sizeof(Dsetstate_obj_num)    , 3, { TOBJ, TNUM, NUMBER}},
   {"єёЄрэютшЄ№_ёюёЄю эшх_юс·хъЄр" , Csetstate_obj        , sizeof(Dsetstate_obj)        , 2, { TOBJ, NUMBER}},
   {"єёЄрэютшЄ№_ЄхъёЄ_юс·хъЄр"     , Couttext_obj_num     , sizeof(Douttext_obj_num)     , 5, { TOBJ, TNUM, TEXT, COLORS, MODES}},
   {"шёяюыэшЄ№_ёюёЄю эшх"          , CexecState           , sizeof(DexecState)           , 1, { NUMBER}},
   {"яюърчрЄ№_¤ыхьхэЄ_ЎтхЄюь"      , Coutpic_obj_num_color, sizeof(Doutpic_obj_num_color), 3, { TOBJ, TNUM, COLORS}},
   {"яюърчрЄ№_юс·хъЄ_ЎтхЄюь"       , Coutpic_obj_color    , sizeof(Doutpic_obj_color)    , 2, { TOBJ, COLORS}},

   {"savestate"           , Csavestate           , 0                            , 0  },
   {"outpic_all"          , Coutpic_all          , 0                            , 0  },
   {"outpic_obj"          , Coutpic_obj          , sizeof(Doutpic_obj)          , 1, { TOBJ}},
   {"outpic_obj_num"      , Coutpic_obj_num      , sizeof(Doutpic_obj_num)      , 2, { TOBJ, TNUM}},
   {"outpic_obj_num_with_rotate", Coutpic_obj_num_with_rotate    , sizeof(Doutpic_obj_num_with_rotate)    , 3, { TOBJ, TNUM, NUMF}},
   {"setcolor_obj_num"    , Csetcolor_obj_num    , sizeof(Dsetcolor_obj_num)    , 4, { TOBJ, TNUM, COLORS, MODES}},
   {"setcolor_obj"        , Csetcolor_obj        , sizeof(Dsetcolor_obj)        , 3, { TOBJ, COLORS, MODES}},
   {"setstate_obj_num"    , Csetstate_obj_num    , sizeof(Dsetstate_obj_num)    , 3, { TOBJ, TNUM, NUMBER}},
   {"setstate_obj"        , Csetstate_obj        , sizeof(Dsetstate_obj)        , 2, { TOBJ, NUMBER}},
   {"outtext_obj_num"     , Couttext_obj_num     , sizeof(Douttext_obj_num)     , 5, { TOBJ, TNUM, TEXT, COLORS, MODES}},
   {"execState"           , CexecState           , sizeof(DexecState)           , 1, { NUMBER}},
   {"outpic_obj_num_color", Coutpic_obj_num_color, sizeof(Doutpic_obj_num_color), 3, { TOBJ, TNUM, COLORS}},
   {"outpic_obj_color"    , Coutpic_obj_color    , sizeof(Doutpic_obj_color)    , 2, { TOBJ, COLORS}}

};

/* ключевые слова режима вывода */
KEY key_modes[] = {
      {"SHOW", SHOW},
      {"NO_SHOW", NO_SHOW}
};

/* ключевые слова кода цвета */
KEY key_colors[] = {
      {"BLACK", BLACK},
      {"BLUE" , BLUE},
      {"GREEN", GREEN},
      {"CYAN" , CYAN},
      {"RED"  , RED},
      {"GOLD" , GOLD},
      {"BOLOTO", BOLOTO},
      {"LIGHTGRAY", LIGHTGRAY},
      {"DARKGRAY" , DARKGRAY},
      {"LIGHTBLUE", LIGHTBLUE},
      {"LIGHTGREEN",LIGHTGREEN},
      {"LIGHTCYAN", LIGHTCYAN},
      {"LIGHTRED",  LIGHTRED},
      {"PASTEL",    PASTEL},
      {"YELLOW",    YELLOW},
      {"WHITE" ,    WHITE}
};

/* текущая команда в таблице */
int curcomm;
/* текущий аргумент в текущей команде */
int curcommarg;

/* Автомат остановлен */
static AMA m = aSTOP;

static char *se866[] = {
"\nСтрока %d, аргументов в команде '%s' должно быть %d.",
"\nСтрока %d, неправльный аргумент в команде '%s'.",
"\nСтрока %d, пропущено ключевое слово '%s'.",
"\nСтрока %d, пропущено двоеточие.",
"\nСтрока %d, неправльное ключевое слово '%s'.",
"\nСтрока %d, пропущен аргумент: '%s'.",
"\nСтрока %d, должно быть '%s'."
};

static char *se1251[] = {
"╤ЄЁюър %d, рЁуєьхэЄют т ъюьрэфх '%s' фюыцэю с√Є№ %d.",
"╤ЄЁюър %d, эхяЁрты№э√щ рЁуєьхэЄ т ъюьрэфх '%s'.",
"╤ЄЁюър %d, яЁюяє∙хэю ъы■ўхтюх ёыютю '%s'.",
"╤ЄЁюър %d, яЁюяє∙хэю фтюхЄюўшх.",
"╤ЄЁюър %d, эхяЁрты№эюх ъы■ўхтюх ёыютю '%s'.",
"╤ЄЁюър %d, яЁюяє∙хэ рЁуєьхэЄ: '%s'.",
"╤ЄЁюър %d, фюыцэю с√Є№ '%s'."
};

#define SE866  0
#define SE1251 1

static char **se[] = {
   se866,
   se1251
};
static int cse =
#ifndef _WIN32_
SE866;
#else
SE1251;
#endif
/* ----------------------------------------------------------------------- */
char* MPCErr(){
}/* MPCErr */
/* ----------------------------------------------------------------------- */
/* печать состояния автомата */
/*
void pm(AMA m){
   printf("line:%d ", (*pnumln));
   switch(m){
      case aSTART    : printf("aSTART\n");    break;
      case aSTOP     : printf("aSTOP\n");     break;
      case aERRORSTOP     : printf("aERRORSTOP\n");     break;
      case aMACRO    : printf("aMACRO\n");    break;
      case aSEPMACRO : printf("aSEPMACRO\n"); break;
      case aNUMMACRO : printf("aNUMMACRO\n"); break;
      case aDESC     : printf("aDESC\n");     break;
      case aSEPDESC  : printf("aSEPDESC\n");  break;
      case aTXTDESC  : printf("aTXTDESC\n");  break;
      case aPICT     : printf("aPICT\n");     break;
      case aSEPPICT  : printf("aSEPPICT\n");  break;
      case aTXTPICT  : printf("aTXTPICT\n");  break;
      case aSTATE    : printf("aSTATE\n");    break;
      case aSTATEFROM    : printf("aSTATEFROM\n");    break;
      case aSEPSTATE : printf("aSEPSTATE\n"); break;
      case aSEPSTATEFROM : printf("aSEPSTATEFROM\n"); break;
      case aNUMSTATE : printf("aNUMSTATE\n"); break;
      case aNUMSTATEFROM : printf("aNUMSTATEFROM\n"); break;
      case aCOM      : printf("aCOM\n");      break;
      case aSEPCOM   : printf("aSEPCOM\n");   break;
      case aARGCOMTXT: printf("aARGCOMTXT\n");break;
      case aARGCOMFLT: printf("aARGCOMFLT\n");break;
      case aARGCOMNUM: printf("aARGCOMNUM\n");break;
      case aARGCOMNUMOBJ: printf("aARGCOMOBJ\n");break;
      case aARGCOMNUMELM: printf("aARGCOMELM\n");break;
      case aARGCOMCOLOR: printf("aARGCOMCOL\n");break;
      case aARGCOMMODE: printf("aARGCOMMOD\n");break;
      default:          printf("NONAME: >%d<\n", m);break;
   }/* switch */
}/* pm */
*/
/* ----------------------------------------------------------------------- */
/* для yylex.c */
int yywrap(){ return 1; }
/* ----------------------------------------------------------------------- */
/* найти ключевое слово */
int fkey(char *fs, KEY *ka, int sa){
   register int i;
   for(i = 0;i < sa;i++){
      if(strcmp(ka[i].str, fs) == 0){
         return ka[i].id;
      }/* if */
   }/* for */
   return -1;
}/* fkey */
/* ----------------------------------------------------------------------- */
/* найти команду в таблице с заполнением текущих величин */
void fcom(char *s){
   int i;
   curcomm = -1;
   for(i = 0;i < sizeof(commands) / sizeof(COMMAND);i++){
      if(strcmp(commands[i].str, s) == 0){
         curcomm = i;
         curcommarg = 0;
         break;
      }/* if */
   }/* for */
}/* fcom */
/*--------------------------------------------------------------------------*/
/* Шаг автомата */
AMA ASTEP(void){
   /* получить лексему */
   int l = yylex();
//   printf("\n.");
/*
   printf("\n lex: >%s< ",l == NUMINT ? "NUMINT" :
                      l == NUMFLT ? "NUMFLT" :
                      l == TXT ? "TXT" :
                      l == SEP ? "SEP" :
                      l == COMS ? "COMS" :
                      l == COME ? "COME" :
                      l == KEYWORD ? "KEYWORD" :
                      l == COMCPP ? "COMCPP" :
                      l == NEWLINE ? "NEWLINE" : "???"
                      );
   pm(m);
*/
   switch(l){
      case NUMINT:
         if(comment == 0){
//            printf("[%d]-", (int)vali);
            switch(m){
               case aSEPMACRO:
                  m = aNUMMACRO;
                  break;
               case aNUMSTATE:
                  m = aCOM;
                  break;
               case aSEPSTATE:
                  m = aNUMSTATE;
                  break;
               case aSEPSTATEFROM:
                  m = aNUMSTATEFROM;
                  break;
               case aSEPCOM:
               case aARGCOMTXT:
               case aARGCOMNUM:
               case aARGCOMNUMOBJ:
               case aARGCOMNUMELM:
               case aARGCOMCOLOR:
               case aARGCOMMODE:
                  if(curcommarg >= commands[curcomm].cntarg){
                     fprintf(mperr, se[cse][0], *pnumln, commands[curcomm].str, commands[curcomm].cntarg);
                     m = aERRORSTOP;
                     break;
                  }/* if */
                  switch(commands[curcomm].typearg[curcommarg]){
                     case NUMBER:
                        curcommarg++;
                        m = aARGCOMNUM;
                        break;
                     case TOBJ:
                        curcommarg++;
                        m = aARGCOMNUMOBJ;
                        break;
                     case TNUM:
                        curcommarg++;
                        m = aARGCOMNUMELM;
                        break;
                     default:
                        fprintf(mperr, se[cse][1], *pnumln, commands[curcomm].str);
                        m = aERRORSTOP;
                        break;
                  }/* switch */
                  break;
               default:
                  fprintf(mperr, se[cse][1], *pnumln, commands[curcomm].str);
                  m = aERRORSTOP;
                  break;
            }/* switch */
         }/* if */
         break;
      case NUMFLT:
         if(comment == 0){
//            printf("[%lf]-", vald);
            switch(m){
               case aARGCOMNUMELM:
                  if(curcommarg >= commands[curcomm].cntarg){
                     fprintf(mperr, se[cse][0], *pnumln, commands[curcomm].str, commands[curcomm].cntarg);
                     m = aERRORSTOP;
                     break;
                  }/* if */
                  switch(commands[curcomm].typearg[curcommarg]){
                     case NUMF:
                        curcommarg++;
                        m = aARGCOMFLT;
                        break;
                     default:
                        fprintf(mperr, se[cse][1], *pnumln, commands[curcomm].str);
                        m = aERRORSTOP;
                        break;
                  }/* switch */
                  break;
               default:
                  fprintf(mperr, se[cse][1], *pnumln, commands[curcomm].str);
                  m = aERRORSTOP;
                  break;
            }/* switch */
         }/* if */
         break;
      case KEYWORD:
         if(comment == 0){
//            printf("[%s]-", buftxt);
            switch(m){
               case aNUMSTATEFROM:
                  /* найти структуру команды */
                  fcom(buftxt);
                  if(commands[curcomm].id != MACRO){
                     fprintf(mperr, se[cse][2], *pnumln, "Макро");
                     m = aERRORSTOP;
                  }/* if */
                  m = aSTOP;
                  break;
               case aSEPCOM:
               case aARGCOMTXT:
               case aARGCOMNUM:
               case aARGCOMFLT:
               case aARGCOMNUMOBJ:
               case aARGCOMNUMELM:
               case aARGCOMCOLOR:
               case aARGCOMMODE:
                  if(curcommarg >= commands[curcomm].cntarg){
                     fcom(buftxt);
                     switch(commands[curcomm].id){
                        case STATE:
                           m = aSTATE;
                           break;
                        case MACRO:
                           // m = aMACRO;
                           m = aSTOP;
                           break;
                        default:
                           if((commands[curcomm].id >= 0) &&
                              (commands[curcomm].id < END_TBG_COMMAND)){
                              m = aCOM;
                           }/* if */
                           else{
                              fprintf(mperr, se[cse][1], *pnumln, commands[curcomm].str);
                              m = aERRORSTOP;
                           }/* elif */
                           break;
                     }/* switch */
                     break;
                  }/* if */
                  switch(commands[curcomm].typearg[curcommarg]){
                     case COLORS:
                        curcommarg++;
                        m = aARGCOMCOLOR;
                        break;
                     case MODES:
                        // // printf(".arg num: <%s>\n", buftxt);
                        curcommarg++;
                        m = aARGCOMMODE;
                        break;
                     default:
                        {
                        int ca = commands[curcomm].typearg[curcommarg];
                        char *cas = ca == TEXT   ? "\"строка\"" :
                                    ca == TOBJ   ? "номер_объекта" :
                                    ca == TNUM   ? "номер_элемента" :
                                    ca == COLORS ? "ЦВЕТ" :
                                    ca == MODES  ? "РЕЖИМ" :
                                    ca == NUMBER ? "число" :
                                    ca == NUMF   ? "число_с_плавающей_точкой" : "???";
                        fprintf(mperr, se[cse][5], *pnumln, cas);

                        }
                        m = aERRORSTOP;
                        break;
                  }/* switch */
                  break;
               case aNUMSTATE:
                  /* найти структуру команды */
                  fcom(buftxt);
                  if((commands[curcomm].id >= 0) &&
                     (commands[curcomm].id < END_TBG_COMMAND)){
                     m = aCOM;
                  }/* if */
                  else{
                     fprintf(mperr, se[cse][0], *pnumln, commands[curcomm].str, commands[curcomm].cntarg);
                     m = aERRORSTOP;
                  }/*  */
                  break;
               case aSTART:
                  /* найти структуру команды */
                  fcom(buftxt);
                  if(commands[curcomm].id == MACRO){
                     m = aMACRO;
                  }/* if */
                  else{
                     fprintf(mperr, se[cse][2], *pnumln, "Макро");
                     m = aERRORSTOP;
                  }/*  */
                  break;
               case aNUMMACRO:
                  fcom(buftxt);
                  if(commands[curcomm].id == DESC){
                     m = aDESC;
                  }/* if */
                  else{
                     fprintf(mperr, se[cse][6], *pnumln, "число");
                     m = aERRORSTOP;
                  }/*  */
                  break;
               case aTXTDESC:
                  fcom(buftxt);
                  if(commands[curcomm].id == PICT){
                     m = aPICT;
                  }/* if */
                  else{
                     fprintf(mperr, se[cse][2], *pnumln, "Картинка");
                     m = aERRORSTOP;
                  }/*  */
                  break;
               case aTXTPICT:
                  fcom(buftxt);
                  if(commands[curcomm].id == STATE){
                     m = aSTATE;
                  }/* if */
                  else{
                     if(commands[curcomm].id == STATEFROM){
                        m = aSTATEFROM;
                        break;
                     }/* if */
                     fprintf(mperr, se[cse][6], *pnumln, "номер состояний");
                     m = aERRORSTOP;
                  }/*  */
                  break;
               default:
                  fprintf(mperr, se[cse][4], *pnumln, buftxt);
                  m = aERRORSTOP;
                  break;
            }/* switch */
         }/* if */
         break;
      case SEP:
         if(comment == 0){
//            printf("[:]-");
            switch(m){
               case aMACRO:
                  m = aSEPMACRO;
                  break;
               case aDESC:
                  m = aSEPDESC;
                  break;
               case aPICT:
                  m = aSEPPICT;
                  break;
               case aSTATE:
                  m = aSEPSTATE;
                  break;
               case aSTATEFROM:
                  m = aSEPSTATEFROM;
                  break;
               case aCOM:
                  m = aSEPCOM;
                  break;
               default:
                  fprintf(mperr, se[cse][3], *pnumln);
                  m = aERRORSTOP;
                  break;
            }/* switch */
         }/* if */
         break;
      case TXT:
         if(comment == 0){
//            printf("[%s]-", buftxt);
            switch(m){
               case aSEPDESC:
                  // // printf(".desc: <%s>\n", buftxt);
                  m = aTXTDESC;
                  break;
               case aSEPPICT:
                  // // printf(".fn tbg: <%s>\n", buftxt);
                  m = aTXTPICT;
                  break;
               case aSEPCOM:
               case aARGCOMTXT:
               case aARGCOMNUM:
               case aARGCOMNUMOBJ:
               case aARGCOMNUMELM:
               case aARGCOMCOLOR:
               case aARGCOMMODE:
                  if(curcommarg >= commands[curcomm].cntarg){
                     fprintf(mperr, se[cse][0], *pnumln, commands[curcomm].str, commands[curcomm].cntarg);
                     m = aERRORSTOP;
                     break;
                  }/* if */
                  if(commands[curcomm].typearg[curcommarg] == TEXT){
                    curcommarg++;
                    m = aARGCOMTXT;
                  }/* if */
                  else{
                     fprintf(mperr, se[cse][6], *pnumln, "\"текст\"");
                     m = aERRORSTOP;
                  }/* elif */
                  break;
               default:
                  fprintf(mperr, se[cse][4], *pnumln, "");
                  m = aERRORSTOP;
                  break;
            }/* switch */
         }/* if */
         break;
      case 0:
//         printf("File end???\n");
         switch(m){
            case aSEPCOM:
            case aARGCOMTXT:
            case aARGCOMNUM:
            case aARGCOMFLT:
            case aARGCOMNUMOBJ:
            case aARGCOMNUMELM:
            case aARGCOMCOLOR:
            case aARGCOMMODE:
            case aNUMSTATEFROM:
               m = aSTOP;
               break;
            default:
               m = aERRORSTOP;
               break;
         }/* switch */
         break;
      default:
         return aNULL;
   }/* switch */
   // // printf("%lu\n", coreleft());
   return m;
}/* ASTEP */
/* ----------------------------------------------------------------------- */
short int MPCFree(MPClass *mpcd){
   MPClass *mpct1;
   SHead    *tsh = NULL;
   ICommand *tic = NULL;
   short int reshcn;

   // // printf("+in MPCFree\n");
   if(mpcd == NULL)
      return Ok;

   reshcn = heapchecknode(mpcd);
   if(reshcn != _USEDENTRY){
      assert(reshcn != _HEAPCORRUPT);
      return Ok;
   }/* if */
   /* ещё кто-то использует его ? */
   // printf("+ <%d> ещё кто-то использует его ?", mpcd->index);
   if(mpcd->nsuse > 0){
      // printf("-Да\n");
      // printf("+out MPCFree\n");
      if(mpcd->nuse > 0)
         mpcd->nuse--;
      return Ok; /* Да */
   }/* if */

   /* ещё кто-то использует его ? */
   // printf("+ <%d> ещё кто-то использует его ?", mpcd->index);
   if(--(mpcd->nuse) > 0){
      // printf("-Да\n");
      // printf("+out MPCFree\n");
      return Ok; /* Да */
   }/* if */

   /* Нет. Надо удалять. */
   // printf("-Нет. Надо удалять.\n");

   // printf("+Проверка: не является ли удаляемый головой?");
   if(mpcd == mpc){
      // printf("-Является.\n++Перестроим голову.\n");
      mpc = mpcd->next;
      // printf("++Голова указывет на следующий после удаляемого.\n");
	}/* if */
   else{
      /* поиск элемента стоящего перед удаляемым */
      // printf("-Не является,\n++поиск элемента стоящего перед удаляемым\n");
      mpct1 = mpc;
      while(mpct1 != NULL){
         if(mpct1->next == mpcd){
            /* Нашли, Found! */
            // printf("+++Нашли\n");
            break;
         }/* if */
         // printf("++следующий класс\n");
         mpct1 = mpct1->next;
      }/* while */
      /* предыдущий теперь указывает на следующий за удаляемым */
      // printf("+предыдущий теперь указывает на следующий за удаляемым\n");
      mpct1->next = mpcd->next;
   }/* elif */

   /* удаляем всё по порядку */
   // printf("+удаляем всё по порядку\n");
   mpcd->next = NULL;
   del_pic(&mpcd->pic);
   print_error_tbg("MPCFree del_pic");
   // printf("+удаляем картинку\n");
   if(mpcd->uname){
      free(mpcd->uname);
      // printf("+текст\n");
   }/* if */

   if(mpcd->nsuse != -1){
      /* по всем состояниям */
      // printf("+по всем состояниям\n");
      while(mpcd->pShead != NULL){
         tsh = mpcd->pShead->SHnext;
         /* по всем командам в этом состоянии */
         // printf("+-по всем командам в этом состоянии\n");
         while(mpcd->pShead->IChead != NULL){
            tic = mpcd->pShead->IChead->next;
            /* данные команды */
            if(mpcd->pShead->IChead->dc){
               if(mpcd->pShead->IChead->tc == Couttext_obj_num)
                  free(((Douttext_obj_num*)(mpcd->pShead->IChead->dc))->txt);
               free(mpcd->pShead->IChead->dc);
               // printf("+---free данные команды\n");
            }/* if */
            /* команда */
            free(mpcd->pShead->IChead);
            // printf("+--- free command\n");
            mpcd->pShead->IChead = tic;
            // printf("+---next command\n");
         }/* while */
         /* класс */
         free(mpcd->pShead);
         // printf("+-free state\n");
         mpcd->pShead = tsh;
         // printf("+-next state\n");
      }/* while */
   }/* if */
   else{
     mpcd->parent->nsuse--; /* уменьшаем количество использований состояний */
   }/* elif */
   free(mpcd);
   // printf("+free class\n");
   // printf("+out MPCFree\n");
   return Ok;
}/* MPCFree */
/* ----------------------------------------------------------------------- */
#pragma argsused
static short int execState(MPClass *tmpc, unsigned short int st, LPMP mp){
   SHead    *tsh;
   ICommand *tic;
   /* поиск нужного состояния */
   tsh = tmpc->pShead;
   while(tsh != NULL){
      if(tsh->nState == st){
         break;
      }/* if */
      tsh = tsh->SHnext;
	}/* while */
   /* нет нужного состояния */
   if(tsh == NULL){
      return No;
   }/* if */
   /* по всем командам в этом состоянии */
   tic = tsh->IChead;
   while(tic != NULL){
      /* выполняем команду интерфейса TBG */
      execCI(tmpc, tic->tc, tic->sized, tic->dc, st, mp);
      tic = tic->next;
   }/* while */
   return Ok;
}/* execState */
/* ----------------------------------------------------------------------- */
void setreper_obj_num(MPClass *tmpc, unsigned char obj, unsigned char num){
   short int ox = 0, oy = 0;
   if(tmpc){

   }/* if */
   return;
}/* setreper_obj_num */
/* ----------------------------------------------------------------------- */
#pragma argsused
static void execCI(MPClass *tmpc,TCommand tc, unsigned short int sd,
                   char *dc, unsigned short int st, LPMP mp){
/* карманы */
/*
   static unsigned short int color;
   static unsigned short int x1, y1, x2, y2;
   static char           *txt;
*/
   OBJECT **pic = tmpc->pic;
   // printf("...in execCI\n");
   switch(tc){
      case Coutpic_all:
            // printf("...Coutpic_all:\n");
            outpic_all(pic);
            print_error_tbg("in execCI Coutpic_all");
         break;
/*
      case Csetreper_obj_num:
		   { /* lockblock */
            Dsetreper_obj_num *d = (Dsetreper_obj_num*)dc;
            // printf("...Coutpic_obj_num:");
            setreper_obj_num(tmpc, d->obj, d->num);
            print_error_tbg("in execCI Csetreper_obj_num:");
         }/* lockblock */
         break;
*/
      case Coutpic_obj_num:
		   { /* lockblock */
            Doutpic_obj_num *d = (Doutpic_obj_num*)dc;
            // printf("...Coutpic_obj_num:");
            outpic_obj_num(pic, d->obj, d->num);
            print_error_tbg("in execCI Coutpic_obj_num:");
         }/* lockblock */
         break;
      case Coutpic_obj_num_with_rotate:
		   { /* lockblock */
            Doutpic_obj_num_with_rotate *d = (Doutpic_obj_num_with_rotate*)dc;
            // printf("...Coutpic_obj_num_with_rotate:");
            outpic_obj_num_with_rotate(pic, d->obj, d->num, d->angle);
            print_error_tbg("in execCI Coutpic_obj_num_with_rotate:");
         }/* lockblock */
         break;
      case Coutpic_obj:
		   { /* lockblock */
            Doutpic_obj *d = (Doutpic_obj*)dc;
            // printf("...Coutpic_obj:\n");
            outpic_obj(pic, d->obj);
            print_error_tbg("in execCI Coutpic_obj");
         }/* lockblock */
         break;
      case Coutpic_obj_num_color:
		   { /* lockblock */
            Doutpic_obj_num_color *d = (Doutpic_obj_num_color*)dc;
            //printf("...Coutpic_obj_num_color: obj:%d num:%d col:%d\n",
				       //d->obj, d->num, d->Color);
            outpic_obj_num_color(pic, d->obj, d->num, d->Color);
            print_error_tbg("in execCI Coutpic_obj_num_color:");
         }/* lockblock */
         break;
      case Coutpic_obj_color:
		   { /* lockblock */
            Doutpic_obj_color *d = (Doutpic_obj_color*)dc;
            //printf("...Coutpic_obj_color: obj:%d col:%d\n",
				       //d->obj, d->Color);
            outpic_obj_color(pic, d->obj, d->Color);
            print_error_tbg("in execCI Coutpic_obj_color");
         }/* lockblock */
         break;
      case Csetcolor_obj_num:
		   { /* lockblock */
            Dsetcolor_obj_num *d = (Dsetcolor_obj_num *)dc;
            // printf("...Csetcolor_obj_num:\n");
            setcolor_obj_num(pic, d->obj, d->num, d->Color, d->Mode);
            print_error_tbg("in execCI Csetcolor_obj_num");
		   } /* lockblock */
         break;
      case Csetcolor_obj:
		   { /* lockblock */
            Dsetcolor_obj *d = (Dsetcolor_obj *)dc;
            // printf("...Csetcolor_obj:\n");
            setcolor_obj(pic, d->obj, d->Color, d->Mode);
            print_error_tbg("in execCI Csetcolor_obj");
		   } /* lockblock */
         break;
      case Couttext_obj_num:
		   { /* lockblock */
            Douttext_obj_num *d = (Douttext_obj_num *)dc;
            // printf("...Couttext_obj_num:\n");
            outtext_obj_num(pic, d->obj, d->num, d->Colorbg, d->txt, d->Mode);
            print_error_tbg("in execCI Couttext_obj_num");
		   } /* lockblock */
         break;
      case Csetstate_obj_num:
		   { /* lockblock */
            Dsetstate_obj_num *d = (Dsetstate_obj_num *)dc;
            // printf("...Csetstate_obj_num:\n");
            setstate_obj_num(pic, d->obj, d->num, d->State, SHOW);
            print_error_tbg("in execCI Csetstate_obj_num");
		   } /* lockblock */
         break;
      case Csetstate_obj:
		   { /* lockblock */
            Dsetstate_obj *d = (Dsetstate_obj *)dc;
            // printf("...Csetstate_obj:\n");
            setstate_obj(pic, d->obj, d->State, SHOW);
            print_error_tbg("in execCI Csetstate_obj");
		   } /* lockblock */
         break;
      case CexecState:
		   { /* lockblock */
            DexecState *d = (DexecState *)dc;
            // printf("..>CexecState:");
            execState(tmpc, d->st, mp);
            // printf("..>%d\n",d->st);
		   } /* lockblock */
         break;
      case Csavestate:
		   { /* lockblock */
            mp->ss = st;
		   } /* lockblock */
         break;
   }/* switch */
   // printf("...out execCI\n");
   return;
}/* execCI */
/* ----------------------------------------------------------------------- */
short int MPCDraw (MPClass *tmpc, unsigned int sx, unsigned int sy,
             unsigned short int st, char *ud, LPMP mp){
   SHead    *tsh;
   ICommand *tic;
   short int ox = 0, oy = 0;


   if(tmpc == NULL)
      return Ok;

   if(strstr(strupr(tmpc->fname), ".TBG") == NULL){
      return Ok;
   }/* if */

   /* поиск нужного состояния */
   // printf("\n ..поиск нужного состояния: %d:%d\n", tmpc->index, st);
   tsh = tmpc->pShead;
   while(tsh != NULL){
      if(tsh->nState == st){
         // printf(" [%d] found\n", tsh->nState);
         break;
      }/* if */
      // printf(",%d", tsh->nState);
      tsh = tsh->SHnext;
	}/* while */
   /* нет нужного состояния */
   if(tsh == NULL){
      // printf("\n нет нужного состояния: %d\n", st);
      return No;
   }/* if */
   // printf(" есть нужное состояние: %d\n", tsh->nState);

   /* смещаем картинку куда надо */
   // printf("..смещаем картинку куда надо\n");

   // get coord reper: 0,0
   getsize_obj_num_reset();
   getsize_obj_num(tmpc->pic, 0, 0, &ox, &oy, &ox, &oy);

   offset_pic(tmpc->pic, sx - ox, sy - oy);
#ifdef __WIN32__
   /* Устанавливаем дисплейный контекст */
   // printf("..устанавливаем дисплейный контекст.\n");
   TBGtoDC(tmpc->pic, tmpc->hdc);
#endif
   /* выполняем пользовательскую функцию */
   // printf("..выполняем пользовательскую функцию\n");
   if(tmpc->cb){
      tmpc->cb(tmpc, st, tsh, ud);
   }/* if */

   /* по всем командам в этом состоянии */
   //printf("\n..по всем командам в этом состоянии\n");
   tic = tsh->IChead;
   while(tic != NULL){
      /* выполняем команду интерфейса TBG */
      // printf("..выполняем команду интерфейса TBG: %d\n", tic->tc);
      execCI(tmpc, tic->tc, tic->sized, tic->dc, tsh->nState, mp);
      tic = tic->next;
   }/* while */

   return Ok;
}/* MPCDraw */
/* ----------------------------------------------------------------------- */
short int MPCHookCB (unsigned short int index, short int (*cb)(MPClass *pmpc, unsigned short int st, SHead *sh, char *ud)){
   MPClass  *tmpc = MPCLoad(index);
   if(tmpc){
      tmpc->cb = cb;
      tmpc->nuse--; /* не увеличивать количество использований (bug) */
   }/* if */
   return Ok;
}/* MPCHookCB */
/* ----------------------------------------------------------------------- */
short int MPCSetPath(char *path){
   MPath = path;
   return Ok;
}/* MPCSetPath */
/* ----------------------------------------------------------------------- */
//#include ".\src\mpcload.c"
MPClass *MPCLoad(unsigned short int index){
   TBG_HEADER ltbg_qload;
   LPTBG_LIST lqpic;

   MPClass  *tmpc = NULL;
   MPClass  *ampc = NULL; // new MPClass
   SHead    *tsh  = NULL;
   ICommand *ic   = NULL;
   struct yy_save_state* ss;
   char s[256];
   short int res = 0;
   int Color = 0, Mode = 0, numln, *oldpnumln = NULL;

   FILE *yyinput = NULL;
   YY_BUFFER_STATE yy_local_buffer = NULL;
   YY_BUFFER_STATE yy_save_buffer = yy_current_buffer;

//printf("\n\n.in MPCLoad");
   /* проверка указателя */
   tmpc = mpc;
   res = heapchecknode(tmpc);
   /* поиск загруженного класса */
   if(res == _USEDENTRY){
//printf("\n.search index: %d", index);
      while(tmpc != NULL){
         if(tmpc->index == index){
            /* загружен */
//printf("\n.[%d] %p %p <- found\n.return\n", tmpc->index, tmpc, tmpc->next);
            tmpc->nuse++;
            return tmpc;
         }/* if */
//printf("\n.[%d] %p %p", tmpc->index, tmpc, tmpc->next);
         if((res = heapchecknode(tmpc->next)) == _USEDENTRY)
            tmpc = tmpc->next;
         else{
            assert(res != _HEAPCORRUPT);
            tmpc = NULL;
         }/* elif */
	   }/* while */
   }/* if */
   else{
//printf("\n.list not found");
   }/* elif */
//printf("\n.index not found");

  /* попытка загрузки класса */
   memset(s, 0, sizeof(s));
   memset(&ltbg_qload, 0, sizeof(ltbg_qload));

   sprintf(s, "%s\\MACROTBM.TXT", MPath);

   yyin = fopen(s, "rb");
   if(yyin == NULL){
      return NULL;
	}/* if */

   yyinput = yyin;
   oldpnumln = pnumln;
   pnumln = &numln;
   (*pnumln) = 1;
   m = aSTART;

//printf("\n.1yy_current_buffer: %p", yy_current_buffer);
   yyrestart( yyinput );
   yy_local_buffer = yy_current_buffer;
//printf("\n.2yy_current_buffer: %p", yy_current_buffer);
//printf("\n. yy_local_buffer  : %p", yy_local_buffer);

//printf("\n.start loading: %d", index);
   while(m != aSTOP){
      switch(ASTEP()){
         case aMACRO:
//printf("\n .aMACRO");
            /* Выделяем память под класс макропримитива */
            ampc = (MPClass*)calloc(1, sizeof(MPClass));
            if(ampc == NULL){
               yy_delete_buffer(yy_current_buffer);
               fclose(yyinput);

               return NULL;
            }/* if */
            break;
         case aNUMMACRO:
//printf("\n .aNUMMACRO searching in file MACROTBM.TXT");
            while((int)vali != index){
//printf("\n  .%d", vali);
               while(m != aSTOP){ // тянем до следующего описания макро
                 ASTEP();
                 if((m == aEOFSTOP) || (m == aERRORSTOP)){
//printf("\n  .STOP or ERROR");
                    free(ampc);
                    yy_delete_buffer(yy_current_buffer);
                    fclose(yyinput);

                    return NULL;
                 }/* if */
               }/* while */
               m = aMACRO;
               while(m != aNUMMACRO){ // пропускаем разделитель
                 ASTEP();
                 if((m == aEOFSTOP) || (m == aERRORSTOP)){
//printf("\n  .STOP or ERROR");
                    free(ampc);
                    yy_delete_buffer(yy_current_buffer);
                    fclose(yyinput);

                    return NULL;
                 }/* if */
               }/* while */
            }/* while */
//printf("\n  .%d <- found!", vali);
            ampc->index = (int)vali;
            break;
         case aTXTDESC:
//printf("\n.aTXTDESC");
            if((ampc->uname = (char*)malloc(strlen(buftxt) + 1)) != NULL){
               memset(ampc->uname, 0, strlen(buftxt) + 1);
         	   strcpy(ampc->uname, buftxt);
//printf(" \"%s\"", buftxt);
            }/* if */
            break;
         case aTXTPICT:
            {
//printf("\n.aTXTPICT");
//yy_print_state();
               memset(s, 0, sizeof(s));
               strcpy(ampc->fname, buftxt);
               sprintf(s, "%s\\%s", MPath, ampc->fname);
               lqpic = calloc(1, sizeof(TBG_LIST));
               if(lqpic == NULL){
                  if(ampc->uname)
                     free(ampc->uname);
                  free(ampc);
                  yy_delete_buffer(yy_local_buffer);
                  yy_current_buffer = yy_save_buffer;
                  fclose(yyinput);
               }/* if */
//printf(" \"%s\"", buftxt);
               lqpic->ToMPClass = ampc;
               ladd_tail((LPGENERIC_HEADER)&ltbg_qload, (LPGENERIC_LIST)lqpic);
//printf("\n. <%s>", lqpic->ToMPClass->fname);

/*
printf("\n.load file TBG: %s", s);
               yy_current_buffer = NULL;
               if(load_file(&(ampc->pic), s) != 0){
                  char b[256];
                  sprintf(b, "load_file <%s> in MPCLoad", s);
                  print_error_tbg(b);
                  if(ampc->uname)
                     free(ampc->uname);
                  free(ampc);
                  yy_delete_buffer(yy_local_buffer);
                  yy_current_buffer = yy_save_buffer;
                  fclose(yyinput);
                  return NULL;
               }/* if */

               yy_current_buffer = yy_local_buffer;
//printf("\n.restore state %p", ss);
//               yy_restore(ss);
//printf("\n. after load_file: %p", yy_current_buffer);
//yy_print_state();
*/
            }
            break;
         case aSTATEFROM:
            break;
         case aNUMSTATEFROM:
//printf("\n.aNUMSTATEFROM %d", index);
            {
               MPClass *parent = mpc;
               /* проверка указателя */
               res = heapchecknode(parent);
               if(res == _USEDENTRY){
                  /* поиск загруженного класса */
//printf("\n .search parent class in mem.");
                  while(parent != NULL){
                     if(parent->index == (int)vali){
                        /* загружен */
                        break;
                     }/* if */
//printf("\n  .%d", parent->index);
                     if((res = heapchecknode(parent->next)) == _USEDENTRY)
                        parent = parent->next;
                     else{
                        assert(res != _HEAPCORRUPT);
                        parent = NULL;
                     }/* elif */
                  }/* while */
               }/* if */
               if(parent == NULL){
//                  AMA      tm;
                  int tcurcomm;
                  int tcurcommarg;
//printf("\n  .not found!");
                  /* класс отсутствует в памяти */
                  /* загрузить класс */
//                  tm = m;
                  tcurcomm = curcomm;
                  tcurcommarg = curcommarg;
//printf("\n  .call MPCLoad: %d", vali);
                  yy_current_buffer = NULL;
                  parent = MPCLoad((int)vali);
                  if(parent == NULL){
//printf("\n   .not load MP");
                     if(ampc->uname)
                        free(ampc->uname);
                     free(ampc);
                     yy_delete_buffer(yy_current_buffer);
                     yy_current_buffer = yy_local_buffer;
                     fclose(yyinput);

                     return NULL;
                  }/* if */
//printf("\n   .load parent class OK");
//                  m = tm;
                  curcomm = tcurcomm;
                  curcommarg = tcurcommarg;
                  yy_switch_to_buffer(yy_local_buffer);
               }/* if */
               else{
//printf("\n  .%d <- found!", parent->index);
               }/* elif */
//printf("\n  .setup parent class");
               /* нельзя наследовать унаследованное ! (будет исправлено) */
               if(parent->nsuse == -1){
                  print_error_tbg("нельзя наследовать унаследованное");
                  fclose(yyinput);
                  if(ampc->uname)
                     free(ampc->uname);
                  free(ampc);
                  yy_delete_buffer(yy_current_buffer);

                  return NULL;
               }/* if */
               /* используем чужие состояния */
               ampc->parent = parent;
               ampc->parent->nsuse++;
               ampc->nsuse = -1;
               ampc->pShead = ampc->parent->pShead;
            }
            break;
         case aSTATE:
//printf("\n.aSTATE");
            tsh = ampc->pShead;
            ampc->pShead = (SHead*)calloc(1, sizeof(SHead));
            assert(ampc->pShead != NULL);
            ampc->pShead->SHnext = tsh;
            tsh = ampc->pShead;
            break;
         case aNUMSTATE:
//printf("\n.aNUMSTATE");
            tsh->nState = (int)vali;
//printf(" %d", tsh->nState);
            break;
         case aCOM:
//printf("\n.aCOM");
            /* выделяем память под команду */
            if(tsh->IChead != NULL){ /* не первая команда */
               ic->next = (ICommand*)calloc(1, sizeof(ICommand));
               ic = ic->next;
            }/* if */
            else{
               tsh->IChead = ic = (ICommand*)calloc(1, sizeof(ICommand));
            }/* if */
            assert(ic != NULL);
            ic->tc = commands[curcomm].id;
            ic->sized = commands[curcomm].sized;
            /* память под данные команды */
            if(commands[curcomm].sized > 0){
               ic->dc = (char*)calloc(1U, commands[curcomm].sized);
               assert(ic->dc != NULL);
            }/* if */
//printf(" %s", commands[curcomm].str);
            break;
         case aARGCOMTXT:
//printf("\n .aARGCOMTXT");
            if(commands[curcomm].id == Couttext_obj_num){
               Douttext_obj_num *td = (Douttext_obj_num*)ic->dc;
               if((td->txt = (char*)calloc(1U, strlen(buftxt) + 1)) != NULL){
                  strcpy(td->txt, buftxt);
//printf(" %s", buftxt);
               }/* if */
               else{
                  fprintf(mperr, "\nСтрока %d, нет памяти.\n", (*pnumln));
               }/* elif */
            }/* if */
            break;
         case aARGCOMFLT:
//printf("\n .aARGCOMFLT");
            switch(commands[curcomm].id){
               case Coutpic_obj_num_with_rotate:
                  ((Doutpic_obj_num_with_rotate*)(ic->dc))->angle = (double)vald;
                  break;
            }/* switch */
//printf(" %lf", vald);
            break;
         case aARGCOMNUM:
//printf("\n .aARGCOMNUM");
            switch(commands[curcomm].id){
               case Csetstate_obj_num:
                  ((Dsetstate_obj_num*)(ic->dc))->State = (int)vali;
                  break;
               case Csetstate_obj:
                  ((Dsetstate_obj*)(ic->dc))->State = (int)vali;
                  break;
               case CexecState:
                  ((DexecState*)(ic->dc))->st = (int)vali;
                  break;
            }/* switch */
//printf(" %d", vali);
            break;
         case aARGCOMNUMOBJ:
//printf("\n .aARGCOMNUMOBJ");
            switch(commands[curcomm].id){
               case Coutpic_obj:
                  ((Doutpic_obj*)(ic->dc))->obj = (int)vali;
                  break;
               case Coutpic_obj_num:
                  ((Doutpic_obj_num*)(ic->dc))->obj = (int)vali;
                  break;
               case Coutpic_obj_num_with_rotate:
                  ((Doutpic_obj_num_with_rotate*)(ic->dc))->obj = (int)vali;
                  break;
               case Coutpic_obj_color:
                  ((Doutpic_obj_color*)(ic->dc))->obj = (int)vali;
                  break;
               case Coutpic_obj_num_color:
                  ((Doutpic_obj_num_color*)(ic->dc))->obj = (int)vali;
                  break;
               case Csetcolor_obj:
                  ((Dsetcolor_obj*)(ic->dc))->obj = (int)vali;
                  break;
               case Csetcolor_obj_num:
                  ((Dsetcolor_obj_num*)(ic->dc))->obj = (int)vali;
                  break;
               case Csetstate_obj_num:
                  ((Dsetstate_obj_num*)(ic->dc))->obj = (int)vali;
                  break;
               case Csetstate_obj:
                  ((Dsetstate_obj*)(ic->dc))->obj = (int)vali;
                  break;
               case Couttext_obj_num:
                  ((Douttext_obj_num*)(ic->dc))->obj = (int)vali;
                  break;
            }/* switch */
//printf(" %d", vali);
            break;
         case aARGCOMNUMELM:
//printf("\n .aARGCOMNUMELM");
            switch(commands[curcomm].id){
               case Coutpic_obj_num:
                  ((Doutpic_obj_num*)(ic->dc))->num = (int)vali;
                  break;
               case Coutpic_obj_num_with_rotate:
                  ((Doutpic_obj_num_with_rotate*)(ic->dc))->num = (int)vali;
                  break;
               case Coutpic_obj_num_color:
                  ((Doutpic_obj_num_color*)(ic->dc))->num = (int)vali;
                  break;
               case Csetcolor_obj_num:
                  ((Dsetcolor_obj_num*)(ic->dc))->num = (int)vali;
                  break;
               case Csetstate_obj_num:
                  ((Dsetstate_obj_num*)(ic->dc))->num = (int)vali;
                  break;
               case Couttext_obj_num:
                  ((Douttext_obj_num*)(ic->dc))->num = (int)vali;
                  break;
            }/* switch */
//printf(" %d", vali);
            break;
         case aARGCOMCOLOR:
//printf("\n .aARGCOMCOLOR");
            if((Color = fkey(buftxt, key_colors, sizeof(key_colors)/sizeof(KEY))) != -1){
               switch(commands[curcomm].id){
                  case Coutpic_obj_color:
                     ((Doutpic_obj_color*)(ic->dc))->Color = Color;
                     break;
                  case Coutpic_obj_num_color:
                     ((Doutpic_obj_num_color*)(ic->dc))->Color = Color;
                     break;
                  case Csetcolor_obj:
                     ((Dsetcolor_obj*)(ic->dc))->Color = Color;
                     break;
                  case Csetcolor_obj_num:
                     ((Dsetcolor_obj_num*)(ic->dc))->Color = Color;
                     break;
                  case Couttext_obj_num:
                     ((Douttext_obj_num*)(ic->dc))->Colorbg = Color;
                     break;
               }/* switch */
//printf(" %s", buftxt);
            }/* if */
            else{
               fprintf(mperr, "\nСтрока %d, аргумент >%s< не ЦВЕТ.\n", (*pnumln), buftxt);
               if(ampc->uname){
                  free(ampc->uname);
               }/* if */
               if(ampc->pic){
                  del_pic(&(ampc->pic));
               }/* if */
               free(ampc);
               yy_delete_buffer(yy_current_buffer);
               fclose(yyinput);

               return NULL;
            }/* elif */
            break;
         case aARGCOMMODE:
//printf("\n .aARGCOMMODE");
            if((Mode = fkey(buftxt, key_modes, sizeof(key_modes)/sizeof(KEY))) != -1){
               switch(commands[curcomm].id){
                  case Csetcolor_obj:
                     ((Dsetcolor_obj*)(ic->dc))->Mode = Mode;
                     break;
                  case Csetcolor_obj_num:
                     ((Dsetcolor_obj_num*)(ic->dc))->Mode = Mode;
                     break;
                  case Couttext_obj_num:
                     ((Douttext_obj_num*)(ic->dc))->Mode = Mode;
                     break;
               }/* switch */
//printf(" %s", buftxt);
            }/* if */
            else{
               fprintf(mperr, "\nСтрока %d, аргумент >%s< не РЕЖИМ.\n", (*pnumln), buftxt);
               if(ampc->uname){
                  free(ampc->uname);
               }/* if */
               if(ampc->pic){
                  del_pic(&(ampc->pic));
               }/* if */
               free(ampc);
               yy_delete_buffer(yy_current_buffer);
               fclose(yyinput);

               return NULL;
            }/* elif */
            break;
         case aERRORSTOP:
//printf("\n.aERRORSTOP");
            if(ampc->uname){
               free(ampc->uname);
            }/* if */
            if(ampc->pic){
               del_pic(&(ampc->pic));
            }/* if */
            free(ampc);
            yy_delete_buffer(yy_current_buffer);
            fclose(yyinput);

            return NULL;
      }/* switch */
   }/* while */
   yy_delete_buffer(yy_current_buffer);

   fclose(yyinput);
   pnumln = oldpnumln;

   lqpic = ldel_head((LPGENERIC_HEADER)&ltbg_qload);
//printf("\n.load all tbg's %p", lqpic);
   while(lqpic){
//printf("\n. <%s> to: %p [%d]", lqpic->ToMPClass->fname, lqpic->ToMPClass, lqpic->ToMPClass->index);
      sprintf(s, "%s\\%s", MPath, lqpic->ToMPClass->fname);
      if(load_file(&(lqpic->ToMPClass->pic), s) != 0){
         char b[256];
         sprintf(b, "load_file <%s> in MPCLoad", s);
         print_error_tbg(b);

         return NULL;
      }/* if */
//printf("\n load <%s>: OK", lqpic->ToMPClass->fname);
      free(lqpic);
      lqpic = ldel_head((LPGENERIC_HEADER)&ltbg_qload);
   }/* while */
//printf("\n.load all tbg's: OK");

//printf("\n.stop loading: %d", index);

   ampc->nuse++;
   ampc->next = mpc;

   mpc = ampc;

/*
printf("\n.before MPCDraw:");
      MPCDraw (mpc, 0, 0, 1, 0, 0);
printf("\n.after MPCDraw:");
*/
/*
   printf("\n.list all MPC:");
   tmpc = mpc;
   while(tmpc){
      printf("\n. %d <%s>:%p", tmpc->index, tmpc->fname, tmpc->pic);
      tmpc = tmpc->next;
   }/* while */
//   printf("\n.end list all MPC");
*/
//printf("\n.return from MPCLoad\n");


   return mpc;

}/* MPCLoad */

/* ----------------------------------------------------------------------- */
