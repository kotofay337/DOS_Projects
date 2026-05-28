/* -----------------------------------------------------------------------
*
*  DATA.H
*
*  Declare global data
*
*
*  ----------------------------------------------------------------------- */
#include "tbg_tc.h"
#include "com.h"

//#define DEBUG
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
   union {
      struct{
         unsigned b0:1;
         unsigned b1:1;
         unsigned b2:1;
         unsigned b3:1;
         unsigned b4:1;
         unsigned b5:1;
         unsigned b6:1;
         unsigned b7:1;
         unsigned b8:1;
         unsigned b9:1;
         unsigned b10:1;
         unsigned b11:1;
         unsigned b12:1;
         unsigned b13:1;
         unsigned b14:1;
         unsigned b15:1;
      }bf;
      unsigned char  byte[2];
      unsigned short word;
   }ER;
/* ----------------------------------------------------------------------- */
typedef
   struct{
      unsigned nP;
      unsigned Bit;
      unsigned Val;
   }EQ;
/* ----------------------------------------------------------------------- */
typedef
   struct{
      int ScanCOMPort;// 1 = COM1, 2 = COM2.
      int FRCOMPort;  // ----"----
      int BCPrinter;  // òä è‡®≠‚•‡ 0 = LPT1
      int Printer;    // è‡®≠‚•‡ 0 = LPT1
      EQ  SwUp,       // äÆ≠Ê•¢®™ ¢•‡Â≠®©
          SwDown,     //          ≠®¶≠®©
          CmUp,       // äÆ¨†≠§† ¢¢•‡Â
          CmDown,     //         ¢≠®ß
          CmStop,     //         ·‚ÆØ
          IR1,        // îÑ1
          IR2;        // îÑ2
   }Config;
/* ----------------------------------------------------------------------- */
typedef
   enum {
      irOFF,    // 0
      irON,     // 1
      irUNKNOWN // 2
   }IRstate;

typedef unsigned short IR;
/* ----------------------------------------------------------------------- */
typedef
   enum {
      shbsUP,
      shbsDOWN,
      shbsUNKNOWN,
      shbsOff,
      shbsOn
   }SHBs;

typedef
   enum {
      shbcUP,
      shbcDOWN,
      shbcSTOP,
      shbcOK
   }SHBc;

typedef unsigned short SHBstate;
typedef unsigned short SHBcommand;

typedef
   enum {
      BEGIN,
      END,
      STOP
   }GOCAR;

typedef
   enum{
      EV_OK
   }Event;
/* ----------------------------------------------------------------------- */

typedef
   enum{
      StartProgram,          // 0
      StopProgram,           // 1
      StopProgramError,      // 2
      LoginSystem,           // 3
      LoginSystemError,      // 4
      ComSHBUp,              // 5
      ComSHBDown,            // 6
      ComSHBStop,            // 7
      StandSHBUp,            // 8
      StandSHBDown,          // 9
      SHBMiddle,             // 10
      IR1On,                 // 11
      IR1Off,                // 12
      IR2On,                 // 13
      IR2Off,                // 14
      CarDetected,           // 15
      SelectPerson1,         // 16
      SelectPerson2,         // 17
      SelectPerson3,         // 18
      SelectPerson4,         // 19
      SelectCar1,            // 20 Service
      SelectCar2,            // 21 Commercial
      ScannerBCInput,        // 22
      ManualBCInput,         // 23
      KeyInput,              // 24
      FRPrint,               // 25
      BCPrint,               // 26
      MoneyOK,               // 27
      ErrorInputREGNUM,      // 28
      ErrorSHBNotWorking,    // 29
      ErrorPrinterNotReady,  // 30
      ErrorPrinterBCNotReady,// 31
      ErrorBCNotFound,       // 32
      Error       ,          // 33
      ErrorNotAvailDiskMem,  // 34
      CtrlCPressed,          // 35
      HardwareFault,         // 36
      StopProcedureINPUT,    // 37
      StopProcedureOUTPUT,   // 38
      ErrorGeneral0     ,    // 39 Undefined state SHB
      ErrorGeneral1     ,    // 40 Error Open database è‡ÆØ„·™†
      ErrorGeneral2     ,    // 41 Error Find BARCODE in è‡ÆØ„·™†
      ErrorGeneral3     ,    // 42 Error memory allocate in Post1_2
      ErrorGeneral4     ,    // 43
      ErrorGeneral5     ,    // 44
      ErrorGeneral6     ,    // 45
      ErrorGeneral7     ,    // 46
      ErrorGeneral8     ,    // 47
      ErrorGeneral9     ,    // 48
      LastEvent
   }Events;


typedef
   enum{
      Commercial,
      Service
   }CarCategory;

typedef
   enum{
      POSTSTANDARD = 1,
      POSTEXTENDED,
      ABONEMENT,
      PROPUSK,
      REPORT1,
      REPORT2,
      REPORT3,
      SETUP,
      POSTTEST
   }NPOST;

typedef
   enum{
      TDABONEMENT,
      TDTALON
   }Typedoc_t;

typedef
   enum{
      TCSTANDART,
      TCPERESTOY
   }Typecalc_t;

typedef
   enum{
      FROff,
      FROn
   }FRState;

#define END    79
#define DEL    83
#define ESC    27
#define HOME   71
#define LEFT   75
#define RIGHT  77

#define ENTER  13
#define BSPACE  8
#define TAB     9

#define F1     59
#define F2     60

/* ----------------------------------------------------------------------- */
extern unsigned long M; // Memory used

extern int far _Cdecl   simplex_font_far[];
extern int far _Cdecl     bold_font_far[];

extern OBJECT **p;

extern PrnStat sBCPrinter, sPrinter;

extern b_COM_PORT_DESC *ScannerCOMPort;
extern b_COM_PORT_DESC *FRCOMPort;

extern Config cfg, cfg4;

/* InfraRed eye's */
extern IR ir1, ir2;
extern IR ir1out, ir2out;

/* SHB's */
extern int iSHBOnOff;
extern SHBstate   shbS1;
extern SHBcommand shbC1;

extern int oSHBOnOff;
extern SHBstate   shbS2;
extern SHBcommand shbC2;
extern float KUSHBTime;

/* Indentify running car robot */
extern GOCAR Robot,
             Robot2;
extern float CarDetectTime;
/* Emulate shlagbaum hardware flag */
extern int Emulate;
/* System Time */
extern struct date dt;
extern struct time tm;
extern long        bt;
/* Keyboard hit */
extern int Key, ExtKey;
/*  */
extern int InpGRN;
extern CarCategory cc;
extern float TService,
             TCommercial,
             TTalon;
extern int TNr,
           SANr,
           NANr,
           DANr;

/*  */
extern NPOST NPost;
/*  */
extern long BC;
/*  */
extern int NumDayInMonth[];
extern BOOL Calendar;
extern BOOL Sunday;
extern BOOL TimeShift;

/*  */
extern int FROnOff;
extern char* FRP;// è†‡Æ´Ï §´Ô îê
extern double Tarif;
extern unsigned char *ftarif, *starif;

extern char *SysAdm;

/* Scan buffer */
extern char ScanBuf[32];
extern int r, Scanned, ClearScan;
extern char *sBell;
extern char ABCEng[],// = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            ABCRus[],// = "ÄÅÇÉÑÖÜáàäãåçéèêëíìîïñóòôúõöùûü†°¢£§•Ò¶ß®™´¨ÆØ‡·„‰ÂÊÁËÈÍÎÏÌÓÔ.",
            DecDig[],// = "01234567890",
            Sep1  [],// = "-",
            Sep2  [],// = ":",
            Sep3  [],// = ",";
            AnyGRN[],// = "*",
            CtrKey[],// = "\x0D\x0A\x08";
            TypeAb[],// = "Ñçë",
            TypeDa[];// = "*êÇ";

extern char Info0[];
extern char Info1[];
extern char Info2[];
extern char Info3[];
extern char Info5[];
extern char Info6[];
extern char Info9[];
extern char Info12[];
/* ----------------------------------------------------------------------- */
void Store_Image(OBJECT **pic,char *filename);
void Restore_Image(OBJECT **pic,char *filename);
void collect(void);
int Post1(void);
int Post1_2(void);
int Post1_3(void);
int Post1_0(void);
int Post4(void);
int Post4_1(void);
int Post4_2(void);
int Post4_3(void);
int Post4_4(void);
int Post4_5(void);
int Post4_1out(void);
int Post4_2out(void);
int Post4_3out(void);
int Post4_4out(void);
int Abonement(void);
int Propusk(void);
int Abon4(int tCall); // 1-Ä§¨®≠®·‚‡, != 1 - éØ•‡†‚Æ‡
int Prop4(void);
int WorkSHB (void);
int WorkSHBOut (void);
int ErrorMsg(unsigned char *msg);
int InfoMsg(unsigned char *msg);
void MessageBox(char *Title, char *MsgFmt, ...);
int RecordEvent(int ev);
int RecordGRN(char *GRN);
int RecordBC(char *BC, int Method);
int RecordFRMoney(double m);
void OutTime(OBJECT *p);
int PrintFR (double tarif);
int OpenSm (unsigned OpNum, char* FIO);
int WorkSHB (void);
int WorkSHBout (void);
int gprintf(char *fmt, ...);
void SetDayNightTime(struct time bday, struct time eday);
void GetDayNightTime(struct time* bday, struct time* eday);

void  SetDNT(void);
char  GetTimeType(void);
int   GetWeekDay(struct date* d);
char* GetWeekDayName(struct date* d);
char* GetMonthName(struct date* d);
int   NumDays(struct date* d1, struct date* d2);
int DayInMonth(struct date* d);
void  PutMonthCalendar(struct date *dat);
char  GetDateType(struct date *dat);
int   GetGatesString(char* dest, char *s, int num);
int   CheckGate(char *s);
int   VisYear(int y);
int   CheckNGD(char *s, int NG, char D);
int   GetAfterHour(struct date* d1, struct date* d2);
int   FutureHour(struct date* sd, struct time* st, int hour);
BOOL  CheckGRN(char *GRNs, char* GRN);

int CheckAfterDate(struct date* sd, struct time* st, struct date* ed, struct time* et);
void CalcTime(struct time,struct time,struct time *,struct time *,int *);
int GetType(int t);
long CalcBC(int Alias);
int PrintBC(int As, char* Pl, char* Type, char* Mon, char* Year, char* FIO, unsigned long BC);
int PrintBCAdmin(int As, char* Type, char* FIO, unsigned long BC);

int PrintSpooler(void);
int AddSpool(int nLPT, char *fn);

double CalcTarifK(int kateg);
double CalcTarifA(struct date* , struct date*);
double CalcTarif(Typedoc_t TypeDoc, Typecalc_t TypeCalc, struct time* st, struct date* sd,
                  struct time* et, struct date* ed);

int ReadConfig(void);
int AdmSys(void);
int AdmFr(void);
int SetOperator(int OpKat);

int TestSHB(void);
int ConfigProgram(void);
/* ----------------------------------------------------------------------- */
void CheckHeap(char *s, int l);
