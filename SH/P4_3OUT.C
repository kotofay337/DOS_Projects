/* -----------------------------------------------------------------------
*
*  P4_3out.C
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

//#undef DEBUG

/* ----------------------------------------------------------------------- */
typedef
   enum{
      SCAN,
      WAITSCAN,
      MANUALINPUT,
      GETBASE,
      GRNINPUT,
      WAITGRNINPUT,
      COMPAREGRN,
      OUTINFO,
      WAITQSL,
      SHB,
      WAITSHB,
      STOPPROGRAM,
      ERROROUT,
      ERROR
   }STEP;

static STEP Step = SCAN;
static LPDBF db, dbParking;
static char stringedit[15];
static int editobj = 100, editnum = 0;
static OBJECT **m = NULL;

static int BlinkObject[2];
static char *GRNs;
static int   GRNLen;
static int   iGRN = 0;
static int NOs, NOe, NOc;
static int NumRunning;
static char *nfPlaceNum    = "NМЕСТА",
            *nfCarCategory = "ЛГ",
            *nfInUse       = "ЗАНЯТО",
            *nfGRN         = "ГРН",
            *nfBC          = "ШК",
            *nfNumRunning  = "ОСТАЛОСЬ" ,
            *keyInFree     = "0",
            *keyInUse      = "1",
            *keyInMark     = "2",
             PlaceNum[6],
             NumRunningBuf[6],
             BCBuf[16],
            *nfsOutPerson[]={
"ФИО",      // 0
"МАРКА",    // 1
"СВД",      // 2
"С",        // 3
"ПО",       // 4
"ОСТАЛОСЬ"  // 5
};
/* ----------------------------------------------------------------------- */
int Post4_3out(void){

   int D, M, Y;
   struct date d;

   if(Step == SCAN){
      load_file(&p, "tbg\\scanner1.tbg");
      outpic_all(p);
      ClearScan = TRUE;
      iGRN = 0;
      BlinkObject[0] = 0;
      BlinkObject[1] = 0;
/*
      db = DBFOpen(3); // Стоянка

      DBFFindRecord(db, nfInUse, keyInFree);
      if(DBFGetErrorNum() != DBF_OK){
         if(DBFGetErrorNum() == DBF_ERROR_RECORD_NOT_FOUND){
            InfoMsg("Свободных мест  "
                    "     НЕТ!       ");
            DBFClose(db);
            Step = ERROROUT;
            return 0;
         }/* if */
         else{
            DBFClose(db);
            return -1;
         }/* elif */
      }/* if */
      else{
         strcpy(PlaceNum, DBFGetFieldAsString(db, nfPlaceNum));
      }/* elif */
      DBFClose(db);
*/
      Step = WAITSCAN;
      return 0;
   }/* if */

   if(Step == WAITSCAN){
      OutTime(p);

      if(Key != 0){
#ifdef DEBUG
         if(Key == ESC){
            del_pic(&p);
            Step = SCAN;
            return 1;
         }/* if */
#endif
         if(Key == 'b'){
            Scanned = TRUE;
         }/* if */
         if(isdigit(Key)){
            load_file(&m, "tbg\\manscan1.tbg");
            outpic_all(m);
            stringedit[0] = 0;
            Edit_Init(m,editobj,editnum,stringedit, strlen_obj_num(m,editobj,editnum) + 1);
            Refresh_Display(m,editobj,editnum);
            EditLine_obj_num(m,editobj,editnum);
            Step = MANUALINPUT;
            return 0;
         }/* if */
      }/* if */
      return 0;
   }/* if */

   if(Step == MANUALINPUT){
      OutTime(p);
      switch(EditLine_obj_num(m,editobj,editnum)){
         case OK      :
            strcpy(ScanBuf, Edit_Stop(m,editobj,editnum));
            del_pic(&m);
            RecordEvent(Scanned ? ScannerBCInput : ManualBCInput);
            RecordBC(ScanBuf, Scanned ? 0 : 1);
            Step = GETBASE;
            return 0;
         case CONTINUE:
            return 0;
         case HELP    :
            return 0;
      }//switch
      return 0;
   }/* if */

   if(Step == GETBASE){
      db = DBFOpen(12); // Пропуска
      if(db == NULL){
         ErrorMsg(SysAdm);
         RecordEvent(ErrorGeneral1);
         Step = SCAN;
         return -1;
      }/* if */

      DBFFindRecord(db, nfBC, ScanBuf);
      if(DBFGetErrorNum() != DBF_OK){
         if(DBFGetErrorNum() == DBF_ERROR_RECORD_NOT_FOUND){
            InfoMsg("Выезд ЗАПРЕЩЕН! "
                     "   Документ     "
                     "недействителен! ");
            RecordEvent(ErrorBCNotFound);
            Step = ERROROUT;
            return 0;
         }/* if */
         else{
            ErrorMsg(SysAdm);
            RecordEvent(ErrorGeneral2);
            Step = SCAN;
            return -1;
         }/* elif */
      }/* if */
/*
      NumRunning = DBFGetFieldAsInt(db, nfNumRunning);
      if(NumRunning == 0){
         InfoMsg("Въезд ЗАПРЕЩЕН! "
                 "Количество въе- "
                 "здов исчерпано");
         DBFClose(db);
         Step = ERROROUT;
         return 0;
      }/* if */

      if(DBFGetFieldAsDate(db, nfsOutPerson[4], &d)){
         if(numdate(&dt, &d) < 1){
            InfoMsg("Въезд запрещен! "
                    "Дата последнего "
                    "въезда прошла.  ");
            DBFClose(db);
            Step = ERROROUT;
            return 0;
         }/* if */
      }/* if */
      else{
         Step = SCAN;
         return -1;
      }/* elif */
*/
      strcpy(BCBuf, DBFGetFieldAsString(db, nfBC));

      GRNLen = strlen(DBFGetFieldAsString(db, nfGRN));
      GRNs = calloc(1U, GRNLen);
      if(GRNs){
         strcpy(GRNs, DBFGetFieldAsString(db, nfGRN));
      }/* if */
      else{
         ErrorMsg(SysAdm);
         RecordEvent(ErrorGeneral3);
         Step = SCAN;
         return -1;
      }/* elif */

      Step = GRNINPUT;
      return 0;
   }/* if */

   if(Step == GRNINPUT){
      del_pic(&p);
      load_file(&p, "tbg\\regnum1.tbg");
      outpic_all(p);
      stringedit[0] = 0;
      Edit_Init(p,editobj,editnum,stringedit, strlen_obj_num(p,editobj,editnum) + 1);
      Refresh_Display(p,editobj,editnum);
      EditLine_obj_num(p,editobj,editnum);
      Step = WAITGRNINPUT;
      return 0;
   }/* if */

   if(Step == WAITGRNINPUT){
      OutTime(p);
      switch(EditLine_obj_num(p,editobj,editnum)){
         case OK      :
            Step = COMPAREGRN;
            return 0;
         case CONTINUE:
            return 0;
         case HELP    :
            return 0;
      }//switch
      return 0;
   }/* if */

   if(Step == COMPAREGRN){
      char *ss = Edit_Stop(p,editobj,editnum);
      RecordGRN(ss);
      if((strlen(ss) > 1) && (strstr(GRNs, ss))){
         if(GRNs){
            free(GRNs);
         }/* if */
         Step = OUTINFO;
         return 0;
/*
         dbParking = DBFOpen(3); // Стоянка
         DBFFindRecord(dbParking, nfGRN, ss);
         if(DBFGetErrorNum() != DBF_OK){
            if(DBFGetErrorNum() == DBF_ERROR_RECORD_NOT_FOUND){
               DBFClose(dbParking);
               Step = OUTINFO;
               return 0;
            }/* if */
            DBFClose(dbParking);
            return -1;
         }/* if */
         else{
            if(strcmp(DBFGetFieldAsString(dbParking, nfInUse), keyInUse) == 0){
               char buf[16 * 3];
               sprintf(buf, "  На стоянке    "
                            "   уже есть     "
                            ">>>>%s<<<<", DBFGetFieldAsString(dbParking, nfGRN));
               InfoMsg(buf);
               DBFClose(dbParking);
               DBFClose(db);
               Step = ERROROUT;
               return 0;
            }/* if */
            DBFClose(dbParking);
         }/* elif */
*/
      }/* if */
      else{
         Step = GRNINPUT;
         iGRN++;
         if(iGRN >= 2){
            ErrorMsg("Выезд ЗАПРЕЩЕН! "
                     " Неправильный   "
                     "     ГРН!       ");
            RecordEvent(ErrorInputREGNUM);
            if(GRNs){
               free(GRNs);
            }/* if */
            DBFClose(db);
            Step = ERROROUT;
         }/* if */
      }/* elif */
      return 0;
   }/* if */

   // biostime();

   if(Step == OUTINFO){
      int i;
      del_pic(&p);
      load_file(&p, "tbg\\person42.tbg");
      outpic_all(p);

      getmin_obj(p, 99, (short int*)&NOs);
      getmax_obj(p, 99, (short int*)&NOe);
      NOc = NOs;

      for(i = NOs;i <= NOe;i++){
         switch(i - NOs){
            case 3:
            case 4:
               if(DBFGetFieldAsDate(db, nfsOutPerson[i - NOs], &d)){
                  sprintf(stringedit, "%2.d-%2.2d-%4.4d", d.da_day, d.da_mon, d.da_year);
                  outtext_obj_num(p, i, 0, EGA_DARKGRAY, stringedit, SHOW);
/*
                  if((i - NOs == 4) && (numdate(&dt, &d) <= 1)){
                     BlinkObject[0] = i;
                  }/* if */
*/
               }/* if */
               break;
/*
            case 5:

               outtext_obj_num(p, i, 0, EGA_DARKGRAY, DBFGetFieldAsString(db, nfsOutPerson[i - NOs]), SHOW);
               NumRunning = DBFGetFieldAsInt(db, nfsOutPerson[i - NOs]);
               if(NumRunning <= 1){
                  BlinkObject[1] = i;
               }/* if */
               break;
            case 6:
               outtext_obj_num(p, i, 0, EGA_DARKGRAY, PlaceNum, SHOW);
               break;
*/
            default:
               outtext_obj_num(p, i, 0, EGA_DARKGRAY, DBFGetFieldAsString(db, nfsOutPerson[i - NOs]), SHOW);
               break;
         }/* switch */
      }/* for */
      DBFClose(db);
      Step = WAITQSL;
      return 0;
   }/* if */

   if(Step == WAITQSL){
      OutTime(p);
      {
         int BlinkIndex;
         static long BlinkTime = 0;
         static int Blink = 0;
         if(((bt - BlinkTime) / CLK_TCK) > .25F){
            BlinkTime = bt;
            for(BlinkIndex = 0;BlinkIndex < sizeof(BlinkObject) / sizeof(BlinkObject[0]);BlinkIndex++){
               if(BlinkObject[BlinkIndex]){
                  if(Blink){
                     outpic_obj_num_color(p, BlinkObject[BlinkIndex], 0, EGA_YELLOW);
                  }/* if */
                  else{
                     outpic_obj_num(p, BlinkObject[BlinkIndex], 0);
                  }/* elif */
               }/* if */
            }/* for */
            Blink = !Blink;
         }/* if */

      }
      if(Key == ENTER){
         del_pic(&p);
         Step = SHB;
      }/* if */
      if(ExtKey == END){
         InfoMsg("Выезд ЗАПРЕЩЕН! ");
         RecordEvent(StopProcedureINPUT);
         Step = ERROROUT;
      }/* if */
      return 0;
   }/* if */

   if(Step == SHB){
      Step = WAITSHB;
      return 0;
   }/* if */

   if(Step == WAITSHB){
      switch(WorkSHB()){
         case -1: // error
            Step = SCAN;
            return -1;
         case  0: // wait
            break;
         case  1: // ok
            {
               char tGRN[16];
               db = DBFOpen(12); // Пропуска
               DBFFindRecord(db, nfBC, BCBuf);
               if(DBFGetErrorNum() != DBF_OK){
                  DBFClose(db);
                  return -1;
               }/* if */
               else{
/*
                  strcpy(tGRN, DBFGetFieldAsString(db, nfGRN));
                  NumRunning = DBFGetFieldAsInt(db, nfNumRunning);
                  NumRunning--;
                  DBFSetFieldAsInt(db, nfNumRunning, NumRunning);
*/
                  DBFSetFieldAsString(db, nfPlaceNum, "");
                  DBFPutRecord(db);
                  DBFClose(db);
               }/* elif */

               dbParking = DBFOpen(3); // Стоянка
               DBFFindRecord(dbParking, nfPlaceNum, PlaceNum);
               if(DBFGetErrorNum() != DBF_OK){
                  DBFClose(dbParking);
                  return -1;
               }/* if */
               else{
                  DBFSetFieldAsString(dbParking, nfInUse, keyInFree);
                  DBFSetFieldAsString(dbParking, nfGRN, " ");
                  DBFPutRecord(dbParking);
                  DBFClose(dbParking);
               }/* elif */

            }
            Step = STOPPROGRAM;
            return 1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == STOPPROGRAM){
      Step = SCAN;
      return 1;
   }/* if */

   if(Step == ERROR){
      OutTime(p);
      if(Key == ENTER){
         del_pic(&p);
         Step = SCAN;
      }/* if */
      return 0;
   }/* if */

   if(Step == ERROROUT){
      OutTime(p);
      if(Key == ENTER){
         InfoMsg(NULL);
         del_pic(&p);
         Step = SCAN;
         return 1;
      }/* if */
      return 0;
   }/* if */

   return -1;

}/* Post4_3out */
/* ----------------------------------------------------------------------- */
