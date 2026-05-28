#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include "tbg_tc.h"
/* ----------------------------------------------------------------------- */
void main (void){

   int gd = DETECT, gm, 
       stop = 0, 
       direction = -1, 
       d = -1,
       key = 0;

   float dtime = 8,
         t = 8;

   OBJECT **p = NULL;
   char *TBG_PATH = "c:\\tbg\\bin";

   initgraph(&gd, &gm, TBG_PATH);
   MPCSetPath(TBG_PATH);
   setpalette_tbg();

   load_file(&p, ".\\demopipe.tbg");

   outpic_all(p);

   setstate_obj_num(p, 255, 255, 4, SHOW); // Off

   while(key != 27){

      if((d != direction) && (t > 8)){
         dtime += .25f;
         if(dtime >= t){
            direction = d;
            if(d == -1){
               setstate_obj_num(p, 0, 1, 41, SHOW);
               setstate_obj_num(p, 0, 2, 41, SHOW);
            }/* if */
            if(d == -2){
               setstate_obj_num(p, 0, 1, 4, SHOW);
               setstate_obj_num(p, 0, 2, 4, SHOW);
            }/* if */
         }/* if */
      }/* if */

      if((d == direction) && (t > 8)){
	 dtime -= 0.25f;
	 if(dtime <= 8)
	    t = 8;
      }/* if */

      delay(dtime);

      setcolor_obj_num(p, 0, 0, direction, SHOW);

      if(kbhit()){
         key = getch();
         switch(key){
            case ',':
               d = -2;
               t = 20;
               break;
            case '.':
               d = -1;
               t = 20;
               break;
            case ' ':
               stop = !stop;
               break;
            case 'c':
               direction = LIGHTBLUE;
               break;
         }/* switch */
      }/* if */

   }/* while */

   del_pic(&p);

   clrscr();

   load_file(&p, ".\\testkey.tbg");

   outpic_all(p);

   getch();
   setstate_obj_num(p, 1, 1, 5, SHOW);

   getch();
   setstate_obj_num(p, 1, 1, 6, SHOW);

   getch();
   setstate_obj_num(p, 1, 1, 7, SHOW);

   getch();
   del_pic(&p);

   closegraph();

}/* main */
/* ----------------------------------------------------------------------- */
