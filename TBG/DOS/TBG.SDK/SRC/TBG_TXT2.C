/* tbg_txt2.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include <stdarg.h>
#include <string.h>
#include "tbg_tc.h" /* обьявления функций и данных */
/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/* (C) Чумак    А.И.    1996 */
/*--------------------------------------------------------------------------*/
short int outtext_obj_num
    (OBJECT **obj,unsigned char numobj,unsigned char num,
     short int color_fon,char const * const text,short int show)
{
  short int i,Ind,tst=-2;
  short int col_txt;
  unsigned short int Search;
  struct tag_Logic *ptr_Logic;//=((struct HTBG*)*(obj))->Logic;

  char buf[256];

  if(obj == NULL){
     tst = error_tbg = ER_NOTPIC;
     goto ret;
  }
  setstateTBG(obj, TBG_BUSY);

  error_tbg = ER_OK; /* нет ошибок */

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
  {/* нет картинки */
   tst = error_tbg = ER_NOTPIC;
   goto ret;
  }/* if */

  if(((struct HTBG*)*(obj))->Num_LE==-1)
  {/* нет логических элементов */
   tst = error_tbg = ER_NOTLE;
   goto ret;
  }/* if */

  set_MaxXY(obj);// установка параметров для вывода в окно

  //if(show != SHOW)
  strcpy(buf,text);// не пpямой показ

  Search=(unsigned short int)((numobj << 8)|(num));
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  if(i != -1){
	  for(;(unsigned short int)*((unsigned long*)ptr_Logic+i) == Search;i++){

	     if(i > ((struct HTBG*)*(obj))->Num_LE)
		break;

	     Ind=ptr_Logic[i].Index_ei;

          if(*((char*)*(obj+Ind))!=OBJ_txt)
          {/* элемент обьекта не текст */
           error_tbg = ER_NOTTEXT;
           goto end;
          }/* if */

          // усекаем текст до возможного для этой стpоки
          // чтобы с ним уже сpавнивать,а не сполной стpокой
          if(show != SHOW) buf[((struct tx*)*(obj+Ind))->txmx]=0;

          if((strcmp(((struct tx*)*(obj+Ind))->txt,buf) != 0)||
             (show == SHOW))
          {// текст изменился

           col_txt=((struct tx*)*(obj+Ind))->txcol;/* исходный цвет запомнить */
           ((struct tx*)*(obj+Ind))->txcol=color_fon; /* цвет фона */
           outpic_pn(obj,Ind);/* старая строкa текста */

           ((struct tx*)*(obj+Ind))->txcol=col_txt; /* исходный цвет восстановить */

           strncpy(((struct tx*)*(obj+Ind))->txt,
                   buf,
                   ((struct tx*)*(obj+Ind))->txmx);

           //завершающий '\0' на всякий случай
           ((struct tx*)*(obj+Ind))->txt[((struct tx*)*(obj+Ind))->txmx]=0;

           outpic_pn(obj,Ind);/* новая строка текста */

          }// текст изменился

          tst=0;/* элемент обьекта найден */

     }// for
  }/* if */

 if(tst==-2)
 {/* нет элемента объекта */
   error_tbg = ER_NOTELEMOBJ;
 }/* if */
end:;
ret:
 setstateTBG(obj, TBG_FREE);
 return tst;

}/* outtext_obj_num */
/*--------------------------------------------------------------------------*/