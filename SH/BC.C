/* -----------------------------------------------------------------------
*
*  BC.C
*
*
*
*
*  ----------------------------------------------------------------------- */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <alloc.h>
#include <dos.h>
#include <bios.h>
#include <time.h>
#include <graphics.h>
#include "tbg_tc.h"
#include "dbf.h"
#include "data.h"
/* ----------------------------------------------------------------------- */
int PrintBC(int As, char* Pl, char* Type,
            char* Mon, char* Year, char* FIO, unsigned long BC){

   char *pjfn;
   FILE *pjf;

   pjfn = tempnam(".\\", "spool");
   pjf = fopen(pjfn, "wb");
   fprintf(pjf,
   "\033Q178"
   "\033S2"
   "\033D5"
   "\033?3"
   "\033A12"
   "\033M12"
   "ТЦ \"Щёлково\""
   "\x0D"
   "\033M10"
   "Автостоянка №: "
"%d" // номер АС
   "\x0D"
   "Место №: "
"%s" // Место
   "\x0D"
"%s" // Тип докум.
   "\x0D"
   "\033A11"
   "действителен до\x0D"
   "\033A12"
"%s" // Мес
   "\x0D"
"%s" // Год
   "\x0D"
"%s" // ФИО
   "\x0D"
   "\033A14"
   "\033X3"
   "\033M03"
   "\033{"
"%6.6lu" // ШК
   "}\x0D"
   "\x0D\x0A"
   "\033A11"
   "\x0D\033C"
   , As, Pl, Type, Mon, Year, FIO, BC);
   fclose(pjf);

   if(Emulate){
      free(pjfn);
      return 1;
   }/* if */

   AddSpool(cfg.BCPrinter, pjfn);
   free(pjfn);

   return 1;

}/* PrintBC */
/* ----------------------------------------------------------------------- */
int PrintBCAdmin(int As, char* Type, char* FIO, unsigned long BC){


   char *pjfn;
   FILE *pjf;

   pjfn = tempnam(".\\", "spool");
   pjf = fopen(pjfn, "wb");
   fprintf(pjf,
   "\033Q178"
   "\033S2"
   "\033D5"
   "\033?3"
   "\033A12"
   "\033M12"
   "ТЦ \"Щёлково\""
   "\x0D"
   "\033M10"
   "%s"
   "\x0D"
   "\033A11"
   "Автостоянка № "
"%d" // номер АС
   "\x0D"
"%s" // ФИО
   "\x0D\x0D"
   "\033A14"
   "\033X3"
   "\033M03"
   "\033{"
"%7.7lu" // ШК
   "}\x0D"
   "\x0D\x0A"
   , Type, As, FIO, BC);
   fprintf(pjf, "\x0D\033C");
   fclose(pjf);

   if(Emulate){
      free(pjfn);
      return 1;
   }/* if */

   AddSpool(cfg.BCPrinter, pjfn);
   free(pjfn);
   return 1;

}/* PrintBCAdmin */
/* ----------------------------------------------------------------------- */
