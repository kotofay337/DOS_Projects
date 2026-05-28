/* -----------------------------------------------------------------------
*
*  Post4_4.C
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
            *nfUseParking  = "НАСТОЯНКЕ",
            *nfCarCategory = "ЛГ",
            *nfInUse       = "ЗАНЯТО",
            *nfGRN         = "ГРН",
            *nfBC          = "ШК",
            *nfNumRunning  = "ОСТАЛОСЬ" ,
            *nfNumAllRunning  = "ВСЕГО" ,
            *nfLastRunning = "ПДВЪЕЗДА" ,
            *keyInFree     = "0",
            *keyInUse      = "1",
            *keyInMark     = "2",
             PlaceNum[16],
             NumRunningBuf[6],
             BCBuf[16],
            *nfsOutPerson[]={
"ФИО",      // 0
"МАРКА",    // 1
"СВД",      // 2
"С",        // 3
"ПО",       // 4
"ОСТАЛОСЬ", // 5
"NМЕСТА",   // 6
"NВЪЕЗДА",  // 7
"КАТЕГОРИЯ" // 8
};

#define FSIZE 4
static char* Formats[][FSIZE] = {
{Info3, ABCRus, DecDig, CtrKey} // грн
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
// въезд по Пропуску
int Post4_4(void){

   int D, M, Y;
   struct date d;
   struct time t;

   if(Step == SCAN){
      load_file(&p, "tbg\\scanner1.tbg");
      outpic_all(p);
      ClearScan = TRUE;
      iGRN = 0;
      BlinkObject[0] = 0;
      BlinkObject[1] = 0;

      db = DBFOpen(3); // Стоянка

      DBFFindRecord(db, nfInUse, keyInFree);
      if(DBFGetErrorNum() != DBF_OK){
         if(DBFGetErrorNum() == DBF_ERROR_RECORD_NOT_FOUND){
            MessageBox("Стоянка", "Извините! Свободных мест для\nвъезда по пропуску НЕТ!");
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
            DBFClose(db);
            MessageBox("Въезд по пропуску ЗАПРЕЩЁН!", "Документ недействителен!");
            RecordEvent(ErrorBCNotFound);
            Step = ERROROUT;
            return 0;
         }/* if */
         else{
            if(DBFMsgError(12) == 0){
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

      DBFGetFieldAsDate(db, "С", &d);
      DBFGetFieldAsTime(db, "СВРЕМЯ", &t);
      if(CheckAfterDate(&dt, &tm, &d, &t) == TRUE){
         MessageBox("Въезд по пропуску", "Въезд ещё не разрешен!");
         DBFClose(db);
         Step = ERROROUT;
         return 0;
      }/* if */

      // проверить место
      if(strcmp(DBFGetFieldAsString(db, nfUseParking), keyInUse) == 0){
         MessageBox("Стоянка", "Место занято!");
         DBFClose(db);
         Step = ERROROUT;
         return 0;
      }/* if */

      switch(CheckNGD(DBFGetFieldAsString(db, "NВЪЕЗДА"), NPost, GetDateType(NULL))){
         case -1:
         case -2:
            MessageBox("Ошибка системы", "Ошибка в описании номеров въездов\nи/или типов дней!");
            DBFClose(db);
            Step = ERROROUT;
            return 0;
         case 0:
            MessageBox("Въезд по пропуску ЗАПРЕЩЁН!",
                       "Номер въезда не совпадает\nни с одним разрешённым\nдля этого пропуска!");
            DBFClose(db);
            Step = ERROROUT;
            return 0;
         case 2:
            MessageBox("Въезд по пропуску", "Разрешается\n"
                                            "но не в сегодняшний день.");
            DBFClose(db);
            Step = ERROROUT;
            return 0;
      }/* switch */

      {// lockblock
         char ldate[16], tdate[16];

         strcpy (ldate, DBFGetFieldAsString(db, nfLastRunning));
         sprintf(tdate, "%4.4d%2.2d%2.2d", dt.da_year, dt.da_mon, dt.da_day);
         // въезжал последний раз не сегодня?
         //  тогда обновить количество оставшихся въездов
         if(strcmp(ldate, tdate) != 0){
            NumRunning = DBFGetFieldAsInt(db, nfNumAllRunning);
            DBFSetFieldAsInt(db, nfNumRunning, NumRunning);
            DBFSetFieldAsString(db, nfLastRunning, tdate);
            DBFPutRecord(db);
         }/* if */
      }// lockblock

      NumRunning = DBFGetFieldAsInt(db, nfNumRunning);
      if(NumRunning == 0){
         MessageBox("Въезд по пропуску ЗАПРЕЩЁН!",
                    "Количество въездов исчерпано");
         DBFClose(db);
         Step = ERROROUT;
         return 0;
      }/* if */

      if(DBFGetFieldAsDate(db, nfsOutPerson[4], &d)){
         if(NumDays(&dt, &d) < 0){
         MessageBox("Въезд по пропуску ЗАПРЕЩЁН!",
                    "Документ просрочен!");
            DBFClose(db);
            Step = ERROROUT;
            return 0;
         }/* if */
      }/* if */
      else{
         Step = SCAN;
         return -1;
      }/* elif */

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
/* расширение
      if(GRNs[0] == '*'){
         Step = OUTINFO;
      }/* if */
*/
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
      if(Key){
         // проверить вводимые символы на допустимость
         // и выдать ИнфоМсг если ошибка
         if(CheckFieldsFormat(Formats, 0, 1, Key) == FALSE){
            MessageBox("Фильтр ввода", Formats[0][0]);
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
         return 0;
      }/* if */
      else{
         Step = GRNINPUT;
         iGRN++;
         if(iGRN >= InpGRN){
            ErrorMsg("Въезд ЗАПРЕЩЕН! "
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
      load_file(&p, "tbg\\person44.tbg");
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
                  if((i - NOs == 4) && (NumDays(&dt, &d) <= 1)){
                     BlinkObject[0] = i;
                  }/* if */
               }/* if */
               break;
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
            case 7:
               outtext_obj_num(p, i, 0, EGA_DARKGRAY, "", SHOW);
               GetGatesString(gettext_obj_num(p, i, 0),
                              DBFGetFieldAsString(db, nfsOutPerson[i - NOs]),
                              strlen_obj_num(p, i, 0));
               outpic_obj_num(p, i, 0);
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
         Step = SHB;
      }/* if */
      if(ExtKey == END){
         MessageBox("Въезд по пропуску", "Въезд отменён оператором");
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
                  strcpy(tGRN, DBFGetFieldAsString(db, nfGRN));
                  NumRunning = DBFGetFieldAsInt(db, nfNumRunning);
                  NumRunning--;
                  DBFSetFieldAsString(db, nfUseParking, keyInUse);
                  DBFSetFieldAsInt(db, nfNumRunning, NumRunning);
                  DBFSetFieldAsString(db, nfPlaceNum, PlaceNum);
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
                  DBFSetFieldAsString(dbParking, nfInUse, keyInUse);
                  DBFSetFieldAsString(dbParking, nfGRN, tGRN);
                  DBFPutRecord(dbParking);
                  DBFClose(dbParking);
               }/* elif */

            }
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
         MessageBox(NULL, NULL);
         del_pic(&p);
         Step = SCAN;
         return 1;
      }/* if */
      return 0;
   }/* if */

   return -1;

}/* Post4_4 */
/* ----------------------------------------------------------------------- */
