/* TBG_Edit_001.CPP
*
*   TBG v7.0
*   20-09-2001 8:33 am msk
*
*/

#include <math.h>
#pragma hdrstop
#include "tbg.hpp"
#include "TBG_Editor.hpp"
#include "tbg_tc.h"
/* -------------------------------------------------------------------------- */
void ClearClipboard(void){
   if(!cb.Empty()){
      do{
         delete cb.DeleteFromHead();
      }while(!cb.Empty());
   }/* if */
};
/*--------------------------------------------------------------------------*/
void ShowSelector(TCanvas *cvs,int x, int y, bool select){
  cvs->Pen->Color = clTeal;
  cvs->Pen->Width = 1;
  cvs->Pen->Style = psSolid;
  cvs->Brush->Color = select ? (TColor)0x008270D6 : (TColor)0x0060D6FC;
  cvs->Brush->Style = bsSolid;
      
  TPoint points[4];
  points[0] = Classes::Point(x, y);
  points[1] = Classes::Point(x - 6, y - 6);
  points[2] = Classes::Point(x + 6, y - 6);
  points[3] = Classes::Point(x, y);
  cvs->Polygon(points, 3);
}//
/*--------------------------------------------------------------------------*/
void ShowCoordSelector(TWorkPlace *wp, int x, int y){
  TCanvas *cvs = wp->getCanvas();
  cvs->Pen->Color = clBlack;
  cvs->Pen->Width = 1;
  cvs->Pen->Style = psSolid;
  cvs->Brush->Color = clLime;
  cvs->Brush->Style = bsSolid;
  int es = (wp->getMaxX() + wp->getMaxY()) / 2. / 300. + 1;
  cvs->Ellipse(x - es, y - es, x + es, y + es);
}//
/*--------------------------------------------------------------------------*/
void myellipse(TCanvas *cvs, int xc,int yc,int rx,int ry){

	double i,b = rx,a = ry;
	double x1,y1,x2,y2;

	if((a <= 0)||(b <= 0))
		return;

	y1 = b;
	x1 = 0;
	for(i = 0;i <= a;i++){
		y2 = b/a * sqrt(a*a - i*i);
		x2 = i;
		Line(cvs->Handle, xc+x1,yc+y1,xc+x2,yc+y2);
		Line(cvs->Handle, xc+x1,yc+(-y1),xc+x2,yc+(-y2));
		x1 = x2;
		y1 = y2;
	}// for

	y1 = -b;
	x1 = 0;
	for(i = 0;i >= -a;i--){
		y2 = -(b/a * sqrt(a*a - i*i));
		x2 = i;
		Line(cvs->Handle, xc+x1,yc+y1,xc+x2,yc+y2);
		Line(cvs->Handle, xc+x1,yc+(-y1),xc+x2,yc+(-y2));
		x1 = x2;
		y1 = y2;
	}// for

}// ellipse
//---------------------------------------------------------------------------


