/* tbg_out0.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include <math.h>
#ifndef __WIN32__
#include <graphics.h>
#else
/* defined in tbg_out1.c */
int XCompanenta(int radius,int fi);
int YCompanenta(int radius,int fi);
#endif
#include "tbg_tc.h" /* обьявления функций и данных */
/*--------------------------------------------------------------------------*/
/* (C) Шевцов С.П. 1993 */
/*--------------------------------------------------------------------------*/
short int outpic_pn_color(OBJECT **obj,short int pn,short int color)
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
      ptr_ln->col = color;
      x += ofsx;
      y += ofsy;

#ifndef __WIN32__
      setlinestyle(ptr_ln->lnstl, 0, ptr_ln->lnthi);
      setcolor(color);
      line(DMX((long)x),
         DMY((long)y),
         DMX((long)x+(long)dx),
         DMY((long)y+(long)dy));
#else
      SelectObject(hdc,
       cEGA[color].hPen[(ptr_ln->lnthi == THICK_WIDTH) ? USERBIT_LINE : ptr_ln->lnstl]);
      Line(hdc, DMX((long)x),
           DMY((long)y),
           DMX((long)x+(long)dx),
           DMY((long)y+(long)dy));
#endif
    }/* lb */
	 break;

      case OBJ_rect:
         {// lockblock
            struct rc *ptr_rc=((struct rc*)*(obj+pn));
            if(ptr_rc->rcstl != GABARIT_LINE){
               x =ptr_rc->rcx;
               y =ptr_rc->rcy;
               dx=ptr_rc->rcdx;
               dy=ptr_rc->rcdy;
               ptr_rc->col = color;
               x += ofsx;
               y += ofsy;
#ifndef __WIN32__
               setlinestyle(ptr_rc->rcstl, 0, ptr_rc->rcthi);
               setcolor(color);
               rectangle(DMX((long)x),
                    DMY((long)y),
                    DMX((long)x+(long)dx),
                    DMY((long)y+(long)dy));
#else
               SelectObject(hdc,
                  cEGA[color].hPen[(ptr_rc->rcthi == THICK_WIDTH) ? USERBIT_LINE : ptr_rc->rcstl]);
               Rect(hdc, DMX((long)x),
                         DMY((long)y),
                         DMX((long)x+(long)dx),
                         DMY((long)y+(long)dy));
#endif
            }/* if */
         }/* lockblock */
	      break;

      case OBJ_bar:
         {// lockblock
         struct br *ptr_br=((struct br*)*(obj+pn));
         x =ptr_br->brx;
         y =ptr_br->bry;
         dx=ptr_br->brdx;
         dy=ptr_br->brdy;
         ptr_br->col = color;
         x += ofsx;
         y += ofsy;
#ifndef __WIN32__
          setfillstyle(ptr_br->brstl, color);
          disable();
          bar(DMX((long)x),
              DMY((long)y),
              DMX((long)x+(long)dx),
              DMY((long)y+(long)dy));
          enable();
#else
         SelectObject(hdc, cEGA[color].hBrush[ptr_br->brstl]);
         Bar(hdc, DMX((long)x),
                  DMY((long)y),
                  DMX((long)x+(long)dx),
                  DMY((long)y+(long)dy));
#endif
    }/* lockblock */
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
         ptr_el->col = color;
         x += ofsx;
         y += ofsy;
#ifndef __WIN32__
	 setlinestyle(ptr_el->elstl, 0, ptr_el->elthi);
	 setcolor(color);
	 ellipse(DMX((long)x),
		 DMY((long)y),
		 fi1,fi2,
		 DMX((long)dx),
		 DMY((long)dy));
#else
         SelectObject(hdc,
          cEGA[color].hPen[(ptr_el->elthi == THICK_WIDTH) ? USERBIT_LINE : ptr_el->elstl]);
         Arc(hdc,DMX((long)x - (long)dx),
                 DMY((long)y - (long)dy),
                 DMX((long)x + (long)dx),
                 DMY((long)y + (long)dy),
                 DMX((long)x + (long)XCompanenta(dx, fi1)),
                 DMY((long)y - (long)YCompanenta(dy, fi1)),
                 DMX((long)x + (long)XCompanenta(dx, fi2)),
                 DMY((long)y - (long)YCompanenta(dy, fi2)));
#endif
    }/* lockblock */
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
         ptr_fel->col=color;
         x += ofsx;
         y += ofsy;
#ifndef __WIN32__
	 setlinestyle(ptr_fel->felstl, 0, ptr_fel->felthi);
	 setcolor(color);
    setfillstyle(ptr_fel->felfstl, color);
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
#else
         SelectObject(hdc,
         cEGA[color].hPen[(ptr_fel->felthi == THICK_WIDTH) ? USERBIT_LINE : ptr_fel->felstl]);
         SelectObject(hdc, cEGA[color].hBrush[ptr_fel->felfstl]);
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
#endif
    }/* lockblock */
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
       ptr_tx->col=color;
       setcolor(color);
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
         ptr_tx->col=color;
         x = ptr_tx->txx;
         y = ptr_tx->txy;
         x += ofsx;
         y += ofsy;
         SelectObject(hdc, cEGA[color].hPen[SOLID_LINE]);
         SetHDCFont(hdc);
         OutTextXY(DMX((long)x), DMY((long)y), (unsigned char*)(ptr_tx->txt));
#endif
         }// lockblock
	 break;

	 case OBJ_fld:{
       struct fl *ptr_fl = ((struct fl*)*(obj+pn));
	    x =ptr_fl->flx;
	    y =ptr_fl->fly;
       ptr_fl->col=color;
       x += ofsx;
       y += ofsy;

#ifndef __WIN32__
	    setfillstyle(ptr_fl->flstl,color);
	    floodfill(DMX((long)x),DMY((long)y),ptr_fl->flbrd);
#else
      SelectObject(hdc, cEGA[color].hBrush[ptr_fl->flstl]);
      FloodFill(hdc, DMX((long)x),DMY((long)y),cEGA[ptr_fl->flbrd].rgb);
#endif
       }
	    break;

	 case OBJ_bkgr:{
       struct bk *ptr_bkgr = ((struct bk*)*(obj+pn));
       ptr_bkgr->col = color;
#ifndef __WIN32__
	    setbkcolor(color);
#else
#endif
     }/* lb */
     break;
    case OBJ_esc:
       {/* lockblock */
          MP *mp = ((MP*)*(obj+pn));
#ifdef __WIN32__
          mp->cl->hdc = hdc;
#endif
          x = mp->mcx;
          y = mp->mcy;
          x += ofsx;
          y += ofsy;
          MPCDraw(mp->cl, x, y, color, mp->ud, mp);
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

        x += ofsx;
        y += ofsy;

        ptr_pln->pl.xs = DMX((long)x);
        ptr_pln->pl.ys = DMY((long)y);
        ptr_pln->pl.xe = DMX((long)x + (long)dx);
        ptr_pln->pl.ye = DMY((long)y + (long)dy);
   #ifndef __WIN32__
         setlinestyle(ptr_pln->lnstl, 0, ptr_pln->lnthi);
         pipeline(&(ptr_pln->pl), color);
   #else
         pipeline(hdc, &(ptr_pln->pl), color, ptr_pln->lnstl, ptr_pln->lnthi);
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

 Show_tbg=OFF; // флаг занятости TBG выводом на экран =свободно=

 return error_tbg;

}/* outpic_pn_color */
/*--------------------------------------------------------------------------*/