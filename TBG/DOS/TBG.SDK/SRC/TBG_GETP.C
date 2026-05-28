/* tbg_getp.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1993 */
/*--------------------------------------------------------------------------*/
char gettip_obj_num(OBJECT **obj,unsigned char numobj,
                               unsigned char num)
{
  short int i,tst=0, Ind;
  unsigned short int Search;
  struct tag_Logic *ptr_Logic;//=((struct HTBG*)*(obj))->Logic;

  if(obj == NULL){
     error_tbg = ER_NOTPIC;
     goto ret;
  }
  setstateTBG(obj, TBG_BUSY);

  if(((struct HTBG*)*(obj))->ident != 0x5a5a) goto ret;
  if(((struct HTBG*)*(obj))->Num_LE==-1) goto ret;

  Search=(unsigned short int)((numobj << 8)|(num));
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  if(i == -1){
     tst = -1;/* есть */
     goto ret;
  }/* if */
  else{
	  if(i > ((struct HTBG*)*(obj))->Num_LE){
	     tst = -1;
        goto ret;
     }/* if */
     Ind = ptr_Logic[i].Index_ei;
	  tst = *((char*)*(obj+Ind));
  }/* elif */

ret:
  setstateTBG(obj, TBG_FREE);
  return tst;
}/* check_obj_num */
/*--------------------------------------------------------------------------*/
