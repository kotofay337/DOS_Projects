/* -----------------------------------------------------------------------
*
*  Post1_3.C
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

#undef DEBUG
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
      WAITINFO,
      SHB,
      WAITSHB,
      STOPPROGRAM,
      ERROROUT,
      ERROR
   }STEP;

static STEP Step = SCAN, SaveStep;
static LPDBF db;
static char stringedit[15];
static int editobj = 100, editnum = 0;
static OBJECT **m = NULL;

static int BlinkObject[2];
static int NumRunning, Type;
static char *GRNs;
static int   GRNLen;
static int iGRN = 0;
static int NOs, NOe, NOc;
static char nfGRN[]       = "ГРН",
            nfBC[]        = "ШК",
            nfName[]      = "ФИО",
            nfMarka[]     = "МАРКА",
            nfSWD[]       = "СВД",
            nfStartDate[] = "С",
            nfEndDate[]   = "ПО",
            nfPlaceNum[]  = "NМЕСТА",
            nfNumRunning[]= "ОСТАЛОСЬ",
            nfType[]      = "ТИП";

static char *nfsOutPerson[] = {
nfName,
nfMarka,
nfSWD,
nfStartDate,
nfEndDate,
nfNumRunning,
nfPlaceNum
};

#define FSIZE 5
static char* Formats[][FSIZE] = {
{Info3, ABCRus, DecDig, CtrKey, NULL} // грн
};
/* ----------------------------------------------------------------------- */
static BOOL CheckFieldsFormat (char *f[][FSIZE], int nf, int maxnf, int ikey){
   int j;
   if(nf < 0 || nf > maxnf){
      return FALSE;
   }/* if */
   for(j = 1;j < FSIZE;j++){
      if(f[nf][j]){
         if(strchr(f[nf][j], ikey) != NULL){
            return TRUE;
         }/* if */
      }/* if */
   }/* for */
   return FALSE;
}/* CheckFieldsFormat */
/* ----------------------------------------------------------------------- */
int Post1_3(void){

   if(Step == SCAN){
      load_file(&p, "tbg\\scanner1.tbg");
      outpic_all(p);
      ClearScan = TRUE;
      iGRN = 0;
      Step = WAITSCAN;
      return 0;
   }/* if */

   if(Step == WAITSCAN){
      OutTime(p);

      if(Key != 0){
#ifdef DEBUG
         if(Key == ESC){
            del_pic(&p);
            del_pic(&m);
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
#ifdef DEBUG
         if(Key == ESC){
            del_pic(&m);
            del_pic(&p);
            Step = SCAN;
            return 1;
         }/* if */
#endif
      switch(EditLine_obj_num(m,editobj,editnum)){
         case OK      :
            strcpy(ScanBuf, Edit_Stop(m, editobj, editnum));
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
            ErrorMsg("Въезд ЗАПРЕЩЕН! "
                     "   Документ     "
                     "недействителен! ");
            RecordEvent(ErrorBCNotFound);
            Step = ERROROUT;
            return 0;
         }/* if */
         else{
            if(DBFMsgError(12) == 0){
               Step = ERROROUT;
               return 0;
            }/* if */
            else{
               ErrorMsg(SysAdm);
               RecordEvent(ErrorGeneral2);
               del_pic(&p);
               Step = SCAN;
               return -1;
            }/* elif */
         }/* elif */
      }/* if */

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
#ifdef DEBUG
         if(Key == ESC){
            del_pic(&p);
            free(GRNs);
            DBFClose(db);
            Step = SCAN;
            return 1;
         }/* if */
#endif
      if(Key){
         // проверить вводимые символы на допустимость
         // и выдать ИнфоМсг если ошибка
         if(CheckFieldsFormat(Formats, 0, 1, Key) == FALSE){
            MessageBox("Фильтр вводимых символов", Formats[0][0]);
            SaveStep = Step;
            Step = WAITINFO;
            return 0;
         }/* if */
      }/* if */
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
      if((strlen(ss) > 1) && (CheckGRN(GRNs, ss))){
         if(GRNs){
            free(GRNs);
         }/* if */
         Step = OUTINFO;
      }/* if */
      else{
         Step = GRNINPUT;
         iGRN++;
         if(iGRN >= InpGRN){
            ErrorMsg("Въезд ЗАПРЕЩЕН! "
                     " Неправильный   "
                     "     ГРН!       ");
            RecordEvent(ErrorInputREGNUM);
            Step = ERROROUT;
         }/* if */
      }/* elif */
      return 0;
   }/* if */

   // biostime();

   if(Step == OUTINFO){
      int i;
      del_pic(&p);
      load_file(&p, "tbg\\person2.tbg");
      outpic_all(p);

      getmin_obj(p, 99, (short int*)&NOs);
      getmax_obj(p, 99, (short int*)&NOe);
      NOc = NOs;

      for(i = NOs;i <= NOe;i++){
         struct date d;
         switch(i - NOs){
            case 3:
            case 4:
               if(DBFGetFieldAsDateString(db, nfsOutPerson[i - NOs], &d, stringedit)){
                  outtext_obj_num(p, i, 0, EGA_DARKGRAY, stringedit, SHOW);
                  if((i - NOs == 4) && (NumDays(&dt, &d) < 1)){
                     BlinkObject[0] = i;
                  }/* if */
               }/* if */
               break;
            case 5:
               NumRunning = DBFGetFieldAsInt(db, nfsOutPerson[i - NOs]);
               if(NumRunning < 1){
                  BlinkObject[1] = i;
               }/* if */
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
#ifdef DEBUG
         if(Key == ESC){
            del_pic(&p);
            Step = SCAN;
            return 1;
         }/* if */
#endif
      {
         int BlinkIndex;
         static long BlinkTime = 0;
         static int Blink = 0;
         if(((bt - BlinkTime) / CLK_TCK) > .25F){
            BlinkTime = bt;
            for(BlinkIndex = 0;BlinkIndex < sizeof(BlinkObject) / sizeof(BlinkObject[0]);BlinkIndex++){
               if(BlinkObject[BlinkIndex]){
                  if(Blink){
                     outpic_obj_num_color(p, BlinkObject[BlinkIndex], 0, EGA_LIGHTRED);
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
         ErrorMsg("Въезд ЗАПРЕЩЕН! ");
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
            db = DBFOpen(12); // Пропуска
            if(db == NULL){
               ErrorMsg(SysAdm);
               RecordEvent(ErrorGeneral1);
               Step = SCAN;
               return -1;
            }/* if */

            DBFFindRecord(db, nfBC, ScanBuf);
            if(DBFGetErrorNum() != DBF_OK){
                  ErrorMsg(SysAdm);
                  RecordEvent(ErrorGeneral2);
                  Step = SCAN;
                  return -1;
            }/* if */
            {
               unsigned n = DBFGetFieldAsInt(db, nfNumRunning);
               n--;
               DBFSetFieldAsInt(db, nfNumRunning, n);
               DBFPutRecord(db);
            }
            DBFClose(db);
            Step = STOPPROGRAM;
            return 1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WAITINFO){
      OutTime(p);
      if(Key == ENTER){
         Step = SaveStep;
         MessageBox(NULL, NULL);
      }/* if */
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
         del_pic(&p);
         Step = SCAN;
         return 1;
      }/* if */
      return 0;
   }/* if */

   return -1;

}/* Post1_3 */
/* ----------------------------------------------------------------------- */
