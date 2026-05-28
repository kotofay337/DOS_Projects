/* -----------------------------------------------------------------------
*
*  Post4.C
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
#include "tbg_tc.h"
#include "dbf.h"
#include "data.h"
/* ----------------------------------------------------------------------- */
typedef
   enum{
      SELECT,
      WAITSELECT,
      WORK1,
      WORK2,
      WORK3,
      WORK4,
      WORK5,
      WORK9,
      SELECTOUT,
      WAITSELECTOUT,
      WORK1OUT,
      WORK2OUT,
      WORK3OUT,
      WORK4OUT,
      CLOSESM,
      ERROROUT,
      ERROR,
      STOPPROGRAM
   }STEP;

static STEP Step = SELECT;
static long t;
/* ----------------------------------------------------------------------- */
int GetType(int t){
   struct time tmStart;
   struct time tmEnd;
   int i, t1 = 0, t2 = 0, t3 = 0;
   LPDBF db;
   gettime(&tm);

   db = DBFOpen(5); // í®ØÎ †°Æ≠•¨•≠‚Æ¢

   for(;DBFGetErrorNum() == DBF_OK;){
      DBFGetRecord(db);
      i = DBFGetFieldAsInt(db, "íàè");
      if(DBFGetErrorNum() != DBF_OK){
         break;
      }/* if */
      DBFGetFieldAsTime(db, "ëÇ" , &tmStart);
      if(DBFGetErrorNum() != DBF_OK){
         break;
      }/* if */
      DBFGetFieldAsTime(db, "èéÇ", &tmEnd);
      if(DBFGetErrorNum() != DBF_OK){
         break;
      }/* if */

      if(
         (tm.ti_hour >= tmStart.ti_hour && tm.ti_hour <= tmEnd.ti_hour) &&
         (tm.ti_min  >= tmStart.ti_min  && tm.ti_min  <= tmEnd.ti_min ) &&
         (tm.ti_sec  >= tmStart.ti_sec  && tm.ti_sec  <= tmEnd.ti_sec )
        ){
        switch(i){
           case 1: t1 = 1; break;
           case 2: t2 = 2; break;
           case 3: t3 = 3; break;
        }/* switch */
      }/* if */
      DBFNext(db);
   }/* for */

   DBFClose(db);

   if(t == t1){
      return t;
   }/* if */

   if(t == t2){
      return t;
   }/* if */

   if(t == t3){
      return t;
   }/* if */

   return 0;

}/* GetType */
/* ----------------------------------------------------------------------- */
int RefreshParking4(void){
   unsigned it1 = 0, it2 = 0, it3 = 0;
   int t1 = 0, t2 = 0, t3 = 0;
   LPDBF db, dbParking;
   char s[16];

   t1 = GetType(1);
   t2 = GetType(2);
   t3 = GetType(3);

   db = DBFOpen(2); // Ä°Æ≠•¨•≠‚Î ® Ø‡ÆØ„·™†
   DBFStart(db);
   for(;DBFGetErrorNum() == DBF_OK;){
      DBFGetRecord(db);
      if(DBFGetErrorNum() != DBF_OK){
         break;
      }/* if */
      if(DBFGetFieldAsInt(db, "NåÖëíÄ") == 0){
         switch(DBFGetFieldAsInt(db, "íàè")){
            case 1:
               if(t1){
                  it1++;
               }/* if */
               break;
            case 2:
               if(t2){
                  it2++;
               }/* if */
               break;
            case 3:
               if(t3){
                  it3++;
               }/* if */
               break;
         }/* switch */
      }/* if */
      DBFNext(db);
   }/* for */

   DBFClose(db);

   dbParking = DBFOpen(3); // ë‚ÆÔ≠™†

   // all unused place clear
   DBFStart(dbParking);
   for(;DBFGetErrorNum() == DBF_OK;){
      DBFGetRecord(dbParking);
      if(DBFGetErrorNum() != DBF_OK){
         break;
      }/* if */
      if(DBFGetFieldAsInt(dbParking, "áÄçüíé") == 2){
         DBFSetFieldAsInt(dbParking, "áÄçüíé", 0);
         DBFSetFieldAsInt(dbParking, "íàè",    0);
         DBFPutRecord(dbParking);
      }/* if */
      DBFNext(dbParking);
   }/* for */

   // reserve type1
   if(t1){
      DBFStart(dbParking);
      for(;it1 && DBFGetErrorNum() == DBF_OK;){
         DBFFindRecord(dbParking, "áÄçüíé", "0");
         if(DBFGetErrorNum() != DBF_OK){
            break;
         }/* if */
         else{
            DBFSetFieldAsString(dbParking, "áÄçüíé", "2");
            DBFSetFieldAsString(dbParking, "íàè", "1");
            DBFPutRecord(dbParking);
            it1--;
         }/* elif */
      }/* for */
   }/* if */
   if(t2){
      DBFStart(dbParking);
      for(;it2 && DBFGetErrorNum() == DBF_OK;){
         DBFFindRecord(dbParking, "áÄçüíé", "0");
         if(DBFGetErrorNum() != DBF_OK){
            break;
         }/* if */
         else{
            DBFSetFieldAsString(dbParking, "áÄçüíé", "2");
            DBFSetFieldAsString(dbParking, "íàè", "2");
            DBFPutRecord(dbParking);
            it2--;
         }/* elif */
      }/* for */
   }/* if */
   if(t3){
      DBFStart(dbParking);
      for(;it3 && DBFGetErrorNum() == DBF_OK;){
         DBFFindRecord(dbParking, "áÄçüíé", "0");
         if(DBFGetErrorNum() != DBF_OK){
            break;
         }/* if */
         else{
            DBFSetFieldAsString(dbParking, "áÄçüíé", "2");
            DBFSetFieldAsString(dbParking, "íàè", "3");
            DBFPutRecord(dbParking);
            it3--;
         }/* elif */
      }/* for */
   }/* if */
   DBFClose(dbParking);

   return 1;

}/* RefreshParking4 */
/* ----------------------------------------------------------------------- */
int Post4(void){
/*
   if((bt - t) / CLK_TCK > 60.F){
      t = bt;
      RefreshParking4();
   }/* if */
*/
   if(Step == SELECT){
      int Free = 0;
      char buf[16];
      LPDBF db;
      load_file(&p, "tbg\\kateg3.tbg");
      db = DBFOpen(3);
      DBFGetRecord(db);
      while(db){
         if(DBFGetFieldAsInt(db, "áÄçüíé") == 0){
            Free++;
         }/* if */
         DBFNext(db);
         DBFGetRecord(db);
         if(DBFGetErrorNum() != DBF_OK){
            break;
         }/* if */
      }/* while */
      DBFClose(db);
      outpic_all(p);
      outtext_obj_num(p, 200, 0, EGA_LIGHTGRAY, itoa(Free, buf, 10), SHOW);
      Step = WAITSELECT;
      return 0;
   }/* if */

   if(Step == SELECTOUT){
      load_file(&p, "tbg\\kat3out.tbg");
      outpic_all(p);
      Step = WAITSELECTOUT;
      return 0;
   }/* if */

   if(Step == WAITSELECT){
      OutTime(p);
#ifdef DEBUG
      if(Key == ESC){
         del_pic(&p);
         Step = STOPPROGRAM;
         return 0;
      }/* if */
#endif
      if(ExtKey == F1){
         del_pic(&p);
         Step = SELECTOUT;
         return 0;
      }/* if */

      if(Key == '0'){
         del_pic(&p);
         Step = WORK1;
         return 0;
      }/* if */

      if(Key == '9'){
         del_pic(&p);
         Step = WORK9;
         return 0;
      }/* if */

      if(Key == '1'){
         del_pic(&p);
         Step = WORK2;
         return 0;
      }/* if */
      if(Key == '2'){
         del_pic(&p);
         Step = WORK3;
         return 0;
      }/* if */
      if(Key == '3'){
         del_pic(&p);
         Step = WORK4;
         return 0;
      }/* if */
      if(Key == '4'){
         del_pic(&p);
         Step = WORK5;
         return 0;
      }/* if */
      if(ExtKey == 68){ // F10
         Step = CLOSESM;
      }/* if */
      return 0;
   }/* if */

   if(Step == CLOSESM){
      OutTime(p);
      switch(PrintFR(-1.0)){
         case 0:// wait
            break;
         case 1:// return
            Step = SELECT;
            RecordEvent(FRPrint);
            del_pic(&p);
            return 2;
         case -1:// error
            Step = SELECT;
            del_pic(&p);
            return -1;
         default:
            break;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WAITSELECTOUT){
      OutTime(p);
#ifdef DEBUG
      if(Key == ESC){
         del_pic(&p);
         Step = STOPPROGRAM;
         return 0;
      }/* if */
#endif
      if(ExtKey == F1){
         del_pic(&p);
         Step = SELECT;
         return 0;
      }/* if */
      if(Key == '1'){
         del_pic(&p);
         Step = WORK1OUT;
         return 0;
      }/* if */
      if(Key == '2'){
         del_pic(&p);
         Step = WORK2OUT;
         return 0;
      }/* if */
      if(Key == '3'){
         del_pic(&p);
         Step = WORK3OUT;
         return 0;
      }/* if */
      if(Key == '4'){
         del_pic(&p);
         Step = WORK4OUT;
         return 0;
      }/* if */
      if(ExtKey == 68){ // F10
         Step = CLOSESM;
      }/* if */
      return 0;
   }/* if */

   if(Step == WORK1){
      switch(Post4_1()){
         case 0: // wait
            return 0;
         case 1: // ok
            Step = SELECT;
            break;
         case -1: // error
            Step = SELECT;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WORK9){
      switch(Abon4(1)){
         case 0: // wait
            return 0;
         case 1: // ok
            Step = SELECT;
            break;
         case -1: // error
            Step = SELECT;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WORK1OUT){
      switch(Post4_1out()){
         case 0: // wait
            return 0;
         case 1: // ok
            Step = SELECTOUT;
            break;
         case -1: // error
            Step = SELECTOUT;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WORK2){
      switch(Post4_2()){
         case 0: // wait
            return 0;
         case 1: // ok
            Step = SELECT;
            break;
         case -1: // error
            Step = SELECT;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WORK2OUT){
      switch(Post4_2out()){
         case 0: // wait
            return 0;
         case 1: // ok
            Step = SELECTOUT;
            break;
         case -1: // error
            Step = SELECTOUT;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WORK3){
      switch(Post4_3()){
         case 0: // wait
            return 0;
         case 1: // ok
            Step = SELECT;
            break;
         case -1: // error
            Step = SELECT;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WORK3OUT){
      switch(Post4_3out()){
         case 0: // wait
            return 0;
         case 1: // ok
            Step = SELECTOUT;
            break;
         case -1: // error
            Step = SELECTOUT;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WORK4){
      switch(Post4_4()){
         case 0: // wait
            return 0;
         case 1: // ok
            Step = SELECT;
            break;
         case -1: // error
            Step = SELECT;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WORK4OUT){
      switch(Post4_4out()){
         case 0: // wait
            return 0;
         case 1: // ok
            Step = SELECTOUT;
            break;
         case -1: // error
            Step = SELECTOUT;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WORK5){
      switch(Post4_5()){
         case 0: // wait
            return 0;
         case 1: // ok
            Step = SELECT;
            break;
         case -1: // error
            Step = SELECT;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == STOPPROGRAM){
      Step = SELECT;
      return 1;
   }/* if */

   if(Step == ERROR){
      if(Key == ENTER){
         Step = SELECT;
      }/* if */
      return 0;
   }/* if */

   if(Step == ERROROUT){
      if(Key == ENTER){
         Step = SELECT;
         return 1;
      }/* if */
      return 0;
   }/* if */

   return -1;

}/* Post4 */
/* ----------------------------------------------------------------------- */

