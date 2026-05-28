/* TBG_Global_data.CPP
*
*   TBG v7.0
*   20-09-2001 8:33 am msk
*
*/

#include "tbg.hpp"
#include "TBG_Editor.hpp"

/* -------------------------------------------------------------------------- */
// Описание глобальных переменных
/* -------------------------------------------------------------------------- */
int   ARGC;
char **ARGV, *ENV;

Commands pc;

int MouseX = 0, OffsetX = 0,
    MouseY = 0, OffsetY = 0;

unsigned char WidthLine = 1;
TPenStyle     StyleLine = psSolid;
TColor        ColorLine = clBlack,
              ColorFill = clWhite,
              ColorText = clBlack;
TBrushStyle   StyleFill = bsSolid;
GradientFillStyle GradientStyleFill = gfsLeftToRight;

char sTextBGI[256];
int  iFntBGI = 10;
int  SizeFntBGI = 4;
double dAngleText = 0.0;

TPictureTBG *EditPic;

LinkList<TBaseGP*> cb; // Clipboard

OSVERSIONINFO vos;


