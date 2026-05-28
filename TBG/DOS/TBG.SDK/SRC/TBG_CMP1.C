/* tbg_cmp1.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

/*--------------------------------------------------------------------------*/
/* (C) Чумак А.И. 1990-93 */
/*--------------------------------------------------------------------------*/
short int cmpcolor_obj_num(OBJECT **obj,unsigned char numobj,unsigned char num,
                     short int color)
{
  short int i,tst=-2;
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

  if(((struct HTBG*)*(obj))->Num_LE == -1)
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
     tst=1;/* элемент обьекта найден */
     for(;(unsigned short int)*((unsigned long*)ptr_Logic+i) == Search;i++){

	if(i > ((struct HTBG*)*(obj))->Num_LE)
	   break;

	Ind=ptr_Logic[i].Index_ei;

	switch( *((char*)*(obj+Ind)) )
   {
	   case OBJ_line:
	      if(((struct ln*)*(obj+Ind))->lncol != color)
		 goto ret;
	      break;
	   case OBJ_rect:
	      if(((struct rc*)*(obj+Ind))->rccol != color)
		 goto ret;
	      break;
	   case OBJ_bar:
	      if(((struct br*)*(obj+Ind))->brcol != color)
		 goto ret;
	      break;
	   case OBJ_elip:
	      if(((struct el*)*(obj+Ind))->elcol != color)
		 goto ret;
	      break;
	   case OBJ_felip:
	      if(((struct fel*)*(obj+Ind))->felfcol != color)
		 goto ret;
	      break;
	   case OBJ_txt:
	      if(((struct tx*)*(obj+Ind))->txcol != color)
		 goto ret;
	      break;
	   case OBJ_fld:
	      if(((struct fl*)*(obj+Ind))->flcol != color)
		 goto ret;
	      break;
	   case OBJ_bkgr:
	      if(((struct bk*)*(obj+Ind))->bkcol != color)
		 goto ret;
	      break;
	   case OBJ_pipeline:
	      if(((struct pln*)*(obj+Ind))->pl.C1 != color)
		 goto ret;
	}/* switch(tipobj) */

     }// for
  }// if

 if(tst==-2)
 {/* нет элемента объекта */
   error_tbg = ER_NOTELEMOBJ;
 }/* if */

ret:
 setstateTBG(obj, TBG_FREE);
 return tst;

}/* cmpcolor_obj_num */
/*--------------------------------------------------------------------------*/