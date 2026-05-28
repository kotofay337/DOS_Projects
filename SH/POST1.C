/* -----------------------------------------------------------------------
*
*  Post1.C
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
#include "tbg_tc.h"
#include "dbf.h"
#include "data.h"

#undef DEBUG

/* ----------------------------------------------------------------------- */
typedef
   enum{
      SWPERSONS,
      WAITKATEGP,
      SIMPLEKATP, // Коммерческий
      VIP1KATP,   // Абонемент
      VIP2KATP,   // Служебный
      ABON,       // Оформить абонемент
      WORKABON,   // Оформить абонемент
      WAITKATEGC,
      CLOSESM,
      SIMPLEP,
      WAITMONEY,
      SHB,
      WAITSHB,
      FR,
      WAITFR,
      WAITERROROUT,
      WORKVIP1KATP,
      WORKVIP2KATP
   }STEP;

static STEP Step = SWPERSONS;
/* ----------------------------------------------------------------------- */
int Post1(void){

#ifdef DEBUG
      if(Key == 27){
         del_pic(&p);
         return 1;
      }/* if */
#endif
      if(Step == SWPERSONS){
         del_pic(&p);
         p = NULL;
         load_file(&p, "tbg\\kateg2.tbg");
         outpic_all(p);
         Step = WAITKATEGP;
         return 0;
      }/* if */

      if(Step == WAITKATEGP){
         OutTime(p);
         if(Key == '1'){
            Step = SIMPLEKATP;
            RecordEvent(SelectPerson1);
         }/* if */
         if(Key == '2'){
            RecordEvent(SelectPerson2);
            Step = VIP1KATP;
         }/* if */
         if(Key == '3'){
            RecordEvent(SelectPerson3);
            Step = VIP2KATP;
         }/* if */
         if(Key == '0'){
            RecordEvent(SelectPerson4);
            Step = ABON;
         }/* if */
         if(ExtKey == 68){ // F10
            Step = CLOSESM;
         }/* if */
         if(Key == '0'){
            RecordEvent(SelectPerson4);
            Step = ABON;
         }/* if */
         return 0;
      }/* if */

      if(Step == CLOSESM){
         OutTime(p);
         switch(PrintFR(-1.0)){
            case 0:// wait
               break;
            case 1:// return
               Step = SWPERSONS;
               RecordEvent(FRPrint);
               del_pic(&p);
               return 2;
            case -1:// error
               Step = SWPERSONS;
               del_pic(&p);
               return -1;
            default:
               break;
         }/* switch */
         return 0;
      }/* if */



      if(Step == VIP1KATP){
         del_pic(&p);
         Step = WORKVIP1KATP;
         return 0;
      }/* if */

      if(Step == VIP2KATP){
         del_pic(&p);
         Step = WORKVIP2KATP;
         return 0;
      }/* if */

      if(Step == ABON){
         del_pic(&p);
         Step = WORKABON;
         return 0;
      }/* if */

      if(Step == WORKVIP1KATP){
         switch(Post1_2()){
            case 0:// wait
               break;
            case 1:// return
               Step = SWPERSONS;
               break;
            case -1:// error
               Step = SWPERSONS;
               return -1;
            default:
               break;
         }/* switch */
         return 0;
      }/* if */

      if(Step == WORKVIP2KATP){
         switch(Post1_3()){
            case 0:// wait
               break;
            case 1:// return
               Step = SWPERSONS;
               break;
            case -1:// error
               Step = SWPERSONS;
               return -1;
            default:
               break;
         }/* switch */
         return 0;
      }/* if */

      if(Step == WORKABON){
         switch(Post1_0()){
            case 0:// wait
               break;
            case 1:// return
               Step = SWPERSONS;
               break;
            case -1:// error
               Step = SWPERSONS;
               return -1;
            default:
               break;
         }/* switch */
         return 0;
      }/* if */

      if(Step == SIMPLEKATP){
         del_pic(&p);
         load_file(&p, "tbg\\kateg1.tbg");
         outpic_all(p);
         Step = WAITKATEGC;
         return 0;
      }/* if */

      if(Step == WAITKATEGC){
         OutTime(p);
         if(Key == '1'){
            cc = Service;
            Step = SIMPLEP;
            RecordEvent(SelectCar1);
         }/* if */
         if(Key == '2'){
            cc = Commercial;
            Step = SIMPLEP;
            RecordEvent(SelectCar2);
         }/* if */
         return 0;
      }/* if */

      if(Step == SIMPLEP){
         Tarif = CalcTarifK(1);
         del_pic(&p);
         load_file(&p, "tbg\\tarif1.tbg");
         outpic_all(p);
         sprintf(starif, ftarif, (unsigned)((long)Tarif),
                                 (unsigned)((Tarif - ((long)Tarif)) * 100.));
         outtext_obj_num(p, 50, 1, getcolor_obj_num(p, 50, 0), starif, SHOW);
         Step = WAITMONEY;
         return 0;
      }/* if */

      if(Step == WAITMONEY){
         OutTime(p);
         if(Key == '*' || Key == '/' || Key == '-'){
            RecordEvent(MoneyOK);
            Step = FR;
         }/* if */
         return 0;
      }/* if */

      if(Step == FR){
         Step = WAITFR;
//         Step = SHB;
         return 0;
      }/* if */

      if(Step == WAITFR){
         switch(PrintFR(Tarif)){
            case 1:// ok
               Step = SHB;
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
         Step = WAITSHB;
         return 0;
      }/* if */

      if(Step == WAITSHB){
         switch(WorkSHB()){
            case -1: // error
               Step = SWPERSONS;
               return -1;
            case  0: // wait
               break;
            case  1: // ok
               Step = SWPERSONS;
               break;
         }/* switch */
         return 0;
      }/* if */

      return -1;

}/* Post1 */
/* ----------------------------------------------------------------------- */
