/*
   DList.HPP

   Ўаблонный тип реализующий элемент двусв€зного списка
   и, собственно, этот список.

   15.10.2001 16:57 msk

*/
#ifndef __DLIST_HPP__
#define __DLIST_HPP__
// Ўаблоны типов

// Ёлемент двусв€зного списка
template <class T> class List {
   List<T> *next, *prev;
   T data;
public:
   List(){next = prev = NULL;};
   inline void  __fastcall SetNext(List *sNext){next = sNext;};
   inline void  __fastcall SetPrev(List *sPrev){prev = sPrev;};
   inline List* __fastcall GetNext(void){return next;};
   inline List* __fastcall GetPrev(void){return prev;};
   inline void  __fastcall SetData(T sdata){data = sdata;};
   inline T     __fastcall GetData(void){return data;};
};

// —писок двусв€зный, управление
template <class T> class LinkList {
   List<T> *head;
   List<T> *current;
   List<T> *tail;
   List<T> *store;
   unsigned long Counter;
public:

   enum MovingModes {_ToHead, _ToTail, _ToNext, _ToPrev};
   
   LinkList(){head = tail = current = store = NULL; Counter = 0UL;};
   ~LinkList(){
      while(!Empty())
         DeleteFromHead();
   };
   inline bool __fastcall Empty(void){return Counter == 0UL;};
   inline unsigned long __fastcall GetCount(void){return Counter;};

   int IndexCurrent(void){
      int ret = -1, i;
      if(!Empty()){
         StoreCurrent();
         ToHead();
         for(i = 0;current != store;i++){
            Next();
         }/* for */
         ret = i;
         ReStoreCurrent();
      }/* if */
      return ret;
   }/* indexcur */
   
   List<T> *operator[](int index){
      List<T> *ret = NULL;
      if((!Empty()) && (index >= 0) && (index <= Counter)){
         StoreCurrent();
         ToHead();
         for(int i = 0;i != index;i++){
            Next();
         }/* for */
         ret = current->GetData();
         ReStoreCurrent();
      }/* if */
      return ret;
   }/* [] */
   
   inline void __fastcall StoreCurrent(void){
      if(current){
         store = current;
      }/* if */
   };
   
   inline void __fastcall ReStoreCurrent(void){
      if(store){
         current = store;
      }/* if */
      else{
         current = tail;
      }/* elif */
   };
   
   void __fastcall AddToHead(T Data){
      List<T> *New = new List<T>;
      if(New){
         New->SetData(Data);
         New->SetNext(head);
         if(head){
            head->SetPrev(New);
         }/* if */
         else{
            tail = current = New;
         }/* elif */
         head = New;
         Counter++;
      }/* if */
   };

   void __fastcall AddToTail(T Data){
      List<T> *New = new List<T>;
      if(New){
         New->SetData(Data);
         New->SetPrev(tail);
         if(tail){
            tail->SetNext(New);
         }/* if */
         else{
            head = current = New;
         }/* elif */
         tail = New;
         Counter++;
      }/* if */
   };

   void __fastcall AddToCurrent(T Data){
      List<T> *New = new List<T>;
      if(New){
         New->SetData(Data);
         if(current){
            // add after current
            if(current->GetNext()){
               current->GetNext()->SetPrev(New);
            }/* if */
            else{
               tail = New;
            }/* elif */
            New->SetPrev(current);
            New->SetNext(current->GetNext());
            current->SetNext(New);
            current = New;
/*
            // add before current
            if(current->GetPrev()){
               current->GetPrev()->SetNext(New);
            }/* if */
            else{
               head = New;
            }/* elif */
            New->SetNext(current);
            New->SetPrev(current->GetPrev());
            current->SetPrev(New);
*/            
         }/* if */
         else{
            if(head == NULL){
               current = tail = head = New;
            }/* if */
            else{
               AddToTail(Data);
               return;
            }/* elif */
         }/* elif */
         Counter++;
      }/* if */
   };

   inline void operator+=(T Data){
      AddToTail(Data);
   };

   T __fastcall DeleteFromHead(void){
      if(head){
         List<T> *t = head;
         T Data = t->GetData();
         head = head->GetNext();
         if(head){
            head->SetPrev(NULL);
         }/* if */
         if(current == t){ // текущий был в голове
            current = head;
         }/* if */
         if(tail == t){
            tail = head;
         }/* if */
         delete t;
         store = NULL;
         Counter--;
         return Data;
      }/* if */
      return NULL;
   };

   T __fastcall DeleteFromTail(void){
      if(head){
         List<T> *t = tail;
         T Data = t->GetData();
         tail = tail->GetPrev();
         if(tail){
            tail->SetNext(NULL);
         }/* if */
         if(current == t){ // текущий был в хвосте
            current = tail;
         }/* if */
         if(head == t){
            head = tail;
         }/* if */
         delete t;
         store = NULL;
         Counter--;
         return Data;
      }/* if */
      return NULL;
   };

   T __fastcall DeleteFromCurrent(void){
      if(head){
         if(current == head){
            return DeleteFromHead();
         }/* if */
         if(current == tail){
            return DeleteFromTail();
         }/* if */

         List<T> *t = current;
         T Data = t->GetData();

         current = current->GetNext();
         current->SetPrev(t->GetPrev());
         t->GetPrev()->SetNext(current);
         
         delete t;
         store = NULL;
         Counter--;
         return Data;
      }/* if */
      return NULL;
   };
   
   inline bool __fastcall ToHead(void){current = head; return true;};
   inline bool __fastcall ToTail(void){current = tail; return true;};

   bool __fastcall Next(void){
      if(current){
         List<T> *set = current->GetNext();
         if(set == NULL){
            return false;
         }/* if */
         else{
            current =  set;
            return true;
         }/* elif */
      }/* if */
      else{
         return false;
      }/* elif */
   };

   bool __fastcall Prev(void){
      if(current){
         List<T> *set = current->GetPrev();
         if(set == NULL){
            return false;
         }/* if */
         else{
            current =  set;
            return true;
         }/* elif */
      }/* if */
      else{
         return false;
      }/* elif */
   };

   inline T __fastcall GetData(void){
      return current ? current->GetData() : NULL;
   };

   void __fastcall CurrentMoveTo(MovingModes to){

      if(current == NULL)
         return;

      // pullout from list
      if(current->GetNext())
         current->GetNext()->SetPrev(current->GetPrev());
      if(current->GetPrev())
         current->GetPrev()->SetNext(current->GetNext());

      if(to == _ToHead){
         // push to head
         current->SetPrev(NULL);
         current->SetNext(head);
         head->SetPrev(current);
         head = current;
      }/* if */

      if(to == _ToTail){
         // push to tail
         current->SetPrev(tail);
         current->SetNext(NULL);
         tail->SetNext(current);
         tail = current;
      }/* if */

      if(to == _ToNext){
         if(current == tail)
            return;
         List<T> *t = current->GetNext();
         if(t->GetNext())
            t->GetNext()->SetPrev(current);
         t->SetNext(current);
         current->SetNext(t->GetNext());
         current->SetPrev(t);
      }/* if */

      if(to == _ToPrev){
         if(current == head)
            return;
         List<T> *t = current->GetPrev();
         if(t->GetPrev())
            t->GetPrev()->SetNext(current);
         t->SetPrev(current);
         current->SetPrev(t->GetPrev());
         current->SetNext(t);
      }/* if */
   };
};

#endif /* __DLIST_HPP__ */
