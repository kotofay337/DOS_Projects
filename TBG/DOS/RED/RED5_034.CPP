// red5_034.cpp

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"
#include "tbg_tc.h"

//-------------------------------------------------------------------------
Reper::Reper(void){//конструктор
  x=mmx(xkom_strt);
  y=mmy(ykom_strt);
// printf("Constructor\n");
};// Macro

inline void Reper::put_y(int p){y=p;/* printf("put mcy=%d\n",y);*/};
inline void Reper::put_x(int p){x=p;/* printf("put mcx=%d\n",x);*/};

inline int  Reper::get_x(void){/*printf("get x=%d\n", x);*/return x;};
inline int  Reper::get_y(void){/*printf("get y=%d\n", y);*/return y;};
inline int  Reper::get_oldx(void){/*printf("get oldx=%d\n", oldx);*/return oldx;};
inline int  Reper::get_oldy(void){/*printf("get oldy=%d\n", oldy);*/return oldy;};

inline char Reper::get_tip(void){/*printf("\n");*/return TIP_reper;};

void Reper::save(FILE *inf)//сохранение в файле
{
//  printf("save %d %d %d %d %d\n", index, mcx, mcy, mode, cs);
  fwrite(&x,1,4,inf);
/*
  fwrite(&mcx,  1,2,inf);
  fwrite(&mcy,  1,2,inf);
*/
}//save

void Reper::load(FILE *outf)//загрузка из файла
{
  fread(&x,1,4,outf);
/*
  fread(&mcx,  1,2,outf);
  fread(&mcy,  1,2,outf);
*/
}//load

     //признак 'попал/не попал' элемент в рамку
inline char Reper::GetSelect(void){/*printf("getselect %d\n", select);*/return select;};

//-------------------------------------------------------------------------
void Reper::Vibor(void)//выбор элемента попавшего в рамку
{
 int xmin,xmax,ymin,ymax;

// printf("Vibor\n");
 //вычисление размера элемента
 xmin=x;ymin=y;
 xmax=x;ymax=y;
 //вычисление размера элемента

 // условие попадания
  if((xmin>=rx_min)&&(ymin>=ry_min)&&
     (xmax<=rx_max)&&(ymax<=ry_max))
    {// попал в рамку
     ind_select=ON;
     count_select++;
     select=ON;
     oldx=x;
     oldy=y;
//     rx=x-rx_min;
//     ry=y-ry_min;
    }// попал в рамку
    else{// не попал в рамку
     select=OFF;
    };   // не попал в рамку

};// Vibor
//-------------------------------------------------------------------------
inline void Reper::clear_old(void)//очистка всех значений выбора
{oldx=oldy=0;};//
//-------------------------------------------------------------------------
inline void Reper::set_old(void)  //восстановление прежних параметров при масштабировании
{x=oldx;y=oldy;};//
//-------------------------------------------------------------------------
