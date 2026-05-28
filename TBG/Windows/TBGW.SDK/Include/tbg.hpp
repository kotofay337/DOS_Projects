/* TBG.HPP
*
*   TBG v7.0
*   20-09-2001 8:33 am msk
*
*/

#ifndef __TBG_HPP__
#define __TBG_HPP__

#include <windows.h>  
#include "gdiplus.h"  // GDI+ 
#include <wingdi.h>

#include <ComCtrls.hpp>
#include <inifiles.hpp>

#include <stdio.h>
#include <string.h>
#include <cstring.h>

#include "DList.HPP" // Double list - templates

//using namespace Graphics;
//using namespace Gdiplus;
//using namespace DllExports;
/* -------------------------------------------------------------------------- */
// Макросы
// Macroses
/* -------------------------------------------------------------------------- */
// Отладочная печать
// Debug operators
//#define D(f) (f)
#define D(f)

// probable absent
#ifndef min
#define min(a, b) ((a)<(b)?(a):(b))
#define max(a, b) ((a)>(b)?(a):(b))
#endif

/* -------------------------------------------------------------------------- */
// Описание констант
// Constants
/* -------------------------------------------------------------------------- */
// Degree TO Radians
#define RAD ((double)57.29577951472)

/* -------------------------------------------------------------------------- */
// Описание типов данных
// Types data
/* -------------------------------------------------------------------------- */

// Значения типов ГП
// GP Type enum
enum TypeGP {
   tUnknown       = -2,
   tSet           = -1, //
   tZero          =  0, //
   tSmooth        =  1, //
   
   tPoint         = 10, //
    tReperPoint   = 11, //

   tLine          = 20, // Линия/Simple Line
    tStyleLine    = 21, //
    tPolyLine     = 22, //
    tPolygon      = 23, //
    tBezier       = 24, //
    tPipe         = 25, //

   tFrameRect     = 30, //
    tRoundRect    = 31, //
    tPolyRect     = 32, //
    tFillRect     = 33, //
    tFillPolyRect = 34, //
    tHideRect     = 35, //
    tGradientFillRect = 36, //

   tEllipse       = 40, //
    tFillEllipse  = 41, // Овал/Ellipse
    tArc          = 42, // Дуга/Arc
    tFillArc      = 43, // Сектор/Pie

   tBGIText       = 50, // DOS codetable
   tTrueTypeText  = 51, // ANSI codetable

   tMacro         = 60 // Extents macroprimitives
};

// ID KeyWords
enum TypeKW {
   kwUnknown = 999,
   kwHide,
   kwLogic,
   kwCoords,
   kwFGColor,
   kwBGColor,
   kwWidth,
   kwFillStyle,
   kwStartAngle,
   kwEndAngle,
   kwAngle,
   kwText,
   kwIndex,
   kwState,
   kwDataPipe,
   kwDataBGIText,
   kwBlur,
   kwBlurToGrayscale,
   kwFilter
};

typedef
   enum tagGradientFillStyle{
      gfsLeftToRight,
      gfsLeftTopToRightBottom,
      gfsUpToDown,
      gfsRightTopToLeftBottom,
      gfsVertical,
      gfsDiagonal,
      gfsHorizontal,
      gfsBackDiagonal,
      gfsLeftTopCorner,
      gfsRightTopCorner,
      gfsRightBottomCorner,
      gfsLeftBottomCorner,
      gfsCenter
   }GradientFillStyle;

// Type reading values
// used for ReadValue
enum {
   rINT,
   rHEX,
   rDOUBLE,
   rSHORTINT
};

#ifdef _EDITOR_
// Опреации над ГП
// GP operations
enum Operations {
   Nop = 0,
   Start,
   Stop
};

// режимы вывода выбранного ГП
// Out GP modes
enum SSMode {
   SSNormal,
   SSSelect,
   SSIndicate,
   SSNone
};

enum CopyGEMode{
   cmWithLogic,
   cmWithOutLogic,
   cmMirror,
   cmMirrorX,
   cmMirrorY
};
#endif

// Show GP modes
enum TypeShow {
   Show,
   NoShow
};

#include "ColorEx.hpp"     // Extent color (HLS, HSV from/to RGB)/not used temporarly
#include "Pointf.hpp"      // Extent Point class
#include "WorkPlace.hpp"   // Interface class
#include "CoordSpace.hpp"  // Coordinate class
#include "Logic.hpp"       // Logic structure in a Picture

// Тип Базовый Графический Примитив
//  обобщает все графические примитивы
//  используемые в системе TBG,
//  абстрагирует конкретные ГП
//  от особенностей реализации

class TBaseGP : public TCoordSpace, public TListLogic {
protected:
   TypeGP      pType;     // тип ГП
   bool        Active;    // флаг возможности выбора
   bool        Selected;  // флаг выбора
   bool        pSmooth;   // флаг сглаживания
   bool        pHide;     // флаг скрытого ГП
   bool        pUpdate;   // флаг изменения ГП
   TWorkPlace *pwp;       // область отображения
public:
   ~TBaseGP();
   __fastcall TBaseGP();

   inline void SetHideState(bool to){ pHide = to; };
   inline bool GetHideState(void){return pHide;};
   __property bool Hide = {read=GetHideState, write=SetHideState};

   inline void SetUpdateState(bool to){ pUpdate = to; };
   inline bool GetUpdateState(void){return pUpdate;};
   __property bool Update = {read=GetUpdateState, write=SetUpdateState};

   TypeGP __fastcall GetType(void){return pType;};
   void __fastcall SetType(TypeGP s){pType = s;};
   __property TypeGP Type = {read=GetType, write=SetType};
   
   virtual bool __fastcall Hit(TWorkPlace* wp, TPoint p);

#ifdef _EDITOR_
   virtual AnsiString __fastcall GetName(void);

   virtual int __fastcall MouseDown(TWorkPlace*, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   virtual int __fastcall MouseMove(TWorkPlace*, TObject *Sender, TShiftState Shift, int X, int Y);
   virtual bool __fastcall Select(TWorkPlace* wp, TPoint p, TShiftState Shift);
   virtual bool __fastcall Select(TWorkPlace* wp, TRect r, TShiftState Shift);
   virtual void __fastcall ShowSelect(TWorkPlace*, SSMode);

   virtual TBaseGP* __fastcall Copy(CopyGEMode m);

   virtual void __fastcall Save(FILE *f);
   virtual void __fastcall SaveAsBase(FILE *f);
   inline void SetSelect(bool to){Selected = to;};
   inline bool GetSelect(void){return Selected;};
   __property bool IsSelect = {read=GetSelect, write=SetSelect};

   TPointf* __fastcall HitToPoint(TPointf hp);

#endif

   virtual bool __fastcall Load(char *buf);

   virtual void __fastcall Show(TWorkPlace*);
   virtual void __fastcall ShowWithColor(TWorkPlace* c, TColor cl);
   virtual void __fastcall ShowWithRotate(TWorkPlace* c, TPointf p, double a);

   virtual void __fastcall SetWidthLine(unsigned w);
   virtual unsigned __fastcall GetWidthLine(void);
   __property unsigned WidthLine = {read=GetWidthLine, write=SetWidthLine};

   virtual void __fastcall SetColor(TColor cl);
   virtual TColor __fastcall GetColor(void);
   __property TColor LineColor = {read=GetColor, write=SetColor};

   virtual void __fastcall SetFillColor(TColor cl);
   virtual TColor __fastcall GetFillColor(void);
   __property TColor FillColor = {read=GetFillColor, write=SetFillColor};

   virtual void __fastcall SetFillStyle(TBrushStyle cl);
   virtual TBrushStyle __fastcall GetFillStyle(void);

   virtual void __fastcall SetPhiStart(TPointf &p);
   virtual void __fastcall SetPhiEnd(TPointf &p);
   virtual TPointf& __fastcall GetPhiStart(void);
   virtual TPointf& __fastcall GetPhiEnd(void);

   virtual void __fastcall Smoothing(bool to = true);
   virtual bool __fastcall Smoothable(void);
   __property bool Smooth = {read=Smoothable, write=Smoothing};

};// BaseGP

// --------------------------------------------------------------------------
// Кэширование логических элементов
class ReferenceLE{
   TLogic le;
   LinkList<TBaseGP*> lstgp;
public:
   ~ReferenceLE(){
      while(!lstgp.Empty())
         lstgp.DeleteFromHead();
   };
};

class CacheLE{
   LinkList<ReferenceLE*> cache;
public:
   ~CacheLE(){
      while(!cache.Empty())
         delete cache.DeleteFromHead();
   };

   bool AddLE(unsigned short No, unsigned short Ne, TBaseGP *gp){
      bool ret = false;
      // пройти по всем откэшированным записям
      // если нет, то добавить в список новую запись
      // добавить в найденную(новую) запись указатель на ГП
      return ret;
   };

   TBaseGP* SearchLE(unsigned short No, unsigned short Ne){ return NULL;};
};
// --------------------------------------------------------------------------

/* -------------------------------------------------------------------------- */
// Картинка TBG
/* -------------------------------------------------------------------------- */
class TPictureTBG{
   TWorkPlace         wp;     // область вывода
   LinkList<TBaseGP*> ListGP; // список ГП

   double         OffsetX,    // Смещение всех ГП картинки
                  OffsetY;    //
   unsigned       SizeX,      // Размеры картинки при рисовании в пикс.
                  SizeY;      //
            long  nCurrentGE; // Номер текущего ГП
   unsigned long  Selected;   // Количество выбранных элементов
   bool           Change;     // Были измемения
   bool           Smooth;     // Сглаживание   
public:
   enum {ToHead, ToCurrent, ToTail};
   enum Bring {ToFront, ToBack, ToStepUp, ToStepDown};

   __fastcall TPictureTBG(char *NameFile);
   __fastcall TPictureTBG(void);

   ~TPictureTBG();

   inline unsigned GetSizeX(void){return SizeX;};
   inline unsigned GetSizeY(void){return SizeY;};

   void __fastcall Load(char *Fn);
   void __fastcall Save(char *Fn);

   void __fastcall DeleteAllGP(void);

   void __fastcall setSizeScreen(int x, int y);

   inline void setMaxX(int x){wp.setMaxX(x);};
   inline void setMaxY(int y){wp.setMaxY(y);};

   inline double getMaxX(void)const{return (double)wp.getMaxX();};
   inline double getMaxY(void)const{return (double)wp.getMaxY();};

   inline TCanvas* getCanvas(void)const{return wp.getCanvasOut();};
   inline void setCanvas(TCanvas *c){wp.setCanvasOut(c);};

   inline TWorkPlace* WorkPlace(void){return &wp;};
   void __fastcall SetWorkPlace(TWorkPlace*);

   void __fastcall setOffset(TPoint po);
   void __fastcall setOffset(TPointf po);

   bool __fastcall IsUpdate(bool SetUpdate = false);

   void __fastcall Show(void);

   void __fastcall InsertGP(TBaseGP *InsGP, int m = ToCurrent);

   TBaseGP* __fastcall FindFirstNo(unsigned short No);
   TBaseGP* __fastcall FindNextNo(unsigned short No);
   TBaseGP* __fastcall FindFirstNe(unsigned short No, unsigned short Ne);
   TBaseGP* __fastcall FindNextNe(unsigned short No, unsigned short Ne);

   void __fastcall Smoothing(bool to = true);
   bool __fastcall GetSmooth(void){return Smooth;};

   bool __fastcall MoveToAllGP(TPoint to, bool mode);

   TBaseGP* __fastcall Hit(TPoint p);

#ifdef _EDITOR_
   TBaseGP* __fastcall Select(POINT p, TShiftState Shift);
   int      __fastcall Select(TRect r, TShiftState Shift);
   void     __fastcall SelectAll(void);

   void __fastcall ShowSelect(SSMode m);

   TBaseGP* __fastcall TPictureTBG::HitToPointGP(TPointf hp);

   long __fastcall GetnCurrentGP(void);

   void __fastcall SmoothAll(bool to = true){};
   void __fastcall SmoothCurrent(bool to = true){};
   void __fastcall SmoothSelect(bool to = true){};

    unsigned long __fastcall GetCount(void){return ListGP.GetCount();};


    int __fastcall IndexCurrentGP(void){
      return ListGP.IndexCurrent();
   }/* IndexCurrentGP */

   inline bool ExistGP(void){return !ListGP.Empty();};
   inline void StoreCGP(void){ListGP.StoreCurrent();};
   inline void RestoreCGP(void){ListGP.ReStoreCurrent();};
   inline bool FirstGP(void){return ListGP.ToHead();};
   inline bool LastGP(void){return ListGP.ToTail();};
   inline bool PrevGP(void){return ListGP.Prev();};
   inline bool NextGP(void){return ListGP.Next();};
   inline TBaseGP* GetGP(void){return ListGP.GetData();};


   inline bool setChangeStatus(bool f){return Change = f;};
   inline bool getChangeStatus(void){return Change;};

   bool __fastcall MoveToSelectedGP(TPoint to, bool mode = true);
   bool __fastcall RotateToSelectedGP(void);
   bool __fastcall CopySelectedGP(CopyGEMode m = cmWithLogic);
   bool __fastcall DeleteSelectedGP(void);
   bool __fastcall DeleteCurrentGP(void);
   bool __fastcall CopySelectedGPIntoClipboard(CopyGEMode m);
   bool __fastcall CopyGPFromClipboard(void);
   bool __fastcall ZoomSelectedGP(double Zk);
   bool __fastcall BringSelectedGP(Bring to = ToFront);

#endif

// --- методы совместимые с TBG ------------------------------------------------
    int __fastcall load_file (char *fn);
    int __fastcall del_pic   (void);
    int __fastcall outpic_all(void);

   bool     __fastcall outpic_obj(unsigned short numobj);
   bool     __fastcall outpic_obj_num(unsigned short numobj, unsigned short num);
   bool     __fastcall outpic_obj_color(unsigned short numobj, TColor color);
   bool     __fastcall outpic_obj_num_color(unsigned short numobj, unsigned short num, TColor color);
   bool     __fastcall setcolor_obj(unsigned short numobj, TColor color, TypeShow show);
   bool     __fastcall setcolor_obj_num(unsigned short numobj, unsigned short num, TColor color, TypeShow show);
   TColor   __fastcall getcolor_obj_num(unsigned short numobj, unsigned short num);
   TColor   __fastcall getcolorset_obj_num(unsigned short numobj, unsigned short num);
   bool     __fastcall getsize_obj_num(unsigned short numobj, unsigned short num, TRect &r);
   bool     __fastcall getsize_obj_num_reset(void);
   TypeGP   __fastcall gettip_obj_num(unsigned short numobj, unsigned short num);
   int      __fastcall cmpcolor_obj_num(unsigned short numobj, unsigned short num, TColor color);
   int      __fastcall cmpcolorset_obj_num(unsigned short numobj, unsigned short num, TColor color);
   char*    __fastcall gettext_obj_num(unsigned short numobj, unsigned short num);
   bool     __fastcall outtext_obj_num(unsigned short numobj, unsigned short num, char *text, TypeShow show);
   int      __fastcall strlen_obj_num(unsigned short numobj, unsigned short num);
   bool     __fastcall getmax_obj(unsigned short numobj, unsigned *num_cou);
   bool     __fastcall getmin_obj(unsigned short numobj, unsigned *num_cou);
   unsigned short __fastcall getpos_mouse_obj(unsigned short numobj, unsigned short StartNum);
   unsigned short __fastcall get_num_obj(unsigned short numobj, unsigned short num, unsigned short numobj2);
   bool     __fastcall pren(unsigned short obj, unsigned short num, TPoint p);
   bool     __fastcall check_obj_num(unsigned short numobj, unsigned short num);
   bool     __fastcall outpic_obj_num_with_rotate(unsigned short numobj, unsigned short num,double a);

   int      __fastcall setstate_obj(unsigned short numobj,unsigned int state, int show);
   int      __fastcall setstate_obj_num(unsigned short numobj,unsigned short num,unsigned int state, int show);
   unsigned int __fastcall getstate_obj_num(unsigned short numobj,unsigned short num);
   int      __fastcall setdata_obj(unsigned short numobj, void *ud);
   int      __fastcall setdata_obj_num(unsigned short numobj,unsigned short num, void *ud);
   void*    __fastcall getdata_obj_num(unsigned short numobj,unsigned char num);
   void     __fastcall savestate(void);
};

//
// Дерево описания маропримитивов
//
//
class TMacroTreeView {
   TTreeView *tv;
   bool TVExtern;
   AnsiString PathToMacro;
   THashedStringList *Hash;

public:
   __fastcall TMacroTreeView(TComponent *);
   ~TMacroTreeView(void);
   const TMacroTreeView& __fastcall operator=(const TMacroTreeView& r);
   void __fastcall SetTreeView(TTreeView*);
   void __fastcall SetPathToMacro(AnsiString);
   bool __fastcall LoadMacroTree(AnsiString);
   TTreeNode* __fastcall SearchOnIndex(unsigned);
   TTreeNode* __fastcall SearchMacroAsParent(TTreeNode*);
   unsigned   __fastcall SearchMacroAsIndex(TTreeNode*);
   TTreeNode* __fastcall SearchStateAsParent(TTreeNode*);
   unsigned   __fastcall SearchStateAsIndex(TTreeNode*);
   TTreeNode* __fastcall SearchState(TTreeNode*, unsigned);
   void __fastcall ExecuteState(TTreeNode*, TPictureTBG*);
   TPictureTBG* __fastcall LoadPic(TTreeNode*);
   void __fastcall DeleteAllPic(void);

};

// Фактически Графические примитивы
// --------------------------------------------------------------------------
// Ломаная
// TBG_004.CPP
class TPolyLine : public TBaseGP {
   TColor  Color, OutColor;        // цвет RGB
   unsigned Width; // толщина в пикселах

#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TPolyLine();
   __fastcall TPolyLine(TPolyLine &s);
   __fastcall TPolyLine(double ssX, double ssY,
         double seX, double seY,
         TColor sColor, unsigned sWidth);
   ~TPolyLine();
   AnsiString __fastcall GetName(void);
   bool __fastcall Hit(TWorkPlace *wp, TPoint p);
#ifdef _EDITOR_
   TPolyLine& __fastcall operator=(TPolyLine& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
   int __fastcall MouseDown(TWorkPlace *wp, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   int __fastcall MouseMove(TWorkPlace*, TObject *Sender, TShiftState Shift, int X, int Y);
   void __fastcall SaveAsBase(FILE *f);
   void __fastcall Save(FILE *f);
   int __fastcall GetCoordCnt(void);
#endif
   void __fastcall SetColor(TColor c){ Color = c; };
   TColor __fastcall GetColor(void){ return Color; };

   void __fastcall SetWidthLine(unsigned w){ Width = w ; };
   unsigned __fastcall GetWidthLine(void){ return Width; };

   bool __fastcall Load(char *buf);

   void __fastcall Show(TWorkPlace*);

   void __fastcall Smoothing(bool to = true);
};
// --------------------------------------------------------------------------
// Линия простая
// TBG_001.CPP
class TLine : public TBaseGP {
   TColor  Color, OutColor;        // цвет RGB
   unsigned Width; // толщина в пикселах

#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TLine();
   __fastcall TLine(double ssX, double ssY,
         double seX, double seY,
         TColor sColor, unsigned sWidth);
   AnsiString __fastcall GetName(void);
   bool __fastcall Hit(TWorkPlace*, TPoint);
#ifdef _EDITOR_
   TLine& __fastcall operator=(TLine& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
   int __fastcall MouseDown(TWorkPlace*, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   int __fastcall MouseMove(TWorkPlace*, TObject *Sender, TShiftState Shift, int X, int Y);

   void __fastcall SaveAsBase(FILE *f);
   void __fastcall Save(FILE *f);
#endif
   bool __fastcall Load(char *buf);

   void __fastcall Show(TWorkPlace *);
   void __fastcall ShowWithColor(TWorkPlace* c, TColor cl);
   void __fastcall ShowWithRotate(TWorkPlace* c, TPointf p, double a);

   void __fastcall SetColor(TColor c){ OutColor = Color = c; };
   TColor __fastcall GetColor(void){ return Color; };

   void __fastcall SetWidthLine(unsigned w){ Width = w ; };
   unsigned __fastcall GetWidthLine(void){ return Width; };


   void __fastcall Smoothing(bool to = true);
};
// --------------------------------------------------------------------------
// Труба
// TBG_008.CPP
class TPipeLine : public TPolyLine {
   short int DO, // Delta Offset
              O, // Current Offset
              S; // Delta Step
   TColor  Color1;        // цвет RGB
   TColor  Color2;        // цвет RGB
   bool Mode;
#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TPipeLine();
   __fastcall TPipeLine(TPipeLine &s);
   __fastcall TPipeLine(double ssX, double ssY,
         double seX, double seY,
         TColor sColor1, TColor sColor2,
         short int sDO,
         short int sO,
         short int sS,
         unsigned sWidth);
   ~TPipeLine();
   void __fastcall PipePaint(TCanvas *cvs, TPoint sp, TPoint ep, bool Mode, TColor color);
   AnsiString __fastcall GetName(void);
#ifdef _EDITOR_
   inline void IncDO(void){DO++;};
   inline void DecDO(void){DO--;};
   inline void IncS(void){S++;};
   inline void DecS(void){S--;};
   TPipeLine& __fastcall operator=(TPipeLine& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);

   void __fastcall Save(FILE *f);
#endif
   void __fastcall SetColor(TColor c){ Color1 = c; };
   TColor __fastcall GetColor(void){ return Color1; };

   void __fastcall SetFillColor(TColor c){ Color2 = c; };
   TColor __fastcall GetFillColor(void){ return Color2; };


   bool __fastcall Load(char *buf);
   void __fastcall Show(TWorkPlace *);
};
// --------------------------------------------------------------------------
// Полигон
// TBG_007.CPP
class TPolygon : public TPolyLine {
   TColor  ColorFill;    // цвет RGB
   TBrushStyle StyleFill; // стиль заливки
#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TPolygon();
   __fastcall TPolygon(TPolygon &s);
   __fastcall TPolygon(double ssX, double ssY, double seX, double seY,
         TColor sColor, TColor sColorFill, TBrushStyle sStyleFill, unsigned sWidth);
   ~TPolygon();
   AnsiString __fastcall GetName(void);
   bool __fastcall Hit(TWorkPlace*, TPoint);
#ifdef _EDITOR_
   TPolygon& __fastcall operator=(TPolygon& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
   void __fastcall SaveAsBase(FILE *f);
   void __fastcall Save(FILE *f);
#endif
   void __fastcall SetFillColor(TColor c){ ColorFill = c; };
   TColor __fastcall GetFillColor(void){ return ColorFill; };

   bool __fastcall Load(char *buf);
   void __fastcall Show(TWorkPlace *);
};
// --------------------------------------------------------------------------
// Прямоугольник по 4 точкам
// TBG_002.CPP
class TPolyRect : public TBaseGP {
   TColor  Color;        // цвет RGB
   unsigned Width; // толщина в пикселах

#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TPolyRect();
   __fastcall TPolyRect(double ssX, double ssY,
         double seX, double seY,
         TColor sColor, unsigned sWidth);
   ~TPolyRect();
   AnsiString __fastcall GetName(void);
   bool __fastcall Hit(TWorkPlace *, TPoint);
#ifdef _EDITOR_
   TPolyRect& __fastcall operator=(TPolyRect& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
   int __fastcall MouseDown(TWorkPlace *, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   int __fastcall MouseMove(TWorkPlace *, TObject *Sender, TShiftState Shift, int X, int Y);

   int __fastcall GetCoordCnt(void){return getSize() - 1;};
   void __fastcall SaveAsBase(FILE *f);
   void __fastcall Save(FILE *f);
#endif
   void __fastcall SetStartXY(double ssX, double ssY);
   void __fastcall SetEndXY(double seX, double seY);
   double __fastcall GetStartX(void);
   double __fastcall GetStartY(void);
   double __fastcall GetEndX(void);
   double __fastcall GetEndY(void);

   void __fastcall SetColor(TColor c){ Color = c; };
   TColor __fastcall GetColor(void){ return Color; };

   void __fastcall SetWidthLine(unsigned w){ Width = w ; };
   unsigned __fastcall GetWidthLine(void){ return Width; };

   bool __fastcall Load(char *buf);

   void __fastcall Show(TWorkPlace *);

   void __fastcall Smoothing(bool to = true);
};
// --------------------------------------------------------------------------
// Прямоугольник по 2 точкам
// TBG_005.CPP
class TRectangle : public TLine {
#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TRectangle();
   __fastcall TRectangle(double ssX, double ssY,
         double seX, double seY,
         TColor sColor, unsigned sWidth);
   ~TRectangle();
   AnsiString __fastcall GetName(void);
   bool __fastcall Hit(TWorkPlace *wp, TPoint p);
#ifdef _EDITOR_
   TRectangle& __fastcall operator=(TRectangle& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
   void __fastcall Save(FILE *f);
#endif
   bool __fastcall Load(char *buf);
   void __fastcall Show(TWorkPlace *);
};
// --------------------------------------------------------------------------
// Прямоугольник заполненный по 4 точкам
// TBG_003.CPP
class TFillPolyRect : public TPolyRect {
   TColor  ColorFill;    // цвет RGB
   TBrushStyle StyleFill; // стиль заливки
#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TFillPolyRect();
   __fastcall TFillPolyRect(double ssX, double ssY,
         double seX, double seY,
         TColor sColor, TColor sColorFill, TBrushStyle sStyleFill, unsigned sWidth);
   ~TFillPolyRect();
   AnsiString __fastcall GetName(void);
   bool __fastcall Hit(TWorkPlace *wp, TPoint p);
#ifdef _EDITOR_
   TFillPolyRect& __fastcall operator=(TFillPolyRect& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);

   void __fastcall Save(FILE *f);
#endif
   void __fastcall SetFillColor(TColor c){ ColorFill = c; };
   TColor __fastcall GetFillColor(void){ return ColorFill; };

   bool __fastcall Load(char *buf);

   void __fastcall Show(TWorkPlace *);
};
// --------------------------------------------------------------------------
// Прямоугольник заполненный по 2 точкам
// TBG_006.CPP
class TFillRectangle : public TLine {
   TColor  ColorFill;    // цвет RGB
   TBrushStyle StyleFill; // стиль заливки
   int Blur;
   bool BlurToGrayscale;

   bool Filter;
   int  m[3][3], w, a;
#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TFillRectangle();
   __fastcall TFillRectangle(double ssX, double ssY,
         double seX, double seY,
         TColor sColor, TColor sColorFill, TBrushStyle sStyleFill, unsigned sWidth);
   ~TFillRectangle();
   AnsiString __fastcall GetName(void);
   bool __fastcall Hit(TWorkPlace *wp, TPoint p);
#ifdef _EDITOR_
   TFillRectangle& __fastcall operator=(TFillRectangle& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
   void __fastcall Save(FILE *f);
#endif
   void __fastcall SetFillColor(TColor c){ ColorFill = c; };
   TColor __fastcall GetFillColor(void){ return ColorFill; };

   void __fastcall SetBlur(int);
   int __fastcall GetBlur(void);

   void __fastcall SetBlurToGrayscale(bool);
   bool __fastcall GetBlurToGrayscale(void);

   bool __fastcall Load(char *buf);
   void __fastcall Show(TWorkPlace *);
};
// --------------------------------------------------------------------------
// Прямоугольник заполненный по 2 точкам
// TBG_015.CPP
class TGradientFillRectangle : public TLine {
   TColor Color2;    
   GradientFillStyle FillStyle;
#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TGradientFillRectangle();
   __fastcall TGradientFillRectangle(double ssX, double ssY,
         double seX, double seY,
         TColor sColor1, TColor sColor2, GradientFillStyle FillStyle);
   ~TGradientFillRectangle();
   AnsiString __fastcall GetName(void);
   bool __fastcall Hit(TWorkPlace *wp, TPoint p);
#ifdef _EDITOR_
   TGradientFillRectangle& __fastcall operator=(TGradientFillRectangle& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
   void __fastcall Save(FILE *f);
#endif
   void __fastcall SetColor2(TColor c){ Color2 = c; };
   TColor __fastcall GetColor2(void){ return Color2; };
   void __fastcall SetGradientFillStyle(GradientFillStyle c){ FillStyle = c; };
   GradientFillStyle __fastcall GetGradientFillStyle(void){ return FillStyle; };

   bool __fastcall Load(char *buf);
   void __fastcall Show(TWorkPlace *);
};
// --------------------------------------------------------------------------
// Дуга
// TBG_011.CPP
class TArc : public TBaseGP {
   double PhiStart,     // угол начала
          PhiEnd;       // угол конца
   TColor  Color;       // цвет RGB
   unsigned Width; // толщина в пикселах

   double Angle;
   TPointf CenterRotatePoint;

   TCoordSpace *cs;

#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TArc();
   __fastcall TArc(TArc &s);
   __fastcall TArc(TPointf *p, TColor sColor, unsigned sWidth);
   ~TArc();
   AnsiString __fastcall GetName(void);
   void __fastcall SetEndXY(double seX, double seY);
   void __fastcall SetPhiStart(double PhiSx, double PhiSy);
   void __fastcall SetPhiEnd(double PhiEx, double PhiEy);
   void __fastcall SetPhiStart(TPointf &p);
   void __fastcall SetPhiEnd(TPointf &p);
   void __fastcall toCircle(void);
   TPointf& __fastcall GetPhiStart(void);
   TPointf& __fastcall GetPhiEnd(void);
//   int __fastcall RotateTo(TPointf cp, double a);
   bool __fastcall Hit(TWorkPlace*, TPoint p);
#ifdef _EDITOR_
   TArc& __fastcall operator=(TArc& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
   int __fastcall MouseDown(TWorkPlace*, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   int __fastcall MouseMove(TWorkPlace*, TObject *Sender, TShiftState Shift, int X, int Y);
//   bool __fastcall Select(TWorkPlace*, TRect r, TShiftState Shift);
   void __fastcall ShowSelect(TWorkPlace*, SSMode);
   void __fastcall SaveAsBase(FILE *f);
   void __fastcall Save(FILE *f);
#endif
   void __fastcall SetColor(TColor c){ Color = c; };
   TColor __fastcall GetColor(void){ return Color; };
   void __fastcall SetWidth(unsigned w){ Width = w; };
   unsigned __fastcall GetWidth(void){ return Width; };
   void __fastcall SetAngle(double a){ Angle = a; };
   double __fastcall GetAngle(void){ return Angle; };

   bool __fastcall Load(char *buf);
   void __fastcall Show(TWorkPlace *);
};
// --------------------------------------------------------------------------
//
// tbg_014.cpp
//
class TFillArc : public TArc {
   TColor  ColorFill;    // цвет RGB
   TBrushStyle StyleFill; // стиль заливки

#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TFillArc();
   __fastcall TFillArc(TFillArc &s);
   __fastcall TFillArc(TPointf *p, TColor sColor, TColor sColorFill, TBrushStyle sStyleFill,unsigned sWidth);
   ~TFillArc();
   AnsiString __fastcall GetName(void);
//   int __fastcall RotateTo(TPointf cp, double a);
//   bool __fastcall Hit(TWorkPlace*, TPoint p);
#ifdef _EDITOR_
   TFillArc& __fastcall operator=(TFillArc& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
//   bool __fastcall Select(TWorkPlace*, TRect r, TShiftState Shift);
   void __fastcall Save(FILE *f);
#endif
   void __fastcall SetFillColor(TColor c){ ColorFill = c; };
   void __fastcall SetFillStyle(TBrushStyle s){ StyleFill = s; };
   TColor __fastcall GetFillColor(void){ return ColorFill; };
   TBrushStyle __fastcall GetFillStyle(void){ return StyleFill; };

   bool __fastcall Load(char *buf);
   void __fastcall Show(TWorkPlace *);
};
// --------------------------------------------------------------------------
// Text BGI
// TBG_009.CPP
class TBGIText : public TBaseGP {
   TColor  Color;        // цвет RGB
   short int BGIFont,
             Direction,
             Size,
             HorJustify,
             VerJustify;
   short int     Sda,   // размер окна при создании текста
                 SSX,
                 SSY;
   unsigned char Mltx;  // увеличение при плавном масштабе
   unsigned char Mlty;  // увеличение при плавном масштабе
   double Angle;
   char *pStr;
   int  lenStr;

#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TBGIText();
   ~TBGIText();
   __fastcall TBGIText(double ssX, double ssY,
         TColor sColor);
   AnsiString __fastcall GetName(void);
   void __fastcall SetFont(int ifont);
   void __fastcall SetDirection(int dir);
   void __fastcall SetSize(short int size);
   void __fastcall SetAngle(double ang);
   void __fastcall SetJustify(short int hj, short int vj);
   void __fastcall SetExt(short int sda, unsigned char mltx, unsigned char mlty);
   void __fastcall SetSS(short int sx, short int sy);
   void __fastcall SetText(char *t);
   char* __fastcall GetText(void);
   __property char* Text = {read=GetText, write=SetText};
   void __fastcall SetColor(TColor c){ Color = c; };
   TColor __fastcall GetColor(void){ return Color; };
   bool __fastcall Hit(TWorkPlace *wp, TPoint p);

#ifdef _EDITOR_
   TBGIText& __fastcall operator=(TBGIText& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);

   int __fastcall MouseDown(TWorkPlace *wp, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   int __fastcall MouseMove(TWorkPlace *wp, TObject *Sender, TShiftState Shift, int X, int Y);

   bool __fastcall Select(TWorkPlace *wp, TRect r, TShiftState Shift);
   int __fastcall RotateTo(TPointf cp, double ang);
   int __fastcall Zoom(double Zk);

   void __fastcall Save(FILE *f);
#endif
   bool __fastcall Load(char *buf);

   void __fastcall Show(TWorkPlace *);
};
/*
// --------------------------------------------------------------------------
// Text TrueType
// TBG_009.CPP
class TTrueTypeText : public TBaseGP {
   TColor  Color;        // цвет RGB
   short int TrueTypeFont,
             Direction,
             Size,
             HorJustify,
             VerJustify;
   double Angle;
   char *pStr;
   int  lenStr;

#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TTrueTypeText();
   ~TTrueTypeText();
   __fastcall TTrueTypeText(double ssX, double ssY,
         TColor sColor);
   AnsiString __fastcall GetName(void);
   void __fastcall SetColor(TColor color);
   void __fastcall SetFont(int ifont);
   void __fastcall SetDirection(int dir);
   void __fastcall SetSize(short int size);
   void __fastcall SetAngle(double ang);
   void __fastcall SetJustify(short int hj, short int vj);
   void __fastcall SetExt(short int sda, unsigned char mltx, unsigned char mlty);
   void __fastcall SetSS(short int sx, short int sy);
   void __fastcall SetText(char *t);
   char* __fastcall GetText(void);
   bool __fastcall Hit(TPointf);
#ifdef _EDITOR_
   TTrueTypeText& __fastcall operator=(TTrueTypeText& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
   int __fastcall MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   int __fastcall MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
   bool __fastcall Select(POINT p, TShiftState Shift);
   bool __fastcall Select(TRect r, TShiftState Shift);

   void __fastcall ShowSelect(TWorkPlace*, SSMode);

   void __fastcall Save(FILE *f);
#endif
   bool __fastcall Load(char *buf);

   int __fastcall MoveTo(double toX, double toY, bool m = true);
   void __fastcall Show(TWorkPlace *);

   void __fastcall Smoothing(bool to = true);
};
*/

//
// TBG_012.CPP
//
class TReperPoint : public TBaseGP {
#ifdef _EDITOR_
   Operations Op;
#endif
public:
   //
   __fastcall TReperPoint();
   __fastcall TReperPoint(TReperPoint&);
   __fastcall TReperPoint(TPointf*);
   AnsiString __fastcall GetName(void);
#ifdef _EDITOR_
   TReperPoint& __fastcall operator=(TReperPoint& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
   int __fastcall MouseDown(TWorkPlace*, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   int __fastcall MouseMove(TWorkPlace*, TObject *Sender, TShiftState Shift, int X, int Y);

   void __fastcall Save(FILE *f);
#endif
   bool __fastcall Load(char *buf);

   void __fastcall Show(TWorkPlace *);

};
// --------------------------------------------------------------------------
//
// Тип Макро
// TBG_013.CPP
//
class TMacro : public TBaseGP {
   unsigned  Index;
   unsigned  State;

   void     *pData;
   unsigned  StateSave;
   unsigned   Indicate;
   TTreeNode *IndicateOnNode, *IndicateOffNode;
   TTreeNode *Node, *NodeState;
   TPictureTBG *Pic;
#ifdef _EDITOR_
   Operations Op;
#endif
public:
   __fastcall TMacro();
   __fastcall TMacro(unsigned i, unsigned s, TPointf p);
   __fastcall TMacro(unsigned Index);
   __fastcall TMacro(TMacro &s);
   AnsiString __fastcall GetName(void);
   bool __fastcall Hit(TWorkPlace*, TPoint);
#ifdef _EDITOR_
   TMacro& __fastcall operator=(TMacro& R);
   TBaseGP* __fastcall Copy(CopyGEMode m);
   int __fastcall MouseDown(TWorkPlace*, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
   int __fastcall MouseMove(TWorkPlace*, TObject *Sender, TShiftState Shift, int X, int Y);

   void __fastcall Save(FILE *f);
#endif
   bool __fastcall Load(char *buf);
   void __fastcall SetState(unsigned state);
   unsigned __fastcall GetState(void);

   void __fastcall SaveState(void);
   unsigned __fastcall GetSaveState(void);

   void __fastcall SetUserData(void *pDataInt);
   void* __fastcall GetUserData(void);

   void __fastcall Show(TWorkPlace *);
//   void __fastcall ShowWithRotate(TWorkPlace* c, TPointf p, double a);
};


/* -------------------------------------------------------------------------- */
// Описание глобальных переменных
// Globals
/* -------------------------------------------------------------------------- */
// tbg_000.cpp
extern char *whitesp;
extern char *breakch;
extern char *quotech;
extern char escape;

// tbg_013.cpp
extern TMacroTreeView* mtv;

// tbg_converts.cpp
extern TColor EGAColors[16];

/* -------------------------------------------------------------------------- */
// Описание прототипов функций и размещение по файлам
// Prototypes global
/* -------------------------------------------------------------------------- */

// tbg_015.cpp
BOOL GradientRectangle(HDC hDC, int x0, int y0, int x1, int y1, COLORREF c0, COLORREF c1, int type);
BOOL SymGradientRectangle(HDC hDC, int x0, int y0, int x1, int y1, COLORREF c0, COLORREF c1, int type);
BOOL CornerGradientRectangle(HDC hDC, int x0, int y0, int x1, int y1, COLORREF c0, COLORREF c1, int corner);
BOOL CenterGradientRectangle(HDC hDC, int x0, int y0, int x1, int y1, COLORREF c0, COLORREF c1);
BOOL RadialGradientFill(HDC hDC, int x0, int y0, int x1, int y1, int r, COLORREF c0, COLORREF c1, int nPart);

// Parser.c by Lloyd Zusman(Master Byte Software 1986)
// Parser.cpp
int __fastcall parser(unsigned inflag, char* token, unsigned tokmax, char* line, char* white, char* brkchar, char* quote, char eschar, char* brkused, int* next, char* quoted);

// tbg_000.cpp
TypeKW __fastcall SearchKeyword(char *buf);
bool __fastcall ReadValue(char *s, int type, void *pv);

// tbg_converts.cpp
double __fastcall XCompanenta(double r, double pfi);
double __fastcall YCompanenta(double r, double pfi);
double __fastcall Angl(TPointf ps, TPointf pe);
void __fastcall Convert(TPictureTBG *NewPic, char *OldFileName);

// WuAALine.cpp
// Xaolin Wu 
void __fastcall WuLine(TCanvas *Canvas, int X0, int Y0, int X1, int Y1, int BaseColor, int NumLevels, unsigned int IntensityBits, unsigned long rgb);
void __fastcall WuPolyline(TCanvas *cvs, TPoint *pts, int maxind);
void __fastcall WuPolyDraw(TCanvas *cvs, POINT *pp, BYTE *pb, int cnt);

// tbg_001.cpp
int __fastcall Cross(int _x0, int _y0,
          int _x1, int _y1,
          int _x2, int _y2, int _z);

// - end ----------------------------------------------------------------------

#endif /* __TBG_HPP__ */
