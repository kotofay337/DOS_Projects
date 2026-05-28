//-----------------------------------------------------------------------
//
//      TurBoGraphica V5.00
//
//      RED5.H
//      Здесь находятся определения
//      классов: Line,Rectangle,...
//               LIST,
//               Logic,
//               BaseGraphElement.;
//
//-----------------------------------------------------------------------

#include <dir.h>

//#define DBG_ListLogic  //отладочная печать класса:если определена

/*
tip=1,2,3,4 Txx=0 все подтипы=0
tip=5 Ttx=1
*/
#define TF  5 // тип записываемого файла
#define Ttx 1 // подтип текста


#define YES  1
#define NOT  0

#define INC  1
#define DEC  0

#define SET    1
#define NOSET  0

#define SZ_sbl 9      //размер шаблона
#define SZ_text 100


#define STEP_GRID_DEFAULT 10

#include "panel.h"    // функции работы с панелью
#include "tbg_tc.h" // интерфейс библиотеки ТурБоГрафики
#include "pcxlib.h"

extern "C"{


int   far pascal  pcxSetDisplay        (int);
int   far pascal  pcxFileDisplay       (char far *,int,int,int);
int   far pascal  pcxDisplayFile       (char far *,int,int,int,int,int);
int   far pascal  pcxGetFileHeader     (char far *,PCXHEADER far *);
int   far pascal  pcxGetFilePalette    (int,char far *,char far *);
int   far pascal  pcxSetDisplayPalette (char far *);

}

typedef unsigned char BYTE;
typedef   signed char CHAR;

#define f_mono 1      // поле однозначного выбора
#define f_poli 2      // поле многозначного выбора
#define f_info 3      // поле информационное
#define SHOW_FLD 1    // показ изменений
#define NO_SHOW_FLD 0 // не показ изменений

typedef struct
{
/*
 int xe_min,ye_min,xe_max,ye_max;// координаты окна -экран-
 int xa_max,ya_max;// координаты окна -абсолют-
 int xca,yca;// координаты центра -абсолют-
*/
 int xa_min,ya_min;   // координаты окна -абсолют-
 int size_xya;// размер стороны окна -абсолют-
} WINDOW;

typedef struct
{
 CHAR div_path[MAXPATH];
 CHAR sbl[SZ_sbl];
 CHAR Fn_fon_pcx[80];//имя фонового файла PCX
 CHAR Fn_fon_tbg[80];//имя фонового файла TBG
 CHAR Fn_work_tbg[13];//имя рабочего файла TBG
 CHAR Fn_work_pcx[13];//имя рабочего файла PCX

 CHAR col_lt;        /* цвет линии,текста */
 CHAR stl_l;         /* стиль линии */
 CHAR thi_l;         /* толщина линии */
 CHAR col_f;         /* цвет закраски */
 CHAR stl_f;         /* стиль закраски */
 CHAR fnt_t;         /* стиль шрифта текста */
 CHAR dir_t;         /* ориентация текста */
 CHAR siz_t;         /* размер символа текста */
 CHAR ig_mode;       // режим инициализации графики
 CHAR Mode_save;     // режим сохранения
 CHAR Mode_load;     // режим загрузки
 CHAR Mode_del;      // режим удаления
 CHAR Mode_copy;     // режим копирования
 CHAR Mode_move;     // режим сдвигa
 CHAR Mode_show;     // режим перевыводa
 CHAR Mode_indic;    // режим индикации
 CHAR Mode_grid[1];  // режим индикации сетки-разметки
 CHAR Mode_korr_lre[3];  // режим корректуры L-R-E
 CHAR Mode_korr_bzf[3];  // режим корректуры B-Z-F
 CHAR Mode_korr_t  [5];  // режим корректуры T
 CHAR stl_thi;       // меню стилей линии
 CHAR hor_t;         // выравнивание горизонталь
 CHAR ver_t;         // выравнивание вертикаль
 CHAR tip_search;    // тип графического элемента для поиска
 CHAR Mode_exit;     // режим выхода
 CHAR Mode_elip;     // режим рисования эллипса filled/wire
 CHAR Mode_line;     // режим рисования линии line/pipeline

 struct{int x,y;} reper[10];/* точки мыши на экpане */
 int KodMacro[128];
 int MaxSizeKodMacro;
 unsigned int mc_index;/* индекс(имя) макропримитива */
 unsigned int mc_state;/* состояние */
} PCK; // описание одного поля панели

extern PCK pck;


#define ON  1 /* вкгючено */
#define OFF 0 /* отглючено */

#define FMS   0 /* Сохранение в файле  */
#define SMS   1 /* Сохранение в памяти */
#define XMS   2 /* Сохранение в XMS    */

#define RESTART   -1 /*  нужен перевывод меню */
#define NORESTART  0 /* не нужен перевывод меню */



#define Ok  1 // успех
#define Fu  0 // Фу,плохо !

#define GABARIT_LINE 5

//#define SIZE_XY 32760 /* предел внутренних координат x,y */

// режимы

//сохранение
#define SAVE_obj 10 // номер обьекта текстов в меню
#define LOAD_obj 13 // номер обьекта текстов в меню

#define mode_save_all_pcx 0 // все PCX
#define mode_save_ram_pcx 1 // в рамке PCX
#define mode_save_all_tbg 2 // все TBG
#define mode_save_ram_tbg 3 // в рамке TBG
#define mode_save_ge_tbg  4 // в рамке TBG
//#define mode_save_pcx     5 // в формате PCX

#define mode_load_fon_pcx 0
#define mode_load_new_tbg 1
#define mode_load_wrk_tbg 2
#define mode_load_fon_tbg 3

//удаление
#define mode_del_all 0     // все
#define mode_del_ram 1     // в рамке
#define mode_del_ge  2     // текущий ГЭ
#define mode_del_fon_pcx 3 // файла фона
#define mode_del_fon_tbg 4 // файла фона

//копирование
#define mode_copy_ram 0 // в рамке
#define mode_copy_ge  1 // текущий ГЭ

//сдвиг
#define mode_move_ram 0 // в рамке
#define mode_move_ge  1 // текущий ГЭ
#define mode_move_lge 2 // по списку

//перевывод
#define mode_show_no  0 // обьекта
#define mode_show_ln  1 // ЛН обьекта
#define mode_show_all 2 // всего

//индикация
#define mode_indic_no  0 // обьекта
#define mode_indic_ln  1 // ЛН обьекта
#define mode_indic_ge  2 // текущий ГЭ

#define mode_grid      0 // позиция в поле
// режимы выхода
#define mode_exit_se   0 // сохранить и выйти
#define mode_exit_nse  1 // без сохранения
#define mode_exit_ne   2 // не выходить

#define mode_text_ficx   0 // режим масштаба текста-фиксированный
#define mode_text_noficx 1 // режим масштаба текста- произвольный

// режимы

/* коды типа элемента */
#define TIP_line    1
#define TIP_rect    2
#define TIP_bar     3
#define TIP_elip    5
#define TIP_txt     6
#define TIP_fld     7
#define TIP_bkgr    8
#define TIP_felip  13
#define TIP_esc    19
#define TIP_reper  20
#define TIP_bitmap 21
#define TIP_pipeline 22
#define max_tip    22

/* коды команд */
#define KOM_line   10
#define KOM_pipeline   11
#define KOM_rect   20
#define KOM_bar    30
#define KOM_elip   50
#define KOM_felip  51
#define KOM_txt    60
#define KOM_fld    70
#define KOM_bkgr   80
#define KOM_copy   90
#define KOM_move  100
#define KOM_mash  110
#define KOM_fst   111
#define KOM_lst   112
#define KOM_raio  120
#define KOM_macro 130
#define KOM_reper 140

//---------------------------------------------------------------------------
//                          Описание типов данных
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//                          Описание прототипов функций
//---------------------------------------------------------------------------


/*---- red?_000 ----*/

void ibell(void);
int KY(int y);
int mmx(int x);
int mmy(int y);
int DMx(long x);
int DMy(long y);
void set_attrib(void);

extern unsigned int markers[][32];
#define HAND  0
#define ARROW 1
#define CROSS 2

/*---- red?_001 ----*/
int tbk_cs(char *file_name);
int painter(void);

/*----  red?_002 ----*/
void help_f1(void);
void attrxy(char x,char y,unsigned char lenght,
			  unsigned char bg,
			  unsigned char fg);
void show(char line);/* красивые переливы линии Copyright */
void press_key(void);
void Split(int address);

/*----  red?_003 ----*/
// Методы класса LIST::

// FILE*LIST::CreateInputFile(char *fname) //создать файл для вывода
// int  LIST::SaveOne(FILE *inf,int Index)//сохранение одного элемента
// void LIST::SaveAll(char *fname) // сохранение всего изображения
// void LIST::LoadFile(char *fname)// загрузка изображения из файла
// LIST::LIST(int InitMax)// конструктор
// void LIST::DI_fi1(int direct)
// void LIST::DI_fi2(int direct)
void Warning(char *errror);
int Crt_Bakup(char const *bakup);// создание файла .BAK

/*----  red?_004 ----*/
/* ввод строки для ГЭ "Text" */
char *EditString(char *string);
char* EditString_obj_num(OBJECT **Edit_Pic,     // картинка
                         unsigned char editobj, // обьект
                         unsigned char editnum, // элемент типа "текст"
                         char *string,
                         unsigned char max);
int   EditLine_obj_num(OBJECT **PicTBG,
                       unsigned char editobj,
                       unsigned char editnum);
void Refresh_Display(OBJECT **Edit_Pic,
                     unsigned char editobj,
                     unsigned char editnum);
void Edit_Init(OBJECT **Edit_Pic,
               unsigned char editobj,
               unsigned char editnum,
               char *String,
               unsigned char max);
char* Edit_Stop(void);


/*----  red?_005 ----*/
// методы LIST
// void LIST::Korr(void)//корректура всего что надо



/*----  red?_006 ----*/
// методы LIST
//void LIST::DelGE(void) //удаление текущего ГЭ
//void LIST::DelGE_All(void)//удаление всех ГЭ
//void LIST::DelGE_Ram(void)//удаление ГЭ в рамке
//void LIST::Set_kf_size(void)

/*----  red?_007 ----*/
// методы LIST
// char* LIST::GetText(void)
// void LIST::ShowCur(void)
// int  LIST::GetPN(void)
// int  LIST::GetPN_TOP(void)
// int  LIST::GetMAX_BGE(void)
// void LIST::EndPN(void)
// void LIST::StartPN(void)
// char LIST::GetTipSearch(void)
// void LIST::DecTipSearch(void)
// void LIST::IncTipSearch(void)
// char LIST::GetTip(void)
// int  LIST::GetX(void)
// int  LIST::GetY(void)
     // добавление элемента в конец списка
// void LIST::AddLE(unsigned char no,unsigned char ln)
     // удаление текущего логического элемента
// void LIST::DelCLE(void)
     // максимальное число логических элементов
// int  LIST::GetMaxLE(void)
     // текущая позиция логического элемента
// int  LIST::GetCurLE(void)
     // номер обьекта текущего графического элемента
// int  LIST::Get_NO_CGE(void)
     // логический номер текущего графического элемента
// int  LIST::Get_LN_CGE(void)
     // изменение NO
// void LIST::DI_NO_CLE(int direct)
     // изменение LN
// void LIST::DI_LN_CLE(int direct)
     // переход назад по списку LE
// void LIST::Pred(void)
     // переход вперед по списку LE
// void LIST::Succ(void)
// void LIST::Search_NO_Pred(void)
// void LIST::Search_NO_Succ(void)
// void LIST::Search_LN_Pred(void)
// void LIST::Search_LN_Succ(void)
// int  LIST::Start_LLE()
// int  LIST::End_LLE()

/*----  red?_008 ----*/
//char *menu_sl(char *text);
char *menu_sl(BYTE);

/*----  red?_009 ----*/
// методы LIST
//void CopyOne(int src,int dst); //копирование текущего ГЭ
//void CopyRam(void);//копирование элементов попавших в рамку
//void CopyGE(void); //копирование текущего ГЭ
//void CopyOneMirrorX(int src,int dst); //копирование текущего ГЭ с отр. по X
//void CopyOneMirrorY(int src,int dst); //----------- " " -------------- по Y

/*----  red?_010 ----*/
int otsek(int x1,int y1,int x2,int y2);//отсечение окном
int handler(int errval,int ax,int bp,int si);// обработчик ошибок железа
void refr(void);//перевывод экрана без отключения мышы и рамки
void refr_t(void); //перевывод экрана с отключением мышы и рамки
void risp(int x,int y);
void ybcur(void);
void set_mode_xor(void);

/*----  red?_011 ----*/
//методы BaseGraphElement::
//void BaseGraphElement::Move (void)//перемещение элемента попавшего в рамку
//void BaseGraphElement::Mash (void)//масштабирование элемента попавшего в рамку


/*----  red?_012 ----*/
int obrab_mouse();
void off_komand();
void ZoomOn(int xzoom);
void ZoomOff(void);
void ZoomDisplay (void);

void Rulers(void);
extern int sRulers;

/*----  red?_013 ----*/
int pos_mouse(void);
// визуализация нажатия кнопки
void visual_on(OBJECT **menu,unsigned char numobj,unsigned char num);

/*----  red?_014 ----*/
int ris_ramka(void);
int ramka1_mouse(void);
int ramka0(void);

/*----  red?_015 ----*/
int oumenu(void);
void indid_cur_fn(void);// индикация теущего имени файла

/*----  red?_016 ----*/
// индикация выравнивания текста
void indic_vira_t(OBJECT **menu);
// индикация текущего стиля и цвета закраски в главном меню
void indic_stl_f(OBJECT **menu);
// индикация текущего цвета в меню стилей закраски
void indic_col_f(OBJECT **menu);
// индикация текущего стиля и цвета линии в главном меню
void indic_stl_l(OBJECT **menu);
// индикация текущего цвета в меню стилей линии
void indic_col_lt(OBJECT **menu);
int pp_pos13(void);
int pp_pos16(void);
int pp_pos161(void);
int pp_pos17(void);
int pp_pos18(void);
int pp_pos19(void);
int pp_pos20(void); /* макропримитив */
int pp_pos30(void);
int pp_pos31(void);
int pp_pos33(void);
int pp_pos34(void);
int pp_pos35(void);
int pp_pos36(void);
int pp_pos39(void);
void pp_indic();

/*----  red?_017 ----*/
void DecTipSearch(void);
void IncTipSearch(void);


/*----  red?_018 ----*/
void indic_mess(void);//вывод сообщений

/*----  red?_020 ----*/
// Методы класса LIST::
// void Vibor  (void);//определение элементов попавших в рамку
// void MoveRam(void);//перемещение элементов попавших в рамку
// void MashRam(void);//масштабирование элементов попавших в рамку
// void ShowAll(void);//перевывод всего изображения
// void ShowNO(void); //перевывод обьекта NO
// void ShowLN(void); //перевывод LN обьекта
// void SetOld (void);//восстановление прежних параметров при масштабировании
// void ClearOld(void);//очистка всех значений выбора
// void IndicNO(void); //индикация NO
// void IndicLN(void); //индикация LN
// void IndicGE(void); //индикация текущего ГЭ


/*----  red?_021 ----*/
// Методы класса LIST::
// void LIST::IncPN(void)
// void LIST::DecPN(void)
// void LIST::Search_NO_Pred_LGE(void)//поиск предыдущего NO в списке CGE
// void LIST::Search_NO_Succ_LGE(void)//поиск следующего  NO в списке CGE
// void LIST::Search_LN_Pred_LGE(void)//поиск предыдущего LN в списке CGE
// void LIST::Search_LN_Succ_LGE(void)//поиск следующего  LN в списке GE
     //поиск предыдущего LN равного теущему в списке CGE
// void LIST::Search_All_LN_Pred_LGE(void)
     //поиск следующего LN равного теущему в списке CGE
// void LIST::Search_All_LN_Succ_LGE(void)
// void LIST::Search_Tip_Pred_LGE(void)//поиск предыдущего типа в списке GE
// void LIST::Search_Tip_Succ_LGE(void)//поиск следующего  типа в списке GE

/*----  red?_022 ----*/
// Методы класса LIST::
// int LIST::CreateGE(int tip)//создание GE заданного типа

/*----  red?_023 ----*/
//методы Line::

/*----  red?_024 ----*/
// Методы класса Rectangle::

/*----  red?_025 ----*/
// Методы класса Bar::

/*----  red?_026 ----*/
// Методы класса Ellipse::

/*----  red?_027 ----*/
// Методы класса Text::

/*----  red?_028 ----*/
// Методы класса Floodfill::

/*----  red?_029 ----*/
// Методы класса Background::

/*----  red?_030 ----*/
// Методы класса BaseGraphElement::
// void BaseGraphElement::ClearLLE(void);// удаление списка LLE
// void BaseGraphElement::ListLogic_PRINT(void)
     // добавление элемента в конец списка
// void BaseGraphElement::BGE_Add_LE (unsigned char no,unsigned char ln)
     // удаление текущего логического элемента
// void BaseGraphElement::BGE_Del_CLE(void)
     // максимальное число логических элементов
// int  BaseGraphElement::BGE_Get_Max_LE(void)
     //сохранить список ЛЭ в файле
// void BaseGraphElement::BGE_Save_LLE(FILE *inf)
     //загрузить список ЛЭ из файла
// void BaseGraphElement::BGE_Load_LLE(FILE *inf,char maxlle)
     // текущая позиция логического элемента
// int  BaseGraphElement::BGE_Get_Cur_LE(void)
// int  BaseGraphElement::BGE_Get_NO_CGE(void)
// int  BaseGraphElement::BGE_Get_LN_CGE(void)
// void BaseGraphElement::BGE_DI_NO_CLE(int direct)     // изменение NO
// void BaseGraphElement::BGE_DI_LN_CLE(int direct)     // изменение LN
// void BaseGraphElement::Pred(void)  // переход назад по списку LE
// void BaseGraphElement::Succ(void)     // переход вперед по списку LE
// void BaseGraphElement::Search_NO_Pred(void)//поиск предыдущего NO
// void BaseGraphElement::Search_NO_Succ(void)//поиск следующего  NO
// void BaseGraphElement::Search_LN_Pred(void)//поиск предыдущего LN
// void BaseGraphElement::Search_LN_Succ(void)//поиск следующего  LN
// int  BaseGraphElement::Check_NO(unsigned char CNO,int setcur)
// int  BaseGraphElement::Check_NOnot(unsigned char CNO,int setcur)
// int  BaseGraphElement::Check_LN(unsigned char CLN,int setcur)
// int  BaseGraphElement::Check_NO_LN(unsigned char CNO,unsigned char CLN,int setcur)
// int  BaseGraphElement::Start_LLE()// сдвинуть текущий в начало
// int  BaseGraphElement::End_LLE() // сдвинуть текущий в конец

/*----  red?_031 ----*/
// Методы класса Logic::

/*----  red?_032 ----*/
//   Mетоды классa  ListLogic::

// ListLogic::ListLogic
// void  ListLogic::ListLogic_Del_Current(void)// удаление текущего элемента
// int   ListLogic::ListLogic_Get_NO(void)// NO текущего логического элемента
// int   ListLogic::ListLogic_Get_LN(void)// LN текущего логического элемента
// void ListLogic::Save_LLE(FILE *inf)//сохранить список ЛЭ в файле
// void ListLogic::Load_LLE(FILE *inf,char maxlle) //загрузить список ЛЭ из файла
// void  ListLogic::ListLogic_Print(void)//Печать на экране списка логических элементов
// void  ListLogic::ListLogic_Print_Current(void)
        // добавление элемента в конец списка /по существу в начало/
// void  ListLogic::ListLogic_Add(unsigned char AddNO,unsigned char AddLN)
        // удаление элемента из конца списка /по существу из начала/
// void  ListLogic::ListLogic_Del(void)
        // удаление элемента из списка по NO LN
// void  ListLogic::ListLogic_Del(unsigned char DNO,unsigned char DLN)
// int  ListLogic::ListLogic_Get_Max_LE(void)// максимальное число логических элементов
      // текущая позиция логического элемента
// int  ListLogic::ListLogic_Get_Cur_LE(void)
// int  ListLogic::Check_NO(unsigned char CNO,int setcur)
// int ListLogic::Check_NOnot(unsigned char CNO,int setcur)
// int  ListLogic::Check_LN(unsigned char CLN,int setcur)
// int   ListLogic::Check_NO_LN(unsigned char CNO,unsigned char CLN,int setcur)
// void  ListLogic::ListLogic_Clear(void)	  // очистка списка
// int ListLogic::Start_LLE()// сдвинуть текущий в начало
// int ListLogic::End_LLE()// сдвинуть текущий в начало
// void  ListLogic::ListLogic_di_NO(int dir)
// void  ListLogic::ListLogic_di_LN(int dir)
   	// сдвинуть текущий вперед
// int ListLogic::ListLogic_Succ()
   	// сдвинуть текущий назад
// int ListLogic::ListLogic_Pred()
// void ListLogic::Search_NO_Pred(void)//поиск предыдущего NO
// void ListLogic::Search_NO_Succ(void)//поиск следующего  NO
// void ListLogic::Search_LN_Pred(void)//поиск предыдущего LN
// void ListLogic::Search_LN_Succ(void)//поиск следующего  LN


/*----  red?_035 ----*/
// Методы класса FEllipse::



/*----  gprntf.c ----*/
#ifdef __cplusplus
extern "C" {
#endif

int gprintf(char *fmt, ...);

#ifdef __cplusplus
}
#endif


/*
      Описание внешних переменных.
   Все переменные определены в RED?_000.C
*/

extern int  MaxX,MaxY; // max значения X,Y
extern char tip_file;  // тип загруженного файла
extern int  LE_TOP;    // число логических элементов в файле
extern int  LE_TOP_Ram;// число логических элементов в рамке

extern unsigned int skankod;//код нажатой клавиши

/* определения для мыши */
extern short int key_mouse,x_mouse,y_mouse,getkey_left,getkey_right;
extern int mouse_yes;   /* есть мышь */
extern int pos_ms;      /* номер позиции мыши */
extern int pos_ms_pred; /* предыдущий номер позиции мыши */
extern char MOUSE;      // подключение драйвера мыша



/* определения для меню */
extern char *menu;/* сохранение места под меню */
extern short int xmin_m, ymin_m, xmax_m, ymax_m;// размер области 'под меню'
extern CHAR SaveTest;// признак необходимости сохранения

/* основное */
extern OBJECT **menu_1;
extern int col_act;/* активный цвет сложной команды  1,1 */
extern int col_pas;/* пассивный цвет сложной команды 1,2 */
extern int col_fon1_mn1;/* цвет фона для текста подсказок */
extern int col_fon2_mn1;/* цвет фона для заданного элемента */
extern int col_fon3_mn1;/* цвет фона для текущих значений элемента */
extern int col_fon4_mn1;/* цвет фона для парaметров списка LE */
extern int col_fon5_mn1;/* цвет фона для парaметров списка GE */


/* все для закраски */
//extern OBJECT **menu_2;/* указатель на меню_2 */

/* все для линии */
//extern OBJECT **menu_3;/* указатель на меню_3 */

/* работа с файлами */
//extern OBJECT **menu_4;/* указатель на меню_4 */

/* индикация выполнения */
extern OBJECT **menu_5;/* указатель на меню_5 */

/* меню сообщений оперативной подсказки */
extern OBJECT **menu_mess;

/* меню 6 */
//extern OBJECT **menu_6;

/* меню 7 */
extern OBJECT **menu_7;

/* меню 8 */
extern OBJECT **menu_8;

/* меню 9 */
extern OBJECT **menu_9;

/* меню 10 */
//---------------------------------------------------------------------------
extern OBJECT **menu_10;


extern int mn1_show; /* признак необходимости вывода меню */
extern int mn1_out;  /* 0-выводится 1-й раз,иначе не 1-й */


/* определения для меню =конец= */

// qqq

extern WINDOW win; /* данные окна редактора */
/*
extern int xe_min,ye_min,xe_max,ye_max;// координаты окна -экран-
extern int xa_max,ya_max;// координаты окна -абсолют-
extern int xca,yca;// координаты центра -абсолют-
extern int size_xya_red;// размер стороны окна -абсолют-
extern int xa_min_red,ya_min_red;   // координаты окна -абсолют-
extern int size_xya_red;// размер стороны окна -абсолют-
*/

extern char kf_size;// код размера окна (1-51)
extern double Kx,Ky;// коэффициенты масштабирования окна
// коэффиценты для вычисления размера масштабируемого текста
//======== size   0   1   2   3   4   5   6   7   8  9   10
extern char multx[11];
extern char multy[11];
extern char divx [11];
extern char divy [11];
extern unsigned char Multx;// текущий масштаб X
extern unsigned char Multy;// текущий масштаб Y
extern char mode_text2;//режим масштаба текста



extern int  x_strt,y_strt,x_end,y_end;/* рамка выбора */
extern int  rx_min,ry_min,rx_max,ry_max;/* рамка выбора */
//координаты рамки для сохранения .PCX
extern int  xmin_pcx,xmax_pcx,ymin_pcx,ymax_pcx;
extern int  ente;/* число нажатий enter для рамки выбора */
extern int  ind_select;  /* есть выбранные рамкой элементы */
extern int  count_select;/* сколько элементов выбано */

// режимы

extern PANEL pn_ms;  // панель режимов сохранения
extern PANEL pn_ml;  // панель режимов загрузки
extern PANEL pn_md;  // панель режимов удаления
extern PANEL pn_mc;  // панель режимов копирования
extern PANEL pn_mm;  // панель режимов сдвига
extern PANEL pn_mh;  // панель режимов перевывода
extern PANEL pn_mi;  // панель режимов индикации
extern PANEL pn_mf_t;// панель выборa стиля шрифта
extern PANEL pn_mk;  // панель режимов корректуры
extern PANEL pn_fst; // панель выборa стиля и цвета закраски
extern PANEL pn_lst; // панель выборa стиля и цвета линии
extern PANEL pn_v_t; // панель выборa выравнивания шрифта
extern PANEL pn_me;  // панель режимов выхода
extern PANEL pn_mr;  // панель Copyright




//корректура

//линия L-R-E
#define mode_korr_stl_lre 0
#define mode_korr_thi_lre 1
#define mode_korr_col_lre 2

//прямоугольник B/F/
#define mode_korr_stl_bzf 0
#define mode_korr_brd_bzf 1
#define mode_korr_col_bzf 2

//текст
#define mode_korr_stl_t 0
#define mode_korr_dir_t 1
#define mode_korr_siz_t 2
#define mode_korr_col_t 3
#define mode_korr_txt_t 4

// режимы

extern int xkom_strt,ykom_strt,xkom_end,ykom_end;/* выполнение команд рисования */
extern int ente1;
extern int ente_fld;
extern int ente_bkgr;

extern char lab[];

extern char *Obme;
extern char *Obza;

extern int xs,ys;
extern int oper[38];    /*место для курсора*/
extern int ind_move;

//extern char *area[480];

extern int size_str,i,x_min,x_max,y_min,y_max;
extern double mx; /* коэфф. масштабирования */

extern int komand;/*код команды*/

extern int pos;/* номер корректируемого параметра */
extern int korr_txt;/* необходимость корректуры текста:0-не нужно;1-нужно */

extern char text[SZ_text];

extern char adres[];
extern char s2[];
extern char f1[];
extern char f2[];
extern char sbl[SZ_sbl];

extern   char curr_file_name[80];

extern char Sname[9];       // шаблон имени
extern char Ext[MAXEXT];    // расширение
extern char Fn_fon_pcx[80]; // имя фонового файла PCX
extern char Fn_fon_tbg[80]; // имя фонового файла TBG
extern OBJECT **FonTbg;     // картинка фона TBG
extern char Fn_work_tbg[13];//имя рабочего файла TBG
extern char Fn_work_pcx[13];//имя рабочего файла PCX


extern int pcxtype;
extern char pcxpal[800];

extern struct palettetype pal;//место для своей палитры

extern char old_ig_mode; // режим в котором была инициалицирована графика
#define max_ig_mode 7    // DETECT


extern char *ARGV;//указатель на строку доступа ко всем файлам редактора


//---------------------------------------------------------------------------
//                          Описание классов
//---------------------------------------------------------------------------
class Logic {// Элемент цепочки
      unsigned char NO;// номер обьекта
      unsigned char LN;// логический номер элемента в пределах обьекта
      Logic *next;     // следующий элемент
public:
      Logic();//конструктор
      Logic(unsigned char LNO,unsigned char LLN,Logic *L);//конструктор

      void set_NO(unsigned char sno);
      void set_LN(unsigned char sln);
      void set_next(Logic *n);

      unsigned char get_NO(void);
      unsigned char get_LN(void);
      Logic*        get_next(void);

};//Logic
//---------------------------------------------------------------------------
class ListLogic:public  Logic{// цепочка односвязных элементов

	  Logic *head,*Current;

public:

ListLogic();//конструктор

void  ListLogic_Del_Current(void);// удаление текущего элемента

int   ListLogic_Get_NO(void);// NO текущего логического элемента

int   ListLogic_Get_LN(void);// LN текущего логического элемента

void  ListLogic_Set_NO(unsigned char sno);// NO текущего логического элемента
void  ListLogic_Set_LN(unsigned char sln);// LN текущего логического элемента

void Save_LLE(FILE *inf);//сохранить список ЛЭ в файле
void Load_LLE(FILE *inf,char maxlle);//сохранить список ЛЭ в файле

//void  ListLogic_Print(void);//печать значений элементов
//void  ListLogic::ListLogic_Print_Current(void);//печать текущего элемента

	  // добавление элемента в конец списка
void  ListLogic_Add(unsigned char AddNO,unsigned char AddLN);
void  ListLogic_Del(void);// удаление элемента из конца списка
	  //Удаление из списка по номеру объекта и лог.номеру элемента
void  ListLogic_Del(unsigned char DNO,unsigned char DLN);

int ListLogic_Get_Max_LE(void);// максимальное число логических элементов
int ListLogic_Get_Cur_LE(void);// текущая позиция логического элемента

void  ListLogic_Clear(void);// удаление элементов

	  // проверить наличие в списке NO и установить туда Current/если надо/
	  // YES если найден,иначе NOT.
	  // SET-установить Current
	  // NOSET- не устанавливать Current
int   Check_NO(unsigned char CNO,int setcur);

	  // проверить есть ли в списке NO отличающиеся от заданного и установить туда Current/если надо/
	  // YES если найден,иначе NOT.
	  // SET-установить Current
	  // NOSET- не устанавливать Current
int   Check_NOnot(unsigned char CNO,int setcur);

	  // проверить наличие в списке LN и установить туда Current/если надо/
	  // YES если найден,иначе NOT.
	  // SET-установить Current
	  // NOSET- не устанавливать Current
int   Check_LN(unsigned char CLN,int setcur);

	  // проверить наличие в списке NO и LN и установить туда Current/если надо/
	  // YES если найден,иначе NOT.
	  // SET-установить Current
	  // NOSET- не устанавливать Current
int   Check_NO_LN(unsigned char CNO,unsigned char CLN,int setcur);



int ListLogic_Succ();// сдвинуть текущий вперед
int ListLogic_Pred();// сдвинуть текущий назад

int  Start_LLE();// сдвинуть текущий в начало
int  End_LLE();  // сдвинуть текущий в конец

void Search_NO_Pred(void);//поиск предыдущего NO
void Search_NO_Succ(void);//поиск следующего  NO

void Search_LN_Pred(void);//поиск предыдущего LN
void Search_LN_Succ(void);//поиск следующего  LN

void ListLogic_di_LN(int direction);// DEC/INC Номера объекта
void ListLogic_di_NO(int direction);// DEC/INC Номера элемента

};//  ListLogic
//---------------------------------------------------------------------------
class BaseGraphElement{

   ListLogic LL; //Связанный список логических элементов

public:

//методы списка логических элементов

void  ClearLLE(void);// удаление списка LLE

//void ListLogic_PRINT(void);
     // добавление элемента в конец списка
void BGE_Add_LE (unsigned char no,unsigned char ln);

void BGE_Del_CLE(void);// удаление текущего логического элемента

int  BGE_Get_Max_LE(void);// максимальное число логических элементов

void BGE_Save_LLE(FILE *inf);//сохранить список ЛЭ в файле

void BGE_Load_LLE(FILE *inf,char maxlle);//загрузить список ЛЭ из файла

int  BGE_Get_Cur_LE(void);// текущая позиция логического элемента

int  BGE_Get_NO_CGE(void);
int  BGE_Get_LN_CGE(void);
void BGE_Set_NO_CGE(unsigned char sno);
void BGE_Set_LN_CGE(unsigned char sln);


void BGE_DI_NO_CLE(int direct); // изменение NO
void BGE_DI_LN_CLE(int direct);// изменение LN
void Pred(void);// переход назад по списку LE

void Succ(void);// переход вперед по списку LE

void Search_NO_Pred(void);//поиск предыдущего NO
void Search_NO_Succ(void);//поиск следующего  NO

void Search_LN_Pred(void);//поиск предыдущего LN
void Search_LN_Succ(void);//поиск следующего  LN

int  Check_NO(unsigned char CNO,int setcur);
int  Check_NOnot(unsigned char CNO,int setcur);
int  Check_LN(unsigned char CLN,int setcur);
int  Check_NO_LN(unsigned char CNO,unsigned char CLN,int setcur);

int  Start_LLE();// сдвинуть текущий в начало
int  End_LLE();  // сдвинуть текущий в конец

//методы MV MS
void Move (void);//перемещение элемента попавшего в рамку
void Mash (void);//масштабирование элемента попавшего в рамку

//виртуальные методы порожденных классов
virtual void Show(void){};
virtual void SetPipelineColor(int c){};
virtual void SetDO(int DO){};
virtual void SetS(int S){};

virtual char get_tip(void){return -1;};

virtual void put_mltx(int){};
virtual void put_mlty(int){};
virtual void put_sda (int){};

virtual void put_x(int){};
virtual void put_y(int){};
virtual void put_dx(int){};
virtual void put_dy(int){};

virtual void put_fi1(int){};
virtual void put_fi2(int){};

virtual void put_stl(int){};
virtual void put_fstl(int){};
virtual void put_thi(int){};
virtual void put_col(int){};
virtual void put_col2(int){};
virtual void put_fcol(int){};
virtual void put_brd(int){};
virtual void put_dir(int){};
virtual void put_siz(int){};
virtual void put_hor(int){};
virtual void put_ver(int){};
virtual void put_txt(char *){};
virtual void put_index(unsigned int){};
virtual void put_state(unsigned int){};
virtual void put_class(MPClass *p){};


virtual int  get_oldmltx(void){return -1;};
virtual int  get_oldmlty(void){return -1;};

virtual int  get_mltx(void){return -1;};
virtual int  get_mlty(void){return -1;};
virtual int  get_sda (void){return -1;};
virtual int  get_hor (void){return 0;};
virtual int  get_ver (void){return 0;};



virtual int  get_x(void){return -1;};
virtual int  get_y(void){return -1;};
virtual int  get_dx(void){return -1;};
virtual int  get_dy(void){return -1;};

virtual int  get_fi1(void){return -1;};
virtual int  get_fi2(void){return -1;};

virtual char get_stl(void){return -1;};
virtual char get_fstl(void){return -1;};
virtual char get_thi(void){return -1;};
virtual char get_col(void){return -1;};
virtual char get_col2(void){return -1;};
virtual char get_fcol(void){return -1;};
virtual char get_brd(void){return -1;};
virtual char get_dir(void){return -1;};
virtual char get_siz(void){return -1;};
virtual char* get_txt(void){return NULL;};
virtual unsigned int get_index(void){return 0;};
virtual unsigned int get_state(void){return 0;};
virtual MPClass*     get_class(void){return NULL;};
virtual void ClearText(void){};
virtual void Set_kf_size(void){Warning("Установка масштаба только для текста");};

virtual void clear_old(void){};//очистка всех значений выбора
virtual void set_old(void){};  //восстановление прежних параметров при масштабировании

virtual int get_oldx(void){return 0;};
virtual int get_oldy(void){return 0;};
virtual int get_olddx(void){return 0;};
virtual int get_olddy(void){return 0;};

virtual void save(FILE *){};//сохранение в файле
virtual void load(FILE *){};//загрузка из файла

             //признак 'попал/не попал' элемент в рамку
virtual char GetSelect(void){return -1;};
virtual void Vibor(void){};//определение элемента попавшего в рамку


virtual void         put_oldstate(unsigned int){};
virtual unsigned int get_oldstate(void        ){return 0;};

};//  BaseGraphElement
//---------------------------------------------------------------------------
class Line:public BaseGraphElement{

      char stl;// стиль   setlinestyle(stl,0,thi);
      char thi;// толшина setlinestyle(stl,0,thi);
      char col;// цвет    setcolor(col);
      int x;   // координата x
      int y;   // координата y
      int dx;  // приращение координаты x: + вправо
      int dy;  // приращение координаты y: + вниз

      int oldx;   // координата x старая
      int oldy;   // координата y старая
      int olddx;  // приращение координаты x старое
      int olddy;  // приращение координаты y старое
      char select;//признак ON-попал;OFF-не попал

public:

Line(void);//конструктор

void put_x(int);
void put_y(int);
void put_dx(int);
void put_dy(int);

void put_stl(int);
void put_thi(int);
void put_col(int);

int  get_x(void);
int  get_y(void);
int  get_dx(void);
int  get_dy(void);

char get_stl(void);
char get_thi(void);
char get_col(void);

void Show(void);
char get_tip(void);

void save(FILE *inf);//сохранение в файле

void load(FILE *ouf);//загрузка из файла

     //признак 'попал/не попал' элемент в рамку
char GetSelect(void);

void Vibor(void);//определение элемента попавшего в рамку

void clear_old(void);//очистка всех значений выбора
void set_old(void);  //восстановление прежних параметров при масштабировании

int get_oldx(void);
int get_oldy(void);
int get_olddx(void);
int get_olddy(void);

};// Line
//---------------------------------------------------------------------------
class Rectangle:public BaseGraphElement{

      char stl;// стиль   setlinestyle(stl,0,thi);
      char thi;// толшина setlinestyle(stl,0,thi);
      char col;// цвет    setcolor(col);
      int x;   // координата x
      int y;   // координата y
      int dx;  // приращение координаты x: + вправо
      int dy;  // приращение координаты y: + вниз

      int oldx;   // координата x старая
      int oldy;   // координата y старая
      int olddx;  // приращение координаты x старое
      int olddy;  // приращение координаты y старое
//      int rx;     // расстояние до рамки выбора:верх-лево
//      int ry;     // расстояние до рамки выбора:верх-лево
      char select;//признак ON-попал;OFF-не попал

public:
Rectangle(void);//конструктор
void put_x(int);
void put_y(int);
void put_dx(int);
void put_dy(int);
void put_stl(int);
void put_thi(int);
void put_col(int);

int  get_x(void);
int  get_y(void);
int  get_dx(void);
int  get_dy(void);
char get_stl(void);
char get_thi(void);
char get_col(void);

void Show(void);

char get_tip(void);

void save(FILE *inf);//сохранение в файле

void load(FILE *ouf);//загрузка из файла

     //признак 'попал/не попал' элемент в рамку
char  GetSelect(void);

void Vibor(void);//определение элемента попавшего в рамку

void clear_old(void);//очистка всех значений выбора
void set_old(void);  //восстановление прежних параметров при масштабировании

int get_oldx(void);
int get_oldy(void);
int get_olddx(void);
int get_olddy(void);
//int get_rx(void);
//int get_ry(void);


};// Rectangle
//---------------------------------------------------------------------------
class Bar:public BaseGraphElement{

      char stl;// стиль   setfillstyle(stl,col);
      char col;// цвет    setfillstyle(stl,col);
      int x;   // координата x
      int y;   // координата y
      int dx;  // приращение координаты x: + вправо
      int dy;  // приращение координаты y: + вниз

      int oldx;   // координата x старая
      int oldy;   // координата y старая
      int olddx;  // приращение координаты x старое
      int olddy;  // приращение координаты y старое
//      int rx;     // расстояние до рамки выбора:верх-лево
//      int ry;     // расстояние до рамки выбора:верх-лево
      char select;//признак ON-попал;OFF-не попал


public:
Bar(void);//конструктор

void put_x(int);
void put_y(int);
void put_dx(int);
void put_dy(int);
void put_stl(int);
void put_col(int);

int  get_x(void);
int  get_y(void);
int  get_dx(void);
int  get_dy(void);
char get_stl(void);
char get_col(void);


void Show(void);

char get_tip(void);

void save(FILE *inf);//сохранение в файле

void load(FILE *ouf);//загрузка из файла

     //признак 'попал/не попал' элемент в рамку
char  GetSelect(void);

void Vibor(void);//определение элемента попавшего в рамку

void clear_old(void);//очистка всех значений выбора
void set_old(void);  //восстановление прежних параметров при масштабировании

int get_oldx(void);
int get_oldy(void);
int get_olddx(void);
int get_olddy(void);
//int get_rx(void);
//int get_ry(void);

};// Bar
//---------------------------------------------------------------------------
class Ellipse:public BaseGraphElement{

      char stl;// стиль   setlinestyle(stl,0,thi);
      char thi;// толшина setlinestyle(stl,0,thi);
      char col;// цвет    setcolor(col);
      int x;   // координата x
      int y;   // координата y
      int dx;  // радиус по X
      int dy;  // радиус по Y
      int fi1;  // начальный угол /градус/
      int fi2;  // конечный  угол /градус/

      int oldx;   // координата x старая
      int oldy;   // координата y старая
      int olddx;  // приращение координаты x старое
      int olddy;  // приращение координаты y старое
//      int rx;     // расстояние до рамки выбора:верх-лево
//      int ry;     // расстояние до рамки выбора:верх-лево
      char select;//признак ON-попал;OFF-не попал

public:
Ellipse(void);//конструктор

void put_x(int);
void put_y(int);
void put_dx(int);
void put_dy(int);
void put_fi1(int);
void put_fi2(int);
void put_stl(int);
void put_thi(int);
void put_col(int);

int  get_x(void);
int  get_y(void);
int  get_dx(void);
int  get_dy(void);
int  get_fi1(void);
int  get_fi2(void);
char get_stl(void);
char get_thi(void);
char get_col(void);

void Show(void);

char get_tip(void);

void save(FILE *inf);//сохранение в файле

void load(FILE *ouf);//загрузка из файла

     //признак 'попал/не попал' элемент в рамку
char  GetSelect(void);

void Vibor(void);//определение элемента попавшего в рамку

void clear_old(void);//очистка всех значений выбора
void set_old(void);  //восстановление прежних параметров при масштабировании

int get_oldx(void);
int get_oldy(void);
int get_olddx(void);
int get_olddy(void);
//int get_rx(void);
//int get_ry(void);

};// Ellipse
//---------------------------------------------------------------------------
class FEllipse:public BaseGraphElement{

      char stl;// стиль линии  setlinestyle(stl,0,thi);
      char fstl;// стиль заливки  setfillstyle(stl,col);
      char thi;// толшина setlinestyle(stl,0,thi);
      char col;// цвет линии setcolor(col);
      char fcol;// цвет заливки setfillstyle(stl,col);
      int x;   // координата x
      int y;   // координата y
      int dx;  // радиус по X
      int dy;  // радиус по Y
      int fi1;  // начальный угол /градус/
      int fi2;  // конечный  угол /градус/

      int oldx;   // координата x старая
      int oldy;   // координата y старая
      int olddx;  // приращение координаты x старое
      int olddy;  // приращение координаты y старое
//      int rx;     // расстояние до рамки выбора:верх-лево
//      int ry;     // расстояние до рамки выбора:верх-лево
      char select;//признак ON-попал;OFF-не попал

public:
FEllipse(void);//конструктор

void put_x(int);
void put_y(int);
void put_dx(int);
void put_dy(int);
void put_fi1(int);
void put_fi2(int);
void put_stl(int);
void put_fstl(int);
void put_thi(int);
void put_col(int);
void put_fcol(int);

int  get_x(void);
int  get_y(void);
int  get_dx(void);
int  get_dy(void);
int  get_fi1(void);
int  get_fi2(void);
char get_stl(void);
char get_fstl(void);
char get_thi(void);
char get_col(void);
char get_fcol(void);

void Show(void);

char get_tip(void);

void save(FILE *inf);//сохранение в файле

void load(FILE *ouf);//загрузка из файла

     //признак 'попал/не попал' элемент в рамку
char  GetSelect(void);

void Vibor(void);//определение элемента попавшего в рамку

void clear_old(void);//очистка всех значений выбора
void set_old(void);  //восстановление прежних параметров при масштабировании

int get_oldx(void);
int get_oldy(void);
int get_olddx(void);
int get_olddy(void);
//int get_rx(void);
//int get_ry(void);

};// FEllipse
//---------------------------------------------------------------------------
class Text:public BaseGraphElement{

      char fnt;// фонт шрифта
      char dir;// ориентация текста
      char siz;// размер символа текста
      char col;// цвет    setcolor(col);
      int  x;  // координата x
      int  y;  // координата y
      char max;// размер текста
      char hor;// выравнивание по горизонтали
      char ver;// выравнивание по вертикали
      int  sda;// размер окна при создании текста
unsigned char mltx; // увеличение при плавном масштабе
unsigned char mlty; // увеличение при плавном масштабе
      char *txt;//начало строки текста
      char tip_txt;//тип текста для интерпретации

      int oldx;   // координата x старая
      int oldy;   // координата y старая
      int oldmltx;// увеличение при плавном масштабе старое
      int oldmlty;// увеличение при плавном масштабе старое

      char select;//признак ON-попал;OFF-не попал

public:

Text(void);//конструктор

inline void put_mltx(int);
inline void put_mlty(int);
inline void put_sda (int);

inline void put_x(int);
inline void put_y(int);
inline void put_stl(int);
inline void put_col(int);
inline void put_dir(int);
inline void put_siz(int);
inline void put_hor(int);
inline void put_ver(int);

       void put_txt(char *);

inline int  get_oldmltx(void);
inline int  get_oldmlty(void);
inline int  get_mltx(void);
inline int  get_mlty(void);
inline int  get_sda (void);
inline int  get_hor(void);
inline int  get_ver(void);



inline int  get_x(void);
inline int  get_y(void);
inline char get_stl(void);
inline char get_col(void);
inline char get_dir(void);
inline char get_siz(void);
inline char* get_txt(void);
inline void ClearText(void);
       void Set_kf_size(void);
       void style(void);

inline int get_oldx(void);
inline int get_oldy(void);

       void Show(void);

inline char get_tip(void);

void save(FILE *inf);//сохранение в файле

void load(FILE *ouf);//загрузка из файла

     //признак 'попал/не попал' элемент в рамку
inline char  GetSelect(void);

void Vibor(void);//определение элемента попавшего в рамку

inline void clear_old(void);//очистка всех значений выбора
inline void set_old(void);  //восстановление прежних параметров при масштабировании


};// Text
//---------------------------------------------------------------------------
class Floodfill:public BaseGraphElement{

      char stl;// стиль   setfillstyle(stl,col);
      char col;// цвет    setfillstyle(stl,col);
      char brd;// цвет границы floodfill(x,y,brd);
      int x;   // координата x
      int y;   // координата y

      int oldx;   // координата x старая
      int oldy;   // координата y старая
//      int rx;     // расстояние до рамки выбора:верх-лево
//      int ry;     // расстояние до рамки выбора:верх-лево
      char select;//признак ON-попал;OFF-не попал

public:

Floodfill(void);//конструктор

void put_x(int);
void put_y(int);
void put_stl(int);
void put_col(int);
void put_brd(int);

int  get_x(void);
int  get_y(void);
char get_stl(void);
char get_col(void);
char get_brd(void);

void Show(void);

char get_tip(void);

void save(FILE *inf);//сохранение в файле

void load(FILE *ouf);//загрузка из файла

     //признак 'попал/не попал' элемент в рамку
char  GetSelect(void);

void Vibor(void);//определение элемента попавшего в рамку

void clear_old(void);//очистка всех значений выбора
void set_old(void);  //восстановление прежних параметров при масштабировании

int get_oldx(void);
int get_oldy(void);
//int get_rx(void);
//int get_ry(void);


};// Floodfill
//---------------------------------------------------------------------------
class Background:public BaseGraphElement{

      char col;// цвет    setbkcolor(col);
//      char select;//признак ON-попал;OFF-не попал

public:
Background(void);//конструктор

int  get_x(void);
int  get_y(void);

void put_col(int);

void Show(void);
//void Vibor(void);

char get_tip(void);

void save(FILE *inf);//сохранение в файле

void load(FILE *ouf);//загрузка из файла

};// Background
//---------------------------------------------------------------------------
class Macro:public BaseGraphElement{

      unsigned int index;/* Индекс */
      int          mcx;  // координата x
      int          mcy;  // координата y
      unsigned int mode; /* режим */
      unsigned int cs;   /* состояние */

      MPClass     *cl;   /* класс макропримитива (не C++)*/
      int oldx;          // координата x старая
      int oldy;          // координата y старая
      unsigned int olds; /* состояние старое */
      char select;       //признак ON-попал;OFF-не попал

public:
Macro(void);//конструктор

inline void Macro::put_x(int p);
inline void Macro::put_y(int p);
inline void Macro::put_index(unsigned int i);
inline void Macro::put_state(unsigned int s);
inline void Macro::put_oldstate(unsigned int s);
inline void Macro::put_class(MPClass *p);

inline int  Macro::get_x(void);
inline int  Macro::get_y(void);
inline unsigned int Macro::get_index(void);
inline unsigned int Macro::get_state(void);
inline unsigned int Macro::get_oldstate(void);
inline MPClass *Macro::get_class(void);
inline char Macro::get_tip(void);

void Show(void);
void save(FILE *inf);//сохранение в файле
void load(FILE *ouf);//загрузка из файла
void Vibor(void);//определение элемента попавшего в рамку

     //признак 'попал/не попал' элемент в рамку
inline char Macro::GetSelect(void);

inline int  Macro::get_oldx(void);
inline int  Macro::get_oldy(void);
inline void Macro::clear_old(void);//очистка всех значений выбора
inline void Macro::set_old(void);  //восстановление прежних параметров при масштабировании

};// Macro
//---------------------------------------------------------------------------
class Reper:public BaseGraphElement{

      int x;       // координата x
      int y;       // координата y

      int oldx;    // координата x старая
      int oldy;    // координата y старая
      char select; //признак ON-попал;OFF-не попал

public:
Reper(void);//конструктор

inline void Reper::put_x(int p);
inline void Reper::put_y(int p);
inline int  Reper::get_x(void);
inline int  Reper::get_y(void);

inline char Reper::get_tip(void);

void save(FILE *inf);//сохранение в файле
void load(FILE *ouf);//загрузка из файла
void Vibor(void);//определение элемента попавшего в рамку

     //признак 'попал/не попал' элемент в рамку
inline char Reper::GetSelect(void);

inline int  Reper::get_oldx(void);
inline int  Reper::get_oldy(void);
inline void Reper::clear_old(void);//очистка всех значений выбора
inline void Reper::set_old(void);  //восстановление прежних параметров при масштабировании

};// Reper
//---------------------------------------------------------------------------
class PipeLine:public BaseGraphElement{

      char stl;// стиль   setlinestyle(stl,0,thi);
      char thi;// толшина setlinestyle(stl,0,thi);
      int x;   // координата x
      int y;   // координата y
      int dx;  // приращение координаты x: + вправо
      int dy;  // приращение координаты y: + вниз

      PIPELINE pl; // Pipeline struct

      char color;
      int oldx;   // координата x старая
      int oldy;   // координата y старая
      int olddx;  // приращение координаты x старое
      int olddy;  // приращение координаты y старое
      char select;//признак ON-попал;OFF-не попал

public:

PipeLine(void);//конструктор

void put_x(int);
void put_y(int);
void put_dx(int);
void put_dy(int);

void put_stl(int);
void put_thi(int);
void put_col(int);
void put_col2(int);

int  get_x(void);
int  get_y(void);
int  get_dx(void);
int  get_dy(void);

char get_stl(void);
char get_thi(void);
char get_col(void);
char get_col2(void);

void Show(void);
void SetPipelineColor(int c);
void PipeLine::SetDO(int DO);
void PipeLine::SetS(int S);

char get_tip(void);

void save(FILE *inf);//сохранение в файле

void load(FILE *ouf);//загрузка из файла

     //признак 'попал/не попал' элемент в рамку
char GetSelect(void);

void Vibor(void);//определение элемента попавшего в рамку

void clear_old(void);//очистка всех значений выбора
void set_old(void);  //восстановление прежних параметров при масштабировании

int get_oldx(void);
int get_oldy(void);
int get_olddx(void);
int get_olddy(void);

};// PipeLine
//---------------------------------------------------------------------------
class Bitmap:public BaseGraphElement{

      int x;   // координата x
      int y;   // координата y
      int dx;  // приращение координаты x: + вправо
      int dy;  // приращение координаты y: + вниз
      char FileName[13];

      int oldx;   // координата x старая
      int oldy;   // координата y старая
      int olddx;  // приращение координаты x старое
      int olddy;  // приращение координаты y старое
//      int rx;     // расстояние до рамки выбора:верх-лево
//      int ry;     // расстояние до рамки выбора:верх-лево
      char select;//признак ON-попал;OFF-не попал

public:
Bitmap(void);//конструктор
void put_x(int);
void put_y(int);
void put_dx(int);
void put_dy(int);

int  get_x(void);
int  get_y(void);
int  get_dx(void);
int  get_dy(void);

void Show(void);

char get_tip(void);

void save(FILE *inf);//сохранение в файле

void load(FILE *ouf);//загрузка из файла

     //признак 'попал/не попал' элемент в рамку
char  GetSelect(void);

void Vibor(void);//определение элемента попавшего в рамку

void clear_old(void);//очистка всех значений выбора
void set_old(void);  //восстановление прежних параметров при масштабировании

int get_oldx(void);
int get_oldy(void);
int get_olddx(void);
int get_olddy(void);
//int get_rx(void);
//int get_ry(void);


};// Bitmap
//---------------------------------------------------------------------------
class LIST{

BaseGraphElement **list;
int PN;         //порядковый номер графического элемента
int PN_TOP;     //сколько всего графических элементов
int MAX_BGE;
//char tip_search;//тип графического элемента для поиска


int  SaveOne(FILE *inf,int Index);//сохранение одного элемента
void CopyOne(int src,int dst); //копирование текущего ГЭ
void CopyOneMirrorX(int src,int dst); //копирование текущего ГЭ с отр. по X
void CopyOneMirrorY(int src,int dst); //----------- " " -------------- по Y

public:

LIST(int InitMax);// конструктор

FILE* CreateInputFile(char *fname);//создать файл для вывода
void SaveAll(char *fname,char mode);// сохранение всего изображения
//void SaveRam(char *fname);// сохранение всего изображения в рамке
//void SaveGE (char *fname);// сохранение текущего ГЭ
int  LoadFile(char *fname);// загрузка изображения из файла

void DI_fi1(int direct);

void DI_fi2(int direct);

void ShowAll(void);//перевывод всего изображения
void ShowNO(void); //перевывод обьекта NO
void ShowLN(void); //перевывод LN обьекта

int  CreateGE(int tip);

void IncPN(void);
void DecPN(void);

void Search_Tip_Pred_LGE(void);//поиск предыдущего типа в списке GE
void Search_Tip_Succ_LGE(void);//поиск следующего  типа в списке GE
char* GetText(void);

MPClass *GetMPClass(void);/* получить указатель на класс макропримитива */

void ShowCur(void);

void SetPipeLineColor(int c);
void ShowPipe(void); //перевывод LN обьекта
void SetDO(int DO){list[PN]->SetDO(DO);};
void SetS(int S){list[PN]->SetS(S);};

int  GetPN(void);
int  GetPN_TOP(void);
int  GetMAX_BGE(void);

void EndPN(void);
void StartPN(void);
//char GetTipSearch(void);

//void DecTipSearch(void);
//void IncTipSearch(void);

char GetTip(void);
int  GetX(void);
int  GetY(void);
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int  GetX2(void);
int  GetY2(void);

void SetX(int x);
void SetY(int y);
void SetX2(int x);
void SetY2(int y);

void SetColor(int color){list[PN]->put_col(color);};
void SetFColor(int color){list[PN]->put_fcol(color);};
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     // добавление элемента в конец списка
void AddLE(unsigned char no,unsigned char ln);
     // удаление текущего логического элемента
void DelCLE(void);
     // максимальное число логических элементов
int  GetMaxLE(void);
     // текущая позиция логического элемента
int  GetCurLE(void);
     // номер обьекта текущего графического элемента
int  Get_NO_CGE(void);
     // логический номер текущего графического элемента
int  Get_LN_CGE(void);
     // номер обьекта текущего графического элемента
void Set_NO_CGE(unsigned char sno);
     // логический номер текущего графического элемента
void Set_LN_CGE(unsigned char sln);
     // добавление логического номера из текущего ГЭ
	  // к объектам попавшим в рамку
void Add_LE_to_GE_in_ramca_from_current_GE(void);
     // удаление логики у ГЭ попавших в РАМКУ
void Del_LE_to_GE_in_ramca(void);

     // изменение NO
void DI_NO_CLE(int direct);
     // изменение LN
void DI_LN_CLE(int direct);
     // переход назад по списку LE
void Pred(void);
     // переход вперед по списку LE
void Succ(void);
void Search_NO_Pred(void);
void Search_NO_Succ(void);
void Search_NO_Pred_LGE(void);//поиск предыдущего NO в списке GE
void Search_NO_Succ_LGE(void);//поиск следующего  NO в списке GE

void Search_LN_Pred_LGE(void);//поиск предыдущего LN в списке GE
void Search_LN_Succ_LGE(void);//поиск следующего  LN в списке GE

     //поиск предыдущего LN равного текущему в списке GE
void Search_All_LN_Pred_LGE(void);

     //поиск следующего LN равного теущему в списке GE
void Search_All_LN_Succ_LGE(void);

void Search_LN_Pred(void);
void Search_LN_Succ(void);

	  // проверить наличие в списке NO и LN и установить туда Current/если надо/
	  // YES если найден,иначе NOT.
	  // SET-установить Current
	  // NOSET- не устанавливать Current
int   Check_NO_LN(unsigned char CNO,unsigned char CLN,int setcur){
   return list[PN]->Check_NO_LN(CNO,CLN,setcur);
};

char GetSelect(void){
   return list[PN]->GetSelect();
};

int  Start_LLE();
int  End_LLE();

void DelGE(void);//удаление текущего ГЭ
void DelGE_All(void);//удаление всех ГЭ
void DelGE_Ram(void);//удаление ГЭ в рамке
void Set_kf_size(void);//установка масштаба окна по тексту


     //признак 'попал/не попал' элемент в рамку

void Vibor  (void);//определение элементов попавших в рамку
void MoveRam(void);//перемещение элементов попавших в рамку
void MoveGE(void);//перемещение текущего ГЭ
int  MashRam(void);//масштабирование элементов попавших в рамку
void SetOld (void);//восстановление прежних параметров при масштабировании
void ClearOld(void);//очистка всех значений выбора

void CopyRam(void);//копирование элементов попавших в рамку
void CopyGE(void); //копирование текущего ГЭ

void IndicNO(void); //индикация NO
void IndicLN(void); //индикация LN
void IndicGE(void); //индикация текущего ГЭ

void Korr(void);//корректура всего что надо

};//  LIST
//---------------------------------------------------------------------------

extern LIST pic;    /*  основная картинка */
//extern LIST *fonpic;/*  фоновая картинка  */