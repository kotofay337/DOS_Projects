

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <bios.h>
#include <string.h>
/* ----------------------------------------------------------------------- */
void main (int argc, char *argv[]){
   FILE *f1, *f2;
   char s1[255], s2[255];
   unsigned long l = 0;

   if(argc == 3){
      f1 = fopen(argv[1], "rb");
      f2 = fopen(argv[2], "rb");

      while(f1 && f2 && !feof(f1) && !feof(f2)){
         if(fgets(s1, sizeof(s1), f1) && fgets(s2, sizeof(s2), f2)){
            l++;
            *(strrchr(s1, '\r\n')) = '\0';
            *(strrchr(s2, '\r\n')) = '\0';
         }/* if */
         if(strcmp(s1, s2)){
            printf("\nLine %4lu: <%s> <%s>", l, s1, s2);
         }/* if */
      }/* while */
      fclose(f2);
      fclose(f1);
   }/* if */
   printf("\n");
   exit(-1);
}/* main */
/* ----------------------------------------------------------------------- */
