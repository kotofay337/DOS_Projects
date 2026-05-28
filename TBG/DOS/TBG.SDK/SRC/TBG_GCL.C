/* tbg_gcl.c ----------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/*--------------------------------------------------------------------------*/
short int getcolor_obj_num
        (OBJECT **obj,unsigned char numobj,unsigned char num)
{
  short int i,color=-2;
  short int Ind;
  unsigned short int Search;
  struct tag_Logic *ptr_Logic;//=((struct HTBG*)*(obj))->Logic;

  if(obj == NULL){
     error_tbg = ER_NOTPIC;
     goto ret;
  }
  setstateTBG(obj, TBG_BUSY);

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
  {/* нет картинки */
   error_tbg = ER_NOTPIC;
   goto ret;
  }/* if */

  if(((struct HTBG*)*(obj))->Num_LE==-1)
  {/* нет логических элементов */
   error_tbg = ER_NOTLE;
   goto ret;
  }/* if */

  error_tbg = ER_OK; /* нет ошибок */

  Search=(unsigned short int)((numobj << 8)|(num));

  ptr_Logic=((struct HTBG*)*(obj))->Logic;

  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  if(i != -1){
	  for(;(unsigned short int)*((unsigned long*)ptr_Logic+i) == Search;i++){
	     if(i > ((struct HTBG*)*(obj))->Num_LE)
		break;

		  Ind=ptr_Logic[i].Index_ei;

		  switch( *((char*)*(obj+Ind)) ){

			  case OBJ_line:
		 color=((struct ln*)*(obj+Ind))->lncol;
	   break;
	   case OBJ_rect:
		 color=((struct rc*)*(obj+Ind))->rccol;
	   break;
	   case OBJ_bar:
		 color=((struct br*)*(obj+Ind))->brcol;
	   break;
	   case OBJ_elip:
		 color=((struct el*)*(obj+Ind))->elcol;
	   break;
	   case OBJ_felip:
		 color=((struct fel*)*(obj+Ind))->felfcol;
	   break;
	   case OBJ_txt:
		 color=((struct tx*)*(obj+Ind))->txcol;
	   break;
	   case OBJ_fld:
		 color=((struct fl*)*(obj+Ind))->flcol;
	   break;
	   case OBJ_bkgr:
		 color=((struct bk*)*(obj+Ind))->bkcol;
	   break;
	  case OBJ_pipeline:
		 color=((struct pln*)*(obj+Ind))->pl.C1;
	   break;

		  };/* switch(tipobj) */
	  }// for
  }/* if */

 if(color==-2)
 {/* нет элемента объекта */
   error_tbg = ER_NOTELEMOBJ;
 }/* if */
ret:
 setstateTBG(obj, TBG_FREE);
 return color;

}/* getcolor_obj_num */
/*--------------------------------------------------------------------------*/