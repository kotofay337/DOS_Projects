/* -----------------------------------------------------------------------
*
*  TARIF.C
*
*  Вычисление нужных тарифов.
*
*
*  ----------------------------------------------------------------------- */
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include "com.h"
#include "dbf.h"
#include "data.h"

//#define DEBUG
#undef DEBUG

/* ----------------------------------------------------------------------- */
double CalcTarif(Typedoc_t TypeDoc, Typecalc_t TypeCalc, struct time* st, struct date* sd,
                  struct time* et, struct date* ed){

  double res = 0.0;
  char pT[256];

  DBF *db020, *db021, *db022, *db023;

  struct time t1=*st; // start
  struct time t2=*et; // end

  struct date d1 = *sd; // start
  struct date d2 = *ed; // end

  int day;
  struct time t_day,t_night;

  GetDayNightTime(&t_day, &t_night);
#ifdef DEBUG
  gotoxy(1, 1);

  printf("\nCalcTarif: в %s по тарифу %s",
  TypeDoc == TDABONEMENT ? "Абонементе" : "Талоне",
  TypeCalc == TCSTANDART ? "Стандартному." : "за Перестой.");
  printf("\nдень нач: %u:%u:%u",t_day.ti_hour,t_day.ti_min,t_day.ti_sec);
  printf("\nдень кон: %u:%u:%u",t_night.ti_hour,t_night.ti_min,t_night.ti_sec);
  printf("\n\nдата нач: %d-%d-%d", d1.da_year, d1.da_mon, d1.da_day);
  printf(" %u:%u:%u",t1.ti_hour,t1.ti_min,t1.ti_sec);
  printf("\nдата кон: %d-%d-%d", d2.da_year, d2.da_mon, d2.da_day);
  printf(" %u:%u:%u",t2.ti_hour,t2.ti_min,t2.ti_sec);
#endif
  day=NumDays(&d1, &d2);

  CalcTime(t1,t2,&t_day,&t_night,&day);
#ifdef DEBUG
  printf("\n суток  : %d", day);
  printf("\n дневных: %u:%u:%u",t_day.ti_hour,t_day.ti_min,t_day.ti_sec);
  printf("\n ночных : %u:%u:%u",t_night.ti_hour,t_night.ti_min,t_night.ti_sec);
#endif
   db020 = DBFOpen(20);
   if(db020 == NULL){
      return -1;
   }/* if */
   DBFGetRecord(db020);

   db021 = DBFOpen(21);
   if(db021 == NULL){
      return -1;
   }/* if */
   DBFGetRecord(db021);

   db022 = DBFOpen(22);
   if(db022 == NULL){
      return -1;
   }/* if */
   DBFGetRecord(db022);

   db023 = DBFOpen(23);
   if(db023 == NULL){
      return -1;
   }/* if */
   DBFGetRecord(db023);

   // считаем сколько дневных часов в руб.коп
   {
      int i;
      double dl = 0;
      char st[10], *endptr;
      char sd[10];

      if(t_day.ti_hour > 0 || t_day.ti_min > 0){
         DBFStart(db023);
         DBFFindRecord(db023, "ДОКУМЕНТ", TypeDoc == TDABONEMENT ? "АБОНЕМЕНТ" :
                                          TypeDoc == TDTALON     ? "ТАЛОН" : "");

         if(DBFGetErrorNum() != DBF_OK){
            ErrorMsg("Не найдено в 23 "
                     "в поле ДОКУМЕНТ ");
            DBFClose(db020);
            DBFClose(db021);
            DBFClose(db022);
            DBFClose(db023);
            return -1;
         }/* if */

         strcpy(pT, DBFGetFieldAsString(db023, TypeCalc == TCSTANDART ? "ТДНЕВНОЙ" :
                                               TypeCalc == TCPERESTOY ? "ТПДЕНЬ" : ""));

#ifdef DEBUG
  printf("\nИщем в 020 в поле ТАРИФ : %s", pT);
#endif
         DBFStart(db020);
         DBFFindRecord(db020, "ТАРИФ", pT);
         if(DBFGetErrorNum() != DBF_OK){
            ErrorMsg("Не найдено в 20 "
                     "в поле ТАРИФ");
            DBFClose(db020);
            DBFClose(db021);
            DBFClose(db022);
            DBFClose(db023);
            return -1;
         }/* if */


         // подсчитать дневные часы
#ifdef DEBUG
  printf("\nдневн. почасово. [", dl);
#endif
         dl = 0;
         for(i = 0;i < t_day.ti_hour;i++){
            double r;
            sprintf(st, "Т%u", i + 1);
            r = strtod(DBFGetFieldAsString(db020, st), &endptr);
#ifdef DEBUG
  printf("%4.2lf ", r);
#endif
            dl += r;
         }/* for */
         res += dl;

         // подсчитать если больше получаса
         dl = 0;
         if(t_day.ti_min > 0 && t_day.ti_min < 30){
            double r;
            sprintf(st, "Т%u", t_day.ti_hour + 1);
            res += r = strtod(DBFGetFieldAsString(db020, st), &endptr) / 2;
            dl += r;
#ifdef DEBUG
  printf(" + %4.2lf(за менее чем последний получас: %d мин.) ", r, t_day.ti_min);
#endif
         }/* if */

         dl = 0;
         if(t_day.ti_min >= 30){
            double r;
            sprintf(st, "Т%u", t_day.ti_hour + 1);
            res += r = strtod(DBFGetFieldAsString(db020, st), &endptr);
            dl += r;
#ifdef DEBUG
  printf(" + %4.2lf(за более чем последний получас: %d мин.) ", r, t_day.ti_min);
#endif
         }/* elif */
#ifdef DEBUG
  printf("]\n начислено за дневные %5.2lf", dl);
#endif
      }/* if */
   }

   // считаем сколько ночных часов в руб.коп
   {
      int i;
      double dl = 0;
      char st[10], *endptr;
      char sd[10];

      if(t_night.ti_hour > 0 || t_night.ti_min > 0){
         DBFStart(db023);
         DBFFindRecord(db023, "ДОКУМЕНТ", TypeDoc == TDABONEMENT ? "АБОНЕМЕНТ" :
                                          TypeDoc == TDTALON     ? "ТАЛОН" : "");
         if(DBFGetErrorNum() != DBF_OK){
            DBFClose(db020);
            DBFClose(db021);
            DBFClose(db022);
            DBFClose(db023);
            return -1;
         }/* if */

         strcpy(pT, DBFGetFieldAsString(db023, TypeCalc == TCSTANDART ? "ТНОЧНОЙ" :
                                         TypeCalc == TCPERESTOY ? "ТПНОЧЬ" : ""));

#ifdef DEBUG
  printf("\nИщем в 021 в поле ТАРИФ : %s", pT);
#endif
         DBFStart(db021);
         DBFFindRecord(db021, "ТАРИФ", pT);
         if(DBFGetErrorNum() != DBF_OK){
            ErrorMsg("Не найдено в 21 "
                     "в поле ТАРИФ");
            DBFClose(db020);
            DBFClose(db021);
            DBFClose(db022);
            DBFClose(db023);
            return -1;
         }/* if */


#ifdef DEBUG
  printf("\nночн. почасово. [", dl);
#endif
         // подсчитать ночные часы
         dl = 0;
         for(i = 0;i < t_night.ti_hour;i++){
            double r;
            sprintf(st, "Т%u", i + 1);
            r = strtod(DBFGetFieldAsString(db021, st), &endptr);
#ifdef DEBUG
  printf("%4.2lf ", r);
#endif
            dl += r;
         }/* for */
         res += dl;

         // подсчитать если меньше получаса
         if(t_night.ti_min > 0 && t_night.ti_min < 30){
            double r;
            sprintf(st, "Т%u", t_night.ti_hour + 1 );
            res += r = strtod(DBFGetFieldAsString(db021, st), &endptr) / 2;
            dl += r;
#ifdef DEBUG
  printf(" + %4.2lf(за менее чем последний получас: %d мин.) ", r, t_night.ti_min);
#endif
         }/* if */

         // подсчитать если меньше получаса
         if(t_night.ti_min >= 30){
            double r;
            sprintf(st, "Т%u", t_night.ti_hour + 1);
            res += r = strtod(DBFGetFieldAsString(db021, st), &endptr);
            dl += r;
#ifdef DEBUG
  printf(" + %4.2lf(за более чем последний час: %d мин.) ", r, t_night.ti_min);
#endif
         }/* elif */
#ifdef DEBUG
  printf("]\n начислено за ночные: %5.2lf", dl);
#endif
      }/* if */
   }

   // считаем сколько суток в руб.коп
   {
      int i;
      double dl = 0;
      char st[10], *endptr;
      char sd[10];

      if(day > 0){
         DBFStart(db023);
         DBFFindRecord(db023, "ДОКУМЕНТ", TypeDoc == TDABONEMENT ? "АБОНЕМЕНТ" :
                                          TypeDoc == TDTALON     ? "ТАЛОН" : "");
         if(DBFGetErrorNum() != DBF_OK){
            DBFClose(db020);
            DBFClose(db021);
            DBFClose(db022);
            DBFClose(db023);
            return -1;
         }/* if */

         strcpy(pT, DBFGetFieldAsString(db023, TypeCalc == TCSTANDART ? "ТСУТОЧНЫЙ" :
                                         TypeCalc == TCPERESTOY ? "ТПСУТОЧНЫЙ" : ""));

         DBFStart(db022);
         DBFFindRecord(db022, "ТАРИФ", pT);
         if(DBFGetErrorNum() != DBF_OK){
            ErrorMsg("Не найдено в 22 "
                     "в поле ТАРИФ");
            DBFClose(db020);
            DBFClose(db021);
            DBFClose(db022);
            DBFClose(db023);
            return -1;
         }/* if */

         // подсчитать суточные в руб.коп
         dl = 0;
         for(i = 0;i < day;i++){
            sprintf(st, "Д%u", i + 1);
            dl += strtod(DBFGetFieldAsString(db022, st), &endptr);
         }/* for */
         res += dl;
#ifdef DEBUG
  printf("\n начислено за суточные: %5.2lf", dl);
#endif
      }/* if */
   }

   DBFClose(db020);
   DBFClose(db021);
   DBFClose(db022);
   DBFClose(db023);

#ifdef DEBUG
  printf("\n начислено всего: %5.2lf", res);
#endif
   return res == 0.0 ? 0.001 : res;
}/* CalcTarif */
/* ----------------------------------------------------------------------- */
