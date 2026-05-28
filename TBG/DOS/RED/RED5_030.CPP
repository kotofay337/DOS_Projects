// red5_030.cpp

#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <conio.h>
#include <graphics.h>

#include "red5.h"

//-----------------------------------------------------------------------
//методы списка логических элементов

void BaseGraphElement::ClearLLE(void){LL.ListLogic_Clear();};// удаление списка LLE

//void BaseGraphElement::ListLogic_PRINT(void){LL.ListLogic_Print();};
     // добавление элемента в конец списка
void BaseGraphElement::BGE_Add_LE (unsigned char no,unsigned char ln){ LL.ListLogic_Add(no,ln);};
//void BaseGraphElement::BGE_Add_LE (void){ LL.ListLogic_Add(0,0); };
     // удаление текущего логического элемента
void BaseGraphElement::BGE_Del_CLE(void){ LL.ListLogic_Del_Current(); };
     // максимальное число логических элементов
int  BaseGraphElement::BGE_Get_Max_LE(void){ return LL.ListLogic_Get_Max_LE(); };
     //сохранить список ЛЭ в файле
void BaseGraphElement::BGE_Save_LLE(FILE *inf){LL.Save_LLE(inf);};
     //загрузить список ЛЭ из файла
void BaseGraphElement::BGE_Load_LLE(FILE *inf,char maxlle){LL.Load_LLE(inf,maxlle);};


     // текущая позиция логического элемента
int  BaseGraphElement::BGE_Get_Cur_LE(void){ return LL.ListLogic_Get_Cur_LE(); };

int  BaseGraphElement::BGE_Get_NO_CGE(void){ return LL.ListLogic_Get_NO(); };
int  BaseGraphElement::BGE_Get_LN_CGE(void){ return LL.ListLogic_Get_LN(); };
void BaseGraphElement::BGE_Set_NO_CGE(unsigned char sno){LL.ListLogic_Set_NO(sno);};
void BaseGraphElement::BGE_Set_LN_CGE(unsigned char sln){LL.ListLogic_Set_LN(sln);};


     // изменение NO
void BaseGraphElement::BGE_DI_NO_CLE(int direct){LL.ListLogic_di_NO(direct);};

     // изменение LN
void BaseGraphElement::BGE_DI_LN_CLE(int direct){LL.ListLogic_di_LN(direct);};

     // переход назад по списку LE
void BaseGraphElement::Pred(void){LL.ListLogic_Pred(); };

     // переход вперед по списку LE
void BaseGraphElement::Succ(void){LL.ListLogic_Succ(); };

void BaseGraphElement::Search_NO_Pred(void){LL.Search_NO_Pred();};//поиск предыдущего NO
void BaseGraphElement::Search_NO_Succ(void){LL.Search_NO_Succ();};//поиск следующего  NO

void BaseGraphElement::Search_LN_Pred(void){LL.Search_LN_Pred();};//поиск предыдущего LN
void BaseGraphElement::Search_LN_Succ(void){LL.Search_LN_Succ();};//поиск следующего  LN

int  BaseGraphElement::Check_NO(unsigned char CNO,int setcur)
		       {return LL.Check_NO(CNO,setcur);};

int  BaseGraphElement::Check_NOnot(unsigned char CNO,int setcur)
		       {return LL.Check_NOnot(CNO,setcur);};


int  BaseGraphElement::Check_LN(unsigned char CLN,int setcur)
		       {return LL.Check_LN(CLN,setcur);};

int  BaseGraphElement::Check_NO_LN(unsigned char CNO,unsigned char CLN,int setcur)
		       {return LL.Check_NO_LN(CNO,CLN,setcur);};

int  BaseGraphElement::Start_LLE(){return LL.Start_LLE();};// сдвинуть текущий в начало
int  BaseGraphElement::End_LLE()  {return LL.End_LLE();};  // сдвинуть текущий в конец

//-----------------------------------------------------------------------
