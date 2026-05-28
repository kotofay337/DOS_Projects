//  red5_016.c

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <dos.h>
#include <dir.h>
#include <math.h>
#include <conio.h>
#include <alloc.h>

#include "red5.h"

static short col_lt = 0;
static short col_f  = 0;
/**************************************************************************/
// индикация выравнивания текста
void indic_vira_t(OBJECT **menu)
{
 short int xmin,ymin,xmax,ymax;

 outpic_obj(menu,10);
 getsize_obj_num_reset();
 getsize_obj_num(menu,10,1,&xmin,&ymin,&xmax,&ymax);

 settextjustify(pck.hor_t,pck.ver_t);
 settextstyle(0,0,1);
 setcolor(EGA_WHITE);
 outtextxy(xmin+(xmax-xmin)/2,ymin,"Justify");

}//  indic_vira_t
/**************************************************************************/
void init_stl_f(OBJECT **menu){
 int i, cf;
 char str[4];

 if(col_f > getmaxcolor())
    col_f = 0;

 cf = col_f;

 for(i = 0;i < 16;i++){
    setcolor_obj_num(menu, 33, i, cf, SHOW);
    sprintf(str, "%3.0u", cf);
    outtext_obj_num(menu, 34, i, cf, str, SHOW);
    cf++;
 }/* for */

 mouse_on();
}/* init_stl_f */
/**************************************************************************/
#pragma warn -par // неиспользуется menu
// индикация текущего стиля и цвета закраски в главном меню
void indic_stl_f(OBJECT **menu)
{
 int i;
 char str[4];
 short int xmin,ymin,xmax,ymax;


 getsize_obj_num_reset();
 getsize_obj_num(menu_1,4,1,&xmin,&ymin,&xmax,&ymax);
 setfillstyle(pck.stl_f, pck.col_f);
 mouse_off();
 bar(xmin,ymin,xmax,ymax);

 if(menu != menu_1){
    col_f += 16;
    init_stl_f(menu);
 }/* if */

 mouse_on();
}//  indic_stl_f
#pragma warn +par // неиспользуется menu

/**************************************************************************/
// индикация текущего цвета в меню стилей закраски
void indic_col_f(OBJECT **menu)
{
 short int xmin,ymin,xmax,ymax;

 pck.col_f += col_f;

 setcolor_obj(menu,3,pck.col_f,SHOW);

 getsize_obj_num_reset();
 getsize_obj_num(menu_1,4,1,&xmin,&ymin,&xmax,&ymax);
 setfillstyle(pck.stl_f,pck.col_f);
 mouse_off();
 bar(xmin,ymin,xmax,ymax);
 mouse_on();
}//  indic_col_f
/**************************************************************************/
void init_stl_lt(OBJECT **menu){
 int i, cl;
 char str[4];

 if(col_lt > getmaxcolor())
    col_lt = 0;
 cl = col_lt;
 for(i = 0;i < 16;i++){
    setcolor_obj_num(menu, 33, i, cl, SHOW);
    sprintf(str, "%3.0u", cl);
    outtext_obj_num(menu, 34, i, cl, str, SHOW);
    cl++;
 }/* for */

 mouse_on();
}/* init_stl_lt */
/**************************************************************************/
#pragma warn -par // неиспользуется menu
// индикация текущего стиля и цвета линии в главном меню
void indic_stl_l(OBJECT **menu)
{
  int i;
  char str[4];

  short int xmin,ymin,xmax,ymax;

  if((pck.stl_thi>=0)&&(pck.stl_thi<=3)) {
     pck.stl_l = pck.stl_thi;
	  pck.thi_l = NORM_WIDTH;
  }// if
  if((pck.stl_thi>=4)&&(pck.stl_thi<=7)) {
     pck.stl_l = pck.stl_thi - 4;
	  pck.thi_l = THICK_WIDTH;
  }// if

 getsize_obj_num_reset();
 getsize_obj_num(menu_1, 6, 1, &xmin, &ymin, &xmax, &ymax);

 mouse_off();

 outpic_obj_num(menu_1, 6, 255);/* предыдущие толщина,стиль,цвет линии удаляем */

 setlinestyle(pck.stl_l,0,pck.thi_l);

 setcolor(pck.col_lt);

 line(xmin,ymin,xmax,ymax);

 if(pck.dir_t==HORIZ_DIR)
    setcolor_obj_num(menu_1,7,2,pck.col_lt,SHOW);/* цвет текста */
 if(pck.dir_t== VERT_DIR)
    setcolor_obj_num(menu_1,7,1,pck.col_lt,SHOW);/* цвет текста */

 if(menu != menu_1){
    col_lt += 16;
    init_stl_lt(menu);
 }/* if */

 mouse_on();

}//  indic_stl_l
#pragma warn +par // неиспользуется menu
/**************************************************************************/
// индикация текущего цвета в меню стилей линии
void indic_col_lt(OBJECT **menu)
{
  short int xmin,ymin,xmax,ymax;

  pck.col_lt += col_lt;

  setcolor_obj(menu,3,pck.col_lt,SHOW);

  if((pck.stl_thi>=0)&&(pck.stl_thi<=3)) {
     pck.stl_l = pck.stl_thi;
	  pck.thi_l = NORM_WIDTH;
  }// if
  if((pck.stl_thi>=4)&&(pck.stl_thi<=7)) {
     pck.stl_l = pck.stl_thi - 4;
	  pck.thi_l = THICK_WIDTH;
  }// if

 getsize_obj_num_reset();
 getsize_obj_num(menu_1, 6, 1, &xmin, &ymin, &xmax, &ymax);

 mouse_off();

 outpic_obj_num(menu_1, 6, 255);/* предыдущие толщина,стиль,цвет линии удаляем */

 setlinestyle(pck.stl_l,0,pck.thi_l);

 setcolor(pck.col_lt);

 line(xmin,ymin,xmax,ymax);

 if(pck.dir_t==HORIZ_DIR)
    setcolor_obj_num(menu_1,7,2,pck.col_lt,SHOW);/* цвет текста */
 if(pck.dir_t== VERT_DIR)
    setcolor_obj_num(menu_1,7,1,pck.col_lt,SHOW);/* цвет текста */

 mouse_on();

}//  indic_col_lt
/**************************************************************************/
int pp_pos13() /* линия,прямоугольник,закрашенный прямоугольник */
{

 int tip;
 PIPELINE pl;

 if((getkey_left>0)&&(ente1==0))
	 {
	  ente1+=1;
	  xkom_strt=xkom_end=x_mouse;ykom_strt=ykom_end=y_mouse;
	   /* нарисовать линию */
	   mouse_off();
	   ybcur();
	   setwritemode(1);
	   if(komand==KOM_line){
              line(xkom_strt,ykom_strt,xkom_end,ykom_end);
      }
	      else{
	         if((komand == KOM_bar) || (komand == KOM_rect)){
				   setlinestyle(SOLID_LINE,0,1);
		         rectangle(xkom_strt,ykom_strt,xkom_end,ykom_end);
            }/* if */

	         if(komand == KOM_pipeline){
               pl.xs = xkom_strt;
               pl.ys = ykom_strt;
               pl.xe = xkom_end;
               pl.ye = ykom_end;
               pl.C1 = pck.col_lt;
               pl.C2 = pck.col_f;
               pl.O  =  0;
               pl.S  = 10;
               pl.DO =  0;
//               setlinestyle(SOLID_LINE,0,1);
               pipeline(&pl, -1);
            }/* if */
	      };

	   setwritemode(0);
	   return(0);
	 };
       if((getkey_left>0)&&(ente1!=0))
	 {ente1=0;
	   /* стереть линию */
	   setwritemode(1);
	   if(komand==KOM_line) line(xkom_strt,ykom_strt,xkom_end,ykom_end);
	      else{
		    if((komand==KOM_bar) || (komand == KOM_rect)){
			    setlinestyle(SOLID_LINE,0,1);
		       rectangle(xkom_strt,ykom_strt,xkom_end,ykom_end);
          }/* if */
	       if(komand == KOM_pipeline){
             pl.xs = xkom_strt;
             pl.ys = ykom_strt;
             pl.xe = xkom_end;
             pl.ye = ykom_end;
             pl.C1 = pck.col_lt;
             pl.C2 = pck.col_f;
             pl.O  =  0;
             pl.S  = 10;
             pl.DO =  0;
//             setlinestyle(SOLID_LINE,0,1);
			    pipeline(&pl, -1);
          }/* if */
		  };
	   setwritemode(0);
           ybcur();
	   mouse_on();
	   return(0);
	 };
       if(ente1!=0)
	 { /* рисовать линию */
          // лекарство от моргания
          if((xkom_end != x_mouse)||(ykom_end != y_mouse)){
	   setwritemode(1);
	   if(komand==KOM_line) {line(xkom_strt,ykom_strt,xkom_end,ykom_end);
				 line(xkom_strt,ykom_strt,x_mouse,y_mouse);}
	     else{
		    if((komand==KOM_bar) || (komand == KOM_rect)){
			    setlinestyle(SOLID_LINE,0,1);
		       rectangle(xkom_strt,ykom_strt,xkom_end,ykom_end);
		       rectangle(xkom_strt,ykom_strt,x_mouse,y_mouse);
          }/* if */
	       if(komand == KOM_pipeline){
             pl.xs = xkom_strt;
             pl.ys = ykom_strt;
             pl.xe = xkom_end;
             pl.ye = ykom_end;
             pl.C1 = pck.col_lt;
             pl.C2 = pck.col_f;
             pl.O  =  0;
             pl.S  = 10;
             pl.DO =  0;
//             setlinestyle(SOLID_LINE,0,1);
			    pipeline(&pl, -1);
             pl.xs = xkom_strt;
             pl.ys = ykom_strt;
             pl.xe = x_mouse;
             pl.ye = y_mouse;
			    pipeline(&pl, -1);
          }/* if */
		  };
	   xkom_end=x_mouse;ykom_end=y_mouse;
	   setwritemode(0);
         }// if
	 };
       /* оставить нарисованную линию */
       if((getkey_right>0)&&(ente1!=0))
	 {ente1=0;
	   /* стереть линию */
	   setwritemode(1);
	   if(komand==KOM_line) line(xkom_strt,ykom_strt,xkom_end,ykom_end);
	      else{ /*zpsst();*/
		    if((komand==KOM_bar) || (komand == KOM_rect)){
			    setlinestyle(SOLID_LINE,0,1);
		       rectangle(xkom_strt,ykom_strt,xkom_end,ykom_end);
          }/* if */
		  };
	   setwritemode(0);
	   if(komand==KOM_line) tip=TIP_line;
	   if(komand == KOM_pipeline){
		   tip=TIP_pipeline;
      }/* if */
	   if(komand==KOM_rect) tip=TIP_rect;
	   if(komand==KOM_bar ) tip=TIP_bar;
      if((peek(0,0x417) & 0x04) != 0){ // Ctrl+RBTN-MOUSE
         pck.stl_l = GABARIT_LINE;
      }/* if */
	   pic.CreateGE(tip);
//	   if(pic.CreateGE(tip) <0) return -1;// создание GE заданного типа
       pic.ShowCur();   // рисование созданного обьекта
	   ybcur();
	   mouse_on();
	   return(-1);
	 };
	 return 0;
}/* pp_pos13 */
/****************************************************************************/
int pp_pos16() /* эллипс */
{
   setlinestyle(SOLID_LINE,0,1);
   if((getkey_left>0)&&(ente1==0))
	 {ente1+=1;
	  xkom_strt=xkom_end=x_mouse;ykom_strt=ykom_end=y_mouse;
	   /* нарисовать эллипс */
	   mouse_off();
	   ybcur();
	   set_mode_xor();
	   ellipse(xkom_strt,ykom_strt,0,360,
		   abs(xkom_strt-xkom_end),abs(ykom_strt-ykom_end));
	   return(0);
	 };/* if */
  if((getkey_left>0)&&(ente1!=0))
	 {ente1=0;
	   /* стереть эллипс */
	   set_mode_xor();
	   ellipse(xkom_strt,ykom_strt,0,360,
		   abs(xkom_strt-xkom_end),abs(ykom_strt-ykom_end));
	   ybcur();
	   mouse_on();
	   return(0);
	 };/* if */
   if(ente1!=0)
	 { /* рисовать эллипс */
          if((xkom_end != x_mouse)||(ykom_end != y_mouse)){
	   set_mode_xor();
	   ellipse(xkom_strt,ykom_strt,0,360,
		   abs(xkom_strt-xkom_end),abs(ykom_strt-ykom_end));
           set_mode_xor();
	   ellipse(xkom_strt,ykom_strt,0,360,
		   abs(xkom_strt-x_mouse),abs(ykom_strt-y_mouse));
	   xkom_end=x_mouse;ykom_end=y_mouse;
           }// if
	 };/*  */
       /* оставить нарисованный эллипс */
   if((getkey_right>0)&&(ente1!=0))
	 {ente1=0;
	   /* стереть эллипс */
       set_mode_xor();
	   ellipse(xkom_strt,ykom_strt,0,360,
		   abs(xkom_strt-xkom_end),abs(ykom_strt-ykom_end));
	   pic.CreateGE(OBJ_elip);// создание GE заданного типа
//	   if(pic.CreateGE(OBJ_elip) <0) return -1;// создание GE заданного типа
       pic.ShowCur();   // рисование созданного обьекта
	   ybcur();
	   mouse_on();
	   return(-1);
	 };/* if */
   return(0);
}/* pp_pos16 */
/****************************************************************************/
int pp_pos161() /* эллипс */
{
   setlinestyle(SOLID_LINE,0,1);
   if((getkey_left>0)&&(ente1==0))
	 {ente1+=1;
	  xkom_strt=xkom_end=x_mouse;ykom_strt=ykom_end=y_mouse;
	   /* нарисовать эллипс */
	   mouse_off();
	   ybcur();
	   set_mode_xor();
	   ellipse(xkom_strt,ykom_strt,0,360,
		   abs(xkom_strt-xkom_end),abs(ykom_strt-ykom_end));
	   return(0);
	 };/* if */
  if((getkey_left>0)&&(ente1!=0))
	 {ente1=0;
	   /* стереть эллипс */
	   set_mode_xor();
	   ellipse(xkom_strt,ykom_strt,0,360,
		   abs(xkom_strt-xkom_end),abs(ykom_strt-ykom_end));
	   ybcur();
	   mouse_on();
	   return(0);
	 };/* if */
   if(ente1!=0)
	 { /* рисовать эллипс */
          if((xkom_end != x_mouse)||(ykom_end != y_mouse)){
	   set_mode_xor();
	   ellipse(xkom_strt,ykom_strt,0,360,
		   abs(xkom_strt-xkom_end),abs(ykom_strt-ykom_end));
           set_mode_xor();
	   ellipse(xkom_strt,ykom_strt,0,360,
		   abs(xkom_strt-x_mouse),abs(ykom_strt-y_mouse));
	   xkom_end=x_mouse;ykom_end=y_mouse;
           }// if
	 };/*  */
       /* оставить нарисованный эллипс */
   if((getkey_right>0)&&(ente1!=0))
	 {ente1=0;
	   /* стереть эллипс */
       set_mode_xor();
	   ellipse(xkom_strt,ykom_strt,0,360,
		   abs(xkom_strt-xkom_end),abs(ykom_strt-ykom_end));
	   pic.CreateGE(OBJ_felip);// создание GE заданного типа
       pic.ShowCur();   // рисование созданного обьекта
	   ybcur();
	   mouse_on();
	   return(-1);
	 };/* if */
   return(0);
}/* pp_pos161 */
/****************************************************************************/
int pp_pos17() /* текст */
{
 int x_txt,y_txt,i;
 char *area[480];// буфер для места под текстом

 	 if(korr_txt==OFF)
	 { korr_txt=ON;
       if(pic.GetTip() == TIP_txt)/* Взять из текущего ГЭ */
		    sprintf(text,"%s",pic.GetText());
       mouse_off();
       sprintf(text,"%s",EditString(text));
       mouse_on();
       ybcur();
       if(strlen(text)==0) {return(-1);};
	 }// if


   if(getkey_left>0)  /* нажата левая кнопка */
   {
     mouse_off();
     xkom_strt=xkom_end=x_mouse;ykom_strt=ykom_end=y_mouse;

     settextjustify(pck.hor_t,pck.ver_t);
//     settextstyle(pck.fnt_t,pck.dir_t,pck.siz_t);//
//     setusercharsize(Multx,10,Multy,10);//

     if((pck.fnt_t) == 0){ //default текст
        settextstyle(pck.fnt_t,pck.dir_t,pck.siz_t);
      }
      else{
        settextstyle(pck.fnt_t,pck.dir_t,0);
        setusercharsize(Multx,10,Multy,10);
      }// if else

//======== определить размер области для сохранения
//     x_txt=textwidth (text);
//     y_txt=textheight(text);
     x_txt=TextWidthCol(text);
     y_txt=TextHeightCol(text);

     int dx,dy;

     if(pck.dir_t == HORIZ_DIR) // горизонтально
     {
      dx=x_txt;
      dy=y_txt;
     }// if
     if(pck.dir_t == VERT_DIR) // вертикально
     {
      dx=y_txt;
      dy=x_txt;
     }// if

      x_min=xkom_strt-dx;
      x_max=xkom_strt+dx;
      y_min=ykom_strt-dy;
      y_max=ykom_strt+dy;
      x_max+=55;
      y_max+=55;

//======== определить размер области для сохранения
    /* проверка на допустимость min,max */
	if(x_min < 0) x_min=0;
	if(x_max > MaxX) x_max=MaxX;
	if(y_min < 0) y_min=0;
	if(y_max > MaxY) y_max=MaxY;

	size_str=imagesize(x_min,y_min,x_max,y_min);

	for(i=y_min;i<= y_max;i++)
	{area[i]=(char *)malloc(size_str);
	 if(area[i]==NULL){Warning("'pp_pos17'???");return(0);};
	 getimage(x_min,i,x_max,i,area[i]);
	};/*for*/


//	outtextxy(xkom_strt,ykom_strt,text);
   OutTextCol(xkom_strt,ykom_strt,text);
// рамка
//    setcolor(EGA_LIGHTRED);
//    rectangle(x_min,y_min,x_max,y_max);
// точка
//    setcolor(EGA_WHITE);
//    rectangle(xkom_strt,ykom_strt,xkom_strt+1,ykom_strt+1);

//getch();


	/* ожидание  */
    press_key();

	for(i=y_min;i<= y_max;i++)
	   {putimage(x_min,i,area[i],COPY_PUT);
	    free(area[i]);
	   };/*for*/

	   mouse_on();
	   return 0;
	 }// if нажата левая кнопка

       /* оставить текст */
   if(getkey_right>0) /* нажата правая кнопка */
	 {
	   mouse_off();
       xkom_strt=xkom_end=x_mouse;
       ykom_strt=ykom_end=y_mouse;
	   pic.CreateGE(OBJ_txt);// создание GE заданного типа
//	   if(pic.CreateGE(OBJ_txt) <0) return -1;// создание GE заданного типа
       pic.ShowCur();   // рисование созданного обьекта
	   ybcur();
	   korr_txt=OFF;//текст не введен
	   mouse_on();
	   return(-1);
	 }// if  нажата правая кнопка
	 return 0;
}/* pp_pos17 */
/****************************************************************************/

int pp_pos18() /* заливка */
{
	   if(getkey_left>0)
	 {
	  if(ente_fld==0) ente_fld=1;
	    else      ente_fld=0;
	  if(ente_fld==1)
	  {
	   xkom_strt=xkom_end=x_mouse;ykom_strt=ykom_end=y_mouse;
	   /* нарисовать заливку */
	   mouse_off();ybcur();
	   floodfill(xkom_strt,ykom_strt,pck.col_lt);
	   ybcur();mouse_on();
	   return(0);
	  };
	  if(ente_fld==0)
	  {
	   return(-1);
	  };

	 };/* if left */

       if(getkey_right>0)
	 {
	  xkom_strt=xkom_end=x_mouse;ykom_strt=ykom_end=y_mouse;
	   mouse_off();
	   pic.CreateGE(OBJ_fld);// создание GE заданного типа
//	   if(pic.CreateGE(OBJ_fld) <0) return -1;// создание GE заданного типа
       pic.ShowCur();   // рисование созданного обьекта
	   ybcur();
	   mouse_on();
	   ente_fld=OFF;
	   return(-1);
	 };
         return(0);

}/* pp_pos18 */
/****************************************************************************/
int pp_pos19() /* фон */
{

   if(getkey_left>0)
	 {
	  if(ente_bkgr==0) ente_bkgr=1;
	     else          ente_bkgr=0;
	  if(ente_bkgr==1)
	  {
	   /* нарисовать фон */
	   mouse_off();
	   setbkcolor(pck.col_f);
//       printf("%d\n",col_f);
/*
       press_key();
	   setbkcolor(EGA_RED);
*/
	   mouse_on();
	   return(0);
	  };
	  if(ente_bkgr==0)
	  {
	   return(-1);
	  };
	 };/* if left */

       if(getkey_right>0)
	 {
	   mouse_off();
	   pic.CreateGE(OBJ_bkgr);// создание GE заданного типа
//	   if(pic.CreateGE(OBJ_bkgr) <0) return -1;// создание GE заданного типа
       pic.ShowCur();   // рисование созданного обьекта
	   ybcur();
	   mouse_on();
	   ente_bkgr=OFF;
	   return(-1);
	 };
         return(0);

}/* pp_pos19 */
/****************************************************************************/
/* ----------------------------------------------------------------------- */
int ABSx(int crd){
   return (int)(0.5+((crd/(double)SX_vp)*SIZE_XY));
}/* ABSx */
/* ----------------------------------------------------------------------- */
int ABSy(int crd){
   return (int)(0.5+((crd/(double)SY_vp)*SIZE_XY));
}/* ABSy */
/* ----------------------------------------------------------------------- */
int pp_pos20() /* макропримитив */
{

   static MPClass *tmpc;

   if(getkey_left>0){

	  if(ente_bkgr==0)
	     ente_bkgr=1;
	  else
	     ente_bkgr=0;


	  if(ente_bkgr==1){
	   /* нарисовать macro */
	   mouse_off();
      xkom_strt=xkom_end=x_mouse;
		ykom_strt=ykom_end=y_mouse;
      // save place
      if(pck.mc_index){
         hide_tbl();
         tmpc = MPCLoad(pck.mc_index);
         restore_tbl();
         if(tmpc)
            MPCDraw(tmpc, ABSx(x_mouse), ABSy(y_mouse), pck.mc_state, NULL, NULL);
      }/* if */
	   mouse_on();
	   return(0);
	  };

	  if(ente_bkgr==0){
      // restore place
      if(tmpc){
		   MPCFree(tmpc);
         tmpc = NULL;
      }/* if */
      // return 0;
	   return(-1);
	  };
   };/* if left */

   if(getkey_right>0){
	   mouse_off();
      xkom_strt=xkom_end=x_mouse;
		ykom_strt=ykom_end=y_mouse;
	   pic.CreateGE(OBJ_esc);// создание GE заданного типа
      pic.ShowCur();   // рисование созданного обьекта
      ybcur();
	   mouse_on();
	   ente_bkgr=OFF;
	   return(-1);
   };
   return(0);

}/* pp_pos20 */
/****************************************************************************/
int pp_pos30()  /* сохранение */
{
 char *name;
 short int x,y;
 mouse_getxy(&x,&y);// берем координаты мыши

 switch(pck.Mode_save){
  case mode_save_ram_pcx:
       if(ente<=1){Warning("Нет рамки");goto end;};
  break;
  case mode_save_all_tbg:
  case mode_save_ram_tbg:
  case mode_save_ge_tbg :
    if(pic.GetPN() < 0){
       Warning("Нет элементов");
       goto end;
    }// if
    if(pck.Mode_save == mode_save_ram_tbg){
       if(ente<=1)        {Warning("Нет рамки");goto end;};
       if(ind_select==OFF){Warning("В рамке нет элементов");goto end;};
    }// if
  break;
 }// switch

  name=menu_sl(SAVE_obj);/* запрос имени файла */
  if(name!=0)  /* coxранение нужно */
    {
     switch(pck.Mode_save){
      case mode_save_all_pcx:
      case mode_save_ram_pcx:
//            pck.Mode_save = mode_save_all_pcx;
            sprintf(pck.Fn_work_pcx,"%s",name);//запоминаем имя файла
            mouse_setxy(x,y);//на место стать
            return RESTART;
//      break;
      case mode_save_all_tbg:
//            pic.SaveAll(name);
//            sprintf(pck.Fn_work_tbg,"%s",name);//запоминаем имя файла
//      break;
      case mode_save_ram_tbg:
//            pic.SaveRam(name);
//            sprintf(pck.Fn_work_tbg,"%s",name);//запоминаем имя файла
//      break;
      case mode_save_ge_tbg :
//            pic.SaveGE(name);// сохранение изображения ГЭ
            pic.SaveAll(name,pck.Mode_save);
//            sprintf(pck.Fn_work_tbg,"%s",name);//запоминаем имя файла
      break;
     }// switch
     indid_cur_fn();// индикация теущего имени файла
    }/* coxранение нужно */

end:;
  mouse_setxy(x,y);//на место стать
  return NORESTART;

}/* pp_pos30 */
/****************************************************************************/
int pp_pos31() /* загрузка */
{
  short int x,y;
  mouse_getxy(&x,&y);// берем координаты мыши

  char	*name=menu_sl(LOAD_obj);/* запрос имени файла */

  mouse_setxy(x,y);//на место стать
  if(name!=0)  /* загрузка нужнa */
    {
     switch(pck.Mode_load){
      case mode_load_fon_pcx:
           sprintf(pck.Fn_fon_pcx,"%s\\%s",pck.div_path,name); // полное имя
           sprintf(pck.Fn_work_pcx,"%s",name);//запоминаем имя файла
      break;
      case mode_load_new_tbg:
           pic.DelGE_All();   // удаление всех ГЭ
           if(pic.LoadFile(name)==Fu) return NORESTART;;// загрузка изображения из файла
           sprintf(pck.Fn_work_tbg,"%s",name);//запоминаем имя файла
           // признак необходимости сохранения для нового файла
           if(pck.Mode_load == mode_load_new_tbg) SaveTest=OFF;
      break;
      case mode_load_wrk_tbg:
           if(pic.LoadFile(name)==Fu) return NORESTART;;// загрузка изображения из файла
//           sprintf(pck.Fn_work_tbg,"%s",name);//запоминаем имя файла
           // признак необходимости сохранения для нового файла
           if(pck.Mode_load == mode_load_new_tbg) SaveTest=OFF;
      break;
/*
      case mode_load_new_tbg:
           pic.DelGE_All();   // удаление всех ГЭ
      case mode_load_wrk_tbg:
           if(pic.LoadFile(name)==Fu) return NORESTART;;// загрузка изображения из файла
           sprintf(pck.Fn_work_tbg,"%s",name);//запоминаем имя файла
           // признак необходимости сохранения для нового файла
           if(pck.Mode_load == mode_load_new_tbg) SaveTest=OFF;
      break;
*/
      case mode_load_fon_tbg:
           sprintf(pck.Fn_fon_tbg,"%s\\%s",pck.div_path,name); // полное имя
           hide_tbl();   // временное отключение библиотеки
           if(FonTbg != NULL) del_pic(&FonTbg); // удаляем предыдущий фон
           if(load_file(&FonTbg,pck.Fn_fon_tbg) < 0)
              pck.Fn_fon_tbg[0]=NULL; // удаляем фон TBG при неуспешной загрузке
           restore_tbl();// восстановление библиотеки
//           sprintf(pck.Fn_work_tbg,"%s",name);//запоминаем имя файла
      break;
     }// switch
     return RESTART;
    }/* загрузка нужнa */

  return NORESTART;
};//pp_pos31() /* загрузка */
/****************************************************************************/

int pp_pos33() /* удаление */
{

 switch(pck.Mode_del){
  case mode_del_all:
  case mode_del_ram:
  case mode_del_ge :
    if(pic.GetPN() < 0){
       Warning("Нет элементов");
       return RESTART;
    }// if
    if(pck.Mode_del == mode_del_ram){
       if(ente<=1)        {Warning("Нет рамки");return RESTART;};
       if(ind_select==OFF){Warning("В рамке нет элементов");return RESTART;};
    }// if
  break;
 }// switch

 switch(pck.Mode_del){
  case mode_del_fon_pcx:
       pck.Fn_fon_pcx[0]=NULL;
       pck.Mode_del=mode_del_ge;//автоматический возврат после удаления фона
       pck.ig_mode=old_ig_mode;
  break;
  case mode_del_fon_tbg:
       if(FonTbg != NULL) del_pic(&FonTbg); // удаляем картинку фона
       pck.Fn_fon_tbg[0]=NULL; // удаляем фон TBG
       pck.Mode_del=mode_del_ge;//автоматический возврат после удаления фона
  break;
  case mode_del_all:
       pic.DelGE_All();//удаление всех ГЭ
       pck.Mode_del=mode_del_ge;//автоматический возврат из полного удаления
  break;
  case mode_del_ram:pic.DelGE_Ram();//удаление ГЭ в рамке
  break;
  case mode_del_ge :pic.DelGE();// удаление текущего ГЭ
  break;
 }// switch

 return RESTART;

}/* pp_pos33 */
/****************************************************************************/

int pp_pos34() //копирование
{
  if(pic.GetPN() >= 0)
    {
     if(pck.Mode_copy==mode_copy_ram){
	     if(ente<=1){Warning("Нет рамки");return RESTART;};
	     if(ind_select==OFF){Warning("В рамке нет элементов");return RESTART;};
        pic.CopyRam();//копирование ГЭ в рамке
     };//if ram
     if(pck.Mode_copy==mode_copy_ge) pic.CopyGE();// копирование текущего ГЭ
    }// if
    else Warning("Нет элементов");

    return RESTART;
}/* pp_pos34 */

/****************************************************************************/
int pp_pos35() /* сдвиг */
{
if(getkey_left>0)
  {

  if(pic.GetPN() >= 0)
    {
     if(pck.Mode_move==mode_move_ge) pic.MoveGE();//сдвиг ГЭ
     if(pck.Mode_move==mode_move_ram)
       {
	if(ente<=1){Warning("Нет рамки");return RESTART;};
	if(ind_select==OFF){Warning("В рамке нет элементов");return RESTART;};
	pic.MoveRam();//сдвиг ГЭ в рамке
       };//if ram
    }
    else Warning("Нет элементов");

    return RESTART;
  };//if

  if(getkey_right>0)
    {if(ind_move==0){ mouse_off();
	ybcur();ind_move=1;pos_ms_pred-=1;
       }/* then if */
       else{
	ybcur();
	mouse_on();ind_move=0;pos_ms_pred-=1;
       };/* else if */
     };//if
  return NORESTART;

}/* pp_pos35 */
/****************************************************************************/

int pp_pos36() /* масштабирование */
{

//rrr  mx=10.0*((double)x_mouse/(double)getmaxx());
  mx=10.0*((double)x_mouse/(double)MaxX);

  char s[30];//буфер вывода
  sprintf(s,"Коэффициент: %f",mx);

//  disable();// запрет прерываний
  outpic_obj_num(menu_mess,255,1);//стираем предыдущее пояснение
//  enable();// разрешение прерываний
  outtext_obj_num(menu_mess,1,32,getcolor_obj_num(menu_mess,255,1),s,SHOW);

   if(getkey_left>0)
	 {
      if(pic.MashRam() < 0) return NORESTART;
      return RESTART;
     };
   if(getkey_right>0)
	 {
      pic.SetOld();
      return RESTART;
     };

  return NORESTART;
}/* pp_pos36 */
/****************************************************************************/
int pp_pos39(void)
{
static int press=OFF;
static int xst_e,xen_e,yst_e,yen_e;
static int txa_min,txa_max,tya_min,tya_max,txca,tyca,
           tsize_xya=SIZE_XY;

char buf[4];
static int tkf_size;

   if(getkey_left>0)/* левая кнопка */
     {
      if(press==OFF) // нажатие первое
	    { press=ON;
//        pos_ms_pred=-1;
	    mouse_off();
        tkf_size=kf_size;

        xst_e=xen_e=x_mouse;
        yst_e=yen_e=y_mouse;

        txa_min=mmx(xst_e);
        tya_min=mmy(yst_e);

        setwritemode(XOR_PUT);
        setcolor(EGA_WHITE);
        setlinestyle(DOTTED_LINE,0,NORM_WIDTH);
        rectangle(xst_e,yst_e,xen_e,yen_e);
        setwritemode(COPY_PUT);
        mouse_on();
	    return NORESTART;

	  }//if  нажатие первое
      else { //нажатие  не первое
        press=OFF;
        mouse_off();
        kf_size=tkf_size;

        setwritemode(XOR_PUT);
        setcolor(EGA_WHITE);
        setlinestyle(DOTTED_LINE,0,NORM_WIDTH);
        rectangle(xst_e,yst_e,xen_e,yen_e);
        setwritemode(COPY_PUT);

        sprintf(buf,"%2.2d",52-kf_size);
        outtext_obj_num(menu_1,3,120,getcolor_obj_num(menu_1,255,1),buf,SHOW);


        mouse_on();
	    return NORESTART;

      }//if else нажатие  не первое
     }//if левая кнопка

   if(getkey_right>0)/* правая кнопка */
     {
      if(press==OFF) // нажатие первое
	    {

        txca=mmx(x_mouse);
        tyca=mmy(y_mouse);

	if(((long)txca+(long)win.size_xya/2L) >= 32760L ) txca=SIZE_XY-win.size_xya/2;
	if((txca-win.size_xya/2) < 0                    ) txca=win.size_xya/2;

	if(((long)tyca+(long)win.size_xya/2L) >= 32760L ) tyca=SIZE_XY-win.size_xya/2;
	if((tyca-win.size_xya/2) < 0                    ) tyca=win.size_xya/2;

       win.xa_min=txca-win.size_xya/2;
//       xa_max=txca+size_xya/2;
       win.ya_min=tyca-win.size_xya/2;
//       ya_max=tyca+size_xya/2;

       return RESTART;

	  }//if  нажатие первое
      else { //нажатие  не первое
	press=OFF;

	win.xa_min=txa_min;
	win.ya_min=tya_min;
	win.size_xya=tsize_xya;

	    return RESTART;

      }//if else нажатие  не первое
     }//if правая кнопка

if(press==ON) // действия без нажатий,но когда тянем ЛУПУ
  {
    tsize_xya=abs(mmx(xst_e)-mmx(x_mouse));
    if(tsize_xya < MaxX) tsize_xya = MaxX;

    if(((long)txa_min+(long)tsize_xya) > 32760L) tsize_xya=SIZE_XY-txa_min;
    if(((long)tya_min+(long)tsize_xya) > 32760L) tsize_xya=SIZE_XY-tya_min;

    txa_max=txa_min+tsize_xya;
    tya_max=tya_min+tsize_xya;

//    printf("%d %d %d %d %d            \r",
//           txa_min,txa_max,tya_min,tya_max,tsize_xya);

    kf_size=tsize_xya/639;
    tsize_xya=kf_size*639;
    if(kf_size==51) tsize_xya=SIZE_XY;


    sprintf(buf,"%2.2d",52-kf_size);
    outtext_obj_num(menu_1,3,120,getcolor_obj_num(menu_1,255,1),buf,SHOW);

//    printf("ts=%d kf=%d            \r",
//           tsize_xya,kf_size);

     mouse_off();
     setwritemode(XOR_PUT);
     setcolor(EGA_WHITE);
     setlinestyle(DOTTED_LINE,0,NORM_WIDTH);

     rectangle(xst_e,yst_e,xen_e,yen_e);

     xen_e=DMx(txa_max);
     yen_e=DMy(tya_max);
     rectangle(xst_e,yst_e,xen_e,yen_e);
     setwritemode(COPY_PUT);
     mouse_on();
  }//if действия без нажатий,но когда тянем ЛУПУ

 return NORESTART;
}// pp_pos39
/****************************************************************************/
void pp_indic()// индикация
{
   ybcur();
   if(pck.Mode_indic==mode_indic_no) pic.IndicNO();
   if(pck.Mode_indic==mode_indic_ln) pic.IndicLN();
   if(pck.Mode_indic==mode_indic_ge) pic.IndicGE();
   ybcur();
   pos_ms_pred=-1;/* перевывод строки подсказки */

};// pp_indic
/****************************************************************************/