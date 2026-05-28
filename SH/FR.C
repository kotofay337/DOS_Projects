/* -----------------------------------------------------------------------
*
*  FR.C
*
*  ----------------------------------------------------------------------- */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <alloc.h>
#include <dos.h>
#include <bios.h>
#include <time.h>
#include <graphics.h>
#include "tbg_tc.h"
#include "com.h"
#include "dbf.h"
#include "data.h"

/* ----------------------------------------------------------------------- */
#define STX 0x02
#define ETX 0x03
#define FRSIZE (1024 * 6)

typedef
   enum{
      Start,
      SetRekvize,
      BCCCalc,
      SendBlock,
      WaitAck,
      ReceiveAck,
      AckDecode,
      Stop,
      WaitOutOfPaper,
      WaitErrorOut
   }STEP;

// статус ФР
typedef
   struct {
      unsigned Smena    : 1;
      unsigned          : 3;
      unsigned Fiscalize: 1;
      unsigned FMemEnogh: 1;
      unsigned FMemNot  : 1;
      unsigned          : 1;
   }SFR;

static char *sErrorFR[] = {
/* 00 */ NULL,
/* 01 */"Ошибка в фискальных данных, аппарат блокирован",
/* 02 */"Не закрыта смена",
/* 03 */"Исчерпан ресурс сменных записей в фискальную память",
/* 04 */"Превышена длина поля команды",
/* 05 */"Неверный формат поля команды",
/* 06 */"Ошибка чтения таймера",
/* 07 */"Неверная дата",
/* 08 */"Неверное время",
/* 09 */"Дата меньше последней даты, зарегистрированой в фискальной памяти",
/* 0A */"Операция прервана пользователем, документ аннулирован",
/* 0B */"Запрещенная команда ПУ",
/* 0C */"Не открыта смена",
/* 0D */ NULL,
/* 0E */"Переполнение приемного буфера",
/* 0F */"Ошибка записи в фискальную память",
/* 10 */"Ошибка установки таймера",
/* 11 */"Неверный пароль НИ",
/* 12 */"Неверный пароль на связь",
/* 13 */"Исчерпан ресурс перерегистраций",
/* 14 */"Аппарат не фискализирован",
/* 15 */"Значение поля команды вне диапазона",
/* 16 */"Ошибка чтения фискальной памяти",
/* 17 */"Переполнение счетчика",
/* 18 */"Обязательное поле команды имеет нулевую длину",
/* 19 */"Неверный формат команды",
/* 1A */"Дата или время последнего документа в смене меньше предыдущего",
/* 1B */ NULL,
/* 1C */"Ошибка в расположении реквизитов",
/* 1D */"Нет такой команды",
/* 1E */"Неверная контрольная сумма",
/* 1F */"Нет фискальных записей",
/* 20 */ NULL,
/* 21 */ NULL,
/* 22 */ NULL,
/* 23 */ NULL,
/* 24 */"Буфер ответа пуст",
/* 25 */"Услуга не введена"
};

static STEP Step = Start, SaveStep;
unsigned long t;
unsigned char c;

/* ----------------------------------------------------------------------- */
/* вычисляет контрольную сумму блока для передачи в ФР */
static unsigned char CalcBCC(char *a, int s){
   register unsigned char BCC = 0;
   register int i;
   for(i = 0;i < s;i++){
      BCC += a[i];
   }/* for */
   return BCC;
}/* CalcBCC */
/* ----------------------------------------------------------------------- */
static unsigned RecordBufSend(char* b, char* s, int n){
   static unsigned CurrentIndex = 0;

   if(n == -1){
      return CurrentIndex;
   }/* if */

   if(b == NULL){ // reset buf
      CurrentIndex = 0;
      return 0;
   }/* if */

   if(n > 0 && s && b){
      memcpy(&(b[CurrentIndex]), s, n);
      CurrentIndex += n;
      return CurrentIndex;
   }/* if */

   return -1;
}/* RecordBufSend */
/* ----------------------------------------------------------------------- */
#pragma argsused
int PrintFR (double tarif){
   static int ir;
   static char *FRBuf;
   char rz[41];

   if(FROnOff == FROff){
      return 1;
   }/* if */

   if(Step == Start){
       RecordBufSend(NULL, NULL, 0); // reset
       FRBuf = calloc(1U, FRSIZE);
       if(FRBuf == NULL){
          Step = WaitErrorOut;
       }/* if */
       Step = SetRekvize;
       return 0;
   }/* if */

   if(Step == SetRekvize){

      memset(rz, 0, sizeof(rz));

      RecordBufSend(FRBuf, "\x02", 1);// STX
      if(tarif > 0.0){ // Фискальный документ
         RecordBufSend(FRBuf, "\x33", 1);// CodeMess

         RecordBufSend(FRBuf, FRP, 5);// Password

         RecordBufSend(FRBuf,    "0", 2);// Продажа
         RecordBufSend(FRBuf,   "15", 3);// Кол-во реквизитов

         // реквизит1
         RecordBufSend(FRBuf,   "01", 3);// тип: 01: Наименование уч-я
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "10", 3);// см по гор.
         RecordBufSend(FRBuf,   "00", 3);// см по гор.
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит2
         RecordBufSend(FRBuf,   "00", 3);// тип: 00: Номер ФР
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "00", 3);// см по гор.
         RecordBufSend(FRBuf,   "02", 3);// см по гор.
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит3
         RecordBufSend(FRBuf,   "10", 3);// тип: 10:ИНН
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "20", 3);// см по гор.
         RecordBufSend(FRBuf,   "02", 3);// см по гор.
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит4
         RecordBufSend(FRBuf,   "02", 3);// тип: 02:Дата
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "00", 3);// см по гор.
         RecordBufSend(FRBuf,   "03", 3);// см по гор.
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит5
         RecordBufSend(FRBuf,   "03", 3);// тип: 03:Время
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "12", 3);// см по гор.
         RecordBufSend(FRBuf,   "03", 3);// см по гор.
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит6
         RecordBufSend(FRBuf,   "04", 3);// тип: 04:Номер докум
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "20", 3);// см по гор.
         RecordBufSend(FRBuf,   "03", 3);// см по гор.
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит7
         RecordBufSend(FRBuf,   "06", 3);// тип: 06:Номер оператора
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "00", 3);// см по гор.
         RecordBufSend(FRBuf,   "04", 3);// см по гор.
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит8
         RecordBufSend(FRBuf,   "09", 3);// тип: 09:Номер счета
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "12", 3);// см по гор.
         RecordBufSend(FRBuf,   "04", 3);// см по гор.
         sprintf(rz, "text 1");
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит9
         RecordBufSend(FRBuf,   "07", 3);// тип: 07:Номер чека
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "20", 3);// см по гор.
         RecordBufSend(FRBuf,   "04", 3);// см по гор.
         memset(rz, 0, sizeof(rz));
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит10
         RecordBufSend(FRBuf,   "99", 3);// тип: 99:Доп
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "00", 3);// см по гор.
         RecordBufSend(FRBuf,   "05", 3);// см по гор.
         sprintf(rz, " ");
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит11
         RecordBufSend(FRBuf,   "99", 3);// тип: 99:Доп
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "00", 3);// см по гор.
         RecordBufSend(FRBuf,   "06", 3);// см по гор.
         memset(rz, 0, sizeof(rz));
         sprintf(rz, "Стоянка № %2.2u", NPost);
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит12
         RecordBufSend(FRBuf,   "05", 3);// тип: 05: сумма услуги
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "08", 3);// см по гор.
         RecordBufSend(FRBuf,   "06", 3);// см по гор.
         memset(rz, 0, sizeof(rz));
         sprintf(rz, "%4.2lf", tarif);
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит13
         RecordBufSend(FRBuf,   "08", 3);// тип: 08: итого сумма
         RecordBufSend(FRBuf,    "0", 2);// флаги
         RecordBufSend(FRBuf,   "00", 3);// см по гор.
         RecordBufSend(FRBuf,   "07", 3);// см по гор.
         memset(rz, 0, sizeof(rz));
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит14
         RecordBufSend(FRBuf,   "11", 3);// тип: 11: уплачено
         RecordBufSend(FRBuf, "\x43", 2);// флаги
         RecordBufSend(FRBuf,   "00", 3);// см по гор.
         RecordBufSend(FRBuf,   "08", 3);// см по гор.
         sprintf(rz, "%4.2lf", tarif);
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         // реквизит15
         RecordBufSend(FRBuf,   "12", 3);// тип: 12: сдача
         RecordBufSend(FRBuf, "\x63", 2);// флаги
         RecordBufSend(FRBuf,   "00", 3);// см по гор.
         RecordBufSend(FRBuf,   "09", 3);// см по гор.
         memset(rz, 0, sizeof(rz));
         RecordBufSend(FRBuf,     rz, sizeof(rz));// реквизит- строка

         RecordBufSend(FRBuf,    "0", 2);// вид оплаты: нал
      }/* if */

      if(tarif == -1.0){
         RecordBufSend(FRBuf, "\x35", 1);// CodeMess

         RecordBufSend(FRBuf, FRP, 5);// Password

         RecordBufSend(FRBuf, "\x30", 2);// X отчёт(фин. отчёт)
      }/* if */

      if(tarif == -2.0){
         RecordBufSend(FRBuf, "\x35", 1);// CodeMess

         RecordBufSend(FRBuf, FRP, 5);// Password

         RecordBufSend(FRBuf, "\x32", 2);// Z отчёт(закрыть смену)
      }/* if */

      Step = BCCCalc;
      return 0;
   }/* if */

   if(Step == BCCCalc){
      char s[16];
      sprintf(s,"%2.2X\x3",CalcBCC(&FRBuf[1], RecordBufSend(NULL, NULL, -1) - 1));
      RecordBufSend(FRBuf, s, 3);// BCC + ETX
      Step = SendBlock;
      if(Emulate)
         Step = Stop;
      return 0;
   }/* if */

   if(Step == SendBlock){
      // читаем остатки
      b_ComRecv(FRCOMPort, NULL);
      // пишем в порт
      b_ComSend(FRCOMPort, FRBuf, RecordBufSend(NULL, NULL, -1));// get size
      memset(FRBuf, 0, FRSIZE);
      Step = WaitAck;
//      Step = Stop;
      t = bt;
      ir = 0;
      return 0;
   }/* if */

   if(Step == WaitAck){
      OutTime(p);

      if(b_ComCheckBuf(FRCOMPort)){
         PrnStat ps;
         t = bt;
         c = b_ComRecvb(FRCOMPort);

         switch(c){
            case 0x00:// ???
            case 0x06:// wait FR
               return 0;
            case 0x05:// error printing
               // ждем символ статуса принтера ФР
               while(!b_ComCheckBuf(FRCOMPort)){
                  OutTime(p);
                  if((bt - t) / CLK_TCK > 2.0F){
                     Step = WaitErrorOut;
                     ErrorMsg("Ошибка пер. ФР ");// ошибка передачи
                     return 0;
                  }/* if */
               }/* while */

               ps.word = b_ComRecvb(FRCOMPort);
               if(ps.ps.IOError || ps.ps.TimeOut){
                  InfoMsg("   Ошибка ФР!   "
                          "Печать отменена.");
                  b_ComSend(FRCOMPort, "\x1", 1);// cancel print
                  Step = WaitErrorOut;
                  return 0;
               }/* if */

               if(ps.ps.OutOfPr){
                  SaveStep = Step;
                  InfoMsg("ФР  Нет бумаги: "
                          "Esc - отмена    "
                          "П   - продолжить");
                  Step = WaitOutOfPaper;
                  return 0;
               }/* if */
               break;
            case 0x03: // STX
               Step = Stop;
               return 0;
         }/* switch */
      }/* if */
      else{
         OutTime(p);
         // время макс ожидания ответа от ФР в сек. сейчас 4 сек.
         if(((bt - t) / CLK_TCK) > 4.0F){
            Step = WaitErrorOut;
            ErrorMsg("ФР не отвечает.");
         }/* if */
      }/* elif */
      return 0;
   }/* if */

   if(Step == Stop){
      free(FRBuf);
      Step = Start;
      return 1;
   }/* if */

   if(Step == WaitErrorOut){
      OutTime(p);
      switch(Key){
         case ENTER:
            free(FRBuf);
            Step = Start;
            return -1;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WaitOutOfPaper){
      OutTime(p);

      if(Key == 'r' || Key == 'R' ||
         Key == 'п' || Key == 'П' ){ // Повтор печати
         b_ComSend(FRCOMPort, "\x4", 1);// resume print
         Step = SaveStep;
         InfoMsg(NULL);
         t = bt;
      }/* if */

      if(Key == ESC){// Отмена печати
         Step = Stop;
         InfoMsg(NULL);
      }/* if */

      return 0;
   }/* if */

   return -1;

}/* PrintFR */
/* ----------------------------------------------------------------------- */
#pragma argsused
int OpenSm (unsigned OpNum, char* FIO){
   static int ir;
   static char *FRBuf;
   char rz[41], NOp[3];

   if(FROnOff == FROff){
      return 1;
   }/* if */

   if(Step == Start){
       RecordBufSend(NULL, NULL, 0); // reset
       FRBuf = calloc(1U, FRSIZE);
       if(FRBuf == NULL){
          Step = WaitErrorOut;
       }/* if */
       Step = SetRekvize;
       return 0;
   }/* if */

   if(Step == SetRekvize){

      memset(rz, 0, sizeof(rz));
      memset(NOp, 0, sizeof(NOp));

      RecordBufSend(FRBuf, "\x02", 1);// STX
      RecordBufSend(FRBuf, "\x31", 1);// CodeMess

      RecordBufSend(FRBuf, FRP, 5);// Password

      sprintf(NOp, "%2.2u", OpNum);
      RecordBufSend(FRBuf,   NOp, 3);// Номер оператора

      strncpy(rz, FIO, sizeof(rz) - 1);
      RecordBufSend(FRBuf,     rz, sizeof(rz));// Фамилия оператора

      Step = BCCCalc;
      return 0;
   }/* if */

   if(Step == BCCCalc){
      char s[16];
      sprintf(s,"%2.2X\x3",CalcBCC(&FRBuf[1], RecordBufSend(NULL, NULL, -1) - 1));
      RecordBufSend(FRBuf, s, 3);// BCC + ETX
      Step = SendBlock;
      if(Emulate)
         Step = Stop;
      return 0;
   }/* if */

   if(Step == SendBlock){
      // читаем остатки
      b_ComRecv(FRCOMPort, NULL);
      // пишем в порт
      b_ComSend(FRCOMPort, FRBuf, RecordBufSend(NULL, NULL, -1));// get size
      memset(FRBuf, 0, FRSIZE);
      Step = WaitAck;
//      Step = Stop;
      t = bt;
      ir = 0;
      return 0;
   }/* if */

   if(Step == WaitAck){
      OutTime(p);
      if(b_ComCheckBuf(FRCOMPort)){
         PrnStat ps;
         t = bt;
         c = b_ComRecvb(FRCOMPort);
         switch(c){
            case 0x00:// ???
            case 0x06:// wait FR
               return 0;
            case 0x05:// error printing
               // ждем символ статуса принтера ФР
               while(!b_ComCheckBuf(FRCOMPort)){
                  OutTime(p);
                  if((bt - t) / CLK_TCK > 1.0F){
                     Step = WaitErrorOut;
                     ErrorMsg("Ошибка перед.  "
                              "ФР не отвечает.");
                     Step = WaitErrorOut;
                     return 0;
                  }/* if */
               }/* while */

               ps.word = b_ComRecvb(FRCOMPort);
               if(ps.ps.IOError || ps.ps.TimeOut){
                  InfoMsg("   Ошибка ФР!   "
                          "Печать отменена.");
                  b_ComSend(FRCOMPort, "\x1", 1);// cancel print
                  Step = WaitErrorOut;
                  return 0;
               }/* if */

               if(ps.ps.OutOfPr){
                  SaveStep = Step;
                  InfoMsg("  Нет бумаги:   "
                          "Esc - отмена    "
                          "П   - продолжить");
                  Step = WaitOutOfPaper;
                  return 0;
               }/* if */
               break;
            case 0x03: // STX
               Step = Stop;
               return 0;
         }/* switch */
      }/* if */
      else{
         OutTime(p);
         // время макс ожидания ответа от ФР в сек. сейчас 5 сек.
         if(((bt - t) / CLK_TCK) > 5.0F){
            Step = WaitErrorOut;
            ErrorMsg("ФР не отвечает.");
         }/* if */
      }/* elif */
      return 0;
   }/* if */

   if(Step == Stop){
      free(FRBuf);
      Step = Start;
      return 1;
   }/* if */

   if(Step == WaitErrorOut){
      OutTime(p);
      if(Key == ENTER){
         free(FRBuf);
         Step = Start;
         return -1;
      }/* if */
      return 0;
   }/* if */

   if(Step == WaitOutOfPaper){
      OutTime(p);

      if(Key == 'r' || Key == 'R' ||
         Key == 'п' || Key == 'П' ){ // Повтор печати
         b_ComSend(FRCOMPort, "\x4", 1);// resume print
         Step = SaveStep;
         InfoMsg(NULL);
         t = bt;
      }/* if */

      if(Key == ESC){// Отмена печати
         Step = Stop;
         InfoMsg(NULL);
      }/* if */

      return 0;
   }/* if */

   return -1;

}/* OpenSm */
/* ----------------------------------------------------------------------- */
