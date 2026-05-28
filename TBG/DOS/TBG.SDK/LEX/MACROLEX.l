%{
#include <string.h>
#include "lex.h"
%}
%%

[\x00-\x09\x0B-\x0C\x0E-\x21\x23-\x2f\x3b-\x40\x5b-\x60\x7b-\x7f\xb0-\xdf\xf2-\xff] {;}

[-0-9]* {
   sscanf(yytext,"%d",&vali);
   return NUMINT;
}
[-0-9.0-9]* {
   sscanf(yytext,"%lf",&vald);
   return NUMFLT;
}

[a-zA-Z -ορ€-π_][0-9a-zA-Z -ορ€-π_]* {
   strncpy(buftxt, yytext, sizeof(buftxt));
   return KEYWORD;
}

[:] {
   return SEP;
}

"/*" {               
   int c, i = 0, p;
while(1){
   while((c = input()) != '/'){
      if(c == '\n'){
         if(pnumln){
            (*pnumln)++;
         }/* if */
      }/* if */
      if(c == EOF)
         return 0;
      p = c;
   }/* while */
   if(p == '*')
      return COME;
}
}

;"*/" {               
//  comment--;
  return COME;
;}

"\"" {
   int c, i = 0;
   while((c = input()) != '\"'){
      if(i < sizeof(buftxt) - 1){
         buftxt[i] = c;
         if(c == '\n'){
            if(pnumln){
               (*pnumln)++;
            }/* if */
         }/* if */
      }/* if */
      else{
         break;
      }/* elif */
      i++;
   }/* while */
   buftxt[i] = 0;
   maxbuftxt = i;
   return TXT;
}
"//" {
   int c;
   do{
      c = input();
   }while((c != '\n') && (c != EOF));/* while */
   if(pnumln){
      (*pnumln)++;
   }/* if */
   return COMCPP;
}

"\x0D\x0A" {
   if(pnumln)
      (*pnumln)++;
   return NEWLINE;
}
