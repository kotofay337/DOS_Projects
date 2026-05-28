//----------------------------------------------------------------------------
#ifndef PropDlg1H
#define PropDlg1H
//----------------------------------------------------------------------------
#include <vcl\ExtCtrls.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include "CGRID.h"
//----------------------------------------------------------------------------
class TPropDlg : public TForm
{
__published:
	TPanel *Panel1;
	TPanel *Panel2;
	TPageControl *PageControl1;
   TTabSheet *TabLineProp;
   TTabSheet *TabFillProp;
   TTabSheet *TabTextProp;
	TButton *OKBtn;
	TButton *HelpBtn;
   TEdit *ThickLineText;
   TUpDown *UpDown1;
   TLabel *Label1;
   TLabel *Label2;
   TComboBox *ComboBox1;
   TTabSheet *TabLogicProp;
   TPanel *Panel3;
   TCheckBox *cbSelLE;
   TEdit *EditNo;
   TEdit *EditNe;
   TShape *Shape1;
   TShape *Shape2;
   TShape *Shape3;
   TShape *Shape4;
   TShape *Shape5;
   TShape *Shape6;
   TShape *Shape7;
   TShape *Shape8;
	TImage *GradientSelectPanel;
   void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
   void __fastcall ThickLineTextChange(TObject *Sender);
   void __fastcall OKBtnClick(TObject *Sender);
   void __fastcall cbSelLEClick(TObject *Sender);
   void __fastcall ShapesMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall GradientSelectPanelMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:
public:
	virtual __fastcall TPropDlg(TComponent* AOwner);
   TPictureTBG *pic;
};
//----------------------------------------------------------------------------
extern PACKAGE TPropDlg *PropDlg;
//----------------------------------------------------------------------------
void ShapesInvalidate(TColor cp, TColor cf);
#endif    
