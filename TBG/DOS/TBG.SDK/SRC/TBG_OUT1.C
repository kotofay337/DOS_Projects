/* tbg_out1.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include <math.h>
#include <string.h>
#include <alloc.h>
#ifndef __WIN32__
#include <graphics.h>
#endif
#include "tbg_tc.h" /* обьявления функций и данных */
/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/*--------------------------------------------------------------------------*/
#ifdef __WIN32__
/* ----------------------------------------------------------------------- */
static const double rad=57.295780;
/*-------------------------------------------------------------------------*/
int XCompanenta(int radius,int fi)/* Є®¬Ї ­Ґ­в  гЈ«  fi */
{
 double x;
 double fit=fi;

 x=cos(fit/rad)*radius;
 if(x >= 0) x+=0.5;
   else     x-=0.5;

 return x;
}/* XCompanenta */
/*-------------------------------------------------------------------------*/
int YCompanenta(int radius,int fi)/* Є®¬Ї ­Ґ­в  гЈ«  fi */
{
 double y;
 double fit=fi;

 y=sin(fit/rad)*radius;
 if(y >= 0) y+=0.5;
   else     y-=0.5;

 return y;
}/* YCompanenta */
/*-------------------------------------------------------------------------*/
#endif
short int outpic_pn(OBJECT **obj,short int pn)
{

 int x,y,dx,dy,fi1,fi2;

 short int fnt;
 short int dir;
 short int siz;
 double mw = 1.0;
 int mxn,myn;

 int ofsx;
 int ofsy;

#ifdef __WIN32__
 HDC hdc = ((struct HTBG*)*(obj))->hdc;
#endif

 if(pn <= 0)
 {
   error_tbg = ER_INDEX_0;
   return error_tbg;
 }/* if */

 if(pn > ((struct HTBG*)*(obj))->count_ei)
 {
   error_tbg = ER_INDEX_MAX;
   return error_tbg;
 }/* if */

 /* Устанавливаем смещение от начала координат */
 ofsx = ((struct HTBG*)*(obj))->ofsx;
 ofsy = ((struct HTBG*)*(obj))->ofsy;

#ifndef __WIN32__
 settextjustify(0,0);/* обязательная установка типа ориентации текста */
#else
 SetTextJustify(0,0);/* обязательная установка типа ориентации текста */
#endif

 Show_tbg=ON; // флаг занятости TBG выводом на экран =занято=

 switch(*((char*)*(obj+pn)))
 {

      case OBJ_line:
         {// lockblock
         struct ln *ptr_ln=((struct ln*)*(obj+pn));
         x =ptr_ln->lnx;
         y =ptr_ln->lny;
         dx=ptr_ln->lndx;
         dy=ptr_ln->lndy;
         ptr_ln->col=ptr_ln->lncol;
         x += ofsx;
         y += ofsy;
#ifndef __WIN32__
   #ifdef __MGL16_DOS__
         MGL_beginDrawing();
         MGL_setColor(ptr_ln->lncol);
         MGL_setBackColor(ptr_ln->lncol);


         MGL_setPenStyle(MGL_BITMAP_SOLID);
         MGL_lineCoord(	DMX((long)x),
                        DMY((long)y),
                        DMX((long)x+(long)dx),
                        DMY((long)y+(long)dy));
         MGL_endDrawing();
   #else /* __MGL16_DOS__ */
         setlinestyle(ptr_ln->lnstl,0,
                      ptr_ln->lnthi);
         setcolor(ptr_ln->lncol);
         line(DMX((long)x),
              DMY((long)y),
              DMX((long)x+(long)dx),
              DMY((long)y+(long)dy));
   #endif /* __MGL16_DOS__ */
#else /* __WIN32__ */
   #ifdef __MGL32_WIN__

   #else  /* __MGL32_WIN__ */
         SelectObject(hdc,
          cEGA[ptr_ln->lncol].hPen[(ptr_ln->lnthi == THICK_WIDTH) ? USERBIT_LINE : ptr_ln->lnstl]);
         Line(hdc, DMX((long)x),
              DMY((long)y),
              DMX((long)x+(long)dx),
              DMY((long)y+(long)dy));
   #endif /* __MGL32_WIN__ */
#endif /* __WIN32__ */
         }// lockblock
         break;

      case OBJ_rect:
         {// lockblock
            struct rc *ptr_rc=((struct rc*)*(obj+pn));
            if(ptr_rc->rcstl != GABARIT_LINE){
	            x =ptr_rc->rcx;
               y =ptr_rc->rcy;
               dx=ptr_rc->rcdx;
               dy=ptr_rc->rcdy;
               x += ofsx;
               y += ofsy;
               ptr_rc->col=ptr_rc->rccol;
#ifndef __WIN32__
   #ifdef __MGL16_DOS__
               {/* lb */
                  short int top=DMY((long)y)
                           ,bottom=DMY((long)y+(long)dy)
                           ,left=DMX((long)x)
                           ,right=DMX((long)x+(long)dx);


                  if(left>right)SWAP(left,right);
                  if(top>bottom)SWAP(top,bottom);

                  MGL_beginDrawing();
                  MGL_setColor(ptr_rc->rccol);
                  MGL_setPenStyle(MGL_BITMAP_SOLID);
                  MGL_rectCoord(left,top,right,bottom);
                  MGL_endDrawing();
               }/* lb */
   #else /* __MGL16_DOS__ */
               setlinestyle(ptr_rc->rcstl,0,
                            ptr_rc->rcthi);
               ptr_rc->col=ptr_rc->rccol;
               setcolor(ptr_rc->rccol);
               rectangle(DMX((long)x),
                         DMY((long)y),
                         DMX((long)x+(long)dx),
                         DMY((long)y+(long)dy));
   #endif /* __MGL16_DOS__ */
#else
   #ifdef __MGL32_WIN__
   #else  /* __MGL32_WIN__ */
               SelectObject(hdc, cEGA[ptr_rc->rccol].hPen[(ptr_rc->rcthi == THICK_WIDTH) ?
                                 USERBIT_LINE :
                                 ptr_rc->rcstl]);
               Rect(hdc, DMX((long)x),
                         DMY((long)y),
                         DMX((long)x+(long)dx),
                         DMY((long)y+(long)dy));
   #endif /* __MGL32_WIN__ */
#endif
            }/* if */
         }// lockblock
         break;

      case OBJ_bar:
         {// lockblock
         struct br *ptr_br=((struct br*)*(obj+pn));
         x =ptr_br->brx;
         y =ptr_br->bry;
         dx=ptr_br->brdx;
         dy=ptr_br->brdy;
         ptr_br->col=ptr_br->brcol;
         x += ofsx;
         y += ofsy;
#ifndef __WIN32__
   #ifdef __MGL16_DOS__
         { /* lb */
         short int top=DMY((long)y)
                  ,bottom=DMY((long)y+(long)dy)
                  ,left=DMX((long)x)
                  ,right=DMX((long)x+(long)dx);


            if(left>right)SWAP(left,right);
            if(top>bottom)SWAP(top,bottom);

         MGL_beginDrawing();
         MGL_setColor(ptr_br->brcol);
         MGL_setPenStyle(MGL_BITMAP_SOLID);
         MGL_fillRectCoord(left,top,right,bottom);
         MGL_endDrawing();
         } /* lb */
   #else /* __MGL16_DOS__ */
         setfillstyle(ptr_br->brstl,
                      ptr_br->brcol);
	      disable();
         bar(DMX((long)x),
             DMY((long)y),
             DMX((long)x+(long)dx),
             DMY((long)y+(long)dy));
         enable();
   #endif  /* __MGL16_DOS__ */
#else
#ifdef __MGL32_WIN__
#else  /* __MGL32_WIN__ */
         SelectObject(hdc, cEGA[ptr_br->brcol].hBrush[ptr_br->brstl]);
         Bar(hdc, DMX((long)x),
                  DMY((long)y),
                  DMX((long)x+(long)dx),
                  DMY((long)y+(long)dy));
#endif /* __MGL32_WIN__ */
#endif
         }// lockblock
         break;

      case OBJ_elip:
         {// lockblock
         struct el *ptr_el=((struct el*)*(obj+pn));
         x =ptr_el->elx;
         y =ptr_el->ely;
         dx =ptr_el->eldx;
         dy =ptr_el->eldy;
         fi1=ptr_el->elfi1;
         fi2=ptr_el->elfi2;
         ptr_el->col=ptr_el->elcol;
         x += ofsx;
         y += ofsy;
#ifndef __WIN32__
   #ifdef __MGL16_DOS__
         {/* lb */
         short int top=DMY((long)y)
               ,bottom=DMY((long)y+(long)dy)
               ,left=DMX((long)x)
               ,right=DMX((long)x+(long)dx)
               ,radX,radY
               ;


         if(left>right)SWAP(left,right);
         if(top>bottom)SWAP(top,bottom);

         radX=(right-left);
         radY=(bottom-top);
         MGL_beginDrawing();
         MGL_setColor(ptr_el->elcol);
         MGL_setPenStyle(MGL_BITMAP_SOLID);
         MGL_ellipseArcCoord(left,top,radX,radY,fi1,fi2);
         MGL_endDrawing();
         }/* lb */
   #else /* __MGL16_DOS__ */
         setlinestyle(ptr_el->elstl,0,
                      ptr_el->elthi);
         setcolor(ptr_el->elcol);
         ellipse(DMX((long)x),
		           DMY((long)y),
                 fi1,fi2,
                 DMX((long)dx),
                 DMY((long)dy));
   #endif  /* __MGL16_DOS__ */
#else
#ifdef __MGL32_WIN__
#else  /* __MGL32_WIN__ */
         SelectObject(hdc,
          cEGA[ptr_el->elcol].hPen[(ptr_el->elthi == THICK_WIDTH) ? USERBIT_LINE : ptr_el->elstl]);
         Arc(hdc,DMX((long)x - (long)dx),
                 DMY((long)y - (long)dy),
                 DMX((long)x + (long)dx),
                 DMY((long)y + (long)dy),
                 DMX((long)x + (long)XCompanenta(dx, fi1)),
                 DMY((long)y - (long)YCompanenta(dy, fi1)),
                 DMX((long)x + (long)XCompanenta(dx, fi2)),
                 DMY((long)y - (long)YCompanenta(dy, fi2)));
#endif /* __MGL32_WIN__ */
#endif
         }// // lockblock
         break;

      case OBJ_felip:
         {// lockblock
         struct fel *ptr_fel=((struct fel*)*(obj+pn));
         x =ptr_fel->felx;
         y =ptr_fel->fely;
         dx =ptr_fel->feldx;
         dy =ptr_fel->feldy;
         fi1=ptr_fel->felfi1;
         fi2=ptr_fel->felfi2;
         ptr_fel->col=ptr_fel->felcol;
         x += ofsx;
         y += ofsy;
#ifndef __WIN32__
   #ifdef __MGL16_DOS__
         {/* lb */
            short int top = DMY((long)y)
            ,bottom = DMY((long)y+(long)dy)
            ,left = DMX((long)x)
            ,right = DMX((long)x+(long)dx)
            ,radX, radY;


            if(left>right)SWAP(left,right);
            if(top>bottom)SWAP(top,bottom);

            radX=(right-left);
            radY=(bottom-top);
            MGL_beginDrawing();
            MGL_setColor(ptr_fel->felcol);
            MGL_setPenStyle(MGL_BITMAP_SOLID);
            MGL_fillEllipseArcCoord(left,top,radX,radY,fi1,fi2);
            MGL_endDrawing();
         }/* lb */
   #else /* __MGL16_DOS__ */
         setlinestyle(ptr_fel->felstl,0,
                      ptr_fel->felthi);
         setcolor(ptr_fel->felcol);
         setfillstyle(ptr_fel->felfstl, ptr_fel->felfcol);
         if((fi1 == 0) && (fi2 == 360)){
            fillellipse(DMX((long)x),
		              DMY((long)y),
                    DMX((long)dx),
                    DMY((long)dy));
         }/* if */
         else{
            sector(DMX((long)x),
		              DMY((long)y),
                    fi1,fi2,
                    DMX((long)dx),
                    DMY((long)dy));
         }/* elif */
   #endif  /* __MGL16_DOS__ */
#else
#ifdef __MGL32_WIN__
#else  /* __MGL32_WIN__ */
         SelectObject(hdc,
         cEGA[ptr_fel->felcol].hPen[(ptr_fel->felthi == THICK_WIDTH) ? USERBIT_LINE : ptr_fel->felstl]);
         SelectObject(hdc, cEGA[ptr_fel->felfcol].hBrush[ptr_fel->felfstl]);
         if((fi1 == 0) && (fi2 == 360)){
            Ellipse(hdc,DMX((long)x - (long)dx),
                    DMY((long)y - (long)dy),
                    DMX((long)x + (long)dx),
                    DMY((long)y + (long)dy));
         }/* if */
         else{
            Pie(hdc,DMX((long)x - (long)dx),
                    DMY((long)y - (long)dy),
                    DMX((long)x + (long)dx),
                    DMY((long)y + (long)dy),
                    DMX((long)x + (long)XCompanenta(dx, fi1)),
                    DMY((long)y - (long)YCompanenta(dy, fi1)),
                    DMX((long)x + (long)XCompanenta(dx, fi2)),
                    DMY((long)y - (long)YCompanenta(dy, fi2)));
         }/* elif */
#endif /* __MGL32_WIN__ */
#endif
         }// // lockblock
         break;

      case OBJ_txt:
         {// lockblock
            struct tx *ptr_tx=((struct tx*)*(obj+pn));
            fnt=ptr_tx->txfnt;
            dir=ptr_tx->txdir;
            siz=ptr_tx->txsiz;

       // установка параметров для различных подтипов элемента 'текст'
#ifndef __WIN32__
       switch(((struct HTBG*)*(obj))->tip_txt)
       {
          case 0:
             settextjustify(0,0);
             settextstyle(fnt,dir,siz);
             break;
          case 1:
             settextjustify(ptr_tx->txhor, ptr_tx->txver);
             if(fnt == 0)
           settextstyle(0,dir,siz);
             else{
           settextstyle(fnt,dir,0);
           mw = (double)ptr_tx->txsda / (double)size_xya;
           mxn=(short int)(((double)(ptr_tx->mltx))*mw*
               (SX_vp / (double)getmaxx()) + .5);
           myn=(short int)(((double)(ptr_tx->mlty))*mw*
               (SY_vp / (double)getmaxy()) + .5);

           if(mxn > iMaxX)
              mxn=iMaxX;
           if(myn > iMaxY)
              myn=iMaxY;
           setusercharsize(mxn,10,myn,10);
             }//if
          break;
       }//switch
       ptr_tx->col=ptr_tx->txcol;
       setcolor(ptr_tx->txcol);
       x=ptr_tx->txx;
       y=ptr_tx->txy;
       x += ofsx;
       y += ofsy;
       if(fnt == 0)
          disable();
       //outtextxy(DMX((long)x),DMY((long)y),ptr_tx->txt);
       OutTextCol(DMX((long)x),DMY((long)y),ptr_tx->txt);
       if(fnt == 0)
          enable();
#else
         // установка параметров для различных подтипов элемента 'текст'
         switch(((struct HTBG*)*(obj))->tip_txt)
         {
            case 0:
               SetTextJustify(0,0);
               SetTextStyle(fnt,dir,siz);
               break;
	         case 1:
               SetTextJustify(ptr_tx->txhor,
                              ptr_tx->txver);

               if(fnt == 0)
                  SetTextStyle(0,dir,siz);
               else{
                  SetTextStyle(fnt,dir,0);
                  mw = (double)ptr_tx->txsda / (double)size_xya;
                  mxn=(short int)(((double)(ptr_tx->mltx))*mw*
                      (SX_vp / ((double)((struct HTBG*)*(obj))->SIZEX)) + .5);
                  myn=(short int)(((double)(ptr_tx->mlty))*mw*
                      (SY_vp / ((double)((struct HTBG*)*(obj))->SIZEY)) + .5);

                  if(mxn > ((struct HTBG*)*(obj))->SIZEX - 1)
                     mxn = ((struct HTBG*)*(obj))->SIZEX - 1;
                  if(myn > ((struct HTBG*)*(obj))->SIZEY - 1)
                     myn = ((struct HTBG*)*(obj))->SIZEY - 1;

                  SetUserCharSize(mxn,10,myn,10);

               }/*if */
            break;
	      }//switch
         ptr_tx->col=ptr_tx->txcol;
         x = ptr_tx->txx;
         y = ptr_tx->txy;
         x += ofsx;
         y += ofsy;
         SelectObject(hdc, cEGA[ptr_tx->txcol].hPen[SOLID_LINE]);
         SetHDCFont(hdc);
         OutTextXY(DMX((long)x), DMY((long)y), (unsigned char*)(ptr_tx->txt));
#endif
         }// lockblock
	 break;

      case OBJ_fld:
            {// lockblock
            struct fl *ptr_fl=((struct fl*)*(obj+pn));
            x =ptr_fl->flx;
            y =ptr_fl->fly;
            ptr_fl->col=ptr_fl->flcol;
            x += ofsx;
            y += ofsy;
#ifndef __WIN32__
   #ifdef __MGL16_DOS__
   #else /* __MGL16_DOS__ */
            setfillstyle(ptr_fl->flstl,
                         ptr_fl->flcol);
            setcolor(ptr_fl->flbrd);
            floodfill(DMX((long)x),DMY((long)y),ptr_fl->flbrd);
   #endif  /* __MGL16_DOS__ */
#else
#ifdef __MGL32_WIN__
#else  /* __MGL32_WIN__ */
            SelectObject(hdc, cEGA[ptr_fl->flcol].hBrush[ptr_fl->flstl]);
            FloodFill(hdc, DMX((long)x),DMY((long)y),cEGA[ptr_fl->flbrd].rgb);
#endif /* __MGL32_WIN__ */
#endif
            }// lockblock
	    break;

         case OBJ_bkgr:
            {// lockblock
            struct bk *ptr_bk=((struct bk*)*(obj+pn));
            ptr_bk->col=ptr_bk->bkcol;
#ifndef __WIN32__
   #ifdef __MGL16_DOS__
   #else /* __MGL16_DOS__ */
            setbkcolor(ptr_bk->bkcol);
   #endif  /* __MGL16_DOS__ */
#else
#ifdef __MGL32_WIN__
#else  /* __MGL32_WIN__ */
#endif /* __MGL32_WIN__ */
#endif
            }// lockblock
            break;

         case OBJ_esc:
            {/* lockblock */
               MP *mp = ((MP*)*(obj+pn));
               if(mp == NULL)
                  break;
#ifndef __WIN32__
   #ifdef __MGL16_DOS__
   #else /* __MGL16_DOS__ */
   #endif  /* __MGL16_DOS__ */
#else  /* __WIN32__ */
   #ifdef __MGL32_WIN__
   #else  /* __MGL32_WIN__ */
               if(mp->cl)
                  mp->cl->hdc = hdc;
   #endif /* __MGL32_WIN__ */
#endif
               x = mp->mcx;
               y = mp->mcy;
               x += ofsx;
               y += ofsy;
// printf("\n. in outpic_pn: %d", mp->cl->index);
               MPCDraw(mp->cl, x, y, mp->cs, mp->ud, mp);
// printf("\n. in outpic_pn");

            }/* lockblock */
            break;
         case OBJ_reper:
            break;
         case OBJ_pipeline:
            {// lockblock
            struct pln *ptr_pln=((struct pln*)*(obj+pn));
            x =ptr_pln->lnx;
            y =ptr_pln->lny;
            dx=ptr_pln->lndx;
            dy=ptr_pln->lndy;
            // ptr_pln->col = color;
            x += ofsx;
            y += ofsy;

            ptr_pln->pl.xs = DMX((long)x);
            ptr_pln->pl.ys = DMY((long)y);
            ptr_pln->pl.xe = DMX((long)x + (long)dx);
            ptr_pln->pl.ye = DMY((long)y + (long)dy);
      #ifndef __WIN32__
            setlinestyle(ptr_pln->lnstl, 0, ptr_pln->lnthi);
            pipeline(&(ptr_pln->pl), ptr_pln->color);
      #else
            pipeline(hdc, &(ptr_pln->pl), ptr_pln->color, ptr_pln->lnstl, ptr_pln->lnthi);
      #endif
            }/* lockblock */
            break;
         default:
         error_tbg = ER_BAD_TYPE_GP;
         break;
 }/* switch */

#ifdef __WIN32__
 ((struct HTBG*)*(obj))->NeedUpdateFlag = TRUE; // Необходимо обновление картинки
#endif

 Show_tbg = OFF; // флаг занятости TBG выводом на экран = свободно =

 return error_tbg;
}/* outpic_pn */
/* ----------------------------------------------------------------------- */
short int ABSx(short int crd){
   return (short int)(0.5+((crd/(double)SX_vp)*SIZE_XY));
}/* ABSx */
/* ----------------------------------------------------------------------- */
short int ABSy(short int crd){
   return (short int)(0.5+((crd/(double)SY_vp)*SIZE_XY));
}/* ABSy */
/*--------------------------------------------------------------------------*/
void offset_pic(OBJECT **obj, int x, int y){

 error_tbg = ER_OK; /* нет ошибок */

 if(((struct HTBG*)*(obj))->ident != 0x5a5a)
 {/* нет картинки */
  error_tbg = ER_NOTPIC;
  return;
 }/* if */

 set_MaxXY(obj);// установка параметров для вывода в окно

 /* Устанавливаем смещение от начала координат */
 ((struct HTBG*)*(obj))->ofsx = x;
 ((struct HTBG*)*(obj))->ofsy = y;

}/* offset_pic */
/* ----------------------------------------------------------------------- */
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
   }/* switch */

   switch(ti.vert){
      case CENTER_TEXT:
         y -= TextHeightCol(txt) / 2;
         break;
      case BOTTOM_TEXT:
         y -= TextHeightCol(txt);
         break;
   }/* switch */

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
      }/* if */
      else{
#ifdef __WIN32__
//         MoveToEx(ti.hdc, x += TextHeight((unsigned char *)t) + 3, y, NULL);
         x += TextHeight((unsigned char *)t) + 3;
#else
         moveto(x += textheight(t) + 3, y);
#endif
      }/* elif */
#ifdef __WIN32__
//      OutText((unsigned char *)t);
      OutTextXY(x, y, (unsigned char *)t);
#else
      outtext(t);
#endif
   }/* while */
   free(dup);

}/* OutTextCol */
/* ----------------------------------------------------------------------- */
int TextWidthCol (char *txt){
   char *dup, *t;
   int smx = 0;

   // ўлзЁб«Ёвм ¤«Ё­л бва®Є ўлЎа вм ¬ ЄбЁ¬ «м­го
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
   }/* if */
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
      }/* if */
   }/* while */
   free(dup);
   return smx;
}/* TextWidthCol */
/* ----------------------------------------------------------------------- */
int TextHeightCol (char *txt){
   char *dup, *t;
   int smy = 0;

   // ўлзЁб«Ёвм ўлб®вл бва®Є Ё б«®¦Ёвм Ёе ўбҐ
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
   }/* if */
   while((t = strtok(NULL, sep)) != NULL){
#ifdef __WIN32__
      smy += TextHeight((unsigned char *)t) + 3;
#else
      smy += textheight(t) + 3;
#endif
   }/* while */
   free(dup);
   return smy;
}/* TextHeightCol */
/* ----------------------------------------------------------------------- */
