// red5_025.cpp

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"

//-------------------------------------------------------------------------
Bar::Bar(void){//конструктор
  x=mmx(xkom_strt);
  y=mmy(ykom_strt);
  dx=mmx(xkom_end)-mmx(xkom_strt);
  dy=mmy(ykom_end)-mmy(ykom_strt);
  col=pck.col_f;
  stl=pck.stl_f;
};// Bar

inline void Bar::put_x(int p){x=p;};
inline void Bar::put_y(int p){y=p;};
inline void Bar::put_dx(int p){dx=p;};
inline void Bar::put_dy(int p){dy=p;};
inline void Bar::put_stl(int p){stl=p;};
inline void Bar::put_col(int p){col=p;};

inline int  Bar::get_x(void){return x;};
inline int  Bar::get_y(void){return y;};
inline int  Bar::get_dx(void){return dx;};
inline int  Bar::get_dy(void){return dy;};
inline char Bar::get_stl(void){return stl;};
inline char Bar::get_col(void){return col;};


void Bar::Show(void)
     {
      setfillstyle(stl,col);

      if((otsek(x,y,x+dx,y+dy)==Ok)||(otsek(x,y+dy,x+dx,y)==Ok)){
      disable();
      bar(DMx((long)x),         DMy((long)y),
          DMx((long)x+(long)dx),DMy((long)y+(long)dy));
      enable();
      }// if
     };// Show

inline char Bar::get_tip(void){return TIP_bar;};

inline void Bar::save(FILE *inf)//сохранение в файле
{
  fwrite(&stl,10,1,inf);
/*
  fwrite(&stl,1,1,inf);
  fwrite(&col,1,1,inf);
  fwrite(&x,  1,2,inf);
  fwrite(&y,  1,2,inf);
  fwrite(&dx, 1,2,inf);
  fwrite(&dy, 1,2,inf);
*/
}//save

inline void Bar::load(FILE *ouf)//загрузка из файла
{
  fread(&stl,10,1,ouf);
/*
  fread(&stl,1,1,ouf);
  fread(&col,1,1,ouf);
  fread(&x,  1,2,ouf);
  fread(&y,  1,2,ouf);
  fread(&dx, 1,2,ouf);
  fread(&dy, 1,2,ouf);
*/
}//load

     //признак 'попал/не попал' элемент в рамку
inline char Bar::GetSelect(void){return select;};

//-------------------------------------------------------------------------
void Bar::Vibor(void)//выбор элемента попавшего в рамку
{
 int x1,y1,xmin,xmax,ymin,ymax;

 //вычисление размера элемента
 x1=x+dx;
 y1=y+dy;

 if(x<=x1){xmin=x; xmax=x1;}
   else   {xmin=x1;xmax=x;};
 if(y<=y1){ymin=y; ymax=y1;}
   else   {ymin=y1;ymax=y;};
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
inline void Bar::clear_old(void)//очистка всех значений выбора
{oldx=oldy=olddx=olddy=select=0;};//
//{oldx=oldy=olddx=olddy=rx=ry=select=0;};//
//-------------------------------------------------------------------------
inline void Bar::set_old(void)  //восстановление прежних параметров при масштабировании
{x=oldx;y=oldy;dx=olddx;dy=olddy;};//
//-------------------------------------------------------------------------
inline int Bar::get_oldx(void) {return oldx;};//
//-------------------------------------------------------------------------
inline int Bar::get_oldy(void) {return oldy;};//
//-------------------------------------------------------------------------
inline int Bar::get_olddx(void){return olddx;};//
//-------------------------------------------------------------------------
inline int Bar::get_olddy(void){return olddy;};//
//-------------------------------------------------------------------------
//int Bar::get_rx(void){return rx;};//
//-------------------------------------------------------------------------
//int Bar::get_ry(void){return ry;};//
//-------------------------------------------------------------------------
