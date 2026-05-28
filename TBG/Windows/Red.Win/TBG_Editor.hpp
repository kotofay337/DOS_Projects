/* TBG_Editor.HPP
*
*   TBG v7.0
*   20-09-2001 8:33 am msk
*
*/

#ifndef __TBG_EDITOR_HPP__
#define __TBG_EDITOR_HPP__

#include "tbg.hpp"

/* -------------------------------------------------------------------------- */
// Определение типов данных для редактора
/* -------------------------------------------------------------------------- */

// Команды редактора
enum Commands{
   cZero = 0,   // Нет команды

     // команды выбора ГП
   cSelect,     // Выбор примитива
   cSelectRect, // Выбор рамкой

     // удаление ГП
   cErase,      // Стирание примитива/примитивов

     // редактирование ГП текущего или выбранных
   cMove,       // Перенос ---"---
   cMoveCrd,    // Редактирование координат выбранных ГП
   cRotate,     // Поворот ---"---
   cZoom,       // Масштабирование ---"---
   cEdit,       // Коррекция ---"---
   cAddLE,      //
   cDelLE,      //

     // команды создания ГП
   cCopy,       // Копирование ---"---

   // Points
   cPoint,      // Точка
   cReperPoint, // Невидимая точка
   // Lines
   cLine,       // Линия сплошная
   cStyleLine,  //  + Стиль
   cPolyline,   // Ломаная
   cPolygon,    // Полигон
   cPipe,       // Труба
   cBezier,     // Кривая Безье
   // Rectangles
   cFrameRect,  // Прямоугольник через 2 точки
   cPolyRect,   // Прямоугольник через 4 точки
   cRoundRect,  //  + скруглённые углы
   cFillRect,   //  + заполнение
   cFillFrameRect, // + заполнение + граница
   cGradientFillFrameRect, // Градиентная заливка
   cPolyFillFrameRect, // + заполнение + граница 4 точки
   cHideRect,   // Невидимый прямоугольник
   // Ellipses
   cEllipse,      // Эллипс
   cFillEllipse,  //  + заполнение
   cRotateEllipse,//  + поворот осей
   cCircle,       // Окружность
   cFilledCircle, //  + заполнение
   cArc,          // Дуга
   cFillArc,      // Сектор
   cChord,        // Хорда
   // Text
   cTextBGI,      // Текст фонтами BGI OEM
   cTextTrueType, // Текст TrueType
   cFrameText,    // Текст вписанный в рамку
   // Extent
   cMacro,        // Макропримитив TBG
   cPicture,      // Картинки TBG
   cImage,        // Картинки растровые
   cIcon,         // Иконки
   cResource,     // Ресурсы Windows: Кнопки,
                  // редактируемые строки,списки и т.д.
   cMultimedia    // Окно мультимедиа
};

/* -------------------------------------------------------------------------- */
// Описание глобальных переменных
/* -------------------------------------------------------------------------- */
extern int ARGC;
extern char **ARGV, *ENV;

extern TPictureTBG *EditPic;

extern int MouseX, OffsetX,
           MouseY, OffsetY;

extern TColor ColorLine,
              ColorFill,
              ColorText;

extern TPenStyle   StyleLine;
extern TBrushStyle StyleFill;
extern GradientFillStyle GradientStyleFill;

extern char sTextBGI[256];
extern int  iFntBGI;
extern int  SizeFntBGI;
extern double dAngleText;
              
extern unsigned char WidthLine;

extern TColor EGAColors[16];

extern Commands pc;

extern LinkList<TBaseGP*> cb; // Clipboard

extern OSVERSIONINFO vos;

/* -------------------------------------------------------------------------- */
// Описание прототипов функций
/* -------------------------------------------------------------------------- */
inline bool Selectoring(void){
   return pc == cSelect ||
          pc == cSelectRect;
}// Selectoring
//---------------------------------------------------------------------------
inline bool Editing(void){
   return pc == cMove ||
          pc == cEdit ||
          pc == cRotate ||
          pc == cMoveCrd ||
          pc == cCopy ||
          pc == cZoom ;
}// Selectoring
//---------------------------------------------------------------------------
inline bool Deleting(void){
   return pc == cErase;
}// Selectoring
//---------------------------------------------------------------------------
inline bool Creation(void){
   return pc > cCopy;
}// Selectoring
//---------------------------------------------------------------------------
void shpLineInvalidate(void);
void shpRectInvalidate(void);
void shpTextInvalidate(void);

void ClearClipboard(void);
void rotate_point(double a,double *Xt,double *Yt,double Xc,double Yc);
void ShowSelector(TCanvas *cvs, int x, int y, bool select);
void ShowCoordSelector(TWorkPlace *, int x, int y);

#endif
