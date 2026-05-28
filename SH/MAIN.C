/* -----------------------------------------------------------------------
*
*  MAIN.C
*
*  Main program
*  master cycle
*
*  ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <dir.h>
#include <conio.h>
#include <string.h>
#include <alloc.h>
#include <process.h>
#include <dos.h>
#include <io.h>
#include <bios.h>
#include <time.h>
#include <graphics.h>
#include <ctype.h>
#include "tbg_tc.h"
#include "dbf.h"
#include "data.h"
#include "editstr.h"
#undef DEBUG

#line 30

/* ----------------------------------------------------------------------- */
typedef
   enum{
      STARTUP,
      WAITSTARTUP,
      LOGIN,
      WAITSCAN,
      MANUALINPUT,
      GETBASE,
      WORKADMINSYS,
      WORKADMINFR,
      WORKPOST,
      OPENSMENA,
      STOPPROGRAM,
      WAIT,
      WAITERROR,
      WAITERRORHARDWARE
   }STEP;

static STEP Step = STARTUP;

float t1 = 0.0F;
static char far *image=NULL;
static unsigned max_pixx=0,_ft=OFF;
static OBJECT **m = NULL;
static OBJECT **pd = NULL;
static char stringedit[15];
static int editobj = 100, editnum = 0, PostMode, Direction1 = 0, Direction2 = 0;
static LPDBF db;
static char sFIO[33];

#define IGNORE  0
#define RETRY   1
#define ABORT   2
/* ----------------------------------------------------------------------- */
int ErrorMsg(unsigned char *msg){
   auto OBJECT **pe = NULL;
   unsigned char buf[3][16];

   memset(buf, 0, sizeof(buf));

   strncpy((char*)buf, msg, 16 * 3 - 1);

   buf[0][15] = 0;
   buf[1][15] = 0;
   buf[2][15] = 0;

   if(load_file(&pe, "tbg\\errormsg.tbg") == 0){
      outpic_all(pe);
      outtext_obj_num(pe, 0, 0, EGA_LIGHTGRAY, buf[0], SHOW);
      outtext_obj_num(pe, 0, 1, EGA_LIGHTGRAY, buf[1], SHOW);
      outtext_obj_num(pe, 0, 2, EGA_LIGHTGRAY, buf[2], SHOW);
      del_pic(&pe);
   }/* if */
   else{
      print_error_tbg("In ErrorMsg");
      printf("\nError: %s", msg);
   }/* elif */

   return 1;
}/* ErrorMsg */
/* ----------------------------------------------------------------------- */
int InfoMsg(unsigned char *msg){
   auto OBJECT **pi = NULL;
   unsigned char buf[3][16];
   auto char* sIm = "tbg\\infomsg.img";
   auto char* sP = "tbg\\infomsg.tbg";

   if(msg != NULL){
      memset(buf, 0, sizeof(buf));

      strncpy((char*)buf, msg, 16 * 3 - 1);

      buf[0][15] = 0;
      buf[1][15] = 0;
      buf[2][15] = 0;

      if(load_file(&pi, sP) == 0){
         Store_Image(pi, sIm);
         outpic_all(pi);
         outtext_obj_num(pi, 0, 0, EGA_LIGHTGRAY, buf[0], SHOW);
         outtext_obj_num(pi, 0, 1, EGA_LIGHTGRAY, buf[1], SHOW);
         outtext_obj_num(pi, 0, 2, EGA_LIGHTGRAY, buf[2], SHOW);
         del_pic(&pi);
      }/* if */
      else{
         print_error_tbg("In InfoMsg");
         printf("\nVarning: %s", msg);
      }/* elif */
      printf(sBell);
   }/* if */
   else{
      if(access(sIm, 0) != 0)
         return 1;
      if(load_file(&pi, sP) == 0){
         Restore_Image(pi, sIm);
         del_pic(&pi);
         unlink(sIm);
      }/* if */
   }/* elif */
   return 1;
}/* InfoMsg */
/* ----------------------------------------------------------------------- */
void OutTime(OBJECT *p){
   static unsigned long tt2 = 0;
   char *st = "00:00";
   if(p == NULL)
      return;
   if((bt - tt2) >= 1L){
      setcolor_obj_num(p, 30, 2, tm.ti_sec % 2 ? -1 : -2, SHOW);
      sprintf(st, "%2d%c%02d", tm.ti_hour, tm.ti_sec % 2 ? ':' : '.', tm.ti_min);
      outtext_obj_num(p, 30, 1, getcolor_obj_num(p, 30, 0), st, NO_SHOW);
      tt2 = bt;
   }/* if */
}/* OutTime */
/* ----------------------------------------------------------------------- */
void OutDate(void){

   static long t = 0L;

   char st[64];

   if(((bt - t) / CLK_TCK) >= 5.0F || bt == 0){
      t = bt;

      if(pd == NULL)
         load_file(&pd, "tbg\\datebar.tbg");

      if(dt.da_year == 0)
         getdate(&dt);

      sprintf(st, "%s %2.1d %s %4.4dг.",
        GetWeekDayName(&dt),
        dt.da_day,
        GetMonthName(&dt),
        dt.da_year);
      setcolor_obj_num(pd, 40, 1, GetDateType(&dt) == 'В' ? EGA_LIGHTRED : EGA_GREEN, NO_SHOW);
      outpic_all(pd);
      outtext_obj_num(pd, 40, 1, getcolor_obj_num(pd, 40, 2), st, NO_SHOW);

   }/* if */

}/* OutDate */
/* ----------------------------------------------------------------------- */
#pragma argsused
double CalcTarifK(int kateg){
   return cc == Service ? TService : TCommercial;
}/* CalcTarif */
/* ----------------------------------------------------------------------- */
int c_break(void){
   RecordEvent(CtrlCPressed);
   return 1;
}/* c_break */
/* ----------------------------------------------------------------------- */
error_win(void){
    int retval;

    MessageBox("Ошибка копирования",
               "Вставьте дискету в дисковод A:\n"
               "'R' : Повторить копирование\n"
               "'A' : Отменить копирование");

 /*
 prompt for user to press a key to abort, retry, ignore
 */
    while(1){
	   retval = getch();
      if(retval == 0)
         getch();

      if(retval == 'a' || retval == 'A' || retval == 'ф' || retval == 'Ф'){
         retval = ABORT;
         break;
      }// if

      if (retval == 'r' || retval == 'R' || retval == 'к' || retval == 'К') 	{
       	retval = RETRY;
         break;
      }// if
/*
      if (retval == 'i' || retval == 'I') 	{
       	retval = IGNORE;
         break;
      }// if
*/
    }// while

    MessageBox(NULL, NULL);

    return(retval);
 }// error_win

 /*
 pragma warn -par reduces warnings which occur
 due to the non use of the parameters errval,
 bp and si to the handler.
 */
 #pragma warn -par

 int hwerrh(int errval,int ax,int bp,int si)
 {

    if (ax < 0)
    {
       /* report the error */
       ErrorMsg("Ош. устройства.");
       /* and return to the program directly requesting abort */
#ifndef DOSX286
       hardretn(ABORT);
#endif
    }

    hardresume(error_win());
    return ABORT;
 }
#pragma warn +par
/* ----------------------------------------------------------------------- */
#ifdef DOSX286
int heapchecknode(void* p){
   return p != NULL ? _USEDENTRY : _FREEENTRY;
}// heapchecknode
#endif
/* ----------------------------------------------------------------------- */
void StoreImg(int x_min, int y_min, int x_max, int y_max, char *filename);
void RestoreImg(int x_min, int y_min, int x_max, int y_max, char *filename);
void MessageBox(char *Title, char *MsgFmt, ...){
   static int minx = -1, miny, maxx, maxy;

   char buffer[512], *dup, *t, *tok, *sep = "\n";
   va_list argptr;
   int smx = 0, ttbTop, tbTop, tbLeft, tbBottom, tbRight,
       smy = 0, sx, sy,
       nl = 0;

   // восстановить место под сообщением и если нужно выйти
   if(minx != -1){
      RestoreImg(minx, miny, maxx, maxy, "tbg\\msgbox.img");
      minx = -1;
      if(Title == NULL){
         return;
      }/* if */
   }/* if */

   // отформатировать в буфер
   va_start(argptr, MsgFmt);
   vsprintf(buffer, MsgFmt, argptr);
   va_end(argptr);
   // для правильного вычисления ширины строки
   settextstyle(SIMPLEX_FONT, HORIZ_DIR, 4);
   settextjustify(LEFT_TEXT, TOP_TEXT);
   // бокс не менее чем титул
   smx = textwidth(Title);
   // вычислить длины строк выбрать максимальную
   t = dup = strdup(buffer);
   strtok(dup, sep);
   if(textwidth(t) >  smx){
      smx = textwidth(t);
   }/* if */
   nl++;
   smy += textheight(t) + 3;
   while((t = strtok(NULL, sep)) != NULL){
      if(textwidth(t) >  smx){
         smx = textwidth(t);
      }/* if */
      nl++;
      smy += textheight(t) + 3;
   }/* while */
   free(dup);
   // вычислить середину сообщения
   sx = (smx - 21) / 2;
   sy = (smy + 1 + 21) / 2;
   // вычислить смещение от центра экрана для середины сообщения
   miny = tbTop  = getmaxy() / 2 - sy;
   minx = tbLeft = getmaxx() / 2 - sx;
   // добавить размеры обвязки
   miny -= 21;
   minx -= 51;
   // вычислить размер сообщения
   maxy = tbBottom = tbTop  + smy + 1;
   maxx = tbRight  = tbLeft + smx;
   // добавить размеры обвязки
   maxy += 6;
   maxx += 6;
   // сохранить то что будет под картинкой в файле
   StoreImg(minx, miny, maxx, maxy, "tbg\\msgbox.img");
   // восстановить настройки
   settextstyle(SIMPLEX_FONT, HORIZ_DIR, 4);
   settextjustify(LEFT_TEXT, TOP_TEXT);
   // тень
   setcolor(EGA_DARKGRAY);
   line(tbRight + 6, tbBottom + 6, tbRight + 6, tbTop - 20);
   line(tbRight + 6, tbBottom + 6, tbLeft - 51, tbBottom + 6);
   // отсвет
   setcolor(EGA_WHITE);
   line(tbLeft - 51, tbTop - 21, tbRight + 6, tbTop - 21);
   line(tbLeft - 51, tbTop - 21, tbLeft - 51, tbBottom + 6);
   // внутреннее пространство
   setfillstyle(SOLID_FILL, EGA_LIGHTGRAY);
   bar(tbLeft - 50, tbTop - 20, tbRight + 5, tbBottom + 5);
   setcolor(EGA_DARKGRAY);
   rectangle(tbLeft - 50, tbTop - 20, tbRight + 5, tbBottom + 5);
   // заголовок
   setfillstyle(SOLID_FILL, EGA_MAGENTA);
   bar(tbLeft - 49, tbTop - 19, tbRight + 4, tbTop - 5);
   line(tbLeft - 49, tbTop - 5, tbRight + 4, tbTop - 5);
   outtextxy(tbLeft - 48, tbTop - 18, Title);
   setcolor(EGA_WHITE);
   outtextxy(tbLeft - 49, tbTop - 19, Title);

   setcolor(EGA_DARKGRAY);
   ttbTop = tbTop;
   moveto(tbLeft + 1, ttbTop += 1);
   t = dup = strdup(buffer);
   strtok(dup, sep);
   outtext(t);
   while((t = strtok(NULL, sep)) != NULL){
      moveto(tbLeft + 1, ttbTop += textheight(t) + 3);
      outtext(t);
   }/* while */
   free(dup);

   setcolor(EGA_WHITE);
   ttbTop = tbTop;
   moveto(tbLeft, ttbTop);
   t = dup = strdup(buffer);
   strtok(dup, sep);
   outtext(t);
   while((t = strtok(NULL, sep)) != NULL){
      moveto(tbLeft, ttbTop += textheight(t) + 3);
      outtext(t);
   }/* while */
   free(dup);


}/* MessageBox */
/* ----------------------------------------------------------------------- */
void OutSHBState(void){
   if(Step > STARTUP){
      setcolor(EGA_DARKGRAY);
      setfillstyle(SOLID_FILL, EGA_LIGHTGRAY);
      moveto(5, 457);
      gprintf("√ВъездШБ: %s %s ФД1:%s ФД2:%s %s ",
         shbS1 == shbsUP      ? "|" :
         shbS1 == shbsDOWN    ? "_" : "\\",
         shbC1 == shbcUP      ? "" :
         shbC1 == shbcDOWN    ? "" :
         shbC1 == shbcSTOP    ? "■" : "?",
         ir1 == irON  ? "ЗАКРЫТ" :
         ir1 == irOFF ? "ОТКРЫТ" : "      ",
         ir2 == irON  ? "ЗАКРЫТ" :
         ir2 == irOFF ? "ОТКРЫТ" : "      ",
         Direction1 ==  1 && ir1 == irOFF && ir2 == irOFF ? " >" :
         Direction1 == -1 && ir1 == irOFF && ir2 == irOFF ? " <" : ""
         );

      if(PostMode == POSTEXTENDED || PostMode == POSTTEST){
         setfillstyle(SOLID_FILL, EGA_LIGHTGRAY);
         moveto(5, 467);
         gprintf("√ВыездШБ: %s %s ФД1:%s ФД2:%s %s ",
            shbS2 == shbsUP      ? "|" :
            shbS2 == shbsDOWN    ? "_" : "/",
            shbC2 == shbcUP      ? "" :
            shbC2 == shbcDOWN    ? "" :
            shbC2 == shbcSTOP    ? "■" : "?",
            ir1out == irON  ? "ЗАКРЫТ" :
            ir1out == irOFF ? "ОТКРЫТ" : "      ",
            ir2out == irON  ? "ЗАКРЫТ" :
            ir2out == irOFF ? "ОТКРЫТ" : "      ",
            Direction2 ==  1 && ir1out == irOFF && ir2out == irOFF  ? " >" :
            Direction2 == -1 && ir1out == irOFF && ir2out == irOFF  ? " <" : ""
            );
      }/* if */
   }/* if */
}/* OutSHBState */
/* ----------------------------------------------------------------------- */
void main (int argc, char *argv[]){
   long tt;
   int Gd = VGA, Gm = VGAHI;
   IR oldir1, oldir2;
   SHBstate oldshbS1;
   IR oldir1out, oldir2out;
   SHBstate oldshbS2;
   char *sp = "\n Автоматизированная система \"Автостоянка\" v 1.0, КД \"Техника\" (С) 2000 г.";
   char *sfm = "\n\n Доступно памяти: %4.3lf Mбт.\n";

   M = coreleft();



   fprintf(stderr, sp);
   printf(sfm, coreleft() / 1024. / 1024.);

   PostMode = -1;

   if(argc == 3 && strcmp(argv[2], "Emulate") == 0){
      Emulate = 1;
   }/* if */

   if(argc > 1){

      if(strcmp(argv[1], "S") == 0){
         PostMode = POSTSTANDARD;
      }/* if */

      if(strcmp(argv[1], "E") == 0){
         PostMode = POSTEXTENDED;
      }/* if */

      if(strcmp(argv[1], "T") == 0){
         PostMode = POSTTEST;
      }/* if */

   }/* if */

   if(PostMode == -1){
      fprintf(stderr,
             "\n Ошибка в командной строке:"
             "\n  Допустимые аргументы:"
             "\n  S, E, T"
             "\n  где: S - Стандартный режим"
             "\n       E - Расширенный режим"
             "\n       T - Тестовый режим");
      exit(1);
   }/* if */

   if(!ReadConfig()){
      printf(sBell);
      Step = STOPPROGRAM;
   }/* if */

   collect();

   // заполнить календарь на тек. месяц
   PutMonthCalendar(NULL);

   // бесконечный цикл
   while(1 != 0){
      collect();

      PrintSpooler();

      if(Step > STARTUP){
         OutDate();
      }/* if */

      if(ExtKey == 0x8a){ // секретная клавиша выхода
         Step = STOPPROGRAM;
      }/* if */

/*
      if(Key == 's'){
         // тест спулерной печати
         char *pjfn = tempnam(".\\", "spool");
         FILE *pjf = fopen(pjfn, "wb");
         fprintf(pjf,"Test spooler print\n");
         fclose(pjf);
         AddSpool(cfg.BCPrinter, pjfn);
         free(pjfn);
      }
*/
      // Event hardware
      if((oldir1 != ir1) && (Step != STARTUP)){
         RecordEvent(ir1 == irOFF ? IR1Off : IR1On);
         sound(1200);
         delay(3);
         nosound();
         OutSHBState();
         Direction1 += ir1 == irOFF ? 1 : -1;
         oldir1 = ir1;
      }/* if */

      if((oldir2 != ir2) && (Step != STARTUP)){
         RecordEvent(ir2 == irOFF ? IR2Off : IR2On);
         sound(1800);
         delay(3);
         nosound();
         OutSHBState();
         Direction1 -= ir2 == irOFF ? 1 : -1;
         oldir2 = ir2;
      }/* if */

      if((oldshbS1 != shbS1) && (Step != STARTUP)){
         sound(1400);
         delay(3);
         nosound();
         RecordEvent(
         shbS1 == shbsUP      ? StandSHBUp   :
         shbS1 == shbsDOWN    ? StandSHBDown :
         shbS1 == shbsUNKNOWN ? SHBMiddle    : ErrorGeneral0);
         oldshbS1 = shbS1;
         OutSHBState();
      }/* if */

      if((oldir1out != ir1out) && (Step != STARTUP)){
         RecordEvent(ir1out == irOFF ? IR1Off : IR1On);
         sound(1300);
         delay(3);
         nosound();
         OutSHBState();
         Direction2 += ir1out == irOFF ? 1 : -1;
         oldir1out = ir1out;
      }/* if */

      if((oldir2out != ir2out) && (Step != STARTUP)){
         RecordEvent(ir2out == irOFF ? IR2Off : IR2On);
         sound(1700);
         delay(3);
         nosound();
         OutSHBState();
         Direction2 -= ir2out == irOFF ? 1 : -1;
         oldir2out = ir2out;
      }/* if */

      if((oldshbS2 != shbS2) && (Step != STARTUP)){
         sound(1500);
         delay(3);
         nosound();
         RecordEvent(
         shbS2 == shbsUP      ? StandSHBUp   :
         shbS2 == shbsDOWN    ? StandSHBDown :
         shbS2 == shbsUNKNOWN ? SHBMiddle    : ErrorGeneral0);
         oldshbS2 = shbS2;
         OutSHBState();
      }/* if */

      if(Step == STARTUP){


         RecordEvent(StartProgram);

#ifndef DOSX286
         harderr(hwerrh);
         registerfarbgidriver(EGAVGA_driver_far);
         registerfarbgifont(simplex_font_far);
         registerfarbgifont(bold_font_far);
         initgraph(&Gd, &Gm, "");
#else
         initgraph(&Gd, &Gm, ".");
#endif

         setpalette_tbg();

         if(PostMode != SETUP){

            ctrlbrk(c_break);


            // Фискальный регистратор
            if(Emulate){
//               FRCOMPort = b_ComInit(cfg.FRCOMPort == 1 ? _COM1_TEST : _COM2_TEST);
            }/* if */
            else{
               FRCOMPort = b_ComInit(cfg.FRCOMPort == 1 ? _COM1 : _COM2);
               b_ComStart(FRCOMPort);
               if(b_ComCheckBuf(FRCOMPort) == TRUE){
                  b_ComRecv(FRCOMPort, NULL);
               }/* if */
            }/* elif */
            // clear buffer


            ClearScan = TRUE;

         }/* if */
         load_file(&p, "tbg\\startup.tbg");
         outpic_all(p);


         tt = bt;
         Step = WAITSTARTUP;

         oldir1 = ir1;
         oldir2 = ir2;
         oldshbS1 = shbS1;

         continue;
      }/* if */

      if(Step == WAITSTARTUP){
         OutTime(p);
         if(((bt - tt) / CLK_TCK) > 3.0F){
            del_pic(&p);
            Step = LOGIN;
         }/* if */
         continue;
      }/* if */

      if((Step == LOGIN) && (PostMode == POSTTEST)){
         switch(TestSHB()){
            case 0:
               break;
            case 1:
               Step = STOPPROGRAM;
               break;
            case -1:
               ErrorMsg(SysAdm);
               Step = WAITERRORHARDWARE;
               break;
         }/* switch */
         continue;
      }/* if */

      if(Step == LOGIN){
         load_file(&p, "tbg\\scanner1.tbg");
         outpic_all(p);
         ClearScan = TRUE;
         Scanned = FALSE;
         Step = WAITSCAN;

         continue;
      }/* if */

      if(Step == WAITSCAN){
         OutTime(p);

         if(Key != 0){
#ifdef DEBUG
            if(Key == ESC){
               del_pic(&p);
               del_pic(&m);
               Step = STOPPROGRAM;
               continue;
            }/* if */
#endif
            if(Key == 'b'){
               Scanned = TRUE;
            }/* if */

            if(isdigit(Key)){
               load_file(&m, "tbg\\manscan1.tbg");
               outpic_all(m);
               stringedit[0] = 0;
               Edit_Init(m,editobj,editnum,stringedit, strlen_obj_num(m,editobj,editnum) + 1);
               Refresh_Display(m,editobj,editnum);
               EditLine_obj_num(m,editobj,editnum);
               Step = MANUALINPUT;

            }/* if */
         }/* if */
         continue;
      }/* if */

      if(Step == MANUALINPUT){
         OutTime(p);
   #ifdef DEBUG
            if(Key == ESC){
               del_pic(&m);
               del_pic(&p);
               Step = WAITERROR;
               continue;
            }/* if */
   #endif
         switch(EditLine_obj_num(m,editobj,editnum)){
            case OK      :
               strcpy(ScanBuf, Edit_Stop(m, editobj, editnum));
               del_pic(&m);
               RecordEvent(Scanned ? ScannerBCInput : ManualBCInput);
               RecordBC(ScanBuf, Scanned ? 0 : 1);
               Step = GETBASE;
               continue;
            case CONTINUE:
               continue;
            case HELP    :
               continue;
         }//switch
         continue;
      }/* if */

      if(Step == GETBASE){
         db = DBFOpen(16); // Пользователи
         if(db == NULL){
            del_pic(&m);
            del_pic(&p);
            ErrorMsg(SysAdm);
            RecordEvent(ErrorGeneral1);
            Step = WAITERROR;

            continue;
         }/* if */

         DBFFindRecord(db, "ШК", ScanBuf);
         if(DBFGetErrorNum() != DBF_OK){
            if(DBFGetErrorNum() == DBF_ERROR_RECORD_NOT_FOUND){
               DBFClose(db);
               ErrorMsg("Вход ЗАПРЕЩЕН!  "
                        "   Документ     "
                        "недействителен! ");
               RecordEvent(ErrorBCNotFound);
               Step = WAIT;
            }/* if */
            else{
               DBFClose(db);
               ErrorMsg(SysAdm);
               RecordEvent(ErrorGeneral2);
               Step = WAITERROR;
            }/* elif */
            continue;
         }/* if */


         BC = DBFGetFieldAsLong(db, "ШК");
         strcpy(sFIO, DBFGetFieldAsString(db, "ФИО"));

         switch(DBFGetFieldAsInt(db, "КАТЕГОРИЯ")){
            case 98: // Администратор кассы
               Step = WORKADMINFR;
               del_pic(&p);
               break;
            case 99: // Администратор программы
               Step = WORKADMINSYS;
               del_pic(&p);
               break;
            default: // Оператор
               Step = OPENSMENA;
               del_pic(&p);
               break;
         }/* switch */

         DBFClose(db);


         db = DBFOpen(18); // Журнал входа пользователей
         if(db == NULL){
            del_pic(&m);
            del_pic(&p);
            ErrorMsg(SysAdm);
            RecordEvent(ErrorGeneral1);
            Step = WAITERROR;
            continue;
         }/* if */
         DBFSetFieldAsLong(db, "ШК", BC);
         DBFSetFieldAsDate(db, "ДАТА", &dt);
         DBFSetFieldAsTime(db, "ВРЕМЯ", &tm);
         DBFAppendRecord(db);
         DBFClose(db);


         continue;
      }/* if */

      if(Step == WORKADMINSYS){
         switch(AdmSys()){
            case 0:// wait
               break;
            case 2:// return
               Step = STOPPROGRAM;
               break;
            case 1:// return
               Step = LOGIN;
               break;
            case -1:// error
               ErrorMsg(SysAdm);
               Step = WAITERRORHARDWARE;
               break;
            default:
               break;
         }/* switch */
         continue;
      }/* if */

      if(Step == WORKADMINFR){
         switch(AdmFr()){
            case 0:// wait
               break;
            case 2:// return
               Step = STOPPROGRAM;
               break;
            case 1:// return
               Step = LOGIN;
               break;
            case -1:// error
               ErrorMsg(SysAdm);
               Step = WAITERRORHARDWARE;
               break;
            default:
               break;
         }/* switch */
         continue;
      }/* if */

      if(Step == OPENSMENA){
         switch(OpenSm(BC % 98, sFIO)){
            case 0: // wait
               break;
            case 1: // ok

               Step = WORKPOST;
               break;
            case -1: // error
               ErrorMsg(SysAdm);
               Step = WAITERROR;
               break;
            default:
               break;
         }/* switch */
         continue;
      }/* if */

      if(Step == WORKPOST){
         switch(PostMode){
            case POSTSTANDARD:
               switch(Post1()){
                  case 0:// wait
                     break;
                  case 1:// return
                     Step = STOPPROGRAM;
                     break;
                  case 2:// return
                     Step = LOGIN;
                     break;
                  case -1:// error
                     ErrorMsg(SysAdm);
                     Step = WAITERRORHARDWARE;
                     break;
                  default:
                     break;
               }/* switch */
               break;
            case POSTEXTENDED:
               switch(Post4()){
                  case 0:// wait
                     break;
                  case 1:// return

                     Step = STOPPROGRAM;
                     break;
                  case 2:// return
                     Step = LOGIN;
                     break;
                  case -1:// error
                     ErrorMsg(SysAdm);
                     Step = WAITERRORHARDWARE;
                     break;
               }/* switch */
               break;
            default:
               Step = STOPPROGRAM;
               break;
         }/* switch */
         continue;
      }/* if */

      if(Step == WAITERROR){
         if(Key == ENTER){ //
            Step = STOPPROGRAM;
         }/* if */
         continue;
      }/* if */

      if(Step == WAIT){
         if(Key == ENTER){ //
            del_pic(&p);
            Step = LOGIN;
         }/* if */
         continue;
      }/* if */

      if(Step == WAITERRORHARDWARE){
         if(Key == 27){ // Esc
            Step = STOPPROGRAM;
         }/* if */
         continue;
      }/* if */

      if(Step == STOPPROGRAM){


         // ожидать печати всех документов в спулере
         while(PrintSpooler() == 0)
            OutTime(p);

         del_pic(&pd);
         del_pic(&p);

         closegraph();

         if(!Emulate){
            b_ComStop(FRCOMPort);
         }/* if */

         RecordEvent(StopProgram);

         fprintf(stderr, sp);

         if(M - coreleft() != 0){
            CheckHeap(__FILE__, __LINE__);
            printf("\nПотери памяти: %ld байт.%s", M - coreleft(), sBell);
         }/* if */
         else{
            printf(sfm, coreleft() / 1024. / 1024.);
         }/* elif */
         return;
      }/* if */

   }/* while */
}/* main */
/* ----------------------------------------------------------------------- */
void Store_Image(OBJECT **pic,char *filename){

   FILE *ft;
   short int x_min,y_min,x_max,y_max,i;
   char area[644];

   getsize_obj_num_reset();
   getsize_obj_num(pic,255,1,&x_min,&y_min,&x_max,&y_max);

   max_pixx=0xFFFF;

   if((ft=fopen(filename,"wb"))==NULL){
      printf("\nStore_Image:Не могу открыть файл '%s'",filename);
      return;
   }//if
   max_pixx=imagesize(x_min,y_min,x_max,y_min);
   if(max_pixx != 0xFFFF){
      for(i=y_min;i<= y_max;i++){
         getimage(x_min,i,x_max,i,area);
         fwrite(area,1,max_pixx,ft);
         if(i == (y_min+10)){
            setfillstyle(SOLID_FILL,EGA_LIGHTRED);
            setlinestyle(0,1,1);
            setcolor(EGA_WHITE);
            settextjustify(CENTER_TEXT,CENTER_TEXT);
            settextstyle(DEFAULT_FONT,0,1);
            bar(x_min,y_min,x_max,y_min+10);
            rectangle(x_min,y_min,x_max,y_min+10);
            outtextxy(x_min+(x_max-x_min)/2,y_min+6,"Waiting...");
         }//if
      }// for
   }/* if */
   fclose(ft);

}// Store_Image
/* ----------------------------------------------------------------------- */
void Restore_Image(OBJECT **pic,char *filename){

   FILE *ft;
   short int x_min,y_min,x_max,y_max,i;
   char area[644];

   getsize_obj_num_reset();
   getsize_obj_num(pic,255,1,&x_min,&y_min,&x_max,&y_max);

   if(max_pixx != 0xFFFF){
      if((ft=fopen(filename,"rb"))==NULL){
         printf("\nRestore_Image:Не могу открыть файл '%s'",filename);
         return;
      }//if

      for(i=y_min;i<= y_max;i++){
         fread(area,1,max_pixx,ft);
         putimage(x_min,i,area,COPY_PUT);
      }// for
      fclose(ft);
   }/* if */

}// Restore_Image
/* ----------------------------------------------------------------------- */
#pragma argsused
static void StoreImg(int x_min, int y_min, int x_max, int y_max, char *filename){

   FILE *ft;
//   short int x_min,y_min,x_max,y_max,
   int i;
   char area[644];

//   getsize_obj_num_reset();
//   getsize_obj_num(pic,255,1,&x_min,&y_min,&x_max,&y_max);

   max_pixx=0xFFFF;

   if((ft=fopen(filename,"wb"))==NULL){
      printf("\nStore_Image:Не могу открыть файл '%s'",filename);
      return;
   }//if
   max_pixx=imagesize(x_min,y_min,x_max,y_min);
   if(max_pixx != 0xFFFF){
      for(i=y_min;i<= y_max;i++){
         getimage(x_min,i,x_max,i,area);
         fwrite(area,1,max_pixx,ft);
         if(i == (y_min+10)){
            setfillstyle(SOLID_FILL,EGA_LIGHTRED);
            setlinestyle(0,1,1);
            setcolor(EGA_WHITE);
            settextjustify(CENTER_TEXT,CENTER_TEXT);
            settextstyle(DEFAULT_FONT,0,1);
            bar(x_min,y_min,x_max,y_min+10);
            rectangle(x_min,y_min,x_max,y_min+10);
            outtextxy(x_min+(x_max-x_min)/2,y_min+6,"Waiting...");
         }//if
      }// for
   }/* if */
   fclose(ft);

}// Store_Image
/* ----------------------------------------------------------------------- */
#pragma argsused
static void RestoreImg(int x_min, int y_min, int x_max, int y_max, char *filename){

   FILE *ft;
//   short int x_min,y_min,x_max,y_max,
   int i;
   char area[644];

//   getsize_obj_num_reset();
//   getsize_obj_num(pic,255,1,&x_min,&y_min,&x_max,&y_max);

   if(max_pixx != 0xFFFF){
      if((ft=fopen(filename,"rb"))==NULL){
         printf("\nRestore_Image:Не могу открыть файл '%s'",filename);
         return;
      }//if

      for(i=y_min;i<= y_max;i++){
         fread(area,1,max_pixx,ft);
         putimage(x_min,i,area,COPY_PUT);
      }// for
      fclose(ft);
   }/* if */

}// Restore_Image
/* ----------------------------------------------------------------------- */

