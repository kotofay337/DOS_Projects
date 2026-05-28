// red5_028.cpp

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"

//-------------------------------------------------------------------------
Floodfill::Floodfill(void){//конструктор
  x=mmx(xkom_strt);
  y=mmy(ykom_strt);
  col=pck.col_f;
  stl=pck.stl_f;
  brd=pck.col_lt;
};// Floodfill

inline void Floodfill::put_x(int p){x=p;};
inline void Floodfill::put_y(int p){y=p;};
inline void Floodfill::put_stl(int p){stl=p;};
inline void Floodfill::put_col(int p){col=p;};
inline void Floodfill::put_brd(int p){brd=p;};

inline int  Floodfill::get_x(void){return x;};
inline int  Floodfill::get_y(void){return y;};
inline char Floodfill::get_stl(void){return stl;};
inline char Floodfill::get_col(void){return col;};
inline char Floodfill::get_brd(void){return brd;};

void Floodfill::Show(void)
     {
      setfillstyle(stl,col);
      if(otsek(x,y,x,y)==Ok)
      floodfill(DMx((long)x),DMy((long)y),brd);
     };// Show

inline char Floodfill::get_tip(void){return TIP_fld;};

inline void Floodfill::save(FILE *inf)//сохранение в файле
{
  fwrite(&stl,7,1,inf);
/*
  fwrite(&stl,1,1,inf);
  fwrite(&col,1,1,inf);
  fwrite(&brd,1,1,inf);
  fwrite(&x,  1,2,inf);
  fwrite(&y,  1,2,inf);
*/
}//save

inline void Floodfill::load(FILE *ouf)//загрузка из файла
{
  fread(&stl,7,1,ouf);
/*
  fread(&stl,1,1,ouf);
  fread(&col,1,1,ouf);
  fread(&brd,1,1,ouf);
  fread(&x,  1,2,ouf);
  fread(&y,  1,2,ouf);
*/
}//load

     //признак 'попал/не попал' элемент в рамку
inline char Floodfill::GetSelect(void){return select;};

//-------------------------------------------------------------------------
void Floodfill::Vibor(void)//выбор элемента попавшего в рамку
{

 int xmin,xmax,ymin,ymax;

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
inline void Floodfill::clear_old(void)//очистка всех значений выбора
{oldx=oldy=select=0;};//
//{oldx=oldy=rx=ry=select=0;};//
//-------------------------------------------------------------------------
inline void Floodfill::set_old(void)  //восстановление прежних параметров при масштабировании
{x=oldx;y=oldy;};//
//-------------------------------------------------------------------------
inline int Floodfill::get_oldx(void) {return oldx;};//
//-------------------------------------------------------------------------
inline int Floodfill::get_oldy(void) {return oldy;};//
//-------------------------------------------------------------------------
//int Floodfill::get_rx(void){return rx;};//
//-------------------------------------------------------------------------
//int Floodfill::get_ry(void){return ry;};//
//-------------------------------------------------------------------------
