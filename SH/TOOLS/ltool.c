/* --------------------------------------------------------------------------
   LTOOL.C
   Implementation
   10:04:39 2-DEC-1997
   (C) Чумак А.И.
   (C) -г┐ Є _._.
-------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#ifdef __WIN32__
#include <windows.h>
#endif
#include "ltool.h"
/* ----------------------------------------------------------------------- */
int ladd_head(LPGENERIC_HEADER lph, LPGENERIC_LIST lpladd){
   if(lph != NULL){
      if(lph->head){
         lpladd->next = lph->head;
      }/* if */
      lph->head = lpladd;
      if(lph->tail == NULL){
         lph->current = lph->tail = lpladd;
//         lpladd->next = NULL;
      }/* if */
      return 0;
   }/* if */
   return 1;
}/* ladd_head */
/* ----------------------------------------------------------------------- */
int ladd_tail(LPGENERIC_HEADER lph, LPGENERIC_LIST lpladd){
   if(lph != NULL){
      if(lph->tail != NULL){
         lph->tail->next = lpladd;
         lph->tail = lpladd;
         lpladd->next = NULL;
      }/* if */
      else{
         return ladd_head(lph, lpladd);
      }/* elif */
      return 0;
   }/* if */
   return 1;
}/* ladd_tail */
/* ----------------------------------------------------------------------- */
LPGENERIC_LIST ldel_head(LPGENERIC_HEADER lph){
   if((lph != NULL) && (lph->head != NULL)){
      LPGENERIC_LIST lplt = lph->head;
      if(lph->current == lph->head){
         lph->current = lplt->next;
      }/* if */
      lph->head = lplt->next;
      if(lph->head == NULL){
         lph->tail = NULL;
      }/* if */
      return lplt;
   }/* if */
   return NULL;
}/* ldel_head */
/* ----------------------------------------------------------------------- */
LPGENERIC_LIST ldel_cur(LPGENERIC_HEADER lph){
   if((lph != NULL) && (lph->head != NULL)){
      LPGENERIC_LIST th = lph->head;
      LPGENERIC_LIST tt = lph->tail;
      LPGENERIC_LIST tc = lph->current;
      LPGENERIC_LIST save;
      // 1
      if((th == NULL) && (tt == NULL) && (tc == NULL)){
         // printf("delcur:(th == NULL) && (tt == NULL) && (tc == NULL)\n");
         return NULL;
      }/* if */

      // 2
      if((th != tc) && (tt == tc)){
         save = tc;
         while(th != NULL){
            if(th->next == tc)
               break;
            th = th->next;
         }/* while */
         lph->tail = th;
         lph->current = th;
         lph->current->next = save->next;
         // printf("delcur:(th != tc) && (tt == tc)\n");
         return save;
      }/* if */

      // 3
      if((th == tc) && (tt != tc)){
         save = th;
         lph->head = th->next;
         lph->current = lph->head;
         // printf("delcur:(th == tc) && (tt != tc)\n");
         return save;
      }/* if */

      // 4
      if((th == tc) && (tc == tt) && (th == tt)){
         save = th;
         lph->head = lph->tail = lph->current = NULL;
         //printf("delcur:(th == tc) && (tc == tt) && (th == tt)\n");
         return save;
      }/* if */
      
      // 5
      if((th != tc) && (tt != tc)){
         save = tc;
         while(th != NULL){
            if(th->next == tc)
               break;
            th = th->next;
         }/* while */
         lph->current = th;

         lph->current->next = save->next;
         lph->current = save->next;
         //printf("delcur:(th != tc) && (tt != tc)\n");
         return save;
      }/* if */
      
   }/* if */
   return NULL;
}/* ldel_cur */
/* ----------------------------------------------------------------------- */
void *ltohead(LPGENERIC_HEADER lph){
   if((lph != NULL) && (lph->head != NULL)){
      return lph->current = lph->head;             
   }/* if */
   return NULL;
}/* ltohead */
/* ----------------------------------------------------------------------- */
void *ltotail(LPGENERIC_HEADER lph){
   if((lph != NULL) && (lph->tail != NULL)){
      return lph->current = lph->tail;             
   }/* if */
   return NULL;
}/* ltotail */
/* ----------------------------------------------------------------------- */
void *lnext(LPGENERIC_HEADER lph){
   if((lph != NULL) && (lph->current != NULL)){
      return lph->current = lph->current->next;
   }/* if */
   return NULL;
}/* lnext */
/* ----------------------------------------------------------------------- */
void *lcur(LPGENERIC_HEADER lph){
   if((lph != NULL) && (lph->current != NULL)){
      return lph->current;
   }/* if */
   return NULL;
}/* lcur */
/* ----------------------------------------------------------------------- */
/* будьте осторожны с этой функцией ! */
int lsetcur(LPGENERIC_HEADER lph, LPGENERIC_LIST lpsc){
   if(lph != NULL){
      lph->current = lpsc;
      return 0;
   }/* if */
   return 1;
}/* lcur */
/* ----------------------------------------------------------------------- */
void *lprev(LPGENERIC_HEADER lph){
   if((lph != NULL) && (lph->head != NULL)){
      LPGENERIC_LIST tc = lph->current;
      LPGENERIC_LIST th = lph->head;
      if(lph->current == lph->head){
         return NULL;
      }/* if */
      else{
         while(th != NULL){
            if(th->next == tc)
               return th;
            th = th->next;
         }/* while */
      }/* elif */
      printf("Список разрушен. Stop");
      exit(EXIT_FAILURE);
   }/* if */
   return NULL;
}/* lprev */
/* ----------------------------------------------------------------------- */
void *lfinds(LPGENERIC_HEADER lph, LPGENERIC_LIST lplkey,
            int (*fcmp)(void *s1, void *s2)){
   LPGENERIC_LIST lpt = (LPGENERIC_LIST)ltohead(lph);
   while(lpt != NULL){
      if(fcmp(lplkey, lpt) == 1)
         break;
      lpt = (LPGENERIC_LIST)lnext(lph);
   }/* while */
   return lpt;
}/* lfind */
/* ----------------------------------------------------------------------- */
/*
s1  > s2 ? result:  1
s1 == s2 ? result:  0
s1  < s2 ? result: -1
int fcmp(LPTARGT s1, LPTARGT s2){
   if(s1->NF > s2->NF)
      return 1;
   if(s1->NF == s2->NF)
      return 0;
   if(s1->NF < s2->NF)
      return -1;
}
*/



void lsort(LPGENERIC_HEADER lph, int (*fcmp)(void *s1, void *s2)){
   LPGENERIC_LIST lpt;
   LPGENERIC_LIST min, del;
   static GENERIC_HEADER th;
   // по всему списку
   while(lph->head && lph->tail){
      min = lpt = (LPGENERIC_LIST)ltohead(lph);
      // ищем минимальный
      while(lpt){
         if(fcmp(lpt, min) == -1){
            min = lpt;
         }/* if */
         lpt = (LPGENERIC_LIST)lnext(lph);
      }/* while */
      // добавляем его в конец
      lsetcur(lph, min);
      del = ldel_cur(lph);
      ladd_tail(&th, del);
   }/* while */
   // переписываем список в старое место
   del = ldel_head(&th);
   while(del){
      ladd_tail(lph, del);
      del = ldel_head(&th);
   }/* while */

   return;
}/* lsort */
/* ----------------------------------------------------------------------- */
//#define DEBUG

#ifdef DEBUG
typedef
   struct TEST {
      struct GENERIC_LIST *next;
      char c;
   }TEST, *LPTEST;

typedef
   struct TEST_HEADER {
      LPGENERIC_LIST head;
      LPGENERIC_LIST tail;
      LPGENERIC_LIST current;
   }TEST_HEADER, *LPTEST_HEADER;

int fcmpc(LPTEST s1, LPTEST s2){
   if(s1->c > s2->c)
      return 1;
   if(s1->c == s2->c)
      return 0;
   if(s1->c < s2->c)
      return -1;
}/* fcmpc */

TEST_HEADER tl;

void main(void){
   LPTEST lptest, min;
   int i;

   lptest = (LPTEST)malloc(sizeof(TEST));
   lptest->c = '9';
   ladd_tail(&tl, lptest);

   lptest = (LPTEST)malloc(sizeof(TEST));
   lptest->c = '2';
   ladd_tail(&tl, lptest);

   lptest = (LPTEST)malloc(sizeof(TEST));
   lptest->c = '0';
   ladd_tail(&tl, lptest);

   lptest = (LPTEST)malloc(sizeof(TEST));
   lptest->c = '1';
   ladd_tail(&tl, lptest);

   lptest = (LPTEST)malloc(sizeof(TEST));
   lptest->c = '3';
   ladd_tail(&tl, lptest);

   /* Print all list */
   lptest = ltohead((LPGENERIC_HEADER)&tl);
   while(lptest){
      printf(". %c \n", lptest->c);
      lptest = lnext((LPGENERIC_HEADER)&tl);
   }/* while */
   printf(".\n");

   printf("Sort...\n");
   lsort(&tl, (int(*)(void*, void*))fcmpc);

   printf(".\n");
   /* Print all list */
   lptest = ltohead((LPGENERIC_HEADER)&tl);
   while(lptest){
      printf(". %c \n", lptest->c);
      lptest = lnext((LPGENERIC_HEADER)&tl);
   }/* while */

   /* Delete all list */
   lptest = ldel_head((LPGENERIC_HEADER)&tl);
   while(lptest){
      free(lptest);
      lptest = ldel_head((LPGENERIC_HEADER)&tl);
   }/* while */

   printf("\n.\n");

   getch();

}/* main */

#endif
/* ----------------------------------------------------------------------- */



