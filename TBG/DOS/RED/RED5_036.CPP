// red5_036.cpp

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"

//-------------------------------------------------------------------------
Bitmap::Bitmap(void){//конструктор
  x=mmx(xkom_strt);
  y=mmy(ykom_strt);
  dx=abs(mmx(xkom_end)-mmx(xkom_strt));
  dy=abs(mmy(ykom_end)-mmy(ykom_strt));
};// Bitmap

inline void Bitmap::put_x(int p){x=p;};
inline void Bitmap::put_y(int p){y=p;};
inline void Bitmap::put_dx(int p){dx=p;};
inline void Bitmap::put_dy(int p){dy=p;};

inline int  Bitmap::get_x(void){return x;};
inline int  Bitmap::get_y(void){return y;};
inline int  Bitmap::get_dx(void){return dx;};
inline int  Bitmap::get_dy(void){return dy;};

void Bitmap::Show(void){
/*
      setcolor(col);
      setlinestyle(stl,0,thi);
      setfillstyle(fstl, fcol);
*/
};// Show

inline char Bitmap::get_tip(void){return TIP_bitmap;};

inline void Bitmap::save(FILE *inf)//сохранение в файле
{
  fwrite(&x,sizeof(int) * 4 + sizeof(FileName),1,inf);
}//save

inline void Bitmap::load(FILE *ouf)//загрузка из файла
{
  fread(&x,sizeof(int) * 4 + sizeof(FileName),1,ouf);
}//load

     //признак 'попал/не попал' элемент в рамку
inline char Bitmap::GetSelect(void){return select;};

//-------------------------------------------------------------------------
void Bitmap::Vibor(void)//выбор элемента попавшего в рамку
{
 int xmin,xmax,ymin,ymax;

 //вычисление размера элемента
 xmin=x-dx;ymin=y-dy;
 xmax=x+dx;ymax=y+dy;
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
     olddx=dx;
     olddy=dy;
//     rx=x-rx_min;
//     ry=y-ry_min;
    }// попал в рамку
    else{// не попал в рамку
     select=OFF;
    };   // не попал в рамку

};// Vibor
//-------------------------------------------------------------------------
inline void Bitmap::clear_old(void)//очистка всех значений выбора
{oldx=oldy=olddx=olddy=select=0;};//
//{oldx=oldy=olddx=olddy=rx=ry=select=0;};//
//-------------------------------------------------------------------------
inline void Bitmap::set_old(void)  //восстановление прежних параметров при масштабировании
{x=oldx;y=oldy;dx=olddx;dy=olddy;};//
//-------------------------------------------------------------------------
inline int Bitmap::get_oldx(void) {return oldx;};//
//-------------------------------------------------------------------------
inline int Bitmap::get_oldy(void) {return oldy;};//
//-------------------------------------------------------------------------
inline int Bitmap::get_olddx(void){return olddx;};//
//-------------------------------------------------------------------------
inline int Bitmap::get_olddy(void){return olddy;};//
//-------------------------------------------------------------------------
//int Bitmap::get_rx(void){return rx;};//
//-------------------------------------------------------------------------
//int Bitmap::get_ry(void){return ry;};//
//-------------------------------------------------------------------------
