/* tbg_out5.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

/*--------------------------------------------------------------------------*/
/* (C) Шевцов С.П. 1993 */
/*--------------------------------------------------------------------------*/
short int outpic_obj_color(OBJECT **obj,unsigned char numobj,short int color)
{
  short int i,tst=-2;
  unsigned short int Search;
  struct tag_Logic *ptr_Logic;//=((struct HTBG*)*(obj))->Logic;

  if(obj == NULL){
     error_tbg = ER_NOTPIC;
     tst = error_tbg;
     goto ret;
  }
  setstateTBG(obj, TBG_BUSY);

  error_tbg = ER_OK; /* нет ошибок */

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
  {/* нет картинки */
   error_tbg = ER_NOTPIC;
   tst = error_tbg;
   goto ret;
  }/* if */

  if(((struct HTBG*)*(obj))->Num_LE==-1)
  {/* нет логических элементов */
   error_tbg = ER_NOTLE;
   tst = error_tbg;
   goto ret;
  }/* if */

  set_MaxXY(obj);// установка параметров для вывода в окно

  Search=(unsigned short int)((numobj << 8)|0);
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ);
  if(i != -1){
     tst=0;
	  for(;((unsigned short int)*((unsigned long*)ptr_Logic+i) & OBJ) == Search;i++){

	     if(i > ((struct HTBG*)*(obj))->Num_LE)
		break;

	     outpic_pn_color(obj,ptr_Logic[i].Index_ei,color);

	  }// for
  }/* if */

 if(tst==-2)
 {/* нет объекта */
   error_tbg = ER_NOTOBJ;
 }/* if */

ret:
 setstateTBG(obj, TBG_FREE);
 return tst;

}/* outpic_obj_color */
/*--------------------------------------------------------------------------*/