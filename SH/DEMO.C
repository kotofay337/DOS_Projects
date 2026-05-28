#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include "tbg_tc.h"
/* ----------------------------------------------------------------------- */
extern int far _Cdecl   simplex_font_far[];
extern int far _Cdecl     bold_font_far[];
/* ----------------------------------------------------------------------- */
void main (void){
   OBJECT *demo = NULL;
   int GDrive = VGA, GMode = VGAHI, errorcode;
   FILE *f;
   char fn[255];

   registerfarbgidriver(EGAVGA_driver_far);
   registerfarbgifont(simplex_font_far);
   registerfarbgifont(bold_font_far);

   initgraph(&GDrive, &GMode, "");

   f = fopen("demo.txt", "rt");

   fgets(fn, sizeof(fn), f);
   while(!feof(f)){
      if(!load_file(&demo, strtok(fn, "- "))){
         outpic_all(demo);
         del_pic(&demo);
         /*
         while(1){

         }/* while */
         */
         getch();
      }/* if */
      fgets(fn, sizeof(fn), f);
   }/* while */

   fclose(f);
   closegraph();

}/* main */
/* ----------------------------------------------------------------------- */
