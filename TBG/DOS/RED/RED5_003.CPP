
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

static char test;
extern char demomess[];

//-----------------------------------------------------------------------
FILE* LIST::CreateInputFile(char *fname) //создать файл для вывода
{
 FILE *inf;

 sprintf(curr_file_name,"%s/%s",pck.div_path,fname);

 if((inf=(fopen(curr_file_name,"r+b")))==NULL)//открываем существующий файл
   {//файла не существует
    if((inf=(fopen(curr_file_name,"wb")))==NULL)//создаем файл для записи
      { //не успех !
        Warning("Не открыт файл для сохранения");return NULL;
      };
   }
   else
   {//файл существует

    //запрос подтверждения изменения

    Crt_Bakup(curr_file_name);// создание файла .BAK
    fclose(inf);//
    //открываем повторно для его полной перезаписи
    if((inf=(fopen(curr_file_name,"wb")))==NULL)//создаем файл
      { //не успех !
        Warning("Не открыт файл для сохранения");return NULL;
      };

   };//открываем существующий файл


   char tip_file = TF;// тип файла
   fwrite(&tip_file,1,1,inf);

   // заголовок файла
   struct HTBG_WR htbg;
   htbg.copyr[0] =' ';
   htbg.copyr[1] ='T';
   htbg.copyr[2] ='B';
   htbg.copyr[3] ='G';
   htbg.copyr[4] =' ';

   htbg.ir2[0] =' ';
   htbg.ir2[1] ='N';
   htbg.ir2[2] ='E';
   htbg.ir2[3] ='W';
   htbg.ir2[4] =' ';

   strncpy(htbg.copyr+5,lab+4,5);// версия
   fwrite(&htbg,sizeof(htbg)-4,1,inf);

  return inf;
};// CreateInputFile
//-----------------------------------------------------------------------
int  LIST::SaveOne(FILE *inf,int Index)//сохранение одного элемента
{

char tipGE=list[Index]->get_tip();//тип ГЭ
     tipGE=-tipGE;//тип ГЭ
     if(fwrite(&tipGE,1,1,inf) != 1) return -1;//тип ГЭ
     list[Index]->save(inf);//сохранить ГЭ

int  maxlle=list[Index]->BGE_Get_Max_LE();
     if(maxlle>=0){
        if(fwrite(&maxlle,1,1,inf) != 1)return -1;//число ЛЭ
        list[Index]->BGE_Save_LLE(inf);
       };//if
     SaveTest=OFF;// признак необходимости сохранения
     return 0;

};// SaveOne
//-----------------------------------------------------------------------
void LIST::SaveAll(char *fname,char mode) // сохранение всего изображения
{
 if(PN<0) return;//список пуст

 FILE *inf;
 int size_le;
 int size_ei;

// printf("\rSave=%d        ",mode);

 switch(mode)
 {
  case mode_save_all_tbg:
       size_le=LE_TOP;
       size_ei=PN_TOP;
  break;
  case mode_save_ram_tbg:
       size_le=LE_TOP_Ram;
       size_ei=count_select;
       if(ente<=1) return; // нет рамки
       if(ind_select==OFF) return; // нет элементов в рамке
  break;
  case mode_save_ge_tbg :
       size_le=list[PN]->BGE_Get_Max_LE();
       size_ei=0;
  break;
  default:
//    fclose(inf);
    return;
 }// switch

 if((inf=CreateInputFile(fname))==NULL) return;//проблемы при открытии файла

 if(strlen(demomess)!=0)
 {
  Warning(demomess);
  if(test==1)
  {
   size_ei = 13;
  }
  else
  {
   test=1;
  }
 }/* if */
 // число  сохраняемых  элементов
 fwrite(&size_le,1,2,inf);
 fwrite(&size_ei,1,2,inf);

// printf("Save le=%d ei=%d   \r",size_le,size_ei);

 if(mode == mode_save_ge_tbg){
    SaveOne(inf,PN);
    fclose(inf);
    return;
 }//if

 /* выполнение */
 Store_Image(menu_5,"image.tmp");// сохраняем место под меню
 outpic_all(menu_5);
 short int xmin, ymin, xmax, ymax;
 getsize_obj_num_reset();
 getsize_obj_num(menu_5,1,1,&xmin,&ymin,&xmax,&ymax);
 setfillstyle(SOLID_FILL,EGA_LIGHTBLUE);


  for(int Index=0;Index<=PN_TOP;Index++)
  { //    запись всех элементов
   if(mode==mode_save_ram_tbg){
     if(list[Index]->GetSelect()==ON) {
        if(SaveOne(inf,Index)<0) break;
     }// if
   }// if
   else{
     if(SaveOne(inf,Index)<0) break;
   }// else if

   // выполнение
  disable();// запрет прерываний
  if(PN_TOP != 0)
     bar(xmin,ymin,xmin+
    	  (int)((double)(xmax-xmin)*((double)Index/(double)PN_TOP)),
         ymax);
  enable();// разрешение прерываний

  }//for запись всех элементов

 Restore_Image(menu_5,"image.tmp");// восстанавливаем место под меню
 fclose(inf);
 return;//нормальное окончание

};// SaveAll
//-----------------------------------------------------------------------
int  LIST::LoadFile(char *fname)// загрузка изображения из файла
{
 FILE *ouf;
 char F_GE=0;//метка файла
 int PN_TOPLF;//число элементов в файле
 char tipGE;//тип ГЭ/Лэ
 long fpred;//позиция перед считыванием байта за ГЭ
 char LN,NO,tip;
 char s[90];

 sprintf(curr_file_name,"%s\\%s",pck.div_path,fname);

 ouf=fopen(curr_file_name,"rb");

 if(ouf==NULL)//открываем существующий файл
   {//файла не существует
    sprintf(s,"Файл \"%s\" не существует",curr_file_name);
    Warning(s);
    return Ok;
   }
   else
   { //файл существует

   fseek(ouf, 0L, SEEK_END);
   long length = ftell(ouf);//длинна файла

   rewind(ouf);//файл в начало

   if(fread(&tip_file,1,1,ouf) != 1) goto error;//тип файла

   struct HTBG_WR htbg;
   fread(&htbg,sizeof(htbg),1,ouf);

  PN_TOPLF=htbg.count_ei; //число элементов изображения в файле
//  printf("Load le=%d  ei=%d   \r",htbg.Num_LE,htbg.count_ei);
//  press_key();

  if((PN_TOP+PN_TOPLF)>MAX_BGE-1)//помещается ли файл в списке ?
    {Warning("Файл не помещается");
     fclose(ouf);
     return Fu;
    };
  /* выполнение */
  Store_Image(menu_5,"image.tmp");// сохраняем место под меню
  outpic_all(menu_5);
  short int xmin, ymin, xmax, ymax;
  getsize_obj_num_reset();
  getsize_obj_num(menu_5,1,1,&xmin,&ymin,&xmax,&ymax);


  setfillstyle(SOLID_FILL,EGA_LIGHTBLUE);

    for(int Index=0;Index<=PN_TOPLF;Index++)
    {// ввод всех элементов
	 // индикация выполнения

     disable();// запрет прерываний
	 if(PN_TOPLF!=0)

     bar(xmin,ymin,xmin+
		(int)((double)(xmax-xmin)*((double)Index/(double)PN_TOPLF)),
        ymax);
     enable();// разрешение прерываний

     switch(tip_file) // тип файла
     {
      case 2://старые типы: принадлежность к одному обьекту
      case 3:
           if(fread(&LN,1,1,ouf) != 1) goto error;
           if(fread(&NO,1,1,ouf) != 1) goto error;
           if(fread(&tip,1,1,ouf) != 1) goto error;
           if(CreateGE(tip) < 0) goto error1;
            list[PN]->load(ouf);//загрузить ГЭ
           if((NO==0)&&(LN==0)){}//нет ЛЭ
             else{list[PN]->BGE_Add_LE(NO,LN); };//есть ЛЭ
           if(PN != PN_TOP) PN++;
      break; //старые типы: принадлежность к одному обьекту
      case 4://новые типы: принадлежность к нескольким обьектам
      case 5:
        if(F_GE==0)
           if(fread(&tipGE,1,1,ouf) != 1) goto error;//тип ГЭ

        if(tipGE<0){  // ГЭ
          F_GE=0;
          if(CreateGE(-tipGE) < 0) goto error1;

          list[PN]->load(ouf);//загрузить ГЭ

          fpred=ftell(ouf);//место байта ГЭ
          if(fpred==length) break;//следующий байт уже конец файла- не надо читать его
                                  //это если за последним ГЭ нет ЛЭ

          if(fread(&tipGE,1,1,ouf) != 1) goto error;//тип ГЭ
          if(tipGE<0){  // ГЭ
             F_GE=1;
             if(PN != PN_TOP) PN++;
            }
            else{
             F_GE=0;
	         list[PN]->BGE_Load_LLE(ouf,tipGE);//файл и число ЛЭ
             if(PN != PN_TOP) PN++;
            };//if
          }  // ГЭ
          else printf("Что-то крутое с Главным байтом\n");
      break; //новые типы: принадлежность к нескольким обьектам
      default:printf("Неизвестный тип файла=%d\n",tip_file);
     }//switch тип файла
    }//for ввод всех элементов
   }//файл существует

   goto end;
//   Restore_Image(menu_5,"image.tmp");// восстанавливаем место под меню
//   fclose(ouf);
//   return Ok;

error:;//ошибка
   Warning("Ошибка при чтении из файла");
   goto end;
error1:;//ошибка
   Warning("Нет памяти для загрузки всех ГЭ из файла");
end:;
   Restore_Image(menu_5,"image.tmp");// восстанавливаем место под меню
   fclose(ouf);
   return Ok;
};// LoadFile

//-----------------------------------------------------------------------
LIST::LIST(int InitMax)// конструктор
{ list = new BaseGraphElement* [InitMax];
  PN=-1;PN_TOP=-1;MAX_BGE=InitMax;//tip_search=TIP_line;
}
//-----------------------------------------------------------------------
/*
LIST::~LIST(int InitMax)// деструктор
{
 delete list;
}
*/
//-----------------------------------------------------------------------
void LIST::DI_fi1(int direct)
{
if(PN<0) return;//список пуст
if(((list[PN]->get_tip()) != TIP_elip) && ((list[PN]->get_tip()) != TIP_felip))
   return;//не эллипс

int dfi=10;//интервал изменения c Shift
if((peek(0,0x417)&3)==0) dfi=1;  // нет нажатых Shift-L или Shift-R

if(direct==DEC) dfi=-dfi;  // уменьшаем fi1

set_mode_xor();  //XOR
list[PN]->Show();//прежнее состояние эллипса

list[PN]->put_fi1(list[PN]->get_fi1()+dfi);//новый fi1

set_mode_xor();  //XOR
list[PN]->Show();//новое состояние эллипса
printf("начало дуги %d град.   \r",list[PN]->get_fi1());//на экран

};// DI_fi1
//-----------------------------------------------------------------------
void LIST::DI_fi2(int direct)
{
if(PN<0) return;//список пуст
if(((list[PN]->get_tip()) != TIP_elip) && ((list[PN]->get_tip()) != TIP_felip))
   return;//не эллипс

int dfi=10;//интервал изменения c Shift
if((peek(0,0x417)&3)==0) dfi=1;  // нет нажатых Shift-L или Shift-R

if(direct==DEC) dfi=-dfi;  // уменьшаем fi2

set_mode_xor();  //XOR
list[PN]->Show();//прежнее состояние эллипса

list[PN]->put_fi2(list[PN]->get_fi2()+dfi);//новый fi2

set_mode_xor();  //XOR
list[PN]->Show();//новое состояние эллипса
printf("конец дуги %d град.   \r",list[PN]->get_fi2());//на экран

};// DI_fi2

//-----------------------------------------------------------------------
void Warning(char *s)
{

   int sz,l,r;
   short int xmin, ymin, xmax, ymax;


   sz=strlen(s)*8;
   l=(MaxX/2)-(sz/2);if(l<0) l=0;
   r=l+sz;
   getsize_obj_num_reset();
   getsize_obj_num(menu_mess,255,1,&xmin,&ymin,&xmax,&ymax);

   setfillstyle(SOLID_FILL,EGA_LIGHTRED);
   bar(l,ymin,r+4,ymax);

   settextstyle(0,0,1);
   setcolor(EGA_WHITE);
//   settextjustify(CENTER_TEXT,CENTER_TEXT);
   settextjustify(0,0);
   outtextxy(l+4,ymax,s);
   pos_ms_pred=0;//убрать предупреждение
//   mn1_out=OFF;
   ibell();

   press_key();

}/*Warning*/
/****************************************************************************/
/* (C) 1992 Чумак А.И. */
int Crt_Bakup(const char *bakup)
{
   char fname[MAXPATH];

   strcpy(fname,bakup);
   *(strchr(fname,'.'))='\0';
   strcat(fname,".bak");

   unlink(fname);

   return  rename(bakup,fname);

}//Crt_Bakup
//--------------------------------------------------------------------
