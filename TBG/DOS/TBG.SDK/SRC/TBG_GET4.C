/* tbg_get4.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

/*--------------------------------------------------------------------------*/
/* (C) Шевцов С.П. 1993 */
/*--------------------------------------------------------------------------*/
short int get_num_obj
	(OBJECT **obj,unsigned char numobj,unsigned char num,
	 unsigned char numobj2)
{
  short int i,i2,tst=-1;
  short int Ind1,Ind2;
  unsigned short int Search;
  unsigned short int Search2;
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

  if(i != -1)
     Ind1=ptr_Logic[i].Index_ei;
  else
     tst = -2;

  Search2=(unsigned short int)((numobj2 << 8)|0);
  i2=BinSearch(Search2,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ);

  if(i2 != -1){
    for(;((unsigned short int)*((unsigned long*)ptr_Logic+i2) & OBJ) == Search2;i2++){

        if(i2 > ((struct HTBG*)*(obj))->Num_LE)
           break;

	Ind2=ptr_Logic[i2].Index_ei;
        if(Ind1 == Ind2){
           tst=ptr_Logic[i2].num;
	   goto end;
        }// if
     }// for
  } else tst = -3;

 end:;

 if(tst==-2)
 {/* нет элемента объекта */
   error_tbg = ER_NOTELEMOBJ;
 }/* if */

 if(tst==-3)
 {/* нет объекта */
   error_tbg = ER_NOTOBJ;
 }/* if */

ret:
 setstateTBG(obj, TBG_FREE);
 return tst;

}/* get_num_obj */
/*--------------------------------------------------------------------------*/