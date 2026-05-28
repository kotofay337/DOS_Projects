/* tbg_tc.h -----------------------------------------------------------------

                    Заголовочный файл библиотеки
                Функции интерфейса C, C++ с TBG-файлами
                          /10-AUG-98/

                  Графический пакет "ТурБоГрафика"
                     (C) Павельев П.И. 1990-95
                     (C) Чумак А. И. 1992-2001

                 Компилятор Borland C++ 2.0 (3.01)
---------------------------------------------------------------------------*/
/*
+--------------------------------------------------------------------------+
¦                           Предупреждение !!!                             ¦
¦                                                                          ¦
¦             При компиляции должна быть отключена опция:                  ¦
¦          Word alignment, т.е. данные выравниваются до байта.             ¦
+--------------------------------------------------------------------------+
*/

#ifndef TBG_TC_H /* однократное включение файла */
#define TBG_TC_H

/* Общие включаемые файлы */
#include <stdio.h>
#include <dos.h>
#include <dir.h>

#ifdef __WIN32__
#include <windows.h>

#ifdef __DLL__
#define DLL_EI __declspec(dllexport)
#else
#define DLL_EI __declspec(dllimport)
#endif

#else
#define DLL_EI
#endif

/* Структура данного файла: */

/* Определение макросов, перечислимых типов и типов данных */
/* Размещение функций и внешних переменных по файлам */
/* Структура файла .TBG v6.00 */

/*---------------------------------------------------------------------------
      Определение макросов, перечислимых типов и типов данных
---------------------------------------------------------------------------*/
typedef void OBJECT;

//-- Maximum fonts ----------------------------------------------------------
#define MaxFONT 20
//---------------------------------------------------------------------------
typedef
   struct {
      unsigned char x;
      unsigned char y;
   } SIZESYMBOL, *LPSIZESYMBOL;
//---------------------------------------------------------------------------
struct TextSettingsType {
    short int font;
    short int direction;
    short int charsize;
    short int horiz;
    short int vert;

    // it's my custom data
    double ScaleX;
    double ScaleY;
    double UserScaleX;
    double UserScaleY;
};
#if !defined(__GRAPHICS_H)
#define __GRAPHICS_H
// - TextDirection ----------------------------------------------------------
enum text_dir {
   HORIZ_DIR, /* _RIGHT */
   VERT_DIR,  /* _UP */
   HORIZ_DIR_LEFT,
   VERT_DIR_DOWN
};
// - TextJustification -------------------------------------------------------
enum text_just {        /* Horizontal and vertical justification
                   for settextjustify */
    LEFT_TEXT   = 0,
    CENTER_TEXT = 1,
    RIGHT_TEXT  = 2,

    BOTTOM_TEXT = 0,
 /* CENTER_TEXT = 1,  already defined above */
    TOP_TEXT    = 2
};
// - Fonts++ ----------------------------------------------------------------
enum ext_font_name {
    DEFAULT_FONT    = 0,    /* 8x8 bit mapped font */
    TRIPLEX_FONT    = 1,    /* "Stroked" fonts */
    SMALL_FONT  = 2,
    SANS_SERIF_FONT = 3,
    GOTHIC_FONT = 4,
    SCRIPT_FONT = 5,
    SIMPLEX_FONT = 6,
    TRIPLEX_SCR_FONT = 7,
    COMPLEX_FONT = 8,
    EUROPEAN_FONT = 9,
    BOLD_FONT = 10
};

enum line_styles {      /* Line styles for get/setlinestyle */
    SOLID_LINE   = 0,
    DOTTED_LINE  = 1,
    CENTER_LINE  = 2,
    DASHED_LINE  = 3,
    USERBIT_LINE = 4,   /* User defined line style */
};

enum line_widths {      /* Line widths for get/setlinestyle */
    NORM_WIDTH  = 1,
    THICK_WIDTH = 3,
};

enum fill_patterns {        /* Fill patterns for get/setfillstyle */
    EMPTY_FILL,     /* fills area in background color */
    SOLID_FILL,     /* fills area in solid fill color */
    LINE_FILL,      /* --- fill */
    LTSLASH_FILL,       /* /// fill */
    SLASH_FILL,     /* /// fill with thick lines */
    BKSLASH_FILL,       /* \\\ fill with thick lines */
    LTBKSLASH_FILL,     /* \\\ fill */
    HATCH_FILL,     /* light hatch fill */
    XHATCH_FILL,        /* heavy cross hatch fill */
    INTERLEAVE_FILL,    /* interleaving line fill */
    WIDE_DOT_FILL,      /* Widely spaced dot fill */
    CLOSE_DOT_FILL,     /* Closely spaced dot fill */
    USER_FILL       /* user defined fill */
};
#endif
// - Ќ з «® ®ЇpҐ¤Ґ«Ґ­Ёп бвpгЄвгp ¤ ­­ле -------------------------------------
typedef
   struct HBGIFONTFILE{
      unsigned short int  HeaderSize;
      unsigned char FontName[4];
      unsigned short int  DataSize;
      unsigned char MjVersion;
      unsigned char MnVersion;
      unsigned char MinMjVersion;
      unsigned char MinMiVersion;
   }HBGIFONTFILE, *LPHBGIFONTFILE;
//---------------------------------------------------------------------------
typedef
   struct IBGIFONT{
      unsigned char Signature;
      unsigned short int NumChar_inFont;
      unsigned char undefined_00;
      unsigned char firstASCII;
      unsigned short int OffsetStrDef;
      unsigned char Scanflag;
          char DisttoTopCap;
          char DisttoBaseline;
          char DisttoBottomDescender;
          char undefined_01[5];
   }IBGIFONT, *LPIBGIFONT;
//---------------------------------------------------------------------------
typedef
   struct BGIFONT{
      unsigned char      *pFilefont;
      LPHBGIFONTFILE     pFHdr;
      LPIBGIFONT         pIFnt;
      unsigned short int *pOffsSD;
      unsigned char      *pWidth;
      SIZESYMBOL         ss[256];
#ifdef __WIN32__
      HDC                hdc;
#endif
   }BGIFONT, *LPBGIFONT;
//---------------------------------------------------------------------------
/* Not Change !!! */
#define FONTBGI      30000
#define _DEFAULT_CHR 30001
#define _SIMP_CHR    30002
#define _TRIP_CHR    30003
#define _LITT_CHR    30004
#define _SANS_CHR    30005
#define _GOTH_CHR    30006
#define _SCRI_CHR    30007
#define _TSCR_CHR    30008
#define _LCOM_CHR    30009
#define _EURO_CHR    30010
#define _BOLD_CHR    30011
/* Not Change !!! */
//---------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
void   DLL_EI     SetPathToBGIFont(char* p);
void   DLL_EI     GetTextSettings(struct TextSettingsType *__texttypeinfo);
short int DLL_EI  InstallUserFont(const unsigned char *__name);
void    DLL_EI    OutText(const unsigned char *__textstring);
void    DLL_EI    OutTextXY(int __x, int __y, const unsigned char *__textstring);
void    DLL_EI    SetTextJustify(int __horiz, int __vert);
void    DLL_EI    SetTextStyle(int __font, int __direction, int __charsize);
int     DLL_EI    RegisterBGIFont(int i);
int     DLL_EI    UnRegisterBGIFont(int i);
void    DLL_EI    SetUserCharSize(short int __multx, short int __divx,
                          short int __multy, short int __divy);
short int DLL_EI  TextHeight(const unsigned char *__textstring);
short int DLL_EI  TextWidth(const unsigned char *__textstring);

SIZESYMBOL CalculateSizeSymbol(unsigned char c);

void DLL_EI  UnloadAllFont(void);
#ifdef __WIN32__
HDC DLL_EI  SetHDCFont(HDC hdc);
#endif
#ifdef __cplusplus
} /* #extern "C" */
#endif
//---------------------------------------------------------------------------

/*------------------------------------------------------------------------ */
/* Макроопределения и подстановки                                          */
/*------------------------------------------------------------------------ */
#if !defined(__COLORS)
#define __COLORS
enum COLORS {
    BLACK = 0,          /* dark colors */
    BLUE  = 1,
    GREEN = 2,
    CYAN = 3,
    RED  = 4,
    GOLD = 5,
    BOLOTO = 6,
    LIGHTGRAY = 7,
    DARKGRAY  = 8,      /* light colors */
    LIGHTBLUE = 9,
    LIGHTGREEN = 10,
    LIGHTCYAN  = 11,
    LIGHTRED   = 12,
    PASTEL = 13,
    YELLOW = 14,
    WHITE  = 15
};
#endif

#ifdef __WIN32__

#define MyPALSIZE 16

typedef
   struct tagEGATools {
      enum COLORS c;
      COLORREF    rgb;
      HPEN        hPen[5];   /*  */
      HBRUSH      hBrush[11]; /*  */
   }EGATools;

extern EGATools cEGA[MyPALSIZE];

#ifdef __cplusplus
extern "C" {
#endif
int DLL_EI  CreateEGATools(void);
int DLL_EI  DeleteEGATools(void);

void DLL_EI  Line(HDC hdc, int left, int top, int right, int bottom);
void DLL_EI  Bar(HDC hdc, int left, int top, int right, int bottom);
void DLL_EI  Rect(HDC hdc, int left, int top, int right, int bottom);
#ifdef __cplusplus
} /* #extern "C" */
#endif

extern HPALETTE hPal;
extern LPLOGPALETTE lpLogPal;

/* Присваивание картинке
   дислейного контекста для вывода( для WINDOWS )
   Вход: указатель картинки, дисплейный контекст */
#ifdef __cplusplus
extern "C" {
#endif
HDC  DLL_EI TBGtoDC(OBJECT **p, HDC hDC);

HDC   DLL_EI TBG_SetDC (OBJECT **obj, HDC hdc);
HDC   DLL_EI TBG_GetDC (OBJECT **obj);

HWND  DLL_EI TBG_SetWND(OBJECT **obj, HWND hwnd);
HWND  DLL_EI TBG_GetWND(OBJECT **obj);

BOOL  DLL_EI TBG_GetUpdateState(OBJECT **p);
BOOL  DLL_EI TBG_UpdateOK(OBJECT **p);
#ifdef __cplusplus
} /* #extern "C" */
#endif

#endif /* __WIN32__ */
/* Определение макросов */
/* инициализация вращения obj-num */
#define init_rotate_obj_num(p,o,n,a) outpic_obj_num((p),(o),(n)+2);\
                                     setwritemode(XOR_PUT);\
                                     outpic_obj_num((p),(o),(n)+2);\
                                     setwritemode(COPY_PUT);\
                                     rotate_obj_num((p),(o),(n),(a));

// макрос вращения точки
#define rp(x1x,y1y,a1a,X1C,Y1C) {double xd=x1x,yd=y1y;\
                                 rotate_point(a1a,&xd,&yd,X1C,Y1C);\
                                 x1x=(short int)xd;\
                                 y1y=(short int)yd;\
                                }

// Просто полезная штучка
#define hTbg(x) ((struct HTBG*)*(x))

#define SIZE_XY         32760 //максимальный размер координат
#define max_copyr       15    //размер "Copyright"
#define Offset_to_TBL   16    //смещение до заголовка библиотекаря

#define ON              1
#define OFF             0

#define OBJ_NUM 0xFFFF
#define OBJ     0xFF00

#define No   0
#define Ok  1 // успех
#define Fu  0 // Фу,плохо !
#define Hide 2

#define SIGNTBG 'AL'

#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#define Hide 2

/* показывать изменение установленного параметра элемента */
/* для outtext_obj_num:показывать текст в любом случае */
#define SHOW    5

/* не показывать изменение установленного параметра */
#define NO_SHOW 1


/* тип линии для прямоугольника габарита */
#define GABARIT_LINE 5

/* Обмен */
#define swap(a,b) {(a)^=(b),(b)^=(a),(a)^=(b);}


/* Определение перечислимых типов */

enum
{/* коды ошибок функций интерфейса с TBG файлами */
 ER_OK,
 ER_NOTPIC,      /* в памяти нет картинки */
 ER_NOTLE,       /* нет логических элементов */
 ER_NOTOBJ,      /* нет объекта */
 ER_NOTELEMOBJ,  /* нет элемента объекта */
 ER_NOTTEXT,     /* элемент объекта не текст */
 ER_NOTDEL,      /* нечего удалять */
 ER_CLOSELIB,    /* cначала закройте библиотеку */
 ER_OPENLIB,     /* не открывается библиотека */
 ER_MEMDIR,      /* нет памяти для каталога библиотеки */
 ER_SETLIB,      /* файл библиотеки не установлен */
 ER_HIDELIB,     /* уже скрыта библиотека */
 ER_NOTHIDELIB,  /* не скрыта библиотека */

 ER_STARTNUM,    /* стартовый номер в getpos_mouse_obj > max */
 ER_INDEX_0,     /* индекс в outpic_pn <= 0 */
 ER_INDEX_MAX,   /* индекс в outpic_pn > count_ei */
 ER_BAD_TYPE_GP, /* неизвестный тип графического примитива */

 ER_WRITE,       /* ошибка при чтении */
 ER_MEM,         /* мало памяти */
 ER_ALIGNMENT,   /* выравнивание при компиляции - до байта */
 ER_BADTYPEFILE, /* неизвестный тип файла */

 ER_MEMNOTFULL,  /* память занята,удалите картинку */
 ER_NOTFOPEN,    /* не удалось открыть файл */
 ER_TBLNOTFILE,  /* в библиотеке нет файла  */
 ER_NOMACRO,     /* ГЭ не макропримитив */
 ER_UDALREADY,   /* повтор установки данных пользователя */
 ER_LOADBGIFONT, /* ошибка загрузки BGI шрифта */
 ER_OPERBGIFONT, /* ошибка работы с BGI шрифтом */
 ER_NODEFAULTBGI,/* нет шрифта по умолчанию SIMP.CHR,\nдолжен быть в текущем каталоге\nили в ресурсах */
 ER_
 /* коды ошибок функций интерфейса с TBG файлами */
};

/* Определение типов данных */

struct ITEM{
   char baFName[13];
   unsigned long ulOffset;
};

struct HTBL{      // Заголовок для элемента библиотеки TBL.
    char name[15];// имя файла картинки
    long len;     // длина файла картинки в байтах
/*
    struct date dat;
    struct time time;
*/
    long z1;     // zzz
    long z2;     // zzz
};// HTBL

struct HTBG_WR{   // для записи-чтения заголовка tbg-файла
 char   copyr[max_copyr];
 struct HTBL sr1;
 char   ir2[35];
 short int Num_LE;      // количество логических элементов
 short int count_ei;    // количество эл-тов изображения
};// HTBG_WR

struct HTBG{      // для tbg-файла после загрузки
 char tip_file;   // тип файла картинки
 short int Num_LE;      // количество логических элементов
 short int  count_ei;   // число элементов картинки (coiunt_ei-1)
 char tip_txt;    // тип элемента текст
 struct tag_Logic *Logic;
 char PicName[9];
 short int  ident;
 int ofsx, ofsy; // смещение картинки в окне вывода
 int SIZEX, SIZEY; // размер окна вывода
#ifdef __WIN32__
 HDC hdc;
 HWND hwnd;
 BOOL NeedUpdateFlag; // TRUE: Картинка была изменена - необходимо обновление
#endif
};// HTBG

#ifndef __WIN32__
typedef
   struct tag_POINT {
      short int x, y;
   } POINT, *LPPOINT;
#endif

typedef
   struct tag_PIPELINE {
      short int xs, ys, xe, ye; // Coordinates pipeline
      short int DO, // Delta Offset
                 O, // Current Offset
                 S, // Delta Step
                 C1,// Color 1
                 C2;// Color 2
   } PIPELINE, *LPPIPELINE;
/* ----------------------------------------------------------------------- */
struct tag_Logic
    {
       unsigned char  num;     // Номер элемента
       unsigned char  numobj;  //       объекта
       short int      Index_ei;// Индекс в массиве ГЭ
    };// tag_Logic

struct ln{
      char tipobj;   /* тип ГЭ */
      char lnstl; /* стиль линии */
      char lnthi; /* толщина линии */
      char lncol; /* цвет линии */
      short int  lnx;   /* x  */
      short int  lny;   /* y  */
      short int  lndx;  /* dx */
      short int  lndy;  /* dy */
      char   col; /* цвет вывода */
    };/* линия */

struct rc{
      char tipobj;   /* тип ГЭ */
           char rcstl; /* стиль линии */
      char rcthi; /* толщина линии */
      char rccol; /* цвет линии */
      short int  rcx;   /* x  */
      short int  rcy;   /* y  */
      short int  rcdx;  /* dx */
      short int  rcdy;  /* dy */
      char   col; /* цвет вывода */
    };/* прямоугольник */

struct br{
      char tipobj;   /* тип ГЭ */
           char brstl; /* стиль закраски */
      char brcol; /* цвет закраски */
      short int  brx;   /* x  */
      short int  bry;   /* y  */
      short int  brdx;  /* dx */
      short int  brdy;  /* dy */
      char   col; /* цвет вывода */
    };/* закрашенный прямоугольник */

struct el{
      char tipobj;   /* тип ГЭ */
      char elstl; /* стиль линии - для эллипса не действует */
      char elthi; /* толщина линии */
      char elcol; /* цвет линии */
      short int  elx;   /* x  */
      short int  ely;   /* y  */
      short int  eldx;  /* dx */
      short int  eldy;  /* dy */
      short int  elfi1; /* угол начала эллипса */
      short int  elfi2; /* угол конца эллипса */
      char   col; /* цвет вывода */
    };/* эллипс */

struct fel{
      char tipobj;   /* тип ГЭ */
      char felstl; /* стиль линии - для эллипса не действует */
      char felfstl; /* стиль заливки */
      char felthi; /* толщина линии */
      char felcol; /* цвет линии */
      char felfcol; /* цвет заливки */
      short int  felx;   /* x  */
      short int  fely;   /* y  */
      short int  feldx;  /* dx */
      short int  feldy;  /* dy */
      short int  felfi1; /* угол начала эллипса */
      short int  felfi2; /* угол конца эллипса */
      char   col; /* цвет вывода */
    };/* заполненный эллипс */

struct tx{
      char tipobj;/* тип ГЭ */
      char txfnt; /* тип шрифта */
      char txdir; /* ориентация 0-гориз 1-вертик */
      char txsiz; /* размер шрифта */
      char txcol; /* цвет текста */
      short int  txx;   /* x  */
      short int  txy;   /* y  */
      unsigned char txmx;  /* число символов текста */
     char txhor; // выравнивание по горизонтали
     char txver; // выравнивание по вертикали
     short int  txsda; // размер окна при создании текста
     unsigned char mltx; // увеличение при плавном масштабе
     unsigned char mlty; // увеличение при плавном масштабе
      char *txt;  /* указатель на строку */
      char   col; /* цвет вывода */
    };/* текст */

struct fl{
      char tipobj;   /* тип ГЭ */
           char flstl; /* стиль заливки */
      char flcol; /* цвет заливки */
      char flbrd; /* цвет линии границы заливки */
      short int  flx;   /* x */
      short int  fly;   /* y */
      char   col; /* цвет вывода */
    };/* заливка области цветом */

struct bk{
      char tipobj;   /* тип ГЭ */
           char bkcol; /* цвет фона */
      char   col; /* цвет вывода */
    };/* фон */

struct reper{
      char tipobj;   /* тип ГЭ */
      short int  x;   /* x */
      short int  y;   /* y */
    };/* реперная точка */
struct pln{
      char tipobj;   /* тип ГЭ */
      char lnstl; /* стиль линии */
      char lnthi; /* толщина линии */
      short int  lnx;   /* x  */
      short int  lny;   /* y  */
      short int  lndx;  /* dx */
      short int  lndy;  /* dy */

      PIPELINE pl; // Pipeline struct

      char color;
};
/* ----------------------------------------------------------------------- */
/*
+++++++++++++++++++
Запись о МП в файле TBG <*.TBG>
. . .
 Тип объекта(char)
 Номер макропримитива(uint)
 Смещение координат(2 * int)
 Положение при рисовании(uint)
 Текущее состояние(uint)
 Выведенное состояние(uint)
. . .
-------------------
*/
/* ----------------------------------------------------------------------- */
/* Всё для работы с макропримитивами */

typedef
   enum tagTCommand{
      Coutpic_all      ,          /* 0 */
      Coutpic_obj_num  ,          /* 1 */
      Coutpic_obj      ,          /* 2 */
      Csetcolor_obj_num,          /* 3 */
      Csetcolor_obj    ,          /* 4 */
      Couttext_obj_num ,          /* 5 */
      Csetstate_obj_num,          /* 6 */
      Csetstate_obj    ,          /* 7 */
      CexecState       ,          /* 8 */
      Coutpic_obj_num_with_rotate,/* 9 */
      Coutpic_obj_num_color,      /* a */
      Coutpic_obj_color,          /* b */
      Csavestate       ,          /* c */
//      Csetreper_obj_num,          /* d */
      END_TBG_COMMAND
   }TCommand;

typedef
   struct tagInterfaceCommand{
      struct tagInterfaceCommand *next;  /* следующая команда */
      TCommand                    tc;    /* тип команды */
      unsigned short int          sized; /* размер данных команды */
      char                       *dc;    /* данные для команды */
   }ICommand;
/*
typedef
   struct tagDsetreper_obj_num{
      unsigned short int obj;
      unsigned short int num;
   }Dsetreper_obj_num;
*/
typedef
   struct tagDoutpic_obj_num{
      unsigned short int obj;
      unsigned short int num;
   }Doutpic_obj_num;

typedef
   struct tagDoutpic_obj{
      unsigned short int obj;
   }Doutpic_obj;

typedef
   struct tagDoutpic_obj_num_with_rotate{
      unsigned short int obj;
      unsigned short int num;
      double             angle;
   }Doutpic_obj_num_with_rotate;

typedef
   struct tagDoutpic_obj_num_color{
      unsigned short int obj;
      unsigned short int num;
      unsigned short int Color;
   }Doutpic_obj_num_color;

typedef
   struct tagDoutpic_obj_color{
      unsigned short int obj;
      unsigned short int Color;
   }Doutpic_obj_color;

typedef
   struct tagDexecState{
      unsigned short int st;
   }DexecState;

typedef
   struct tagDsetcolor_obj_num{
      unsigned short int obj;
      unsigned short int num;
      unsigned short int Color;
      unsigned short int Mode;/* SHOW, NOSHOW ... */
   }Dsetcolor_obj_num;

typedef
   struct tagDsetstate_obj_num{
      unsigned short int obj;
      unsigned short int num;
      unsigned short int State;
      unsigned short int Mode;/* SHOW, NOSHOW ... */
   }Dsetstate_obj_num;

typedef
   struct tagDsetstate_obj{
      unsigned short int obj;
      unsigned short int State;
      unsigned short int Mode;/* SHOW, NOSHOW ... */
   }Dsetstate_obj;

typedef
   struct tagDsetcolor_obj{
      unsigned short int obj;
      unsigned short int Color;
      unsigned short int Mode;/* SHOW, NOSHOW ... */
   }Dsetcolor_obj;

typedef
   struct tagDouttext_obj_num{
      unsigned short int obj;
      unsigned short int num;
      unsigned short int Colorbg;
      unsigned short int Mode;/* SHOW, NOSHOW ... */
      char              *txt;
   }Douttext_obj_num;


typedef
   struct tagStateHead{
      struct tagStateHead *SHnext;  /* след. состояние */
      unsigned short int   nState;  /* номер состояния */
      ICommand            *IChead;  /* первая команда */
   }SHead;

typedef
   struct MPClass{
      struct MPClass     *next;     /* след. класс */
      unsigned short int  index;    /* индекс(имя) */
      char                fname[13];/* имя файла картинки */
      SHead              *pShead;   /* массив HEAD's состояний */
      OBJECT            **pic;      /* картинка макро TBG */
//      short int           xR;       /* reper coord for x */
//      short int           yR;       /* reper coord for y */

                                    /* функция пользователя вызываемая при
                                       изменении состояния */
      short int         (*cb)(struct MPClass *pmpc, unsigned short int st,
                              SHead *sh, char *ud);

      char               *uname;     /* имя для пользователя */
      unsigned short int  nuse;      /* количество использований класса */
      struct MPClass     *parent;    /* чьи состояния использует класс */
               short int  nsuse;     /* использование состояний:
                                         -1 использую чужие состояния
                                         =0 не используют мои состояния
                                         >0 используют мои состояния */
#ifdef __WIN32__
      HDC                 hdc;       /* Дисплейный контекст для вывода класса */
#endif
   }MPClass, *LPMPClass;

typedef
   struct macro{
      char               tipobj;/* тип ГЭ */
      unsigned short int index; /* Номер макропримитива */
      short int          mcx;   /* Смещение координат по X */
      short int          mcy;   /* Смещение координат по Y */
      unsigned short int mode;  /* Положение */
      unsigned short int cs;    /* Состояние */
      MPClass           *cl;    /* Указатель на класс макропримтива(загруженный) */
      char              *ud;    /* Указатель на пользовательские данные данного
                                   экземпляра макропримитива */
      unsigned short int ss;    /* savestate */
   }MP, *LPMP;/* макропримитив */

/* externs */
extern MPClass *mpc;

/* proto */
#ifdef __cplusplus
extern "C" {
#endif
/* загрузка класса макропримитива,
   если уже был загружен, то загрузка не производится,
   наращивается количество использований (nuse) */
MPClass *MPCLoad(unsigned short int index);

/* удаление класса макропримитива
   если класс ещё испльзуется то удаления не происходит
   количество использований уменьшается на 1 */
short int MPCFree(MPClass *mpcd);

/* отрисовка класса */
short int MPCDraw (MPClass *mpc, unsigned int sx, unsigned int sy,
             unsigned short int st, char *ud, LPMP mp);

/* Зацепка пользовательсой функции к классу макропримтивов */
short int MPCHookCB (unsigned short int index,
               short int (*cb)(MPClass *pmpc,unsigned short int st, SHead *sh, char *ud));

/* Установка маршрута доступа к файлам картинок макропримитивов */
/* Например: MPCSetPath("D:\\TBG\\BIN\\"); */
short int MPCSetPath(char *path);

/* TBG_SST1.C */
short int  DLL_EI setstate_obj
  (OBJECT **obj,unsigned char numobj,unsigned short int state,short int show);
short int  DLL_EI setstate_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num,unsigned short int state,
   short int show);
unsigned short int DLL_EI  getstate_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num);
short int  DLL_EI setdata_obj
  (OBJECT **obj,unsigned char numobj, void *ud);
short int  DLL_EI setdata_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num, void *ud);
void*  DLL_EI getdata_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num);

void  DLL_EI savestate(void);

#ifdef __cplusplus
} /* extern "C" */
#endif
/* ----------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif
// Pipe water
#ifdef __WIN32__
void  DLL_EI pipeline(HDC hdc, LPPIPELINE lppl, int color, int thick);
#else
void pipeline(LPPIPELINE lppl, int color);
#endif
/*
   if color == EGA_BLACK ... EGA_WHITE then paint simple line this color

   if color == -1 then pipe moved from xs,ys to xe,ye
   if color == -2 then otherwise from xe,ye to xs,ys
*/
#ifdef __cplusplus
} /* extern "C" */
#endif
//---------------------------------------------------------------------------
//   ОПИСАНИЕ КОДОВ ТИПА ГЭ
//---------------------------------------------------------------------------  CreateWindow

#define OBJ_line    1 /* линия  */
#define OBJ_rect    2 /* прямоугольник  */
#define OBJ_bar     3 /* закрашенный прямоугольник  */
#define OBJ_elip    5 /* эллипс */
#define OBJ_txt     6 /* текст  */
#define OBJ_fld     7 /* заливка цветом  */
#define OBJ_bkgr    8 /* фон  */
#define OBJ_felip  13 /* закрашенный эллипс                   */
#define OBJ_esc    19 /* команда  */
#define OBJ_reper  20 /* реперная точка  */
#define OBJ_winres 21 /* ресурсы WINDOWS */
#define OBJ_pipeline 22 /* ресурсы WINDOWS */
/*
 --- from TBN ---
#define OBJ_line     1 /* ышэш_                      */
#define OBJ_rect     2 /* яЁ_ьюєуюы№эшъ              */
#define OBJ_bar      3 /* чръЁр°хээ_щ яЁ_ьюєуюы№эшъ  */
#define OBJ_elip     5 /* ¤ыышяё                     */
#define OBJ_txt      6 /* ЄхъёЄ                      */
#define OBJ_fld      7 /* чрыштър ЎтхЄюь             */
#define OBJ_bkgr     8 /* Їюэ                        */
#define OBJ_poly     9 /* ыюьрэр_                    */
#define OBJ_fpoly   10 /* ьэюуюєуюы№эшъ              */
#define OBJ_bezier  11 /* ъЁштр_ -хч№х               */
#define OBJ_spline  12 /* -яырщэ - Spline            */
#define OBJ_felip   13 /* чряюыэхээ_щ ¤ыышяё         */
#define OBJ_circle  14 /* юъЁєцэюёЄ№                 */
#define OBJ_fcircle 15 /* ъЁєу                       */
#define OBJ_arc     16 /* фєур                       */
#define OBJ_farc    17 /* ёхъЄюЁ                     */
#define OBJ_intxt   18 /* тяшёрээ_щ ЄхъёЄ            */
#define OBJ_esc     19 /* ¦ръЁюЁрё°шЁхэшх            */
#define OBJ_reper   20 /* reper point                */
-----------------
*/
/*-------------------------- К О Н Е Ц --------------------------------------
      Определение макросов, перечислимых типов и типов данных
---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------
         Размещение функций и внешних переменных по файлам
---------------------------------------------------------------------------*/

/* tbg_dat.c --------------------------------------------------------------*/
extern char Tbl_Present; /* библиотека есть */
extern struct HTBL   hl;
extern struct ITEM *pitDir;

extern FILE *fd,*fd_tmp;

extern char old_name[80];

extern unsigned short int iCountITEM;
extern unsigned short int iALLITEM;
extern int  GraphDriver,GraphMode;
extern char Show_tbg;/* флаг занятости TBG выводом на экран */


extern short int size_xya; /* текущий размер окна             */
extern short int xa_min,ya_min;/* начало окна -абсолютный размер- */

extern double SX_vp;/* размер стороны видеопорта */
extern double SY_vp;/* размер стороны видеопорта */

/* размеры видеопорта */
extern short int MaxX_vp;
extern short int MaxY_vp;

/* размеры полного экрана */
extern short int    iMaxX;
extern short int    iMaxY;
extern double dMaxX;
extern double dMaxY;

extern char MouseSet;/* признак наличия 'мыши' */
extern short int lpress_lb;/* эмуляция было нажатие левой кнопки 'мыша' */
extern short int lpress_rb;/* эмуляция было нажатие правой кнопки 'мыша' */
extern short int cpress_lb;/* эмуляция сейчас нажата левая кнопки 'мыша' */
extern short int cpress_rb;/* эмуляция сейчас нажата правая кнопки 'мыша' */

extern short int error_tbg;/* ошибка выполнения функции интерфейса */

/* tbg_load.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
long ffind(char *name); /* Поиск файла в библиотеке */

/* загрузка графического файла в память */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int DLL_EI load_file(OBJECT ***objret,/* начало "картинки" */
              char *fnam       /* имя загружаемого файла(с расширением) */
             );/*   0 = при успешном завершении */
               /* !=0 = при остальных ошибках   */

/* tbg_del.c --------------------------------------------------------------*/
/* удаление "картинки" из памяти */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int DLL_EI  del_pic(OBJECT ***obj // начало "картинки"
      );//  0 = при успешном завершении
        // <0 = при остальных ошибках

/* tbg_idx1.c -------------------------------------------------------------*/
/* выдает первый индекс в списке ГЭ по NO LE */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI Index_obj_num(OBJECT **obj,unsigned char numobj,unsigned char num);
// Индекс в случае нормального завершения
// -1 = при остальных ошибках

/* tbg_tbl1.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  CreateDir(FILE *LibFile,char *Name_Lib);/* Завести каталог библиотеки */

/* tbg_tbl2.c -------------------------------------------------------------*/
/* устанавливает библиотеку TBG файлов */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
FILE * DLL_EI set_tbl(char *name_tbl // имя файла библиотеки(с расширением)
        );//  указатель на открытый файл = при успешном завершении
          //  0 = при остальных ошибках

/* tbg_tbl3.c -------------------------------------------------------------*/
/* отменяет установку библиотеки TBG файлов */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void   DLL_EI unset_tbl(void);

/* tbg_tbl4.c -------------------------------------------------------------*/
/* скрывает использование библиотеки TBG файлов */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void  DLL_EI hide_tbl(void);

/* tbg_tbl5.c -------------------------------------------------------------*/
/* отменяет скрытие библиотеки TBG файлов */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void  DLL_EI restore_tbl(void);

/* tbg_dmxy.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI DMX(long x);/* приведение к экранным координатам */

#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI DMY(long y);/* приведение к экранным координатам */

/* tbg_xmxy.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void  set_MaxXY(OBJECT **obj);/* установка параметров для вывода в окно */

/* tbg_out0.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int outpic_pn_color(OBJECT **obj,short int pn,short int color);

/* tbg_out1.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int outpic_pn(OBJECT **obj,short int pn);

#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
/* Смещениеначала координат картинки */
void  DLL_EI offset_pic(OBJECT **obj, int x, int y);
/* tbg_bins.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int BinSearch(unsigned short int Search,
              unsigned long *array,
              unsigned short int Size,unsigned short int Mask);

/* tbg_out2.c -------------------------------------------------------------*/
/* показ всей "картинки" */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI outpic_all(OBJECT **obj /* начало "картинки" */
              );/*   0 = при успешном завершении */
                /* !=0 = при остальных ошибках */

/* tbg_out3.c -------------------------------------------------------------*/
/* показ всего объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI outpic_obj(OBJECT **obj,        // начало "картинки"
               unsigned char numobj // номер объекта
              );//   0 = при успешном завершении
                // !=0 = при остальных ошибках

/* tbg_out4.c -------------------------------------------------------------*/
/* показ элемента объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI outpic_obj_num(OBJECT **obj,        // начало "картинки"
         unsigned char numobj,// номер объекта
         unsigned char num    // номер элемента объекта
        );//   0 = при успешном завершении
          // !=0 = при остальных ошибках

/* tbg_out5.c -------------------------------------------------------------*/
/* показ всего объекта заданным цветом */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI outpic_obj_color(OBJECT **obj,        // начало "картинки"
           unsigned char numobj,// номер объекта
           short int color            // заданный цвет
         );//   0 = при успешном завершении
      // !=0 = при остальных ошибках

/* tbg_out6.c -------------------------------------------------------------*/
/* показ элемента объекта заданным цветом */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI outpic_obj_num_color(OBJECT **obj,        // начало "картинки"
          unsigned char numobj,// номер объекта
          unsigned char num,   // номер элемента объекта
          short int color            // заданный цвет
        );//   0 = при успешном завершении
          // !=0 = при остальных ошибках

/* tbg_scl1.c -------------------------------------------------------------*/
/* установка цвета всего объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI setcolor_obj(OBJECT **obj,         // начало "картинки"
                 unsigned char numobj, // номер объекта
                 short int color,            // заданный цвет
                 short int show              // SHOW- показ изменений
                                       // NO_SHOW- нет показа изменений
                );//   0 = при успешном завершении
                  // !=0 = при остальных ошибках

/* tbg_scl2.c -------------------------------------------------------------*/
/* установка цвета элемента объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI setcolor_obj_num
   (OBJECT **obj,        // начало "картинки"
    unsigned char numobj,// номер объекта
    unsigned char num,   // номер элемента объекта
    short int color,           // заданный цвет
    short int show             // SHOW- показ изменений
                         // NO_SHOW- нет показа изменений
   );//   0 = при успешном завершении
     // !=0 = при остальных ошибках

/* tbg_gcl.c --------------------------------------------------------------*/
/* возвращает цвет элемента объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI getcolor_obj_num
    (OBJECT **obj,        // начало "картинки"
     unsigned char numobj,// номер объекта
     unsigned char num    // номер элемента объекта
    );// >=0 = при успешном завершении/цвет/
      // <0  = при остальных ошибках

/* tbg_gcls.c -------------------------------------------------------------*/
/* возвращает цвет элемента объекта выведенного на экран */
/* ф-ей outpic_obj_num_color */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI getcolorset_obj_num
    (OBJECT **obj,        // начало "картинки"
     unsigned char numobj,// номер объекта
     unsigned char num    // номер элемента объекта
    );// >=0 = при успешном завершении/цвет/
      // <0  = при остальных ошибках

/* tbg_gsz1.c -------------------------------------------------------------*/
/* возвращает размер элемента объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI getsize_obj_num
    (OBJECT **obj,        // начало "картинки"
     unsigned char numobj,// номер объекта
     unsigned char num,   // номер элемента объекта
     short int *xmin,short int *ymin,short int *xmax,short int *ymax // размеры элемента
    );//   0 = при успешном завершен
      // !=0 = при остальных ошибках

/* производит сброс уставок предыд. ф-ии -> поиск от начала картинки */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI getsize_obj_num_reset(void);

/* ----------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
#endif
// return tip GE
char  DLL_EI gettip_obj_num(OBJECT **obj, unsigned char numobj, unsigned char num);

/* tbg_cmp1.c -------------------------------------------------------------*/
/* сравнение цвета элемента объекта с заданным */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI cmpcolor_obj_num
    (OBJECT **obj,        // начало "картинки"
     unsigned char numobj,// номер объекта
     unsigned char num,   // номер элемента объекта
     short int color            // цвет
     );//  0 = при не сравнении
       // <0 = при остальных ошибках
       // >0 = если цвет элемента объекта совпал с заданным

/* tbg_cmp2.c -------------------------------------------------------------*/
/* сравнение выведенного цвета элемента объекта с заданным */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI cmpcolorset_obj_num
    (OBJECT **obj,        // начало "картинки"
     unsigned char numobj,// номер объекта
     unsigned char num,   // номер элемента объекта
     short int color            // цвет
     );//  0 = при не сравнении
       // <0 = при остальных ошибках
       // >0 = если цвет элемента объекта совпал с заданным

/* tbg_txt1.c -------------------------------------------------------------*/
/* возвращает указатель на строку текста элемента объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
char*  DLL_EI gettext_obj_num
    (OBJECT **obj,        // начало "картинки"
     unsigned char numobj,// номер объекта
     unsigned char num    // номер элемента объекта
    );//  указатель на строку = при успешном завершении
      //  NULL                = при остальных ошибках

/* tbg_txt2.c -------------------------------------------------------------*/
/* изменение содержимого текста для элемента объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI outtext_obj_num
    (OBJECT **obj,        // начало "картинки"
     unsigned char numobj,// номер объекта
     unsigned char num,   // номер элемента объекта
     short int color_fon,       // цвет фона,на котором расположен текст
     char const * const text,          // новый текст
     short int show             // SHOW - обязательный показ изменений
                          // NO_SHOW - показ только если
                          // есть фактические изменения текста по сравнению
                          // с уже хранящимся
    );//   0 = при успешном завершении
      // !=0 = при остальных ошибках

/* tbg_txt3.c -------------------------------------------------------------*/
/* возвращает предельный размер строки текста элемента объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI strlen_obj_num
    (OBJECT **obj,        // начало "картинки"
     unsigned char numobj,// номер объекта
     unsigned char num    // номер элемента объекта
    );//  размер = при успешном завершении
      // <0      = при остальных ошибках

/* tbg_get1.c -------------------------------------------------------------*/
/* возвращает максимальный номер элемента объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI getmax_obj
   (OBJECT **obj,         // начало "картинки"
    unsigned char numobj, // номер объекта
    short int           *num_cou// максимальный номер
   );//   0 = при успешном завершении
     // !=0 = при остальных ошибках

/* tbg_get2.c -------------------------------------------------------------*/
/* возвращает минимальный номер элемента объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI getmin_obj
   (OBJECT **obj,         // начало "картинки"
         unsigned char numobj, // номер объекта
         short int           *num_cou// минимальный номер
        );//   0 = при успешном завершении
          // !=0 = при остальных ошибках

/* tbg_get3.c -------------------------------------------------------------*/
/* возвращает номер элемента объекта на который указывает 'мышь' */
/* выяснение позиции "мыши" */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI getpos_mouse_obj
   (OBJECT **pic,         // начало "картинки"
    unsigned char numobj, // номер объекта
    unsigned char StartNum// номер элемента с которого начинается анализ
   );//  номер = при успешном завершении
     // -1    = 'мышь' не указывает ни на один элемент

/* tbg_get4.c -------------------------------------------------------------*/
/* возвращает номер элемента объекта для заданного элемента объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI get_num_obj
      (OBJECT **obj,       // начало "картинки"
      unsigned char numobj,// номер объекта для фиксации граф.примитива
      unsigned char num,// номер элемента объекта для фиксации граф.примитива
      unsigned char numobj2 // номер объекта для поиска
      ); // номер элемента объекта numobj2
         // <0 = при ошибках

/* tbg_set1.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void  DLL_EI setviewport_obj_num(OBJECT **obj,unsigned char numobj,unsigned char num);

/* tbg_set2.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void setpalette_tbg(void);// установка "улучшенной" палитры TBG

/* tbg_set3.c -------------------------------------------------------------*/
/* перемещение объекта заданием новых координат (пока только линии) */
/* координаты экранные лево,верх,право,низ */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI setcoordinate_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num,
   short int x1,short int y1,short int x2,short int y2
  );//   0 = при успешном завершении
    // !=0 = при остальных ошибках

/* tbg_pren.c -------------------------------------------------------------*/
/* попадание в элемент объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI pren(OBJECT **pic,short int obj,short int num,short int x,short int y);


/* tbg_chk1.c -------------------------------------------------------------*/
/* проверяет наличие заданного объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI check_obj(OBJECT **obj,        // начало "картинки"
         unsigned char numobj // номер объекта
        );//  1 = при наличии объекта
          //  0 = при отсутствии

/* tbg_chk2.c -------------------------------------------------------------*/
/* проверяет наличие заданного элемента объекта */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI check_obj_num(OBJECT **obj,        // начало "картинки"
        unsigned char numobj,// номер объекта
        unsigned char num    // номер элемента объекта
       );//  1 = при наличии элемента объекта
         //  0 = при отсутствии

/* tbg_mou0.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" { /* объявление для C++ */
#endif
short int mouse_ini(void);
/*  mouse_ini - инициализация мыши,
         возвращает < 0,если плата и драйвер установлены;
            0,если плата и драйвер не установлены.*/


enum {
   ABSENT,
   PRESENT,
   MOUSE_OFF,
   MOUSE_ON,
   MOUSE_DRAW,
   MOUSE_NODRAW,
   CURSOR_OFF,
   CURSOR_ON,
   CURSOR_DRAW,
   CURSOR_NODRAW
};

extern int mouse_mode;

void mouse_draw(void);

#ifdef __cplusplus
} /* объявление для C++ */
#endif

/* tbg_mou1.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void  mouse_on(void);
/*  включение отображения мыши */

/* tbg_mou2.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void  mouse_off(void);
/*  выключение отображения мыши */

/* tbg_mou3.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void mouse_setxy(short int x,short int y);
/*  установка мыши по заданным x,y */

/* tbg_mou4.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
/* Для корректной работы mouse_getxy()
   в режиме CGAC0 320*200 4 цвета
   инициализировать графику так:
   initgraph(&GraphDriver, &GraphMode,"путь"); */
void  DLL_EI mouse_getxy(short int *x,short int *y);
/* Координаты 'мыша' в момент вызова функции.*/

/* tbg_mou5.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void mouse_cpk(short int *keys);
/* Нажаты ли кнопки в момент вызова функции.*/
/*            keys:1-левая нажатa; 2-правая нажатa; 3-обе нажаты  */

/* tbg_mou6.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void mouse_lpk(short int *left,short int *right);
/* Были ли нажатия с момента последнего вызова функции.*/
/*            left,right:1-была нажата; 0- не была нажата */

/* tbg_mou7.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void mouse_lrk(short int *left,short int *right);
/* Были ли отпускания с момента последнего вызова функции
   'last release key' */

/* tbg_mou8.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void mouse_mask(unsigned short int *mask,char Hotpix_column,char Hotpix_line);
/* Установка пользовательского курсора мыши, */
/* mask - адрес массива из 32 short int-ов(16-экранная маска,16-курсорная маска)*/
/* Hotpix_... - координаты горячей точки курсора. */

/* tbg_mou9.c -------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
void mouse_keyxy(short int *key_left,short int *key_right,short int *key_press,
                short int *x_mouse,  short int *y_mouse);

/* tbg_rtt1.c -------------------------------------------------------------*/
/*
     Вращение объекта-элемента.
     Центр поворота -> obj,num (только эллипс),
   исходное состояние объекта -> obj,num+1 (только линии,любого цвета),
   копия obj,num+1 => obj,num+2 (только линии,только цвет фона),
   угол задается в градусах относительно исходного состояния;
     Запрещается использовать в объекте поворота(obj,num+1),линии разных
   стилей и цветов.
     Перед поворотом необходима инициализация макросом
   "init_rotate_obj_num(OBJECT**,obj,num,a);"(строка 44 тек. файла).
     Желательно рисовать obj,num+1 поверх obj,num+2 чтобы в редакторе
   было видно исходное состояние объекта.
     Не рекомендуется рисовать несколько объектов вращения друг на друге
   т.к. при инициализации прорисовка ведется XOR-ом и поэтому возможны
   побочные эффекты.
*/
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
short int  DLL_EI rotate_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num,double a
  );//   0 = при успешном завершении
    // !=0 = при остальных ошибках

short int  DLL_EI outpic_obj_num_with_rotate
  (OBJECT **obj,unsigned char numobj,unsigned char num,double a);

/* tbg_err.c --------------------------------------------------------------*/
/* печатает сообщения об ошибке */
#ifdef __cplusplus
extern "C" /* объявление для C++ */
#endif
char*  DLL_EI print_error_tbg(char* mes /* сообщение о месте вызова функции */
                    );

/*-------------------------- К О Н Е Ц --------------------------------------
    Размещение функций и внешних переменных по файлам
---------------------------------------------------------------------------*/

/*
         Структура файла .TBG v6.00
         1992-94(C) Павельев П.И., Чумак А.И.
Смещение  длина
      +0      1   Тип файла
      +1     80   Заголовок файла(HTBG_WR)
     +82      1   Тип ГЭ( < 0), напр. 0xFF - Линия,0xFE - Прямоугольник.
     +83     ??   Информация о ГЭ (длина в зависимости от типа)
     +??      1   Тип ГЭ( < 0) || ЛЭ-ты для ГЭ( > 0)количество ЛЭ.
     +??      1   Номер объекта
     +??      1   Номер элемента

или:
               количество ЛЭ для этого ГЭ(ЛЭ+1)-+
                            V
+-------++-----------+------------------++-----------++-----------+----------+
¦Т¦Заг-к¦¦ГЭ инф.о ГЭ¦ЛЭ¦obj num¦obj num¦¦ГЭ инф.о ГЭ¦¦ГЭ инф.о ГЭ¦ЛЭ¦obj num¦
+-------++-----------+------------------++-----------++-----------+----------+
             A
             +-количество ЛЭ для этого ГЭ(ЛЭ+1)
//---------------------------------------------------------------------------
Дамп TBG файла: 10 ГЭ,10 ЛЭ

000  05 20 54 42  47 20 36 2E  30 30 20 4C  01 08 05 09 . TBG 6.00
010  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 20
020  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 20
030  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 20
040  20 20 20 20  20 20 20 20  20 20 20 20  20 20 09 00               ..
050  09 00 FF 00  01 02 D8 23  BB 21 6C 10  00 00 00 01 .._...+#+!l.....
060  01 FE 00 01  02 3F 24 C4  25 9F 0F 80  05 00 01 01 .....?$-%Я¤А....
070  FD 01 02 3F  24 08 30 9F  0F CD 08 00  01 01 FB 00 ¤..?$.0Я¤-...._.
080  01 02 42 2C  5D 42 9C 07  66 04 00 00  68 01 00 01 ..B,]BЬ.f...h...
090  01 FA 06 00  04 02 D9 25  4C 50 0A 00  00 F8 7F 0A ......+%LP...°.
0A0  0A 31 32 33  34 35 36 37  38 39 30 00  01 01 FF 00 .1234567890..._.
0B0  01 09 AF 42  BB 21 6C 10  00 00 00 02  02 FE 00 01 ..пB+!l......_..
0C0  09 16 43 C4  25 9F 0F 80  05 00 02 02  FD 01 09 16 ..C-%Я¤А....¤...
0D0  43 08 30 9F  0F CD 08 00  02 02 FB 00  01 09 19 4B C.0Я¤-...._....K
0E0  5D 42 9C 07  66 04 00 00  68 01 00 02  02 FA 06 00 ]BЬ.f...h.......
0F0  04 09 B0 44  4C 50 0A 00  00 F8 7F 0A  0A 31 32 33 ...DLP...°..123
100  34 35 36 37  38 39 30 00  02 02                    4567890...
//---------------------------------------------------------------------------

         Структура памяти при загрузке TBG.
         1992-94(C) Чумак А.И.

                  + +---------+ +
         struct tag_Logic[Max_LE]     ¦ ¦ Элемент ¦ ¦
         +---------------------+      ¦ +---------¦ ¦
   +---->¦ ¦ ¦ ¦ ¦ ¦ ¦ . . . ¦ ¦<-----¦ ¦ Объект  ¦ + sizeof(long)
   ¦     +---------------------+      ¦ +---------¦ ¦
   ¦          + - - - - - - - - - - - + ¦ Индекс  ¦ ¦
   ¦      0 1 ¦ void *[Max_GE+1]      + +---------+ +
   ¦     +----V----------------+
   ¦     ¦*¦*¦*¦*¦*¦*¦ . . . ¦*¦
        ¦     +---------------------+
        ¦      ¦ ¦ ¦ +-----------------------------+
        ¦      ¦ ¦ +-------------------+           ¦
        ¦      ¦ +---------+           ¦           ¦
        ¦  +---V-----+ +---V-----+ +---V-----+ +---V-----+
        ¦  +---------¦ ¦ Ellipse ¦ ¦ FldFill ¦ ¦  Text   ¦
        ¦  +---------¦ +---------¦ +---------¦ +---------¦
        ¦  ¦  . . .  ¦ ¦  . . .  ¦ ¦  . . .  ¦ ¦  . . .  ¦
        ¦  +---------¦ +---------¦ +---------¦ +---------¦       +----------+
        ¦  +---------¦ +---------¦ +---------+ +---------¦  +----> "Hello." ¦
        +--¦ Logic*  ¦ +---------¦             +---------¦  ¦    +----------+
           +---------¦ +---------+             +---------¦  ¦
           ¦ Max_LE  ¦   A                     +---------¦  ¦
           +---------¦   ¦                     ¦  char*  +--+
           ¦ Max_GE  ¦  --- - - - -            +---------¦
           +---------+ ¦ struct el ¦           +---------+
             A          - - - - - -
             ¦    - - - - - - -
             +---¦ struct HTBG ¦
        - - - - - - -
*/
#endif /* TBG_TC_H */
