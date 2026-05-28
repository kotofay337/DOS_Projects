/* -----------------------------------------------------------------------
*
*  Post1_0.C
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
      WAITFR,
      SHB,
      WAITSHB,
      WAITQSL,
      WAITINFO,
      WAITERROROUT,
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
             PlaceNum[ 10],
                  GRN[ 16],
            *nfsInpPerson[]={
"ФИО",      // 0
"АРЕНДА",   // 1
"С",        // 2
"ПО",       // 3
};

static struct date StartDate, EndDate;
static int NumRunning;
static long BC;
/* ----------------------------------------------------------------------- */
int Post1_0(void){
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

      db = DBFOpen(2); // Абонементы

      load_file(&p, "tbg\\person10.tbg");

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
                  MessageBox("Ввод формы", "Введите все поля формы !");
                  SaveStep = Step;
                  Step = WAITINFO;
                  return 0;
               }/* if */
            }/* for */
            DBFAppendRecord(db);
            DBFClose(db);
            Edit_Stop(p,NOc,0);
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
            switch(EditLine_obj_num(p,NOc,0)){
               case OK      :
                  if(strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[2]) == 0 ||
                     strcmp(nfsInpPerson[NOc - NOs], nfsInpPerson[3]) == 0 ){
                     if(sscanf(String_Edit, "%2d-%2d-%2d", &D, &M, &Y) != 3){
                        MessageBox("Поле ввода даты", "Формат даты: ДД-ММ-ГГ");
                        SaveStep = Step;
                        Step = WAITINFO;
                        break;
                     }/* if */
                     Y += 2000;
                     if(Y % 4 == 0){
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

                           DBFGetFieldAsDate(db, nfsInpPerson[2], &StartDate);

                           Tarif = CalcTarifA(&StartDate, &EndDate);
                           sprintf(starif, ftarif, (unsigned)((long)Tarif),
                                                   (unsigned)((Tarif - ((long)Tarif)) * 100.));
                           outtext_obj_num(p, 200, 0, EGA_LIGHTGRAY, starif, SHOW);
                        }/* if */
                        else{
                           MessageBox("Ввод даты", "Ошибка в дате!\nДата должна быть больше чем сегодня.");
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

   if(Step == PRINTFR){
      Step = WAITFR;
   }/* if */

   if(Step == WAITFR){
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
      switch(NOc - NOs){
         case 2:
         case 3:
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
}/* Post1_0 */
/* ----------------------------------------------------------------------- */
