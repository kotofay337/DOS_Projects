/* -----------------------------------------------------------------------
*
*  Abon4.C
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
            *nfsInpPerson[]={
"ФИО",      // 0
"NТЕЛ",     // 1
"МАРКА",    // 2
"ГРН",      // 3
"С",        // 4
"СВРЕМЯ",   // 5
"ПЕРИОД",   // 6
"ПО",       // 7
"ПОВРЕМЯ",  // 8
"ТИП",      // 9
"АРЕНДА",   // 10
"NЧЕКА",    // 11
"NВЪЕЗДА",  // 12
"ОСТАЛОСЬ", // 13
"КАТЕГОРИЯ" // 14
};

static struct date StartDate, EndDate;
static struct time StartTime, EndTime;
static int NumRunning, Hours = 3;
static long BC;

#define FSIZE 5
static char* Formats[][FSIZE] = {
{Info0, ABCRus, CtrKey, NULL  , NULL}, // 0 ФИО
{Info1, DecDig, Sep1  , CtrKey, NULL}, // 1 НТЕЛ
{Info2, ABCRus, ABCEng, CtrKey, DecDig}, // 2 Марка
{Info3, ABCRus, DecDig, CtrKey, NULL}, // 3 грн
{Info1, DecDig, Sep1  , CtrKey, NULL}, // 4 с
{Info5, DecDig, Sep2  , CtrKey, NULL}, // 5 свремя
{Info6, DecDig, NULL  , CtrKey, NULL}, // 6 период
{Info1, DecDig, Sep1  , CtrKey, NULL}, // 7 по
{Info5, DecDig, Sep2  , CtrKey, NULL}, // 8 повремя
{Info9, TypeAb, CtrKey, NULL  , NULL}, // 9 тип
{Info6, DecDig, ABCRus, CtrKey, "-_"}, // 10 аренда
{Info6, DecDig, NULL  , CtrKey, NULL}, // 11 нчека
{Info12,DecDig, TypeDa, CtrKey, Sep3}, // 12 Нвъезда
{Info6, DecDig, NULL  , CtrKey, NULL}, // 13 осталось
{Info3, DecDig, ABCRus, CtrKey, NULL}  // 14 категория
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
int Abon4(int tCall){
   int i;
   char DateBuf[32], BCBuf[16], NumRunningBuf[8], Type;
   char TimeBuf[16];
   int D, M, Y, Hr, Mn;

   if(Step == DETECTFREEPLACE){

      // обнаружить свободные места
      db = DBFOpen(3); // Стоянка
      DBFFindRecord(db, nfInUse, keyInFree);
      if(DBFGetErrorNum() != DBF_OK){
         if(DBFGetErrorNum() == DBF_ERROR_RECORD_NOT_FOUND){
            MessageBox("Стоянка", "Свободных мест НЕТ!");
            DBFClose(db);
            Step = WAITQSL;
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

      if(VisYear(dt.da_year)){
         NumDayInMonth[1] = 29;
      }/* if */
      else{
         NumDayInMonth[1] = 28;
      }/* elif */

      load_file(&p, "tbg\\timebar.tbg");
      outpic_all(p);

      Step = INPUTPERSON;
      return 0;
   }/* if */

   if(Step == INPUTPERSON){
      del_pic(&p);

      db = DBFOpen(tCall == 0 ? 2 : 102); // Абонементы или времАбонементы

      load_file(&p, "tbg\\Abon4.tbg");

      getmin_obj(p, 99, (short int*)&NOs);
      getmax_obj(p, 99, (short int*)&NOe);
      NOc = NOs;

//      NOe = tCall == 0 ? NOe : NOe - 4;

      // all field clear
      for(i = NOs;i <= NOe;i++){
         *(gettext_obj_num(p, i, 0)) = '\0';
      }/* for */

      outpic_all(p);

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
               if((tCall != 0) && ((i - NOs) == 11)){
                  i++;
               }/* if */
               if((getcolor_obj_num(p, i, 0) != EGA_LIGHTGREEN) ||
                  (strlen(gettext_obj_num(p, i, 0)) == 0)       ){
                  MessageBox("Ввод формы", "Введите все поля формы");
                  SaveStep = Step;
                  Step = WAITINFO;
                  return 0;
               }/* if */
            }/* for */

            DBFGetFieldAsDate(db, nfsInpPerson[4], &StartDate);
            DBFGetFieldAsTime(db, nfsInpPerson[5], &StartTime);
            DBFGetFieldAsDate(db, nfsInpPerson[7], &EndDate);
            DBFGetFieldAsTime(db, nfsInpPerson[8], &EndTime);

            if(CheckAfterDate(&EndDate, &EndTime, &StartDate, &StartTime) == TRUE){
               MessageBox("Ввод формы", "Ошибка в дате или времени,\n"
                                        "поле ПО: д.б. быть позже чем поле С:");
               SaveStep = Step;
               Step = WAITINFO;
               break;
            }/* if */

            Edit_Stop(p,NOc,0);

            DBFSetFieldAsString(db, nfUseParking, keyInFree);
            {
               char sBC[16];
               BC = CalcBC(7);
               sprintf(sBC, "%6.6lu", BC);
               DBFSetFieldAsString(db, nfBC, sBC);
               DBFSetFieldAsString(db, nfPlaceNum, PlaceNum);
               DBFAppendRecord(db);
            }

            // бронируем МЕСТО НА СТОЯНКЕ
            dbParking = DBFOpen(3); // Стоянка
            DBFFindRecord(dbParking, nfPlaceNum, PlaceNum);
            DBFSetFieldAsString(dbParking, nfInUse, keyInMark);
            DBFSetFieldAsString(dbParking, nfGRN, "Абонемент");
            DBFPutRecord(dbParking);
            DBFClose(dbParking);

            Step = PRINTBC;
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
                  // проверить ввод даты
                  if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[4]) == 0 ||
                     strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[7]) == 0 ){
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
                     if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[5]) == 0 ||
                        strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[8]) == 0 ){
                        if(sscanf(String_Edit, "%2d:%2d", &Hr, &Mn) != 2){
                           MessageBox("Ввод времени", "Формат ввода времени ЧЧ:ММ");
                           SaveStep = Step;
                           Step = WAITINFO;
                           break;
                        }/* if */

                        if(Hr < 0 || Hr > 23){
                           MessageBox("Ввод времени", "Часы введены неправильно,\n"
                                                      "вводите от 0 до 23");
                           SaveStep = Step;
                           Step = WAITINFO;
                           break;
                        }/* if */

                        if(Mn < 0 || Mn > 59){
                           MessageBox("Ввод времени", "Минуты введены неправильно,\n"
                                                      "вводите от 0 до 59");
                           SaveStep = Step;
                           Step = WAITINFO;
                           break;
                        }/* if */

                        sprintf(DateBuf, "%2.2d:%2.2d:00", Hr, Mn);
                        DBFSetFieldAsString(db, nfsInpPerson[NOc - NOs], DateBuf);
                     }/* if */
                     else{
                        // проверить ввод типа
                        if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[9]) == 0){
                           if(String_Edit[0] == 'Д' ||
                              String_Edit[0] == 'Н' ||
                              String_Edit[0] == 'С'){
                              DBFSetFieldAsString(db, nfsInpPerson[NOc - NOs], String_Edit);
                           }/* if */
                           else{
                              MessageBox("Ввод типа абонемента",
                                         "Введённое значение не правильно, вводите:"
                                         "'Д' - дневной\n"
                                         "'Н' - ночной\n"
                                         "'С' - суточный");
                              SaveStep = Step;
                              Step = WAITINFO;
                              break;
                           }/* elif */
                        }/* if */
                        else{
                           // проверить введенный период времени час.
                           if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[6] ) == 0){

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
                  }/* elif */

                  // проверить ввденные номера въездов и типы дней
                  if(NOc - NOs == 12){
                      if(CheckGate(String_Edit)){
                         MessageBox("Ошибка ввода",
                                 "Ошибка в списке номеров въезда!\n"
                                 "вводите например: 4Р,5В,6*,\n"
                                 "где: 4, 5, 6 или число от 1 до 999 -- номера въездов\n"
                                 "',' - разделитель полей",
                                 "'Р' - рабочие дни\n"
                                 "'В' - выходные\n"
                                 "'*' - любые");
                         SaveStep = Step;
                         Step = WAITINFO;
                         break;
                      }/* if */
                  }/* if */

                  // проверить C
                  if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[5] ) == 0){
                     DBFGetFieldAsDate(db, nfsInpPerson[4], &StartDate);
                     DBFGetFieldAsTime(db, nfsInpPerson[5], &StartTime);

                     if(CheckAfterDate(&StartDate, &StartTime, &dt, &tm) == TRUE){
                        MessageBox("Ввод поля 'С'", "Поле д.б. позже чем сейчас.");
                        SaveStep = Step;
                        Step = WAITINFO;
                        break;
                     }/* if */
                  }/* if */

                  // проверить введенный период
                  if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[8] ) == 0){
                     DBFGetFieldAsDate(db, nfsInpPerson[4], &StartDate);
                     DBFGetFieldAsTime(db, nfsInpPerson[5], &StartTime);
                     DBFGetFieldAsDate(db, nfsInpPerson[7], &EndDate);
                     DBFGetFieldAsTime(db, nfsInpPerson[8], &EndTime);

                     if(CheckAfterDate(&EndDate, &EndTime, &StartDate, &StartTime) == TRUE){
                        MessageBox("Ввод поля 'ПО'", "Поле д.б. позже чем\n."
                                                     "поле 'С'");
                        SaveStep = Step;
                        Step = WAITINFO;
                        break;
                     }/* if */

                     Tarif = CalcTarif(TDABONEMENT, TCSTANDART, &StartTime, &StartDate, &EndTime, &EndDate);
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

   if(Step == PRINTBC){
      if(tCall == 0){
         Step = STOPPROGRAM;
         DBFGetFieldAsDateString(db, "ПО", NULL, DateBuf);
         DBFGetFieldAsTimeString(db, "ПОВРЕМЯ", NULL, TimeBuf);
         strcat(DateBuf, " ");
         strcat(DateBuf, TimeBuf);
         Type = DBFGetFieldAsString(db, "ТИП")[0];
         PrintBC(NPost, PlaceNum, "Абонемент", DateBuf,
            Type == 'Д' ? "Дневной" :
            Type == 'Н' ? "Ночной" :
            Type == 'С' ? "Суточный" : "",
            DBFGetFieldAsString(db, "ФИО"), BC);
      }/* if */
      else{
         Step = MONEYGET;
      }/* elif */
      DBFClose(db);
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

   if(Step == PRINTFR){
      switch(PrintFR(Tarif)){
         case 1:// ok
            Step = STOPPROGRAM;
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

   if(Step == NEXTFIELD){
      NOc++;
      if(NOc > NOe){
         NOc = NOs;
      }/* if */

      if(tCall != 0){
         switch(NOc - NOs){
            case 11: // пропустить NЧЕКА
               NOc++;
               if(NOc > NOe){
                  NOc = NOs;
               }/* if */
         }/* switch */
      }/* if */

      switch(NOc - NOs){
         case 4:
         case 7:
            if(getcolor_obj_num(p, NOc, 0) == EGA_LIGHTGREEN){
               DBFGetFieldAsDate(db, nfsInpPerson[NOc - NOs], &dt);
            }/* if */

            if((NOc - NOs == 7)
               && (getcolor_obj_num(p, 104, 0) == EGA_LIGHTGREEN)
               && (getcolor_obj_num(p, 105, 0) == EGA_LIGHTGREEN)
               && (getcolor_obj_num(p, 106, 0) == EGA_LIGHTGREEN)){
               DBFGetFieldAsDate(db, nfsInpPerson[4], &dt);
               DBFGetFieldAsTime(db, nfsInpPerson[5], &tm);
               Hours = DBFGetFieldAsInt(db, nfsInpPerson[6]);
               FutureHour(&dt, &tm, Hours);
            }/* if */

            sprintf(DateBuf, "%2.2d-%2.2d-%2.2d", dt.da_day, dt.da_mon, dt.da_year - 2000);
            Edit_Init(p,NOc,0, DateBuf, strlen(DateBuf) + 1);
            break;

         case 6:
            if(getcolor_obj_num(p, NOc, 0) == EGA_LIGHTGREEN){
               Hours = DBFGetFieldAsInt(db, nfsInpPerson[NOc - NOs]);
            }/* if */

            sprintf(DateBuf, "%d", Hours);
            Edit_Init(p,NOc,0, DateBuf, 6);
            break;

         case 5:
         case 8:
            if(getcolor_obj_num(p, NOc, 0) == EGA_LIGHTGREEN){
               DBFGetFieldAsTimeString(db, nfsInpPerson[NOc - NOs], NULL, DateBuf);
            }/* if */
            else{
               DBFGetFieldAsTimeString(db, nfsInpPerson[NOc - NOs], &tm, DateBuf);
               sprintf(DateBuf, "%2.1d:%2.2d", tm.ti_hour, tm.ti_min);
            }/* elif */

            if((NOc - NOs == 8)
               &&(getcolor_obj_num(p, 104, 0) == EGA_LIGHTGREEN)
               && (getcolor_obj_num(p, 105, 0) == EGA_LIGHTGREEN)
               && (getcolor_obj_num(p, 106, 0) == EGA_LIGHTGREEN)){
               DBFGetFieldAsDate(db, nfsInpPerson[4], &dt);
               DBFGetFieldAsTime(db, nfsInpPerson[5], &tm);
               Hours = DBFGetFieldAsInt(db, nfsInpPerson[6]);
               FutureHour(&dt, &tm, Hours);
               sprintf(DateBuf, "%2.1d:%2.2d", tm.ti_hour, tm.ti_min);
            }/* if */

            Edit_Init(p,NOc,0, DateBuf, strlen(DateBuf) + 1);
            break;
         default:
            Edit_Init(p,NOc,0, DBFGetFieldAsString(db, nfsInpPerson[NOc - NOs]), DBFGetFieldLength(db, nfsInpPerson[NOc - NOs]) + 1);
            break;
      }/* switch */
      Refresh_Display(p,NOc,0);
      EditLine_obj_num(p,NOc,0);
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
}/* Abonement */
/* ----------------------------------------------------------------------- */
