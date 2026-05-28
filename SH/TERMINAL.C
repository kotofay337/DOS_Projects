/* ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <bios.h>
#include <dos.h>
#include <time.h>
#include "tools\com.h"
/* ----------------------------------------------------------------------- */
typedef
   union{
      struct{
         unsigned TimeOut:1;
                         :2;
         unsigned IOError:1;
         unsigned Select :1;
         unsigned OutOfPr:1;
         unsigned Ack    :1;
         unsigned Notbusy:1;
                         :8;
      }ps;
      unsigned word;
   }PrnStat;

typedef
   union{
      struct{
         unsigned Error  :1;
         unsigned Select :1;
         unsigned OutOfPr:1;
         unsigned Ack    :1;
         unsigned Busy   :1;
      }ps;
      unsigned char b;
   }PS;

typedef
   union{
      struct{
         unsigned Strobe :1;
         unsigned AutoCR :1;
         unsigned Init   :1;
         unsigned SelIn  :1;
         unsigned IntOn  :1;
      }pc;
      unsigned char b;
   }PC;

enum{
   WRITE  = 0,
   INIT   = 1,
   STATUS = 2
};

/* ----------------------------------------------------------------------- */
static unsigned char bsend;
static unsigned char brecv[1024];
static b_COM_PORT_DESC *p;
static char *testf = "Test port COM%c: OK!\x0d\x0a";
static char buf[255] = {"Test port COM: OK! \x0d\x0a"};
/* ----------------------------------------------------------------------- */
int c_break(void)
{
   bsend = 3;
   b_ComSend(p, &bsend, 1);
   return 1;
}
/* ----------------------------------------------------------------------- */
void StatPRN(PrnStat sPrinter){
   printf("\rLPT%d: %s %s %s %s %s %s", 1,
       sPrinter.ps.TimeOut ? "Timeout  " : "         ",
       sPrinter.ps.IOError ? "IOError  " : "         ",
       sPrinter.ps.Select  ? "Select   " : "         ",
       sPrinter.ps.OutOfPr ? "OutPaper " : "         ",
       sPrinter.ps.Ack     ? "Ack      " : "         ",
       sPrinter.ps.Notbusy ? "NotBusy  " : "         ");
}/* StatPRN */
/* ----------------------------------------------------------------------- */
int dLPT(int LPTNum){
   int* Addr = MK_FP(0, LPTNum == 1 ? 0x0408 :
                        LPTNum == 2 ? 0x040a :
                        LPTNum == 3 ? 0x040c : 0x0408);
   if(*Addr != 0){
      return *Addr;
   }/* if */
   else{
      return -1;
   }/* elif */
}/* dLPT */
/* ----------------------------------------------------------------------- */
void cLPT(int bLPT, unsigned char d){
   outportb(bLPT + 2, (d ^ 0x0b) ^ 0x0e);
}/* cLPT */
/* ----------------------------------------------------------------------- */
void wLPT(int bLPT, unsigned char d){
   outportb(bLPT, d);
}/* wLPT */
/* ----------------------------------------------------------------------- */
unsigned char sLPT(int bLPT){
   return ((inportb(bLPT + 1) >> 3) ^ 0x11);
}/* sLPT */
/* ----------------------------------------------------------------------- */
int main (int argc, char *argv[]){

   { // Direct out port Printing
   /*
      Протокол работы с Centronics:
    1 Инициализация порта:
         Установть сигналы SelectIn и Init.
         Ждать 0.5 мксек
         Сбросить Init.

    2 Проверить сигнал Busy
      Если !Занят(Busy) и !Конец бумаги(EndPaper) и Online(Select) то
         отправить байт в порт данных,
         выставить сигнал Strobe,
         подождать 0.5 мксек.
         снять сигнал Strobe,
         ждать сигнал Ack 5 сек.
         если время вышло
            выдать ошибку: Timeout:Принтер не отвечает.
         иначе
            конец обмена.
   */
      char ts[] = "Test string: Direct printing.\n\r";
      int i;
      int bLPT1;
      PS ps;
      PC pc;

      pc.b = 0;

      // init LPT1
      bLPT1 = dLPT(1);
      if(bLPT1 == -1){
         printf("\nLPT1 is absent.");
         return 0;
      }/* if */
      printf("\nLPT1 present. 0x%x\n", bLPT1);

      // Инициализируем по LPT1
      pc.pc.Init = 1;
      pc.pc.SelIn = 1;
      cLPT(bLPT1, pc.b);
      delay(1);
      pc.pc.Init = 0;
      cLPT(bLPT1, pc.b);

      i = 0;
      while(!kbhit()){
         long bt;
         // Получаем статус принтера
         ps.b = sLPT(bLPT1);

         // ЕСЛИ: не занят, нет ошибок, и есть бумага
//         if(!ps.ps.Busy && !ps.ps.Error && !ps.ps.OutOfPr){
         if(!ps.ps.OutOfPr){

            // Пишем байт
            wLPT(bLPT1, ts[i++ % sizeof(ts)]);
            // Стробируем его
            pc.pc.Strobe = 1;
            cLPT(bLPT1, pc.b);
            delay(1);
            {// Delay strobe: 1 mcsec.
               register i;
               for(i = 0;i < 5;i++)
                  inportb(bLPT1);
            }
            pc.pc.Strobe = 0;
            cLPT(bLPT1, pc.b);

            // Ждем сигнала Acknowledge
            ps.b = sLPT(bLPT1);
            bt = biostime(0, 0L);
            while(!ps.ps.Ack){
               ps.b = sLPT(bLPT1);
               if((biostime(0, 0L) - bt) / CLK_TCK > 5.0f){
                  printf("\nTime Out!");
                  break;
               }/* if */
            }/* while */

            // Цикл обмена завершен

         }/* if */
         else{
            printf("\r%s%s%s%s%s",
                ps.ps.Error   ? "Error    " : "         ",
                ps.ps.Select  ? "Online   " : "         ",
                ps.ps.OutOfPr ? "EndPaper " : "         ",
                ps.ps.Ack     ? "Ack      " : "         ",
                ps.ps.Busy    ? "Busy     " : "         ");
         }/* elif */
      }/* while */
      getch();

   }

   return 0;

/*
   { // BIOS Printing
      int i;
      static char bs[] = "1234567890\n";
      PrnStat sPrinter;

      sPrinter.word = biosprint(INIT, 0, 0);
      StatPRN(sPrinter);

      while(!kbhit()){
         sPrinter.word = biosprint(STATUS, 0, 0);
         StatPRN(sPrinter);
      }/* while */
      getch();

      for(i = 0;;){
         if(kbhit()){
            if(getch() == 27){
               biosprint(STATUS, 0, 0);
               return 0;
            }/* if */
         }/* if */

         if(sPrinter.ps.Select  && !(sPrinter.ps.OutOfPr && sPrinter.ps.Ack && sPrinter.ps.IOError)){
            printf("%c", bs[i % sizeof(bs)]);
            sPrinter.word = biosprint(WRITE, bs[i % sizeof(bs)], 0);
            StatPRN(sPrinter);
            i++;
         }/* if */
         else{
            sPrinter.word = biosprint(STATUS, 0, 0);
            StatPRN(sPrinter);
         }/* elif */

      }/* for */
   }
*/

/*
   int i, size, es;

   clrscr();

   if(argc == 2){
      p = b_ComInit(argv[1][0] == '1' ? _COM1 :
                    argv[1][0] == '2' ? _COM2 : _COM1);
      if(argv[1][0] >= '1' && argv[1][0] <= '2'){
         printf(testf, argv[1][0]);
      }/* if */
   }/* if */
   else{
      p = b_ComInit(_COM1_TEST);
   }/* elif */

   b_ComStart(p);

   ctrlbrk(c_break);

   while(1){
      if(b_ComCheckBuf(p) == TRUE){
         size = b_ComRecv(p, brecv);
/*
         for(i = 0;i < size;i++)
            printf("\n%c", brecv[i], brecv[i]);
         printf("\n");
*/
         for(i = 0;i < size;i++)
            printf("\n%2.2X ", brecv[i], brecv[i]);
//         printf("\n");

      }/* if */
      if(kbhit()){
	    es = getch();
	    if(es == 0){
	       es = getch();
	       switch(es){
		  case 79:
		     b_ComStop(p);
		     return 0;
		  case 73:
		     bsend = 0;
		     b_ComSend(p, &bsend, 1);
		     break;
	       }/* switch */
	    }/* if */
	    else{
	       if(es == 32){
		       clrscr();
             b_ComSend(p, buf, strlen(buf));
	       }/* if */
          else{
             bsend = es;
             b_ComSend(p, &bsend, 1);
          }/* elif */
	    }/* elif */
      }/* if */
   }/* while */

*/
   return 0;

}/* main */
/* ----------------------------------------------------------------------- */
