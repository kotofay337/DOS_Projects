/* tbg_get2.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

/*--------------------------------------------------------------------------*/
/* (C)  Чумак А.И. 1992 */
/* (C) Павельев П.И. 1992 */
/* (C) Павельев П.И. 1993 */
/*--------------------------------------------------------------------------*/
/*
   Функция возвращает минимальный номер
   элемента заданного обьекта (очень удобно
   для подсчета min количества пунктов меню).
*/
short int getmin_obj(OBJECT **obj,unsigned char numobj,short int *num_cou)
{
  short int i,tst=-2;
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

  *num_cou = 0;

  error_tbg = ER_OK; /* нет ошибок */

  Search=(unsigned short int)((numobj << 8)|0);
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ);
  if(i != -1){
     tst=0;
     *num_cou = (ptr_Logic+i)->num;//номер элемента;
  }//if

 if(tst==-2)
 {/* нет объекта */
   error_tbg = ER_NOTOBJ;
 }/* if */
ret:
 setstateTBG(obj, TBG_FREE);
 return error_tbg;

}/* getmin_obj */
/*--------------------------------------------------------------------------*/