// red5_026.cpp

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"

//-------------------------------------------------------------------------
Ellipse::Ellipse(void){//конструктор
  x=mmx(xkom_strt);
  y=mmy(ykom_strt);
  dx=abs(mmx(xkom_end)-mmx(xkom_strt));
  dy=abs(mmy(ykom_end)-mmy(ykom_strt));

  col=pck.col_lt;
  stl=pck.stl_l;
  thi=pck.thi_l;
  fi1 =0;
  fi2 =360;
};// Ellipse

inline void Ellipse::put_x(int p){x=p;};
inline void Ellipse::put_y(int p){y=p;};
inline void Ellipse::put_dx(int p){dx=p;};
inline void Ellipse::put_dy(int p){dy=p;};
inline void Ellipse::put_fi1(int p){fi1=p;};
inline void Ellipse::put_fi2(int p){fi2=p;};
inline void Ellipse::put_stl(int p){stl=p;};
inline void Ellipse::put_thi(int p){thi=p;};
inline void Ellipse::put_col(int p){col=p;};

inline int  Ellipse::get_x(void){return x;};
inline int  Ellipse::get_y(void){return y;};
inline int  Ellipse::get_dx(void){return dx;};
inline int  Ellipse::get_dy(void){return dy;};
inline int  Ellipse::get_fi1(void){return fi1;};
inline int  Ellipse::get_fi2(void){return fi2;};
inline char Ellipse::get_stl(void){return stl;};
inline char Ellipse::get_thi(void){return thi;};
inline char Ellipse::get_col(void){return col;};

void Ellipse::Show(void)
     {
      setcolor(col);
      setlinestyle(stl,0,thi);

   if((otsek(x-dx,y-dy,x+dx,y+dy)==Ok)||(otsek(x-dx,y+dy,x+dx,y-dy)==Ok))
      ellipse(DMx((long)x), DMy((long)y),fi1,fi2,
	      DMx((long)win.xa_min+(long)dx),DMy((long)win.ya_min+(long)dy));
     };// Show

inline char Ellipse::get_tip(void){return TIP_elip;};

inline void Ellipse::save(FILE *inf)//сохранение в файле
{
  fwrite(&stl,15,1,inf);
/*
  fwrite(&stl,1,1,inf);
  fwrite(&thi,1,1,inf);
  fwrite(&col,1,1,inf);
  fwrite(&x,  1,2,inf);
  fwrite(&y,  1,2,inf);
  fwrite(&dx, 1,2,inf);
  fwrite(&dy, 1,2,inf);
  fwrite(&fi1,1,2,inf);
  fwrite(&fi2,1,2,inf);
*/
}//save

inline void Ellipse::load(FILE *ouf)//загрузка из файла
{
  fread(&stl,15,1,ouf);
/*
  fread(&stl,1,1,ouf);
  fread(&thi,1,1,ouf);
  fread(&col,1,1,ouf);
  fread(&x,  1,2,ouf);
  fread(&y,  1,2,ouf);
  fread(&dx, 1,2,ouf);
  fread(&dy, 1,2,ouf);
  fread(&fi1,1,2,ouf);
  fread(&fi2,1,2,ouf);
*/
}//load

     //признак 'попал/не попал' элемент в рамку
inline char Ellipse::GetSelect(void){return select;};

//-------------------------------------------------------------------------
void Ellipse::Vibor(void)//выбор элемента попавшего в рамку
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
inline void Ellipse::clear_old(void)//очистка всех значений выбора
{oldx=oldy=olddx=olddy=select=0;};//
//{oldx=oldy=olddx=olddy=rx=ry=select=0;};//
//-------------------------------------------------------------------------
inline void Ellipse::set_old(void)  //восстановление прежних параметров при масштабировании
{x=oldx;y=oldy;dx=olddx;dy=olddy;};//
//-------------------------------------------------------------------------
inline int Ellipse::get_oldx(void) {return oldx;};//
//-------------------------------------------------------------------------
inline int Ellipse::get_oldy(void) {return oldy;};//
//-------------------------------------------------------------------------
inline int Ellipse::get_olddx(void){return olddx;};//
//-------------------------------------------------------------------------
inline int Ellipse::get_olddy(void){return olddy;};//
//-------------------------------------------------------------------------
//int Ellipse::get_rx(void){return rx;};//
//-------------------------------------------------------------------------
//int Ellipse::get_ry(void){return ry;};//
//-------------------------------------------------------------------------
