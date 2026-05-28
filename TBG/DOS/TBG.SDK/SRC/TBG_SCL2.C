/* tbg_scl2.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/* (C) Чумак А.И. 1990-93 */
/*--------------------------------------------------------------------------*/
short int setcolor_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num,short int color,short int show)
{

  short int i,tst=-2,Ind;
  unsigned short int Search;
  struct tag_Logic *ptr_Logic;

  if(obj == NULL){
     tst = error_tbg = ER_NOTPIC;
     goto ret;
  }
  setstateTBG(obj, TBG_BUSY);

  ptr_Logic=((struct HTBG*)*(obj))->Logic;

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

  set_MaxXY(obj);// установка параметров для вывода в окно

  Search=(unsigned short int)((numobj << 8)|(num));

  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  if(i != -1){
	  for(;(unsigned short int)*((unsigned long*)ptr_Logic+i) == Search;i++){

	  if(i > ((struct HTBG*)*(obj))->Num_LE)
	    break;

		  Ind=ptr_Logic[i].Index_ei;

		  switch(*((char*)*(obj+Ind))){

			  case OBJ_line:
				  ((struct ln*)*(obj+Ind))->lncol=color;
				  break;
			  case OBJ_rect:
				  ((struct rc*)*(obj+Ind))->rccol=color;
				  break;
			  case OBJ_bar:
				  ((struct br*)*(obj+Ind))->brcol=color;
				  break;
			  case OBJ_elip:
				  ((struct el*)*(obj+Ind))->elcol=color;
				  break;
			  case OBJ_felip:
				  ((struct fel*)*(obj+Ind))->felfcol = color;
				  ((struct fel*)*(obj+Ind))->felcol  = color;
				  break;
			  case OBJ_txt:
				  ((struct tx*)*(obj+Ind))->txcol=color;
				  break;
			  case OBJ_fld:
				  ((struct fl*)*(obj+Ind))->flcol=color;
				  break;
			  case OBJ_bkgr:
				  ((struct bk*)*(obj+Ind))->bkcol=color;
				  break;
	        case OBJ_pipeline:
	           ((struct pln*)*(obj+Ind))->color=color;
	           break;

		  };/* switch(tipobj) */

			 if(show==NO_SHOW);
			 else
				 outpic_pn(obj,Ind);

			 tst=0;/* элемент обьекта найден */
	  }// for
  }//if

 if(tst==-2)
 {/* нет элемента объекта */
   error_tbg = ER_NOTELEMOBJ;
 }/* if */
ret:
 setstateTBG(obj, TBG_FREE);
 return tst;

}/* setcolor_obj_num */
/*--------------------------------------------------------------------------*/