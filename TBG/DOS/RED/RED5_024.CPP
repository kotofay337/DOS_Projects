// red5_024.cpp

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"

//-------------------------------------------------------------------------
Rectangle::Rectangle(void){//конструктор
  x=mmx(xkom_strt);
  y=mmy(ykom_strt);
  dx=mmx(xkom_end)-mmx(xkom_strt);
  dy=mmy(ykom_end)-mmy(ykom_strt);
/*
  dx=mmx(xkom_end-xkom_strt);
  dy=mmy(ykom_end-ykom_strt);
*/
  col=pck.col_lt;
  stl=pck.stl_l;
  thi=pck.thi_l;
};// Rectangle

inline void Rectangle::put_x(int p){x=p;};
inline void Rectangle::put_y(int p){y=p;};
inline void Rectangle::put_dx(int p){dx=p;};
inline void Rectangle::put_dy(int p){dy=p;};
inline void Rectangle::put_stl(int p){stl=p;};
inline void Rectangle::put_thi(int p){thi=p;};
inline void Rectangle::put_col(int p){col=p;};

inline int  Rectangle::get_x(void){return x;};
inline int  Rectangle::get_y(void){return y;};
inline int  Rectangle::get_dx(void){return dx;};
inline int  Rectangle::get_dy(void){return dy;};
inline char Rectangle::get_stl(void){return stl;};
inline char Rectangle::get_thi(void){return thi;};
inline char Rectangle::get_col(void){return col;};


void Rectangle::Show(void)
     {
      if(stl == GABARIT_LINE) /* прямоугольник невидимого габарита */
         return;
      setcolor(col);
      setlinestyle(stl,0,thi);

      if((otsek(x,y,x+dx,y+dy)==Ok)||(otsek(x,y+dy,x+dx,y)==Ok))
      rectangle(DMx((long)x),         DMy((long)y),
                DMx((long)x+(long)dx),DMy((long)y+(long)dy));
     };// Show

inline char Rectangle::get_tip(void){return TIP_rect;};

inline void Rectangle::save(FILE *inf)//сохранение в файле
{

  fwrite(&stl,11,1,inf);
/*
  fwrite(&stl,1,1,inf);
  fwrite(&thi,1,1,inf);
  fwrite(&col,1,1,inf);
  fwrite(&x,  1,2,inf);
  fwrite(&y,  1,2,inf);
  fwrite(&dx, 1,2,inf);
  fwrite(&dy, 1,2,inf);
*/
}//save

inline void Rectangle::load(FILE *ouf)//загрузка из файла
{
  fread(&stl,11,1,ouf);
/*
  fread(&stl,1,1,ouf);
  fread(&thi,1,1,ouf);
  fread(&col,1,1,ouf);
  fread(&x,  1,2,ouf);
  fread(&y,  1,2,ouf);
  fread(&dx, 1,2,ouf);
  fread(&dy, 1,2,ouf);
*/
}//load

     //признак 'попал/не попал' элемент в рамку
inline char Rectangle::GetSelect(void){return select;};

//-------------------------------------------------------------------------
void Rectangle::Vibor(void)//выбор элемента попавшего в рамку
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
//   printf("NO\n");
//   printf("-----------------------------------------\r");
    };   // не попал в рамку

};// Vibor
//-------------------------------------------------------------------------
inline void Rectangle::clear_old(void)//очистка всех значений выбора
{oldx=oldy=olddx=olddy=select=0;};//
//{oldx=oldy=olddx=olddy=rx=ry=select=0;};//
//-------------------------------------------------------------------------
inline void Rectangle::set_old(void)  //восстановление прежних параметров при масштабировании
{x=oldx;y=oldy;dx=olddx;dy=olddy;};//
//-------------------------------------------------------------------------
inline int Rectangle::get_oldx(void) {return oldx;};//
//-------------------------------------------------------------------------
inline int Rectangle::get_oldy(void) {return oldy;};//
//-------------------------------------------------------------------------
inline int Rectangle::get_olddx(void){return olddx;};//
//-------------------------------------------------------------------------
inline int Rectangle::get_olddy(void){return olddy;};//
//-------------------------------------------------------------------------
//int Rectangle::get_rx(void){return rx;};//
//-------------------------------------------------------------------------
//int Rectangle::get_ry(void){return ry;};//
//-------------------------------------------------------------------------
