/* -----------------------------------------------------------------------
*
*  DATA.H
*
*  Defined global data
*
*
*  ----------------------------------------------------------------------- */
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include "com.h"
#include "dbf.h"
#include "data.h"

#undef DEBUG

/* ----------------------------------------------------------------------- */
unsigned long M; // Memory used

OBJECT **p = NULL;

PrnStat sBCPrinter, sPrinter;

b_COM_PORT_DESC *ScannerCOMPort;
b_COM_PORT_DESC *FRCOMPort;

Config cfg, cfg4;

/* InfraRed eye's */
IR ir1 = irUNKNOWN,
   ir2 = irUNKNOWN;

IR ir1out = irUNKNOWN,
   ir2out = irUNKNOWN;

/* SHB's */
int iSHBOnOff = shbsOff;        // есть/нет ШБ на въезде: нет
SHBstate   shbS1 = shbsUNKNOWN; // состояние ШБ: неизвестно
SHBcommand shbC1 = shbcSTOP;    // команды ШБ: стоп

int oSHBOnOff = shbsOff;        // есть/нет ШБ на выезде: нет
SHBstate   shbS2 = shbsUNKNOWN; // состояние ШБ: неизвестно
SHBcommand shbC2 = shbcSTOP;    // команды ШБ: останов

float KUSHBTime = 0.5F;         // Время замыкания кнопки ШБ

/* Indentify running car robot */
GOCAR Robot  = STOP;
GOCAR Robot2 = STOP;
float CarDetectTime = .988F;

/* Emulate shlagbaum hardware */
int Emulate = 0;

/* System Time */
struct date dt;
struct time tm;
long        bt;

/* Keyboard hit */
int Key, ExtKey;

int InpGRN = 2;
CarCategory cc = Service;
float TService = 20.0f,
      TCommercial = 30.0f,
      TTalon = 15.0f;

int TNr = 1,
    SANr = 1,
    NANr = 1,
    DANr = 1;

NPOST NPost = 0;

long BC; // current BARCODE
                     //j   f   m   a   m   j   j   a   s   o   n   d
int NumDayInMonth[] = {31, 00, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
BOOL Calendar = FALSE;
BOOL Sunday = FALSE;
BOOL TimeShift = FALSE;

double Tarif;

int FROnOff = FROn;
char* FRP = "0000";// Пароль для ФР

unsigned char *ftarif = "%3.1uруб. %2.2uкоп.",
              *starif = "                  ";

char *SysAdm = "    Вызовите    "
               "   системного   "
               " администратора ";
/* Scanner data */
char ScanBuf[32];
int r, Scanned, ClearScan;

char *EventStr[]={
"Старт программы",                           // StartProgram,          // 0
"Останов программы",                         // StopProgram,           // 1
"Останов программы по ошибке",               // StopProgramError,      // 2
"Вход в систему",                            // LoginSystem,           // 3
"Ошибка при входе в систему",                // LoginSystemError,      // 4
"Команда ВВЕРХ",                             // ComSHBUp,              // 5
"Команда ВНИЗ",                              // ComSHBDown,            // 6
"Команда СТОП",                              // ComSHBStop,            // 7
"ШБ вверху",                                  // StandSHBUp,            // 8
"ШБ внизу",                                  // StandSHBDown,          // 9
"ШБ в промежуточном положении",              // SHBMiddle,             // 10
"ФД1 включен",                               // IR1On,                 // 11
"ФД1 выключен",                              // IR1Off,                // 12
"ФД2 включен",                               // IR2On,                 // 13
"ФД2 выключен",                              // IR2Off,                // 14
"Автомобиль прошел",                         // CarDetected,           // 15
"Выбран посетитель 1",                       // SelectPerson1,         // 16
"Выбран посетитель 2",                       // SelectPerson2,         // 17
"Выбран посетитель 3",                       // SelectPerson3,         // 18
"Выбран посетитель 4",                       // SelectPerson4,         // 19
"Выбран легковой",                           // SelectCar1,            // 20 Service
"Выбран грузовой",                           // SelectCar2,            // 21 Commercial
"ШК отсканирован",                           // ScannerBCInput,        // 22
"ШК введен вручную",                         // ManualBCInput,         // 23
"Нажатие клавиши",                           // KeyInput,              // 24
"ФР отпечатал",                              // FRPrint,               // 25
"ШК отпечатал",                              // BCPrint,               // 26
"Расчет произведен",                         // MoneyOK,               // 27
"Ошибка при повторном вводе ГРН",            // ErrorInputREGNUM,      // 28
"ШБ не работает",                            // ErrorSHBNotWorking,    // 29
"Принтер не готов",                          // ErrorPrinterNotReady,  // 30
"ШК принтер не готов",                       // ErrorPrinterBCNotReady,// 31
"ШК не найден в базе",                       // ErrorBCNotFound,       // 32
"",                                                                    // 33
"Нет дисковой памяти(< 2k)",                 // ErrorNotAvailDiskMem,  // 34
"Нажатие Ctrl-C",                            // CtrlCPressed,          // 35
"Отказ оборудования IBM PC",                 // HardwareFault,         // 36
"Принудительное завершение процедуры ВЪЕЗДА",// StopProcedureINPUT,    // 37
"Состояние ШБ неадекватно",                  // ErrorGeneral0     ,    // Undefined state SHB
"Не открывается БД: Пропуск",                // ErrorGeneral1     ,    // Error Open database Пропуска
"Не найден ШК в БД: Пропуск",                // ErrorGeneral2     ,    // Error Find BARCODE in Пропуска
"Ошибка при распределении памяти 1",         // ErrorGeneral3     ,    // Error memory allocate in Post1_2
"Ошибка в Абонементы база не открывается",   // ErrorGeneral4
"Ошибка в Абонементы пустая база",           // 5
"Ошибка в Талоны база не открывается",       // ErrorGeneral6
"Ошибка в Талоны пустая база",               // 7
};

char *sBell = "\007";

char ABCEng[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz. ",
     ABCRus[] = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯабвгдеёжзийклмнопрстуфхцчшщьыъэюя. ",
     DecDig[] = "01234567890",
     Sep1  [] = "-",
     Sep2  [] = ":",
     Sep3  [] = ",",
     AnyGRN[] = "*",
     CtrKey[] = "\x0D\x0A\x08",
     TypeAb[] = "ДНС",
     TypeDa[] = "*РВ";

char Info0[] =
"В это поле можно вводить\n"
"значения от А до я и \".\"";
char Info1[] =
 "В это поле можно вводить\n"
 "значения от 0 до 9 и \"-\"";
char Info2[] =
 "Можно вводить значения от 0 до 9,\n"
 "от А до я, A до z, и\".\"";
char Info3[] =
 "Можно вводить значения \n"
 "от А до я, от 0 до 9" ;
char Info5[] =
 "В это поле можно вводить\n"
 "значения от 0 до 9 и \":\"";
char Info6[] =
 "В это поле можно вводить   "
 " значения от 0 до 9" ;
char Info7[] =
 "В это поле можно вводить\n"
 "значения от 0 до 9 ,\"-\",\"_\"" ;
char Info9[] =
 "В это поле можно вводить символы:\n"
 "'Д' - дневной;\n"
 "'Н' - ночной;\n"
 "'С' - суточный.\n";
char Info12[] =
 "В это поле можно вводить\n"
 "символы от 0 до 9, типы дней:\n"
 "'Р' - рабочие;\n"
 "'В' - выходные;\n"
 "'*' - любые.";

/* ----------------------------------------------------------------------- */
/* Запись кода события в базу событий */
int RecordEvent(int ev){

   static unsigned long Num = 1;
   unsigned char buf[10];
   LPDBF lpdbf;
   struct dfree fr;
   long avail;
   int drive;

   drive = getdisk();
   getdfree(drive+1, &fr);
   if (fr.df_sclus != 0xFFFF){
      avail =  (long) fr.df_avail
               * (long) fr.df_bsec
               * (long) fr.df_sclus;
     // printf("Drive %c: has %ld bytes available\n", 'A' + drive, avail);
      if(avail < 2048L){
         return 0;
      }/* if */
   }/* if */     // harderr
/*
   {
      // тест спулерной печати
      char *pjfn = tempnam(".\\", "spool");
      FILE *pjf = fopen(pjfn, "wb");
      fprintf(pjf, "\r\n%5.1lu %2.1u:%2.2u:%2.2u < %s >", Num, tm.ti_hour, tm.ti_min, tm.ti_sec, EventStr[ev]);
      fclose(pjf);
      AddSpool(cfg.Printer, pjfn);
      free(pjfn);
   }
*/
   lpdbf = DBFOpen(1);// события
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsDate(lpdbf, "ДАТА", &dt);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsTime(lpdbf, "ВРЕМЯ", &tm);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   sprintf(buf, "%d", ev);
   DBFSetFieldAsString(lpdbf, "СОБЫТИЕ", buf);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsLong(lpdbf, "НОМЕР", Num);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFAppendRecord(lpdbf);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFClose(lpdbf);

   Num++;

   return 1;
}/* RecordEvent */
/* ----------------------------------------------------------------------- */
/* Запись суммы пробитой ФР-м в базу */
int RecordFRMoney(double m){

   unsigned char buf[10];
   LPDBF lpdbf;
   struct dfree free;
   long avail;
   int drive;

   drive = getdisk();
   getdfree(drive+1, &free);
   if (free.df_sclus != 0xFFFF){
      avail =  (long) free.df_avail
               * (long) free.df_bsec
               * (long) free.df_sclus;
     // printf("Drive %c: has %ld bytes available\n", 'A' + drive, avail);
      if(avail < 2048L){
         return 0;
      }/* if */
   }/* if */     // harderr

   lpdbf = DBFOpen(19);// события
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsDate(lpdbf, "ДАТА", &dt);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsTime(lpdbf, "ВРЕМЯ", &tm);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   sprintf(buf, "%4.2lf", m);
   DBFSetFieldAsString(lpdbf, "ФР_СУММА", buf);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFAppendRecord(lpdbf);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFClose(lpdbf);

   return 1;
}/* RecordFRMoney */
/* ----------------------------------------------------------------------- */
/* Запись введенного ГРН в базу */
int RecordGRN(char *GRN){
   unsigned char buf[10];
   LPDBF lpdbf;
   struct dfree free;
   long avail;
   int drive;

   drive = getdisk();
   getdfree(drive+1, &free);
   if (free.df_sclus != 0xFFFF){
      avail =  (long) free.df_avail
               * (long) free.df_bsec
               * (long) free.df_sclus;
     // printf("Drive %c: has %ld bytes available\n", 'A' + drive, avail);
      if(avail < 2048L){
         return 0;
      }/* if */
   }/* if */     // harderr

   lpdbf = DBFOpen(4); // ГРН
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsDate(lpdbf, "ДАТА", &dt);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsTime(lpdbf, "ВРЕМЯ", &tm);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsString(lpdbf, "ГРН", GRN);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFAppendRecord(lpdbf);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFClose(lpdbf);
   return 1;
}/* RecordGRN */
/* ----------------------------------------------------------------------- */
/* Запись введенного ШК в базу */
int RecordBC(char *BC, int Method){
   unsigned char buf[10];
   LPDBF lpdbf;
   struct dfree free;
   long avail;
   int drive;

   drive = getdisk();
   getdfree(drive+1, &free);
   if (free.df_sclus != 0xFFFF){
      avail =  (long) free.df_avail
               * (long) free.df_bsec
               * (long) free.df_sclus;
     // printf("Drive %c: has %ld bytes available\n", 'A' + drive, avail);
      if(avail < 2048L){
         return 0;
      }/* if */
   }/* if */     // harderr

   lpdbf = DBFOpen(10); //
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsDate(lpdbf, "ДАТА", &dt);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsTime(lpdbf, "ВРЕМЯ", &tm);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsString(lpdbf, "ШК", BC);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFSetFieldAsInt(lpdbf, "МЕТОД", Method);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFAppendRecord(lpdbf);
   if(DBFGetErrorNum() != DBF_OK){
      return 0;
   }/* if */

   DBFClose(lpdbf);
   return 1;
}/* RecordBC */
/* ----------------------------------------------------------------------- */
long CalcBC(int Alias){
   long BCMin, BCMax;
   char *nfP = "PRINTING";
   LPDBF db = DBFOpen(Alias);
   DBFGetRecord(db);
   BC    = DBFGetFieldAsLong(db, nfP);
   BCMin = DBFGetFieldAsLong(db, "MIN");
   BCMax = DBFGetFieldAsLong(db, "MAX");
   BC++;
   if(BC > BCMax || BC < BCMin){
      BC = BCMin;
   }/* if */
   DBFSetFieldAsLong(db, nfP, BC);
   DBFPutRecord(db);
   DBFClose(db);
   return BC;
}/* CalcBC */
/* ----------------------------------------------------------------------- */
#pragma argsused
double CalcTarifA(struct date* StartDate, struct date* EndDate){
   int d = NumDays(StartDate, EndDate);
   if(d < 0){
      d = abs(d);
      d += 1;
   }/* if */
   if(d == 0)
      d = 1;
   return (double)d * (cc == Service ? TService : TCommercial) + TTalon;
}/* CalcTarif4 */
/* ----------------------------------------------------------------------- */
int ReadConfig(void){
   int r, s = 0;
   FILE *f;
   char buf[128], name[32], port[16];
   char *fmt[] ={
      "%s %s",
      "%s %x %d %d",
      "\n  строка: %d аргумент: %d.\n",
      "\n Ошибка конфигурации:",
      "%s %f",
      "%s %d"
   };
   char *Ports[] = {
      "COM1:",
      "COM2:",
      "LPT1:",
      "LPT2:"
   };

   char *OnOff[] = {
      "Есть",
      "Нет",
      "Да"
   };

   enum{
      COM1,
      COM2,
      LPT1,
      LPT2
   };

   f = fopen("ShConfig.txt", "rb");
   while(!feof(f)){
      if(fgets(buf, sizeof(buf), f)){
         s++;
         if(sscanf(buf, "%s", name) == 1){

            if(strcmp("Въезд№", name) == 0){
               if((r = sscanf(buf, fmt[5], name, &NPost)) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("ВВГРН", name) == 0){
               if((r = sscanf(buf, fmt[5], name, &InpGRN)) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("Сканер", name) == 0){
               if((r = sscanf(buf, fmt[0], name, port)) == 2){
                  cfg.ScanCOMPort = 0;
                  if(strcmp(Ports[COM1], port) == 0){
                     cfg.ScanCOMPort = 1;
                  }/* if */
                  else{
                     if(strcmp(Ports[COM2], port) == 0){
                        cfg.ScanCOMPort = 2;
                     }/* if */
                     else{
                        printf(fmt[3]);
                        printf(fmt[2], s, r);
                        fclose(f);
                        return 0;
                     }/* elif */
                  }/* elif */
               }/* if */
               else{
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* elif */
            }/* if */

            if(strcmp("ФР", name) == 0){
               if((r = sscanf(buf, fmt[0], name, port)) == 2){
                  cfg.FRCOMPort = 0;
                  if(strcmp(Ports[COM1], port) == 0){
                     cfg.FRCOMPort = 1;
                  }/* if */
                  else{
                     if(strcmp(Ports[COM2], port) == 0){
                        cfg.FRCOMPort = 2;
                     }/* if */
                     else{
                        printf(fmt[3]);
                        printf(fmt[2], s, r);
                        fclose(f);
                        return 0;
                     }/* elif */
                  }/* elif */
               }/* if */
               else{
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* elif */
            }/* if */

            if(strcmp("iШБ", name) == 0){
               if((r = sscanf(buf, fmt[0], name, buf)) == 2){
                  iSHBOnOff = shbsOff;
                  if(strcmp(OnOff[0], buf) == 0){
                     iSHBOnOff = shbsOn;
                  }/* if */
                  else{
                     if(strcmp(OnOff[1], buf) == 0){
                        iSHBOnOff = shbsOff;
                     }/* if */
                     else{
                        printf(fmt[3]);
                        printf(fmt[2], s, r);
                        fclose(f);
                        return 0;
                     }/* elif */
                  }/* elif */
               }/* if */
               else{
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* elif */
            }/* if */

            if(strcmp("Календарь", name) == 0){
               if((r = sscanf(buf, fmt[0], name, buf)) == 2){
                  if(strcmp(OnOff[2], buf) == 0){
                     Calendar = TRUE;
                  }/* if */
                  else{
                     if(strcmp(OnOff[1], buf) == 0){
                        Calendar = FALSE;
                     }/* if */
                     else{
                        printf(fmt[3]);
                        printf(fmt[2], s, r);
                        fclose(f);
                        return 0;
                     }/* elif */
                  }/* elif */
               }/* if */
               else{
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* elif */
            }/* if */

            if(strcmp("Воскресение", name) == 0){
               if((r = sscanf(buf, fmt[0], name, buf)) == 2){
                  if(strcmp(OnOff[2], buf) == 0){
                     Sunday = TRUE;
                  }/* if */
                  else{
                     if(strcmp(OnOff[1], buf) == 0){
                        Sunday = FALSE;
                     }/* if */
                     else{
                        printf(fmt[3]);
                        printf(fmt[2], s, r);
                        fclose(f);
                        return 0;
                     }/* elif */
                  }/* elif */
               }/* if */
               else{
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* elif */
            }/* if */

            if(strcmp("АвтПереводВремени", name) == 0){
               if((r = sscanf(buf, fmt[0], name, buf)) == 2){
                  if(strcmp(OnOff[2], buf) == 0){
                     TimeShift = TRUE;
                  }/* if */
                  else{
                     if(strcmp(OnOff[1], buf) == 0){
                        TimeShift = FALSE;
                     }/* if */
                     else{
                        printf(fmt[3]);
                        printf(fmt[2], s, r);
                        fclose(f);
                        return 0;
                     }/* elif */
                  }/* elif */
               }/* if */
               else{
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* elif */
            }/* if */

            if(NPost == 4){
               if(strcmp("oШБ", name) == 0){
                  if((r = sscanf(buf, fmt[0], name, buf)) == 2){
                     oSHBOnOff = shbsOff;
                     if(strcmp(OnOff[0], buf) == 0){
                        oSHBOnOff = shbsOn;
                     }/* if */
                     else{
                        if(strcmp(OnOff[1], buf) == 0){
                           oSHBOnOff = shbsOff;
                        }/* if */
                        else{
                           printf(fmt[3]);
                           printf(fmt[2], s, r);
                           fclose(f);
                           return 0;
                        }/* elif */
                     }/* elif */
                  }/* if */
                  else{
                     printf(fmt[3]);
                     printf(fmt[2], s, r + 1);
                     fclose(f);
                     return 0;
                  }/* elif */
               }/* if */
            }/* if */

            if(strcmp("ФРП", name) == 0){
               if((r = sscanf(buf, fmt[0], name, FRP)) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("ШКПринтер", name) == 0){
               if((r = sscanf(buf, fmt[0], name, port)) == 2){
                  cfg.BCPrinter = 0;
                  if(strcmp(Ports[LPT1], port) == 0){
                     cfg.BCPrinter = 1;
                  }/* if */
                  else{
                     if(strcmp(Ports[LPT2], port) == 0){
                        cfg.BCPrinter = 2;
                     }/* if */
                     else{
                        printf(fmt[3]);
                        printf(fmt[2], s, r);
                        fclose(f);
                        return 0;
                     }/* elif */
                  }/* elif */
               }/* if */
               else{
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* elif */
            }/* if */

            if(strcmp("Принтер", name) == 0){
               if((r = sscanf(buf, fmt[0], name, port)) == 2){
                  cfg.Printer = 0;
                  if(strcmp(Ports[LPT1], port) == 0){
                     cfg.Printer = 1;
                  }/* if */
                  else{
                     if(strcmp(Ports[LPT2], port) == 0){
                        cfg.Printer = 2;
                     }/* if */
                     else{
                        printf(fmt[3]);
                        printf(fmt[2], s, r);
                        fclose(f);
                        return 0;
                     }/* elif */
                  }/* elif */
               }/* if */
               else{
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* elif */
            }/* if */

            if(strcmp("iКЦВЕРХ", name) == 0){
               if((r = sscanf(buf, fmt[1], name, &(cfg.SwUp.nP), &(cfg.SwUp.Bit), &(cfg.SwUp.Val))) != 4){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("iКЦНИЗ", name) == 0){
               if((r = sscanf(buf, fmt[1], name, &(cfg.SwDown.nP), &(cfg.SwDown.Bit), &(cfg.SwDown.Val))) != 4){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("iВВЕРХ", name) == 0){
               if((r = sscanf(buf, fmt[1], name, &(cfg.CmUp.nP), &(cfg.CmUp.Bit), &(cfg.CmUp.Val) )) != 4){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("iВНИЗ", name) == 0){
               if((r = sscanf(buf, fmt[1], name, &(cfg.CmDown.nP), &(cfg.CmDown.Bit), &(cfg.CmDown.Val) )) != 4){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("iСТОП", name) == 0){
               if((r = sscanf(buf, fmt[1], name, &(cfg.CmStop.nP), &(cfg.CmStop.Bit), &(cfg.CmStop.Val) )) != 4){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("iФД1", name) == 0){
               if((r = sscanf(buf, fmt[1], name, &(cfg.IR1.nP), &(cfg.IR1.Bit), &(cfg.IR1.Val) )) != 4){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("iФД2", name) == 0){
               if((r = sscanf(buf, fmt[1], name, &(cfg.IR2.nP), &(cfg.IR2.Bit), &(cfg.IR2.Val) )) != 4){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(NPost == 4){
               if(strcmp("oКЦВЕРХ", name) == 0){
                  if((r = sscanf(buf, fmt[1], name, &(cfg4.SwUp.nP), &(cfg4.SwUp.Bit), &(cfg4.SwUp.Val))) != 4){
                     printf(fmt[3]);
                     printf(fmt[2], s, r + 1);
                     fclose(f);
                     return 0;
                  }/* if */
               }/* if */

               if(strcmp("oКЦНИЗ", name) == 0){
                  if((r = sscanf(buf, fmt[1], name, &(cfg4.SwDown.nP), &(cfg4.SwDown.Bit), &(cfg4.SwDown.Val))) != 4){
                     printf(fmt[3]);
                     printf(fmt[2], s, r + 1);
                     fclose(f);
                     return 0;
                  }/* if */
               }/* if */

               if(strcmp("oВВЕРХ", name) == 0){
                  if((r = sscanf(buf, fmt[1], name, &(cfg4.CmUp.nP), &(cfg4.CmUp.Bit), &(cfg4.CmUp.Val) )) != 4){
                     printf(fmt[3]);
                     printf(fmt[2], s, r + 1);
                     fclose(f);
                     return 0;
                  }/* if */
               }/* if */

               if(strcmp("oВНИЗ", name) == 0){
                  if((r = sscanf(buf, fmt[1], name, &(cfg4.CmDown.nP), &(cfg4.CmDown.Bit), &(cfg4.CmDown.Val) )) != 4){
                     printf(fmt[3]);
                     printf(fmt[2], s, r + 1);
                     fclose(f);
                     return 0;
                  }/* if */
               }/* if */

               if(strcmp("oСТОП", name) == 0){
                  if((r = sscanf(buf, fmt[1], name, &(cfg4.CmStop.nP), &(cfg4.CmStop.Bit), &(cfg4.CmStop.Val) )) != 4){
                     printf(fmt[3]);
                     printf(fmt[2], s, r + 1);
                     fclose(f);
                     return 0;
                  }/* if */
               }/* if */

               if(strcmp("oФД1", name) == 0){
                  if((r = sscanf(buf, fmt[1], name, &(cfg4.IR1.nP), &(cfg4.IR1.Bit), &(cfg4.IR1.Val) )) != 4){
                     printf(fmt[3]);
                     printf(fmt[2], s, r + 1);
                     fclose(f);
                     return 0;
                  }/* if */
               }/* if */

               if(strcmp("oФД2", name) == 0){
                  if((r = sscanf(buf, fmt[1], name, &(cfg4.IR2.nP), &(cfg4.IR2.Bit), &(cfg4.IR2.Val) )) != 4){
                     printf(fmt[3]);
                     printf(fmt[2], s, r + 1);
                     fclose(f);
                     return 0;
                  }/* if */
               }/* if */
            }/* if */

            if(strcmp("АвтВремя", name) == 0){
               if((r = sscanf(buf, fmt[4], name, &CarDetectTime )) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("КУШБВремя", name) == 0){
               if((r = sscanf(buf, fmt[4], name, &KUSHBTime )) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("ТЛегковой", name) == 0){
               if((r = sscanf(buf, fmt[4], name, &TService )) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("ТГрузовой", name) == 0){
               if((r = sscanf(buf, fmt[4], name, &TCommercial )) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("ТВъездов", name) == 0){
               if((r = sscanf(buf, fmt[5], name, &TNr)) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("ТТалон", name) == 0){
               if((r = sscanf(buf, fmt[4], name, &TTalon)) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("САВъездов", name) == 0){
               if((r = sscanf(buf, fmt[5], name, &SANr)) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("НАВъездов", name) == 0){
               if((r = sscanf(buf, fmt[5], name, &NANr)) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

            if(strcmp("ДАВъездов", name) == 0){
               if((r = sscanf(buf, fmt[5], name, &DANr)) != 2){
                  printf(fmt[3]);
                  printf(fmt[2], s, r + 1);
                  fclose(f);
                  return 0;
               }/* if */
            }/* if */

         }/* if */
      }/* if */
   }/* while */

   fclose(f);

#ifdef DEBUG
   printf("\nСканер: COM%d", cfg.ScanCOMPort);
   printf("\nФР:     COM%d", cfg.FRCOMPort);
   printf("\nШКпринтер: LPT%d", cfg.BCPrinter + 1);
   printf("\nпринтер:   LPT%d", cfg.Printer + 1);
   printf("\niКЦВЕРХ: %X %d %d", cfg.SwUp.nP, cfg.SwUp.Bit, cfg.SwUp.Val);
   printf("\niКЦНИЗ:  %X %d %d", cfg.SwDown.nP, cfg.SwDown.Bit, cfg.SwDown.Val);
   printf("\niВВЕРХ:  %X %d %d", cfg.CmUp.nP, cfg.CmUp.Bit, cfg.CmUp.Val);
   printf("\niВНИЗ:   %X %d %d", cfg.CmDown.nP, cfg.CmDown.Bit, cfg.CmDown.Val);
   printf("\niСТОП:   %X %d %d", cfg.CmStop.nP, cfg.CmStop.Bit, cfg.CmStop.Val);
   printf("\niФД1:    %X %d %d", cfg.IR1.nP, cfg.IR1.Bit, cfg.IR1.Val);
   printf("\niФД2:    %X %d %d", cfg.IR2.nP, cfg.IR2.Bit, cfg.IR2.Val);
   printf("\noКЦВЕРХ: %X %d %d", cfg4.SwUp.nP, cfg4.SwUp.Bit, cfg4.SwUp.Val);
   printf("\noКЦНИЗ:  %X %d %d", cfg4.SwDown.nP, cfg4.SwDown.Bit, cfg4.SwDown.Val);
   printf("\noВВЕРХ:  %X %d %d", cfg4.CmUp.nP, cfg4.CmUp.Bit, cfg4.CmUp.Val);
   printf("\noВНИЗ:   %X %d %d", cfg4.CmDown.nP, cfg4.CmDown.Bit, cfg4.CmDown.Val);
   printf("\noСТОП:   %X %d %d", cfg4.CmStop.nP, cfg4.CmStop.Bit, cfg4.CmStop.Val);
   printf("\noФД1:    %X %d %d", cfg4.IR1.nP, cfg4.IR1.Bit, cfg4.IR1.Val);
   printf("\noФД2:    %X %d %d", cfg4.IR2.nP, cfg4.IR2.Bit, cfg4.IR2.Val);
   printf("\nCarDetectTime: %f сек.", CarDetectTime);
   printf("\nКУШБВремя:     %f сек.", KUSHBTime);
   getch();
#endif
   if((cfg.ScanCOMPort == cfg.FRCOMPort)||
      (cfg.BCPrinter   == cfg.Printer))
      printf(fmt[3]);

   if(cfg.ScanCOMPort == cfg.FRCOMPort){
      printf("\n Порты сканера и ФР совпадают.");
   }/* if */
   if(cfg.BCPrinter == cfg.Printer){
      printf("\n Порты ШК и принтера совпадают.");
   }/* if */

   if(cfg.ScanCOMPort == 0){
      printf("\n Порт для сканера ШК не назначен.");
//      return  0;
   }/* if */

   if(cfg.FRCOMPort == 0){
      printf("\n Порт для ФР не назначен.");
      return  0;
   }/* if */

   if(cfg.Printer == 0){
      printf("\n Порт для Принтера не назначен.");
//      return  0;
   }/* if */

   if(cfg.BCPrinter == 0){
      printf("\n Порт для ШК Принтера не назначен.");
      return  0;
   }/* if */

   if((cfg.ScanCOMPort == cfg.FRCOMPort)||
      (cfg.BCPrinter   == cfg.Printer))
      return 0;

   // Установить времена
   SetDNT();

   return 1;
}/* ReadConfig */
/* ----------------------------------------------------------------------- */
int GetGatesString(char* dest, char *s, int num){
   char *t, c, ctok[8];
   char dup[80];
   int nt = 0, n;

   if(s == NULL){
      return 1;
   }/* if */

   strcpy(dup, s);
   dest[0] = 0;

   t = strtok(dup, ",");
   do{
      if(t){
         if((r = sscanf(t, "%d%c", &n, &c)) == 2){
            if(strlen(dest) < num){
               if(nt){
                  strcat(dest, ",");
               }/* if */
               sprintf(ctok, "%d", n);
               strcat(dest, ctok);
               nt++;
            }/* if */
            else{
               return 0;
            }/* elif */
         }/* if */
         else{
            return -1;
         }/* elif */
      }/* if */
   }while((t = strtok(NULL, ",")) != NULL);
   return 0;

}/* GetGatesString */
/* ----------------------------------------------------------------------- */
int CheckGate(char *s){
   char *t, c = -1, ctok[8];
   char dup[80];
   int nt = 0, n;

   if(s == NULL){
      return 1;
   }/* if */

   strcpy(dup, s);

   t = strtok(dup, ",");
   do{
      if(t){
         nt++;
         n = c = -1;
         if((r = sscanf(t, "%d%c", &n, &c)) == 2){
            if(n < 0){
               return 1;
            }/* if */
         }/* if */
         else{
            return 1;
         }/* elif */
      }/* if */
   }while((t = strtok(NULL, ",")) != NULL);
   return 0;

}/* CheckGate */
/* ----------------------------------------------------------------------- */
int CheckNGD(char *s, int NGate, char Day){
   char *t, c, *sep = ",";
   char dup[80];
   int nt = 0;
   int n;

   if(s == NULL){
      return -2; // нет описания
   }/* if */

   strcpy(dup, s);

   t = strtok(dup, sep);
   do{
      if(t){
         nt++;
         if((r = sscanf(t, "%d%c", &n, &c)) == 2){
            // сверить номер въезда и день
            if(NGate == n){

               Day = Day == 'П' ? 'В' : Day;

               if(c == Day || c == '*'){
                  return 1; // OK!
               }/* if */
               else{
                  return 2; // не в этот день
               }/* elif */
            }/* if */
         }/* if */
         else{
            return -1; // ошибка в описании въездов и типа дня
         }/* elif */
      }/* if */
   }while((t = strtok(NULL, sep)) != NULL);

   return 0;// не найден въезд

}/* CheckNGD */
/* ----------------------------------------------------------------------- */
BOOL CheckGRN(char *GRNs, char* GRN){
   char *sep = " ,.", *t;
   char *dup, *tok;

   if(GRNs == NULL){
      return FALSE; // нет описания
   }/* if */

   tok = dup = strdup(GRNs);
   while((t = strtok(tok, sep)) != NULL){
      tok = NULL;
      // сверить GRN
      if(strcmp(GRN, t) == 0){
         free(dup);
         return TRUE;
      }/* if */
   }/* while */
   free(dup);
   return FALSE;
}/* CheckGRN */
/* ----------------------------------------------------------------------- */
void CheckHeap(char *s, int l){
   struct farheapinfo hi;
   hi.ptr = NULL;
   printf( "\n\n Pointer   Size   Status Dump  %s : %d\n", s, l);
   printf(     "----:---- ------- ------ -------------------------------------------------------\n");
   while( farheapwalk( &hi ) == _HEAPOK ){
      printf(  "%p %7lu   %s", hi.ptr, hi.size, hi.in_use ? "used " : "free " );
      if(hi.in_use){
         int i;
         for(i = 0;;i++){
            printf("%c", *(((unsigned char*)hi.ptr) + i) >= 32 ? *(((unsigned char*)hi.ptr) + i) : '.');
            if((i > hi.size) || (i > 52)){
               break;
            }/* if */
         }/* for */
      }/* if */
      printf("\n");
   }/* while */

}/* CheckHeap */
/* ----------------------------------------------------------------------- */

