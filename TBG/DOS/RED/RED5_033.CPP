// red5_033.cpp

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
Macro::Macro(void){//конструктор
  index = pck.mc_index;
  mcx   = mmx(xkom_strt);
  mcy   = mmy(ykom_strt);
  mode  = 0;
  cs    = pck.mc_state;
  cl    = NULL;
};// Macro

inline void Macro::put_x(int p){mcx=p;/* printf("put mcx=%d\n",mcx);*/};
inline void Macro::put_y(int p){mcy=p;/* printf("put mcy=%d\n",mcy);*/};
inline void Macro::put_index(unsigned int i){index=i;/*printf("put index=%d\n",index);*/};
inline void Macro::put_state(unsigned int s){cs=s;/*printf("put state=%d\n",cs);*/};
inline void Macro::put_class(MPClass *p){cl = p;/*printf("put class %p\n", cl);*/};

inline void Macro::put_oldstate(unsigned int s){olds=s;/*printf("put oldstate=%d\n",olds);*/};
inline unsigned int Macro::get_oldstate(void){/*printf("get oldstate=%d\n", olds);*/return olds;};

inline int  Macro::get_x(void){/*printf("get mcx=%d\n",mcx);*/return mcx;};
inline int  Macro::get_y(void){/*printf("get mcy=%d\n",mcy);*/return mcy;};
inline int  Macro::get_oldx(void){/*printf("get oldx=%d\n", oldx);*/return oldx;};
inline int  Macro::get_oldy(void){/*printf("get oldy=%d\n", oldy);*/return oldy;};
inline unsigned int Macro::get_index(void){/*printf("get index=%d\n", index);*/return index;};
inline unsigned int Macro::get_state(void){/*printf("get state=%d\n", cs);*/return cs;};
inline MPClass *Macro::get_class(void){/*printf("get class %p\n", cl);*/return cl;};

void Macro::Show(void){
   if(otsek(mcx, mcy, mcx, mcy) == Ok){
      if(cl == NULL){
         hide_tbl();
         cl = MPCLoad(index);
         restore_tbl();
      }/* if */
      MPCDraw(cl, mcx, mcy, cs, NULL, NULL);
	}/* if */
};// Show

inline char Macro::get_tip(void){/*printf("\n");*/return TIP_esc;};

void Macro::save(FILE *inf)//сохранение в файле
{
//  printf("save %d %d %d %d %d\n", index, mcx, mcy, mode, cs);
  fwrite(&index,1,10,inf);
/*
  fwrite(&index,1,2,inf);
  fwrite(&mcx,  1,2,inf);
  fwrite(&mcy,  1,2,inf);
  fwrite(&mode, 1,2,inf);
  fwrite(&cs  , 1,2,inf);
*/
}//save

void Macro::load(FILE *outf)//загрузка из файла
{
  fread(&index,1,10,outf);
/*
  fread(&index,1,2,outf);
  fread(&mcx,  1,2,outf);
  fread(&mcy,  1,2,outf);
  fread(&mode, 1,2,outf);
  fread(&cs  , 1,2,outf);
*/
  // printf("***load %d %d %d %d %d ", index, mcx, mcy, mode, cs);
  hide_tbl();
  cl = MPCLoad(index);
  restore_tbl();
  // printf("***load class:%p\n", cl);
}//load

     //признак 'попал/не попал' элемент в рамку
inline char Macro::GetSelect(void){/*printf("getselect %d\n", select);*/return select;};

//-------------------------------------------------------------------------
void Macro::Vibor(void)//выбор элемента попавшего в рамку
{
 int xmin,xmax,ymin,ymax;

// printf("Vibor\n");
 //вычисление размера элемента
 xmin=mcx;ymin=mcy;
 xmax=mcx;ymax=mcy;
 //вычисление размера элемента

 // условие попадания
  if((xmin>=rx_min)&&(ymin>=ry_min)&&
     (xmax<=rx_max)&&(ymax<=ry_max))
    {// попал в рамку
     ind_select=ON;
     count_select++;
     select=ON;
     oldx=mcx;
     oldy=mcy;
//     rx=x-rx_min;
//     ry=y-ry_min;
    }// попал в рамку
    else{// не попал в рамку
     select=OFF;
    };   // не попал в рамку

};// Vibor
//-------------------------------------------------------------------------
inline void Macro::clear_old(void)//очистка всех значений выбора
{oldx=oldy=0;};//
//{oldx=oldy=olddx=olddy=rx=ry=select=0;};//
//-------------------------------------------------------------------------
inline void Macro::set_old(void)  //восстановление прежних параметров при масштабировании
{mcx=oldx;mcy=oldy;};//
//-------------------------------------------------------------------------
