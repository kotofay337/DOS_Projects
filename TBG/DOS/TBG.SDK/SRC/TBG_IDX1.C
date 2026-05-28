/* tbg_idx1.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

#include <string.h>
#include <stdlib.h>
#include <dir.h>
#include <io.h>

/*--------------------------------------------------------------------------*/
short int Index_obj_num(OBJECT **obj,unsigned char numobj,unsigned char num)
{
  struct tag_Logic *ptr_Logic;//=((struct HTBG*)*(obj))->Logic;
  short int Ind,i, tst;
  unsigned short int Search;

  if(obj == NULL){
     error_tbg = ER_NOTPIC;
     goto ret;
  }
  setstateTBG(obj, TBG_BUSY);

  error_tbg = ER_OK; /* нет ошибок */

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

  Search=(unsigned short int)((numobj << 8)|(num));
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  if(i != -1){
     Ind=ptr_Logic[i].Index_ei;
     tst = Ind;
     goto ret;
  }/* thif */
  else{
     error_tbg = ER_NOTELEMOBJ;
     tst = i;
     goto ret;
  }/* elif */
ret:
  setstateTBG(obj, TBG_FREE);
  return tst;
}/* Index_obj_num */
/*--------------------------------------------------------------------------*/