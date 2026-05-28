/* tbg_chk1.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1993 */
/*--------------------------------------------------------------------------*/
short int check_obj(OBJECT **obj,unsigned char numobj)
{
  short int i,tst=0;
  unsigned short int Search;
  struct tag_Logic *ptr_Logic;

  setstateTBG(obj, TBG_BUSY);

  if(obj == NULL)
     goto ret;
  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
     goto ret;
  if(((struct HTBG*)*(obj))->Num_LE==-1)
     goto ret;

  ptr_Logic=((struct HTBG*)*(obj))->Logic;

  set_MaxXY(obj);// установка параметров для вывода в окно

  Search=(unsigned short int)((numobj << 8)|0);

  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ);

  if(i != -1)
     tst = 1;/* есть */
ret:
  setstateTBG(obj, TBG_FREE);
  return tst;

}/* check_obj */

/*--------------------------------------------------------------------------*/