// red5_007.cpp

#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <conio.h>
#include <graphics.h>

#include "red5.h"

//-------------------------------------------------------------------------
char* LIST::GetText(void){if(PN<0) return 0;//список пуст
		    return list[PN]->get_txt();}
//-------------------------------------------------------------------------
MPClass* LIST::GetMPClass(void){
   if(PN < 0)
	   return 0;//список пуст
	return list[PN]->get_class();
}/* LIST::GetMPClass */
//-------------------------------------------------------------------------
void LIST::ShowCur(void){if(PN<0) return;//список пуст
		   list[PN]->Show();}
//-------------------------------------------------------------------------
int  LIST::GetPN(void){return PN;}
//-------------------------------------------------------------------------
int  LIST::GetPN_TOP(void){return PN_TOP;}
//-------------------------------------------------------------------------
int  LIST::GetMAX_BGE(void){return MAX_BGE;}
//-------------------------------------------------------------------------
void LIST::EndPN(void){if(PN>=0) PN=PN_TOP;}
//-------------------------------------------------------------------------
void LIST::StartPN(void){if(PN>=0) PN=0;}
//-------------------------------------------------------------------------
char LIST::GetTip(void){if(PN<0) return -1;//список пуст
		  return list[PN]->get_tip();}
//-------------------------------------------------------------------------
int  LIST::GetX(void) {if(PN<0) return 10;//список пуст
		  return DMx(long(list[PN]->get_x()));}
//-------------------------------------------------------------------------
int  LIST::GetY(void) {if(PN<0) return 10;//список пуст
		  return DMy(long(list[PN]->get_y()));}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int  LIST::GetX2(void) {if(PN<0) return 10;//список пуст
		  return DMx(long(list[PN]->get_x()) + long(list[PN]->get_dx()));}
int  LIST::GetY2(void) {if(PN<0) return 10;//список пуст
		  return DMy(long(list[PN]->get_y()) + long(list[PN]->get_dy()));}

void  LIST::SetX(int x) {if(PN<0) return;//список пуст
		  list[PN]->put_x(x);}
void  LIST::SetY(int y) {if(PN<0) return;//список пуст
		  list[PN]->put_y(y);}
void  LIST::SetX2(int x) {if(PN<0) return;//список пуст
		  list[PN]->put_dx(x);}
void  LIST::SetY2(int y) {if(PN<0) return;//список пуст
		  list[PN]->put_dy(y);}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     // добавление элемента в конец списка
void LIST::AddLE(unsigned char no,unsigned char ln)
{
 list[PN]->BGE_Add_LE(no,ln);
 SaveTest=ON;// признак необходимости сохранения
}
//-------------------------------------------------------------------------
     // удаление текущего логического элемента
void LIST::DelCLE(void)
{
 if(PN<0) return;//список пуст
 list[PN]->BGE_Del_CLE();
 SaveTest=ON;// признак необходимости сохранения
}
//-------------------------------------------------------------------------
     // максимальное число логических элементов
int  LIST::GetMaxLE(void){if(PN<0) return -1;//список пуст
		    return list[PN]->BGE_Get_Max_LE();}
//-------------------------------------------------------------------------
     // текущая позиция логического элемента
int  LIST::GetCurLE(void){if(PN<0) return -1;//список пуст
		    return list[PN]->BGE_Get_Cur_LE();}
//-------------------------------------------------------------------------
     // номер обьекта текущего графического элемента
int  LIST::Get_NO_CGE(void){if(PN<0) return -1;//список пуст
                 return list[PN]->BGE_Get_NO_CGE(); }
//-------------------------------------------------------------------------
     // логический номер текущего графического элемента
int  LIST::Get_LN_CGE(void){if(PN<0) return -1;//список пуст
	             return list[PN]->BGE_Get_LN_CGE(); }
//-------------------------------------------------------------------------
     // номер обьекта текущего графического элемента
void LIST::Set_NO_CGE(unsigned char sno){if(PN<0) return ;//список пуст
                list[PN]->BGE_Set_NO_CGE(sno); }
//-------------------------------------------------------------------------
     // логический номер текущего графического элемента
void LIST::Set_LN_CGE(unsigned char sln){if(PN<0) return ;//список пуст
                list[PN]->BGE_Set_LN_CGE(sln); }

//-------------------------------------------------------------------------
     // изменение NO
void LIST::DI_NO_CLE(int direct)
{
 if(PN<0) return ;//список пуст
 list[PN]->BGE_DI_NO_CLE(direct);
 SaveTest=ON;// признак необходимости сохранения
}
//-------------------------------------------------------------------------
     // изменение LN
void LIST::DI_LN_CLE(int direct)
{
 if(PN<0) return ;//список пуст
 list[PN]->BGE_DI_LN_CLE(direct);
 SaveTest=ON;// признак необходимости сохранения
}
//-------------------------------------------------------------------------
     // переход назад по списку LE
void LIST::Pred(void){if(PN<0) return ;//список пуст
		list[PN]->Pred();}
//-------------------------------------------------------------------------
     // переход вперед по списку LE
void LIST::Succ(void){if(PN<0) return ;//список пуст
		list[PN]->Succ();}
//-------------------------------------------------------------------------
void LIST::Search_NO_Pred(void){if(PN<0) return ;//список пуст
			  list[PN]->Search_NO_Pred();}//поиск предыдущего NO
//-------------------------------------------------------------------------
void LIST::Search_NO_Succ(void){if(PN<0) return ;//список пуст
			  list[PN]->Search_NO_Succ();}//поиск следующего  NO
//-------------------------------------------------------------------------
void LIST::Search_LN_Pred(void){if(PN<0) return ;//список пуст
			  list[PN]->Search_LN_Pred();}//поиск предыдущего LN
//-------------------------------------------------------------------------
void LIST::Search_LN_Succ(void){if(PN<0) return ;//список пуст
			  list[PN]->Search_LN_Succ();}//поиск следующего  LN
//-------------------------------------------------------------------------
int  LIST::Start_LLE(){if(PN<0) return -1;//список пуст
		 return list[PN]->Start_LLE();}// сдвинуть текущий в начало
//-------------------------------------------------------------------------
int  LIST::End_LLE()  {if(PN<0) return -1;//список пуст
		 return list[PN]->End_LLE();}  // сдвинуть текущий в конец
//-------------------------------------------------------------------------
