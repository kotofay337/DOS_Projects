// red5_023.cpp

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"

//-------------------------------------------------------------------------
Line::Line(void){//конструктор
  x=mmx(xkom_strt);
  y=mmy(ykom_strt);
  dx=mmx(xkom_end)-mmx(xkom_strt);
  dy=mmy(ykom_end)-mmy(ykom_strt);

//  dx=mmx(xkom_end-xkom_strt);
//  dy=mmy(ykom_end-ykom_strt);
  col=pck.col_lt;
  stl=pck.stl_l;
  thi=pck.thi_l;
};// Line
//-------------------------------------------------------------------------
void Line::Show(void)
     {

      setcolor(col);
      setlinestyle(stl,0,thi);

      if(otsek(x,y,x+dx,y+dy)==Ok)
	 line(DMx((long)x),         DMy((long)y),
	          DMx((long)x+(long)dx),DMy((long)y+(long)dy));
/*
      int x1=x;
      int y1=y;
      int x2=x+dx;
      int y2=y+dy;
      int xa_max=xa_min+size_xya;
      int ya_max=ya_min+size_xya;


      if(((x1 < xa_max)&&
          (x1 > xa_min)&&
          (y1 < ya_max)&&
          (y1 > ya_min)
         )||
         ((x2 < xa_max)&&
          (x2 > xa_min)&&
          (y2 < ya_max)&&
          (y2 > ya_min)
         )||
         ((x1 < xa_max)&&
          (x1 > xa_min)&&
          (y2 < ya_max)&&
          (y2 > ya_min)
         )||
         ((x2 < xa_max)&&
          (x2 > xa_min)&&
          (y1 < ya_max)&&
          (y1 > ya_min)
         )
        )
      line(DMx((long)x),         DMy((long)y),
	       DMx((long)x+(long)dx),DMy((long)y+(long)dy));

*/
     }// Show
//-------------------------------------------------------------------------
inline char Line::get_tip(void){return TIP_line;};
//-------------------------------------------------------------------------
inline void Line::save(FILE *inf)//сохранение в файле
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
//-------------------------------------------------------------------------
inline void Line::load(FILE *ouf)//загрузка из файла
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
};//load
//-------------------------------------------------------------------------
     //признак 'попал/не попал' элемент в рамку
inline char Line::GetSelect(void){return select;};

//-------------------------------------------------------------------------
void Line::Vibor(void)//выбор элемента попавшего в рамку
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
inline void Line::clear_old(void)//очистка всех значений выбора
{oldx=oldy=olddx=olddy=select=0;};//
//{oldx=oldy=olddx=olddy=rx=ry=select=0;};//
//-------------------------------------------------------------------------
inline void Line::set_old(void)  //восстановление прежних параметров при масштабировании
{x=oldx;y=oldy;dx=olddx;dy=olddy;};//
//-------------------------------------------------------------------------
inline void Line::put_x  (int p){x=p;};
inline void Line::put_y  (int p){y=p;};
inline void Line::put_dx (int p){dx=p;};
inline void Line::put_dy (int p){dy=p;};
inline void Line::put_stl(int p){stl=p;};
inline void Line::put_thi(int p){thi=p;};
inline void Line::put_col(int p){col=p;};

//-------------------------------------------------------------------------
inline int  Line::get_x  (void){return x;};
inline int  Line::get_y  (void){return y;};
inline int  Line::get_dx (void) {return dx;};
inline int  Line::get_dy (void) {return dy;};
inline char Line::get_stl(void){return stl;};
inline char Line::get_thi(void){return thi;};
inline char Line::get_col(void){return col;};

//-------------------------------------------------------------------------
int Line::get_oldx(void) {return oldx;};
int Line::get_oldy(void) {return oldy;};
int Line::get_olddx(void){return olddx;};
int Line::get_olddy(void){return olddy;};
//int Line::get_rx(void){return rx;};
//int Line::get_ry(void){return ry;};
//-------------------------------------------------------------------------
