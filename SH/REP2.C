/*



   Borland C++ v3.1 for DOS

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <mem.h>
#include <graphics.h>
#include <bios.h>
#include <time.h>
#include "tbg_tc.h"
#include "dbf.h"
#include "data.h"
/* ----------------------------------------------------------------------- */
LPDBF db;
/* ----------------------------------------------------------------------- */
void PrintSum (char *ds, double CurFRSum){
   printf("\n За %s: %7.1u руб. %2.2u коп. \n",
    ds,
    (unsigned)((long)CurFRSum),
    (unsigned)((CurFRSum - ((long)CurFRSum)) * 100.));
}/* PrintSum */
/* ----------------------------------------------------------------------- */
void main (void){
   char *ts, *ds, oldds[16] = {"00-00-0000"};
   int fs = 1;
   double CurFRSum = 0.0;
   printf("\n Финансовый отчет системы \"Автостоянка\".\n");
   db = DBFOpen(19); // FR Sum
   while(1){

      DBFGetRecord(db);
      DBFGetFieldAsDateString(db, "ДАТА", NULL, ds);
      DBFGetFieldAsTimeString(db, "ВРЕМЯ", NULL, ts);

      if((strcmp(oldds, ds) != 0) && (fs != 1)){
         PrintSum(oldds, CurFRSum);
         strcpy(oldds, ds);
         CurFRSum = 0.0;
      }/* if */

      printf("\n %s < %s >", ts, DBFGetFieldAsString(db, "ФР_СУММА"));
      CurFRSum += DBFGetFieldAsDouble(db, "ФР_СУММА");

      DBFNext(db);

      if(DBFGetErrorNum() == DBF_ERROR_FILE_END){
         break;
      }/* if */

      if(fs){
         strcpy(oldds, ds);
         fs = 0;
      }/* if */

   }/* while */

   PrintSum(ds, CurFRSum);

   DBFClose(db);
   return;
}/* main */
/* ----------------------------------------------------------------------- */
#pragma argsused
void MessageBox(char *Title, char *MsgFmt, ...){};
#pragma argsused
void  SetDNT(void){};
#pragma argsused
int   NumDays(struct date* d1, struct date* d2){return 0;};
/* ----------------------------------------------------------------------- */
