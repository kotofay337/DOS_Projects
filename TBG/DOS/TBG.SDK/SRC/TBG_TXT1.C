/* tbg_txt1.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/*--------------------------------------------------------------------------*/
char* gettext_obj_num
    (OBJECT **obj,unsigned char numobj,unsigned char num)
{
  short int i,Ind;
  char *ptr=NULL;
  unsigned short int Search;
  struct tag_Logic *ptr_Logic;//=((struct HTBG*)*(obj))->Logic;

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
	  for(;(unsigned short int)*((unsigned long*)ptr_Logic+i) == Search;i++){

	     if(i > ((struct HTBG*)*(obj))->Num_LE)
		break;

	     Ind=ptr_Logic[i].Index_ei;

        if(*((char*)*(obj+Ind))==OBJ_esc) continue;

        if(*((char*)*(obj+Ind))!=OBJ_txt)
        {/* элемент обьекта не текст */
         error_tbg = ER_NOTTEXT;
         goto end;
        }/* if */

        ptr=(char*)((struct tx*)*(obj+Ind))->txt;
        /* элемент обьекта найден */
        goto end;
     }/* for */
  }/* if */

 if(ptr==NULL)
 {/* нет элемента объекта */
   error_tbg = ER_NOTELEMOBJ;
 }/* if */
 end:;

ret:
 setstateTBG(obj, TBG_FREE);
 return ptr;

}/* gettext_num_obj */
/*--------------------------------------------------------------------------*/