#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
/* ----------------------------------------------------------------------- */
typedef
   union {
      struct {
         unsigned b0:1;
         unsigned b1:1;
         unsigned b2:1;
         unsigned b3:1;
         unsigned b4:1;
         unsigned b5:1;
         unsigned b6:1;
         unsigned b7:1;
      }bf;
      unsigned char c;
   }BYTE;
/* ----------------------------------------------------------------------- */
BYTE byte;
void main (void){
   FILE *matrix;
   int i = 0, cc = 0, bfc = 0;
   int r;

   fprintf(stdprn,
   "\033Q178"
   "\033S2"
   "\033D7"
   "\033?1"
   "\033A11"
   "\033M07"
   "Администратор\x0D"
   "\033M01"
   "\033X2"
   "\033A18"
   "\033"
   "{1000000}\x0D"
   "\033A12"
   );
   fprintf(stdprn, "\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D"); return;

   fprintf(stdprn,
   "\033Q178"
   "\033S2"
   "\033D5"
   "\033?1"
   "\033A11"
   "\033M18"
   "BARCODE 128\x0D"
   "\033"
   "{12345678901234}\x0D"
   "\033M28"
   "123.45"
   "\x0D\x0A"
   "\033M01"
   );
   fprintf(stdprn, "\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D"); return;


   matrix = fopen("matrix.dat", "rb");
   if(matrix == NULL){
      return ;
   }/* if */
   fprintf(stdprn, "\033G10");
   byte.c = 0;
   bfc = 7;
   while(!feof(matrix)){
      r = fgetc(matrix);
      i++;
      if((r == ' ') || (r == '*')){
	 if(bfc < 0){
	    bfc = 7;
	    byte.c = 0;
	 }/* if */
	 switch(bfc){
	    case 0: r == '*' ? byte.bf.b0 = 1 : (byte.bf.b0 = 0); break;
	    case 1: r == '*' ? byte.bf.b1 = 1 : (byte.bf.b0 = 0); break;
	    case 2: r == '*' ? byte.bf.b2 = 1 : (byte.bf.b0 = 0); break;
	    case 3: r == '*' ? byte.bf.b3 = 1 : (byte.bf.b0 = 0); break;
	    case 4: r == '*' ? byte.bf.b4 = 1 : (byte.bf.b0 = 0); break;
	    case 5: r == '*' ? byte.bf.b5 = 1 : (byte.bf.b0 = 0); break;
	    case 6: r == '*' ? byte.bf.b6 = 1 : (byte.bf.b0 = 0); break;
	    case 7: r == '*' ? byte.bf.b7 = 1 : (byte.bf.b0 = 0); break;
	 }/* switch */
	 if(bfc == 0){
	    fputc(byte.c, stdprn);
	 }/* if */
	 bfc--;
      }/* if */
      else{
	 if(r == '\n'){
	    i = 0;
	    fprintf(stdprn, "\033G10", 13);
	 }/* if */
      }/* elif */
   }/* while */
   fprintf(stdprn, "\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0A\033C");
   fclose(matrix);

}/* main */
/* ----------------------------------------------------------------------- */
