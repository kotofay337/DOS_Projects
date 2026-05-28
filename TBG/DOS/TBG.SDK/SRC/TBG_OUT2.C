/* tbg_out2.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/*--------------------------------------------------------------------------*/
short int outpic_all(OBJECT **obj)
{
  short int i, tst;

  if(obj == NULL){
     error_tbg = ER_NOTPIC;
     tst = error_tbg;
     goto ret;
  }
  setstateTBG(obj, TBG_BUSY);

  error_tbg = ER_OK; /* нет ошибок */

  if(obj != NULL){
     if(((struct HTBG*)*(obj))->ident != 0x5a5a)
     {/* нет картинки */
      error_tbg = ER_NOTPIC;
      tst = error_tbg;
      goto ret;
     }/* if */
  }/* if */
  else{/* нет картинки */
     error_tbg = ER_NOTPIC;
     tst = error_tbg;
     goto ret;
  }/* elif */

  set_MaxXY(obj);// установка параметров для вывода в окно

// printf("\n.in outpic_all: %s ", ((struct HTBG*)*(obj))->PicName);
  for(i=1;i<=((struct HTBG*)*(obj))->count_ei;i++)
  {
// printf("\n. i:%d",i);
     if(outpic_pn(obj,i))
        break;
  }/* for */

// printf("\n.return outpic_all");
ret:
  setstateTBG(obj, TBG_FREE);
  return tst;

}/* outpic_all */
/*--------------------------------------------------------------------------*/