/* tbg_gsz1.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */
#ifndef __WIN32__
#include <graphics.h>
#endif
/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/*--------------------------------------------------------------------------*/
static OBJECT **tobj = NULL;
static unsigned char tnumobj = 0, tnum = 0;
static int ti = 0;
/*--------------------------------------------------------------------------*/
/* сброс предыдущих уставок поиска - поиск с начала*/
short int getsize_obj_num_reset(void){
    tobj = NULL;
    tnumobj = 0;
    tnum = 0;
    ti = 0;
    return 0;
}/* getsize_obj_num_reset */
/*--------------------------------------------------------------------------*/
short int getsize_obj_num
	(OBJECT **obj,unsigned char numobj,unsigned char num,
	 short int *xmin,short int *ymin,short int *xmax,short int *ymax)
{
  short int i,tst=-2;
  short int x,y,x1,y1;
  short int Ind;
  unsigned short int Search;
  short int ofsx, ofsy;
  struct tag_Logic *ptr_Logic;

  OBJECT **mpic = NULL;
  short int tx, ty;

  if(obj == NULL){
     error_tbg = ER_NOTPIC;
     goto ret;
  }
  setstateTBG(obj, TBG_BUSY);

  error_tbg = ER_OK; /* нет ошибок */

  if(((struct HTBG*)*(obj))->ident != 0x5a5a){/* нет картинки */
   error_tbg = ER_NOTPIC;
   goto ret;
  }/* if */

  if(((struct HTBG*)*(obj))->Num_LE==-1){/* нет логических элементов */
   error_tbg = ER_NOTLE;
   goto ret;
  }/* if */

  *xmin = *ymin = *xmax = *ymax = 0;

  set_MaxXY(obj);// установка параметров для вывода в окно

  /* Устанавливаем смещение от начала координат */
  ofsx = ((struct HTBG*)*(obj))->ofsx;
  ofsy = ((struct HTBG*)*(obj))->ofsy;

  ofsx = DMX((long)ofsx);
  ofsy = DMY((long)ofsy);

  Search=(unsigned short int)((numobj << 8)|(num));

  ptr_Logic=((struct HTBG*)*(obj))->Logic;

  if((tobj != obj) || (tnumobj != numobj) || (tnum != num)){
     i = BinSearch(Search,(unsigned long*)ptr_Logic,
                 ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  }/* if */
  else{
     i = ti;
  }/* elif */

  if(i != -1){
	  if((unsigned short int)*((unsigned long*)ptr_Logic + i) == Search){
        if(i <= ((struct HTBG*)*(obj))->Num_LE){
           Ind = ptr_Logic[i].Index_ei;
           switch(*((char*)*(obj + Ind))){
            case OBJ_line:
             x= DMX((long)((struct ln*)*(obj+Ind))->lnx);
             y= DMY((long)((struct ln*)*(obj+Ind))->lny);
             x1=DMX((long)((struct ln*)*(obj+Ind))->lnx+
               (long)((struct ln*)*(obj+Ind))->lndx);
             y1=DMY((long)((struct ln*)*(obj+Ind))->lny+
               (long)((struct ln*)*(obj+Ind))->lndy);
            break;
            case OBJ_rect:
             x= DMX((long)((struct rc*)*(obj+Ind))->rcx);
             y= DMY((long)((struct rc*)*(obj+Ind))->rcy);
             x1=DMX((long)((struct rc*)*(obj+Ind))->rcx+
               (long)((struct rc*)*(obj+Ind))->rcdx);
             y1=DMY((long)((struct rc*)*(obj+Ind))->rcy+
               (long)((struct rc*)*(obj+Ind))->rcdy);
            break;
            case OBJ_bar:
             x= DMX((long)((struct br*)*(obj+Ind))->brx);
             y= DMY((long)((struct br*)*(obj+Ind))->bry);
             x1=DMX((long)((struct br*)*(obj+Ind))->brx+
               (long)((struct br*)*(obj+Ind))->brdx);
             y1=DMY((long)((struct br*)*(obj+Ind))->bry+
               (long)((struct br*)*(obj+Ind))->brdy);
            break;
            case OBJ_elip:
             x= DMX((long)((struct el*)*(obj+Ind))->elx-
               (long)((struct el*)*(obj+Ind))->eldx);
             y= DMY((long)((struct el*)*(obj+Ind))->ely-
               (long)((struct el*)*(obj+Ind))->eldy);
             x1=DMX((long)((struct el*)*(obj+Ind))->elx+
               (long)((struct el*)*(obj+Ind))->eldx);
             y1=DMY((long)((struct el*)*(obj+Ind))->ely+
               (long)((struct el*)*(obj+Ind))->eldy);
            break;
            case OBJ_felip:
             x= DMX((long)((struct fel*)*(obj+Ind))->felx-
               (long)((struct fel*)*(obj+Ind))->feldx);
             y= DMY((long)((struct fel*)*(obj+Ind))->fely-
               (long)((struct fel*)*(obj+Ind))->feldy);
             x1=DMX((long)((struct fel*)*(obj+Ind))->felx+
               (long)((struct fel*)*(obj+Ind))->feldx);
             y1=DMY((long)((struct fel*)*(obj+Ind))->fely+
               (long)((struct fel*)*(obj+Ind))->feldy);
            break;
            case OBJ_txt:
             x=x1= DMX((long)((struct tx*)*(obj+Ind))->txx);
             y=y1= DMY((long)((struct tx*)*(obj+Ind))->txy);
            break;
            case OBJ_fld:
             x=x1= DMX((long)((struct fl*)*(obj+Ind))->flx);
             y=y1= DMY((long)((struct fl*)*(obj+Ind))->fly);
            break;
            case OBJ_bkgr:
             x=x1= 0;
             y=y1= 0;
            break;
            case OBJ_esc:
               mpic = ((struct macro*)*(obj+Ind))->cl->pic;
               tx = ((struct macro*)*(obj+Ind))->mcx;
               ty = ((struct macro*)*(obj+Ind))->mcy;
               offset_pic(mpic, tx, ty);
               getsize_obj_num(mpic, 255, 0, &x, &y, &x1, &y1);
               if(error_tbg != ER_OK){
                  error_tbg = ER_OK;
                  x=x1= DMX((long)tx);
                  y=y1= DMY((long)ty);
               }/* if */
            break;
            case OBJ_reper:
             if((numobj == 0) && (num == 0)){
                x=x1= ((struct reper*)*(obj+Ind))->x;
                y=y1= ((struct reper*)*(obj+Ind))->y;
                *xmax=max(x,x1);
                *xmin=min(x,x1);
                *ymax=max(y,y1);
                *ymin=min(y,y1);
                goto end;
             }/* if */
             else{
                x=x1= DMX((long)((struct reper*)*(obj+Ind))->x);
                y=y1= DMY((long)((struct reper*)*(obj+Ind))->y);
             }/* elif */
            break;
           case OBJ_pipeline:
            x= DMX((long)((struct pln*)*(obj+Ind))->lnx);
            y= DMY((long)((struct pln*)*(obj+Ind))->lny);
            x1=DMX((long)((struct pln*)*(obj+Ind))->lnx+
              (long)((struct pln*)*(obj+Ind))->lndx);
            y1=DMY((long)((struct pln*)*(obj+Ind))->lny+
              (long)((struct pln*)*(obj+Ind))->lndy);
           break;
           }// switch

      /*
           *xmax=(((x) > (x1)) ? (x) : (x1));
           *xmin=(((x) < (x1)) ? (x) : (x1));
           *ymax=(((y) > (y1)) ? (y) : (y1));
           *ymin=(((y) < (y1)) ? (y) : (y1));
      */

           x  += ofsx;
           y  += ofsy;
           x1 += ofsx;
           y1 += ofsy;

           *xmax=max(x,x1);
           *xmin=min(x,x1);
           *ymax=max(y,y1);
           *ymin=min(y,y1);

           if(*xmin < 0) *xmin=0;
           if(*ymin < 0) *ymin=0;

           if(*xmax > MaxX_vp) *xmax=MaxX_vp;
           if(*ymax > MaxY_vp) *ymax=MaxY_vp;
      /*
           setlinestyle(SOLID_LINE,0,1);
           setcolor(EGA_RED);
           rectangle(*xmin,*ymin,*xmax,*ymax);
      */
          tst=0;
          i++;
          goto end;
        }/* if */
     }// if
  }/* if */

 if(tst==-2){/* нет элемента объекта */
    error_tbg = ER_NOTELEMOBJ;
    tobj = NULL;
    tnumobj = 0;
    tnum = 0;
    ti = 0;
 }/* if */
 goto ret;

end:;

 tobj = obj;
 tnumobj = numobj;
 tnum = num;
 ti = i;

ret:;
 setstateTBG(obj, TBG_FREE);
 return error_tbg;

}/* getsize_obj_num */
/*--------------------------------------------------------------------------*/