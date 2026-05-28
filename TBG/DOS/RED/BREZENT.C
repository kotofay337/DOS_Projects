#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <graphics.h>
/* ----------------------------------------------------------------------- */
#define STEP 6
#define CalcXY {  t->x = x;\
                  t->y = y;\
                  t++;\
                  dp++;\
               }
/* ----------------------------------------------------------------------- */
typedef
   struct point{
      int x;
      int y;
   }POINT;
/* ----------------------------------------------------------------------- */
POINT *LINE2PIPE (POINT *l,int *count){
   int x,y,dx,dy,s1,s2,tmp,e,swap,i,p = 0;
   int x1,y1,x2,y2,dp;
   POINT *pipe,*t;
   x1 = l->x;
   y1 = l->y;
   x2 = (l+1)->x;
   y2 = (l+1)->y;
   x = x1;
   y = y1;
   dx = abs(x2 - x1);
   dy = abs(y2 - y1);
   s1 = (x2 - x1) <  0 ? -1 :
        (x2 - x1) == 0 ?  0 : 1 ;
   s2 = (y2 - y1) <  0 ? -1 :
        (y2 - y1) == 0 ?  0 : 1 ;
   if(dy > dx){
      tmp = dx;
      dx = dy;
      dy = tmp;
      swap = 1;
   }/* if */
   else
      swap = 0;
   e = 2 * dy - dx;
   dp = p = 0;
   dp++;
   for(i = 1;i <= dx;i++){
      if(p == STEP){
         dp++;
         p = 0;
      }/* if */
      else
         p++;
      while(e >= 0){
         if(swap == 1)
            x += s1;
         else
            y += s2;
         e = e - 2 * dx;
      }/* while */
      if(swap == 1)
         y += s2;
      else
         x += s1;
      e = e + 2 * dy;
   }/* for */
   dp++;
   t = pipe = (POINT*)calloc(dp,sizeof(POINT));
   if(t == NULL)
      return NULL;
   x = x1;
   y = y1;
   e = 2 * dy - dx;
   dp = p = 0;
   CalcXY;
   for(i = 1;i <= dx;i++){
      if(p == STEP){
         CalcXY;
         p = 0;
      }/* if */
      else
         p++;
      while(e >= 0){
         if(swap == 1)
            x += s1;
         else
            y += s2;
         e = e - 2 * dx;
      }/* while */
      if(swap == 1)
         y += s2;
      else
         x += s1;
      e = e + 2 * dy;
   }/* for */
   CalcXY;
   *count = dp;
   return pipe;
}/* LINE2PIPE */
/* ----------------------------------------------------------------------- */
void main (void){
   int gm = EGAHI,gd = EGA;
   int dp,i;
   POINT l[2],*p;
   initgraph(&gd,&gm,"e:\\bc\\bgi");

   l[0].x = 100;
   l[0].y = 200;
   l[1].x = 305;
   l[1].y = 200;
   p = LINE2PIPE(l,&dp);
   setlinestyle(SOLID_LINE,0,NORM_WIDTH);
   for(i = 0;i < dp;i++){
      setcolor((i % 2) + 1);
      line((p  +i)->x,(p  +i)->y,
           (p+1+i)->x,(p+1+i)->y);
   }/* for */
   free(p);
   getch();
   closegraph();

}/* main */
/* ----------------------------------------------------------------------- */
