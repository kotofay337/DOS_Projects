/****************************************************************************
*
*                   Copyright (C) 1997 KTO Software.
*                         All rights reserved.
*
* Filename:     $Workfile:   font.c $
* Version:      $Revision:   3.0  $
*
* Language:     ANSI C
* Environment:  WIN32+WINDOWS/WIN32+MGL
*
* Description:  This module contain subroutine emulate
*               BGI action with CHR stroke font
*
*
* $Date:   10 Jul 1997 22:17:44  $ $Author:   Tchumark A$
*
****************************************************************************/
#ifndef __WIN32__
   #ifdef _MGL16_DOS__
   #include <mgraph.h>
   #else /* _MGL16_DOS__ */
   #include <graphics.h>
   #endif /* _MGL16_DOS__ */
#else /* __WIN32__ */
   #ifdef _MGL32_WIN__
   #else /* _MGL32_WIN__ */
   #include <windows.h>
   #endif /* _MGL32_WIN__ */
#endif /* __WIN32__ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>
#include <math.h>
#include "tbg_tc.h"
// - Определения типов данных -----------------------------------------------
typedef
   struct {
        signed coo :7;
      unsigned op  :1;
   } DECODE;
//- Локальные прототипы функций ---------------------------------------------
static void CalcJustify(const char *s, short int *x, short int *y);
#ifndef __WIN32__
   #ifdef _MGL16_DOS__
   #else /* _MGL16_DOS__ */
   #endif /* _MGL16_DOS__ */
static void DrawChar(DECODE *Ch);
#else /* __WIN32__ */
   #ifdef _MGL32_WIN__
   #else /* _MGL32_WIN__ */
static void DrawChar(HDC hdc, DECODE *Ch);
   #endif /* _MGL32_WIN__ */
#endif /* __WIN32__ */
static LPBGIFONT fload(int font);
static DECODE *SymFind(unsigned char c);
//- Локальные константы -----------------------------------------------------
#define MOVE  0
#define DRAW  1
#define CCalcX(x) (ScaleX * UserScaleX * ((double)(x)))
#define CCalcY(y) (ScaleY * UserScaleY * ((double)(y)))
//- Локальные данные --------------------------------------------------------
typedef
   struct FTABLE{
      char *fname;
      int  resnum;
      char *pf;
   } FTABLE, *LPFTABLE;
//- Локальные данные --------------------------------------------------------
static FTABLE FontTable[MaxFONT]={
   {"SIMP.CHR", _DEFAULT_CHR, NULL},
   {"TRIP.CHR", _TRIP_CHR   , NULL},
   {"LITT.CHR", _LITT_CHR   , NULL},
   {"SANS.CHR", _SANS_CHR   , NULL},
   {"GOTH.CHR", _GOTH_CHR   , NULL},
   {"SCRI.CHR", _SCRI_CHR   , NULL},
   {"SIMP.CHR", _SIMP_CHR   , NULL},
   {"TSCR.CHR", _TSCR_CHR   , NULL},
   {"LCOM.CHR", _LCOM_CHR   , NULL},
   {"EURO.CHR", _EURO_CHR   , NULL},
   {"BOLD.CHR", _BOLD_CHR   , NULL}
};
static LPBGIFONT bf[MaxFONT] = { NULL, };
static int       ff[MaxFONT] = { 0, };
static short int cf = DEFAULT_FONT;
static double    tsize[11] = {1., 1., 1., 1., 1.,
                              1.333 , 1.666 , 1.999,
                              2.4985, 2.998 , 3.997 };
static short int cX,cY;
static double    UserScaleX     = 1.,
                 UserScaleY     = 1.,
                 ScaleX         = 1.,
                 ScaleY         = 1.,
                 Angle          = 0.;
static short int rX = 0, rY = 0; // Центр поворота
static short int TextVerJustify = BOTTOM_TEXT,
                 TextHorJustify = LEFT_TEXT,
                 direction      = HORIZ_DIR,
                 tSize          = 4;
#ifndef __WIN32__
static char pathtofont[MAXDRIVE + MAXDIR + MAXFILE + MAXEXT]
#else
static char pathtofont[512]
#endif
 ={".\\"};
//- Начало описания функций -------------------------------------------------
void DLL_EI GetTextSettings(struct TextSettingsType *ti){
#ifdef __WIN32__
   ti->hdc = bf[cf]->hdc;
#endif
    ti->font = cf;
    ti->direction = direction;
    ti->charsize = tSize;
    ti->horiz = TextHorJustify;
    ti->vert = TextVerJustify;

    // it's my custom data
    ti->ScaleX = ScaleX;
    ti->ScaleY = ScaleY;
    ti->UserScaleX = UserScaleX;
    ti->UserScaleY = UserScaleY;
}/* GetTextSettings */
/*--------------------------------------------------------------------------*/
void DLL_EI SetTextSettings(struct TextSettingsType *ti){
#ifdef __WIN32__
   bf[cf]->hdc = ti->hdc;
#endif
    cf = ti->font;
    direction = ti->direction;
    tSize = ti->charsize;
    TextHorJustify = ti->horiz;
    TextVerJustify = ti->vert;

    // it's my custom data
    ScaleX = ti->ScaleX;
    ScaleY = ti->ScaleY;
    UserScaleX = ti->UserScaleX;
    UserScaleY = ti->UserScaleY;
}/* SetTextSettings */
/*--------------------------------------------------------------------------*/
/* (C) Чумак А.И. 1994 */
/*--------------------------------------------------------------------------*/
// ф-ия вращения точки
void rotate_point(double a,double *Xt,double *Yt,double Xc,double Yc)
{

  static short int xa = 0,ya = 0;
  double dXt = (*Xt) - Xc,
         dYt = (*Yt) - Yc,
         sina,cosa,
         tx,ty;

  a /= 57.29577951472;

  sina = sin(a);
  cosa = cos(a);

  tx = dXt;
  ty = dYt;

  dXt=(tx * cosa) - (ty * sina);
  dYt=(tx * sina) + (ty * cosa);
#ifndef __WIN32__
  if(!ya){ // взять отношение сторон дисплея
     getaspectratio((int far *)&xa, (int far *)&ya);
  }// if

  // скорректировать "неквадратность" точки
  if((xa != 10000)||(ya != 10000)){
     dYt /= ((double)ya / 10000);
     dXt /= ((double)xa / 10000);
  }// if
#endif
  *Xt = Xc + dXt;
  *Yt = Yc + dYt;
}// rotate_point
/*--------------------------------------------------------------------------*/
static LPBGIFONT fload(int font){
   FILE *f;
   unsigned long fsize;
   unsigned char *pf, *tp;
   LPBGIFONT lpbf;
#ifndef __WIN32__
   char s[MAXDRIVE + MAXDIR + MAXFILE + MAXEXT];
#else
   char s[512];
#endif

   // printf(".in fload.\n");  // fnsplit
   if(ff[font] == 0){
      strcpy(s, pathtofont);
      strcat(s, FontTable[font].fname);
      if((f = fopen(s, "rb")) == NULL){
         error_tbg = ER_LOADBGIFONT;
         return NULL;
      }/* if */
      // printf(".fopen: %p\n", f);
      fseek(f, 0L, SEEK_END);
      fsize = ftell(f);
      fseek(f, 0L, SEEK_SET);
      // printf(".fsize: %ld\n", fsize);
      if((pf = (unsigned char*)calloc(1, fsize)) == NULL){
         fclose(f);
         error_tbg = ER_LOADBGIFONT;
         return NULL;
      }/* if */
      if(fread(pf, 1, fsize, f) != fsize){
         free(pf);
         fclose(f);
         error_tbg = ER_LOADBGIFONT;
         return NULL;
      }/* if */
      fclose(f);
   }/* if */
   else{
#ifdef __WIN32__
      LPTSTR lpsres = MAKEINTRESOURCE(FontTable[font].resnum);
      LPTSTR lpsfontbgi = MAKEINTRESOURCE(FONTBGI);
      HRSRC  hrsrc = FindResource(NULL, lpsres, lpsfontbgi);
      HGLOBAL hG = LoadResource(NULL, hrsrc);
      if((pf = (unsigned char*)LockResource(hG)) == NULL){
         error_tbg = ER_LOADBGIFONT;
         return NULL;
      }/* if */
#else
#endif
   }/* elif */

   if(*((unsigned long*)pf) != 0x08084B50UL){/* "PK\8\8" */
      free(pf);
      error_tbg = ER_LOADBGIFONT;
      return NULL;
   }/* if */

   if((lpbf = (LPBGIFONT)calloc(1, sizeof(BGIFONT))) == NULL){
      fclose(f);
      error_tbg = ER_LOADBGIFONT;
      return NULL;
   }/* if */
   lpbf->pFilefont = pf;
   tp = pf;
   while(*tp++ != 0x1a);/* '^z' - end header */
   // printf(".end header.\n");
   lpbf->pFHdr = (LPHBGIFONTFILE)tp;
   tp = pf;
   while(*tp != 0x2b)/* '+' - start information */
      tp++;
   // printf(".start info.\n");
   lpbf->pIFnt = (LPIBGIFONT)tp;
   lpbf->pOffsSD = (unsigned short int*)(((char*)lpbf->pIFnt) + sizeof(IBGIFONT));
   lpbf->pWidth = (unsigned char*)((char*)lpbf->pIFnt + sizeof(IBGIFONT) +
                          lpbf->pIFnt->NumChar_inFont * sizeof(short int));
#ifdef __WIN32__
   lpbf->hdc = NULL;
#endif
   // printf(".ret: %p\n", lpbf);
   error_tbg = ER_OK;
   return lpbf;
}/* fload */
//---------------------------------------------------------------------------
int RegisterBGIFont(int i){
   if((i < 0) || (i > (sizeof(FontTable) / sizeof(FTABLE)))){
      error_tbg = ER_OPERBGIFONT;
      return -1;
   }/* if */
   if(ff[i] == 0){
      if(bf[i] != NULL){
         free(bf[i]->pFilefont);
         free(bf[i]);
      }/* if */
      bf[i] = NULL;
      ff[i] = 1;
   }/* if */
   return 0;
}/* registerbgifont */
//---------------------------------------------------------------------------
int UnRegisterBGIFont(int i){
   if((i < 0) || (i > (sizeof(FontTable) / sizeof(FTABLE)))){
      error_tbg = ER_OPERBGIFONT;
      return -1;
   }/* if */
   if(ff[i] == 1){
      bf[i] = NULL;
      ff[i] = 0;
   }/* if */
   return 0;
}/* unregisterbgifont */
//---------------------------------------------------------------------------
void SetPathToBGIFont(char* p){
   strncpy(pathtofont, p, sizeof(pathtofont) - 1);
}/* SetPathToBGIFont */
//---------------------------------------------------------------------------
void SetTextStyle(int font, int dir, int charsize){
   if((font < 0) || (font > (sizeof(FontTable) / sizeof(FTABLE)))){
      if(bf[DEFAULT_FONT] == NULL){
         RegisterBGIFont(DEFAULT_FONT);
         if((bf[DEFAULT_FONT] = fload(DEFAULT_FONT)) == NULL){
            UnRegisterBGIFont(DEFAULT_FONT);
            if((bf[DEFAULT_FONT] = fload(DEFAULT_FONT)) == NULL){
               cf = DEFAULT_FONT;
               error_tbg = ER_OPERBGIFONT;
               return;
            }/* if */
         }/* if */
      }/* if */
      ScaleX = ScaleY = tsize[4];
      direction = (short int)dir;
      cf = DEFAULT_FONT;
      return;
   }/* if */
   if((charsize >= 0) && (charsize <= 10))
      tSize = (short int)charsize;
   else
      tSize = 4;
   if(bf[font] != NULL){
      ScaleX = ScaleY = tsize[tSize];
      direction = (short int)dir;
      cf = (short int)font;
      error_tbg = ER_OK;
      return;
   }/* if */
   bf[font] = fload(font);
   if(bf[font] == NULL){
      if(bf[DEFAULT_FONT] == NULL){
         RegisterBGIFont(DEFAULT_FONT);
         if((bf[DEFAULT_FONT] = fload(DEFAULT_FONT)) == NULL){
            UnRegisterBGIFont(DEFAULT_FONT);
            if((bf[DEFAULT_FONT] = fload(DEFAULT_FONT)) == NULL){
               error_tbg = ER_OPERBGIFONT;
               return;
            }/* if */
         }/* if */
      }/* if */
      ScaleX = ScaleY = tsize[4];
      direction = (short int)dir;
      cf = DEFAULT_FONT;
      error_tbg = ER_OK;
      return;
   }/* if */
   ScaleX = ScaleY = tsize[tSize];
   direction = (short int)dir;
   cf = (short int)font;
   error_tbg = ER_OK;
   return;
}/* SetTextStyle */
//---------------------------------------------------------------------------
void UnloadAllFont(void){
   int i;
   for(i = 0;i < MaxFONT;i++)
      if(bf[i] != NULL){
         if(ff[i] == 0){
            free(bf[i]->pFilefont);
            free(bf[i]);
         }/* if */
         bf[i] = NULL;
      }/* if */
}/* UnloadAllFont */
//---------------------------------------------------------------------------
void SetTextJustify(int horiz, int vert){
   if((horiz <= RIGHT_TEXT) || (horiz >= LEFT_TEXT))
      TextHorJustify = (short int)horiz;
   if((vert <= TOP_TEXT) || (vert >= BOTTOM_TEXT))
      TextVerJustify = (short int)vert;
}/* SetTextJustify */
//---------------------------------------------------------------------------
#pragma argsused
#ifndef __WIN32__
   #ifdef _MGL16_DOS__
   #else /* _MGL16_DOS__ */
   #endif /* _MGL16_DOS__ */
static void DrawChar(DECODE *Ch){
#else /* __WIN32__ */
   #ifdef _MGL32_WIN__
   #else /* _MGL32_WIN__ */
static void DrawChar(HDC hdc, DECODE *Ch){
   #endif /* _MGL32_WIN__ */
#endif /* __WIN32__ */

   unsigned Mode;
   double xx, yy;
   short int   x, y;
   short int  ax,ay;
   DECODE X, Y;
   double stx, sty;

#ifndef __WIN32__
   #ifdef _MGL16_DOS__
   #else /* _MGL16_DOS__ */
   #endif /* _MGL16_DOS__ */
#else /* __WIN32__ */
   #ifdef _MGL32_WIN__
   #else /* _MGL32_WIN__ */
   LOGPEN lp;
   #endif /* _MGL32_WIN__ */
#endif /* __WIN32__ */
   // printf(".in DrawChar\n");
   if(Ch == NULL)
      return;
#ifndef __WIN32__
   #ifdef _MGL16_DOS__
   #else /* _MGL16_DOS__ */
   #endif /* _MGL16_DOS__ */
#else /* __WIN32__ */
   #ifdef _MGL32_WIN__
   #else /* _MGL32_WIN__ */
   //GetObject(GetCurrentObject(hdc, OBJ_PEN), sizeof(lp), &lp);
   #endif /* _MGL32_WIN__ */
#endif /* __WIN32__ */
   // printf("usx: %lf usy: %lf\n", UserScaleX, UserScaleY);
   // printf("sx: %lf sy: %lf\n", ScaleX, ScaleY);
   for(;;){
      X = *Ch++;
      Y = *Ch++;

      if(X.op == 0)
         break;

      Mode = Y.op ? DRAW : MOVE;

      if(direction == HORIZ_DIR){
         x = (short int)X.coo;
         y = (short int)-Y.coo;
      }/* if */

      if(direction == VERT_DIR){
         x = (short int)-Y.coo;
         y = (short int)-X.coo;
      }/* if */

      // continue with float point

      if((UserScaleX != 1.) || (ScaleX != 1.)){
         xx = (ScaleX * UserScaleX * (double)x);
      }/* if */
      else{
         xx = (double)x;
      }/* elif */

      if((UserScaleY != 1.) || (ScaleY != 1.)){
         yy = (ScaleY * UserScaleY * (double)y);
      }/* if */
      else{
         yy = (double)y;
      }/* elif */

      ax = xx;
      ay = yy;

      xx += cX;
      yy += cY;

      // rotate x, y
      if(Angle != 0.0){
         rotate_point(Angle, &xx, &yy, rX, rY);
      }/* if */

      if(Mode){
#ifndef __WIN32__
      lineto(xx, yy);
#else /* __WIN32__ */
      //LineTo(hdc, xx, yy);
      //SetPixel(hdc, xx, yy, lp.lopnColor);
      Line(hdc, stx, sty, xx, yy);
#endif /* __WIN32__ */
      }/* if */
      else
#ifndef __WIN32__
      moveto(xx, yy);
#else /* __WIN32__ */
      MoveToEx(hdc, xx, yy, NULL);
      stx = xx;
      sty = yy;
#endif /* __WIN32__ */
   }// for
   cX += ax;
   cY += ay;
}// DrawChar
//---------------------------------------------------------------------------
SIZESYMBOL CalculateSizeSymbol(unsigned char c){
   SIZESYMBOL s = {0, 0};
   register short int x, y;
   DECODE  X, Y;
   DECODE* Ch;
   if((Ch = SymFind(c)) == NULL)
      return s;
   for(;;){
      X = *Ch++;
      Y = *Ch++;
      if(X.op == 0)
         break;
      x = (short int)X.coo;
      y = (short int)Y.coo;
      if(x > s.x)
         s.x = (short int)x;
      if(y > s.y)
         s.y = (short int)y;
   }// for
   return s;
}/* CalculateSizeSymbol */
//---------------------------------------------------------------------------
static DECODE *SymFind(unsigned char c){
   LPBGIFONT lpbf = bf[cf];
   short int Ofs;
   unsigned char first = lpbf->pIFnt->firstASCII;
   if(c < first){
      return NULL;
   }/* if */
   Ofs = lpbf->pOffsSD[c - first];
   return (DECODE*)((char*)lpbf->pIFnt + lpbf->pIFnt->OffsetStrDef + Ofs);
}/* SymFind */
//---------------------------------------------------------------------------
static void CalcJustify(const char *s, short int *x, short int *y){
   short int th = 0, tw = 0;
   short int xx = *x, yy = *y;

   switch(direction){
      case HORIZ_DIR:
         th = TextHeight((unsigned char*)s);
         tw = TextWidth((unsigned char*)s);
         break;
      case VERT_DIR:{/* lockblock */
         LPIBGIFONT lpibf = bf[cf]->pIFnt;
         xx += CCalcY(lpibf->DisttoTopCap
                      + lpibf->DisttoBaseline
                      + (-(lpibf->DisttoBottomDescender)));
         tw = TextHeight((unsigned char*)s);
         th = TextWidth((unsigned char*)s);
         }/* lockblock */
         break;
   }/* switch */
   switch(TextVerJustify){
      case TOP_TEXT:
         yy += th;
         break;
      case CENTER_TEXT:
         yy = (short int)(yy + (th / 2));
         break;
   }/* switch */
   switch(TextHorJustify){
      case CENTER_TEXT:
         xx = (short int)(xx - (tw / 2));
         break;
      case RIGHT_TEXT:
         xx -= tw;
         break;
   }/* switch */

   if(Angle != 0.0){
      double dx = xx, dy = yy;
      rotate_point(-Angle, &dx, &dy, rX, rY);
      xx = dx;
      yy = dy;
   }/* if */

   *x = xx;
   *y = yy;
   return;
}/* CalcJustify */
//---------------------------------------------------------------------------
void OutText(const unsigned char *textstring){
   unsigned char *c = (unsigned char*)textstring;
#ifdef __WIN32__
   HDC hdc;
#endif
   if(bf[cf] == NULL)
      return;
#ifdef __WIN32__
   hdc = bf[cf]->hdc;
#endif
   rX = cX;
   rY = cY;
   CalcJustify((char*)c, &cX, &cY);
   while(*c)
#ifndef __WIN32__
   #ifdef _MGL16_DOS__
   #else /* _MGL16_DOS__ */
   #endif /* _MGL16_DOS__ */
      DrawChar((DECODE*)SymFind(*c++));
#else /* __WIN32__ */
   #ifdef _MGL32_WIN__
   #else /* _MGL32_WIN__ */
      DrawChar(hdc, (DECODE*)SymFind(*c++));
   #endif /* _MGL32_WIN__ */
#endif /* __WIN32__ */
}/* OutText */
//---------------------------------------------------------------------------
void OutTextXY(int x, int y, const unsigned char *textstring){
   if(bf[cf] == NULL)
      return;
   cX = (short int)x;
   cY = (short int)y;
#ifndef __WIN32__
   #ifdef _MGL16_DOS__
   MGL_moveToCoord(cX, cY);
   #else /* _MGL16_DOS__ */
   moveto(cX, cY);
   #endif /* _MGL16_DOS__ */
#else /* __WIN32__ */
   #ifdef _MGL32_WIN__
   #else /* _MGL32_WIN__ */
   MoveToEx(bf[cf]->hdc, cX, cY, NULL);
   #endif /* _MGL32_WIN__ */
#endif /* __WIN32__ */
   OutText(textstring);
}/* OutTextXY */
//---------------------------------------------------------------------------
#pragma argsused
short int TextHeight(const unsigned char *textstring){
   LPBGIFONT lpbf;
   short int H = 0;
   if((lpbf = bf[cf]) == NULL)
      return H;
   H = lpbf->pIFnt->DisttoTopCap;
   H += lpbf->pIFnt->DisttoBaseline;
   H += (short int) -(lpbf->pIFnt->DisttoBottomDescender);
   if(direction == HORIZ_DIR)
      H = CCalcY(H);
   if(direction == VERT_DIR)
      H = CCalcX(H);
   return H;
}/* TextHeight */
//---------------------------------------------------------------------------
short int TextWidth(const unsigned char *textstring){
   LPBGIFONT lpbf;
   unsigned char *c = (unsigned char*)textstring;
   unsigned short int W = 0;
   unsigned char first;
   if((lpbf = bf[cf]) == NULL)
      return W;
   first = lpbf->pIFnt->firstASCII;
   while(*c){
      W += lpbf->pWidth[(*c) - first];
      c++;
   }/* while */
   if(direction == HORIZ_DIR)
      W = CCalcX(W);
   if(direction == VERT_DIR)
      W = CCalcY(W);
   return W;
}/* TextWidth */
//---------------------------------------------------------------------------
void SetUserCharSize(short int multx, short int divx, short int multy, short int divy){
   if(divx != 0)
      UserScaleX = ((double)multx) / ((double)divx);
   if(divy != 0)
      UserScaleY = ((double)multy) / ((double)divy);
}/* SetUserCharSize */
//---------------------------------------------------------------------------
#ifndef __WIN32__
   #ifdef _MGL16_DOS__
   #else /* _MGL16_DOS__ */
   #endif /* _MGL16_DOS__ */
#else /* __WIN32__ */
   #ifdef _MGL32_WIN__
   #else /* _MGL32_WIN__ */
HDC SetHDCFont(HDC hdc){
   HDC ohdc = NULL;
   if(bf[cf]){
      ohdc = bf[cf]->hdc;
      bf[cf]->hdc = hdc;
   }/* if */
   return ohdc;
}/* SetHDCFont */
   #endif /* _MGL32_WIN__ */
#endif /* __WIN32__ */
//---------------------------------------------------------------------------
void SetAngle(double a){
   Angle = a;
}/* SetAngle */
//---------------------------------------------------------------------------
