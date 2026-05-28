
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bios.h>
#include <dos.h>
#include <dir.h>
#include <sys\stat.h>
#include <io.h>
#include <conio.h>
#include <alloc.h>
#include <graphics.h>
#include <keyboard.h>

#include "red5.h"

//-----------------------------------------------------------------------
ListLogic::ListLogic(){ Current=head=NULL; };//конструктор
//-----------------------------------------------------------------------
void  ListLogic::ListLogic_Del_Current(void)// удаление текущего элемента
		 { ListLogic_Del(Current->get_NO(),Current->get_LN()); };
//-----------------------------------------------------------------------
int   ListLogic::ListLogic_Get_NO(void)// NO текущего логического элемента
	      {
	       if(Current==NULL) return -1;
	       return Current->get_NO();};
//-----------------------------------------------------------------------
int   ListLogic::ListLogic_Get_LN(void)// LN текущего логического элемента
	      {
	       if(Current==NULL) return -1;
	       return Current->get_LN();};
//-----------------------------------------------------------------------
void  ListLogic::ListLogic_Set_NO(unsigned char sno)// NO текущего логического элемента
	      {
	       if(Current==NULL) return;
	       Current->set_NO(sno);
          SaveTest=ON;// были изменения
         }
//-----------------------------------------------------------------------
void  ListLogic::ListLogic_Set_LN(unsigned char sln)// LN текущего логического элемента
	      {
	       if(Current==NULL) return;
	       Current->set_LN(sln);
          SaveTest=ON;// были изменения
         }
//-----------------------------------------------------------------------
void ListLogic::Save_LLE(FILE *inf)//сохранить список ЛЭ в файле
{
       unsigned char no,ln;
//       int i;

       if(head==NULL) {return;};

       End_LLE();// сдвинуть текущий в конец
       do{
        no=Current->get_NO();
        ln=Current->get_LN();
        fwrite(&no,1,1,inf);
        fwrite(&ln,1,1,inf);

//        printf("%d %d %d \n",i,no,ln);
//        i++;

       }//while
       while(ListLogic_Pred()==YES);//список не кончился

};// Save_LLE
//-----------------------------------------------------------------------
void ListLogic::Load_LLE(FILE *inf,char maxlle) //загрузить список ЛЭ из файла
{
       unsigned char no,ln;
       int Index;

//       printf("maxlle%d\n",maxlle);getch();
       for(Index=0;Index<=maxlle;Index++)
       {
        fread(&no,1,1,inf);
        fread(&ln,1,1,inf);
        ListLogic_Add(no,ln);
//        printf("Load_LLE%d\n",Index);

       };//for

};// Load_LLE
//-----------------------------------------------------------------------
/*
//Печать на экране списка логических элементов
void  ListLogic::ListLogic_Print(void)
      {
       Logic *cur;
       char counter=0;

		// возможно эта ф-я изменится

       if(head==NULL) {//printf("ListLogic_Print:Список пуст.\n\007");
                       return;};
       cur=head;//начало
       while(1!=0){
       printf("NO=%u LN=%u\n",cur->get_NO(),cur->get_LN());
       if(cur->get_next()==NULL) break; //последний элемент распечатан
       cur=cur->get_next();
       counter++;
       };
//       printf("Print:--------- %d\n",counter);
      };//  ListLogic_Print
//-----------------------------------------------------------------------
//Печать на экране списка логических элементов
void  ListLogic::ListLogic_Print_Current(void)
      {
	   printf("Current: NO=%d,LN=%d,P=%p\n",Current->get_NO(),
											Current->get_LN(),Current);
      };//  ListLogic_Print
*/
//-----------------------------------------------------------------------
	  // добавление элемента в конец списка /по существу в начало/
void  ListLogic::ListLogic_Add(unsigned char AddNO,unsigned char AddLN)
      {
	   Logic *add;
	   char error[]={"ListLogic_Add:Мало памяти для элемента логики\n\007"};

	   if((add=new Logic(AddNO,AddLN,head))==NULL)
		 {printf("%s\n",error);return;};
	   head=add;
	   Current=head; // Текущим становится 1-й
// dgdgdgd
       LE_TOP++;   // число логических элементов в файле
	  };// ListLogic_Add
//-----------------------------------------------------------------------
	  // удаление элемента из конца списка /по существу из начала/
void  ListLogic::ListLogic_Del(void)
      {
	   Logic *del;

	   if(head==NULL) { printf("ListLogic_Del:Список пуст.\n");return;};//if
	   del=head;
	   head=del->get_next();
	   delete del;
	   Current=head; // Текущим становится 1-й
       LE_TOP--;   // число логических элементов в файле
	  };// ListLogic_Del
//---------------------------------------------------------------------------
	  // удаление элемента из списка по NO LN
void  ListLogic::ListLogic_Del(unsigned char DNO,unsigned char DLN)
{
	   Logic *cur,*pred;
	   if(head==NULL){
		  #ifdef DBG_ListLogic
			printf("ListLogic_Del_NO_LN: Список пуст.\n");
          #endif
          return;
         };
	   if((peek(0,0x417)&3)!=0)
		{ /* нажат любой Shift */

			cur=head;// Начнем с начала
			pred=cur;
			while(1!=0){
				if((cur->get_NO()==DNO)&&
				   (cur->get_LN()==DLN)){
					break;//Элемент найден - выйти из цикла
				};//if
				pred=cur;
				cur=cur->get_next();
				if(cur==NULL){
				  #ifdef DBG_ListLogic
					printf("ListLogic_Del: Элемент %d,%d отсутствует.\n\007",DNO,DLN);
				  #endif
					return;
				};//нет элемента с LN NO
			};//while

			if(cur==pred){// Ура! Найден 1-й элемент в списке !
				head=head->get_next(); //Перенести из 1 элемента адрес 2-го
									   //в head и можно удалять.
				delete cur;//удалить 1 элемент
		 		Current=head; // Текущим становится 1-й
                LE_TOP--;   // число логических элементов в файле
		 		return;
			};// if удаляем 1 элемент

			pred->set_next(cur->get_next());
			delete cur;
			Current=head;// Текущим становится 1-й
            LE_TOP--;   // число логических элементов в файле
			return;
	   } /* нажат любой Shift */
	   else{
			Warning("При нажатом Shift");
	   };
};// ListLogic_Del NO LN
//---------------------------------------------------------------------------
int  ListLogic::ListLogic_Get_Max_LE(void)// максимальное число логических элементов
{
 Logic *cur;
 int max=-1;

 cur=head;
 while(cur!=NULL)
 {
  cur=cur->get_next();
  max++;
 };//while
 return max;
};// ListLogic_Get_Max_LE
//---------------------------------------------------------------------------
     // текущая позиция логического элемента
int  ListLogic::ListLogic_Get_Cur_LE(void)
{
 Logic *cur;
 int count=-1;

 cur=head;
 while(cur!=Current)
 {
  cur=cur->get_next();
  count++;
 };//while
 return count;
};// ListLogic_Get_Cur_LE
//---------------------------------------------------------------------------
	  // проверить наличие в списке NO и установить туда Current/если надо/
	  // YES если найден,иначе NOT.
	  // SET-установить Current
	  // NOSET- не устанавливать Current
int  ListLogic::Check_NO(unsigned char CNO,int setcur)
{
	   if(head==NULL) return NOT;

	   Logic *ch;
	   int tst=0;

	   ch=head;// Начнем с начала
	   while(ch!=NULL)
	   {
	    if((ch->get_NO())==CNO )
	      {
	       tst=1;break;
	      };// if
	    ch=ch->get_next();
	   };//while

	   if(tst==0)
		 {return NOT;}
	   else
		 {if(setcur==SET) Current=ch;//надо устанавливать
		  return YES;
		 };
};//  Check_NO
//---------------------------------------------------------------------------
	  // проверить есть ли в списке NO отличающиеся от заданного и установить туда Current/если надо/
	  // YES если найден,иначе NOT.
	  // SET-установить Current
	  // NOSET- не устанавливать Current
int ListLogic::Check_NOnot(unsigned char CNO,int setcur)
{
	   if(head==NULL) return NOT;

	   Logic *ch;
	   int tst=0;

	   ch=head;// Начнем с начала
	   while(ch!=NULL)
	   {
	    if((ch->get_NO())!=CNO )
	      {
	       tst=1;break;
	      };// if
	    ch=ch->get_next();
	   };//while

	   if(tst==0)
		 {return NOT;}
	   else
		 {if(setcur==SET) Current=ch;//надо устанавливать
		  return YES;
		 };

};//Check_NOnot

//---------------------------------------------------------------------------
	  // проверить наличие в списке LN и установить туда Current/если надо/
	  // YES если найден,иначе NOT.
	  // SET-установить Current
	  // NOSET- не устанавливать Current
int  ListLogic::Check_LN(unsigned char CLN,int setcur)
{
	   if(head==NULL) return NOT;

	   Logic *ch;
	   int tst=0;

	   ch=head;// Начнем с начала
	   while(ch!=NULL) {
			 if((ch->get_LN())==CLN )
				{
				 tst=1;break;
				};// if
			 ch=ch->get_next();
	   };//while

	   if(tst==0)
		 {return NOT;}
	   else
		 {if(setcur==SET) Current=ch;//надо устанавливать
		  return YES;
		 };
};//  Check_LN
//---------------------------------------------------------------------------
	  // проверить наличие в списке NO и LN и установить туда Current/если надо/
	  // YES если найден,иначе NOT.
	  // SET-установить Current
	  // NOSET- не устанавливать Current
int   ListLogic::Check_NO_LN(unsigned char CNO,unsigned char CLN,int setcur)
{
	   if(head==NULL) return NOT;

	   Logic *ch;
	   int tst=0;

	   ch=head;// Начнем с начала
	   while(ch!=NULL)
	   {
	    if(((ch->get_NO())==CNO)&&((ch->get_LN())==CLN))
	      {
	       tst=1;break;
	      };// if
	    ch=ch->get_next();
	   };//while

	   if(tst==0)
		 {return NOT;}
	   else
		 {if(setcur==SET) Current=ch;//надо устанавливать
		  return YES;
		 };

};//  Check_NO_LN

//---------------------------------------------------------------------------

	  // очистка списка
void  ListLogic::ListLogic_Clear(void)
	  {
		while(head!=NULL)
			ListLogic_Del();
		Current=head;
      };// ListLogic_Del
//---------------------------------------------------------------------------
int ListLogic::Start_LLE()// сдвинуть текущий в начало
	  {
		if(head==NULL){
		  #ifdef DBG_ListLogic
			printf("ListLogic_Start: Список пуст.\n");
          #endif
		  return NOT;
		};
			Current=head;
			return YES;
	  };// Start_LLE
//---------------------------------------------------------------------------
int ListLogic::End_LLE()// сдвинуть текущий в начало
	  {
		if(head==NULL){
		  #ifdef DBG_ListLogic
			printf("ListLogic_Start: Список пуст.\n");
          #endif
		  return NOT;
		};
		Logic *cur;
		cur=head;
		while(1!=0){
			if(cur->get_next()==NULL) break; //последний элемент найден
			cur=cur->get_next();
		};//while
	   Current=cur;
	   return YES;
	  };// End_LLE
//---------------------------------------------------------------------------

void  ListLogic::ListLogic_di_NO(int dir){
      unsigned char tmp=Current->get_NO();

	if(dir==INC){
        if((peek(0,0x417)&3)==0) tmp++; /* нет нажатых Shift-L или Shift-R */
          else                   tmp+=10;
//  	if((ListLogic_Check(tmp))==NOT)//исключить повторение номеров объектов
		Current->set_NO(tmp);
	}
	else{// dir==DEC
        if((peek(0,0x417)&3)==0) tmp--; /* нет нажатых Shift-L или Shift-R */
          else                   tmp-=10;
//      if((ListLogic_Check(tmp))==NOT)//исключить повторение номеров объектов
		Current->set_NO(tmp);
	};//if
};//ListLogic_di_NO
//---------------------------------------------------------------------------
void  ListLogic::ListLogic_di_LN(int dir){

	int tmp=Current->get_LN();
	if(dir==INC){
        if((peek(0,0x417)&3)==0) tmp++; /* нет нажатых Shift-L или Shift-R */
          else                   tmp+=10;
		Current->set_LN(tmp);
	}
	else{// dir==DEC
        if((peek(0,0x417)&3)==0) tmp--; /* нет нажатых Shift-L или Shift-R */
          else                   tmp-=10;
		Current->set_LN(tmp);
	};//if
};// ListLogic_di_LN
//---------------------------------------------------------------------------
	// сдвинуть текущий вперед
int ListLogic::ListLogic_Succ()
	  {
		if(Current==NULL) {// стоим в начале,список пуст
		  #ifdef DBG_ListLogic
//			printf("ListLogic_Succ: Список пуст.\n\007");
		  #endif
			return NOT;
		};//error

		Logic *save;
		save=Current;

		if((Current=Current->get_next())==NULL){
		  #ifdef DBG_ListLogic
//			printf("ListLogic_Succ: Некуда двигать.\n\007");
		  #endif
			Current=save;//достижение конца списка
			return NOT;
		};//if

		return YES;
	  };// ListLogic_Succ
//---------------------------------------------------------------------------
	// сдвинуть текущий назад
int ListLogic::ListLogic_Pred()
	  {
		if(Current==NULL) {// стоим в начале,список пуст
		  #ifdef DBG_ListLogic
			printf("ListLogic_Pred: Список пуст.\n\007");
		  #endif
			return NOT;
		};//error

		if(Current==head){
			#ifdef DBG_ListLogic
				printf("ListLogic_Pred: Некуда двигать.\n\007");
			#endif
			return NOT;
		};//if

		Logic *cur,*pred;
		cur=head;
		pred=cur;
		while(cur!=Current){
			pred=cur;
			cur=cur->get_next();
		};//while
		Current=pred;
		return YES;
	  };// ListLogic_Pred
//---------------------------------------------------------------------------
void ListLogic::Search_NO_Pred(void)//поиск предыдущего NO
     {
      Logic *search;
      unsigned char num;

      search=Current;// на случай неуспешного поиска,чтобы остаться там же
      num=Current->get_NO();//этот номер ищем

      while(ListLogic_Pred()!=NOT)//есть куда двигаться
      {if(num!=Current->get_NO()) return;//найден следующий NO
      };//while
      // не нашли NO
      Current=search;
     };// Search_NO_Pred
//---------------------------------------------------------------------------
void ListLogic::Search_NO_Succ(void)//поиск следующего  NO
     {
      Logic *search;
      unsigned char num;

      search=Current;// на случай неуспешного поиска,чтобы остаться там же
      num=Current->get_NO();//этот номер ищем

      while(ListLogic_Succ()!=NOT)//есть куда двигаться
      {if(num!=Current->get_NO()) return;//найден следующий NO
      };//while
      // не нашли NO
      Current=search;
     };// Search_NO_Succ
//---------------------------------------------------------------------------
void ListLogic::Search_LN_Pred(void)//поиск предыдущего LN
     {
      Logic *search;
      unsigned char num;

      search=Current;// на случай неуспешного поиска,чтобы остаться там же
      num=Current->get_NO();//этот номер ищем

      while(ListLogic_Pred()!=NOT)//есть куда двигаться
      {if(num == Current->get_NO()) return;//найден следующий LN
      };//while
      // не нашли LN
      Current=search;
     };// Search_LN_Pred
//---------------------------------------------------------------------------
void ListLogic::Search_LN_Succ(void)//поиск следующего  LN
     {
      Logic *search;
      unsigned char num;

      search=Current;// на случай неуспешного поиска,чтобы остаться там же
      num=Current->get_NO();//этот номер ищем

      while(ListLogic_Succ()!=NOT)//есть куда двигаться
      {if(num == Current->get_NO()) return;//найден следующий LN
      };//while
      // не нашли LN
      Current=search;
     };// Search_LN_Succ
/****************************************************************************/
