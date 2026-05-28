/* tbg_sst.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

/*--------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/* (C) Чумак А.И. 1990-97 */
/*--------------------------------------------------------------------------*/
short int setstate_obj
  (OBJECT **obj,unsigned char numobj,unsigned short int state,short int show)
{

  short int i,tst=-2,Ind;
  unsigned short int Search;
  struct tag_Logic *ptr_Logic;//=((struct HTBG*)*(obj))->Logic;

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

  Search=(unsigned short int)(numobj << 8);
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ);
  if(i != -1){
	  for(;((unsigned short int)*((unsigned long*)ptr_Logic+i) & OBJ) == Search;i++){

	     if(i > ((struct HTBG*)*(obj))->Num_LE)
	       break;

        Ind=ptr_Logic[i].Index_ei;

        if(*((char*)*(obj+Ind)) == OBJ_esc){
           ((MP*)*(obj+Ind))->cs = state;
           if(show != NO_SHOW)
	           outpic_pn(obj,Ind);
        }/* if */
        else{
           tst = error_tbg = ER_NOMACRO;
           goto ret;
		  }/* elif */
        tst = 0;/* элемент обьекта найден */
	  }// for
  }//if

 if(tst==-2)
 {/* нет элемента объекта */
   tst = error_tbg = ER_NOTELEMOBJ;
 }/* if */
ret:
 setstateTBG(obj, TBG_FREE);
 return tst;

}/* setstate_obj_num */
/*--------------------------------------------------------------------------*/
short int setstate_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num,unsigned short int state,short int show)
{

  short int i,tst=-2,Ind;
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

  set_MaxXY(obj);// установка параметров для вывода в окно

  Search=(unsigned short int)((numobj << 8)|(num));
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  if(i != -1){
	  for(;(unsigned short int)*((unsigned long*)ptr_Logic+i) == Search;i++){
	     if(i > ((struct HTBG*)*(obj))->Num_LE)
	       break;

        Ind=ptr_Logic[i].Index_ei;

        if(*((char*)*(obj+Ind)) == OBJ_esc){
           ((MP*)*(obj+Ind))->cs = state;
           if(show != NO_SHOW)
	           outpic_pn(obj,Ind);
        }/* if */
        else{
           error_tbg = ER_NOMACRO;
//           return 0;
		  }/* elif */


        tst = 0;/* элемент обьекта найден */
	  }// for
  }//if

 if(tst==-2)
 {/* нет элемента объекта */
   error_tbg = ER_NOTELEMOBJ;
 }/* if */

ret:
 setstateTBG(obj, TBG_FREE);
 return error_tbg;

}/* setstate_obj_num */
/*--------------------------------------------------------------------------*/
unsigned short int getstate_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num)
{

  short int i,tst=-2,Ind;
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
   tst = error_tbg = ER_NOTPIC;
   goto ret;
  }/* if */

  if(((struct HTBG*)*(obj))->Num_LE==-1)
  {/* нет логических элементов */
   tst = error_tbg = ER_NOTLE;
   goto ret;
  }/* if */

  set_MaxXY(obj);// установка параметров для вывода в окно

  Search=(unsigned short int)((numobj << 8)|(num));
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  if(i != -1){
	  for(;(unsigned short int)*((unsigned long*)ptr_Logic+i) == Search;i++){
	     if(i > ((struct HTBG*)*(obj))->Num_LE)
	       break;

        Ind=ptr_Logic[i].Index_ei;

        if(*((char*)*(obj+Ind)) == OBJ_esc){
           tst = ((MP*)*(obj+Ind))->cs;
        }/* if */
        else{
           tst = error_tbg = ER_NOMACRO;
           goto ret;
		  }/* elif */
	  }// for
  }//if

 if(tst==-2)
 {/* нет элемента объекта */
   tst = error_tbg = ER_NOTELEMOBJ;
 }/* if */
ret:
 setstateTBG(obj, TBG_FREE);
 return tst;

}/* getstate_obj_num */
/*--------------------------------------------------------------------------*/
unsigned short int getsavestate_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num)
{

  short int i,tst=-2,Ind;
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
   tst = error_tbg = ER_NOTPIC;
   goto ret;
  }/* if */

  if(((struct HTBG*)*(obj))->Num_LE==-1)
  {/* нет логических элементов */
   tst = error_tbg = ER_NOTLE;
   goto ret;
  }/* if */

  set_MaxXY(obj);// установка параметров для вывода в окно

  Search=(unsigned short int)((numobj << 8)|(num));
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  if(i != -1){
	  for(;(unsigned short int)*((unsigned long*)ptr_Logic+i) == Search;i++){
	     if(i > ((struct HTBG*)*(obj))->Num_LE)
	       break;

        Ind=ptr_Logic[i].Index_ei;

        if(*((char*)*(obj+Ind)) == OBJ_esc){
           tst = ((MP*)*(obj+Ind))->ss;
           goto ret;
        }/* if */
        else{
           tst = error_tbg = ER_NOMACRO;
           goto ret;
		  }/* elif */
	  }// for
  }//if

 if(tst==-2)
 {/* нет элемента объекта */
   tst = error_tbg = ER_NOTELEMOBJ;
 }/* if */

ret:
 setstateTBG(obj, TBG_FREE);
 return tst;

}/* getstate_obj_num */
/*--------------------------------------------------------------------------*/
short int setdata_obj
  (OBJECT **obj,unsigned char numobj, void *ud)
{

  short int i,tst=-2,Ind;
  unsigned short int Search;
  struct tag_Logic *ptr_Logic;//=((struct HTBG*)*(obj))->Logic;

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

  Search=(unsigned short int)(numobj << 8);
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ);
  if(i != -1){
	  for(;((unsigned short int)*((unsigned long*)ptr_Logic+i) & OBJ) == Search;i++){
	     if(i > ((struct HTBG*)*(obj))->Num_LE)
	       break;

        Ind=ptr_Logic[i].Index_ei;

        if(*((char*)*(obj+Ind)) == OBJ_esc){
           if(((MP*)*(obj+Ind))->ud == NULL){
              ((MP*)*(obj+Ind))->ud = ud;
           }/* if */
           else{
              tst = error_tbg = ER_UDALREADY;
              goto ret;
           }/* elif */
        }/* if */
        else{
           tst = error_tbg = ER_NOMACRO;
           goto ret;
		  }/* elif */

        tst = 0;/* элемент обьекта найден */
	  }// for
  }//if

 if(tst==-2)
 {/* нет элемента объекта */
   error_tbg = ER_NOTELEMOBJ;
 }/* if */

ret:
 setstateTBG(obj, TBG_BUSY);
 return tst;

}/* setdata_obj_num */
/*--------------------------------------------------------------------------*/
short int setdata_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num, void *ud)
{

  short int i,tst=-2,Ind;
  unsigned short int Search;
  struct tag_Logic *ptr_Logic;//=((struct HTBG*)*(obj))->Logic;

  if(obj == NULL){
     tst = error_tbg = ER_NOTPIC;
     goto ret;
  }

  setstateTBG(obj, TBG_BUSY);

  error_tbg = ER_OK; /* нет ошибок */

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
  {/* нет картинки */
   error_tbg = ER_NOTPIC;
   return error_tbg;
  }/* if */

  if(((struct HTBG*)*(obj))->Num_LE==-1)
  {/* нет логических элементов */
   error_tbg = ER_NOTLE;
   return error_tbg;
  }/* if */

  set_MaxXY(obj);// установка параметров для вывода в окно

  Search=(unsigned short int)((numobj << 8)|(num));
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  if(i != -1){
	  for(;(unsigned short int)*((unsigned long*)ptr_Logic+i) == Search;i++){
	     if(i > ((struct HTBG*)*(obj))->Num_LE)
	       break;

        Ind=ptr_Logic[i].Index_ei;

        if(*((char*)*(obj+Ind)) == OBJ_esc){
           if(((MP*)*(obj+Ind))->ud == NULL){
              ((MP*)*(obj+Ind))->ud = ud;
           }/* if */
           else{
              tst = error_tbg = ER_UDALREADY;
              goto ret;
           }/* elif */
        }/* if */
        else{
           tst = error_tbg = ER_NOMACRO;
           goto ret;
		  }/* elif */

        tst = 0;/* элемент обьекта найден */
	  }// for
  }//if

 if(tst==-2)
 {/* нет элемента объекта */
   tst = error_tbg = ER_NOTELEMOBJ;
 }/* if */
ret:
 setstateTBG(obj, TBG_FREE);
 return tst;

}/* setdata_obj_num */
/*--------------------------------------------------------------------------*/
void* getdata_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num)
{

  short int i,tst=-2,Ind;
  void *retv = NULL;
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

  set_MaxXY(obj);// установка параметров для вывода в окно

  Search=(unsigned short int)((numobj << 8)|(num));
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i=BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  if(i != -1){
	  for(;(unsigned short int)*((unsigned long*)ptr_Logic+i) == Search;i++){
	     if(i > ((struct HTBG*)*(obj))->Num_LE)
	       break;

        Ind=ptr_Logic[i].Index_ei;

        if(*((char*)*(obj+Ind)) == OBJ_esc){
           retv = ((MP*)*(obj+Ind))->ud;
           goto ret;
        }/* if */
        else{
           error_tbg = ER_NOMACRO;
           goto ret;
		  }/* elif */

	  }// for
  }//if

 if(tst==-2)
 {/* нет элемента объекта */
   error_tbg = ER_NOTELEMOBJ;
 }/* if */

ret:
 setstateTBG(obj, TBG_FREE);
 return retv;

}/* getdata_obj_num */
/*--------------------------------------------------------------------------*/

