// red5_015.cpp

#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <conio.h>
#include <graphics.h>

#include "red5.h"

/****************************************************************************/
int oumenu(void)
{

 char buf[30];//буфер для всех строк вывода

if(mn1_out==OFF)/* меню нет ? */
  {mn1_out=ON;  /* выведено */
   outpic_all(menu_1);/* вывод меню */
  };/* if  меню нет ? */

/* закраска */
indic_stl_f(menu_1);

/* линия */
indic_stl_l(menu_1);

/* текст */
//sprintf(buf,"???");
if(pck.fnt_t==0)  sprintf(buf ,"D%2.2d",pck.siz_t);
if(pck.fnt_t==1)  sprintf(buf ,"T%2.2d",pck.siz_t);
if(pck.fnt_t==2)  sprintf(buf ,"L%2.2d",pck.siz_t);
if(pck.fnt_t==3)  sprintf(buf ,"S%2.2d",pck.siz_t);
if(pck.fnt_t==4)  sprintf(buf ,"G%2.2d",pck.siz_t);
if(pck.fnt_t==5)  sprintf(buf ,"C%2.2d",pck.siz_t);
if(pck.fnt_t==6)  sprintf(buf ,"I%2.2d",pck.siz_t);
if(pck.fnt_t==7)  sprintf(buf ,"R%2.2d",pck.siz_t);
if(pck.fnt_t==8)  sprintf(buf ,"M%2.2d",pck.siz_t);
if(pck.fnt_t==9)  sprintf(buf ,"U%2.2d",pck.siz_t);
if(pck.fnt_t==10) sprintf(buf ,"B%2.2d",pck.siz_t);

if(pck.dir_t==HORIZ_DIR) outtext_obj_num(menu_1,7,2,pck.col_lt,buf,SHOW);/* стиль текста */
if(pck.dir_t== VERT_DIR) outtext_obj_num(menu_1,7,1,pck.col_lt,buf,SHOW);/* стиль текста */
outpic_obj_num(menu_1,2,71);/* стирание индикации текста */
if(pck.dir_t==HORIZ_DIR) setcolor_obj_num(menu_1,7,2,pck.col_lt,123);/* стиль текста */
if(pck.dir_t== VERT_DIR) setcolor_obj_num(menu_1,7,1,pck.col_lt,123);/* стиль текста */

// индикация стиля в окне "стиля"
*(buf+1)=NULL;
outtext_obj_num(menu_1,99,1,EGA_DARKGRAY,buf,SHOW);
/*
 outpic_obj_num(menu_1,99,255);
 settextjustify(CENTER_TEXT,CENTER_TEXT);
 setcolor(pck.col_lt);
 settextstyle(0,pck.dir_t,1);

 int xmin,ymin,xmax,ymax;

 getsize_obj_num_reset();
 getsize_obj_num(menu_1,99,255,&xmin,&ymin,&xmax,&ymax);

 outtextxy((xmin+(xmax-xmin)/2),(ymin+(ymax-ymin)/2),buf);
*/

/* сложные команды */
// цвет в исходное
setcolor_obj_num(menu_1,1,254,EGA_WHITE,123);
setcolor_obj_num(menu_1,1,255,EGA_DARKGRAY,123);
// цвет буквы в меню выбранной команды
setcolor_obj(menu_1,3,EGA_DARKGRAY,123);
if(komand != 0) {// цвет активный
   setcolor_obj_num(menu_1,3,komand,EGA_RED,123);
   setcolor_obj_num(menu_1,komand,100,EGA_DARKGRAY,123);
   setcolor_obj_num(menu_1,komand,200,EGA_WHITE,123);
} // if

sprintf(buf,"%2.2d",52-kf_size);
outtext_obj_num(menu_1,3,120,col_fon1_mn1,buf,SHOW);


/* тип текущего элемента */
switch(pic.GetTip())
{
case OBJ_line:
      sprintf(buf,"L");
     break;
case OBJ_rect:
      sprintf(buf,"R");
     break;
case OBJ_bar:
      sprintf(buf,"B");
     break;
case OBJ_elip:
      sprintf(buf,"E");
     break;
case OBJ_felip:
      sprintf(buf,"Q");
     break;
case OBJ_txt:
      sprintf(buf,"T");
     break;
case OBJ_fld:
      sprintf(buf,"Z");
     break;
case OBJ_bkgr:
      sprintf(buf,"F");
     break;
case OBJ_esc:
      sprintf(buf,"M");
     break;
case OBJ_reper:
      sprintf(buf,"r");
     break;
case OBJ_pipeline:
      sprintf(buf,"P");
     break;
default:
 sprintf(buf,"?");
break;
};/* switch((*(wr+PN))->tipobj) */
outtext_obj_num(menu_1,10,21,col_fon3_mn1,buf,SHOW);/* тип текущего элемента */

/* тип графического элемента для поиска */
switch(pck.tip_search)
{
case OBJ_line:
      sprintf(buf,"L");
     break;
case OBJ_rect:
      sprintf(buf,"R");
     break;
case OBJ_bar:
      sprintf(buf,"B");
     break;
case OBJ_elip:
      sprintf(buf,"E");
     break;
case OBJ_felip:
      sprintf(buf,"Q");
     break;
case OBJ_txt:
      sprintf(buf,"T");
     break;
case OBJ_fld:
      sprintf(buf,"Z");
     break;
case OBJ_bkgr:
      sprintf(buf,"F");
     break;
case OBJ_esc:
      sprintf(buf,"M");
     break;
case OBJ_reper:
      sprintf(buf,"r");
     break;
case OBJ_pipeline:
      sprintf(buf,"P");
     break;
default:
 sprintf(buf,"?");
break;
};/* switch*/
outtext_obj_num(menu_1,10,15,col_fon2_mn1,buf,SHOW);/* заданный тип элемента */

sprintf(buf,"%d",pic.Get_NO_CGE());
outtext_obj_num(menu_1,10,23,col_fon3_mn1,buf,SHOW);/* номер обьекта */
sprintf(buf,"%d",pic.Get_LN_CGE());
outtext_obj_num(menu_1,10,24,col_fon3_mn1,buf,SHOW);/* логический номер */

sprintf(buf,"%d",pic.GetPN());/* номер текущего графического элемента */
outtext_obj_num(menu_1,10,26,col_fon5_mn1,buf,SHOW);

sprintf(buf,"%d",pic.GetPN_TOP());/* всего графических элементов */
outtext_obj_num(menu_1,10,27,col_fon5_mn1,buf,SHOW);

/* всего логических элементов */
sprintf(buf,"%d",pic.GetMaxLE());
outtext_obj_num(menu_1,10,25,col_fon4_mn1,buf,SHOW);

/* текущий логический элемент */
sprintf(buf,"%d",pic.GetMaxLE()-pic.GetCurLE()-1);
outtext_obj_num(menu_1,10,30,col_fon4_mn1,buf,SHOW);

outtext_obj_num(menu_1, 3, 50, EGA_LIGHTGRAY, pck.Mode_elip == KOM_elip ? "El" : "Fe", SHOW);
outtext_obj_num(menu_1, 3, 10, EGA_LIGHTGRAY, pck.Mode_line == KOM_line ? "Ln" : "P", SHOW);

indid_cur_fn();//текущий файл


return 0;

}/* oumenu */
/****************************************************************************/
void indid_cur_fn(void)// индикация теущего имени файла
{
 char buf[sizeof(pck.Fn_work_tbg)];

 //текущий файл
 sprintf(buf,"%s",pck.Fn_work_tbg);
 *strrchr(buf,'\.')='\0';/* расширение */
 outtext_obj_num(menu_1,5,2,getcolor_obj_num(menu_1,255,3),buf,SHOW);

}//  indid_cur_fn


