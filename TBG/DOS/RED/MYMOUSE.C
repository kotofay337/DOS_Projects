#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <graphics.h>
/* ----------------------------------------------------------------------- */
int Cursor_mode = 1;                            //  _|ð- ò£ ¡r _
int Cursor_count = 0;                           // ¡§á¢§ðò ò£ ¡r _
double Cursor_angle = 0.;                       // £_r ôr÷r r¢_ ò£ ¡r _
int oldmx, oldmy;
//=================================================================================================
// ôr÷r r¢ ¢r§òð Xt Yt r¢-r¡ð¢_¬-r ¦_-¢ _ Xc Yc -_ £_r a
static void rotate_point(double a,double *Xt,double *Yt,double Xc,double Yc){

  static short int xa = 0,ya = 0;
  double dXt = (*Xt) - Xc,
         dYt = (*Yt) - Yc,
         sina, cosa,
         tx, ty;

  a /= 57.29577951472;

  sina = sin(a);
  cosa = cos(a);

  tx = dXt;
  ty = dYt;

  dXt = (tx * cosa) - (ty * sina);
  dYt = (tx * sina) + (ty * cosa);

  if(!ya){ // ¢§_âì ®â­®è¥­¨¥ áâ®à®­ ¤¨á¯«¥_
     getaspectratio((int far *)&xa, (int far *)&ya);
  }// if

  // áª®àà¥ªâ¨à®¢ âì "­¥ª¢ ¤à â­®áâì" â®çª¨
  if((xa != 10000)||(ya != 10000)){
     dYt /= ((double)ya / 10000);
     dXt /= ((double)xa / 10000);
  }// if

  *Xt = Xc + dXt;
  *Yt = Yc + dYt;

}// rotate_point
/*--------------------------------------------------------------------------*/
// ¢ðô« ý¯ £ýrö¡¢÷_ ôr÷r r¢r÷
typedef
   struct POINT {
      int x;
      int y;
   }POINT, *LPPOINT;

typedef
   struct LINE {
      POINT ps;
      POINT pe;
   }LINE, *LPLINE;

// ò£ ¡r («)

LINE c1[] = {
 {{-11, - 4}, {-11, + 4}},
 {{ 11, - 4}, { 11, + 4}},

 {{- 4, -11}, {+ 4, -11}},
 {{- 4, +11}, {+ 4, +11}},

 {{- 9,   0}, {- 5,   0}},
 {{+ 5,   0}, {+ 9,   0}},

 {{  0, - 9}, {  0, - 5}},
 {{  0, + 5}, {  0, + 9}},

 {{- 1,   0}, {+ 1,   0}}
};

LINE c2[] = {
 {{-11, - 3}, {-11, + 3}},
 {{ 11, - 3}, { 11, + 3}},

 {{- 3, -11}, {+ 3, -11}},
 {{- 3, +11}, {+ 3, +11}},

 {{-10,   0}, {- 6,   0}},
 {{+ 6,   0}, {+10,   0}},

 {{  0, -10}, {  0, - 6}},
 {{  0, + 6}, {  0, +10}},

 {{  0,   0}, {  0,   0}}
};

LINE c3[] = {
 {{-12, - 4}, {-12, + 4}},
 {{ 12, - 4}, { 12, + 4}},

 {{- 4, -12}, {+ 4, -12}},
 {{- 4, +12}, {+ 4, +12}},

 {{- 9,   0}, {- 5,   0}},
 {{+ 5,   0}, {+ 9,   0}},

 {{  0, - 9}, {  0, - 5}},
 {{  0, + 5}, {  0, + 9}},

 {{- 1,   0}, {+ 1,   0}}
};

LINE c4[] = {
 {{-12, - 3}, {-12, + 3}},
 {{ 12, - 3}, { 12, + 3}},

 {{- 3, -12}, {+ 3, -12}},
 {{- 3, +12}, {+ 3, +12}},

 {{-10,   0}, {- 6,   0}},
 {{+ 6,   0}, {+10,   0}},

 {{  0, -10}, {  0, - 6}},
 {{  0, + 6}, {  0, +10}},

 {{  0,   0}, {  0,   0}}
};
// ô_÷_®©ð_ ý¯ ÷«§ð¡_-ð¯ ôr÷r r¢_
static double __x, __y, __a;
/*--------------------------------------------------------------------------*/
//  ð¡r÷_-ð_ ð-ðð ¡ ôr÷r r¢r-
void DrawLineA(int ia, LPLINE c){
   LINE l;

   __a = (double)Cursor_angle;

   __x = (double)(oldmx + c[ia].ps.x);
   __y = (double)(oldmy + c[ia].ps.y);
   rotate_point(__a, &__x, &__y, (double)oldmx, (double)oldmy);
   l.ps.x = (int)(__x + .5);
   l.ps.y = (int)(__y + .5);

   __x = (double)(oldmx + c[ia].pe.x);
   __y = (double)(oldmy + c[ia].pe.y);
   rotate_point(__a, &__x, &__y, (double)oldmx, (double)oldmy);
   l.pe.x = (int)(__x + .5);
   l.pe.y = (int)(__y + .5);

   line(l.ps.x, l.ps.y,
        l.pe.x, l.pe.y);

}/* DrawLineA */
/*--------------------------------------------------------------------------*/
// ¢r-|_ ö_ ôr÷r r¢_
void DrawLineB(int ia, LPLINE c){

   line(oldmx + c[ia].ps.x, oldmy + c[ia].ps.y,
        oldmx + c[ia].pe.x, oldmy + c[ia].pe.y);

}/* DrawLineB */
/*--------------------------------------------------------------------------*/
void Cursor(void) {                              // _____ _________ ____

   int i;

   setlinestyle(SOLID_LINE,0,THICK_WIDTH);
   setcolor(8);
   switch(Cursor_mode){
      case 1:
      case 2:
         for(i = 0;i < (sizeof(c1) / sizeof(LINE)) ;i++)
            DrawLineA(i, c1);
         break;
      case 3:
      case 4:
         for(i = 0;i < (sizeof(c3) / sizeof(LINE)) ;i++)
            if(Cursor_count % 2)
               DrawLineB(i, c1);
            else
               DrawLineB(i, c3);
         break;
   }/* switch */

   setlinestyle(SOLID_LINE,0,NORM_WIDTH);
   setcolor(6);
   switch(Cursor_mode){
      case 1:
      case 2:
         for(i = 0;i < (sizeof(c2) / sizeof(LINE)) ;i++)
            DrawLineA(i, c2);
         break;
      case 3:
      case 4:
         for(i = 0;i < (sizeof(c4) / sizeof(LINE)) ;i++)
            if(Cursor_count % 2)
               DrawLineB(i, c2);
            else
               DrawLineB(i, c4);
         break;
   }/* switch */

}/*void Cursor(void)*/
//=================================================================================================
void HideShowMyMouseCursor() {                   // _____ _________ _________ ____ (_____-_______)
 setwritemode(XOR_PUT);                          // _¡¢_-_÷ò_  _|ð-_ _ýýð¢ð÷-r_r ÷«÷rý_
 Cursor();
 setwritemode(COPY_PUT);                         // _r¡¡¢_-_÷_-ð_  _|ð-_ ô ¯-r_r ÷«÷rý_
}
//=================================================================================================
