/* -----------------------------------------------------------------------
*
*  AdmFr.C
*
*
*  ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <bios.h>
#include <time.h>
#include <graphics.h>
#include <ctype.h>
#include "tbg_tc.h"
#include "dbf.h"
#include "com.h"
#include "data.h"
#include "editstr.h"

/* ----------------------------------------------------------------------- */
typedef
   enum{
      START,
      WAITSELECT,
      WORKZ,
      WORKX,
      WAITERROR,
      WAITERROROUT,
      STOPPROGRAM
   }STEP;

static STEP Step = START;
/* ----------------------------------------------------------------------- */
int AdmFr(void){

   if(Step == START){
      Step = WAITSELECT;
      load_file(&p, "tbg\\admfr.tbg");
      outpic_all(p);
      return 0;
   }/* if */

   if(Step == WAITSELECT){
      OutTime(p);
      if(Key){
         if(Key == '1'){
            Step = WORKZ;
         }/* if */
         if(Key == '2'){
            Step = WORKX;
         }/* if */
         if(Key == '3'){
            Step = STOPPROGRAM;
            del_pic(&p);
         }/* if */
      }/* if */
      return 0;
   }/* if */

   if(Step == STOPPROGRAM){
      Step = START;
      return 1;
   }/* if */

   if(Step == WORKX || Step == WORKZ){
      switch(PrintFR(Step == WORKX ? -2.0 : Step == WORKZ ? -1.0 : -3.0)){
         case 0: // wait
            break;
         case 1:
            RecordEvent(FRPrint);
            printf(sBell);
            Step = WAITSELECT;
            break;
         case -1:
            return -1;
         default:
            break;
      }/* switch */
      return 0;
   }/* if */

   return -1;
}/* AdmFr */
/* ----------------------------------------------------------------------- */
