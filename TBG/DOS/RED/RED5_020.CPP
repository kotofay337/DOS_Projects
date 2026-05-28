// red5_020.cpp

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bios.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <conio.h>
#include <graphics.h>
#include "red5.h"

/* ----------------------------------------------------------------------- */
void LIST::SetPipeLineColor(int c){
 if(PN<0) return;//список пуст
 if(list[PN]->get_tip() == TIP_pipeline){
    list[PN]->SetPipelineColor(c);
 }/* if */
}/* SetPipeLineColor */
//---------------------------------------------------------------------------
void LIST::Vibor  (void)//определение элементов попавших в рамку
{
 if(PN<0) return;//список пуст
// if(ind_select=OFF) return;//ни одного элемента в рамке
 LE_TOP_Ram=-1;   // число логических элементов в рамке

 for(int Index=0;Index <= PN_TOP;Index++)
    {
     list[Index]->Vibor();
     if(list[Index]->GetSelect() == ON) {
        LE_TOP_Ram+=(list[Index]->BGE_Get_Max_LE()+1);
     }// if
    }// for

}// Vibor
//---------------------------------------------------------------------------
void LIST::MoveRam(void)//перемещение элементов попавших в рамку
{
 if(PN<0) return;//список пуст
 if(ind_select==OFF) return;//ни одного элемента в рамке

 for(int Index=0;Index <= PN_TOP;Index++)
 {
     if((list[Index]->GetSelect())==ON) list[Index]->Move();
 }// for
 SaveTest=ON;// признак необходимости сохранения
}// MoveRam
//---------------------------------------------------------------------------
void LIST::MoveGE(void)//перемещение текущего ГЭ
{
 if(PN<0) return;//список пуст

 list[PN]->put_x(mmx(x_mouse));
 list[PN]->put_y(mmy(y_mouse));

 SaveTest=ON;// признак необходимости сохранения
}// MoveGE
//---------------------------------------------------------------------------
int  LIST::MashRam(void)//масштабирование элементов попавших в рамку
{
 if(PN < 0) {Warning("Нет элементов");return -1;}//список пуст
 if(ente <= 1){Warning("Нет рамки");return -1;} // if
 if(ind_select == OFF){
    Warning("В рамке нет элементов");return -1;
 }//ни одного элемента в рамке

 for(int Index=0;Index <= PN_TOP;Index++)
    {
     if((list[Index]->GetSelect())==ON) list[Index]->Mash();
    }// for
 SaveTest=ON;// признак необходимости сохранения
 return 0;
}// MashRam
//---------------------------------------------------------------------------
void LIST::ShowAll(void) //перевывод всего изображения
{
 setbkcolor(EGA_BLACK);//очистка возможно установленного фона
 if(PN<0) return;//список пуст


 for(int Index=0;Index <= PN_TOP;Index++)
    {
     list[Index]->Show();
     if(kbhit()!=0) if(getch()==033) return;
    }
}// ShowAll
//-----------------------------------------------------------------------
void LIST::ShowNO(void) //перевывод обьекта NO
{
 setbkcolor(EGA_BLACK);//очистка возможно установленного фона
 if(PN<0) return;//список пуст

 int num_NO=Get_NO_CGE();// текущий NO CGE

 for(int Index=0;Index <= PN_TOP;Index++)
    {
     if(list[Index]->Check_NO(num_NO,NOSET)==YES) list[Index]->Show();
    }
}// ShowNO
//-----------------------------------------------------------------------
void LIST::ShowLN(void) //перевывод LN обьекта
{
 setbkcolor(EGA_BLACK);//очистка возможно установленного фона
 if(PN<0) return;//список пуст

 int num_NO=Get_NO_CGE();// текущий NO CGE
 int num_LN=Get_LN_CGE();// текущий LN CGE

 for(int Index=0;Index <= PN_TOP;Index++)
    {
     if(list[Index]->Check_NO_LN(num_NO,num_LN,NOSET)==YES)
	     list[Index]->Show();
    }
}// ShowLN
//-----------------------------------------------------------------------
void LIST::ShowPipe(void) //перевывод LN обьекта
{
   if(PN<0) return;//список пуст

   if(GetMaxLE() >= 0){
      int num_NO=Get_NO_CGE();// текущий NO CGE
      int num_LN=Get_LN_CGE();// текущий LN CGE
      for(int Index=0;Index <= PN_TOP;Index++){
         if(list[Index]->Check_NO_LN(num_NO,num_LN,NOSET)==YES)
            if(list[Index]->get_tip() == TIP_pipeline)
               list[Index]->Show();
      }/* for */
   }/* if */
   else{
      if(list[PN]->get_tip() == TIP_pipeline)
         list[PN]->Show();
   }/* elif */
}// ShowPipe
//-----------------------------------------------------------------------
void LIST::SetOld (void)//восстановление прежних параметров при масштабировании
{
 if(PN<0) return;//список пуст

 for(int Index=0;Index <= PN_TOP;Index++)
    {
     if((list[Index]->GetSelect())==ON)list[Index]->set_old();
    }
}// SetOld
//-----------------------------------------------------------------------
void LIST::ClearOld(void)//очистка всех значений выбора
{
 if(PN<0) return;//список пуст

 for(int Index=0;Index <= PN_TOP;Index++)
    {
     list[Index]->clear_old();
    }

}// ClearOld
//-----------------------------------------------------------------------
void LIST::IndicNO(void) //индикация NO
{
 if(PN<0) return;//список пуст

 int num_NO=Get_NO_CGE();// текущий NO CGE


 for(int Index=0;Index <= PN_TOP;Index++)
    {
     if(list[Index]->Check_NO(num_NO,NOSET)==YES)
       {
          list[Index]->put_col(list[Index]->get_col()+1);
          if(list[Index]->get_tip() == TIP_esc){
             list[Index]->put_oldstate(list[Index]->get_state());
             list[Index]->put_state(2);
          }/* if */
          list[Index]->Show();
       }//if
    }//for
     if(mn1_show==ON)
       {
	outpic_obj_num(menu_mess,255,1);//стираем предыдущее пояснение
	outpic_obj_num(menu_mess,5,1);//новое
       }/* if */

     press_key();

 for(Index=0;Index <= PN_TOP;Index++)
    {
     if(list[Index]->Check_NO(num_NO,NOSET)==YES)
       {
          list[Index]->put_col(list[Index]->get_col()-1);
          if(list[Index]->get_tip() == TIP_esc){
             list[Index]->put_state(3);
          }/* if */
          list[Index]->Show();
          if(list[Index]->get_tip() == TIP_esc){
             list[Index]->put_state(list[Index]->get_oldstate());
          }/* if */
       }//if
    }//for



}// IndicNO
//-----------------------------------------------------------------------
void LIST::IndicLN(void) //индикация LN
{
 if(PN<0) return;//список пуст

 int num_NO=Get_NO_CGE();// текущий NO CGE
 int num_LN=Get_LN_CGE();// текущий LN CGE


 for(int Index=0;Index <= PN_TOP;Index++)
    {
     if(list[Index]->Check_NO_LN(num_NO,num_LN,NOSET)==YES)
       {
          list[Index]->put_col(list[Index]->get_col()+1);
          if(list[Index]->get_tip() == TIP_esc){
             list[Index]->put_oldstate(list[Index]->get_state());
             list[Index]->put_state(2);
          }/* if */
          list[Index]->Show();
       }//if
    }//for
     if(mn1_show==ON)
       {
	       outpic_obj_num(menu_mess,255,1);//стираем предыдущее пояснение
	       outpic_obj_num(menu_mess,5,1);//новое
       }/* if */

     press_key();

 for(Index=0;Index <= PN_TOP;Index++)
    {
     if(list[Index]->Check_NO_LN(num_NO,num_LN,NOSET)==YES)
       {
	       list[Index]->put_col(list[Index]->get_col()-1);
          if(list[Index]->get_tip() == TIP_esc){
             list[Index]->put_state(3);
          }/* if */
	       list[Index]->Show();
          if(list[Index]->get_tip() == TIP_esc){
             list[Index]->put_state(list[Index]->get_oldstate());
          }/* if */
       }//if
    }//for




}// IndicLN
//-----------------------------------------------------------------------
void LIST::IndicGE(void) //индикация текущего ГЭ
{

   if(PN<0)
	   return;//список пуст


   if(list[PN]->get_tip() == TIP_esc){
      list[PN]->put_oldstate(list[PN]->get_state());
      list[PN]->put_state(2);
   }/* if */
   else{
      if(list[PN]->get_tip() != TIP_pipeline)
         list[PN]->put_col(list[PN]->get_col()+1);
   }/* elif */

   list[PN]->Show();

   if(mn1_show==ON){
      outpic_obj_num(menu_mess,255,1);//стираем предыдущее пояснение
      outpic_obj_num(menu_mess,5,1);//новое
   }/* if */

   if(list[PN]->get_tip() == TIP_pipeline){
      clock_t c;
      short int l,r,tst;

      tst=0;
      c = clock();
      while(tst==0){
       if(bioskey(1)!=0)
          tst=bioskey(0);
       mouse_lpk(&l,&r);/* какие кнопки были нажаты */
       if((l!=0)||(r!=0))
	       break;
       if((clock() - c) > (CLK_TCK / 20)){
          list[PN]->Show();
          c = clock();
       }/* if */
      };/* while */
   }/* if */
   else{
      press_key();
   }/* elif */


   if(list[PN]->get_tip() == TIP_esc){
      list[PN]->put_state(3);
   }/* if */
   else{
      if(list[PN]->get_tip() != TIP_pipeline)
         list[PN]->put_col(list[PN]->get_col()-1);
   }/* elif */

   list[PN]->Show();

   if(list[PN]->get_tip() == TIP_esc){
      list[PN]->put_state(list[PN]->get_oldstate());
   }/* if */

}// IndicGE
//-----------------------------------------------------------------------