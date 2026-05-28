// panel.c ==============================================================

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <conio.h>
#include <bios.h>
#include <string.h>
#include <mem.h>
#include <graphics.h>
#include <alloc.h>

#include "tbg_tc.h"

#include "panel.h"

extern unsigned int markers[][32];
#define HAND  0
#define ARROW 1
#define CROSS 2

/****************************************************************************/
static int col255_1,
           col255_2,
           col255_3,
           col255_4,
           col255_5,
           col255_6;
static char far *image=NULL;
static unsigned max_pixx=0,_ft=OFF;
/****************************************************************************/
#define _SPACE_ 14624
#define _ESC_ 	283
#define _ENTER_	7181
#define _UP_ 	18432
#define _DOWN_ 	20480
#define _TAB_   3849
#define	_LEFT_ 	19200
#define _RIGHT_ 19712


int panel_mode(PANEL *pnl)// установка режимов панели
{
 int i;
 int ret=Ok;

 OBJECT **menu=NULL;
 OBJECT **menu_col=NULL;
 short int x,y;
 short int xp,yp;
 short int left,right;/* какие кнопки были нажаты */
 short int cur_posfld;
 short int cur_pospnl;
 short int maxfld;

 short int skod;
 CHAR FlafUD=OFF;

 long predtime;

 predtime=biostime(0,0L);

 mouse_getxy(&x,&y);// берем координаты мыши

 if(load_file(&menu,pnl->fname)         < 0) return -1;/* загрузка */
 if(load_file(&menu_col,pnl->fname_col) < 0) return -1;/* загрузка */

 Store_Image(menu,"image.tmp");// сохраняем место под меню

 mouse_off();

 col255_1=getcolor_obj_num(menu_col,255,1);
 col255_2=getcolor_obj_num(menu_col,255,2);
 col255_3=getcolor_obj_num(menu_col,255,3);
 col255_4=getcolor_obj_num(menu_col,255,4);
 col255_5=getcolor_obj_num(menu_col,255,5);
 col255_6=getcolor_obj_num(menu_col,255,6);

 outpic_all(menu);// показ меню
 show_pospnl(pnl,menu);
 show_posfld(pnl,menu);

 // показ режимов
 for(i=0;i < pnl->maxpnl;i++ ) {
     show_panel(pnl,menu,i);
     if(pnl->init_fun != NULL) // функция есть
        pnl->init_fun(menu);   // выполнение функции
/*
     if(fRUN)
        if((*(pnl->pf+i))->funfld != NULL) // функция есть
           (*(pnl->pf+i))->funfld(menu);   // выполнение функции
*/
 }// for

 mouse_mask((unsigned short int*)(markers[HAND]),0,0);
 mouse_on();

 do{

 /* рисуем мышь */
 mouse_draw();

 mouse_lpk(&left,&right);/* какие кнопки были нажаты */

 if(bioskey(1)!=0) {
    skod=bioskey(0);
    if((*(pnl->pf+(pnl->pospnl)-1))->type == f_info) {skod = _ENTER_;} //if
 }
 else skod=0;

 // выход по любой кнопке мыши для иформационного поля
 if((left != 0)||(right != 0)){
   if((*(pnl->pf+(pnl->pospnl)-1))->type == f_info) {skod = _ENTER_;} //if
 }// if

 if(left)
 {/* двойной щелчок */
   long curtime=biostime(0,0l);
   if(labs(predtime-curtime) < 2)
      skod = _ENTER_;
   predtime=curtime;
 }/* if */

 if(skod == _ESC_)   {right=1;ret=Fu;}// if
 if(skod == _ENTER_) {right=1;ret=Ok;}// if

 cur_pospnl=getpos_mouse_obj(menu,253,1);

/*
 if(left!=0) cur_pospnl=getpos_mouse_obj(menu,253,1);
   else      cur_pospnl=(pnl->pospnl);
*/

 if(skod == _TAB_) {
    cur_pospnl=(pnl->pospnl);
    if(++cur_pospnl > pnl->maxpnl) cur_pospnl=1;
 }// if

 switch(cur_pospnl)
 {
  case -1: break;
  default:
  if(cur_pospnl != (pnl->pospnl)){
     (pnl->pospnl)=cur_pospnl;
     show_pospnl(pnl,menu);
     show_posfld(pnl,menu);
//!!!     left = 0;
  }// if

 }// switch

 cur_posfld=getpos_mouse_obj(menu,(*(pnl->pf+(pnl->pospnl)-1))->objfld,0);

 if((skod == _DOWN_)||(skod == _UP_)||
    (skod == _LEFT_)||(skod == _RIGHT_)
   )
 {
    cur_posfld=(*(pnl->pf+(pnl->pospnl)-1))->posfld;
    getmax_obj(menu,(*(pnl->pf+(pnl->pospnl)-1))->objfld,&maxfld);
    FlafUD=ON;
 }// if

 switch(skod)
 {
  case _RIGHT_:
  case _DOWN_:if(++cur_posfld > maxfld) cur_posfld=0;
  break;
  case _LEFT_:
  case _UP_:  if(--cur_posfld < 0) cur_posfld=maxfld;
  break;
 }// switch

    mouse_getxy(&xp,&yp);// берем координаты мыши
    // мышь вне панели=кнопка не действует
    if(pren(menu,255,1,xp,yp) == OFF) left=0;

    switch(cur_posfld)
    {
     case -1: // если -1
           cur_posfld=(*(pnl->pf+(pnl->pospnl)-1))->posfld;
//     break;
     default:
     if(cur_posfld != (*(pnl->pf+(pnl->pospnl)-1))->posfld){
        (*(pnl->pf+(pnl->pospnl)-1))->posfld=cur_posfld;
        show_posfld(pnl,menu);
       if(((*(pnl->pf+(pnl->pospnl)-1))->type == f_mono)&&(FlafUD==ON)){
        skod = _SPACE_;
        FlafUD=OFF;
       }// if
     }// if

    if((left != 0) || (skod == _SPACE_))
      { // нажата левая кнопка или "пробел"

      switch((*(pnl->pf+(pnl->pospnl)-1))->type)
      {
       case f_mono:
             if((*(pnl->pf+(pnl->pospnl)-1))->mode != NULL)
                *((*(pnl->pf+(pnl->pospnl)-1))->mode)=cur_posfld;
             show_panel(pnl,menu,(pnl->pospnl)-1);// показ режимов
             if((*(pnl->pf+(pnl->pospnl)-1))->funfld != NULL)// функция есть
                (*(pnl->pf+(pnl->pospnl)-1))->funfld(menu, cur_posfld); //выполнение функции
       break;
       case f_poli:
             if((*(pnl->pf+(pnl->pospnl)-1))->mode != NULL){
                if(*((*(pnl->pf+(pnl->pospnl)-1))->mode+cur_posfld)==0){
                   *((*(pnl->pf+(pnl->pospnl)-1))->mode+cur_posfld)=1;
                }/* if */
                else{
                   *((*(pnl->pf+(pnl->pospnl)-1))->mode+cur_posfld)=0;
                }/* elif */
             }/* if */
             show_panel(pnl,menu,(pnl->pospnl)-1);// показ режимов
             if((*(pnl->pf+(pnl->pospnl)-1))->funfld != NULL)// функция есть
                (*(pnl->pf+(pnl->pospnl)-1))->funfld(menu, cur_posfld); //выполнение функции

      break;
     }//switch
    } // нажата левая кнопка
   }//switch
/*
   {
      static short oldmx, oldmy;
             short mx, my;

      mouse_getxy(&mx, &my);
		if((oldmx != mx) || (oldmy != my)){
         setwritemode(XOR_PUT);
         setfillstyle(SOLID_FILL, 15);
         rectangle(oldmx - 1, oldmy - 1, oldmx + 1, oldmy + 1);
         rectangle(mx - 1, my - 1, mx + 1, my + 1);
         setwritemode(COPY_PUT);
         oldmx = mx;
         oldmy = my;
      }/* if */
	}
*/
}while(right==0);/* нажата правая кнопка */

  Restore_Image(menu,"image.tmp");// восстанавливаем место под меню

  del_pic(&menu);
  del_pic(&menu_col);
  mouse_mask((unsigned short int*)(markers[ARROW]),0,0);
  mouse_setxy(x,y);//мышь на место

  return ret;
}// panel_mode
/*-----------------------------------------------*/
// показ режимов
void show_panel(PANEL *pnl,
                OBJECT **menu,
                unsigned char numpnl
               )// показ режимов
{
 int i=0;
 short int maxnum;

 if(((*(pnl->pf+numpnl))->iv) == SHOW_FLD)
   { // признак наличия индикации выбора позиции
     mouse_off();
      switch((*(pnl->pf+numpnl))->type)
      {
       case f_mono: // индикация поля однозначного выбора
          if((*(pnl->pf+numpnl))->mode){
             setcolor_obj    (menu,((*(pnl->pf+numpnl))->objfld)+2,col255_1,SHOW);
             setcolor_obj_num(menu,((*(pnl->pf+numpnl))->objfld)+2,
                                  *((*(pnl->pf+numpnl))->mode),col255_2,SHOW);
          }/* if */
        break;       // индикация поля однозначного выбора
       case f_poli: // индикация поля многозначного выбора
        getmax_obj(menu,((*(pnl->pf+numpnl))->objfld)+2,&maxnum);

        if((*(pnl->pf+numpnl))->maxmode != maxnum+1)
           printf("Многозначное поле=%d,элементов обьекта %d=%d \r",
                   (*(pnl->pf+numpnl))->maxmode,
                   ((*(pnl->pf+numpnl))->objfld)+2,
                   maxnum+1);

        for(i=0; i <= maxnum; i++)
        {
//        printf("%d    \r",i);
          if((*(pnl->pf+numpnl))->mode){
              if(*((*(pnl->pf+numpnl))->mode+i)==1){
                 setcolor_obj_num(menu,((*(pnl->pf+numpnl))->objfld)+2,i,col255_2,SHOW);
              }/* if */
              else{
                 setcolor_obj_num(menu,((*(pnl->pf+numpnl))->objfld)+2,i,col255_1,SHOW);
			     }/* elif */
          }/* if */
        }//for
       break;      // индикация поля многозначного выбора
      }//switch
     mouse_on();
   }// признак наличия индикации выбора позиции


}// show_panel
//-----------------------------------------------------------------------
void show_pospnl(PANEL *pnl,OBJECT **menu)
{
   mouse_off();
   setcolor_obj    (menu,254,col255_5,SHOW);
   setcolor_obj_num(menu,254,pnl->pospnl,col255_6,SHOW);
   mouse_on();
}//  show_pospnl
//-----------------------------------------------------------------------
void show_posfld(PANEL *pnl,OBJECT **menu)
{
 int i;

 mouse_off();

 for(i=0;i < pnl->maxpnl;i++ ){
  if(((*(pnl->pf+i))->iu) == SHOW_FLD)
  { // признак наличия индикации указания позиции
     setcolor_obj(menu,((*(pnl->pf+i))->objfld)+1,col255_3,SHOW);
  }// if
 }// for

 if(((*(pnl->pf+((pnl->pospnl)-1)))->iu) == SHOW_FLD)
 { // признак наличия индикации указания позиции

    setcolor_obj_num(menu,((*(pnl->pf+((pnl->pospnl)-1)))->objfld)+1,
                          ((*(pnl->pf+(pnl->pospnl)-1))->posfld),
                          col255_4,SHOW);
// printf("-------   \n");

 }// if
 mouse_on();
}//  show_posfld
//-----------------------------------------------------------------------
void Store_Image(OBJECT **pic,char *filename)
{

   FILE *ft;
   short int x_min,y_min,x_max,y_max,i;
   char area[644];


   // printf("..getsize_obj_num\n");
   getsize_obj_num_reset();
   getsize_obj_num(pic,255,1,&x_min,&y_min,&x_max,&y_max);
   // printf("..imagesize");
/*
   max_pixx=imagesize(x_min,y_min,x_max,y_max);
   // printf(": %u\n", max_pixx);

   if(max_pixx==0xFFFF){
   // printf(".._ft=ON\n");
      _ft=ON;
   }//if
   else{
   // printf(".._ft=OFF\n");
      _ft=OFF;
   // printf("..malloc\n");
//      if((image=(char far *)malloc(max_pixx))==NULL)
         _ft=ON;
         max_pixx==0xFFFF;
   }//if else
*/
   _ft=ON;
   max_pixx=0xFFFF;

   // printf("..mouse_off\n");
   mouse_off();
   // printf(".._ft==ON ?\n");
   if(_ft==ON){ //сохраняемся в файле
   // printf("..fopen\n");
      if((ft=fopen(filename,"wb"))==NULL){
         printf("\nStore_Image:Не могу открыть файл '%s'",filename);
         mouse_on();
         return;
      }//if
   // printf("..imagesize\n");
      max_pixx=imagesize(x_min,y_min,x_max,y_min);
   // printf("..for\n");
      for(i=y_min;i<= y_max;i++){
         getimage(x_min,i,x_max,i,area);
         fwrite(area,1,max_pixx,ft);
         if(i == (y_min+10)){
            setfillstyle(SOLID_FILL,EGA_LIGHTRED);
            setlinestyle(0,1,1);
            setcolor(EGA_WHITE);
            settextjustify(CENTER_TEXT,CENTER_TEXT);
            settextstyle(DEFAULT_FONT,0,1);
            bar(x_min,y_min,x_max,y_min+10);
            rectangle(x_min,y_min,x_max,y_min+10);
            outtextxy(x_min+(x_max-x_min)/2,y_min+6,"Ждите...");
         }//if
      }// for
   // printf("..fclose\n");
      fclose(ft);
   }//if
   else{ //сохраняемся в памяти
   // printf("..getimage\n");
      if(image)
         getimage(x_min,y_min,x_max,y_max,image);
   }//if else
   // printf("..mouse_on\n");
   mouse_on();
   // printf("..return\n");

}// Store_Image
//---------------------------------------------------------------------------
void Restore_Image(OBJECT **pic,char *filename)
{

   FILE *ft;
   short int x_min,y_min,x_max,y_max,i;
   char area[644];

   getsize_obj_num_reset();
   getsize_obj_num(pic,255,1,&x_min,&y_min,&x_max,&y_max);

   mouse_off();
   if(_ft==ON){ //восстаналиваемся из файла
      if((ft=fopen(filename,"rb"))==NULL){
         printf("\nRestore_Image:Не могу открыть файл '%s'",filename);
         mouse_on();
         return;
      }//if

      for(i=y_min;i<= y_max;i++){
         fread(area,1,max_pixx,ft);
         putimage(x_min,i,area,COPY_PUT);
      }// for
      fclose(ft);
//      unlink(filename);
   }//if
   else{ // восстанавливаемся из памяти
      putimage(x_min,y_min,image,COPY_PUT);
      free(image);
   }//if else
   mouse_on();

}// Restore_Image
// end panel.c ==========================================================