/* -----------------------------------------------------------------------
*
*  Propusk.C
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
            *nfCarCategory = "ЛГ",
            *nfInUse       = "ЗАНЯТО",
            *nfGRN         = "ГРН",
            *nfBC          = "ШК",
            *nfNumRunning  = "ОСТАЛОСЬ" ,
            *keyInFree     = "0",
            *keyInUse      = "1",
            *keyInMark     = "2",
             PlaceNum[ 16],
             GRN     [ 16],
             Type    [ 32],
             FIO     [ 32],
             YearEnd [ 16],
            *nfsInpPerson[]={
"ФИО",      // 0
"NТЕЛ",     // 1
"ГРН",      // 2
"С",        // 3
"ПО",       // 4
"ТИП",      // 5
};

static struct date StartDate, EndDate;
static int NumRunning;
static long BC;
/* ----------------------------------------------------------------------- */
int Propusk(void){
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

      Step = INPUTPERSON;
      return 0;
   }/* if */

   if(Step == INPUTPERSON){
      del_pic(&p);

      db = DBFOpen(12); // Пропуска

      load_file(&p, "tbg\\Propusk.tbg");

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
            BC = CalcBC(14);
            sprintf(BCBuf, "%6.6lu", BC);
            DBFSetFieldAsString(db, nfBC, BCBuf);
            DBFSetFieldAsInt(db, nfNumRunning, 999);
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
            switch(EditLine_obj_num(p,NOc,0)){
               case OK      :
                  if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[3]) == 0 ||
                     strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[4]) == 0 ){
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

                     // current date or greater?
                     if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[3] ) == 0){
                        EndDate.da_year   = Y;
                        EndDate.da_mon    = M;
                        EndDate.da_day    = D;
                        if(NumDays(&dt, &EndDate) >= 1){
                        }/* if */
                        else{
                           MessageBox("Ввод поля 'С'", "Поле д.б. позже чем сейчас.");
                           SaveStep = Step;
                           Step = WAITINFO;
                           break;
                        }/* elif */
                     }/* if */

                     Edit_Stop(p,NOc,0);
                     sprintf(DateBuf, "%4.4d%2.2d%2.2d", Y, M, D);
                     DBFSetFieldAsString(db, nfsInpPerson[NOc - NOs], DateBuf);
                  }/* if */
                  else{
                     DBFSetFieldAsString(db, nfsInpPerson[NOc - NOs], Edit_Stop(p,NOc,0));
                  }/* elif */
                  // Mark input field
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
      strcpy(Type, DBFGetFieldAsString(db, "ТИП"));
      strcpy(FIO, DBFGetFieldAsString(db, "ФИО"));
      sprintf(YearEnd, "конца %4.4d г.\x0D", dt.da_year);

      PrintBC(NPost, "любое", "Пропуск",  YearEnd, Type, FIO, BC);
      DBFClose(db);
      Step = STOPPROGRAM;
      return 0;
   }/* if */

   if(Step == NEXTFIELD){
      NOc++;
      if(NOc > NOe){
         NOc = NOs;
      }/* if */
      switch(NOc - NOs){
         case 3:
         case 4:
            sprintf(DateBuf, "%2.2d-%2.2d-%2.2d", dt.da_day, dt.da_mon, dt.da_year - 2000);
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
