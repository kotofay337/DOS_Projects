//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>

#include "LoadPreview.h"
#include "Affine.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLoadPreview_frm *LoadPreview_frm;
//---------------------------------------------------------------------------
__fastcall TLoadPreview_frm::TLoadPreview_frm(TComponent* Owner)
   : TForm(Owner)
{
   ClientWidth = Screen->Width / 3;
   ClientHeight = Screen->Height / 3;
}
//---------------------------------------------------------------------------
// size x size square in logical coordinate space
void Face(HDC hDC, COLORREF color, int size)
{
	for (int x=0; x<size; x++)
	for (int y=0; y<size; y++)
		SetPixel(hDC, x, y, color);
}


// use three transformations to map square in logical coordinate space to faces of a 3D cube
void Draw_Cube(HDC hDC, int width, int height, int degree)
{
	KAffine af;

	//SetMapMode(hDC, MM_ANISOTROPIC);
	//SetWindowExtEx  (hDC, 1, 1,				   NULL); // default
	//SetWindowOrgEx  (hDC, 0, 0,				   NULL);
	
	//SetViewportExtEx(hDC, 1, -1,			   NULL); // y axis goes up
	//SetViewportOrgEx(hDC, width/2, height*3/4, NULL); // 1/2, 3/4

	int ret = SetGraphicsMode(hDC, GM_ADVANCED);

	const float pi   = (float) 3.1415926;
	const int   size = 150;
	const float dx   = (float) (size * cos(pi*degree/180));
	const float dy   = (float) (size * sin(pi*degree/180));
   af.MapTri(0, 0, (float)size, 0, 0, (float)size,   0, 0, dx, dy, 0, (float)size);
   af.Translate(50., 0.);
   af.Scale((FLOAT)0.7, (FLOAT)0.7);
   //af.Rotate(45.);
	SetWorldTransform(hDC, & af.m_xm);
/*
	Face(hDC, RGB(0xFF, 0, 0), size);

	af.MapTri(0, 0, (float)size, 0, 0, (float)size,   0, 0, -dx, dy, 0, (float)size);
	ret = SetWorldTransform(hDC, & af.m_xm);
	Face(hDC, RGB(0, 0xFF, 0), size);

	af.MapTri(0, 0, (float)size, 0, 0, (float)size,   0, (float)size, dx, size + dy, -dx, size + dy);
	ret = SetWorldTransform(hDC, & af.m_xm);
	Face(hDC, RGB(0, 0, 0xFF), size);
*/   
}	
//---------------------------------------------------------------------------
void __fastcall TLoadPreview_frm::FormPaint(TObject *Sender)
{
   pic.setSizeScreen(ClientWidth, ClientHeight);
   pic.setCanvas(Canvas);
   pic.WorkPlace()->setEditProcessing(true);
   pic.Show();
   pic.WorkPlace()->setEditProcessing(false);
}
//---------------------------------------------------------------------------
void __fastcall TLoadPreview_frm::FormResize(TObject *Sender)
{
   Paint();   
}
//---------------------------------------------------------------------------
