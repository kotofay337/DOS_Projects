// red5_029.cpp

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"


//-------------------------------------------------------------------------
Background::Background(void){//конструктор
  col=pck.col_f;
};// Background

inline int  Background::get_x(void){return 16380;};
inline int  Background::get_y(void){return 16380;};

inline void Background::put_col(int p){col=p;};

inline void Background::Show(void)
     {
      setbkcolor(col);
     };// Show
/*
void Background::Vibor(void)
     {
      ind_select=ON;
      count_select++;
      select=ON;
     }// Vibor
*/
inline char Background::get_tip(void){return TIP_bkgr;};

inline void Background::save(FILE *inf)//сохранение в файле
{
  fwrite(&col,1,1,inf);
};//save

inline void Background::load(FILE *ouf)//загрузка из файла
{
  fread(&col,1,1,ouf);
};//load
//-------------------------------------------------------------------------

