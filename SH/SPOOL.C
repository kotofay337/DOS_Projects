/* ----------------------------------------------------------------------- */
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <dir.h>
#include <conio.h>
#include <string.h>
#include <bios.h>
#include <dos.h>
#include <time.h>
#include <ltool.h>
#include "data.h"
/* ----------------------------------------------------------------------- */
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

typedef
   struct FILES {
      struct FILES *next;
      char PrintJob[MAXPATH]; // имя файла в спулере хранящем содержимое
                              // документа для печати
      long CurrentPosPrint;   // позиция в файле с которой начнется печать
   }FILES, *LPFILES;

typedef
   struct SPOOL {
      LPFILES head;
      LPFILES tail;
      LPFILES current;
   }SPOOL, *LPSPOOL;

typedef
   struct SPOOLER {
      SPOOL Spool[3];
      PS    sLPT [3];
   }SPOOLER, *LPSPOOLER;
/* ----------------------------------------------------------------------- */
static SPOOLER Spooler;
/* ----------------------------------------------------------------------- */
static int dLPT(int nLPT){
   // Порт для данного LPT
   return *((int*)MK_FP(0, nLPT == 1 ? 0x0408 :
                           nLPT == 2 ? 0x040a :
                           nLPT == 3 ? 0x040c : 0x0408));
}/* dLPT */
/* ----------------------------------------------------------------------- */
static int InitLPT(int nLPT){
   PC pc;
   int bLPT = dLPT(nLPT);
   if(bLPT){
      // Инициализируем принтер по nLPT
      biosprint(1, 0, nLPT - 1);
   }/* if */
   return bLPT;
}/* InitLPT */
/* ----------------------------------------------------------------------- */
static PS PrintByte(int nLPT, char c){
      int i;
      PS ps;
      PC pc;
      long bt;
      // Получаем статус принтера
      ps.b = ((biosprint(2, 0, nLPT - 1) >> 3) & 0x1F);
      if(!ps.ps.Error && !ps.ps.OutOfPr){
         // Пишем байт
         ps.b = ((biosprint(0, (int)c, nLPT - 1) >> 3) & 0x1F);
         // Цикл обмена завершен
         return ps;
      }/* if */
      return ps;
}/* PrintByte */
/* ----------------------------------------------------------------------- */
static int PrintCurrentJob(int nLPT, LPSPOOL pSpool){
   LPFILES cpj;
   PS s;
   int bLPT = dLPT(nLPT);
   FILE *fj;
   static unsigned char c;
   // выбрать из очереди первый элемент
   // если тек.поз в файле 0 то инициализ. принтер
   //      открыть файл и напечатать байт закрыть файл
   //      продвинуть указ. вперед
   // иначе
   //   если тек.поз в файле больше макс. кол-ва символов в файле
   //        то файл закрыть и удалить элемент из очереди
   //   иначе открыть файл с тек.поз. напечатать байт закрыть файл
   //         продвинуть тек.поз.
   //
   // общие требования:
   //  перед печатью байта проверять отсутствие бумаги
   //  и готовность принтера


   cpj = (LPFILES)ltohead((LPGENERIC_HEADER)pSpool);
   if(cpj){
      // порт не определен в системе
      if(bLPT == 0){
         unlink(cpj->PrintJob);
         free(ldel_head((LPGENERIC_HEADER)pSpool));
         return 0;
      }/* if */

      fj = fopen(cpj->PrintJob, "rb");
      if(cpj->CurrentPosPrint == 0L){
         s.b = ((biosprint(2, 0, nLPT - 1) >> 3) & 0x1F);
         if(!s.ps.OutOfPr){
            bLPT = InitLPT(nLPT);
         }/* if */
         else{
            fclose(fj);
            return 0;
         }/* elif */
      }/* if */
      else{
         if(cpj->CurrentPosPrint >= filelength(fileno(fj))){
            fclose(fj);
            unlink(cpj->PrintJob);
            free(ldel_head((LPGENERIC_HEADER)pSpool));
            return 0;
         }/* if */
         else{
            fseek(fj, cpj->CurrentPosPrint, SEEK_SET);
         }/* elif */
      }/* elif */
      fread(&c, 1, 1, fj);
      fclose(fj);
      s = PrintByte(nLPT, c);
      if(s.ps.OutOfPr){
         // если кончилась бумага во время печати документа
         // то начать печать сначала
         cpj->CurrentPosPrint = 0L;
      }/* if */
      else{
         cpj->CurrentPosPrint += 1L;
      }/* elif */
      return 0;
   }/* if */
   return 0;
}/* PrintCurrentJob */
/* ----------------------------------------------------------------------- */
int PrintSpooler(void){
   int nLPT;
   int Stop = 1;
   // отработать для всех принтеров
   for(nLPT = 0;nLPT < 3;nLPT++){
      PrintCurrentJob(nLPT + 1, &Spooler.Spool[nLPT]);
      if(Spooler.Spool[nLPT].head){
         Stop = 0;
      }/* if */
   }/* for */
   return Stop;
}/* PrintSpooler */
/* ----------------------------------------------------------------------- */
int AddSpool(int nLPT, char *fn){
   LPFILES pj;
   if(nLPT <= 0 || nLPT > 3){
      return 0;
   }/* if */
   pj = calloc(1U, sizeof(FILES));
   strcpy(pj->PrintJob, fn);
   pj->CurrentPosPrint = 0L;
   ladd_tail((LPGENERIC_HEADER)&Spooler.Spool[nLPT - 1], (LPGENERIC_LIST)pj);
   return 0;
}/* AddSpool */
/* ----------------------------------------------------------------------- */
