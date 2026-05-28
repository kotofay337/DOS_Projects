//   TBX.H
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <bios.h>
#include <conio.h>
#include <dos.h>
#include <dir.h>
#include <ctype.h>
#include <string.h>
#include <graphics.h>

#define OK  1
#define NO  0

//---------------------------------------------------------------------------
#define TBG_IVX        8
#define TBG_TBX        1
#define TBX_TBG        2
#define PLT_TBG        3
#define VIEW           4
#define PRINT          5
#define C_convert      6
#define Pascal_convert 7

#define OBJ_line 1 /* линия  */
#define OBJ_rect 2 /* прямоугольник  */
#define OBJ_bar  3 /* закрашенный прямоугольник  */
#define OBJ_elip 5 /*  эллипс */
#define OBJ_txt  6 /* текст  */
#define OBJ_fld  7 /* заливка цветом  */
#define OBJ_bkgr 8 /* фон  */
#define OBJ_Logic 9 /* ЛЭ  */

#define max_copyr 15

struct HTBL{      // Заголовок для элемента библиотеки TBL.
    char name[15];// имя файла картинки
    long len;     // длина файла картинки в байтах
};// HTBL

struct HTBG_WR{   // для записи-чтения заголовка tbg-файла
 char   copyr[max_copyr];
 struct HTBL sr1;
 char   ir2[43];
 int Num_LE;      // количество логических элементов
 int count_ei;    // количество эл-тов изображения
};// HTBG_WR
//--------------------------------------------------------------------------
// tbx_000.c


// tbx_001.c
void tbxp(void);
int  DMX(long x);
int  DMY(long y);
int  FindLex(char *search,char *strings[],int Count);
int  mmx(int x);
int  mmy(int y);
void setpalette_tbg(void); // установка "улучшеной" палитры

// tbx_002.c
void convert_file(FILE *tbg);

// tbx_003.c
int Read_GE(char tip,FILE *fd);

// tbx_004.c
void restore_file(FILE *tbx);
int  find_string(char *strings[],char *search,int Count);
int  over_color(int color);



//---------------------------------------------------------------------------
struct ln{
	   char tipobj;   /* тип ГЭ */
           char lnstl; /* стиль линии */
	   char lnthi; /* толщина линии */
	   char lncol; /* цвет линии */
	   int  lnx;   /* x  */
	   int  lny;   /* y  */
	   int  lndx;  /* dx */
	   int  lndy;  /* dy */
	 };/* линия */

struct rc{
	   char tipobj;   /* тип ГЭ */
           char rcstl; /* стиль линии */
	   char rcthi; /* толщина линии */
	   char rccol; /* цвет линии */
	   int  rcx;   /* x  */
	   int  rcy;   /* y  */
	   int  rcdx;  /* dx */
	   int  rcdy;  /* dy */
	 };/* прямоугольник */

struct br{
	   char tipobj;   /* тип ГЭ */
           char brstl; /* стиль закраски */
	   char brcol; /* цвет закраски */
	   int  brx;   /* x  */
	   int  bry;   /* y  */
	   int  brdx;  /* dx */
	   int  brdy;  /* dy */
	 };/* закрашенный прямоугольник */

struct el{
	   char tipobj;   /* тип ГЭ */
           char elstl; /* стиль линии - для эллипса не действует */
	   char elthi; /* толщина линии */
	   char elcol; /* цвет линии */
	   int  elx;   /* x  */
	   int  ely;   /* y  */
	   int  eldx;  /* dx */
	   int  eldy;  /* dy */
	   int  elfi1; /* угол начала эллипса */
	   int  elfi2; /* угол конца эллипса */
	 };/* эллипс */

struct tx{
	   char tipobj;   /* тип ГЭ */
           char txfnt; /* тип шрифта */
	   char txdir; /* ориентация 0-гориз 1-вертик */
	   char txsiz; /* размер шрифта */
	   char txcol; /* цвет текста */
	   int  txx;   /* x  */
	   int  txy;   /* y  */
	   char txmx;  /* число символов текста */
           char txhor; // выравнивание по горизонтали
           char txver; // выравнивание по вертикали
           int  txsda; // размер окна при создании текста
           unsigned char mltx; // увеличение при плавном масштабе
           unsigned char mlty; // увеличение при плавном масштабе
	   char *txt;  /* указатель на строку */
	 };/* текст */

struct fl{
	   char tipobj;   /* тип ГЭ */
           char flstl; /* стиль заливки */
	   char flcol; /* цвет заливки */
	   char flbrd; /* цвет линии границы заливки */
	   int  flx;   /* x */
	   int  fly;   /* y */
	 };/* заливка области цветом */

struct bk{
	   char tipobj;   /* тип ГЭ */
           char bkcol; /* цвет фона */
	 };/* фон */

//----------------------------------------------------------
extern char mode_key;

extern int XmaxPlt,XminPlt,YmaxPlt,YminPlt;
extern double MaxX,MaxY;

extern char Cache[1024],String[100];

extern int XmaxPlt,XminPlt,YmaxPlt,YminPlt;
/*extern char huge CheckObj[256][256];*/

extern struct ln ln;
extern struct rc rc;
extern struct br br;
extern struct el el;
extern struct tx tx;
extern struct fl fl;
extern struct bk bk;

extern int lstl,lthi,
    col,
    fstl,fcol,
    tfnt,tdir,tsiz,
    thor,tver,
    tmltx,tmlty;

extern double MaxX,MaxY;// EGAHI

extern char text[255];

extern char *PltLex[];

extern char *GraphElements[];

extern char *Colors[];

extern char *Widths_line[];

extern char *Styles_line[];

extern char *Styles_flood[];

extern char *Justify[];

extern char *Directions[];

extern char *Fonts[];

extern char argv1[3], argv2[13];

extern char ErOpenFile[];
extern char sfmt_01 [];
extern char sfmt_02 [];
extern char sfmt_021[];
extern char ErKey   [];
extern char Title   [];
extern char Usage   [];
extern int tip_txt,mode;

extern FILE *out,*in;
extern int GD,GM;

//----------------------------------------------------------