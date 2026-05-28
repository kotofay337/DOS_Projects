/* -----------------------------------------------------------------------
*
*  tstSHB.C
*
*  ’¥αβ θ« £΅ γ¬ 
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

#define DEBUG

/* ----------------------------------------------------------------------- */
typedef
   enum{
      tSHBSTART,
      tSHBWAITCOMMAND,
      tTSTSCANNER,
      tWAITTSTSCANNER,
      tTSTBCPRINTER,
      tWAITTSTBCPRINTER,
      STOPPROGRAM
   }tSHB;

static tSHB Step = tSHBSTART;
static long tt;
static char* psTstScan;
static int   ncTstScan;

int TestSHB(void){

   if(Step == tSHBSTART){
      load_file(&p, "tbg\\test1.tbg");
      outpic_all(p);
      Step = tSHBWAITCOMMAND;
      return 0;
   }/* if */

   if(Step == tSHBWAITCOMMAND){
      OutTime(p);
      if(Key == '1' && shbC1 == shbcOK){
         setcolor_obj_num(p, 3, 0, EGA_LIGHTGREEN, SHOW);
         shbC1 = shbcUP;
         sound(800);
         delay(4);
         nosound();
         tt = bt;
      }/* if */
      if(Key == '2' && shbC1 == shbcOK){
         setcolor_obj_num(p, 3, 1, EGA_LIGHTGREEN, SHOW);
         shbC1 = shbcDOWN;
         sound(400);
         delay(4);
         nosound();
         tt = bt;
      }/* if */

      if(Key == '3' && shbC1 == shbcOK){
         Step = tTSTSCANNER;
         setcolor_obj_num(p, 99, 0, EGA_LIGHTGRAY, SHOW);
         setcolor_obj_num(p, 3, 2, EGA_LIGHTGREEN, SHOW);
         psTstScan = gettext_obj_num(p, 99, 0);
         ncTstScan = 0;
         return 0;
      }/* if */

      if(Key == '4' && shbC1 == shbcOK){
         setcolor_obj_num(p, 3, 3, EGA_LIGHTGREEN, SHOW);
         PrintBC(9999, "9999", "’…‘’‚›‰", "β¥αβ®Άλ©", "β¥αβ®Άλ©", "β¥αβ®Άλ©", 2345678);
         setcolor_obj_num(p, 3, 3, EGA_WHITE, SHOW);
         return 0;
      }/* if */

      if(ExtKey == 68){ // F10
         Step = STOPPROGRAM;
      }/* if */

      outtext_obj_num(p, 1, 0, EGA_LIGHTGRAY, ir1 == irON ? "‚‹." : "‚›‹.", NO_SHOW);
      outtext_obj_num(p, 1, 1, EGA_LIGHTGRAY, ir2 == irON ? "‚‹." : "‚›‹.", NO_SHOW);

      outtext_obj_num(p, 2, 0, EGA_LIGHTGRAY, shbS1 == shbsUP   ? "‚‹." : "‚›‹.", NO_SHOW);
      outtext_obj_num(p, 2, 1, EGA_LIGHTGRAY, shbS1 == shbsDOWN ? "‚‹." : "‚›‹.", NO_SHOW);

      if((shbC1 != shbcOK) && (((bt - tt) / CLK_TCK) > KUSHBTime)){
         shbC1 = shbcOK;
         setcolor_obj_num(p, 3, 0, EGA_WHITE, SHOW);
         setcolor_obj_num(p, 3, 1, EGA_WHITE, SHOW);
      }/* if */

      return 0;
   }/* if */

   if(Step == tTSTSCANNER){
      OutTime(p);
      if(Key){
         if(ncTstScan < strlen_obj_num(p, 99, 0)){

            psTstScan[ncTstScan] = (unsigned)Key >= ' ' ? (unsigned)Key : '.';

            if(ncTstScan == 0){
               setcolor_obj_num(p, 99, 0, (psTstScan[ncTstScan] == 'b') ? EGA_LIGHTGREEN : EGA_LIGHTRED, NO_SHOW);
            }/* if */

            if(Key == ENTER){
               psTstScan[ncTstScan] = '\0';
               outpic_obj_num(p, 99, 0);
               setcolor_obj_num(p, 3, 2, EGA_WHITE, SHOW);
               Step = tSHBWAITCOMMAND;
            }/* if */
            ncTstScan++;
         }/* if */
         else{
            psTstScan[ncTstScan] = 0;
         }/* elif */
      }/* if */
      if(ExtKey == 68){
         Step = tSHBWAITCOMMAND;
         setcolor_obj_num(p, 3, 2, EGA_WHITE, SHOW);
      }/* if */
      return 0;
   }/* if */

   if(Step == STOPPROGRAM){
      del_pic(&p);
      Step = tSHBSTART;
      return 1;
   }/* if */

   return 1;
}/* TestSHB */
