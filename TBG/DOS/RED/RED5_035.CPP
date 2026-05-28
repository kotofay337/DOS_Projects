// red5_035.cpp

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"

//-------------------------------------------------------------------------
FEllipse::FEllipse(void){//конструктор
  x=mmx(xkom_strt);
  y=mmy(ykom_strt);
  dx=abs(mmx(xkom_end)-mmx(xkom_strt));
  dy=abs(mmy(ykom_end)-mmy(ykom_strt));

  fcol=pck.col_f;
  fstl=pck.stl_f;

  col=pck.col_f;
  stl=SOLID_LINE;

  thi = 1;
  fi1 = 0;
  fi2 = 360;
};// FEllipse

inline void FEllipse::put_x(int p){x=p;};
inline void FEllipse::put_y(int p){y=p;};
inline void FEllipse::put_dx(int p){dx=p;};
inline void FEllipse::put_dy(int p){dy=p;};
inline void FEllipse::put_fi1(int p){fi1=p;};
inline void FEllipse::put_fi2(int p){fi2=p;};
inline void FEllipse::put_stl(int p){stl=p;};
inline void FEllipse::put_thi(int p){thi=p;};
inline void FEllipse::put_col(int p){col=p;};
inline void FEllipse::put_fstl(int p){fstl=p;};
inline void FEllipse::put_fcol(int p){fcol=p;};

inline int  FEllipse::get_x(void){return x;};
inline int  FEllipse::get_y(void){return y;};
inline int  FEllipse::get_dx(void){return dx;};
inline int  FEllipse::get_dy(void){return dy;};
inline int  FEllipse::get_fi1(void){return fi1;};
inline int  FEllipse::get_fi2(void){return fi2;};
inline char FEllipse::get_stl(void){return stl;};
inline char FEllipse::get_thi(void){return thi;};
inline char FEllipse::get_col(void){return col;};
inline char FEllipse::get_fstl(void){return fstl;};
inline char FEllipse::get_fcol(void){return fcol;};

void FEllipse::Show(void)
     {
      setcolor(col);
      setlinestyle(stl,0,thi);
      setfillstyle(fstl, fcol);

   if((otsek(x-dx,y-dy,x+dx,y+dy)==Ok)||(otsek(x-dx,y+dy,x+dx,y-dy)==Ok))
      if(fi1 == 0 && fi2 == 360){
         fillellipse(DMx((long)x), DMy((long)y),
	         DMx((long)win.xa_min+(long)dx),DMy((long)win.ya_min+(long)dy));
		}/* if */
      else{
         sector(DMx((long)x), DMy((long)y),fi1,fi2,
	         DMx((long)win.xa_min+(long)dx),DMy((long)win.ya_min+(long)dy));
      }/* elif */
   };// Show

inline char FEllipse::get_tip(void){return TIP_felip;};

inline void FEllipse::save(FILE *inf)//сохранение в файле
{
  fwrite(&stl,17,1,inf);
}//save

inline void FEllipse::load(FILE *ouf)//загрузка из файла
{
  fread(&stl,17,1,ouf);
}//load

     //признак 'попал/не попал' элемент в рамку
inline char FEllipse::GetSelect(void){return select;};

//-------------------------------------------------------------------------
void FEllipse::Vibor(void)//выбор элемента попавшего в рамку
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
inline void FEllipse::clear_old(void)//очистка всех значений выбора
{oldx=oldy=olddx=olddy=select=0;};//
//{oldx=oldy=olddx=olddy=rx=ry=select=0;};//
//-------------------------------------------------------------------------
inline void FEllipse::set_old(void)  //восстановление прежних параметров при масштабировании
{x=oldx;y=oldy;dx=olddx;dy=olddy;};//
//-------------------------------------------------------------------------
inline int FEllipse::get_oldx(void) {return oldx;};//
//-------------------------------------------------------------------------
inline int FEllipse::get_oldy(void) {return oldy;};//
//-------------------------------------------------------------------------
inline int FEllipse::get_olddx(void){return olddx;};//
//-------------------------------------------------------------------------
inline int FEllipse::get_olddy(void){return olddy;};//
//-------------------------------------------------------------------------
//int FEllipse::get_rx(void){return rx;};//
//-------------------------------------------------------------------------
//int FEllipse::get_ry(void){return ry;};//
//-------------------------------------------------------------------------
