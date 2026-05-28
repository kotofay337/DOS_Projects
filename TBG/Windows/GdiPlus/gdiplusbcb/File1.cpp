

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "gdiplus.h"
using namespace Gdiplus;
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma argsused
int DemoGDIPlus(HDC hDC)
{
   using namespace Gdiplus;
   using namespace DllExports;

   ULONG_PTR token;
   Status s;
   GdiplusStartupInput input;
   GdiplusStartupOutput output;

   s = GdiplusStartup(&token, &input, &output);

   SolidBrush *ClearBrush = new SolidBrush(Color(180, 255, 255, 255));

   Graphics* graphics = new Graphics(hDC);
   graphics->SetSmoothingMode(SmoothingModeAntiAlias);

   Pen* pen = new Pen(Color(64, 128, 256, 0x55));
   Pen* f = new Pen(Color(170, 256, 256, 256));


   graphics->DrawRectangle(f, 0, 0, 800, 600);
   graphics->DrawLine(pen, 50, 50, 350, 550);
   graphics->DrawRectangle(pen, 50, 50, 300, 500);
   graphics->DrawEllipse(pen, 50, 50, 300, 500);
   delete pen;
   delete f;


   {// Hatch Style demo
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);
      HatchStyle hs = HatchStyleHorizontal;
      for(int y = 0;y < 7;y++)
         for(int x = 0;x < 8;x++){
            if(hs < HatchStyleMax){
               HatchBrush hBrush(hs, Color(128, 255, 0, 0),  // ARGB
                                     Color(128, 255, 255, 0));
               graphics->FillEllipse(&hBrush, x * 40 + 350, y * 40, 40, 40);
               hs++;
            }
         }
   }

   {
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);

      Point points[] = {Point(100, 0), Point(200, 200), Point(0, 200)};
      PathGradientBrush pthGrBrush(points, 3);

      Color col[] = {
         Color(255, 255, 0, 0),     // red
         Color(255, 0, 0, 255),     // blue
         Color(255, 0, 255, 255)};  // aqua

      REAL pos[] = {
         0.0f,    // red at the boundary
         0.4f,    // blue 40 percent of the way from the boundary to the center
         1.0f};   // aqua at the center

      pthGrBrush.SetInterpolationColors(col, pos, 3);
      graphics->FillRectangle(&pthGrBrush, 0, 0, 300, 300);
   }

   {
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);
      // Create a path that consists of a single ellipse.
      GraphicsPath path;
      path.AddEllipse(0, 0, 200, 100);

      // Use the path to construct a brush.
      PathGradientBrush pthGrBrush(&path);

      // Set the color at the center of the path to red.
      pthGrBrush.SetCenterColor(Color(255, 255, 0, 0));

      // Set the color along the entire boundary of the path to blue.
      Color colors[] = {Color(255, 0, 0, 255)};
      INT count = 1;
      pthGrBrush.SetSurroundColors(colors, &count);

      pthGrBrush.SetBlendBellShape(0.4f, 0.5f);

      // The color is blue on the boundary and at the center.
      // At points that are 20 percent of the way from the boundary to the
      // center, the color is 70 percent red and 30 percent blue.

      graphics->FillRectangle(&pthGrBrush, 0, 0, 300, 300);
   }

   {
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);
      // Put the points of a polygon in an array.
      Point points[] = {Point(75, 0), Point(100, 50),
                        Point(150, 50), Point(112, 75),
                        Point(150, 150), Point(75, 100),
                        Point(0, 150), Point(37, 75),
                        Point(0, 50), Point(50, 50)};

      // Use the array of points to construct a path.
      GraphicsPath path;
      path.AddLines(points, 10);

      // Use the path to construct a path gradient brush.
      PathGradientBrush pthGrBrush(&path);

      // Set the color at the center of the path to red.
      pthGrBrush.SetCenterColor(Color(255, 255, 0, 0));

      // Set the colors of the points in the array.
      Color colors[] = {Color(255, 0, 0, 0),   Color(255, 0, 255, 0),
                        Color(255, 0, 0, 255), Color(255, 255, 255, 255),
                        Color(255, 0, 0, 0),   Color(255, 0, 255, 0),
                        Color(255, 0, 0, 255), Color(255, 255, 255, 255),
                        Color(255, 0, 0, 0),   Color(255, 0, 255, 0)};

      int count = 10;
      pthGrBrush.SetSurroundColors(colors, &count);

      // Fill the path with the path gradient brush.
      graphics->FillPath(&pthGrBrush, &path);
      pthGrBrush.SetGammaCorrection(TRUE);
      graphics->TranslateTransform(200.0f, 0.0f);
      graphics->FillPath(&pthGrBrush, &path);
      graphics->ResetTransform();
   }

   {
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);
      // Construct a path gradient brush based on an array of points.
      PointF ptsF[] = {PointF(0.0f, 0.0f),
                       PointF(80.0f, 0.0f),
                       PointF(80.0f, 100.0f),
                       PointF(40.0f, 75.0f),
                       PointF(0.0f, 100.0f)};

      PathGradientBrush pBrush(ptsF, 5);

      // An array of five points was used to construct the path gradient
      // brush. Set the color of each point in that array.
      Color colors[] = {Color(255, 255, 0, 0),  // (0, 0) red
                        Color(255, 0, 255, 0),  // (160, 0) green
                        Color(255, 0, 255, 0),  // (160, 200) green
                        Color(255, 0, 0, 255),  // (80, 150) blue
                        Color(255, 255, 0, 0)}; // (0, 200) red

      int count = 5;
      pBrush.SetSurroundColors(colors, &count);

      // Set the center color to white.
      pBrush.SetCenterColor(Color(255, 255, 255, 255));

      // Use the path gradient brush to fill a rectangle.
      graphics->FillRectangle(&pBrush, Rect(0, 0, 180, 220));
   }


   {
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);
      // Create a path that consists of a single ellipse.
      GraphicsPath path;
      path.AddEllipse(0, 0, 140, 70);

      // Use the path to construct a brush.
      PathGradientBrush pthGrBrush(&path);

      // Set the color at the center of the path to blue.
      pthGrBrush.SetCenterColor(Color(255, 0, 0, 255));

      // Set the color along the entire boundary of the path to aqua.
      Color colors[] = {Color(255, 0, 255, 255)};
      int count = 1;
      pthGrBrush.SetSurroundColors(colors, &count);

      graphics->FillEllipse(&pthGrBrush, 0, 0, 140, 70);
   }

   {
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);
      Pen pen(Color(255, 0, 0, 255), 8);
      pen.SetStartCap(LineCapArrowAnchor);
      pen.SetEndCap(LineCapRoundAnchor);
      graphics->DrawLine(&pen, 20, 175, 300, 175);
   }

   {
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);
      Pen pen(Color(255, 255, 0, 0));
      graphics->ResetTransform();
      graphics->RotateTransform(30.0f);            // World transformation
      graphics->DrawEllipse(&pen, 30, 0, 50, 25);
      graphics->SetPageUnit(UnitMillimeter);       // Page transformation
      graphics->DrawEllipse(&pen, 30, 0, 50, 25);

      graphics->ResetTransform();
      graphics->SetPageUnit(UnitPixel);       // Page transformation
   }

   {
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);
      WCHAR string[] =
         L"Use StringFormat and RectF objects to center text in a rectangle.";

      FontFamily   fontFamily(L"Arial");
      Font         font(&fontFamily, 16, FontStyleBold, UnitPoint);
      RectF        rectF(180.0f, 120.0f, 180.0f, 180.0f);
      StringFormat stringFormat;
      SolidBrush   sBrush(Color(255, 0, 0, 255));
      HatchBrush   hBrush(HatchStyleSmallCheckerBoard, Color(255, 255,   0,   0),  // ARGB
                                                       Color(128,  255, 255, 255));
      // Center-justify each line of text.
      stringFormat.SetAlignment(StringAlignmentCenter);

      // Center the block of text (top to bottom) in the rectangle.
      stringFormat.SetLineAlignment(StringAlignmentCenter);

      graphics->DrawString(string, -1, &font, rectF, &stringFormat, &hBrush);

      Pen pen(Color(255, 0, 0, 0));
      graphics->DrawRectangle(&pen, rectF);
   }

///*
   {
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);
      Pen pen(Color(255, 0, 0, 255));

      Matrix matrix;
      matrix.Scale(2.0f, 2.0f);                     // first a scaling
      matrix.Shear(3.0f, 0.0f, MatrixOrderAppend);  // then a shear

      graphics->SetTransform(&matrix);
      graphics->DrawRectangle(&pen, 0, 0, 100, 50);
      graphics->ResetTransform();
      graphics->SetPageUnit(UnitPixel);       // Page transformation
   }
//*/
   {
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);
      // Create a blank bitmap.
      Bitmap bitmap(180, 100);

      // Create a Graphics object that we can use to draw on the bitmap.
      Graphics bitmapGraphics(&bitmap);

      // Create a red brush and a green brush, each with an alpha value of 160.
      SolidBrush redBrush(Color(210, 255, 0, 0));
      SolidBrush greenBrush(Color(210, 0, 255, 0));

      // Set the compositing mode so that when we draw overlapping ellipses,
      // the colors of the ellipses are not blended.
      bitmapGraphics.SetCompositingMode(CompositingModeSourceCopy);

      // Fill an ellipse using a red brush that has an alpha value of 160.
      bitmapGraphics.FillEllipse(&redBrush, 0, 0, 150, 70);

      // Fill a second ellipse using green brush that has an alpha value of 160.
      // The green ellipse overlaps the red ellipse, but the green is not
      // blended with the red.
      bitmapGraphics.FillEllipse(&greenBrush, 30, 30, 150, 70);

      // Prepare to draw on the screen.
      graphics->SetCompositingQuality(CompositingQualityGammaCorrected);

      // Draw a multicolored background.
      SolidBrush brush(Color((ARGB)Color::Aqua));
      graphics->FillRectangle(&brush, 200, 0, 60, 100);
      brush.SetColor(Color((ARGB)Color::Yellow));
      graphics->FillRectangle(&brush, 260, 0, 60, 100);
      brush.SetColor(Color((ARGB)Color::Fuchsia));
      graphics->FillRectangle(&brush, 320, 0, 60, 100);

      // Display the bitmap on a white background.
      graphics->DrawImage(&bitmap, 0, 0);

      // Display the bitmap on a multicolored background.
      graphics->DrawImage(&bitmap, 200, 0);
   }

   {
      Sleep(5000);
      graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);

      // Brushes and pens used for drawing and painting
      SolidBrush blueBrush(Color(255, 0, 0, 255));
      SolidBrush redBrush(Color(100, 255, 0, 0));
      Pen        blackPen(Color(255, 0, 0, 0));

      // Layout rectangle used for drawing string
      RectF   layoutRect(20.0f, 220.0f, 220.0f, 240.0f);

      // Three ranges of character positions within the string
      CharacterRange charRanges[3] = { CharacterRange(2, 3),
                                       CharacterRange(14, 17),
                                       CharacterRange(24, 30), };

      // Font and string format used to apply to string when drawing
      Font         myFont(L"Times New Roman", 16.0f);
      StringFormat strFormat;

       // Other variables
      Region* pCharRangeRegions; // pointer to CharacterRange regions
      short   i;                 // loop counter
      INT     count;             // number of character ranges set
      WCHAR   string[] = L"The quick, brown fox easily jumps over the lazy dog.\nВ чащах юга жил-был цитрус, да, но фальшивый экземпляр.";

      // Set three ranges of character positions.
      strFormat.SetMeasurableCharacterRanges(3, charRanges);

      // Get the number of ranges that have been set, and allocate memory to
      // store the regions that correspond to the ranges.
      count = strFormat.GetMeasurableCharacterRangeCount();
      pCharRangeRegions = new Region[count];

      // Get the regions that correspond to the ranges within the string.
      // Then draw the string and show the regions.
      graphics->MeasureCharacterRanges(string, -1,
         &myFont, layoutRect, &strFormat, count, pCharRangeRegions);
      graphics->DrawString(string, -1,
         &myFont, layoutRect, &strFormat, &blueBrush);
      graphics->DrawRectangle(&blackPen, layoutRect);
      for ( i = 0; i < count; i++)
      {
         graphics->FillRegion(&redBrush, pCharRangeRegions + i);
      }

      delete[] pCharRangeRegions;
   }

   Sleep(5000);
   graphics->FillRectangle(ClearBrush, 0, 0, 800, 600);

   delete ClearBrush;
   delete graphics;
   GdiplusShutdown(token);
   
	return 0;
}
//---------------------------------------------------------------------------

