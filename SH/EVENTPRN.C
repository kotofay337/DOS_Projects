/*


   CREATEDB.C

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
/* ----------------------------------------------------------------------- */
void main (void){
   char *ts;
   int ev;
   db = DBFOpen(1); // Events
   while(1){
      DBFGetRecord(db);
      DBFGetFieldAsTimeString(db, "‚…ŒŸ", NULL, ts);
      ev = DBFGetFieldAsInt(db, "‘Ž›’ˆ…");
      if((ev >= StartProgram) && (ev < LastEvent)){
         printf("\n%3.1u %s %2.2d:%s ", DBFGetFieldAsInt(db, "ŽŒ…"), ts, ev, EventStr[ev]);
      }/* if */
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

