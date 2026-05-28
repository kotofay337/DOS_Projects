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
extern char *EventStr[];
/*
"Останов программы по ошибке",               // StopProgramError,      // 2
"Ошибка при входе в систему",                // LoginSystemError,      // 4
"ШК введен вручную",                         // ManualBCInput,         // 23
"Ошибка при повторном вводе ГРН",            // ErrorInputREGNUM,      // 28
"ШБ не работает",                            // ErrorSHBNotWorking,    // 29
"Принтер не готов",                          // ErrorPrinterNotReady,  // 30
"ШК не найден в базе",                       // ErrorBCNotFound,       // 32
"Нет дисковой памяти(< 2k)",                 // ErrorNotAvailDiskMem,  // 34
"Принудительное завершение процедуры ВЪЕЗДА",// StopProcedureINPUT,    // 37
"Не открывается БД: Пропуск",                // ErrorGeneral1     ,    // Error Open database Пропуска
"Не найден ШК в БД: Пропуск",                // ErrorGeneral2     ,    // Error Find BARCODE in Пропуска
"Ошибка при распределении памяти 1",         // ErrorGeneral3     ,    // Error memory allocate in Post1_2
*/
/* ----------------------------------------------------------------------- */
void main (void){
   char *ts, *ds;

   printf("\n Отчет о нарушениях системы \"Автостоянка\".\n");

   db = DBFOpen(1); // Events
   while(1){
      DBFGetRecord(db);

      switch(DBFGetFieldAsInt(db, "СОБЫТИЕ")){
         case StopProgramError:
         case LoginSystemError:
         case ManualBCInput:
         case ErrorInputREGNUM:
         case ErrorSHBNotWorking:
         case ErrorPrinterNotReady:
         case ErrorBCNotFound:
         case ErrorNotAvailDiskMem:
         case StopProcedureINPUT:
         case ErrorGeneral1:
         case ErrorGeneral2:
         case ErrorGeneral3:
            DBFGetFieldAsDateString(db, "ДАТА", NULL, ds);
            DBFGetFieldAsTimeString(db, "ВРЕМЯ", NULL, ts);
            printf("\n %s %s < %s >", ds, ts, EventStr[DBFGetFieldAsInt(db, "СОБЫТИЕ")]);
            break;
         default:
            break;
      }/* switch */
      DBFNext(db);
      if(DBFGetErrorNum() == DBF_ERROR_FILE_END){
         break;
      }/* if */
   }/* while */

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
