
#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <dos.h>
#include <io.h>
#include <graphics.h>

#include "red5.h"

/****************************************************************************/
int pos_mouse()
{
 short int x,y;

 //   номер объекта -9
 //   номер элем.   -1  2  3  4  5  6  7  8  9 10 11
 static char m9 []={13,14,15,16,17,18,19,34,35,36,20};

 static char m11[]=
// 1  2  3  4  5  6  7  8  9  -  1  2  3  4  5  6  7  8  9  -
 { 4, 6, 1, 5, 8,10,12,30,31,32,
  33,37,38,39,40,48,50,49,46,47,
  43,51,41,42,52,44,45,53,54,55,
  2
 };

 int pos_mm;

 if(mn1_show==OFF) return 0; // меню нет
 mouse_getxy(&x,&y);/* берем координаты мыши */
 if(pren(menu_1,255,1,x,y) == OFF)
    return 0; // 'мышь' вне меню
 pos_mm=getpos_mouse_obj(menu_1,9,1);// выяснение позиции "мыши"
// printf("%d  \r",m1[pos_mm-1]);
 switch(pos_mm)
 {
  case -1:
  break;
  default:
  return m9[pos_mm-1];
 }//switch

 pos_mm=getpos_mouse_obj(menu_1,11,1);// выяснение позиции "мыши"
// printf("i=%d  %d  \r",pos_mm,m11[pos_mm-1]);
 switch(pos_mm)
 {
  case -1:
  break;
  default:
  return m11[pos_mm-1];
 }//switch

/*
 if(pren(&menu_1,1, 1)==ON) return 13;/* линия */
 if(pren(&menu_1,1, 2)==ON) return 14;/* R */
 if(pren(&menu_1,1, 3)==ON) return 15;/* B */
 if(pren(&menu_1,1, 4)==ON) return 16;/* E */
 if(pren(&menu_1,1, 5)==ON) return 17;/* T */
 if(pren(&menu_1,1, 6)==ON) return 18;/* Z */
 if(pren(&menu_1,1, 7)==ON) return 19;/* F */
 if(pren(&menu_1,1, 8)==ON) return 34;/* CP */
 if(pren(&menu_1,1, 9)==ON) return 35;/* MV */
 if(pren(&menu_1,1,10)==ON) return 36;/* MS */
*/

// if(pren(&menu_1,1, 8)==ON) return 20;/* сплайн */
/*
 if(pren(&menu_1,2, 1)==ON) return  4;/* помощь */
 if(pren(&menu_1,2, 2)==ON) return  6;/* выход */
 if(pren(&menu_1,2, 3)==ON) return  3;/* линия */
 if(pren(&menu_1,2, 4)==ON) return  5;/* закраска */
 if(pren(&menu_1,2, 5)==ON) return  8;/* ориентация */
 if(pren(&menu_1,2, 6)==ON) return 10;/* стиль */
 if(pren(&menu_1,2, 7)==ON) return 11;/* размер */
 if(pren(&menu_1,2, 8)==ON) return 30;/* сохранение */
 if(pren(&menu_1,2, 9)==ON) return 31;/* LD */
 if(pren(&menu_1,2,10)==ON) return 32;/* KR */
 if(pren(&menu_1,2,11)==ON) return 33;/* DL */
 if(pren(&menu_1,2,12)==ON) return 37;/* OU */
 if(pren(&menu_1,2,13)==ON) return 38;/* ID */
 if(pren(&menu_1,2,14)==ON) return 39;/* @ */
 if(pren(&menu_1,2,15)==ON) return 40;/*  */
 if(pren(&menu_1,2,16)==ON) return 48;/*  */
 if(pren(&menu_1,2,17)==ON) return 50;/*  */
 if(pren(&menu_1,2,18)==ON) return 49;/*  */
 if(pren(&menu_1,2,19)==ON) return 46;/*  */
 if(pren(&menu_1,2,20)==ON) return 47;/*  */
 if(pren(&menu_1,2,21)==ON) return 43;/*  */
 if(pren(&menu_1,2,22)==ON) return 51;/*  */
 if(pren(&menu_1,2,23)==ON) return 41;/*  */
 if(pren(&menu_1,2,24)==ON) return 42;/*  */
 if(pren(&menu_1,2,25)==ON) return 52;/*  */
 if(pren(&menu_1,2,26)==ON) return 44;/*  */
 if(pren(&menu_1,2,27)==ON) return 45;/*  */
 if(pren(&menu_1,2,28)==ON) return 53;/*  */
 if(pren(&menu_1,2,29)==ON) return 54;/*  */
 if(pren(&menu_1,2,30)==ON) return 55;/*  */
*/

 return 0;

}/* pos_mouse */
/***************************************************************************/

// визуализация нажатия кнопки
void visual_on(OBJECT **menu,unsigned char numobj,unsigned char num)
{
 int colx = getcolor_obj_num(menu,numobj,num );
 int coly = getcolor_obj_num(menu,numobj,num+1);
 // нажали
 setcolor_obj_num(menu,numobj,num,  coly,SHOW);
 setcolor_obj_num(menu,numobj,num+1,colx,SHOW);
 delay(100);

 // отпустили
 setcolor_obj_num(menu,numobj,num,  colx,SHOW);
 mouse_off();
 setcolor_obj_num(menu,numobj,num+1,coly,SHOW);
 mouse_on();

}//  visual_on
/***************************************************************************/


