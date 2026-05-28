//---------------------------------------------------------------------------
//
//  22.Nov.1993
// Чумак А.И.
//
//---------------------------------------------------------------------------
#define NEW_RAMKA

#ifdef NEW_RAMKA

#define THICK

#endif

#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <process.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <graphics.h>
#include <io.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>
#include <keyboard.h>
#include "red5.h"
//---------------------------------------------------------------------------
#ifdef NEW_RAMKA

 static int x_old,
	    y_old,
	    fm=0;

#ifdef  THICK

#define START_TIME 300

 static int      iCounter;
 static unsigned uPattern,
		 uScreen;

#endif //THICK

#endif //NEW_RAMKA

//---------------------------------------------------------------------------
int ris_ramka(){

   if(getkey_left>0){/* левая кнопка */
      if(komand==0){
	if(ente!=0){
	    ente=0;
	    // убрать рамку
	    pos_ms_pred=-1;
	    mouse_off();
	    ramka0();
	    mouse_on();

	    if(fm == 1)
	       mouse_on(),fm=0;

		/* очистка всего  */
	    ind_select=OFF;
	    count_select=-1;
	    LE_TOP_Ram=-1;
	    pic.ClearOld();
		/* очистка всего  */

	    //координаты рамки для сохранения .PCX
		    xmin_pcx=ymin_pcx=0;
		    xmax_pcx=MaxX;
		    ymax_pcx=MaxY;

		return 0;
	     }//if
      }//if
   }//if

   if(getkey_left>0){/* левая кнопка */
      if(komand==0){
	 ente+=1;
	 if(ente>32000)
	    ente=32000;
	 if(ente==1){
	    x_strt=x_end=x_mouse;
	    y_strt=y_end=y_mouse;
	    /* нарисовать рамку */
	    pos_ms_pred=-1;
	    mouse_off();
	    ramka0();
	    mouse_on();
	     }// if
      }// if
   }// if левая кнопка

   if(getkey_right>0){/* правая кнопка */
      if((komand==0)&&(ente!=0)){
	 ente+=1;
	 if(ente==2){
	    //тянули рамку
#ifdef THICK

	    if(iCounter >= START_TIME){
	       mouse_off();
	       setwritemode(1);
	       setcolor(EGA_WHITE);
	       setlinestyle(USERBIT_LINE,~uScreen,NORM_WIDTH);
	       rectangle(x_strt,y_strt,x_end,y_end);
	       setwritemode(0);
	       mouse_on();
	       x_end=x_mouse;
	       y_end=y_mouse;
	       iCounter=0;
	       uScreen=0xFFFF;
	    }//if

#endif //THICK
	    if(fm == 1)
	       mouse_on(),fm=0;

		    pos_ms_pred=-1;
		    /* вычисление размера рамки */

		    if(x_strt<=x_end){
	       rx_min=x_strt;
	       rx_max=x_end;
	    }//if
		    else{
	       rx_min=x_end;
	       rx_max=x_strt;
	    }//if else

		    if(y_strt<=y_end){
	       ry_min=y_strt;
	       ry_max=y_end;
	    }//if
		    else{
	       ry_min=y_end;
	       ry_max=y_strt;
	    }//if else

	    //координаты рамки для сохранения .PCX
		    xmin_pcx=rx_min;
		    xmax_pcx=rx_max;
		    ymin_pcx=ry_min;
		    ymax_pcx=ry_max;


	    //абсолютные координаты рамки
		    rx_min=mmx(rx_min);
		    rx_max=mmx(rx_max);
		    ry_min=mmy(ry_min);
		    ry_max=mmy(ry_max);

          /*  */
		    pic.Vibor();//попадание в рамку выясняем

		    return 0;
		 }//if тянули рамку
	  }// if

      if((komand==0)&&(pic.GetPN()>=0))
	 pp_indic();  // индикация

   }// if правая кнопка

   if((ente==1)&&(komand==0)){
      if(fm == 0){
         mouse_off();
         fm=1;
      }/* if */
      ramka1_mouse();/* двигать рамку  */
   };/* if */


   return(0);

}/* ris_ramka */
//---------------------------------------------------------------------------
int ramka0()
{

 setwritemode(1);
 setcolor(EGA_WHITE);

#ifdef THICK

 if(iCounter >= START_TIME){

    setlinestyle(USERBIT_LINE,uScreen,NORM_WIDTH);
    rectangle(x_strt,y_strt,x_end,y_end);
    setwritemode(0);
    x_end=x_mouse;
    y_end=y_mouse;
    iCounter=0;
    uScreen=0xFFFF;
    return 0;

 }//if
 else{

#endif //THICK

    setlinestyle(SOLID_LINE,0,NORM_WIDTH);
    rectangle(x_strt,y_strt,x_end,y_end);
    setwritemode(0);

#ifdef THICK

 }//if else

#endif //THICK

 return(0);

}/* ramka0 */
//---------------------------------------------------------------------------

#ifndef NEW_RAMKA

//---------------------------------------------------------------------------
int ramka1_mouse()
{

   setwritemode(1);
   setcolor(EGA_WHITE);
   setlinestyle(SOLID_LINE,0,NORM_WIDTH);
   rectangle(x_strt,y_strt,x_end,y_end);
   rectangle(x_strt,y_strt,x_mouse,y_mouse);
   setwritemode(0);
   x_end=x_mouse;
   y_end=y_mouse;
   return(0);

}/* ramka1_mouse */
//---------------------------------------------------------------------------

#else  // define NEW_RAMKA

//---------------------------------------------------------------------------
int ramka1_mouse()
{

   if((x_old != x_mouse)||
      (y_old != y_mouse)){//мышь ползет
      setwritemode(1);
      setcolor(EGA_WHITE);

#ifdef THICK

      if(iCounter >= START_TIME){

	 setlinestyle(USERBIT_LINE,uScreen,NORM_WIDTH);
	 rectangle(x_strt,y_strt,x_end,y_end);
	 setlinestyle(SOLID_LINE,0,NORM_WIDTH);
	 rectangle(x_strt,y_strt,x_end,y_end);
	 setwritemode(0);
	 iCounter=0;
	 uScreen=0xFFFF;
	 return 0;

      }//if

#endif //THICK

      setlinestyle(SOLID_LINE,0,NORM_WIDTH);
      rectangle(x_strt,y_strt,x_end,y_end);
      rectangle(x_strt,y_strt,x_mouse,y_mouse);
      setwritemode(0);

#ifdef THICK

      iCounter=0;
      uScreen=0xFFFF;

#endif //THICK

   }//if
   else{//мышь стоит

#ifdef THICK

      setcolor(EGA_WHITE);
      setwritemode(1);

      switch(iCounter){
	 case START_TIME :
	    uPattern=0x8787;
	    uScreen^=uPattern;
	    setlinestyle(USERBIT_LINE,uPattern,NORM_WIDTH);
	    rectangle(x_strt,y_strt,x_end,y_end);
	    uPattern=0x8888;
	    break;
	 case START_TIME+4:
	 case START_TIME+8:
	 case START_TIME+12:
	 case START_TIME+20:
	 case START_TIME+24:
	 case START_TIME+28:
	    uPattern>>=1;
	    uScreen^=uPattern;
	    setlinestyle(USERBIT_LINE,uPattern,NORM_WIDTH);
	    rectangle(x_strt,y_strt,x_end,y_end);
	    break;
	 case START_TIME+32:
	 case START_TIME+16:
	    uPattern=0x8888;
	    uScreen^=uPattern;
	    setlinestyle(USERBIT_LINE,uPattern,NORM_WIDTH);
	    rectangle(x_strt,y_strt,x_end,y_end);
	    break;
      }//switch

      iCounter++ > START_TIME+18 ? iCounter=START_TIME+1 : iCounter;

      setwritemode(0);


#endif //THICK

   }//if else

   x_old=x_end=x_mouse;
   y_old=y_end=y_mouse;

   return(0);

}/* ramka1_mouse */
//---------------------------------------------------------------------------

#endif // define NEW_RAMKA
