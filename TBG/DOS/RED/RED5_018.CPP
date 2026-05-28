//  red5_018.c

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <dos.h>
#include <dir.h>
#include <math.h>
#include <conio.h>

#include "red5.h"

/**************************************************************************/
void indic_mess(void)//вывод сообщений
{
if((pos_ms_pred!=pos_ms)&&(mn1_show==ON))
  {//позиция мыша изменилась и меню есть

   char buf[80];
   char n[6];
   int num=1;

   switch(pos_ms)
   {
    case 0:
	 if(komand==OFF){
	    if(ente==1) num=2;
	    if(ente >1) num=3;
	   }
	   else{
		if(komand==KOM_line)   num=13;
		if(komand==KOM_pipeline)   num=13;
	    if(komand==KOM_rect)  num=14;
	    if(komand==KOM_bar )  num=15;
	    if(komand==KOM_elip)  num=16;
	    if(komand==KOM_felip)  num=16;
	    if(komand==KOM_txt )  num=17;
	    if(komand==KOM_fld )  num=18;
	    if(komand==KOM_bkgr)  num=19;
	    if(komand==KOM_copy)  num=10+pck.Mode_copy;
	    if(komand==KOM_move)  num=20+pck.Mode_move;
	    if(komand==KOM_raio)  num=214;
	    if(komand==KOM_macro) num=215;
	   };// if
    break;
    case 30:
	 num=pck.Mode_save;
    break;
    case 31:
	 num=pck.Mode_load;
    break;
    case 32:

     char tip=pic.GetTip();

     strcpy(buf,"Корректировка:");

	 if((tip==TIP_line)||
	    (tip==TIP_rect)||
	    (tip==TIP_elip)
       )
     {
        if(pck.Mode_korr_lre[mode_korr_stl_lre] == ON) strcat(buf," стиль");
        if(pck.Mode_korr_lre[mode_korr_thi_lre] == ON) strcat(buf," толщина");
        if(pck.Mode_korr_lre[mode_korr_col_lre] == ON) strcat(buf," цвет");

     }// if

     if(tip==TIP_fld)
     {
        if(pck.Mode_korr_bzf[mode_korr_stl_bzf] == ON) strcat(buf," стиль");
        if(pck.Mode_korr_bzf[mode_korr_brd_bzf] == ON) strcat(buf," цвет-границы");
        if(pck.Mode_korr_bzf[mode_korr_col_bzf] == ON) strcat(buf," цвет");

     }// if

     if(tip==TIP_bar)
     {
        if(pck.Mode_korr_bzf[mode_korr_stl_bzf] == ON) strcat(buf," стиль");
        if(pck.Mode_korr_bzf[mode_korr_col_bzf] == ON) strcat(buf," цвет");
	 }// if

     if(tip==TIP_bkgr)
     {
        if(pck.Mode_korr_bzf[mode_korr_col_bzf] == ON) strcat(buf," цвет фона");
     }// if

     if(tip==TIP_txt)
     {
        if(pck.Mode_korr_t[mode_korr_stl_t] == ON) strcat(buf," стиль");
        if(pck.Mode_korr_t[mode_korr_dir_t] == ON) strcat(buf," напр.");
        if(pck.Mode_korr_t[mode_korr_siz_t] == ON) strcat(buf," разм.");
        if(pck.Mode_korr_t[mode_korr_col_t] == ON) strcat(buf," цвет");
        if(pck.Mode_korr_t[mode_korr_txt_t] == ON) strcat(buf," содерж.");
     }// if

     if(tip==TIP_esc)
     {
        strcat(buf," макропримитива");
        sprintf(n, " %u", pck.mc_index);
        strcat(buf, n);
        sprintf(n, " %u", pck.mc_state);
        strcat(buf, n);
     }// if

    break;
    case 33:
	 num=pck.Mode_del;
    break;
    case 34:
	 num=pck.Mode_copy;
    break;
    case 35:
	 num=pck.Mode_move;
    break;
    case 37:
     num=pck.Mode_show;
    break;
    case 38:
     num=pck.Mode_indic;
    break;
   };// switch
mouse_off();
if(pos_ms!=32)
  {
   outpic_obj_num(menu_mess,255,1);//стираем предыдущее пояснение
      if(num != 215)
         outpic_obj_num(menu_mess,num,pos_ms);//новое
      else{
         strcpy(buf, "Рисуем макропримитив");
         sprintf(n, " %u", pck.mc_index);
         strcat(buf, n);
         sprintf(n, " %u", pck.mc_state);
         strcat(buf, n);
         outtext_obj_num(menu_mess,num,pos_ms,getcolor_obj_num(menu_mess,255,1),buf,SHOW);
      }/* elif */
  }
  else{
   outpic_obj_num(menu_mess,255,1);//стираем предыдущее пояснение
   outtext_obj_num(menu_mess,1,32,getcolor_obj_num(menu_mess,255,1),buf,SHOW);
//   outpic_obj_num(menu_mess,1,32);// новое
  };
mouse_on();

pos_ms_pred=pos_ms;

};/* if */


};// indic_mess
/**************************************************************************/
