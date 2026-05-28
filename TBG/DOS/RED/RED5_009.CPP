#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <conio.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"

//--------------------------------------------------------------------
void LIST::CopyOne(int src,int dst) //копирование текущего ГЭ
{
 if(PN < 0) return;//нет графических элементов

// printf("in CopyOne\n");

 PN=dst;

 CreateGE(list[src]->get_tip());//создаем ГЭ типа источника

 //копируем
 if(list[src]->GetSelect()==ON)
   {
    list[PN]->put_x((list[src]->get_oldx()-rx_min) + mmx(x_mouse));
    list[PN]->put_y((list[src]->get_oldy()-ry_min) + mmy(y_mouse));
   }
   else
   {
    list[PN]->put_x(mmx(x_mouse));
    list[PN]->put_y(mmy(y_mouse));
   };//if

 list[PN]->put_dx  (list[src]->get_dx());
 list[PN]->put_dy  (list[src]->get_dy());
 list[PN]->put_fi1 (list[src]->get_fi1());
 list[PN]->put_fi2 (list[src]->get_fi2());
 list[PN]->put_stl (list[src]->get_stl());
 list[PN]->put_thi (list[src]->get_thi());
 list[PN]->put_col (list[src]->get_col());
 list[PN]->put_col2 (list[src]->get_col2());
 list[PN]->put_brd (list[src]->get_brd());
 list[PN]->put_dir (list[src]->get_dir());
 list[PN]->put_hor (list[src]->get_hor());
 list[PN]->put_ver (list[src]->get_ver());

 list[PN]->put_siz (list[src]->get_siz());
 list[PN]->put_mltx(list[src]->get_mltx());
 list[PN]->put_mlty(list[src]->get_mlty());
 list[PN]->put_sda (list[src]->get_sda ());
 list[PN]->put_txt (list[src]->get_txt());

 list[PN]->put_index(list[src]->get_index());
 list[PN]->put_state(list[src]->get_state());
 list[PN]->put_class(MPCLoad(list[src]->get_index()));

 // logic copy !!!
 list[src]->Start_LLE();

 for(int maxlle = 0;maxlle <= list[src]->BGE_Get_Max_LE();maxlle++, list[src]->Succ()){
    int tno = list[src]->BGE_Get_NO_CGE(),
	     tln = list[src]->BGE_Get_LN_CGE();
    if((peek(0,0x417) & 0x08) != 0){
       tno++;
    }/* if */
    list[PN]->BGE_Add_LE (tno, tln); // ADD !
 }/* for */

// printf("out CopyOne\n");
// PN=dst;
};// CopyOne
//--------------------------------------------------------------------
void LIST::CopyOneMirrorX(int src,int dst) //копирование текущего ГЭ
{//относительно вертикальной оси
 if(PN < 0) return;//нет графических элементов

 PN=dst;

 CreateGE(list[src]->get_tip());//создаем ГЭ типа источника

 //копируем
 int x = 2 * rx_max - list[src]->get_oldx();

 list[PN]->put_x(x);

 list[PN]->put_y( list[src]->get_oldy());

 if(list[src]->get_tip() != TIP_elip){
    list[PN]->put_dx  (-list[src]->get_dx());
    list[PN]->put_dy  (list[src]->get_dy());
 }// if
 else{
    list[PN]->put_dx  (list[src]->get_dx());
    list[PN]->put_dy  (list[src]->get_dy());
    list[PN]->put_fi1 (180 - list[src]->get_fi2());
    list[PN]->put_fi2 (180 - list[src]->get_fi1());
 }// elif

 list[PN]->put_stl (list[src]->get_stl());
 list[PN]->put_thi (list[src]->get_thi());
 list[PN]->put_col (list[src]->get_col());
 list[PN]->put_col2 (list[src]->get_col2());
 list[PN]->put_brd (list[src]->get_brd());
 list[PN]->put_dir (list[src]->get_dir());

 auto int hj;

 switch(list[src]->get_hor()){
    case LEFT_TEXT:
       hj = RIGHT_TEXT;
       break;
    case RIGHT_TEXT:
       hj = LEFT_TEXT;
       break;
    case CENTER_TEXT:
       hj = CENTER_TEXT;
       break;
 }// switch

 list[PN]->put_hor (hj);

 list[PN]->put_ver (list[src]->get_ver());

 list[PN]->put_siz (list[src]->get_siz());
 list[PN]->put_mltx(list[src]->get_mltx());
 list[PN]->put_mlty(list[src]->get_mlty());
 list[PN]->put_sda (list[src]->get_sda ());
 list[PN]->put_txt (list[src]->get_txt());
 list[PN]->put_index(list[src]->get_index());
 list[PN]->put_state(list[src]->get_state());
 list[PN]->put_class(MPCLoad(list[src]->get_index()));

 // logic copy !!!
 list[src]->Start_LLE();

 for(int maxlle = 0;maxlle <= list[src]->BGE_Get_Max_LE();maxlle++, list[src]->Succ()){
    int tno = list[src]->BGE_Get_NO_CGE(),
	     tln = list[src]->BGE_Get_LN_CGE();
    list[PN]->BGE_Add_LE (tno, tln); // ADD !
 }/* for */

};// CopyOneMirrorX
//--------------------------------------------------------------------
void LIST::CopyOneMirrorY(int src,int dst) //копирование текущего ГЭ
{//относительно горизонтальной оси
 if(PN < 0) return;//нет графических элементов

 PN=dst;

 CreateGE(list[src]->get_tip());//создаем ГЭ типа источника

 //копируем
 int y = 2 * ry_max - list[src]->get_oldy();

 list[PN]->put_y(y);

 list[PN]->put_x( list[src]->get_oldx());

 if(list[src]->get_tip() != TIP_elip){
    list[PN]->put_dx  (list[src]->get_dx());
    list[PN]->put_dy  (-list[src]->get_dy());
 }// if
 else{
    list[PN]->put_dx  (list[src]->get_dx());
    list[PN]->put_dy  (list[src]->get_dy());
    list[PN]->put_fi1 (-list[src]->get_fi2());
    list[PN]->put_fi2 (-list[src]->get_fi1());
 }// elif

 list[PN]->put_stl (list[src]->get_stl());
 list[PN]->put_thi (list[src]->get_thi());
 list[PN]->put_col (list[src]->get_col());
 list[PN]->put_col2 (list[src]->get_col2());
 list[PN]->put_brd (list[src]->get_brd());
 list[PN]->put_dir (list[src]->get_dir());

 auto int vj;

 switch(list[src]->get_ver()){
    case BOTTOM_TEXT:
       vj = TOP_TEXT;
       break;
    case TOP_TEXT:
       vj = BOTTOM_TEXT;
       break;
    case CENTER_TEXT:
       vj = CENTER_TEXT;
       break;
 }// switch

 list[PN]->put_ver (vj);
 list[PN]->put_hor (list[src]->get_hor());

 list[PN]->put_siz (list[src]->get_siz());
 list[PN]->put_mltx(list[src]->get_mltx());
 list[PN]->put_mlty(list[src]->get_mlty());
 list[PN]->put_sda (list[src]->get_sda ());
 list[PN]->put_txt (list[src]->get_txt());
 list[PN]->put_index(list[src]->get_index());
 list[PN]->put_state(list[src]->get_state());
 list[PN]->put_class(MPCLoad(list[src]->get_index()));

 // logic copy !!!
 list[src]->Start_LLE();

 for(int maxlle = 0;maxlle <= list[src]->BGE_Get_Max_LE();maxlle++, list[src]->Succ()){
    int tno = list[src]->BGE_Get_NO_CGE(),
	     tln = list[src]->BGE_Get_LN_CGE();
    list[PN]->BGE_Add_LE (tno, tln); // ADD !
 }/* for */

};// CopyOneMirrorY
//--------------------------------------------------------------------
void LIST::CopyRam(void){//копирование элементов попавших в рамку

   if(PN < 0) return;//нет графических элементов

   int index=count_select;

   if((peek(0,0x417) & 0x07) == 0){
      for(int Index=0;Index <= PN_TOP;Index++){
         if((list[Index]->GetSelect())==ON){
		      /* не нажат Shift */
	         CopyOne(Index,PN_TOP);
	         if(--index<0){
               break;//все выбранные ГЭ закончились
            }// if
         }//if
      }//for
   }// if
   else{
      if((peek(0,0x417) & 0x04) != 0){// Ctrl (любой)
         for(int Index=0;Index <= PN_TOP;Index++){
            if((list[Index]->GetSelect())==ON){
		         /* нажат Ctrl */
	            CopyOneMirrorY(Index,PN_TOP);
	            if(--index<0){
                  break;//все выбранные ГЭ закончились
               }// if
            }//if
         }//for
      }// if
      if((peek(0,0x417) & 0x02) != 0){//
         for(int Index=0;Index <= PN_TOP;Index++){
            if((list[Index]->GetSelect())==ON){
		         /* нажат Shift */
	            CopyOneMirrorX(Index,PN_TOP);
	            if(--index<0){
                  break;//все выбранные ГЭ закончились
               }// if
            }//if
         }//for
      }// if
   }// elif

};// CopyRam
//--------------------------------------------------------------------
void LIST::CopyGE(void) //копирование текущего ГЭ
{
 if(PN < 0) return;//нет графических элементов
 CopyOne(PN,PN_TOP);
};// CopyGE
//--------------------------------------------------------------------
void LIST::Add_LE_to_GE_in_ramca_from_current_GE(void){

   if(PN < 0) return;//нет графических элементов

   char cobj = list[PN]->BGE_Get_NO_CGE(),
        cnum = list[PN]->BGE_Get_LN_CGE();
   int  index = count_select;

   for(int Index = 0;Index <= PN_TOP;Index++){
      if((list[Index]->GetSelect()) == ON){
         if(list[Index]->Check_NO_LN(cobj, cnum, NOSET) == NOT)
            list[Index]->BGE_Add_LE(cobj, cnum);
	      if(--index < 0){
            break;//все выбранные ГЭ закончились
         }// if
      }//if
   }//for

   ibell();

};// Add_LE_to_GE_in_ramca_from_current_GE
//--------------------------------------------------------------------
void LIST::Del_LE_to_GE_in_ramca(void){

   if(PN < 0) return;//нет графических элементов

   int  index = count_select;

   for(int Index = 0;Index <= PN_TOP;Index++){
      if((list[Index]->GetSelect()) == ON){
         list[Index]->ClearLLE();
	      if(--index < 0){
            break;//все выбранные ГЭ закончились
         }// if
      }// if
   }// for

   ibell();
};// Del_LE_to_GE_in_ramca
//--------------------------------------------------------------------
