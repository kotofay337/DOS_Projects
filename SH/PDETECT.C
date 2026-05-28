#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <bios.h>
/* ----------------------------------------------------------------------- */
void print16bit(unsigned short int uw, int sb, int eb){
   int i;
   unsigned short int t = uw;
   sb = 15 - sb;
   eb = 15 - eb;
   if(sb >= 0 && eb >= sb){
      for(i = sb;i <= eb;i++){
         cprintf("%c", ((t << i) & 0x8000) == 0 ? '0' : '1');
      }/* for */
   }/* if */
}/* print16bit */
/* ----------------------------------------------------------------------- */
#pragma argsused
void main (int argc, char *argv[]){
   unsigned PortNum, PortNumStart = 0, PortNumEnd = 0xFFFFU, p, v, y = 1;
   unsigned *pa, pb = 0;
   printf("\nPort DETECT");
   if(argc >= 3){
      sscanf(argv[1], "%x", &PortNumStart);
      sscanf(argv[2], "%x", &PortNumEnd);
      printf(":from:%X to:%X", PortNumStart, PortNumEnd);
      if(argv[3][0] == 'B'){
         pb = 1;
         printf(" + Binary decode");
      }/* if */
   }/* if */
   else{
      printf("\nUse: pdetect SPort EPort"
      "\nfor: SPort & EPort = HEX number value ports");
      exit(1);
   }/* elif */
   if(PortNumEnd < PortNumStart){
      printf("\nAttention: SPort must be greater EPort.");
      exit(-1);
   }/* if */
   getch();
   clrscr();
   pa = calloc(PortNumEnd - PortNumStart + 1, sizeof(int));

   for(p = 0, PortNum = PortNumStart ;PortNum < PortNumEnd;PortNum++, p++){
      if(kbhit()){
         if(getch() == 0){
            getch();
         }/* if */
         free(pa);
         exit(2);
      }/* if */
      pa[p] = inportb(PortNum);
   }/* for */
//   randomize();
   while(!kbhit()){
      gotoxy(1, 1); y = 1;
      for(p = 0, PortNum = PortNumStart ;PortNum <= PortNumEnd;PortNum++, p++){
         if(p && p % 25 == 0){
            if(pb){
               y += 13;
            }/* if */
            else{
               y += 7;
            }/* elif */
            y = (y % (pb ? 80 - 13 : 80 - 7));
         }/* if */
         v = inportb(PortNum);
//         v = random(0xF);
         if(pa[p] != v){

            gotoxy(y, (p % 25) + 1);

            cprintf("|%3X:", PortNum);
            if(pb){
               print16bit(v, 7, 0);
            }/* if */
            else{
               cprintf("%2X", v);
            }/* elif */
            pa[p] = v;
         }/* if */
      }/* for */
   }/* while */
   if(getch() == 0){
      getch();
   }/* if */
   free(pa);
   printf("\n");
   exit(1);
}/* main */
/* ----------------------------------------------------------------------- */

