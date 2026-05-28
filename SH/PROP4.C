/* -----------------------------------------------------------------------
*
*  Prop4.C
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
            *keyInFree     = "0",
            *keyInUse      = "1",
            *keyInMark     = "2",
             PlaceNum[ 16],
                  GRN[ 16],
             Type    [ 32],
             FIO     [ 32],
             YearEnd [ 16],
            *nfsInpPerson[]={
"ФИО",      // 0
"NТЕЛ",     // 1
"МАРКА",    // 2
"СВД",      // 3
"ГРН",      // 4
"С",        // 5
"СВРЕМЯ",   // 6
"ПО",       // 7
"ПОВРЕМЯ",  // 8
"NВЪЕЗДА",  // 9
"ВСЕГО",    // 10
"КАТЕГОРИЯ" // 11
};

static struct date StartDate, EndDate;
static struct time StartTime, EndTime;
static int NumRunning;
static long BC;

#define FSIZE 5
static char* Formats[][FSIZE] = {
{Info0, ABCRus, CtrKey, NULL  , NULL}, // 0 ФИО
{Info1, DecDig, Sep1  , CtrKey, NULL}, // 1 НТЕЛ
{Info2, ABCRus, ABCEng, CtrKey, DecDig}, // 2 Марка
{Info3, ABCRus, DecDig, CtrKey, NULL}, // 3 свд
{Info3, ABCRus, DecDig, CtrKey, NULL}, // 4 грн
{Info1, DecDig, Sep1  , CtrKey, NULL}, // 5 с
{Info5, DecDig, Sep2  , CtrKey, NULL}, // 6 свремя
{Info1, DecDig, Sep1  , CtrKey, NULL}, // 7 по
{Info5, DecDig, Sep2  , CtrKey, NULL}, // 8 повремя
{Info12,DecDig, TypeDa, CtrKey, Sep3}, // 9  Нвъезда
{Info6, DecDig, NULL  , CtrKey, NULL}, // 10 осталось
{Info3, DecDig, ABCRus, CtrKey, NULL}  // 11 категория
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
// оформить пропуск
int Prop4(void){
   int i;
   char DateBuf[9], BCBuf[16], NumRunningBuf[8];
   int D, M, Y, Hr, Mn;

   if(Step == DETECTFREEPLACE){

      if(dt.da_year % 4 == 0){
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

      db = DBFOpen(12); // Пропуска

      load_file(&p, "tbg\\Prop4.tbg");

      getmin_obj(p, 99, (short int*)&NOs);
      getmax_obj(p, 99, (short int*)&NOe);
      NOc = NOs;

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
               if((getcolor_obj_num(p, i, 0) != EGA_LIGHTGREEN) || (strlen(gettext_obj_num(p, i, 0)) == 0)){
                  MessageBox("Ввод формы", "Введите все поля формы");
                  SaveStep = Step;
                  Step = WAITINFO;
                  return 0;
               }/* if */
            }/* for */

            DBFGetFieldAsDate(db, nfsInpPerson[5], &StartDate);
            DBFGetFieldAsTime(db, nfsInpPerson[6], &StartTime);
            DBFGetFieldAsDate(db, nfsInpPerson[7], &EndDate);
            DBFGetFieldAsTime(db, nfsInpPerson[8], &EndTime);

            if(CheckAfterDate(&EndDate, &EndTime, &StartDate, &StartTime) == TRUE){
               MessageBox("Ввод формы", "Ошибка в дате или времени,\n"
                                        "поле ПО: д.б. быть позже чем поле С:");
               SaveStep = Step;
               Step = WAITINFO;
               break;
            }/* if */

            DBFSetFieldAsString(db, nfUseParking, keyInFree);

            BC = CalcBC(14);
            sprintf(BCBuf, "%6.6lu", BC);
            DBFSetFieldAsString(db, nfBC, BCBuf);
            DBFAppendRecord(db);
            Edit_Stop(p,NOc,0);
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
                  if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[5]) == 0 ||
                     strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[7]) == 0 ){
                     if(sscanf(String_Edit, "%2d-%2d-%2d", &D, &M, &Y) != 3){
                        MessageBox("Ввод даты", "Формат даты: ДД-ММ-ГГ");
                        SaveStep = Step;
                        Step = WAITINFO;
                        break;
                     }/* if */
                     Y += 2000;
                     if(VisYear(Y) == 0){
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
                     if(dt.da_year % 4 == 0){
                        NumDayInMonth[1] = 29;
                     }/* if */
                     else{
                        NumDayInMonth[1] = 28;
                     }/* elif */

                     sprintf(DateBuf, "%4.4d%2.2d%2.2d", Y, M, D);
                     DBFSetFieldAsString(db, nfsInpPerson[NOc - NOs], DateBuf);
                  }/* if */
                  else{
                     if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[6]) == 0 ||
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
                        DBFSetFieldAsString(db, nfsInpPerson[NOc - NOs], String_Edit);
                     }/* elif */
                  }/* elif */

                  // проверить ввденные номера въездов и типы дней
                  if(NOc - NOs == 9){
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

                  if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[8] ) == 0){
                     DBFGetFieldAsDate(db, nfsInpPerson[5], &StartDate);
                     DBFGetFieldAsTime(db, nfsInpPerson[6], &StartTime);
                     DBFGetFieldAsDate(db, nfsInpPerson[7], &EndDate);
                     DBFGetFieldAsTime(db, nfsInpPerson[8], &EndTime);

                     if(CheckAfterDate(&EndDate, &EndTime, &StartDate, &StartTime) == TRUE){
                        MessageBox("Ввод поля 'ПО'", "Поле д.б. позже чем\n."
                                                     "поле 'С'");
                        SaveStep = Step;
                        Step = WAITINFO;
                        break;
                     }/* if */

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
      Step = STOPPROGRAM;
      strcpy(Type, "Катег: ");
      strcat(Type, DBFGetFieldAsString(db, nfsInpPerson[11]));
      strcpy(FIO, DBFGetFieldAsString(db, nfsInpPerson[0]));
      DBFClose(db);
      sprintf(YearEnd, "конца %4.4d г.\x0D", dt.da_year);
      PrintBC(NPost, "любое", "Пропуск",  YearEnd, Type, FIO, BC);
      return 0;
   }/* if */

   if(Step == NEXTFIELD){
      NOc++;
      if(NOc > NOe){
         NOc = NOs;
      }/* if */
//      printf("\n%d[%s]: %d", NOc - NOs, nfsInpPerson[NOc - NOs], DBFGetFieldLength(db, nfsInpPerson[NOc - NOs]));
      switch(NOc - NOs){
         case 7:
         case 5:
            if(getcolor_obj_num(p, NOc, 0) == EGA_LIGHTGREEN){
               DBFGetFieldAsDate(db, nfsInpPerson[NOc - NOs], &dt);
            }/* if */
            sprintf(DateBuf, "%2.2d-%2.2d-%2.2d", dt.da_day, dt.da_mon, dt.da_year - 2000);
            Edit_Init(p,NOc,0, DateBuf, strlen(DateBuf) + 1);
            break;
         case 8:
         case 6:
            if(getcolor_obj_num(p, NOc, 0) == EGA_LIGHTGREEN){
               DBFGetFieldAsTimeString(db, nfsInpPerson[NOc - NOs], NULL, DateBuf);
            }/* if */
            else{
               switch(NOc - NOs){
                  case 6:
                     sprintf(DateBuf, "%2.2d:%2.2d", tm.ti_hour, tm.ti_min);
                     break;
                  case 8:
                     sprintf(DateBuf, "23:00");
                     break;
               }/* switch */
            }/* elif */
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
}/* Propusk */
/* ----------------------------------------------------------------------- */
