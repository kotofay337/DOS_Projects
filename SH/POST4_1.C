/* -----------------------------------------------------------------------
*
*  Post4_1.C
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
#include <alloc.h>
#include "tbg_tc.h"
#include "dbf.h"
#include "com.h"
#include "data.h"
#include "editstr.h"

#line 24
/* ----------------------------------------------------------------------- */
typedef
   enum{
      DETECTFREEPLACE,
      CARCATEGORY,
      WAITINPUTCARCATTEGORY,
      INPUTPERSON,
      WAITINPUTPERSON,
      NEXTFIELD,
      CALCTARIF,
      MONEYGET,
      WAITMONEYGET,
      PRINTBC,
      PRINTFR,
      SHB,
      WAITSHB,
      WAITQSL,
      WAITINFO,
      STOPPROGRAM
   }STEP;

static STEP Step = DETECTFREEPLACE, SaveStep;
static LPDBF db, dbParking;
static int NOs, NOe, NOc;

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
             PlaceNum[ 16],
                  GRN[ 16],
                  FIO[ 32],
                 Date[ 32],
                 Time[ 16],
            *nfsInpPerson[]={
"ФИО",      // 0  а-я .
"NТЕЛ",     // 1  0-9 -
"ГРН",      // 2  0-9 а-я
"МАРКА",    // 3  а-я a-z
"СВД",      // 4  0-9 а-я
"С",        // 5  0-9 -
"СВРЕМЯ",   // 6  0-9 :
"ПЕРИОД",   // 7  0-9
"ПО",       // 8  0-9 -
"ПОВРЕМЯ",  // 9  0-9 :
"NМЕСТА"    // 10 0-9
};

static struct date StartDate, EndDate;
static struct time StartTime, EndTime;

static int NumRunning, Hr, Mn, Hours;
static long BC;

#define FSIZE 5
static char* Formats[][FSIZE] = {
{Info0, ABCRus, CtrKey, NULL  , NULL}, // 0 ФИО
{Info1, DecDig, Sep1  , CtrKey, NULL}, // 1 НТЕЛ
{Info3, ABCRus, DecDig, CtrKey, NULL}, // 2 грн
{Info2, ABCRus, ABCEng, CtrKey, DecDig}, // 3 Марка
{Info3, ABCRus, DecDig, CtrKey, NULL}, // 4 свд
{Info1, DecDig, Sep1  , CtrKey, NULL}, // 5 с
{Info5, DecDig, Sep2  , CtrKey, NULL}, // 6 свремя
{Info6, DecDig, NULL  , CtrKey, NULL}, // 7 период
{Info1, DecDig, Sep1  , CtrKey, NULL}, // 8 по
{Info5, DecDig, Sep2  , CtrKey, NULL}, // 9 повремя
{Info3, DecDig, ABCRus, CtrKey, NULL}  // 10 НМеста
};

static int hc;
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
// оформление стояночного талона и въезд по нему
int Post4_1(void){
   int i;
   char DateBuf[9], BCBuf[16], NumRunningBuf[8];
   int D, M, Y;

   if(Step == DETECTFREEPLACE){


      if(dt.da_year % 4 == 0){
         NumDayInMonth[1] = 29;
      }/* if */
      else{
         NumDayInMonth[1] = 28;
      }/* elif */

      load_file(&p, "tbg\\timebar.tbg");
      outpic_all(p);

      db = DBFOpen(3); // Стоянка

      DBFFindRecord(db, nfInUse, keyInFree);
      if(DBFGetErrorNum() != DBF_OK){
         if(DBFGetErrorNum() == DBF_ERROR_RECORD_NOT_FOUND){
            MessageBox("Стоянка", "Свободных мест НЕТ!");
            DBFClose(db);
            SaveStep = Step;
            Step = WAITINFO;
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

      Step = CARCATEGORY;
      return 0;
   }/* if */

   if(Step == CARCATEGORY){

      del_pic(&p);
      load_file(&p, "tbg\\kateg1.tbg");
      outpic_all(p);
      Step = WAITINPUTCARCATTEGORY;
      return 0;
   }/* if */

   if(Step == WAITINPUTCARCATTEGORY){
      OutTime(p);
      switch(Key){
         case '1':
            cc = Service;
            Step = INPUTPERSON;
            break;
         case '2':
            cc = Commercial;
            Step = INPUTPERSON;
            break;
      }/* switch */
      return 0;
   }/* if */

   if(Step == INPUTPERSON){

      del_pic(&p);

      db = DBFOpen(8); // Стояночные талоны 4-й стоянки

      load_file(&p, "tbg\\person41.tbg");

      getmin_obj(p, 99, (short int*)&NOs);
      getmax_obj(p, 99, (short int*)&NOe);
      NOc = NOs;

      // all field clear
      for(i = NOs;i <= NOe;i++){
         *(gettext_obj_num(p, i, 0)) = '\0';
      }/* for */

      outpic_all(p);

      outtext_obj_num(p, 201, 0, EGA_LIGHTGRAY, PlaceNum, SHOW);

      outtext_obj_num(p, 202, 0, EGA_LIGHTGRAY, cc == Service ? "Легковой" : "Грузовой", SHOW);


      Edit_Init(p,NOc,0, DBFGetFieldAsString(db, nfsInpPerson[NOc - NOs]), DBFGetFieldLength(db, nfsInpPerson[NOc - NOs]) + 1);
      Refresh_Display(p,NOc,0);
      EditLine_obj_num(p,NOc,0);

      Step = WAITINPUTPERSON;
      return 0;
   }/* if */

   if(Step == WAITINPUTPERSON){
      OutTime(p);
      switch(ExtKey){
         case 0:
            break;
         case F2:

            // all field set?
            for(i = NOs;i <= NOe;i++){
               if((getcolor_obj_num(p, i, 0) != EGA_LIGHTGREEN) ||
                  (strlen(gettext_obj_num(p, i, 0)) == 0)){
                  MessageBox("Ввод формы", "Введите все поля формы");
                  SaveStep = Step;
                  Step = WAITINFO;
                  return 0;
               }/* if */
            }/* for */

            DBFGetFieldAsDate(db, nfsInpPerson[5], &StartDate);
            DBFGetFieldAsTime(db, nfsInpPerson[6], &StartTime);
            DBFGetFieldAsDate(db, nfsInpPerson[8], &EndDate);
            DBFGetFieldAsTime(db, nfsInpPerson[9], &EndTime);

            if(CheckAfterDate(&EndDate, &EndTime, &StartDate, &StartTime) == TRUE){
               MessageBox("Ввод формы", "Ошибка в дате или времени,\n"
                                        "поле ПО: д.б. быть позже чем поле С:");
               SaveStep = Step;
               Step = WAITINFO;
               return 0;
            }/* if */

            Edit_Stop(p,NOc,0);

            strcpy(FIO, DBFGetFieldAsString(db, nfsInpPerson[0]));
            DBFGetFieldAsDateString(db, nfsInpPerson[8], &dt, Date);
            DBFGetFieldAsTimeString(db, nfsInpPerson[9], &tm, Time);
            strcat(Date, " ");
            strcat(Date, Time);

            DBFSetFieldAsString(db, nfCarCategory, cc == Service ? "Л" : "Г");
            DBFSetFieldAsString(db, nfPlaceNum, PlaceNum);
            DBFSetFieldAsString(db, nfUseParking, keyInFree);
            DBFSetFieldAsInt(db, nfNumRunning, TNr);
            BC = CalcBC(9);
            DBFSetFieldAsLong(db, nfBC, BC);
            DBFAppendRecord(db);
            DBFClose(db);

            dbParking = DBFOpen(3); // Стоянка
            DBFFindRecord(dbParking, nfPlaceNum, String_Edit);
            if(DBFGetErrorNum() != DBF_OK){
               DBFClose(dbParking);
               if(DBFGetErrorNum() != DBF_ERROR_RECORD_NOT_FOUND){
                  if(DBFMsgError(3) == 0){
                     SaveStep = Step;
                     Step = WAITINFO;
                     return 0;
                  }/* if */
                  else{
                     ErrorMsg(SysAdm);
                     RecordEvent(ErrorGeneral2);
                     del_pic(&p);
                     Step = DETECTFREEPLACE;
                     return -1;
                  }/* elif */
               }/* if */
            }/* if */
            else{

               // установить
               DBFSetFieldAsString(dbParking, nfInUse, keyInMark);
               DBFSetFieldAsString(dbParking, nfGRN, "Талон");
               DBFPutRecord(dbParking);

               DBFClose(dbParking);

            }/* elif */

            Step = CALCTARIF;
            break;
      }/* switch */
      switch(Key){
         case ESC:
            DBFClose(db);
            Step = STOPPROGRAM;
            break;
         case TAB:
            Step = NEXTFIELD;
            Edit_Stop(p,NOc,0);
            break;
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
            switch(EditLine_obj_num(p,NOc,0)){
               case OK      :
                  // N места на стоянке
                  if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[10]) == 0){
                     dbParking = DBFOpen(3); // Стоянка
                     DBFFindRecord(dbParking, nfPlaceNum, String_Edit);
                     if(DBFGetErrorNum() != DBF_OK){
                        DBFClose(dbParking);
                        if(DBFGetErrorNum() != DBF_ERROR_RECORD_NOT_FOUND){
                           if(DBFMsgError(3) == 0){
                              SaveStep = Step;
                              Step = WAITINFO;
                              return 0;
                           }/* if */
                           else{
                              ErrorMsg(SysAdm);
                              RecordEvent(ErrorGeneral2);
                              del_pic(&p);
                              Step = DETECTFREEPLACE;
                              return -1;
                           }/* elif */
                        }/* if */
                        else{
                           MessageBox("Стоянка", "N∙ места неправильный.");
                           SaveStep = Step;
                           Step = WAITINFO;
                           return 0;
                        }/* elif */
                     }/* if */
                     else{
                        if(strcmp(keyInFree, DBFGetFieldAsString(dbParking, nfInUse)) != 0){
                           MessageBox("Стоянка", "Извините! Но это место занято.");
                           SaveStep = Step;
                           Step = WAITINFO;
                           DBFClose(dbParking);
                           return 0;
                        }/* if */

                        DBFClose(dbParking);

                        strcpy(PlaceNum, String_Edit);
                        outtext_obj_num(p, 201, 0, EGA_LIGHTGRAY, PlaceNum, SHOW);
                        setcolor_obj_num(p, NOc, 0, EGA_LIGHTGREEN, SHOW);
                        Edit_Stop(p, NOc, 0);
                        Step = NEXTFIELD;
                     }/* elif */
                     break;
                  }/* if */

                  // проверить ввод даты
                  if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[5]) == 0 ||
                     strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[8]) == 0 ){
                     if(sscanf(String_Edit, "%2d-%2d-%2d", &D, &M, &Y) != 3){
                        MessageBox("Ввод даты", "Формат даты: ДД-ММ-ГГ");
                        SaveStep = Step;
                        Step = WAITINFO;
                        break;
                     }/* if */
                     Y += 2000;
                     if(VisYear(Y)){
                        NumDayInMonth[1] = 29;
                     }/* if */
                     else{
                        NumDayInMonth[1] = 28;
                     }/* elif */
                     if(M < 1 || M > 12){
                        MessageBox("Ввод месяца", "Номер месяца неправилен\n"
                                                  "вводите от 1 до 12.");
                        SaveStep = Step;
                        Step = WAITINFO;
                        break;
                     }/* if */
                     if(D < 1 || D > NumDayInMonth[M - 1]){
                        MessageBox("Ввод числа месяца", "Число ошибочно.\nВводите от 1 до %d", NumDayInMonth[M - 1]);
                        SaveStep = Step;
                        Step = WAITINFO;
                        break;
                     }/* if */

                     if(VisYear(Y)){
                        NumDayInMonth[1] = 29;
                     }/* if */
                     else{
                        NumDayInMonth[1] = 28;
                     }/* elif */

                     sprintf(DateBuf, "%4.4d%2.2d%2.2d", Y, M, D);
                     DBFSetFieldAsString(db, nfsInpPerson[NOc - NOs], DateBuf);

                  }/* if */
                  else{
                     // проверить ввод времени
                     if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[6]) == 0 ||
                        strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[9]) == 0 ){
                        if(sscanf(String_Edit, "%2d:%2d", &Hr, &Mn) != 2){
                           MessageBox("Ввод времени", "Формат ввода времени ЧЧ:ММ");
                           SaveStep = Step;
                           Step = WAITINFO;
                           break;
                        }/* if */

                        if(Hr < 0 || Hr > 23){
                           MessageBox("Ввод времени", "Часы неправильно,\n"
                                                      " вводите от 0 до 23");
                           SaveStep = Step;
                           Step = WAITINFO;
                           break;
                        }/* if */

                        if(Mn < 0 || Mn > 59){
                           MessageBox("Ввод времени", "Минуты неправильно,\n"
                                                      "вводите от 0 до 59");
                           SaveStep = Step;
                           Step = WAITINFO;
                           break;
                        }/* if */

                        sprintf(DateBuf, "%2.2d:%2.2d:00", Hr, Mn);
                        DBFSetFieldAsString(db, nfsInpPerson[NOc - NOs], DateBuf);
                     }/* if */
                     else{
                        // проверить введенный период времени час.
                        if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[7] ) == 0){
                           if(sscanf(String_Edit, " %d ", &Hours) == 1){
                              if(Hours > 26352 || Hours <= 0){
                                 MessageBox("Ввод периода", "Период от 1 до 26352 часа.");
                                 SaveStep = Step;
                                 Step = WAITINFO;
                                 break;
                              }/* if */
                           }/* if */
                           else{
                              MessageBox("Ввод периода", "Вводите период в часах.");
                              SaveStep = Step;
                              Step = WAITINFO;
                              break;
                           }/* elif */
                        }/* if */

                        DBFSetFieldAsString(db, nfsInpPerson[NOc - NOs], String_Edit);

                     }/* elif */
                  }/* elif */

                  // проверить C
                  if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[6] ) == 0){
                     DBFGetFieldAsDate(db, nfsInpPerson[5], &StartDate);
                     DBFGetFieldAsTime(db, nfsInpPerson[6], &StartTime);

                     if(CheckAfterDate(&StartDate, &StartTime, &dt, &tm) == TRUE){
                        MessageBox("Ввод поля 'С'", "Поле д.б. позже чем сейчас.");
                        SaveStep = Step;
                        Step = WAITINFO;
                        break;
                     }/* if */
                  }/* if */

                  // проверить введенный период
                  if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[9] ) == 0){
                     DBFGetFieldAsDate(db, nfsInpPerson[5], &StartDate);
                     DBFGetFieldAsTime(db, nfsInpPerson[6], &StartTime);
                     DBFGetFieldAsDate(db, nfsInpPerson[8], &EndDate);
                     DBFGetFieldAsTime(db, nfsInpPerson[9], &EndTime);

                     if(CheckAfterDate(&EndDate, &EndTime, &StartDate, &StartTime) == TRUE){
                        MessageBox("Ввод поля 'ПО'", "Поле д.б. позже чем\n."
                                                     "поле 'С'");
                        SaveStep = Step;
                        Step = WAITINFO;
                        break;
                     }/* if */

                     Tarif = CalcTarif(TDTALON, TCSTANDART, &StartTime, &StartDate, &EndTime, &EndDate);
                     if(Tarif <= 0.01){
                        MessageBox("Ввод периода", "Сумма слишком мала.\nОформите на больший период");
                        SaveStep = Step;
                        Step = WAITINFO;
                        break;
                     }/* if */

                     sprintf(starif, ftarif, (unsigned)((long)Tarif),
                                             (unsigned)((Tarif - ((long)Tarif)) * 100.));
                     outtext_obj_num(p, 200, 0, EGA_LIGHTGRAY, starif, SHOW);
                  }/* if */

                  // Mark input field
                  Edit_Stop(p,NOc,0);
                  setcolor_obj_num(p, NOc, 0, EGA_LIGHTGREEN, SHOW);
                  Step = NEXTFIELD;
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

   if(Step == CALCTARIF){
      Step = MONEYGET;
      return 0;
   }/* if */

   if(Step == MONEYGET){

      del_pic(&p);
      load_file(&p, "tbg\\tarif1.tbg");
      outpic_all(p);
      outtext_obj_num(p, 50, 1, getcolor_obj_num(p, 50, 0), starif, SHOW);
      Step = WAITMONEYGET;
      return 0;
   }/* if */

   if(Step == WAITMONEYGET){
      OutTime(p);
      if(Key == '*'){
         Step = PRINTFR;
      }/* if */
      return 0;
   }/* if */

   if(Step == PRINTBC){

      Step = SHB;
      PrintBC(NPost, PlaceNum, "Талон", Date, "", FIO, BC);

      db = DBFOpen(8); // Стояночные талоны 4-й стоянки
      sprintf(BCBuf, "%6.6lu", BC);
      DBFFindRecord(db, nfBC, BCBuf);
      if(DBFGetErrorNum() != DBF_OK){
         if(DBFMsgError(8) == 0){
            SaveStep = STOPPROGRAM;
            Step = WAITINFO;
            return 0;
         }/* if */
         else{
            ErrorMsg(SysAdm);
            RecordEvent(ErrorGeneral2);
            del_pic(&p);
            Step = STOPPROGRAM;
            return 0;
         }/* elif */
      }/* if */
      else{
         DBFGetFieldAsDate(db, nfsInpPerson[5], &StartDate);
         DBFGetFieldAsTime(db, nfsInpPerson[6], &StartTime);
         DBFClose(db);
         if(CheckAfterDate(&dt, &tm, &StartDate, &StartTime) == TRUE){
            MessageBox("Въезд по стояночному талону",
               "Въезд еще не разрешен!\n"
               "");
            SaveStep = STOPPROGRAM;
            Step = WAITINFO;
         }/* if */
      }/* elif */
      return 0;
   }/* if */

   if(Step == PRINTFR){
      switch(PrintFR(Tarif)){
         case 1:// ok
            Step = PRINTBC;
            RecordFRMoney(Tarif);
            RecordEvent(FRPrint);

            break;
         case 0:// wait
            break;
         case -1:// error
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == SHB){

      del_pic(&p);
      Step = WAITSHB;
      return 0;
   }/* if */

   if(Step == WAITSHB){
      switch(WorkSHB()){
         case -1: // error
            Step = DETECTFREEPLACE;
            return -1;
         case  0: // wait
            break;
         case  1: // ok


            {
               char tGRN[16];
               db = DBFOpen(8); // Стояночные талоны 4-й стоянки
               sprintf(BCBuf, "%6.6lu", BC);
               DBFFindRecord(db, nfBC, BCBuf);
               if(DBFGetErrorNum() != DBF_OK){
                  DBFClose(db);
                  return -1;
               }/* if */
               else{
                  strcpy(tGRN, DBFGetFieldAsString(db, nfGRN));

                  DBFSetFieldAsString(db, nfUseParking, keyInUse);

                  NumRunning = DBFGetFieldAsInt(db, nfNumRunning);
                  NumRunning--;
                  DBFSetFieldAsInt(db, nfNumRunning, NumRunning);
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

            return 0;
      }/* switch */
      return 0;
   }/* if */


   if(Step == NEXTFIELD){
      NOc++;
      if(NOc > NOe){
         NOc = NOs;
      }/* if */
      switch(NOc - NOs){
         case 10:
            Edit_Init(p,NOc, 0, PlaceNum, 11);
            break;
         case 5:
         case 8:
            if(getcolor_obj_num(p, NOc, 0) == EGA_LIGHTGREEN){
               DBFGetFieldAsDate(db, nfsInpPerson[NOc - NOs], &dt);
            }/* if */

            if((NOc - NOs == 8)
               && (getcolor_obj_num(p, 105, 0) == EGA_LIGHTGREEN)
               && (getcolor_obj_num(p, 106, 0) == EGA_LIGHTGREEN)
               && (getcolor_obj_num(p, 107, 0) == EGA_LIGHTGREEN)){
               DBFGetFieldAsDate(db, nfsInpPerson[5], &dt);
               DBFGetFieldAsTime(db, nfsInpPerson[6], &tm);
               Hours = DBFGetFieldAsInt(db, nfsInpPerson[7]);
               FutureHour(&dt, &tm, Hours);
            }/* if */

            sprintf(DateBuf, "%2.2d-%2.2d-%2.2d", dt.da_day, dt.da_mon, dt.da_year - 2000);
            Edit_Init(p,NOc,0, DateBuf, strlen(DateBuf) + 1);
            break;

         case 7:
            if(getcolor_obj_num(p, NOc, 0) == EGA_LIGHTGREEN){
               Hours = DBFGetFieldAsInt(db, nfsInpPerson[NOc - NOs]);
            }/* if */

            sprintf(DateBuf, "%d", Hours);
            Edit_Init(p,NOc,0, DateBuf, 6);
            break;

         case 6:
         case 9:
            if(getcolor_obj_num(p, NOc, 0) == EGA_LIGHTGREEN){
               DBFGetFieldAsTimeString(db, nfsInpPerson[NOc - NOs], NULL, DateBuf);
            }/* if */
            else{
               DBFGetFieldAsTimeString(db, nfsInpPerson[NOc - NOs], &tm, DateBuf);
               sprintf(DateBuf, "%2.1d:%2.2d", tm.ti_hour, tm.ti_min);
            }/* elif */

            if((NOc - NOs == 9)
               &&(getcolor_obj_num(p, 105, 0) == EGA_LIGHTGREEN)
               && (getcolor_obj_num(p, 106, 0) == EGA_LIGHTGREEN)
               && (getcolor_obj_num(p, 107, 0) == EGA_LIGHTGREEN)){
               DBFGetFieldAsDate(db, nfsInpPerson[5], &dt);
               DBFGetFieldAsTime(db, nfsInpPerson[6], &tm);
               Hours = DBFGetFieldAsInt(db, nfsInpPerson[7]);
               FutureHour(&dt, &tm, Hours);
               sprintf(DateBuf, "%2.1d:%2.2d", tm.ti_hour, tm.ti_min);
            }/* if */

            Edit_Init(p,NOc,0, DateBuf, strlen(DateBuf) + 1);
            break;
         default:
            Edit_Init(p,NOc,0, DBFGetFieldAsString(db, nfsInpPerson[NOc - NOs]), DBFGetFieldLength(db, nfsInpPerson[NOc - NOs]) + 1);
            break;
      }/* switch */
      Refresh_Display(p,NOc, 0);
      EditLine_obj_num(p,NOc, 0);
      Step = WAITINPUTPERSON;
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

   if(Step == WAITQSL){
      if(Key == ENTER){
         Step = STOPPROGRAM;
         MessageBox(NULL, NULL);
      }/* if */
      return 0;
   }/* if */

   if(Step == STOPPROGRAM){

      del_pic(&p);
      Step = DETECTFREEPLACE;
      return 1;
   }/* if */

   return -1;
}/* Post4_1 */
/* ----------------------------------------------------------------------- */
