// red5_027.cpp

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <conio.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"

//-------------------------------------------------------------------------
Text::Text(void){//конструктор
  x=mmx(xkom_strt);
  y=mmy(ykom_strt);
  col=pck.col_lt;
  fnt=pck.fnt_t;
  dir=pck.dir_t;
  siz=pck.siz_t;
  hor=pck.hor_t;
  ver=pck.ver_t;
  sda=win.size_xya;// размер окна при создании текста
  mltx=Multx;
  mlty=Multy;
  max=strlen(text);
  txt=(char *)new char* [max+1];
  sprintf(txt,"%s",text);

  tip_txt=Ttx;//подтип текста

}// Text

inline void  Text::ClearText(void){delete txt;max=0;};

// Установка масштаба окна по тексту
// Текст: в центр окна
void  Text::Set_kf_size(void){

 if(sda<50) sda=SIZE_XY;//для старых типов элементов,где не было окон
 win.size_xya=sda;
 kf_size=win.size_xya/639;
 if(kf_size==51) win.size_xya=SIZE_XY;

 int txca=x;
 int tyca=y;

 if(((long)txca+(long)win.size_xya/2L) >= 32760L ) txca=SIZE_XY-win.size_xya/2;
 if((txca-win.size_xya/2) < 0                    ) txca=win.size_xya/2;

 if(((long)tyca+(long)win.size_xya/2L) >= 32760L ) tyca=SIZE_XY-win.size_xya/2;
 if((tyca-win.size_xya/2) < 0                    ) tyca=win.size_xya/2;

 win.xa_min=txca-win.size_xya/2;
 win.ya_min=tyca-win.size_xya/2;

}// Set_kf_size

void  Text::style(void)
{
 double mw=1.0;
 int mxn,myn;

      switch(tip_txt)
      {
       case 0:
        settextjustify(0,0);
        settextstyle(fnt,dir,siz);
       break;
       case 1:
        settextjustify(hor,ver);
        if(fnt == 0) settextstyle(0,dir,siz);
           else{
             settextstyle(fnt,dir,0);
	     mw=(double)sda/(double)win.size_xya;

//             mxn=(int)((double)multx[siz]*mw);
//             myn=(int)((double)multy[siz]*mw);

	     mxn=(int)((double)mltx*mw);
	     myn=(int)((double)mlty*mw);

	     if(mxn > MaxX) mxn=MaxX;
	     if(myn > MaxY) myn=MaxY;
//             if(mxn == 0 )  mxn=1;
//             if(myn == 0 )  myn=1;

	     setusercharsize(mxn,10,myn,10);
//             setusercharsize(multx[siz],divx[siz],multy[siz],divy[siz]);
	}//if
       break;
      }//switch

} // style

inline void  Text::put_mltx(int p){mltx=p;}
inline void  Text::put_mlty(int p){mlty=p;}
inline void  Text::put_sda (int p){ sda=p;}
inline void  Text::put_hor (int p){ hor=p;}
inline void  Text::put_ver (int p){ ver=p;}


inline void  Text::put_x(int p){x=p;}
inline void  Text::put_y(int p){y=p;}
inline void  Text::put_stl(int p){fnt=p;}
inline void  Text::put_col(int p){col=p;}
inline void  Text::put_dir(int p){dir=p;}
inline void  Text::put_siz(int p){siz=p;}

void  Text::put_txt(char *p)
{delete txt;
 max=strlen(p);
 txt=(char *)new char* [max+1];
 sprintf(txt,"%s",p);
}// put_txt

inline int   Text::get_oldmltx(void){return oldmltx;}
inline int   Text::get_oldmlty(void){return oldmlty;}

inline int   Text::get_mltx(void){return mltx;}
inline int   Text::get_mlty(void){return mlty;}
inline int   Text::get_sda (void){return sda; }
inline int   Text::get_hor (void){return hor; }
inline int   Text::get_ver (void){return ver; }



inline int   Text::get_x(void){return x;};
inline int   Text::get_y(void){return y;};
inline char  Text::get_stl(void){return fnt;};
inline char  Text::get_col(void){return col;};
inline char  Text::get_dir(void){return dir;};
inline char  Text::get_siz(void){return siz;};
inline char* Text::get_txt(void){return txt;};

void Text::Show(void)
{

 if(otsek(x,y,x,y)==Ok){
    style();
    setcolor(col);
//    outtextxy(DMx(long(x)),DMy(long(y)),txt);
    OutTextCol(DMx(long(x)),DMy(long(y)),txt);
 }//if
}// Show

inline char Text::get_tip(void){return TIP_txt;};

void Text::save(FILE *inf)//сохранение в файле
{
  fwrite(&fnt,15,1,inf);
//  fwrite(&fnt,9,1,inf);
/*
  fwrite(&fnt,1,1,inf);
  fwrite(&dir,1,1,inf);
  fwrite(&siz,1,1,inf);
  fwrite(&col,1,1,inf);
  fwrite(&x,  1,2,inf);
  fwrite(&y,  1,2,inf);
  fwrite(&max,1,1,inf);
*/
  fwrite(txt, 1,max,inf);

//  unsigned char t=max-1;
//  fwrite(&t,1,1,inf);
//  fwrite(txt,1,t,inf);
//  printf("len=%d max=%d  t=%d  %s      \r",strlen(txt),max,t,txt);
//  press_key();


}//save

void Text::load(FILE *ouf)//загрузка из файла
{
  // подтипы элементов
  switch(tip_file) // тип файла
  {
   case 2:
   case 3:
   case 4:tip_txt=0;
   break;
   case 5:tip_txt=1;
   break;
  }//switch
  // подтипы элементов

  switch(tip_txt)
  {
   case 0:fread(&fnt,9,1,ouf);
          hor=0;
          ver=0;
   break;
   case 1:fread(&fnt,15,1,ouf);
   break;
  }//switch

//  if(sda < 0) printf("sda=%d \n",sda);

/*
  fread(&fnt,1,1,ouf);
  fread(&dir,1,1,ouf);
  fread(&siz,1,1,ouf);
  fread(&col,1,1,ouf);
  fread(&x,  1,2,ouf);
  fread(&y,  1,2,ouf);
  fread(&max,1,1,ouf);
*/
  delete txt;//память от старой строки

//  для формата где 0 не хранится или не хранится
  txt=(char *)new char* [max+1];//память для вводимой
  fread(txt, 1,max,ouf);//сама строка
  if((max-1)!=strlen(txt)) txt[max]=0;//добавить 0 при его отсутствии

}//load

     //признак 'попал/не попал' элемент в рамку
inline char Text::GetSelect(void){return select;};

//-------------------------------------------------------------------------
void Text::Vibor(void)//выбор элемента попавшего в рамку
{
 int x_txt,y_txt,xmin,xmax,ymin,ymax;

//========== вычисление размера элемента
   style();

//   x_txt=textwidth (txt)/2;
//   y_txt=textheight(txt)/2;

   x_txt=TextWidthCol (txt)/2;
   y_txt=TextHeightCol(txt)/2;

   int dx=x_txt;// горизонтально
   int dy=y_txt;// горизонтально

   if(dir == VERT_DIR) // вертикально
   {
     dx=y_txt;
     dy=x_txt;
   }// if

   switch(ver)
   {
    case BOTTOM_TEXT:
         ymin=mmy(DMy(y)-dy);
    break;
    case CENTER_TEXT:
         ymin=mmy(DMy(y));
    break;
    case TOP_TEXT:
         ymin=mmy(DMy(y)+dy);
    break;
   }///switch

   switch(hor)
   {
    case LEFT_TEXT:
         xmin=mmx(DMx(x)+dx);
         // вертикальный default
         if((dir == VERT_DIR)&&(fnt == DEFAULT_FONT)) xmin=mmx(DMx(x)-dx);
    break;
    case CENTER_TEXT:
         xmin=mmx(DMx(x));
    break;
    case RIGHT_TEXT:
         xmin=mmx(DMx(x)-dx);
    break;
   }///switch

   xmax=xmin;
   ymax=ymin;
//========== вычисление размера элемента

//   printf("%d %d %d %d   \r",xmin,xmax,ymin,ymax);

// начало текста
//setcolor(EGA_LIGHTGREEN);
//rectangle(DMx(x),DMy(y),DMx(x)+1,DMy(y)+1);

// точка выбора
//setcolor(EGA_WHITE);
//rectangle(DMx(xmin),DMy(ymin),DMx(xmax),DMy(ymax));

//rectangle(mmy(DMy(y)),DMy(ymin),DMx(xmax),DMy(ymax));

 // условие попадания
  if((xmin>=rx_min)&&(ymin>=ry_min)&&
     (xmax<=rx_max)&&(ymax<=ry_max)
    ){// попал в рамку
     ind_select=ON;
     count_select++;
     select=ON;
     oldx=x;
     oldy=y;
     oldmltx=mltx;
     oldmlty=mlty;
//     printf("%d  \n",BaseGraphElement::BGE_Get_Max_LE());// максимальное число логических элементов
    }// попал в рамку
    else{// не попал в рамку
     select=OFF;
    };   // не попал в рамку

}// Vibor
//-------------------------------------------------------------------------
inline void Text::clear_old(void)//очистка всех значений выбора
{oldx=oldy=select=0;};//
//{oldx=oldy=rx=ry=select=0;};//
//-------------------------------------------------------------------------
inline void Text::set_old(void)  //восстановление прежних параметров при масштабировании
{x=oldx;y=oldy;mltx=oldmltx;mlty=oldmlty;}//
//-------------------------------------------------------------------------
inline int Text::get_oldx(void) {return oldx;};//
//-------------------------------------------------------------------------
inline int Text::get_oldy(void) {return oldy;};//
//-------------------------------------------------------------------------
