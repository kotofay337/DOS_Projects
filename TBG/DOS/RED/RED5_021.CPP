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

//-----------------------------------------------------------------------
void LIST::IncPN(void)
{
 if(PN < 0) return;//нет графических элементов
 if(PN+1 > PN_TOP) return;//уже конец списка
 int old_PN=PN;//сохраняем для использования в случае неуспешного поиска

 if(ind_select==ON)
   {
    for(int Index=PN+1;Index<=PN_TOP;Index++)
    {
     PN=Index;
     if((list[PN]->GetSelect())==ON) return;
    }//for
    PN=old_PN;// не нашли
   }
   else if(++PN > PN_TOP) PN = PN_TOP;

}//IncPN
//-----------------------------------------------------------------------
void LIST::DecPN(void)
{
 if(PN < 0) return;//нет графических элементов
 if(PN-1<0)return;//уже конец списка
 int old_PN=PN;//сохраняем для использования в случае неуспешного поиска

 if(ind_select==ON)
   {
    for(int Index=PN-1;Index>=0;Index--)
    {
     PN=Index;
     if((list[PN]->GetSelect())==ON) return;
    }//for
    PN=old_PN;// не нашли
   }
   else if(--PN < 0) PN = 0;

}//DecPN
//-----------------------------------------------------------------------
void LIST::Search_NO_Pred_LGE(void)//поиск предыдущего NO в списке CGE
{
      if(PN < 0) return;//нет графических элементов
      if(PN-1<0) {ibell();return;}//уже конец списка

      int old_PN=PN;//сохраняем для использования в случае неуспешного поиска

      int num_NO=Get_NO_CGE();// текущий NO CGE

      for(int Index=PN-1;Index>=0;Index--)
      {
       PN=Index;
//       printf("%d\n",Index);
       if(ind_select==ON)
	 {
	  if((list[Index]->GetSelect())==ON)
	    if(((list[Index]->Check_NOnot(num_NO,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO
	 }
	 else
	    if(((list[Index]->Check_NOnot(num_NO,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO

//      PN=old_PN;// не нашли
//      ibell();
      }//for
  PN=old_PN;// не нашли
  ibell();

}//  Search_NO_Pred_LGE
//---------------------------------------------------------------------------
void LIST::Search_NO_Succ_LGE(void)//поиск следующего  NO в списке CGE
     {
      if(PN < 0)         return;//нет графических элементов
      if(PN+1 > PN_TOP) {ibell();return;}//уже конец списка

      int old_PN=PN;//сохраняем для использования в случае неуспешного поиска

      int num_NO=Get_NO_CGE();// текущий NO CGE

      for(int Index=PN+1;Index<=PN_TOP;Index++)
      {
       PN=Index;
       if(ind_select==ON)
	 {
	  if((list[Index]->GetSelect())==ON)
	    if(((list[Index]->Check_NOnot(num_NO,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO
	 }
	 else
	    if(((list[Index]->Check_NOnot(num_NO,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO

      }//for
  PN=old_PN;// не нашли
  ibell();
}//  Search_NO_Succ_LGE
//---------------------------------------------------------------------------
void LIST::Search_LN_Pred_LGE(void)//поиск предыдущего LN в списке CGE
     {
      if(PN < 0) return;//нет графических элементов
      if(PN-1<0) {ibell();return;}//уже конец списка

      int old_PN=PN;//сохраняем для использования в случае неуспешного поиска

      int num_NO=Get_NO_CGE();// текущий NO CGE

      for(int Index=PN-1;Index>=0;Index--)
      {
       PN=Index;
       if(ind_select==ON)
	 {
	  if((list[Index]->GetSelect())==ON)
	    if(((list[Index]->Check_NO(num_NO,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO
	 }
	 else
	    if(((list[Index]->Check_NO(num_NO,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO

      }//for
  PN=old_PN;// не нашли
  ibell();
}//  Search_LN_Pred_LGE
//---------------------------------------------------------------------------
void LIST::Search_LN_Succ_LGE(void)//поиск следующего  LN в списке GE
     {
      if(PN < 0)         return;//нет графических элементов
      if(PN+1 > PN_TOP)  {ibell();return;}//уже конец списка

      int old_PN=PN;//сохраняем для использования в случае неуспешного поиска

      int num_NO=Get_NO_CGE();// текущий NO CGE

      for(int Index=PN+1;Index<=PN_TOP;Index++)
      {
       PN=Index;
       if(ind_select==ON)
	 {
	  if((list[Index]->GetSelect())==ON)
	    if(((list[Index]->Check_NO(num_NO,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO
	 }
	 else
	    if(((list[Index]->Check_NO(num_NO,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO

      }//for
      PN=old_PN;// не нашли
      ibell();
     }//  Search_LN_Succ_LGE
//---------------------------------------------------------------------------
     //поиск предыдущего LN равного теущему в списке CGE
void LIST::Search_All_LN_Pred_LGE(void)
     {
      if(PN < 0)  return;//нет графических элементов
      if(PN-1 <0) {ibell();return;}//уже конец списка

      int old_PN=PN;//сохраняем для использования в случае неуспешного поиска

      int num_LN=Get_LN_CGE();// текущий LN CGE

      for(int Index=PN-1;Index>=0;Index--)
      {
       PN=Index;
       if(ind_select==ON)
	 {
	  if((list[Index]->GetSelect())==ON)
	    if(((list[Index]->Check_LN(num_LN,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO
	 }
	 else
	    if(((list[Index]->Check_LN(num_LN,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO

      }//for
      PN=old_PN;// не нашли
      ibell();
     }//  Search_All_LN_Pred_LGE
//---------------------------------------------------------------------------
     //поиск следующего LN равного теущему в списке CGE
void LIST::Search_All_LN_Succ_LGE(void)
     {
      if(PN < 0)         return;//нет графических элементов
      if(PN+1 > PN_TOP)  {ibell();return;}//уже конец списка

      int old_PN=PN;//сохраняем для использования в случае неуспешного поиска

      int num_LN=Get_LN_CGE();// текущий LN CGE

      for(int Index=PN+1;Index<=PN_TOP;Index++)
      {
       PN=Index;
       if(ind_select==ON)
	 {
	  if((list[Index]->GetSelect())==ON)
	    if(((list[Index]->Check_LN(num_LN,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO
	 }
	 else
	    if(((list[Index]->Check_LN(num_LN,SET))==YES)&&(Get_NO_CGE()!=-1)) return; //нашли следующий NO

      }//for
      PN=old_PN;// не нашли
      ibell();
     }//  Search_All_LN_Succ_LGE
//---------------------------------------------------------------------------
void LIST::Search_Tip_Pred_LGE(void)//поиск предыдущего типа в списке GE
     {
      if(PN < 0) return;//нет графических элементов
      if(PN-1 <0){ibell();return;}//уже конец списка

      int old_PN=PN;//сохраняем для использования в случае неуспешного поиска

      for(int Index=PN-1;Index>=0;Index--)
      {
       PN=Index;
       if(ind_select==ON)
	 {
	  if((list[Index]->GetSelect())==ON)
            if(pck.tip_search==GetTip()) return; //нашли предыдущий заданный тип GE
	 }
	 else
            if(pck.tip_search==GetTip()) return; //нашли предыдущий заданный тип GE

      }//for
      PN=old_PN;// не нашли
      ibell();
}//  Search_Tip_Pred_LGE
//---------------------------------------------------------------------------
void LIST::Search_Tip_Succ_LGE(void)//поиск следующего  типа в списке GE
     {
      if(PN < 0)         return;//нет графических элементов
      if(PN+1 > PN_TOP) {ibell();return;}//уже конец списка

      int old_PN=PN;//сохраняем для использования в случае неуспешного поиска

      for(int Index=PN+1;Index<=PN_TOP;Index++)
      {
       PN=Index;
       if(ind_select==ON)
	 {
	  if((list[Index]->GetSelect())==ON)
            if(pck.tip_search==GetTip()) return; //нашли предыдущий заданный тип GE
	 }
	 else
            if(pck.tip_search==GetTip()) return; //нашли предыдущий заданный тип GE

      }//for
      PN=old_PN;// не нашли
      ibell();
}//  Search_Tip_Succ_LGE
//---------------------------------------------------------------------------
