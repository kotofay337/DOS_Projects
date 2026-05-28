//---------------------------------------------------------------------------

#ifndef ToolBarH
#define ToolBarH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Registry.hpp>
//---------------------------------------------------------------------------
class TSpeedBar : public TForm
{
__published:	// IDE-managed Components
        TSpeedButton *sbHelp;
        TSpeedButton *sbAbout;
        TSpeedButton *sbQuit;
        TSpeedButton *sbTextJustify;
        TSpeedButton *sbFontSelection;
        TSpeedButton *sbLine;
        TSpeedButton *sbRect;
        TSpeedButton *sbBar;
        TSpeedButton *sbEllipse;
        TSpeedButton *sbText;
        TSpeedButton *sbMacro;
        TSpeedButton *sbSave;
   TEdit *eFileName;
   TEdit *eGEName;
        TSpeedButton *sbLoad;
        TSpeedButton *sbCorrect;
        TSpeedButton *sbDel;
        TSpeedButton *sbCopy;
        TSpeedButton *sbMove;
        TSpeedButton *sbZoom;
        TSpeedButton *sbOut;
        TSpeedButton *sbIdication;
        TSpeedButton *sbSearchGE;
        TSpeedButton *SpeedButton21;
        TSpeedButton *SpeedButton22;
        TSpeedButton *SpeedButton23;
        TSpeedButton *SpeedButton24;
        TSpeedButton *SpeedButton25;
   TSpeedButton *sbDelLE;
   TSpeedButton *sbAddLE;
        TSpeedButton *sbCurrentLE;
        TSpeedButton *sbAllLE;
        TSpeedButton *sbCurrentGE;
        TSpeedButton *sbAllGE;
        TSpeedButton *sbEllipsePlus;
        TSpeedButton *sbEllipseMinus;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TOpenPictureDialog *OpenPictureDialog1;
        TFontDialog *FontDialog1;
        TColorDialog *ColorDialog1;
        TPrintDialog *PrintDialog1;
        TPrinterSetupDialog *PrinterSetupDialog1;
        TPopupMenu *ppmLine;
   TMenuItem *miLineSimple;
   TMenuItem *miLinePoly;
   TMenuItem *miLineBezier;
   TMenuItem *miLinePolygon;
        TPopupMenu *ppmRect;
   TMenuItem *miRect2p;
   TMenuItem *miRect4p;
   TMenuItem *miLinePipe;
        TPopupMenu *ppmExtent;
        TMenuItem *N7;
        TMenuItem *OLE1;
        TMenuItem *N8;
        TMenuItem *OLE2;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TStatusBar *StatusBar1;
        TTimer *TimeDate;
        TPopupMenu *ppmOutMode;
        TMenuItem *WuSmoothLine;
        TMenuItem *N9;
        TMenuItem *N10;
        TMenuItem *N10x101;
        TSaveDialog *SavePictureDialog1;
   TEdit *EditNUM;
   TPopupMenu *ppmClear;
   TEdit *EditOBJ;
   TPopupMenu *ppmDeleteMode;
   TMenuItem *miDelCurrent;
   TMenuItem *miDelSelected;
   TMenuItem *miDelAll;
   TPanel *LineStl;
   TPanel *FillStl;
   TPanel *TextStl;
   TImage *shpLine;
   TImage *shpRect;
   TImage *shpText;
   TPopupMenu *ppmCopyMode;
   TMenuItem *miCopyWLogic;
   TMenuItem *miCopyWOLogic;
   TPopupMenu *ppmMoved;
   TMenuItem *miMoved;
   TMenuItem *miRotated;
   TMenuItem *miMirrorX;
   TMenuItem *miMirrorY;
   TMenuItem *miMirrorXY;
   TMenuItem *N11;
   TMenuItem *miBringToFront;
   TMenuItem *miBringToBack;
   TMenuItem *miBringToUp;
   TMenuItem *miBringToDown;
   TMenuItem *miLineStyled;
   TPopupMenu *ppmBar;
   TMenuItem *miBar;
   TMenuItem *miBarPoly;
   TPopupMenu *ppmFonts;
   TMenuItem *miBGI;
   TMenuItem *miTrueType;
   TPopupMenu *ppmEllipse;
   TMenuItem *miEllipse;
   TMenuItem *miArc;
   TTimer *Timer1;
	TMenuItem *N1;
	TMenuItem *miFillArc;
	TPopupMenu *ppmLoad;
	TMenuItem *miImport;
	TPopupMenu *ppmSave;
	TMenuItem *miSaveAs;
	TMenuItem *miBarGradient;
	TMenuItem *miBlur;
	TMenuItem *miBlurToGrayscale;
	TMenuItem *miBlurToColor;
	TMenuItem *N2;
   TMenuItem *miGradientFillEllipse;
        void __fastcall sbQuitClick(TObject *Sender);
        void __fastcall sbLoadClick(TObject *Sender);
        void __fastcall sbSaveClick(TObject *Sender);
        void __fastcall sbEllipseClick(TObject *Sender);
        void __fastcall sbLineClick(TObject *Sender);
        void __fastcall sbRectClick(TObject *Sender);
        void __fastcall sbBarClick(TObject *Sender);
        void __fastcall sbTextClick(TObject *Sender);
        void __fastcall sbMacroClick(TObject *Sender);
        void __fastcall sbCurrentGEMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall TimeDateTimer(TObject *Sender);
        void __fastcall sbDelClick(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall WuSmoothLineClick(TObject *Sender);
   void __fastcall sbMoveClick(TObject *Sender);
   void __fastcall sbNUMMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall sbCurrentLEMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall sbAllLEMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall sbAllGEMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall EditNUMMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall EditOBJMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall EditOBJChange(TObject *Sender);
   void __fastcall EditNUMChange(TObject *Sender);
   void __fastcall sbAddLEClick(TObject *Sender);
   void __fastcall sbDelLEMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall miDelAllClick(TObject *Sender);
   void __fastcall miDelCurrentClick(TObject *Sender);
   void __fastcall miDelSelectedClick(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall shpLineMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall shpRectMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall shpTextMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall sbCopyClick(TObject *Sender);
   void __fastcall sbZoomClick(TObject *Sender);
   void __fastcall miCopyWLogicClick(TObject *Sender);
   void __fastcall miCopyWOLogicClick(TObject *Sender);
   void __fastcall sbCorrectMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall sbFontSelectionMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall miMovedClick(TObject *Sender);
   void __fastcall miRotatedClick(TObject *Sender);
   void __fastcall sbCorrectClick(TObject *Sender);
   void __fastcall sbAboutClick(TObject *Sender);
   void __fastcall miMirrorXClick(TObject *Sender);
   void __fastcall miMirrorYClick(TObject *Sender);
   void __fastcall miMirrorXYClick(TObject *Sender);
   void __fastcall miBringTo(TObject *Sender);
   void __fastcall sbAddLEMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall OpenDialog1Show(TObject *Sender);
   void __fastcall OpenDialog1Close(TObject *Sender);
   void __fastcall OpenDialog1SelectionChange(TObject *Sender);
   void __fastcall OpenDialog1CanClose(TObject *Sender, bool &CanClose);
   void __fastcall miLine(TObject *Sender);
   void __fastcall miRectClick(TObject *Sender);
   void __fastcall miBarClick(TObject *Sender);
   void __fastcall miSelectTypeFontsClick(TObject *Sender);
   void __fastcall miEllipseClick(TObject *Sender);
   void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall miBarGradientClick(TObject *Sender);
	void __fastcall miBlurToClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSpeedBar(TComponent* Owner);

        TRegistry *TBGReg;
        AnsiString s, key, val;

        ULONG_PTR token;
};
//---------------------------------------------------------------------------
extern PACKAGE TSpeedBar *SpeedBar;
void shpLineInvalidate(void);
//---------------------------------------------------------------------------
#endif
