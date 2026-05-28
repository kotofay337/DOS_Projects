// red5_037.cpp

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"

//-------------------------------------------------------------------------
PipeLine::PipeLine(void){//конструктор
  x=mmx(xkom_strt);
  y=mmy(ykom_strt);
  dx=mmx(xkom_end)-mmx(xkom_strt);
  dy=mmy(ykom_end)-mmy(ykom_strt);

  pl.C1=pck.col_lt;
  pl.C2=pck.col_f;

  pl.O  =  0;
  pl.DO =  1;
  pl.S  = 10;

  stl=pck.stl_l;
  thi=pck.thi_l;

  color = -1;

};// PipeLine
//-------------------------------------------------------------------------
void PipeLine::Show(void)
     {

      setlinestyle(stl,0,thi);

      if(otsek(x,y,x+dx,y+dy)==Ok)

	   pl.xs = DMx((long)x);
		pl.ys = DMy((long)y);
	   pl.xe = DMx((long)x+(long)dx);
		pl.ye = DMy((long)y+(long)dy);

      pipeline(&pl, color);

     }// Show
/* ----------------------------------------------------------------------- */
void PipeLine::SetDO(int DO){
   pl.DO += DO;
   if(pl.DO < 1){
      pl.DO = 1;
   }/* if */
}/* SetDO */
/* ----------------------------------------------------------------------- */
void PipeLine::SetS(int S){
   pl.S += S;
   if(pl.S < 1){
      pl.S = 1;
   }/* if */
}/* SetS */
/* ----------------------------------------------------------------------- */
#pragma argsused
void PipeLine::SetPipelineColor(int c){
   color = -1;
}/* SetPipelineColor */
//-------------------------------------------------------------------------
inline char PipeLine::get_tip(void){return TIP_pipeline;};
//-------------------------------------------------------------------------
inline void PipeLine::save(FILE *inf)//сохранение в файле
{
  pl.O = 0;
  fwrite(&stl,1,10 + sizeof(PIPELINE),inf);
}//save
//-------------------------------------------------------------------------
inline void PipeLine::load(FILE *ouf)//загрузка из файла
{
  fread(&stl,1,10 + sizeof(PIPELINE),ouf);
  pl.O = 0;
};//load
//-------------------------------------------------------------------------
     //признак 'попал/не попал' элемент в рамку
inline char PipeLine::GetSelect(void){return select;};
//-------------------------------------------------------------------------
void PipeLine::Vibor(void)//выбор элемента попавшего в рамку
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
inline void PipeLine::clear_old(void)//очистка всех значений выбора
{oldx=oldy=olddx=olddy=select=0;};//
//{oldx=oldy=olddx=olddy=rx=ry=select=0;};//
//-------------------------------------------------------------------------
inline void PipeLine::set_old(void)  //восстановление прежних параметров при масштабировании
{x=oldx;y=oldy;dx=olddx;dy=olddy;};//
//-------------------------------------------------------------------------
inline void PipeLine::put_x  (int p){x=p;};
inline void PipeLine::put_y  (int p){y=p;};
inline void PipeLine::put_dx (int p){dx=p;};
inline void PipeLine::put_dy (int p){dy=p;};
inline void PipeLine::put_stl(int p){stl=p;};
inline void PipeLine::put_thi(int p){thi=p;};
inline void PipeLine::put_col(int p){pl.C1=p;};
inline void PipeLine::put_col2(int p){pl.C2=p;};

//-------------------------------------------------------------------------
inline int  PipeLine::get_x  (void){return x;};
inline int  PipeLine::get_y  (void){return y;};
inline int  PipeLine::get_dx (void) {return dx;};
inline int  PipeLine::get_dy (void) {return dy;};
inline char PipeLine::get_stl(void){return stl;};
inline char PipeLine::get_thi(void){return thi;};
inline char PipeLine::get_col(void){return pl.C1;};
inline char PipeLine::get_col2(void){return pl.C2;};

//-------------------------------------------------------------------------
int PipeLine::get_oldx(void) {return oldx;};
int PipeLine::get_oldy(void) {return oldy;};
int PipeLine::get_olddx(void){return olddx;};
int PipeLine::get_olddy(void){return olddy;};
//int PipeLine::get_rx(void){return rx;};
//int PipeLine::get_ry(void){return ry;};
//-------------------------------------------------------------------------
