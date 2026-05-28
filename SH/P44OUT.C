/* -----------------------------------------------------------------------
*
*  P4_4out.C
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
      WAITINFO,
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
static int iNOs, iNOe, iNOc;
static int NumRunning;
static char *nfPlaceNum    = "NМЕСТА",
            *nfUseParking  = "НАСТОЯНКЕ",
            *nfCarCategory = "ЛГ",
            *nfInUse       = "ЗАНЯТО",
            *nfGRN         = "ГРН",
            *nfBC          = "ШК",
            *nfNumRunning  = "ОСТАЛОСЬ" ,
            *keyInFree     = "0",
            *keyInUse      = "1",
            *keyInMark     = "2",
             PlaceNum[16],
             NumRunningBuf[6],
             BCBuf[16],
            *nfsOutPerson[]={
"ФИО",      // 0
"NУДЛ",     // 1
"МАРКА",    // 2
"СВД",      // 3
"NПОДР"     // 4
},
            *nfsInpPerson[]={
"ИНФО"      // 0
};

#define FSIZE 4
static char* Formats[][FSIZE] = {
{Info0, ABCRus, CtrKey, NULL  }, // 0 Info
{Info3, ABCRus, DecDig, CtrKey}  // грн
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
// выезд по служ. стояночному талону
int Post4_4out(void){

   int D, M, Y;
   struct date d;

   if(Step == SCAN){
      load_file(&p, "tbg\\scan1out.tbg");
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
            MessageBox("Свободных мест  "
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
      db = DBFOpen(13); // Служебный стояночный талон
      if(db == NULL){
         MessageBox("Ошибка открытия БД", "БД 'Сл. стояночный талон' не открывается.");
         RecordEvent(ErrorGeneral1);
         Step = ERROROUT;
         return 0;
      }/* if */

      DBFFindRecord(db, nfBC, ScanBuf);
      if(DBFGetErrorNum() != DBF_OK){
         if(DBFGetErrorNum() == DBF_ERROR_RECORD_NOT_FOUND){
            DBFClose(db);
            MessageBox("Выезд по сл. стояночному талону ЗАПРЕЩЁН!", "Документ недействителен!");
            RecordEvent(ErrorBCNotFound);
            Step = ERROROUT;
            return 0;
         }/* if */
         else{
            if(DBFMsgError(13) == 0){
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

      if(DBFGetFieldAsString(db, nfUseParking)[0] == keyInFree[0]){
         MessageBox("Стоянка", "Выезд по сл. стояночному талону запрещен!\nНет на стоянке.");
         DBFClose(db);
         Step = ERROROUT;
         return 0;
      }/* if */

      strcpy(BCBuf, DBFGetFieldAsString(db, nfBC));
      strcpy(PlaceNum, DBFGetFieldAsString(db, nfPlaceNum));

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
      load_file(&p, "tbg\\reg1out.tbg");
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
         if(CheckFieldsFormat(Formats, 1, 2, Key) == FALSE){
            MessageBox("Фильтр ввода", Formats[1][0]);
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
               MessageBox(buf);
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
         if(iGRN >= InpGRN){
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
      load_file(&p, "tbg\\p45out.tbg");
      outpic_all(p);
      // взять номера объектов для вывода
      getmin_obj(p, 99, (short int*)&NOs);
      getmax_obj(p, 99, (short int*)&NOe);
      NOc = NOs;
      // взять номера объектов для ввода
      getmin_obj(p, 88, (short int*)&iNOs);
      getmax_obj(p, 88, (short int*)&iNOe);
      iNOc = iNOs;

      // all input field clear
      for(i = iNOs;i <= iNOe;i++){
         *(gettext_obj_num(p, i, 0)) = '\0';
      }/* for */

      for(i = NOs;i <= NOe;i++){
         switch(i - NOs){
            default:
               outtext_obj_num(p, i, 0, EGA_DARKGRAY, DBFGetFieldAsString(db, nfsOutPerson[i - NOs]), SHOW);
               break;
         }/* switch */
      }/* for */

      Edit_Init(p,iNOc,0, DBFGetFieldAsString(db, nfsInpPerson[iNOc - iNOs]), DBFGetFieldLength(db, nfsInpPerson[iNOc - iNOs]) + 1);
      Refresh_Display(p,iNOc,0);
      EditLine_obj_num(p,iNOc,0);

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

      }// local block end

      switch(ExtKey){
         case 0:
            break;
         case END:
            Edit_Stop(p,iNOc,0);
            MessageBox("Выезд по  сл. стояночному талону", "Выезд отменён оператором");
            RecordEvent(StopProcedureINPUT);
            DBFClose(db);
            Step = ERROROUT;
            break;
         case F2:
            if(getcolor_obj_num(p, iNOc, 0) != EGA_LIGHTGREEN){
               MessageBox("Ввод формы", "Введите дополнительную информацию");
               SaveStep = Step;
               Step = WAITINFO;
               break;
            }/* if */
            Step = SHB;
            break;
      }/* switch */

      switch(Key){
         case 0:
            if(ExtKey == 0)
               break;
         default:
            // проверить вводимые символы на допустимость
            // и выдать ИнфоМсг если ошибка
            if(CheckFieldsFormat(Formats, NOc - NOs, sizeof(nfsInpPerson) / sizeof(nfsInpPerson[0]), Key) == FALSE){
               MessageBox("Фильтр ввода", Formats[NOc - NOs][0]);
               SaveStep = Step;
               Step = WAITINFO;
               break;
            }/* if */
            switch(EditLine_obj_num(p,iNOc,0)){
               case OK      :
                  DBFSetFieldAsString(db, nfsInpPerson[iNOc - iNOs], Edit_Stop(p,iNOc,0));
                  setcolor_obj_num(p, iNOc, 0, EGA_LIGHTGREEN, SHOW);
                  DBFPutRecord(db);
                  DBFClose(db);
                  break;
               case CONTINUE:
                  break;
               case HELP    :
                  break;
            }//switch
            break;
      }/* switch */
      return 0;
   }/* if */

   if(Step == SHB){
      del_pic(&p);
      Step = WAITSHB;
      return 0;
   }/* if */

   if(Step == WAITSHB){
      switch(WorkSHBOut()){
         case -1: // error
            Step = SCAN;
            return -1;
         case  0: // wait
            break;
         case  1: // ok
            {
               char tGRN[16];
               db = DBFOpen(13); // Пропуска
               DBFFindRecord(db, nfBC, BCBuf);
               if(DBFGetErrorNum() != DBF_OK){
                  DBFClose(db);
               }/* if */
               else{
                  DBFSetFieldAsDate(db, "ПО", &dt);
                  DBFSetFieldAsTime(db, "ПОВРЕМЯ", &tm);
                  DBFSetFieldAsString(db, nfPlaceNum, "");
                  DBFSetFieldAsString(db, nfUseParking, keyInFree);
                  DBFPutRecord(db);
                  DBFClose(db);
               }/* elif */

               dbParking = DBFOpen(3); // Стоянка
               DBFFindRecord(dbParking, nfPlaceNum, PlaceNum);
               if(DBFGetErrorNum() != DBF_OK){
                  DBFClose(dbParking);
               }/* if */
               else{
                  DBFSetFieldAsString(dbParking, nfInUse, keyInFree);
                  DBFSetFieldAsString(dbParking, nfGRN, "");
                  DBFPutRecord(dbParking);
                  DBFClose(dbParking);
               }/* elif */

            }
            Step = STOPPROGRAM;
            return 0;
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

   // ждать реакции пользователя на
   // информационное сообщение
   if(Step == WAITINFO){
      // показываем время
      OutTime(p);
      // нажал ENTER
      if(Key == ENTER){
         // вспомнить на каком шаге
         Step = SaveStep;
         // удалить окно сообщения с восстановлением под ним картинки
         MessageBox(NULL, NULL);
      }/* if */
      return 0;
   }/* if */

   return -1;

}/* Post4_4out */
/* ----------------------------------------------------------------------- */
