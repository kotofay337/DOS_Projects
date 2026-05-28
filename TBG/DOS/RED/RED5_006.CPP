
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
#include <alloc.h>
#include "red5.h"


//---------------------------------------------------------------------------
void LIST::DelGE(void) //удаление текущего ГЭ
{
 if(PN<0) return;//список пуст

 list[PN]->ClearLLE();// удаление списка LLE

 switch(list[PN]->get_tip()){
    case TIP_txt:
       list[PN]->ClearText();
       break;
    case TIP_esc:
       MPClass *tmpc = list[PN]->get_class();
       if(tmpc){
          // printf("-free: %p %u\n", tmpc, tmpc->nuse);
          MPCFree(tmpc);
       }/* if */
       break;
    default:
       break;
 }/* switch */

 delete list[PN];

 if(PN==PN_TOP){
    PN--;
 }/* if */
 else{// сдвиг обьектов влево
    for(int i=PN;i<PN_TOP;i++)
	    list[i]=list[i+1];
 }/* elif */
 PN_TOP--;
 SaveTest = ON;// признак необходимости сохранения
}// DelGE
//---------------------------------------------------------------------------
void LIST::DelGE_All(void)//удаление всех ГЭ
{
 if(PN<0) return;//список пуст

 int max=PN_TOP,index=0;// выполнение

 PN=PN_TOP;


 /* выполнение */
 outpic_all(menu_5);
 short int xmin, ymin, xmax, ymax;
 getsize_obj_num_reset();
 getsize_obj_num(menu_5,1,1,&xmin,&ymin,&xmax,&ymax);
 setfillstyle(SOLID_FILL,EGA_LIGHTBLUE);


 while(PN_TOP >= 0)
 {
   DelGE();
   disable();// запрет прерываний
   if(max!=0)
   bar(xmin,ymin,xmin+
	  (int)((double)(xmax-xmin)*((double)index/(double)max)),
      ymax);// выполнение
   enable();// разрешение прерываний

   index++;// выполнение
 }//while
 SaveTest=OFF;// признак необходимости сохранения
}// DelGE_All
//---------------------------------------------------------------------------
void LIST::DelGE_Ram(void)//удаление ГЭ в рамке
{if(PN<0) return;//список пуст
 if(ente<=1) return;
 if(ind_select==OFF) return;
 int old=PN;
 int max=count_select;// выполнение
 int index=0;// выполнение

 /* выполнение */
 outpic_all(menu_5);
 short int xmin, ymin, xmax, ymax;
 getsize_obj_num_reset();
 getsize_obj_num(menu_5,1,1,&xmin,&ymin,&xmax,&ymax);
 setfillstyle(SOLID_FILL,EGA_LIGHTBLUE);


 for(int Index=0;Index <= PN_TOP;Index++)
    {
     PN=Index;
     if(list[Index]->GetSelect()==ON)
       {DelGE();Index=-1;
        if(old>PN) old--;
		// выполнение
       disable();// запрет прерываний
	   if(max!=0)
		bar(xmin,ymin,xmin+
			(int)((double)(xmax-xmin)*((double)index/(double)max)),
           ymax);
        enable();// разрешение прерываний
        index++;// выполнение
       }//if
    }//for
    PN=old;

}// DelGE_Ram
//---------------------------------------------------------------------------
void LIST::Set_kf_size(void)//установка масштаба по тексту
{
 if(PN<0) return;//список пуст

 list[PN]->Set_kf_size();
}// Set_kf_size
//---------------------------------------------------------------------------
