/* tbg_tbl2.c ---------------------------------------------------------------
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
/* (C) Чумак А.И. 1992 */
/*--------------------------------------------------------------------------*/
/* (C) Чумак А.И. 1992 */
FILE *set_tbl(char *name_tbl)
{

   if((Tbl_Present==Ok  )&&
      (Tbl_Present==Hide))
   {
      error_tbg = ER_CLOSELIB;/* cначала закройте библиотеку */
      return (FILE*)0;
   }//if

   if((fd=fopen(name_tbl,"rb"))==0)
   {
      error_tbg = ER_OPENLIB;/* не открывается библиотека */
      return (FILE*)0;
   }//if

   if(CreateDir(fd,name_tbl)!=0)
   {
      error_tbg = ER_MEMDIR; /* нет памяти для каталога библиотеки */
      return (FILE*)0;
   }//if

   Tbl_Present=Ok;
   sprintf(old_name,"%s",name_tbl);
#ifdef __WIN32__
   sprintf(CurrentTBLName,"%s",name_tbl);
#endif
   return fd;
}/* set_tbl */
/*--------------------------------------------------------------------------*/
#ifdef __WIN32__
char* DLL_EI get_tbl_name(void){
   return CurrentTBLName;
}/* get_tbl_name */
#endif
/*--------------------------------------------------------------------------*/
