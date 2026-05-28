/* -----------------------------------------------------------------------
*
*  SHBOut.C
*
*  Work with SHB
*
*  ----------------------------------------------------------------------- */
#include <stdio.h>
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
      SHBSTART,
      SHBCOMMANDUP,
      SHBWAITUP,
      SHBWAITCAR,
      SHBCOMMANDDOWN,
      SHBWAITDOWN,
      SHBEND,
      SHBCOMMANDSTOP,
      SHBERROR,
      SHBWAITERROR
   }SHBSTATE;

typedef
   enum{
      SHBEROR_NOTDOWN,
      SHBEROR_NOTUP,
      SHBEROR_TIMEOUT,
      SHBEROR_NOTWORK
   }SHBER;

int mf;

int WorkSHBOut (void){

float t1 = 0.0F;
char st[6];
static double out1timeUP, out1timeDN;
static int tfUP = 0, tfDN = 0;
static int cUP = 0, cDN = 0;
static long t = 0, tup;
static SHBSTATE s = SHBSTART;
static double angleUP, angleDN;
static OBJECT **p = NULL;

      if(oSHBOnOff == shbsOff){
         return 1;
      }/* if */

      if((Robot == STOP) && (ir1out == irON && ir2out == irON)){
         t = bt;
         Robot = BEGIN;
         t1 = 0.0F;
      }/* if */

      if((Robot == BEGIN) && (ir1out == irOFF || ir2out == irOFF)){
         t = bt;
      }/* if */

      if((Robot == BEGIN) && (ir1out == irON && ir2out == irON)){
         t1 = (bt - t) / CLK_TCK;
         setcolor(EGA_DARKGRAY);
         setfillstyle(SOLID_FILL, EGA_LIGHTGRAY);
         moveto(5, 447);
         gprintf("√Время детектирования: %4.2f ", t1);
         if(t1 > CarDetectTime){
            setcolor_obj_num(p, 100, 10, EGA_LIGHTGREEN, SHOW);
            setcolor_obj_num(p, 100, 11, EGA_LIGHTGREEN, SHOW);
            Robot = END;
            setcolor(EGA_DARKGRAY);
            setfillstyle(SOLID_FILL, EGA_LIGHTGREEN);
            gprintf("OK!\n");
         }/* if */
      }/* if */

      if(s == SHBSTART){
         load_file(&p, "tbg\\shb1out.tbg");
         outpic_all(p);
         angleUP = 0.0;
         angleDN = 90.0;
         s = SHBCOMMANDUP;
         cDN = cUP = 1;
         mf = 0;
         return 0;
      }/* if */

      if(s == SHBCOMMANDUP){
         if(shbS2 != shbsDOWN){
            if(mf == 0){
/*
               if(!Emulate){
                  RecordEvent(ErrorSHBNotWorking);
                  s = SHBWAITERROR;
               }/* if */
*/
               ErrorMsg("  Шлагбаум не   "
                        "   на нижнем    "
                        "   концевике.   ");
               mf = 1;
            }/* if */
            return 0;
         }/* if */
         outpic_all(p);
         mf = 0;
         shbC2 = shbcUP;
         RecordEvent(ComSHBUp);
         s = SHBWAITUP;
         t = bt;
         return 0;
      }/* if */

      if(s == SHBWAITUP){
         OutTime(p);

         setcolor_obj_num(p, 100, 10, ir1out == irOFF ? EGA_RED : EGA_LIGHTGREEN, SHOW);
         setcolor_obj_num(p, 100, 11, ir2out == irOFF ? EGA_RED : EGA_LIGHTGREEN, SHOW);

         if(((bt - t) / CLK_TCK) >= KUSHBTime){
            shbC2 = shbcOK;
         }/* if */

         if(shbS2 == shbsDOWN){
            if(((bt - t) / CLK_TCK) > 5.0F){
               RecordEvent(ErrorSHBNotWorking);
               shbC2 = shbcSTOP;
               s = SHBWAITERROR;
               ErrorMsg("  Шлагбаум не   "
                        "ушел с нижнего  "
                        "  концевика.    ");
            }/* if */
         }/* if */


         if(shbS2 == shbsUNKNOWN){
            if(tfUP == 0){
               outpic_obj_num(p, 100, 2);// screen clear
               if(bt % 2){
                  outpic_obj_num_with_rotate(p, 100, 1, 90.); // draw line
               }/* if */
               sprintf(st, "%.1f", (bt - t) / CLK_TCK);
               outtext_obj_num(p, 100, 3, getcolor_obj_num(p, 100, 2), st, SHOW);
               cUP++;
            }/* if */
            else{
               outpic_obj_num(p, 100, 2);// screen clear
               outpic_obj_num_with_rotate(p, 100, 1, angleUP > 90. ? 90. : angleUP); // draw line
               angleUP += out1timeUP;
            }/* elif */

            if(((bt - t) / CLK_TCK) > 10.0F){
               RecordEvent(ErrorSHBNotWorking);
               shbC2 = shbcSTOP;
               s = SHBWAITERROR;
               ErrorMsg("  Шлагбаум не   "
                        "стал на верхний "
                        "    концевик.   ");
            }/* if */
            tup = bt - t;

            if(Robot == END){
               t1 = (bt - t) / CLK_TCK;
               setcolor(EGA_DARKGRAY);
               setfillstyle(SOLID_FILL, EGA_LIGHTGRAY);
               moveto(5, 447);
               gprintf("√Время перекрытия: %4.2f    ", t1);
               if (ir1out == irOFF && ir2out == irOFF){
                  s = SHBWAITCAR;
                  outpic_obj_num(p, 100, 2);// screen clear
                  outpic_obj_num_with_rotate(p, 100, 1, 90./*angle*/); // draw line

                  shbC2 = shbcSTOP;
                  RecordEvent(ComSHBStop);
                  s = SHBWAITCAR;
                  t = bt;
               }/* if */
            }/* if */
         }/* if */

         if(shbS2 == shbsUP){
            sound(800);
            delay(10);
            nosound();

            if(tfUP == 0 && cUP != 0){
               out1timeUP = 90. / (double)cUP;
               tfUP = 1;
            }/* if */

            if(angleUP > 90.){
               cUP = 0;
               tfUP = 0;
            }/* if */
            outpic_obj_num(p, 100, 2);// screen clear
            outpic_obj_num_with_rotate(p, 100, 1, 90./*angle*/); // draw line

            shbC2 = shbcSTOP;
            RecordEvent(ComSHBStop);
            s = SHBWAITCAR;
            t = bt;
         }/* if */

         return 0;
      }/* if */

      if(s == SHBWAITCAR){
         OutTime(p);
         setcolor_obj_num(p, 100, 10, ir1out == irOFF ? EGA_RED : EGA_LIGHTRED, SHOW);
         setcolor_obj_num(p, 100, 11, ir2out == irOFF ? EGA_RED : EGA_LIGHTRED, SHOW);

         if(((bt - t) / CLK_TCK) >= KUSHBTime){
            shbC2 = shbcOK;
         }/* if */
// ----------------
         if((Robot == END) && (ir1out == irOFF && ir2out == irOFF)){
            Robot = STOP;
            s = SHBCOMMANDDOWN;
            mf = 0;
            sound(3180);
            delay(10);
            nosound();
            RecordEvent(CarDetected);
         }/* if */
// ----------------
         return 0;
      }/* if */

      // команда Опустить ШБ
      if(s == SHBCOMMANDDOWN){
/*
         if(shbS2 != shbsUP){
            if(mf == 0){
               if(!Emulate){
                  RecordEvent(ErrorSHBNotWorking);
                  s = SHBWAITERROR;
               }/* if */
               ErrorMsg("  Шлагбаум не   "
                        "  на верхнем    "
                        "   концевике.   ");
               mf = 1;
            }/* if */
            return 0;
         }/* if */
*/
         shbC2 = shbcDOWN;
         RecordEvent(ComSHBDown);
         s = SHBWAITDOWN;
         t = bt;
         return 0;
      }/* if */

      if(s == SHBWAITDOWN){
         OutTime(p);

         setcolor_obj_num(p, 100, 10, ir1out == irOFF ? EGA_RED : EGA_LIGHTRED, SHOW);
         setcolor_obj_num(p, 100, 11, ir2out == irOFF ? EGA_RED : EGA_LIGHTRED, SHOW);

         if(((bt - t) / CLK_TCK) >= KUSHBTime){
            shbC2 = shbcOK;
         }/* if */
/*
         if(shbS2 == shbsUP){
            if(((bt - t) / CLK_TCK) > 5.0F){
               RecordEvent(ErrorSHBNotWorking);
               shbC2 = shbcSTOP;
               s = SHBWAITERROR;
               ErrorMsg("  Шлагбаум не   "
                        "ушел с верхнего "
                        "  концевика.    ");
            }/* if */
         }/* if */
*/
         if(shbS2 == shbsUNKNOWN){
            if(tfDN == 0){
               outpic_obj_num(p, 100, 2);// screen clear
               if(bt % 2){
                  outpic_obj_num_with_rotate(p, 100, 1, 0.); // draw line
               }/* if */
               sprintf(st, "%.1f", (bt - t) / CLK_TCK);
               outtext_obj_num(p, 100, 3, getcolor_obj_num(p, 100, 2), st, SHOW);
               cDN++;
            }/* if */
            else{
               outpic_obj_num(p, 100, 2);// screen clear
               outpic_obj_num_with_rotate(p, 100, 1, angleDN < 0. ? 0. : angleDN); // draw line
               angleDN -= out1timeDN;
            }/* elif */

            if(((bt - t) / CLK_TCK) > 10.0F){
               RecordEvent(ErrorSHBNotWorking);
               shbC2 = shbcSTOP;
               s = SHBWAITERROR;
               ErrorMsg("  Шлагбаум не   "
                        "стал на нижний  "
                        "   концевик.    ");
            }/* if */
            tup = bt - t;
         }/* if */

         if(shbS2 == shbsDOWN){
            sound(400);
            delay(10);
            nosound();
            if(tfDN == 0 && cDN != 0){
               out1timeDN = 90. / (double)cDN;
               tfDN = 1;
            }/* if */

            if(angleDN < 0.){
               cDN = 0;
               tfDN = 0;
            }/* if */

            outpic_obj_num(p, 100, 2);// screen clear
            outpic_obj_num_with_rotate(p, 100, 1, 0./*angle*/); // draw line

            shbC2 = shbcSTOP;
            RecordEvent(ComSHBStop);
            s = SHBEND;
         }/* if */
         return 0;
      }/* if */

      if(s == SHBEND){
         del_pic(&p);
//         Restore_Image(p, "tbg\\shb.img");
         p = NULL;
         s = SHBSTART;
         return 1;
      }/* if */

      if(s == SHBWAITERROR){
         OutTime(p);
         if(Key == 13){ // Enter
            s = SHBERROR;
         }/* if */
         return 0;
      }/* if */

      if(s == SHBERROR){
         del_pic(&p);
         p = NULL;
         s = SHBSTART;
         return -1;
      }/* if */

    return 1;

}/* WorkSHBOut */
/* ----------------------------------------------------------------------- */
