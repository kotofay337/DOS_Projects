// red?_004.cpp ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <alloc.h>
#include <conio.h>
#include <mem.h>
#include <string.h>
#include <graphics.h>

#include "red5.h"
#include "tbg_tc.h"


#define OFF      0
#define ON       1
#define OK       2
#define CANCEL   3
#define HELP     4
#define CONTINUE 5


#define START      0
#define MIDDLE     1
#define STOP       2
#define START_STOP 3

#define END    79
#define DEL    83
#define ESC    27
#define HOME   71
#define LEFT   75
#define RIGHT  77
#define ENTER  13
#define BSPACE  8

#define F1     59
#define F2     60

//---------------------------------------------------------------------------
static char String_Edit       [128];
static char String_Edit_Duplex[128];

static short int
    Min,
    Max,
    Current,
    Cursor,
    Start,
    Edit_Pos,
    Lenght_Display,
    First_Key,
    All_Max=sizeof(String_Edit),
    Bar_XMax,Bar_XMin,
    Bar_YMax,Bar_YMin,
    Str_XMax,Str_XMin,
    Str_YMax,Str_YMin,
    Bar_Color,Cursor_Color,
    Overflow_Color;


/* ----------------------------------------------------------------------- */
// Редактирование строки ввода текста в TBG.
// (C) Чумак А.И. 1992
char *EditString(char *string)
{

   OBJECT **Edit_Pic=NULL;
   char *ret = NULL;

   load_file(&Edit_Pic,"tbg_mne.tbg");
   // printf(".load_file\n");
   print_error_tbg("load_file in EditString");

   assert(heapcheck() != _HEAPCORRUPT);

   Store_Image(Edit_Pic,"image_es.tmp");
   // printf(".Store_Image\n");

   assert(heapcheck() != _HEAPCORRUPT);

   outpic_all(Edit_Pic);
   // printf(".outpic_all\n");
   print_error_tbg("outpic_all in EditString");

   assert(heapcheck() != _HEAPCORRUPT);

   Edit_Init(Edit_Pic,1,1,string,127);
   // printf(".Edit_Init\n");
   assert(heapcheck() != _HEAPCORRUPT);

   Refresh_Display(Edit_Pic,1,1);
   // printf(".Refresh_Display\n");
   assert(heapcheck() != _HEAPCORRUPT);


   while(ON){

      assert(heapcheck() != _HEAPCORRUPT);

      switch(EditLine_obj_num(Edit_Pic,1,1)){

         case OK      :
            // printf(".EditLine_obj_num: OK\n");
            break;
         case CANCEL  :
            // printf(".EditLine_obj_num: Cancel\n");
            *String_Edit='\0';
            break;
         case HELP    :
            // printf(".EditLine_obj_num: Help\n");
            printf("\007\007");
            continue;
         case CONTINUE:
            // printf(".EditLine_obj_num: Continue\n");
            continue;
      }//switch Status
      break;
   }//while

   // printf(".break\n");

   Restore_Image(Edit_Pic,"image_es.tmp");
   // printf(".RestoreImage\n");

   assert(heapcheck() != _HEAPCORRUPT);

   del_pic(&Edit_Pic);
   // printf(".del_pic\n");

   assert(heapcheck() != _HEAPCORRUPT);

   ret = Edit_Stop();

   assert(heapcheck() != _HEAPCORRUPT);

   return ret;



}//EditString
//-----------------------------------------------------------------------
// Редактирование в TBG НО или ЛН.
// (C) Чумак А.И. 1992
char* EditString_obj_num(OBJECT **Edit_Pic,     // картинка
                         unsigned char editobj, // обьект
                         unsigned char editnum, // элемент типа "текст"
                         char *string,
                         unsigned char max)
{

   Edit_Init(Edit_Pic,editobj,editnum,string,max);
   Refresh_Display(Edit_Pic,editobj,editnum);

   while(ON){
      switch(EditLine_obj_num(Edit_Pic,editobj,editnum)){
         case OK      :
            break;
         case CANCEL  :
            sprintf(String_Edit,"%s",string);
            break;
         case CONTINUE:
            continue;
         case HELP    :
            continue;
      }//switch
      break;
   }//while

   return Edit_Stop();

}//EditString_obj_num
//---------------------------------------------------------------------------
// Ядро редактирования String_Edit.
// (C) Чумак А.И. 1992
int EditLine_obj_num(OBJECT **Edit_Pic,       // картинка
                       unsigned char editobj, // обьект
                       unsigned char editnum) // элемент типа "текст"
{

   char Key;

   if(!kbhit())
      return CONTINUE;

   Key=getch();

   switch(Key){

      case BSPACE:
         switch(Edit_Pos){
            case START:
               return CONTINUE;
            case STOP:
            case MIDDLE:
               memmove(String_Edit+Current-1,
                        String_Edit+Current,
                        Max-Current);
               Max--;
               Current--;
               if(Cursor <= Min)
                  Start--;
               else
                  Cursor--;
               String_Edit[Max]=NULL;
               break;
         }//switch Edpos
         break;

      case ESC   :
         return CANCEL;

      case ENTER :
         return OK;

      case NULL  :
         First_Key=ON;
         switch(getch()){
            case LEFT :
               switch(Edit_Pos){
                  case START :
                     return CONTINUE;
                  case STOP  :
                  case MIDDLE:
                     Current--;
                     if(Cursor <= Min)
                        Start--;
                     else
                        Cursor--;
                     break;
               }//switch Edpos
               break;
            case RIGHT:
               switch(Edit_Pos){
                  case STOP  :
                     return CONTINUE;
                  case START :
                  case MIDDLE:
                     Current++;
                     if(Cursor == Lenght_Display)
                        Start++;
                     else
                        Cursor++;
                     break;
               }//switch Edpos
               break;
            case DEL  :
               switch(Edit_Pos){
                  case STOP  :
                     return CONTINUE;
                  case MIDDLE:
                  case START :
                     memmove(String_Edit+Current,
                             String_Edit+Current+1,
                             Max-Current);
                     Max--;
                     break;
               }//switch Edpos
               break;
            case HOME :
               switch(Edit_Pos){
                  case START :
                     return CONTINUE;
                  case STOP  :
                  case MIDDLE:
                     Cursor=Start=Current=Min;
                     break;
               }//switch Edpos
               break;
            case END  :
               switch(Edit_Pos){
                  case STOP  :
                     return CONTINUE;
                  case START :
                  case MIDDLE:
                     Current=Max;
                     if(Max > Lenght_Display){
                        Start=Max-(Lenght_Display);
                        Cursor=Lenght_Display;
                     }//if
                     else{
                        Start=Min;
                        Cursor=Max;
                     }//if else
                     break;
               }//switch Edpos
               break;
            case F1   :
               return HELP;
            default   :
               return CONTINUE;
         }//switch
         break;

      default    :

         if(First_Key == OFF){
            memset(String_Edit,0,Max);
            Max = 0;
         }//if

         if(Max == All_Max-1)
            return CONTINUE;

         memmove(String_Edit+Current+1,
                 String_Edit+Current,
                 Max-Current);

         Max++;

         if(Cursor == Lenght_Display)
            Start++;
         else
            Cursor++;

         String_Edit[ Current++ ] = Key;
         String_Edit[ Max ] = NULL;

         First_Key=ON;

         break;
   }//switch

   if(Current==Min)
      Edit_Pos=START;

   if(Current==Max)
      Edit_Pos=STOP;

   if((Current!=Min)&&
      (Current!=Max))
      Edit_Pos=MIDDLE;

   if(Min==Max)
      Edit_Pos=START_STOP;


   Refresh_Display(Edit_Pic,editobj,editnum);
   return CONTINUE;

}/* EditLine_obj_num */
//---------------------------------------------------------------------------
// Функция отображения процесса редактирования.
// (C) Чумак А.И. 1992
void Refresh_Display(OBJECT **Edit_Pic,unsigned char editobj,
                                         unsigned char editnum)
{
 int sc,ec;
 char* ps;

   outpic_obj_num(Edit_Pic,editobj,editnum+1);
//   outtext_obj_num(Edit_Pic,editobj,editnum,Bar_Color,String_Edit+Start,SHOW);

   setcolor(EGA_DARKGRAY);
   settextstyle(SIMPLEX_FONT, HORIZ_DIR, 4);
   outtextxy(Str_XMin, Str_YMin, String_Edit+Start);

   ps = gettext_obj_num(Edit_Pic,editobj,editnum);

   strcpy(String_Edit_Duplex,ps);
   String_Edit_Duplex[Cursor+1] = '\0';
   ec=textwidth(String_Edit_Duplex);

   String_Edit_Duplex[Cursor  ] = '\0';
   sc=textwidth(String_Edit_Duplex);

   if(Start > Min)
      setcolor_obj_num(Edit_Pic,editobj,editnum+2,Overflow_Color,SHOW);
   else
      setcolor_obj_num(Edit_Pic,editobj,editnum+2,Bar_Color,SHOW);

   if(Max > (Start+Lenght_Display))
      setcolor_obj_num(Edit_Pic,editobj,editnum+3,Overflow_Color,SHOW);
   else
      setcolor_obj_num(Edit_Pic,editobj,editnum+3,Bar_Color,SHOW);

   setcolor(Cursor_Color);
   setlinestyle(SOLID_LINE,0,1);
   rectangle((Str_XMin + sc) - 1,
	          Str_YMin + 4,
				 (Str_XMin + ec) + 1,
				 Str_YMin - 10);

/*
   setcolor(Cursor_Color);
   setlinestyle(SOLID_LINE,0,1);
   rectangle(Str_XMin+(textwidth(String_Edit)-textwidth(String_Edit+Cursor)-1),
             Str_YMax-textheight("W"),
             Str_XMin+(textwidth(String_Edit)-textwidth(String_Edit+Cursor))-1,
             Str_YMax-2);
*/
}//Refresh_Display
//---------------------------------------------------------------------------
// Функция инициализации редактирования
// (C) Чумак А.И. 1992
void Edit_Init(OBJECT **Edit_Pic,unsigned char editobj,unsigned char editnum,
               char *String,unsigned char max)
{

    memset(String_Edit,0,sizeof(String_Edit));

    Min=0;

    Max=strlen(String);

    if(Max > max)
       Max=max-1;

    All_Max=max;
    Start=Current=Cursor=0;
    Edit_Pos=START;

    Lenght_Display=strlen_obj_num(Edit_Pic,editobj,editnum);

    if(Lenght_Display > max)
       Lenght_Display=max;

    First_Key=OFF;

    memmove(String_Edit,String,Max);

    getsize_obj_num_reset();
    getsize_obj_num(Edit_Pic,editobj,editnum+1,
                    &Bar_XMin,&Bar_YMin,&Bar_XMax,&Bar_YMax);
    getsize_obj_num_reset();
    getsize_obj_num(Edit_Pic,editobj,editnum,
                    &Str_XMin,&Str_YMin,&Str_XMax,&Str_YMax);
    Bar_Color     =getcolor_obj_num(Edit_Pic,editobj,editnum+1);

    Cursor_Color=EGA_WHITE;

    if((Bar_Color==EGA_LIGHTGRAY   )||
       (Bar_Color==13              )|| //PASTEL
       (Bar_Color==EGA_CYAN        )||
       (Bar_Color==EGA_LIGHTBLUE   ))
       Cursor_Color=EGA_RED;

    Overflow_Color=getcolor_obj_num(Edit_Pic,editobj,editnum+2);

}//Edit_Init
//---------------------------------------------------------------------------
// Функция окончания редактирования.
// Возвращает адрес String_Edit
// (C) Чумак А.И. 1992
char* Edit_Stop(void)
{

    First_Key=OFF;
    return String_Edit;

}//Edit_Stop
//---------------------------------------------------------------------------
// end red?_004.cpp =====================================================