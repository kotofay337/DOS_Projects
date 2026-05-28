/* tbg_del.c ----------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

#include <alloc.h>
//#define DEBUG
extern int lo;
/*-------------------------------------------------------------------------*/
/* (C) Павельев П.И. 1990-92 */
/*-------------------------------------------------------------------------*/
short int del_pic(OBJECT ***objret)
{

 short int i;
 OBJECT **obj=*objret;

 error_tbg = ER_OK; /* нет ошибок */
   if(obj != NULL){
#ifdef DEBUG
      printf("\n %d Delpic %p pict: %s ", lo, obj, hTbg(obj)->PicName);
#endif
     if(((struct HTBG*)*(obj))->ident != 0x5a5a)
     {/* нет картинки */
      error_tbg = ER_NOTDEL;
      return error_tbg;
     }/* if */
   }/* if */
   else{/* нет картинки */
     error_tbg = ER_NOTDEL;
     return error_tbg;
   }/* elif */

 for(i=1;i<=((struct HTBG*)*(obj))->count_ei;i++)
 {

  switch( *((char*)*(obj+i)) )
  {
   case OBJ_line:
     free(((struct ln*)*(obj+i)));
	break;
   case OBJ_rect:
     free(((struct rc*)*(obj+i)));
	break;
   case OBJ_bar:
     free(((struct br*)*(obj+i)));
	break;
   case OBJ_elip:
     free(((struct el*)*(obj+i)));
	break;
   case OBJ_felip:
     free(((struct fel*)*(obj+i)));
	break;
   case OBJ_txt:
     free(((struct tx*)*(obj+i))->txt);
     free(((struct tx*)*(obj+i)));
	break;
   case OBJ_fld:
     free(((struct fl*)*(obj+i)));
	break;
   case OBJ_bkgr:
     free(((struct bk*)*(obj+i)));
	break;
   case OBJ_esc:
     MPCFree(((MP*)*(obj+i))->cl);
     free   (((MP*)*(obj+i)));
	break;
   case OBJ_reper:
     free(((struct reper*)*(obj+i)));
	break;
   case OBJ_pipeline:
     free(((struct pln*)*(obj+i)));
	break;
  }/* switch(tipobj) */

  }/* for */

  /* размер рисунка  */
  if(((struct HTBG*)*(obj))->Num_LE >= 0)
  {
    free(((struct HTBG*)*(obj))->Logic);
  }/* if */

  free(((struct HTBG*)*(obj)));

  free(obj);
  *objret=NULL;
#ifdef DEBUG
   printf("OK!");
   lo--;
#endif

  return error_tbg;
}/* del_pic */
/*-------------------------------------------------------------------------*/