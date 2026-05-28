// panel.h =============================================================

#ifndef PANEL_H
#define PANEL_H

#include "tbg_tc.h"

typedef unsigned char BYTE;
typedef   signed char CHAR;


#define f_mono 1      // поле однозначного выбора
#define f_poli 2      // поле многозначного выбора
#define f_info 3      // поле информационное
#define SHOW_FLD 1    // показ изменений
#define NO_SHOW_FLD 0 // не показ изменений

typedef struct field
{
 char type;         // тип поля:f_mono...
 char maxmode;      // размерность поля данных
 char posfld;       // номер активной строки поля
 char *mode;        // переменная(ные) режима поля
 unsigned char objfld; // номер обьекта поля
 char iv;           // признак наличия индикации выбора позиции
 char iu;           // признак наличия индикации указания позиции
 void (*funfld)(OBJECT **,...); // функция выполняющаяся при выборе позиции
} FIELD; // описание одного поля панели

typedef struct panel
{
 char *fname;       // имя .TBG файла панели
 char *fname_col;   // имя .TBG файла цветов панели
 char maxpnl;       // число полей
 char pospnl;       // номер активного поля панели
 FIELD **pf;        // описание полей панели
 void (*init_fun)(OBJECT **); // функция выполняющаяся при вызове панели
} PANEL; // описание всей панели

#ifdef __cplusplus
extern "C" {
#endif
int  panel_mode (PANEL *pnl);// установка режимов панели
void show_panel (PANEL *pnl,OBJECT **menu,unsigned char numpnl);// показ режимов панели
void show_pospnl(PANEL *pnl,OBJECT **menu);// показ позиции поля на панели
void show_posfld(PANEL *pnl,OBJECT **menu);// показ указанной позиции поля
void Store_Image  (OBJECT **pic,char *filename);
void Restore_Image(OBJECT **pic,char *filename);

void mouse_draw(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

/* ----------------------------------------------------------------------- */
/* переключить флаг запуска функции выполняющейся при выборе позиции */
/*void set_fRUN (void);*/
/* ----------------------------------------------------------------------- */
#endif  //PANEL_H
// end panel.h =========================================================