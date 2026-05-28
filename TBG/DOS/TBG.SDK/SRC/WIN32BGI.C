#include <tbg_tc.h>
/* ----------------------------------------------------------------------- */
#ifdef __WIN32__
HDC TBGtoDC(OBJECT **obj, HDC hDC){
   HDC hDCOld;
   SIZE Size;

  if(obj == NULL)
      return NULL;

  setstateTBG(obj, TBG_BUSY);

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
      return NULL;

   hDCOld = ((struct HTBG*)*(obj))->hdc;
   ((struct HTBG*)*(obj))->hdc = hDC;
   SetBkMode(hDC, TRANSPARENT);

   setstateTBG(obj, TBG_FREE);
   return hDCOld;

}/* TBGtoDC */
/* ----------------------------------------------------------------------- */
HDC TBG_SetDC(OBJECT **obj, HDC hdc){

   return TBGtoDC(obj, hdc);

}/* TBG_SetDC */
/* ----------------------------------------------------------------------- */
HDC TBG_GetDC(OBJECT **obj){

  if(obj == NULL)
      return NULL;
  setstateTBG(obj, TBG_BUSY);

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
      return NULL;

   setstateTBG(obj, TBG_FREE);
   return ((struct HTBG*)*(obj))->hdc;

}/* TBG_GetDC */
/* ----------------------------------------------------------------------- */
HWND TBG_SetWND(OBJECT **obj, HWND hwnd){
   RECT r;
   HWND hWNDOld;

  if(obj == NULL)
      return NULL;
   setstateTBG(obj, TBG_BUSY);

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
      return NULL;

   hWNDOld = ((struct HTBG*)*(obj))->hwnd;
   ((struct HTBG*)*(obj))->hwnd = hwnd;

   GetClientRect(((struct HTBG*)*(obj))->hwnd, &r);
   ((struct HTBG*)*(obj))->SIZEX = r.right < 640 ? 640 : r.right;
   ((struct HTBG*)*(obj))->SIZEY = r.bottom < 480 ? 480 : r.bottom;

   setstateTBG(obj, TBG_FREE);
   return hWNDOld;

}/* TBG_SetWND */
/* ----------------------------------------------------------------------- */
HWND TBG_GetWND(OBJECT **obj){

  if(obj == NULL)
      return NULL;
   setstateTBG(obj, TBG_BUSY);

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
      return NULL;

   setstateTBG(obj, TBG_FREE);
   return ((struct HTBG*)*(obj))->hwnd;

}/* TBG_GetWND */
/* ----------------------------------------------------------------------- */
BOOL TBG_GetUpdateState(OBJECT **obj){

  if(obj == NULL)
      return NULL;
   setstateTBG(obj, TBG_BUSY);

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
      return NULL;

   setstateTBG(obj, TBG_FREE);
   return ((struct HTBG*)*(obj))->NeedUpdateFlag;

}/* TBG_GetUpdateState */
/* ----------------------------------------------------------------------- */
BOOL TBG_UpdateOK(OBJECT **obj){

  if(obj == NULL)
      return (BOOL)-1;
   setstateTBG(obj, TBG_BUSY);

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
      return (BOOL)-1;

   setstateTBG(obj, TBG_FREE);
   return ((struct HTBG*)*(obj))->NeedUpdateFlag = FALSE;

}/* TBG_UpdateOK */
/* ----------------------------------------------------------------------- */
#ifdef _MGL_
#else
EGATools cEGA[16]={
   {BLACK,       RGB(0x00,0x00,0x00)},
   {BLUE,        RGB(0x00,0x00,0xAA)},
   {GREEN,       RGB(0x00,0xAA,0x00)},
   {CYAN,        RGB(0x00,0xAA,0xAA)},
   {RED,         RGB(0xAA,0x00,0x00)},
   {GOLD,        RGB(0xFF,0xAA,0x00)},
   {BOLOTO,      RGB(0xAA,0xAA,0x5F)},
   {LIGHTGRAY,   RGB(0xAA,0xAA,0xAA)},
   {DARKGRAY,    RGB(0x5A,0x5A,0x5A)},
   {LIGHTBLUE,   RGB(0x00,0x00,0xFF)},
   {LIGHTGREEN,  RGB(0x00,0xFF,0x00)},
   {LIGHTCYAN,   RGB(0x00,0xFF,0xFF)},
   {LIGHTRED,    RGB(0xFF,0x00,0x00)},
   {PASTEL,      RGB(0xFF,0xAA,0xAA)},
   {YELLOW,      RGB(0xFF,0xFF,0x00)},
   {WHITE,       RGB(0xFF,0xFF,0xFF)}
};

HPALETTE hPal, holdPal, holdPalPaint;
LPLOGPALETTE lpLogPal;
#endif

/* -------------------------------------------------------------------------- */
int CreateEGATools(void){
#ifdef _MGL_
#else
   int i;
   COLORREF c;

   lpLogPal = (LPLOGPALETTE)GlobalAlloc(GMEM_FIXED,
              (sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * MyPALSIZE));

   lpLogPal->palVersion = 0x300;
   lpLogPal->palNumEntries = MyPALSIZE;

   for(i = BLACK;i <= WHITE;i++){

      c = cEGA[i].rgb;

      cEGA[i].hPen[SOLID_LINE]   = CreatePen(PS_SOLID,   1, c);
      cEGA[i].hPen[DOTTED_LINE]  = CreatePen(PS_DOT,     1, c);
      cEGA[i].hPen[CENTER_LINE]  = CreatePen(PS_DASHDOT, 1, c);
      cEGA[i].hPen[DASHED_LINE]  = CreatePen(PS_DASH,    1, c);
      cEGA[i].hPen[USERBIT_LINE] = CreatePen(PS_SOLID,   2, c);

      cEGA[i].hBrush[SOLID_FILL]      = CreateSolidBrush(c);
      cEGA[i].hBrush[LINE_FILL]       = CreateHatchBrush(HS_HORIZONTAL, c);
      cEGA[i].hBrush[LTSLASH_FILL]    =
      cEGA[i].hBrush[SLASH_FILL]      = CreateHatchBrush(HS_BDIAGONAL,  c);
      cEGA[i].hBrush[BKSLASH_FILL]    =
      cEGA[i].hBrush[LTBKSLASH_FILL]  = CreateHatchBrush(HS_FDIAGONAL,  c);
      cEGA[i].hBrush[XHATCH_FILL]     =
      cEGA[i].hBrush[INTERLEAVE_FILL] =
      cEGA[i].hBrush[CLOSE_DOT_FILL]  = CreateHatchBrush(HS_DIAGCROSS,  c);
      cEGA[i].hBrush[HATCH_FILL]      =
      cEGA[i].hBrush[WIDE_DOT_FILL]   = CreateHatchBrush(HS_CROSS,      c);

      lpLogPal->palPalEntry[i].peRed   = GetRValue(c);
      lpLogPal->palPalEntry[i].peGreen = GetGValue(c);
      lpLogPal->palPalEntry[i].peBlue  = GetBValue(c);
      lpLogPal->palPalEntry[i].peFlags = NULL;

   }/* for */

   hPal = CreatePalette((LPLOGPALETTE)lpLogPal);
   if(hPal == NULL){
      MessageBox(HWND_DESKTOP, "Палитра не создана.", "Создание палитры", MB_OK);
   }/*  */

   GlobalFree(lpLogPal);

   return 0;
#endif

}/* CreateEGAPens */
/* -------------------------------------------------------------------------- */
int DeleteEGATools(void){
#ifdef _MGL_
#else
   int i;

   DeleteObject(hPal);

   for(i = BLACK;i <= WHITE;i++){
      DeleteObject(cEGA[i].hPen[SOLID_LINE]);
      DeleteObject(cEGA[i].hPen[DOTTED_LINE]);
      DeleteObject(cEGA[i].hPen[CENTER_LINE]);
      DeleteObject(cEGA[i].hPen[DASHED_LINE]);
      DeleteObject(cEGA[i].hPen[USERBIT_LINE]);

      DeleteObject(cEGA[i].hBrush[SOLID_FILL]);
      DeleteObject(cEGA[i].hBrush[LINE_FILL]);
      DeleteObject(cEGA[i].hBrush[SLASH_FILL]);
      DeleteObject(cEGA[i].hBrush[LTBKSLASH_FILL]);
      DeleteObject(cEGA[i].hBrush[CLOSE_DOT_FILL]);
      DeleteObject(cEGA[i].hBrush[WIDE_DOT_FILL] );
   }/* for */

   return 0;
#endif

}/* DeleteEGAPens */
/* ----------------------------------------------------------------------- */
#ifdef _MGL_
#else
void Line(HDC hdc, int left, int top, int right, int bottom){
   /* Microsoft очень умный, он не ставит последнюю точку на линии. */
   LOGPEN lp;
   GetObject(GetCurrentObject(hdc, OBJ_PEN), sizeof(lp), &lp);
   if(
      ((GetRValue(lp.lopnColor) == 0) || (GetRValue(lp.lopnColor) == 0x5A)) &&
      ((GetGValue(lp.lopnColor) == 0) || (GetGValue(lp.lopnColor) == 0x5A)) &&
      ((GetBValue(lp.lopnColor) == 0) || (GetBValue(lp.lopnColor) == 0x5A)) &&
      (lp.lopnWidth.x == 1) &&
      (lp.lopnStyle == PS_SOLID)&&
      (SmoothEnable)
     ){
      WuLine(hdc, left, top, right, bottom, lp.lopnColor, 255, 8);
   }/* if */
   else{
      MoveToEx(hdc, left, top, NULL);
      LineTo(hdc, right, bottom);
      SetPixel(hdc, right, bottom, lp.lopnColor);
   }/* if */
}/* Line */
#endif
/* -------------------------------------------------------------------------- */
#ifdef _MGL_
#else
void Bar(HDC hdc, int left, int top, int right, int bottom){
   RECT r;
   /* Microsoft очень умный, не смог сделать координаты по нормальному,
      допустим, как в BGI */
   r.left   = left   < right  ? left       : right     ;
   r.top    = top    < bottom ? top        : bottom    ;
   r.right  = right  > left   ? right      : left      ;
   r.bottom = bottom > top    ? bottom     : top       ;
   r.right++;
   r.bottom++;
   FillRect(hdc, &r, (HBRUSH)GetCurrentObject(hdc, OBJ_BRUSH));
}/* Bar */
#endif
/* -------------------------------------------------------------------------- */
#ifdef _MGL_
#else
void Rect(HDC hdc, int left, int top, int right, int bottom){
   MoveToEx(hdc, left, top, NULL);
   LineTo(hdc, right, top);
   LineTo(hdc, right, bottom);
   LineTo(hdc, left, bottom);
   LineTo(hdc, left, top);
}/* Rect */
#endif
/* -------------------------------------------------------------------------- */
#endif

