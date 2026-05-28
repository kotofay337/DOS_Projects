#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
/* ----------------------------------------------------------------------- */
int GetGates(char *s);
/* ----------------------------------------------------------------------- */
#pragma argsused
void main (int argc, char *argv[]){
   GetGates(argv[1]);
}/* main */
/* ----------------------------------------------------------------------- */
int GetGates(char *s){
   char *t, c, ctok[16];
   char all[256] = "";
   int nt = 0;
   int n, r;

   if(s == NULL){
      return 0;
   }/* if */

   t = strtok(s, ",");
   do{
      if(t){
         nt++;
         if((r = sscanf(t, " %d %c ", &n, &c)) == 2){
            sprintf(ctok,"%d%c,", n, c);
            strcat(all, ctok);
         }/* if */
         else{
            printf("\nError in param %d arg %d\n", nt, r + 1);
            return -1;
         }/* elif */
      }/* if */
   }while((t = strtok(NULL, ",")) != NULL);
   n = strlen(all) - 1;
   if(n)
      all[n] = 0;

   return 1;

}/* GetGates */
/* ----------------------------------------------------------------------- */
