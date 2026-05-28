/* tbg_set3.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/


#ifndef __WIN32__
#include <graphics.h>
#endif
#include "tbg_tc.h" /* обьявления функций и данных */
/*--------------------------------------------------------------------------*/
/* (C) Чумак А.И. 1990-93 */
/*--------------------------------------------------------------------------*/
short int setcoordinate_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num,
   short int x1,short int y1,short int x2,short int y2)
{

  short int i,tst=-2,Ind;
  unsigned short int Search;
  struct tag_Logic *ptr_Logic;//=((struct HTBG*)*(obj))->Logic;

  if(obj == NULL){
     tst = error_tbg = ER_NOTPIC;
     goto ret;
  }
  setstateTBG(obj, TBG_BUSY);

  error_tbg = ER_OK; /* нет ошибок */

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
  {/* нет картинки */
   tst = error_tbg = ER_NOTPIC;
   goto ret;
  }/* if */

  if(((struct HTBG*)*(obj))->Num_LE==-1)
  {/* нет логических элементов */
   tst = error_tbg = ER_NOTLE;
   goto ret;
  }/* if */

  x2-=x1; //x2 => dx
  y2-=y1; //y2 => dy

  Search=(unsigned short int)((numobj << 8)|(num));
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  if(i != -1){
	  for(;(unsigned short int)*((unsigned long*)ptr_Logic+i) == Search;i++){

	  if(i > ((struct HTBG*)*(obj))->Num_LE)
	    break;

		  Ind=ptr_Logic[i].Index_ei;

		  switch(*((char*)*(obj+Ind))){
#ifndef __WIN32__
			  case OBJ_line:
				  ((struct ln*)*(obj+Ind))->lnx =
              (short int)(0.5+((x1/(double)getmaxx())*SIZE_XY));

				  ((struct ln*)*(obj+Ind))->lny =
              (short int)(0.5+((y1/(double)getmaxy())*SIZE_XY));

				  ((struct ln*)*(obj+Ind))->lndx=
              (short int)(0.5+((x2/(double)getmaxx())*SIZE_XY));

				  ((struct ln*)*(obj+Ind))->lndy=
              (short int)(0.5+((y2/(double)getmaxy())*SIZE_XY));
				  break;
#else
			  case OBJ_line:
				  ((struct ln*)*(obj+Ind))->lnx =
              (short int)(0.5+((x1/(double)((struct HTBG*)*(obj))->SIZEX)*SIZE_XY));

				  ((struct ln*)*(obj+Ind))->lny =
              (short int)(0.5+((y1/(double)((struct HTBG*)*(obj))->SIZEY)*SIZE_XY));

				  ((struct ln*)*(obj+Ind))->lndx=
              (short int)(0.5+((x2/(double)((struct HTBG*)*(obj))->SIZEX)*SIZE_XY));

				  ((struct ln*)*(obj+Ind))->lndy=
              (short int)(0.5+((y2/(double)((struct HTBG*)*(obj))->SIZEY)*SIZE_XY));
				  break;
#endif
           default:
              break;
		  };/* switch(tipobj) */
			 tst=0;/* элемент обьекта найден */
	  }// for
  }//if


 if(tst==-2)
 {/* нет элемента объекта */
   error_tbg = ER_NOTELEMOBJ;
 }/* if */
ret:
 setstateTBG(obj, TBG_BUSY);
 return tst;

}/* setcoordinate_obj_num */
/*--------------------------------------------------------------------------*/