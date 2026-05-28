/*


   POST_TEST.C

   Borland C++ v3.1 for DOS

*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <mem.h>
#include <graphics.h>
#include <bios.h>
#include <time.h>
#include "tbg_tc.h"
#include "dbf.h"
/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */
#define PORT_SH 0x3EC
/* ----------------------------------------------------------------------- */
/* +++ Define data types */
typedef
   struct tag_pW{
      unsigned s1c_UP   :1; // 0
      unsigned s1c_DN   :1; // 1
      unsigned s1c_STOP :1; // 2
      unsigned s2c_UP   :1; // 3
      unsigned s2c_DN   :1; // 4
      unsigned s2c_STOP :1; // 5
      unsigned reserve1 :2; // 6, 7
   }_pW;

typedef
   struct tag_pR{
      unsigned s1k_UP  :1; // 0
      unsigned s1k_DN  :1; // 1
      unsigned s2k_UP  :1; // 2
      unsigned s2k_DN  :1; // 3
      unsigned C       :1; // 4
      unsigned P       :1; // 5
      unsigned reserve1:2; // 6, 7
      unsigned s1ir_1  :1; // 8
      unsigned s1ir_2  :1; // 9
      unsigned s2ir_1  :1; // 10
      unsigned s2ir_2  :1; // 11
      unsigned reserve2:4; // 12, 13, 14, 15
   }_pR;

typedef
   union tagpW{
      _pW bf;
      unsigned char b;
   }pW;

typedef
   union tagpR{
      _pR bf;
      unsigned int w;
   }pR;
/* ----------------------------------------------------------------------- */
void print16bit(unsigned short int uw){
   int i;
   unsigned short int t = uw;
   for(i = 0;i <= 15;i++){
      printf("%c", ((t << i) & 0x8000) == 0x0 ? '0' : '1');
   }/* for */
}/* print16bit */
/* ----------------------------------------------------------------------- */
void outbitscr(OBJECT **test, int pos, int obj, unsigned val){
   if((val == 0) || (val == 1)){
      char s[2];
      sprintf(s, "%u", val);
      mouse_off();
      outtext_obj_num(test, obj, pos, LIGHTGRAY, s, SHOW);
      mouse_on();
   }/* if */
}/* outbitscr */
/* ----------------------------------------------------------------------- */
LPDBF lpdbf;
void main (int argc, char *argv[]){
   OBJECT **test = NULL;
   OBJECT **demo = NULL;
   OBJECT **demo1 = NULL;
   int gm = VGAHI, gd = VGA;
   unsigned mkeyl = 0;
   unsigned mkeyr = 0;

   double angle = 0.;
   int direct = 1; // 1-up, 0-down

   unsigned char *ps;
   long ts, te;

   pW pw, old_pw;
   pR pr, old_pr;

   pw.b = 0;
   pr.w = 0;

   old_pw = pw;
   old_pr = pr;
/*
   lpdbf = DBFOpen(argv[1]);
   if(lpdbf == NULL){
      printf("\nFailed open database: %s",DBFGetErrorString());
   }/* if */
/*
   ts = clock();
   DBFFindRecord(lpdbf, "˜Š", "0147479647");
   if(DBFGetErrorNum() != DBF_OK){
      printf("\nError FindRecord: %s",DBFGetErrorString());
   }/* if */

   te = clock();
   printf("\n Record: %ld spend: %.4f sec.", lpdbf->Current, (te - ts) / CLK_TCK);

// „€’€   ‚…ŒŸ   ”ˆŽ   „€’€_‘   „€’€_Ž  ˜Š ŠŽ‹_‚Ž   …†ˆŒ ŽŒ…€_‚š…
// ƒŽ‘ŽŒ…€   Š€‘‘—…Š  Š€’…ƒŽˆŸ
   if(DBFGetErrorNum == DBF_OK){
      ps = "„€’€";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));
      ps = "‚…ŒŸ";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));
      ps = "˜Š";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));
      ps = "”ˆŽ";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));
      ps = "„€’€_‘";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));
      ps = "„€’€_Ž";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));
      ps = "ŠŽ‹_‚Ž";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));
      ps = "…†ˆŒ";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));
      ps = "ŽŒ…€_‚š…";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));
      ps = "ƒŽ‘ŽŒ…€";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));
      ps = "Š€‘‘—…Š";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));
      ps = "Š€’…ƒŽˆŸ";
      printf("\n | %s: >%s< ",ps , DBFGetFieldAsString(lpdbf, ps));

      if(DBFGetErrorNum() != DBF_OK){
         printf("\nError: %s",DBFGetErrorString());
      }/* if */
   }/* if */
*/
/*
   {
      unsigned long val;
      unsigned char sval[11];
      for(val = 0x7FFFFFFF;val > 0x7FFF0000;val--){
         ultoa(val, sval, 10);
         printf("\r %s ", sval);
         DBFSetFieldAsString(lpdbf, "˜Š", sval);
         DBFAppendRecord(lpdbf);
         if(DBFGetErrorNum() != DBF_OK){
            printf("\nError AppendRecord: %s",DBFGetErrorString());
         }/* if */
      }/* for */
   }
*/
   DBFClose(lpdbf);

   return;
*/
   initgraph(&gd, &gm, "");

   load_file(&test, "post_tst.tbg");

//   load_file(&demo, "in01.tbg");
//   load_file(&demo1, "in011.tbg");

   mouse_ini();
/*
   setcolor_obj(demo, 101, EGA_WHITE, NO_SHOW);
   outpic_all(demo);

   mouse_on();

   while(1){// animate sh

      outpic_obj_num(demo, 100, 2);// screen clear
      outpic_obj_num_with_rotate(demo, 100, 1, angle); // draw line

      if(kbhit()){
         if(getch() == 0){ // key
            getch();       // extended key
         }/* if */
         mouse_off();
         break;
      }/* if */

      mouse_lpk(&mkeyl, &mkeyr);
      if(mkeyl){
         int pos;

         pos = getpos_mouse_obj(demo, 10, 1);
         if(pos == 1){
            mouse_off();
            break;
         }/* if */
      }/* if */
      switch(direct){
         case 1:
            angle += .25;
            if(angle > 90.){
               direct = 0;
               setcolor_obj_num(demo, 101, 1, EGA_WHITE, SHOW);
               outpic_obj_num_color(demo, 102, 1, EGA_LIGHTRED);
               delay(1000);
               outpic_obj_num(demo, 102, 1);
            }/* if */
            else{
               setcolor_obj_num(demo, 101, 1, -2, SHOW);
            }/* elif */
            break;
         case 0:
            setcolor_obj_num(demo, 101, 2, -2, SHOW);
            angle -= .25;
            if(angle < 0.){
               direct = 1;
               setcolor_obj_num(demo, 101, 2, EGA_WHITE, SHOW);
               outpic_obj_num_color(demo, 102, 1, EGA_LIGHTRED);
               delay(1000);
               outpic_obj_num(demo, 102, 1);
            }/* if */
            break;
      }/* switch */

      delay(10);
   }/* while */

   outpic_all(demo1);
   mouse_on();

   while(1){// animate sh

      if(kbhit()){
         if(getch() == 0){ // key
            getch();       // extended key
         }/* if */
         mouse_off();
         break;
      }/* if */

      mouse_lpk(&mkeyl, &mkeyr);
      if(mkeyl){
         int pos;

         pos = getpos_mouse_obj(demo1, 10, 1);
         if(pos == 1){
            mouse_off();
            break;
         }/* if */
      }/* if */
   }/* while */
*/
   outpic_all(test);

   mouse_on();

   while(1){

      if((kbhit()) && (getch() == 27)){
         break;
      }/* if */

      mouse_lpk(&mkeyl, &mkeyr);
      if(mkeyl){
         int pos;

         pos = getpos_mouse_obj(test, 10, 1);
         if(pos == 1){
            break;
         }/* if */

         pos = getpos_mouse_obj(test, 1, 0);
         switch(pos){
            case 0:
               pw.bf.s1c_UP = ~pw.bf.s1c_UP;
               outbitscr(test, pos, 2, pw.bf.s1c_UP);
               break;
            case 1:
               pw.bf.s1c_DN = ~pw.bf.s1c_DN;
               outbitscr(test, pos, 2, pw.bf.s1c_DN);
               break;
            case 2:
               pw.bf.s1c_STOP = ~pw.bf.s1c_STOP;
               outbitscr(test, pos, 2, pw.bf.s1c_STOP);
               break;
            case 3:
               pw.bf.s2c_UP = ~pw.bf.s2c_UP;
               outbitscr(test, pos, 2, pw.bf.s2c_UP);
               break;
            case 4:
               pw.bf.s2c_DN = ~pw.bf.s2c_DN;
               outbitscr(test, pos, 2, pw.bf.s2c_DN);
               break;
            case 5:
               pw.bf.s2c_STOP = ~pw.bf.s2c_STOP;
               outbitscr(test, pos, 2, pw.bf.s2c_STOP);
               break;
         }/* if */
         if((pos != -1) && (old_pw.b != pw.b)){
            old_pw.b = pw.b;
            outportb(PORT_SH, pw.b);
         }/* if */
      }/* if */
      pr.w = inport(PORT_SH);
      if(old_pr.w != pr.w){
         old_pr.w = pr.w;
         outbitscr(test, 0, 3, pr.bf.s1k_UP);
         outbitscr(test, 1, 3, pr.bf.s1k_DN);
         outbitscr(test, 2, 3, pr.bf.s2k_UP);
         outbitscr(test, 3, 3, pr.bf.s2k_DN);
         outbitscr(test, 4, 3, pr.bf.C);
         outbitscr(test, 5, 3, pr.bf.P);
         outbitscr(test, 8, 3, pr.bf.s1ir_1);
         outbitscr(test, 9, 3, pr.bf.s1ir_2);
         outbitscr(test,10, 3, pr.bf.s2ir_1);
         outbitscr(test,11, 3, pr.bf.s2ir_2);
//         printf("\r ");
//         print16bit(pr.w);
      }/* if */
   }/* while */
   mouse_off();

   del_pic(&test);

   closegraph();
}/* main */
/* ----------------------------------------------------------------------- */
