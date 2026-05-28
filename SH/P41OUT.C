/* -----------------------------------------------------------------------
*
*  P41out.C
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
      OUTINFO,
      DOLG,
      MONEYGET,
      WAITMONEYGET,
      WAITFR,
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
static int editobj = 100, editnum = 0, dolg = 0;
static OBJECT **m = NULL, **pd = NULL;

static int BlinkObject[2];
static int NOs, NOe, NOc;
static int NumRunning;
static char nfPlaceNum    []= "NМЕСТА",
            nfUseParking  []= "НАСТОЯНКЕ",
            nfFIO         []= "ФИО",
            nfCarCategory []= "ЛГ",
            nfMarka       []= "МАРКА",
            nfInUse       []= "ЗАНЯТО",
            nfGRN         []= "ГРН",
            nfBC          []= "ШК",
            nfNumRunning  []= "ОСТАЛОСЬ" ,
            keyInFree     []= "0",
            keyInUse      []= "1",
            keyInMark     []= "2",
            PlaceNum[16],
            NumRunningBuf[6],
            BCBuf[16],
            GRN[16],
            *nfsOutPerson[]={
nfFIO,      // 0
nfGRN,      // 1
nfMarka,    // 2
"СВД",      // 3
"С",        // 4
"ПО",       // 5
nfNumRunning // 6
};

static struct date EndDate;
static struct time EndTime;
/* ----------------------------------------------------------------------- */
// выезд по талону
int Post4_1out(void){

   int D, M, Y;
   struct date d;

   if(Step == SCAN){
      load_file(&p, "tbg\\scan1out.tbg");
      outpic_all(p);
      ClearScan = TRUE;
      BlinkObject[0] = 0;
      BlinkObject[1] = 0;
      Step = WAITSCAN;
      dolg = 0;
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
            RecordEvent(Scanned == TRUE ? ScannerBCInput : ManualBCInput);
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
      db = DBFOpen(8);// стояночные талоны
      if(db == NULL){
         ErrorMsg(SysAdm);
         RecordEvent(ErrorGeneral1);
         Step = SCAN;
         del_pic(&p);
         return -1;
      }/* if */

      DBFFindRecord(db, nfBC, ScanBuf);
      if(DBFGetErrorNum() != DBF_OK){
         if(DBFGetErrorNum() == DBF_ERROR_RECORD_NOT_FOUND){
            DBFClose(db);
            MessageBox("Выезд по стояночному талону ЗАПРЕЩЕН!",
                       "Документ недействителен! ");
            RecordEvent(ErrorBCNotFound);
            DBFClose(db);
            Step = ERROROUT;
            return 0;
         }/* if */
         else{
            if(DBFMsgError(8) == 0){
               DBFClose(db);
               Step = ERROROUT;
               return 0;
            }/* if */
            else{
               DBFClose(db);
               ErrorMsg(SysAdm);
               RecordEvent(ErrorGeneral2);
               del_pic(&p);
               Step = SCAN;
               return -1;
            }/* elif */
         }/* elif */
      }/* if */

      strcpy(PlaceNum, DBFGetFieldAsString(db, nfPlaceNum));
      strcpy(BCBuf, DBFGetFieldAsString(db, nfBC));

      if(DBFGetFieldAsString(db, nfUseParking)[0] == keyInFree[0]){
         MessageBox("Выезд по стояночному талону", "Нет на стоянке!");
         DBFClose(db);
         Step = ERROROUT;
         return 0;
      }/* if */

      Step = OUTINFO;
      return 0;
   }/* if */

   if(Step == OUTINFO){
      int i;
      del_pic(&p);
      load_file(&p, "tbg\\p41out.tbg");
      outpic_all(p);

      getmin_obj(p, 99, (short int*)&NOs);
      getmax_obj(p, 99, (short int*)&NOe);
      NOc = NOs;

      for(i = NOs;i <= NOe;i++){
         switch(i - NOs){
            case 4:
            case 5:
               if(DBFGetFieldAsDateString(db, nfsOutPerson[i - NOs], &d, stringedit)){

                  outtext_obj_num(p, i, 0, EGA_DARKGRAY, stringedit, SHOW);
                  if(i - NOs == 5){
                     double t = 0.0;
                     DBFGetFieldAsDate(db, "ПО", &EndDate);
                     DBFGetFieldAsTime(db, "ПОВРЕМЯ", &EndTime);
                     if(CheckAfterDate(&EndDate, &EndTime, &dt, &tm)){
                        t = CalcTarif(TDTALON, TCPERESTOY, &EndTime, &EndDate, &tm, &dt);
                        if(t >= 0.01){// 1 коп.
                           // клиент задолжал 1 копейку и более
                           BlinkObject[0] = i;
                           load_file(&pd, "tbg\\dolg.tbg");
                           outpic_all(pd);
                           if(strcmp(DBFGetFieldAsString(db, nfCarCategory), "Л")  == 0){
                              cc = Service;
                           }/* if */
                           else{
                              cc = Commercial;
                           }/* elif */
                           sprintf(starif, ftarif, (unsigned)((long)t),
                                                   (unsigned)((t - ((long)t)) * 100.));
                           outtext_obj_num(pd, 200, 0, EGA_CYAN, starif, SHOW);
                           outtext_obj_num(pd, 201, 0, EGA_CYAN, cc == Service ? "Легковой" : "Грузовой", SHOW);
                           del_pic(&pd);
                           dolg = 1;
                        }/* if */
                     }/* if */
                  }/* if */
               }/* if */
               break;
            case 6:
               outtext_obj_num(p, i, 0, EGA_DARKGRAY, DBFGetFieldAsString(db, nfsOutPerson[i - NOs]), SHOW);
               NumRunning = DBFGetFieldAsInt(db, nfsOutPerson[i - NOs]);
               if(NumRunning < 1){
                  BlinkObject[1] = i;
               }/* if */
               break;
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
         if(dolg){
            dolg = 0;
            Step = MONEYGET;
            return 0;
         }/* if */
         Step = SHB;
      }/* if */
      if(ExtKey == END){
         MessageBox("Выезд по стояночному талону", "Выезд отменён оператором");
         RecordEvent(StopProcedureOUTPUT);
         Step = ERROROUT;
      }/* if */
      return 0;
   }/* if */

   if(Step == MONEYGET){
      load_file(&p, "tbg\\t41out.tbg");
      outpic_all(p);
      outtext_obj_num(p, 50, 1, getcolor_obj_num(p, 50, 0), starif, SHOW);
      Step = WAITMONEYGET;
      return 0;
   }/* if */

   if(Step == WAITMONEYGET){
      OutTime(p);
      if(Key == '*'){
         del_pic(&p);
         Step = WAITFR;
      }/* if */
      if(ExtKey == END){
         MessageBox("Оплата через ФР", "Отменена оператором");
         RecordEvent(StopProcedureOUTPUT);
         Step = ERROROUT;
      }/* if */
      return 0;
   }/* if */

   if(Step == WAITFR){
      OutTime(p);
      switch(PrintFR(Tarif)){
         case 1:// ok
            Step = SHB;
            RecordFRMoney(Tarif);
            RecordEvent(FRPrint);
            break;
         case 0:// wait
            break;
         case -1:// error
            del_pic(&p);
            Step = SCAN;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == SHB){
      Step = WAITSHB;
      return 0;
   }/* if */

   if(Step == WAITSHB){
      switch(WorkSHBOut()){
         case -1: // error
            del_pic(&p);
            Step = SCAN;
            return -1;
         case  0: // wait
            break;
         case  1: // ok
            {
               db = DBFOpen(8); // стояночные талоны
               DBFFindRecord(db, nfBC, ScanBuf);
               if(DBFGetErrorNum() == DBF_OK){
                  DBFSetFieldAsString(db, nfUseParking, keyInFree);// mark: free
                  DBFPutRecord(db);
               }/* elif */
               DBFClose(db);

               dbParking = DBFOpen(3); // стоянка
               DBFFindRecord(dbParking, nfPlaceNum, PlaceNum);
               if(DBFGetErrorNum() == DBF_OK){
                  DBFSetFieldAsString(dbParking, nfInUse, keyInMark);// забронировать
                  DBFSetFieldAsString(dbParking, nfGRN, "Талон");
                  DBFPutRecord(dbParking);
               }/* elif */
               DBFClose(dbParking);
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
         MessageBox(NULL, NULL);
         del_pic(&p);
         Step = SCAN;
         return 1;
      }/* if */
      return 0;
   }/* if */

   return -1;

}/* Post4_1out */
/* ----------------------------------------------------------------------- */
