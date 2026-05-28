/* TBG_CONVERTS.CPP
*
*   TBG v7.0
*   27-09-2001 8:33 am msk
*
*/
#include <math.h>
#include <string.h>
#include "tbg.hpp"
#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#endif
#include "tbg_tc.h"
#pragma hdrstop

TColor EGAColors[16] = {
   (TColor)RGB(0x00,0x00,0x00), //  0 Black
   (TColor)RGB(0x00,0x00,0xAA), //  1 Blue
   (TColor)RGB(0x00,0xAA,0x00), //  2 Green
   (TColor)RGB(0x00,0xAA,0xAA), //  3 Cyan
   (TColor)RGB(0xAA,0x00,0x00), //  4 Red
   (TColor)RGB(0xFF,0xAA,0x00), //  5 Gold
   (TColor)RGB(0xAA,0xAA,0x5F), //  6 Boloto
   (TColor)RGB(0xAA,0xAA,0xAA), //  7 LightGray
   (TColor)RGB(0x5A,0x5A,0x5A), //  8 DarkGray
   (TColor)RGB(0x00,0x00,0xFF), //  9 LightBlue
   (TColor)RGB(0x00,0xFF,0x00), // 10 LightGreen
   (TColor)RGB(0x00,0xFF,0xFF), // 11 LightCyan
   (TColor)RGB(0xFF,0x00,0x00), // 12 LightRed
   (TColor)RGB(0xFF,0xAA,0xAA), // 13 Pastel
   (TColor)RGB(0xFF,0xFF,0x00), // 14 Yellow
   (TColor)RGB(0xFF,0xFF,0xFF)  // 15 White
};

// бедность стандартных стилей заливки Windows просто поражает!
TBrushStyle EGAFillStyle[] = {
   bsClear,       // EMPTY_FILL
   bsSolid,       // SOLID_FILL
   bsHorizontal,  // LINE_FILL
   bsBDiagonal,   // LTSLASH_FILL
   bsBDiagonal,   // SLASH_FILL
   bsFDiagonal,   // BKSLASH_FILL
   bsFDiagonal,   // LTBKSLASH_FILL
   bsDiagCross,	// XHATCH_FILL
   bsDiagCross,	// INTERLEAVE_FILL
   bsDiagCross,	// CLOSE_DOT_FILL
   bsCross,       // HATCH_FILL
   bsCross        // WIDE_DOT_FILL
};


// - Конвертация старых картинок TBG ------------------------------------------
TBaseGP* __fastcall convert_pn(OBJECT **obj,short int pn);

// ----------------------------------------------------------------------------
int __fastcall XCompanenta(int radius,int fi)/* ЄR¬Ї -_-в  г_<  fi */
{
 double x;
 double fit=fi;

 x=cos(fit/RAD)*radius;
 if(x >= 0) x+=0.5;
   else     x-=0.5;

 return x;
}/* XCompanenta */
/*-------------------------------------------------------------------------*/
int __fastcall YCompanenta(int radius,int fi)/* ЄR¬Ї -_-в  г_<  fi */
{
 double y;
 double fit=fi;

 y=sin(fit/RAD)*radius;
 if(y >= 0) y+=0.5;
   else     y-=0.5;

 return y;
}/* YCompanenta */
/*-------------------------------------------------------------------------*/
double __fastcall XCompanenta(double r, double pfi){
   return cos(pfi / RAD) * r;
};
/*-------------------------------------------------------------------------*/
double __fastcall YCompanenta(double r, double pfi){
   return sin(pfi / RAD) * r;
};
/*-------------------------------------------------------------------------*/
double __fastcall Angl(TPointf ps, TPointf pe){
   double x1, y1, x2, y2, a = 0.0;
   x1 = ps.getx();
   y1 = ps.gety();
   x2 = pe.getx();
   y2 = pe.gety();
   if((x2 - x1)){
     a = atan((y2 - y1)/(x2 - x1)) * RAD;
     if((x2 - x1) < 0.0){
        a += 180.0;
     }/* if */
   }/* if */
   return a;
};
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void __fastcall convert_le(OBJECT **OldPic, short int i, TBaseGP *gp){
   int le;
   unsigned char sNo, sNe;

   struct HTBG *OldPicHeader = (struct HTBG*)*(OldPic);
   for(le = 0;le <= OldPicHeader->Num_LE;le++){
      if(OldPicHeader->Logic[le].Index_ei == i){
         sNo = OldPicHeader->Logic[le].numobj;
         sNe = OldPicHeader->Logic[le].num;
         gp->InsertLE(sNo, sNe);
      }/* if */
   }/* for */
}/* ConvertLE */
// ----------------------------------------------------------------------------
void __fastcall Convert(TPictureTBG *NewPic, char *OldName){

   OBJECT **OldPic;

   OldPic = NULL;
   //LoadMP = 0;
   load_file(&OldPic, OldName);
   if(OldPic){
      DeleteEGATools();
      CreateEGATools();
      struct HTBG *OldPicHeader = (struct HTBG*)*(OldPic);
      for(short int i = 1;i <= OldPicHeader->count_ei;i++){
         TBaseGP *gp = convert_pn(OldPic, i);
         if(gp){
            NewPic->InsertGP(gp);
            convert_le(OldPic, i, gp);
         }/* if */
      }/* for */
      del_pic(&OldPic);
      OldPic = NULL;
   }// if
   //LoadMP = 1;
}/* Convert */
// ----------------------------------------------------------------------------
double __fastcall DmXY(int xy){
   return (((double)xy) / (double)SIZE_XY);
}// DmXY
// ----------------------------------------------------------------------------
TBaseGP* __fastcall convert_pn(OBJECT **obj,short int pn){

 int x,y,dx,dy,fi1,fi2;

 short int fnt;
 short int dir;
 short int siz;
 double mw = 1.0;
 int mxn,myn;

 int ofsx = 0;
 int ofsy = 0;

 if(pn <= 0)
 {
   error_tbg = ER_INDEX_0;
   return NULL;
 }// if

 if(pn > ((struct HTBG*)*(obj))->count_ei)
 {
   error_tbg = ER_INDEX_MAX;
   return NULL;
 }// if

 switch(*((char*)*(obj+pn)))
 {

      case OBJ_line:
         {// lockblock
            struct ln *ptr_ln=((struct ln*)*(obj+pn));
            x =ptr_ln->lnx;
            y =ptr_ln->lny;
            dx=ptr_ln->lndx;
            dy=ptr_ln->lndy;
            return new TLine(
                 DmXY(x), DmXY(y),
                 DmXY(x + dx), DmXY(y + dy),
                 EGAColors[ptr_ln->lncol], ptr_ln->lnthi);
         }// lockblock
      case OBJ_rect:
         {// lockblock
            struct rc *ptr_rc=((struct rc*)*(obj+pn));
            x =ptr_rc->rcx;
            y =ptr_rc->rcy;
            dx=ptr_rc->rcdx;
            dy=ptr_rc->rcdy;
            if(ptr_rc->rcstl == GABARIT_LINE){
               TFillPolyRect *gp =  new TFillPolyRect(
                    DmXY(x), DmXY(y),
                    DmXY(x + dx), DmXY(y + dy),
                    EGAColors[8],
                    EGAColors[8],
                    (TBrushStyle)EGAFillStyle[XHATCH_FILL], 1);
                    gp->SetHideState(true);
                    return gp;
            }// if
            else{
               TPolyRect *gp =  new TPolyRect(
                    DmXY(x), DmXY(y),
                    DmXY(x + dx), DmXY(y + dy),
                    EGAColors[ptr_rc->rccol],
                    ptr_rc->rcthi);
                    return gp;
            }// elif
         }// lockblock
      case OBJ_bar:
         {// lockblock
            struct br *ptr_br=((struct br*)*(obj+pn));
            x =ptr_br->brx;
            y =ptr_br->bry;
            dx=ptr_br->brdx;
            dy=ptr_br->brdy;
            return new TFillPolyRect(
                    DmXY(x), DmXY(y),
                    DmXY(x + dx), DmXY(y + dy),
                    EGAColors[ptr_br->brcol],
                    EGAColors[ptr_br->brcol],
                    (TBrushStyle)EGAFillStyle[ptr_br->brstl], 1);
         }// lockblock
      case OBJ_elip:
         {// lockblock
            struct el *ptr_el=((struct el*)*(obj+pn));
            x =ptr_el->elx;
            y =ptr_el->ely;
            dx =ptr_el->eldx;
            dy =ptr_el->eldy;
            fi1=ptr_el->elfi1;
            fi2=ptr_el->elfi2;
            TPointf p[4];
            p[0].moveto(DmXY(x - dx), DmXY(y - dy));
            p[1].moveto(DmXY(x + dx), DmXY(y + dy));
            p[2].moveto(DmXY(x + XCompanenta(dx, fi1)),
                        DmXY(y - YCompanenta(dy, fi1)));
            p[3].moveto(DmXY(x + XCompanenta(dx, fi2)),
                        DmXY(y - YCompanenta(dy, fi2)));
            return new TArc(p, EGAColors[ptr_el->elcol], ptr_el->elthi);
         }// lockblock
      case OBJ_felip:
         {// lockblock
            struct fel *ptr_fel=((struct fel*)*(obj+pn));
            x =ptr_fel->felx;
            y =ptr_fel->fely;
            dx =ptr_fel->feldx;
            dy =ptr_fel->feldy;
            fi1=ptr_fel->felfi1;
            fi2=ptr_fel->felfi2;

            TPointf p[4];
            p[0].moveto(DmXY(x - dx), DmXY(y - dy));
            p[1].moveto(DmXY(x + dx), DmXY(y + dy));
            p[2].moveto(DmXY(x + XCompanenta(dx, fi1)),
                        DmXY(y - YCompanenta(dy, fi1)));
            p[3].moveto(DmXY(x + XCompanenta(dx, fi2)),
                        DmXY(y - YCompanenta(dy, fi2)));
            return new TFillArc(p, EGAColors[ptr_fel->felcol], EGAColors[ptr_fel->felfcol], (TBrushStyle)EGAFillStyle[ptr_fel->felfstl], ptr_fel->felthi);
            /*
            SelectObject(hdc,
            cEGA[ptr_fel->felcol].hPen[(ptr_fel->felthi == THICK_WIDTH) ? USERBIT_LINE : ptr_fel->felstl]);
            SelectObject(hdc, cEGA[ptr_fel->felfcol].hBrush[ptr_fel->felfstl]);
            if((fi1 == 0) && (fi2 == 360)){
               Ellipse(hdc,DMX((long)x - (long)dx),
                       DMY((long)y - (long)dy),
                       DMX((long)x + (long)dx),
                       DMY((long)y + (long)dy));
               Pie(hdc,DMX((long)x - (long)dx),
                       DMY((long)y - (long)dy),
                       DMX((long)x + (long)dx),
                       DMY((long)y + (long)dy),
                       DMX((long)x + (long)XCompanenta(dx, fi1)),
                       DMY((long)y - (long)YCompanenta(dy, fi1)),
                       DMX((long)x + (long)XCompanenta(dx, fi2)),
                       DMY((long)y - (long)YCompanenta(dy, fi2)));
              */
         }// // lockblock
      case OBJ_txt:
         {// lockblock
         struct tx *ptr_tx=((struct tx*)*(obj+pn));
         TBGIText *t = new TBGIText(DmXY(ptr_tx->txx), DmXY(ptr_tx->txy), EGAColors[ptr_tx->txcol]);
         if(t){
            t->SetFont(ptr_tx->txfnt);
            t->SetDirection(ptr_tx->txdir);
            t->SetJustify(ptr_tx->txhor, ptr_tx->txver);
            t->SetSize(0);
            t->SetExt(ptr_tx->txsda, ptr_tx->mltx, ptr_tx->mlty);
            t->SetSS(640, 480);
            t->SetAngle(0.0);
            t->SetText(ptr_tx->txt);
         }/* if */
         return t;
         }// lockblock
         case OBJ_fld:
         case OBJ_bkgr:
            break;
         case OBJ_esc:
            {// lockblock
               MP *mp = ((MP*)*(obj+pn));
               if(mp == NULL)
                  break;
               //if(mp->cl)
               //   mp->cl->hdc = hdc;
               x = mp->mcx;
               y = mp->mcy;
              // MPCDraw(mp->cl, x, y, mp->cs, mp->ud, mp);
              return new TMacro((unsigned)(mp->index), (unsigned)(mp->cs), TPointf(DmXY(x), DmXY(y)));
            }// lockblock
         case OBJ_reper:{
               struct reper *r = ((struct reper*)*(obj+pn));
               x = r->x;
               y = r->y;
               return new TReperPoint(&TPointf(DmXY(x), DmXY(y)));
         	}
         case OBJ_pipeline:
            {// lockblock
            struct pln *ptr_pln=((struct pln*)*(obj+pn));
            x =ptr_pln->lnx;
            y =ptr_pln->lny;
            dx=ptr_pln->lndx;
            dy=ptr_pln->lndy;
            return new TPipeLine(DmXY(x), DmXY(y),
                                 DmXY(x + dx), DmXY(y + dy),
                                 EGAColors[ptr_pln->pl.C1],
                                 EGAColors[ptr_pln->pl.C2],
                                 ptr_pln->pl.DO,
                                 ptr_pln->pl.O,
                                 ptr_pln->pl.S,
                                 ptr_pln->lnthi);

            }// lockblock
         default:
         error_tbg = ER_BAD_TYPE_GP;
         break;
 }// switch

 return NULL;
}// convert_pn
/*
// -----------------------------------------------------------------------
short int ABSx(short int crd){
   return (short int)(0.5+((crd/(double)SX_vp)*SIZE_XY));
}// ABSx
// -----------------------------------------------------------------------
short int ABSy(short int crd){
   return (short int)(0.5+((crd/(double)SY_vp)*SIZE_XY));
}// ABSy
//--------------------------------------------------------------------------
static char *sep = "\x0E";
void OutTextCol(int x, int y, char *txt){
   char *dup, *t;

#ifdef __WIN32__
   struct TextSettingsType ti;
   GetTextSettings(&ti);
#else
   struct textsettingstype ti;
   gettextsettings(&ti);
#endif

   switch(ti.horiz){
      case CENTER_TEXT:
         x -= TextWidthCol(txt) / 2;
         break;
      case RIGHT_TEXT:
         x -= TextWidthCol(txt);
         break;
   }// switch 

   switch(ti.vert){
      case CENTER_TEXT:
         y -= TextHeightCol(txt) / 2;
         break;
      case BOTTOM_TEXT:
         y -= TextHeightCol(txt);
         break;
   }// switch 

#ifdef __WIN32__
   SetTextJustify(LEFT_TEXT, TOP_TEXT);
#else
   settextjustify(LEFT_TEXT, TOP_TEXT);
#endif
   t = dup = strdup(txt);
   strtok(dup, sep);
#ifdef __WIN32__
//   MoveToEx(ti.hdc, x, y, NULL);
   OutTextXY(x, y, (unsigned char *)t);
#else
   moveto(x, y);
   outtext(t);
#endif
   while((t = strtok(NULL, sep)) != NULL){
      if(ti.direction == HORIZ_DIR){
#ifdef __WIN32__
//         MoveToEx(ti.hdc, x, y += TextHeight((unsigned char *)t) + 3, NULL);
         y += TextHeight((unsigned char *)t) + 3;
#else
         moveto(x, y += textheight(t) + 3);
#endif
      }// if 
      else{
#ifdef __WIN32__
//         MoveToEx(ti.hdc, x += TextHeight((unsigned char *)t) + 3, y, NULL);
         x += TextHeight((unsigned char *)t) + 3;
#else
         moveto(x += textheight(t) + 3, y);
#endif
      }// elif 
#ifdef __WIN32__
//      OutText((unsigned char *)t);
      OutTextXY(x, y, (unsigned char *)t);
#else
      outtext(t);
#endif
   }// while 
   free(dup);

}// OutTextCol 
// ----------------------------------------------------------------------- 
int TextWidthCol (char *txt){
   char *dup, *t;
   int smx = 0;

   // ўлзЁб<Ёвм ¤<Ё-л бваRЄ ўлЎа вм ¬ ЄбЁ¬ <м-го
   t = dup = strdup(txt);
   strtok(dup, sep);
#ifdef __WIN32__
   if(TextWidth((unsigned char *)t) >  smx){
#else
   if(textwidth(t) >  smx){
#endif
#ifdef __WIN32__
      smx = TextWidth((unsigned char *)t);
#else
      smx = textwidth(t);
#endif
   }// if 
   while((t = strtok(NULL, sep)) != NULL){
#ifdef __WIN32__
      if(TextWidth((unsigned char *)t) >  smx){
#else
      if(textwidth(t) >  smx){
#endif
#ifdef __WIN32__
         smx = TextWidth((unsigned char *)t);
#else
         smx = textwidth(t);
#endif
      }// if 
   }// while 
   free(dup);
   return smx;
}// TextWidthCol 
// ----------------------------------------------------------------------- 
int TextHeightCol (char *txt){
   char *dup, *t;
   int smy = 0;

   // ўлзЁб<Ёвм ўлбRвл бваRЄ Ё б<R¦Ёвм Ёе ўб_
   t = dup = strdup(txt);
   strtok(dup, sep);
#ifdef __WIN32__
   if(TextHeight((unsigned char *)t) >  smy){
#else
   if(textheight(t) >  smy){
#endif
#ifdef __WIN32__
      smy += TextHeight((unsigned char *)t);
#else
      smy += textheight(t);
#endif
   }// if 
   while((t = strtok(NULL, sep)) != NULL){
#ifdef __WIN32__
      smy += TextHeight((unsigned char *)t) + 3;
#else
      smy += textheight(t) + 3;
#endif
   }// while 
   free(dup);
   return smy;
}// TextHeightCol 
*/


