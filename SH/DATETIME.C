/* ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <dos.h>
#include "dbf.h"
#include "data.h"
//#define DEBUG
/* ----------------------------------------------------------------------- */
extern struct time tm;
extern struct date dt;

                      //j   f   m   a   m   j   j   a   s   o   n   d
static int month1[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static char *wdname[] = {
"Воскресенье",
"Понедельник",
"Вторник",
"Среда",
"Четверг",
"Пятница",
"Суббота"
};

static char *mname[] = {
"Января",
"Февраля",
"Марта",
"Апреля",
"Мая",
"Июня",
"Июля",
"Августа",
"Сентября",
"Октября",
"Ноября",
"Декабря"
};

/*

   Все функции связанные с вычислением количества
дневных и ночных часов автор Попова Юлия. (С) 2001

*/
//static int fd(int,int,int);
void f1(struct time,struct time,struct time *,struct time *);
int check(struct time,struct time);
void sum(struct time *,struct time);
void f(struct time,struct time,struct time *);

void CalcTime(struct time,struct time,struct time *,struct time *,int *);

static struct time t0        = {0,0,0,0};
static struct time t_end_n   = {0,24,0,0};

static struct time t_begin_d;
static struct time t_end_d;
/* ----------------------------------------------------------------------- */
void SetDayNightTime(struct time bday, struct time eday){
   t_begin_d = bday;
   t_end_d = eday;
}/* SetDayNightTime */
/* ----------------------------------------------------------------------- */
void GetDayNightTime(struct time* bday, struct time* eday){
   *bday = t_begin_d;
   *eday = t_end_d;
}/* SetDayNightTime */
/* ----------------------------------------------------------------------- */
#ifdef DEBUG

#pragma argsused
int InfoMsg(unsigned char *msg){
   return;
}

void main(){

  struct time t1={0,8,0,0}; // start
  struct time t2={0,20,0,0}; // end

  struct date d1; // start
  struct date d2; // end

  struct time t_day,t_night;
  int day;

  SetDNT();

//  getdate(&d2);
//  gettime(&t2);

  d1.da_year = 2001;
  d1.da_mon = 1;
  d1.da_day = 1;

  d2.da_year = 2001;
  d2.da_mon = 1;
  d2.da_day = 2;


  printf("\nStart: ");
  printf("\n1 %d-%d-%d", d1.da_year, d1.da_mon, d1.da_day);
  printf("\n2 %d-%d-%d", d2.da_year, d2.da_mon, d2.da_day);
  printf("\n  %u %u %u",t1.ti_hour,t1.ti_min,t1.ti_sec);
  printf("\n  %u %u %u",t2.ti_hour,t2.ti_min,t2.ti_sec);
  getch();

  day=NumDays(&d1, &d2);

  CalcTime(t1,t2,&t_day,&t_night,&day);

  printf("\n   day   %d", day);
  printf("\n t day   %u:%u:%u",t_day.ti_hour,t_day.ti_min,t_day.ti_sec);
  printf("\n t night %u:%u:%u",t_night.ti_hour,t_night.ti_min,t_night.ti_sec);
  getch();

}//main
#endif
/* --------------------------------------------------------------------------------------- */
/* Calculate the number of day`s and night`s hours                                         */
/* --------------------------------------------------------------------------------------- */
void CalcTime(struct time t1,struct time t2,struct time *t_day,struct time *t_night,int *day)
{
  struct time t1_day,t1_night;

  if(*day == 0){
// If the work begins and is finished within the same day
    f1(t1,t2,t_day,t_night);
  }/* if */
  else{
// If the work begins and is finished in different days
    if(check(t1,t2)){
// If the number of hours between the beginning and the end of the work
// is more than 24
    f1(t1,t2,t_day,t_night);
    *day=*day+1;
    }//if
    else{
      f1(t1,t_end_n,t_day,t_night);
      f1(t0,t2,&t1_day,&t1_night);

      sum(t_night,t1_night);
      sum(t_day,t1_day);
    }//else
    *day=*day-1;
  }/* elif */
}//funct
/* --------------------------------------------------------------------------------------- */
void f1(struct time t1,struct time t2,struct time *t_day,struct time *t_night){
  struct time tt;

  if(check(t_end_d,t1)){
     if(check(t_end_d,t2)){
     // If t1 > t_end_d && t2 > t_end_d
        f(t1,t2,t_night);
        t_day->ti_hour=0;
        t_day->ti_min=0;
        t_day->ti_sec=0;
     }//if
  } else {
     if(check(t_begin_d,t1)){
        if(check(t_end_d,t2)){
        // If (t_begin_d < t1 < t_end_d) && t2 > t_end_d
           f(t1,t_end_d,t_day);
           f(t_end_d,t2,t_night);
        }
        else{
           if(check(t_begin_d,t2)){
           // If (t_begin_d < t1 < t_end_d) && (t_begin_d < t2 < t_end_d
              f(t1,t2,t_day);
              t_night->ti_hour=0;
              t_night->ti_min=0;
              t_night->ti_sec=0;
           }
        }
     }//if
     else {
        if(check(t0,t1)){
           if(check(t_end_d,t2)){
           // If (t0 < t1 < t_begin_d) && t2 > t_end_d
              f(t_begin_d,t_end_d,t_day);
              f(t_end_d,t2,t_night);
              f(t1,t_begin_d,&tt);
              sum(t_night,tt);
           }//if
           else{
              if(check(t_begin_d,t2)){
              // If (t0 < t1 < t_begin_d) && (t_begin_d < t2 < t_end_d)
                 f(t_begin_d,t2,t_day);
                 f(t1,t_begin_d,t_night);
              }
              else {
                 if(check(t0,t2)){
                 // If (t0 < t1 < t_begin_d) && (t0 < t2 < t_begin_d)
                    f(t1,t2,t_night);
                    t_day->ti_hour=0;
                    t_day->ti_min=0;
                    t_day->ti_sec=0;
                 }
              }
           }//else
        }//if
     }//else
  }//else
}//f1
/* --------------------------------------------------------------------------------------- */
/*   t = t2 - t1                                                                   */
/* --------------------------------------------------------------------------------------- */
void f(struct time t1,struct time t2,struct time *t)
{
  int d,d1,d2;

  t->ti_hour = t2.ti_hour - t1.ti_hour;

  d1=t1.ti_min*60+t1.ti_sec;
  d2=t2.ti_min*60+t2.ti_sec;

  if(d1 <= d2) d=d2-d1;
  else         { d=3600+d2-d1;
                 t->ti_hour--;
               }
  t->ti_min=d/60;
  t->ti_sec=d%60;

}//f
/* --------------------------------------------------------------------------------------- */
/*     return 0 if t2 < t1   and   return 1 if t2 > t1                                     */
/* --------------------------------------------------------------------------------------- */
int check(struct time t1,struct time t2)
{
  int delta;

  if((t2.ti_hour-t1.ti_hour)<0) return 0;
  if((t2.ti_hour-t1.ti_hour)==0)
  { delta=(t2.ti_min-t1.ti_min)*60+(t2.ti_sec-t1.ti_sec);
    if(delta<0) return 0;
    else        return 1;
  }
  else return 1;
}//check
/* --------------------------------------------------------------------------------------- */
/*    t1 = t1+t2                                                                         */
/* --------------------------------------------------------------------------------------- */
void sum(struct time* t1,struct time t2)
{
  t1->ti_hour+=t2.ti_hour;
  t1->ti_min+=t2.ti_min;
  t1->ti_sec+=t2.ti_sec;
  if(t1->ti_sec >= 60) { t1->ti_sec=t1->ti_sec-60;
                    t1->ti_min++;
                  }
  if(t1->ti_min >= 60) { t1->ti_min=t1->ti_min-60;
                    t1->ti_hour++;
                  }
}//sum
/* --------------------------------------------------------------------------------------- */
/*
static int fd(int month,int day,int pr)
{
   int n=0;
   int i;

   for(i=1;i<month;i++)
      n+=month1[i-1];
      n+=day;
      if(!pr && (month>2)) n++;
  return n;
}
*/
/*
int numdate (struct date* d1, struct date* d2){
   int numday;
   int year1,year2,d_year;

   if(d1 && d2 &&
     (d1->da_mon <= 12 && d1->da_mon >= 1)&&
     (d2->da_mon <= 12 && d2->da_mon >= 1)){
      year1=d1->da_year;
      year2=d2->da_year;

      d_year=year2-year1;

      if(!d_year) { int pr=year1%4;
                    numday=fd(d2->da_mon,d2->da_day,pr)-
                    fd(d1->da_mon,d1->da_day,pr)+1;

                  }
      else        { int pr=year1%4;
                    numday=365-fd(d1->da_mon,d1->da_day,pr);
                    if(!pr) numday++;
                    numday+=(d_year-1)*365+d_year/4;
                    pr=year2%4;
                    numday+=fd(d2->da_mon,d2->da_day,pr);
                  }

      return numday;
   }/* if */
   else{
      return -1;
   }/* elif */
}/* numdate */
*/
/* ----------------------------------------------------------------------- */
int CheckAfterDate(struct date* sd, struct time* st,
                    struct date* ed, struct time* et){

   struct date StartDate, EndDate;
   struct time StartTime, EndTime;

   if(sd && ed){

      StartDate = *sd;
      EndDate = *ed;

      if(
         (EndDate.da_year  > StartDate.da_year) ||
         (EndDate.da_year == StartDate.da_year) &&
         ((EndDate.da_mon  > StartDate.da_mon) ||
          (EndDate.da_mon == StartDate.da_mon) &&
          (EndDate.da_day  > StartDate.da_day))
      ){
         return 1;
      }/* if */
      else{
         if(st && et){
            StartTime = *st;
            EndTime = *et;
            StartTime.ti_sec = EndTime.ti_sec = 0;
            if((EndDate.da_year == StartDate.da_year)&&
               (EndDate.da_mon  == StartDate.da_mon )&&
               (EndDate.da_day  == StartDate.da_day )){
               if((EndTime.ti_hour  > StartTime.ti_hour) ||
                  (EndTime.ti_hour == StartTime.ti_hour) &&
                  ((EndTime.ti_min  > StartTime.ti_min) ||
                   (EndTime.ti_min == StartTime.ti_min) &&
                   (EndTime.ti_sec  > StartTime.ti_sec))
               ){
                  return 1;
               }/* if */
            }/* if */
         }/* if */
      }/* elif */

      return 0;

   }/* if */
   else{
      return -1;
   }/* elif */
}/* CheckAfterDate */
/* ----------------------------------------------------------------------- */
// дать название месяца
char* GetMonthName(struct date* d){
   if(d){
      return mname[d->da_mon - 1 % 12];
   }/* if */
   else{
      return "...";
   }/* elif */
}/* GetWeekDayName */
/* ----------------------------------------------------------------------- */
// високосный год
int VisYear(int y){
   return (y % 400 == 0) || (y % 100 != 0) && (y % 4 == 0);
}/* VisYear */
/* ----------------------------------------------------------------------- */
// дней в месяце
int DayInMonth(struct date* d){
   int m = ((d->da_mon >= 1) && (d->da_mon)) <= 12 ? d->da_mon : 1;
   if((m == 2) && (VisYear(d->da_year))){
      month1[1] = 29;
   }/* if */
   else{
      month1[1] = 28;
   }/* elif */
   return month1[m - 1];
}/* DayInMonth */
/* ----------------------------------------------------------------------- */
// дней в году
static int dni(struct date* d1, struct date* d2){
   int y, d;
   d = 0;
   for(y = d1->da_year;y <= d2->da_year - 1;y++){
      if(VisYear(y)){
         d += 366;
      }/* if */
      else{
         d += 365;
      }/* elif */
   }/* for */
   return d;
}/* dni */
/* ----------------------------------------------------------------------- */
// дней от начала года до целого месяца
static int dnd(struct date* d1){
   struct date dt;
   int d, m;
   d = 0;
   dt = *d1;
   for(m = 1;m <= d1->da_mon - 1;m++){
      dt.da_mon = m;
      d += DayInMonth(&dt);
   }/* for */
   return d + d1->da_day;
}/* dnd */
/* ----------------------------------------------------------------------- */
// дней от даты до даты
int NumDays(struct date* d1, struct date* d2){
   if(CheckAfterDate(d1, NULL, d2, NULL) == 0){
      return -(dni(d2, d1) - dnd(d2) + dnd(d1));
   }/* if */
   return dni(d1, d2) - dnd(d1) + dnd(d2);
}/* NumDays */
/* ----------------------------------------------------------------------- */
// дать день недели
int GetWeekDay(struct date* d){
   unsigned wd;
   static struct date ds;
   // известное воскресенье (мой день рожденья!)
   ds.da_year = 1967;
   ds.da_mon  = 6;
   ds.da_day  = 18;
   if(d){
      if(CheckAfterDate(&ds, NULL, d, NULL)){
         wd = NumDays(&ds, d) % 7;
      }/* if */
      else{
         wd = (7 - (-NumDays(&ds, d)) % 7) % 7;
      }/* elif */
      return wd;
   }/* if */
   else{
      return -1;
   }/* elif */
}/* GetWeekDayName */
/* ----------------------------------------------------------------------- */
// дать имя дня недели
char* GetWeekDayName(struct date* da){
   int d = GetWeekDay(da);
   if(d >= 0)
      return wdname[d];
   else
      return "...";
}/* GetWeekDayName */
/* ----------------------------------------------------------------------- */
// дать время дня день/ночь
char GetTimeType(void){
   unsigned long lst, let, ldt;
   struct time st, et;

   GetDayNightTime(&st, &et);
   gettime(&tm);

   lst  = st.ti_hour * 3600UL;
   lst += st.ti_min  * 60UL;
   lst += st.ti_sec;

   let  = et.ti_hour * 3600UL;
   let += et.ti_min  * 60UL;
   let += et.ti_sec;

   ldt  = tm.ti_hour * 3600UL;
   ldt += tm.ti_min  * 60UL;
   ldt += tm.ti_sec;

   if((ldt >= lst) && (ldt <= let)){// сейчас: День
      return 'Д';
   }/* if */
   else{
      return 'Н';
   }/* elif */
}/* GetTimeType */
/* ----------------------------------------------------------------------- */
// установить время дня день/ночь из БД
void SetDNT (void){
  int d;
  static struct time t_day,t_night;

  DBF *db = DBFOpen(24);
  DBFGetRecord(db);
  DBFGetFieldAsTime(db, "ДНАЧАЛО", &t_day);
  DBFGetFieldAsTime(db, "ДКОНЕЦ", &t_night);
  DBFClose(db);
  if(DBFGetErrorNum() != DBF_OK){
     printf("\n\007 Ошибка при чтении базы времени (24) : %s", DBFGetErrorString());
     getch(); exit(-1);
  }/* if */
  SetDayNightTime(t_day, t_night);

}/* SetDNT */
/* ----------------------------------------------------------------------- */
// заполнить календарь на месяц в 025.dbf
void PutMonthCalendar(struct date *dat){
  int d;
  DBF *dbf = NULL;
  struct date td;
  char sType[2] = "*";

  if(dat){
     td = *dat;
  }/* if */
  else{
     td = dt;
  }/* elif */
  // заполнить календарь на тек. месяц
  dbf = DBFOpen(25);
  d = DayInMonth(&td);
  for(td.da_day = 1;td.da_day <= 31;td.da_day++){
     DBFClearRecord(dbf);
     if(td.da_day <= d){
        DBFSetFieldAsInt(dbf, "ЧИСЛО", td.da_day);
        DBFSetFieldAsString(dbf, "ДЕНЬ", GetWeekDayName(&td));
        sType[0] = GetDateType(&td);
        DBFSetFieldAsString(dbf, "ТИП", sType);
     }/* if */
     DBFPutRecord(dbf);
     DBFNext(dbf);
  }/* for */
  DBFClose(dbf);

}/* PutMonthCalendar */
/* ----------------------------------------------------------------------- */
// дать тип дня вых/раб/праздн
char GetDateType(struct date* dat){
  DBF *db;
  struct date td;
  char cdate[16], r = 'Р';
  char *cdf = "%4.4d%2.2d%2.2d";

  if(dat){
     td = *dat;
  }/* if */
  else{
     td = dt;
  }/* elif */

  if(Sunday == TRUE){
     // учитывать воскресенья
     if(GetWeekDay(&td) == 0){// воскресенье - выходной:('В')
        r = 'В';
     }/* if */
  }/* if */

  if(Calendar == TRUE){
     // уставки любого года
     sprintf(cdate, cdf, 0, td.da_mon, td.da_day);
     db = DBFOpen(26);
     DBFFindRecord(db, "ДАТА", cdate);
     if(DBFGetErrorNum() == DBF_OK){
        r = *DBFGetFieldAsString(db, "ТИП");
     }/* if */
     // уставки указанного года
     sprintf(cdate, cdf, td.da_year, td.da_mon, td.da_day);
     DBFStart(db);
     DBFFindRecord(db, "ДАТА", cdate);
     if(DBFGetErrorNum() == DBF_OK){
        r = *DBFGetFieldAsString(db, "ТИП");
     }/* if */
     DBFClose(db);
  }/* if */

  return r;
}/* GetDateType */
/* ----------------------------------------------------------------------- */
// дней в году
int dny(struct date* sd){
   if(VisYear(sd->da_year)){
      return 366;
   }/* if */
   else{
      return 365;
   }/* elif */
}/* dny */
/* ----------------------------------------------------------------------- */
// дату через N дней
int FutureDay(struct date* sd, int d){
   int m, day = d;
   struct date td = *sd;

   // дней с начала года
   for(m = 1;m <= sd->da_mon - 1;m++){
      td.da_mon = m;
      day += DayInMonth(&td);
   }/* for */
   // дней с начала месяца
   day += td.da_day;

   // сколько лет
   while(day > dny(&td)){
      day -= dny(&td);
      td.da_year += 1;
   }/* while */

   // сколько месяцев
   td.da_mon = 1;
   while(day > DayInMonth(&td)){
      day -= DayInMonth(&td);
      td.da_mon += 1;
   }/* while */

   // и дней
   td.da_day = day;

   *sd = td;

   return TRUE;
}/* FutureDay */
/* ----------------------------------------------------------------------- */
// дату через N часов
int FutureHour(struct date* sd, struct time* st, int hour){
   struct date td;
   struct time tt;
   int d = hour / 24;
   int h = hour % 24;

   if(st->ti_hour + h >= 24){
      d += 1;
      st->ti_hour += h;
      st->ti_hour %= 24;
   }/* if */
   else{
      st->ti_hour += h;
   }/* elif */
   if(d){
      FutureDay(sd, d);
   }/* if */

   return TRUE;
}/* FutureHour */
/* ----------------------------------------------------------------------- */
// часов от даты до даты
int GetAfterHour(struct date* d1, struct date* d2){
   return NumDays(d1, d2) * 24;
}/* GetAfterHour */
/* ----------------------------------------------------------------------- */
