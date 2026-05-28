
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <conio.h>
#include <graphics.h>
#include <bios.h>
#include <keyboard.h>
#include "red5.h"

int outpic_Display(OBJECT **obj);
int CompareName( const void *a, const void *b);

typedef struct{
   char fname[16];
}Name;
/****************************************************************************/
static int    spf_dir_max,spf_reg_max,spf_drv_max;

static Name *spf_reg,
	    *spf_dir,
	    *spf_drv;
/****************************************************************************/
void del_spf(void)
{

   if(spf_drv)
      free((void*)spf_drv);
   if(spf_dir)
      free((void*)spf_dir);
   if(spf_reg)
      free((void*)spf_reg);

}// del_spf
/****************************************************************************/
static long predtime;
char *menu_sl(BYTE mode)
{
/* char dc=OFF;/* double clik */*/
 CHAR Ext[5];
 char show=OFF;

 char p1=ON; // имена файлов
 char p2=ON; // директории
 char p3=ON; // диски
 char p4=ON; // шаблон
 char p6=ON; // путь
 char p7=ON; // имя рабочего файла

 short int xmin, ymin, xmax, ymax;
 int col_fon1;/* фон текста приглашения */
 int col_fon;/*  фон текстов */
 /* работа с файлами */
 OBJECT **menu_4 =NULL;/* указатель на меню_4 */
 OBJECT **Display=NULL;/* указатель на MicroDisplay */


static int posf_reg=0;
static int posf_dir=0;
static int posf_drv=0;

static char ext_mode=2;

char s[80];// буфер для всего
static char fname[15],path[80];
int del_mouse=OFF;/* нужно удалять мышь при выводе имен файлов */

struct ffblk ffblk;
int done=0;

int i=0,d=0;
int end=OFF;
int save,disk;

int pos_mm;// позиция мыши на меню
short int keys,left,right;


predtime=biostime(0,0L);

//if(load_file(&menu_4,"tbg_m325.tbg") < 0) return NULL;// загрузка меню

if(load_file(&menu_4,"tbg_m004.tbg") < 0) return NULL;// загрузка меню

if(mode == SAVE_obj) {
   switch(pck.Mode_save){
    case mode_save_all_pcx:
    case mode_save_ram_pcx:
         sprintf(Ext,".pcx");
         ext_mode = 1;// для индикации
    break;
    default:
    sprintf(Ext,".tbg");
    ext_mode = 2;// для индикации
   }// switch
}// if
if(mode == LOAD_obj) {
   switch(pck.Mode_load){
	 case mode_load_fon_pcx:
         sprintf(Ext,".pcx");
         ext_mode = 1;// для индикации
    break;
    default:
    sprintf(Ext,".tbg");
    ext_mode = 2;// для индикации
   }// switch

}// if
if(ext_mode == 1) sprintf(fname,"%s",pck.Fn_work_pcx);
if(ext_mode == 2) sprintf(fname,"%s",pck.Fn_work_tbg);

// подсчет числа дисков

spf_drv_max=0;//число дисков
save = getdisk();// сохраняем текущий диск

for (disk = 0;disk < 26;++disk)
{
   setdisk(disk);
   if (disk == getdisk()) spf_drv_max++;
}//for
// подсчет числа дисков
// создание списка дисков

// память для указателей списка дисков
spf_drv=(Name*)calloc(spf_drv_max,sizeof(Name));// 0-spf_drv_max
if(spf_drv==NULL) {
   printf("\nНЕТ памяти для списка дисков\007");
   Restore_Image(menu_4,"image.tmp");// восстанавливаем место под меню
   del_pic(&menu_4);
   return 0;
}//if

i=0;
for (disk = 0;disk < 26;++disk)
{
   setdisk(disk);
   if (disk == getdisk()){
	sprintf(spf_drv[i].fname,"%c:",disk + 'A');
//        printf("\n%s",*(spf_drv+i));
        i++;
   }// if
}//for
setdisk(save);//восстанавливаем текущий диск
// создание списка дисков

do{ /* самый большой цикл  */

// подсчет числа директорий

sprintf(path,"%s\\*.*",pck.div_path);//все-все файлы
//sprintf(path,"%s\\*.*",Drive,Pathe);//все-все файлы

done=findfirst(path,&ffblk,FA_RDONLY|
                           FA_HIDDEN|
                           FA_SYSTEM|
                           FA_LABEL |
                           FA_DIREC |
                           FA_ARCH);

//printf("errno=%d \n",errno);
spf_dir_max=0;//число директорий

while(!done){
   if (ffblk.ff_attrib & FA_DIREC)  //выделяем директории
      {
       if((ffblk.ff_name[0]=='.')&&(ffblk.ff_name[1]==0)){
		 }//вход в себя не нужен
       else
		    spf_dir_max++;//число директорий
//       printf("\n%s",ffblk.ff_name);
      }//if
   done=findnext(&ffblk);
}//while

//       printf("\n%d  ",spf_dir_max);
// подсчет числа директорий
// создание списка директорий

if(spf_dir_max != 0){//есть директории
// память для указателей списка директорий
spf_dir=(Name*)calloc(spf_dir_max,sizeof(Name));// 0-spf_dir_max
if(spf_dir==NULL) {
   printf("НЕТ памяти для директорий");
   Restore_Image(menu_4,"image.tmp");// восстанавливаем место под меню
   del_pic(&menu_4);
   return 0;
}//if

done=findfirst(path,&ffblk,FA_RDONLY|
                           FA_HIDDEN|
                           FA_SYSTEM|
                           FA_LABEL |
                           FA_DIREC |
									FA_ARCH);
i=0;// адресация

while(!done){
   if (ffblk.ff_attrib & FA_DIREC)  //выделяем директории
      {
       if((ffblk.ff_name[0]=='.')&&(ffblk.ff_name[1]==0)){
		 }//вход в себя не нужен
       else{
	   sprintf(spf_dir[i].fname,"%s",ffblk.ff_name);
//           printf("\n%s",*(spf_dir+i));
           i++;
         }//else if вход в себя

      }//if
   done=findnext(&ffblk);
}//while
}//if есть директории
// создание списка директорий

// подсчет числа файлов по шаблону

 sprintf(path,"%s\\%s%s",pck.div_path,pck.sbl,Ext);

done=findfirst(path,&ffblk,FA_DIREC);

spf_reg_max=0;//число файлов по шаблону

while(!done){

   spf_reg_max++;//число файлов по шаблону
   done=findnext(&ffblk);

}//while

// подсчет числа файлов по шаблону

// создание списка файлов по шаблону

if(spf_reg_max!=0){// есть файлы
// память для указателей списка файлов по шаблону
spf_reg=(Name*)calloc(spf_reg_max,sizeof(Name));// 0-spf_reg_max

if(spf_reg==NULL) {
   printf("НЕТ памяти для файлов");
   Restore_Image(menu_4,"image.tmp");// восстанавливаем место под меню
   del_pic(&menu_4);
   return 0;
}//if

done=findfirst(path,&ffblk,FA_DIREC);
//done=findfirst(sreg,&ffblk,FA_DIREC);
i=0;// адресация

while(!done){

   sprintf((char*)(spf_reg[i].fname),"%s",ffblk.ff_name);
//   printf("\n%d %s  Own file",i,*(spf_reg+i));
   i++;
   done=findnext(&ffblk);
}//while
}// if есть файлы

qsort(spf_reg,spf_reg_max,sizeof(Name),CompareName);
qsort(spf_dir,spf_dir_max,sizeof(Name),CompareName);

if(show == OFF)
{
 mouse_off();
 col_fon1=getcolor_obj_num(menu_4,1,1);/* фон текста приглашения */
 col_fon= getcolor_obj_num(menu_4,1,2);/*  фон текстов */

 // рисование меню
 // заголовки убрать
 setcolor_obj    (menu_4,SAVE_obj,col_fon1,NO_SHOW);
 setcolor_obj    (menu_4,LOAD_obj,col_fon1,NO_SHOW);

 Store_Image(menu_4,"image.tmp");// сохраняем место под меню

 outpic_all(menu_4);
 // заголовки показать
 if(mode == SAVE_obj){
    setcolor_obj_num(menu_4,SAVE_obj,pck.Mode_save,EGA_WHITE,SHOW);
 }// if
 if(mode == LOAD_obj){
    setcolor_obj_num(menu_4,LOAD_obj,pck.Mode_load ,EGA_WHITE,SHOW);
 }// if

 getsize_obj_num_reset();
 getsize_obj_num(menu_4,1,4,&xmin,&ymin,&xmax,&ymax);
 mouse_setxy(xmax-5,ymax-5);/* мышь на "НЕТ" */

 setcolor_obj(menu_4,12,EGA_DARKGRAY,123);        // режим расширения файла
 setcolor_obj_num(menu_4,12,ext_mode,EGA_RED,123);


 mouse_on();
 show=ON;
}// show


do{ /* основной опрос */

/* имена файлов */
if(del_mouse==ON) mouse_off();

//disable();// запрет прерываний

// новые имена
if(p1 == ON){
disable();// запрет прерываний
outpic_obj(menu_4,30);/* стираем все имена */
enable();// разрешение прерываний
outtext_obj_num(menu_4,11,1,col_fon,"<NEW>",SHOW);
if((posf_reg+0)<(spf_reg_max)) outtext_obj_num(menu_4,11,2,col_fon,spf_reg[posf_reg+0].fname,SHOW);
if((posf_reg+1)<(spf_reg_max)) outtext_obj_num(menu_4,11,3,col_fon,spf_reg[posf_reg+1].fname,SHOW);
if((posf_reg+2)<(spf_reg_max)) outtext_obj_num(menu_4,11,4,col_fon,spf_reg[posf_reg+2].fname,SHOW);
if((posf_reg+3)<(spf_reg_max)) outtext_obj_num(menu_4,11,5,col_fon,spf_reg[posf_reg+3].fname,SHOW);
if((posf_reg+4)<(spf_reg_max)) outtext_obj_num(menu_4,11,6,col_fon,spf_reg[posf_reg+4].fname,SHOW);
p1=OFF;
}// if

if(p2 == ON){
// новые директории
disable();// запрет прерываний
outpic_obj(menu_4,40);/* стираем все директории */
enable();// разрешение прерываний
if((posf_dir+0)<(spf_dir_max)) outtext_obj_num(menu_4,16,1,col_fon,spf_dir[posf_dir+0].fname,SHOW);
if((posf_dir+1)<(spf_dir_max)) outtext_obj_num(menu_4,16,2,col_fon,spf_dir[posf_dir+1].fname,SHOW);
if((posf_dir+2)<(spf_dir_max)) outtext_obj_num(menu_4,16,3,col_fon,spf_dir[posf_dir+2].fname,SHOW);
if((posf_dir+3)<(spf_dir_max)) outtext_obj_num(menu_4,16,4,col_fon,spf_dir[posf_dir+3].fname,SHOW);
if((posf_dir+4)<(spf_dir_max)) outtext_obj_num(menu_4,16,5,col_fon,spf_dir[posf_dir+4].fname,SHOW);
if((posf_dir+5)<(spf_dir_max)) outtext_obj_num(menu_4,16,6,col_fon,spf_dir[posf_dir+5].fname,SHOW);
p2=OFF;
}// if

if(p3 == ON){
// новые диски
disable();// запрет прерываний
outpic_obj(menu_4,50);// стираем все диски
enable();// разрешение прерываний
if((posf_drv+0)<(spf_drv_max)) outtext_obj_num(menu_4,21,1,col_fon,spf_drv[posf_drv+0].fname,SHOW);
if((posf_drv+1)<(spf_drv_max)) outtext_obj_num(menu_4,21,2,col_fon,spf_drv[posf_drv+1].fname,SHOW);
if((posf_drv+2)<(spf_drv_max)) outtext_obj_num(menu_4,21,3,col_fon,spf_drv[posf_drv+2].fname,SHOW);
if((posf_drv+3)<(spf_drv_max)) outtext_obj_num(menu_4,21,4,col_fon,spf_drv[posf_drv+3].fname,SHOW);
if((posf_drv+4)<(spf_drv_max)) outtext_obj_num(menu_4,21,5,col_fon,spf_drv[posf_drv+4].fname,SHOW);
if((posf_drv+5)<(spf_drv_max)) outtext_obj_num(menu_4,21,6,col_fon,spf_drv[posf_drv+5].fname,SHOW);
p3=OFF;
}// if

if(p4 == ON){
outpic_obj_num(menu_4,1,15);/* стираем шаблон */
outtext_obj_num(menu_4,7,2,col_fon,pck.sbl,SHOW);     /* шаблон */
p4=OFF;
}// if

if(del_mouse==ON) {mouse_on();del_mouse=OFF;};

if(p6 == ON){
outtext_obj_num(menu_4,8,2,col_fon,pck.div_path,SHOW);/* имя директории */
p6=OFF;
}// if

if(p7 == ON){
outpic_obj_num(menu_4,1,2);/* стираем имя файла для работы */
outtext_obj_num(menu_4,2,2,col_fon,fname,SHOW);/* имя файла для работы */
p7=OFF;
}// if

end=OFF; //выходить не надо
do{/* опрос 'мыша' */

 mouse_draw();

 mouse_cpk(&keys);/* какие кнопки сейчас нажаты */
 mouse_lpk(&left,&right);/* какие кнопки были нажаты */

// printf("%d %d %d   \r",keys,left,right);


 pos_mm=getpos_mouse_obj(menu_4,1,1);// выяснение позиции "мыши"
// printf("%d %d %d %d   \r",spf_dir_max,spf_reg_max,posf_dir,posf_reg);
// printf("%d   \r",pos_mm);

 if(kbhit()!=0)
 {
 switch(getch())
 {/* эмуляция 'мыша' */
  case 13:/* Enter */
/*  case 45:/* - левая */*/
       lpress_lb=1;
       cpress_lb=1;
  break;
  case 43:/* + пpавая */
       lpress_rb=1;
       cpress_rb=1;
  case 27:/* Esc */
    del_spf();
    Restore_Image(menu_4,"image.tmp");// восстанавливаем место под меню
    del_pic(&menu_4);
    return NULL;
/*  break;*/
 }/* switch */

 }//if
 if(left)
 {
   long curtime=biostime(0,0l);
/*   if(labs(predtime-curtime) < 10) dc=ON;
      else                         dc=OFF;
*/
/*   if(labs(predtime-curtime) < 10) pos_mm=3;//ДА*/
   if((labs(predtime-curtime) < 10)&&
      ((pos_mm>=9)&&(pos_mm<=13))) pos_mm=3;// ДА
   predtime=curtime;
/*   printf("\r%ld  ",labs(predtime-curtime));*/
/*   printf("\r%d   ",dc);*/
 }/* if */

 if(right)
 { pos_mm=4; //НЕТ
   left=1;
 }/* if */

/*
 if(dc)/* двойной щелчок */
 {
    switch(pos_mm)
    {
    }//switch
 }/* if двойной щелчок */
*/

 if(left > 0) //нажата левая кнопка
   {
    switch(pos_mm)
    {
     case  3: //ДА
     case  4: //НЕТ
	    visual_on(menu_4,pos_mm,98);
     case 34:
	    del_spf();
	    Restore_Image(menu_4,"image.tmp");// восстанавливаем место под меню
	    del_pic(&menu_4);
	    if(pos_mm == 3) return fname; //ДА
            return NULL;
//     break;
     case 5://удаление файла

           // ничего не удаляем!!! Это кнопка ПОМОЩИ!
	   visual_on(menu_4,pos_mm,98);
/*
	   visual_on(menu_4,pos_mm,98);
           setcolor_obj_num(menu_4,1,2,LIGHTRED,SHOW);
           setcolor_obj_num(menu_4,2,2,WHITE   ,SHOW);
           sprintf(s," Файл '%s' удалять ?",fname);
           outtext_obj_num(menu_4,2,2,LIGHTRED,s,SHOW);
           ibell();
           getsize_obj_num_reset();
           getsize_obj_num(menu_4,1,4,&xmin,&ymin,&xmax,&ymax);
           mouse_setxy(xmax-5,ymax-5);/* мышь на "НЕТ" */

 do{ /* подтверждение удаления */
   pos_mm=getpos_mouse_obj(menu_4,1,1);// выяснение позиции "мыши"

   mouse_lpk(&left,&right);/* какие кнопки были нажаты */

   if(left > 0) //нажата левая кнопка
     {
      switch(pos_mm)
      {
       case 3://да
             visual_on(menu_4,pos_mm,98);// визуализация нажатия кнопки
             sprintf(s,"%s\\%s",pck.div_path,fname);
             unlink(s);
       case 4://нет
             visual_on(menu_4,pos_mm,98);// визуализация нажатия кнопки
             setcolor_obj_num(menu_4,1,2,LIGHTGRAY,NO_SHOW);
             setcolor_obj_num(menu_4,2,2,BLACK    ,NO_SHOW);
             p1=ON;  // освежить имена файлов
             p7=ON;  // освежить имя рабочего файла
             goto start;
    }//switch
   }// нажата левая кнопка

 }while(1!=0);// подтверждение удаления
*/
     break; //удаление файла

     case 8:
           // при загрузке нового имени не вводится
	   if(mode == LOAD_obj) break;
	   *(strchr(fname,'.'))='\0';
	   mouse_off();
	   sprintf(fname,"%s",EditString_obj_num(menu_4,14,1,fname,8+1));
	   mouse_on();
	   strcat(fname,Ext);
	   p7=ON;  // освежить имя рабочего файла
	   end=ON; //надо выходить
     break;
     case 9: if(pos_mm ==  9) d=0;
     case 10:if(pos_mm == 10) d=1;
     case 11:if(pos_mm == 11) d=2;
     case 12:if(pos_mm == 12) d=3;
     case 13:if(pos_mm == 13) d=4;

	   sprintf(fname,"%s",spf_reg[posf_reg+d].fname);
			  del_mouse=ON;// надо отключить мышь
			  end=ON; // надо выходить
			  p7=ON;  // освежить имя рабочего файла

           if(
              (
               (mode == LOAD_obj) &&
               (
	        (pck.Mode_load == mode_load_new_tbg)||
	        (pck.Mode_load == mode_load_wrk_tbg)||
	        (pck.Mode_load == mode_load_fon_tbg)
               )
              )
              ||
              (
               (mode == SAVE_obj) &&
               (
                (pck.Mode_save == mode_save_all_tbg)||
		(pck.Mode_save == mode_save_ram_tbg)||
                (pck.Mode_save == mode_save_ge_tbg )
               )
              )
             ){
	      char DisplayName[80];

              strcpy(DisplayName,pck.div_path);
	      strcat(DisplayName,"\\");
	      strcat(DisplayName,fname);
	      hide_tbl();
	      load_file(&Display,DisplayName);
         if(error_tbg != ER_OK){
            break;
         }/* if */
	      restore_tbl();
         if(error_tbg != ER_OK){
            break;
         }/* if */
         outpic_obj_num(menu_4,77,2);
         if(error_tbg != ER_OK){
            break;
         }/* if */
         setviewport_obj_num(menu_4,77,1);
         if(error_tbg != ER_OK){
            break;
         }/* if */
         outpic_Display(Display);
         if(error_tbg != ER_OK){
            break;
         }/* if */
         setviewport_obj_num(NULL,0,0);
         if(error_tbg != ER_OK){
            break;
         }/* if */
	      del_pic(&Display);
         if(error_tbg != ER_OK){
            break;
         }/* if */
         }// if
     break;
     case 15:
           sprintf(pck.sbl,"%s",EditString_obj_num(menu_4,9,1,pck.sbl,8+1));
           posf_dir=posf_reg=0;
           del_mouse=ON;// надо отключить мышь
           p4=ON;  // освежить шаблон
           p1=ON;  // освежить имена файлов
           goto start; //надо выходить на перебор файлов по новому
                       // шаблону
//     break;

     case 20:if(pos_mm == 20) d=0;
     case 21:if(pos_mm == 21) d=1;
     case 22:if(pos_mm == 22) d=2;
     case 23:if(pos_mm == 23) d=3;
     case 24:if(pos_mm == 24) d=4;
     case 25:if(pos_mm == 25) d=5;
	   sprintf(s,"\\%s",spf_dir[posf_dir+d].fname);
           if((s[1]=='.')&&(s[2]==0)  ) break;//вход в саму себя не нужен
                                        //выход из поддиректории
           if((s[1]=='.')&&(s[2]=='.')) *(strrchr(pck.div_path,'\\'))='\0';
             else                       strcat(pck.div_path,s);//новая поддиректория
           posf_dir=posf_reg=0;
           del_mouse=ON;// надо отключить мышь
           p2=ON;  // освежить директории
           p1=ON;  // освежить имена файлов
           p6=ON;  // освежить путь
           goto start; //надо выходить на перебор файлов по новому
                       // пути
//     break;
     case 28:if(pos_mm == 28) d=0;
     case 29:if(pos_mm == 29) d=1;
     case 30:if(pos_mm == 30) d=2;
     case 31:if(pos_mm == 31) d=3;
     case 32:if(pos_mm == 32) d=4;
     case 33:if(pos_mm == 33) d=5;
	   sprintf(pck.div_path,"%s",spf_drv[posf_drv+d].fname);
           posf_dir=posf_reg=0;
           del_mouse=ON;// надо отключить мышь
           p3=ON;  // освежить диски
           p2=ON;  // освежить директории
           p1=ON;  // освежить имена файлов
           p6=ON;  // освежить путь

           goto start; //надо выходить на перебор файлов по новому
                       // диску

//     break;


    }//switch

   }// if нажата левая кнопка

   if(keys==1) {// удерживается левая кнопка
     switch(pos_mm)
     {
      case  6:
      case  7:
      case 18:
      case 19:
      case 26:
      case 27:
            visual_on(menu_4,pos_mm,98);
      break;
     }// switch

     switch(pos_mm)
     {
      case 7:
             posf_reg++;if(posf_reg>=spf_reg_max) posf_reg=spf_reg_max-1;
             if(posf_reg < 0) posf_reg=0;
             end=ON; //надо выходить
             p1=ON;  // освежить имена файлов
      break;
      case 6:
             posf_reg--;if(posf_reg<0) posf_reg=0;
             end=ON; //надо выходить
             p1=ON;  // освежить имена файлов
      break;
      case 19:
             posf_dir++;if(posf_dir>=spf_dir_max) posf_dir=spf_dir_max-1;
             if(posf_dir < 0) posf_dir=0;
             end=ON; // надо выходить
             p2=ON;  // освежить директории
      break;
      case 18:
             posf_dir--;if(posf_dir<0) posf_dir=0;
             end=ON; // надо выходить
             p2=ON;  // освежить директории
      break;
      case 27:
             posf_drv++;if(posf_drv>=spf_drv_max) posf_drv=spf_drv_max-1;
             if(posf_drv < 0) posf_drv=0;
             end=ON; // надо выходить
             p3=ON;  // освежить диски
      break;
      case 26:
             posf_drv--;if(posf_drv<0) posf_drv=0;
             end=ON; // надо выходить
             p3=ON;  // освежить диски
      break;

     }//switch
//     delay(60);
    }//if удерживается левая кнопка




}while(end!=ON);/* опрос 'мыша' */
}while(1!=0);/* основной опрос */

start:;
}while(1!=0);/* самый большой цикл */

}/* menu_sl */
/****************************************************************************/
/* 1990-92(C) Павельев П.И. */
int outpic_Display(OBJECT **obj)
{
  int i;

  if(obj==NULL){printf("'outpic_Display' нет рисунка\n\007");
	     return(-1);};/* if */

  set_MaxXY(obj);// установка параметров для вывода в окно

  for(i=1;i<=((struct HTBG*)*(obj))->count_ei;i++)
  {
/*
   if(
       ( *((char*)*(obj+i)) == OBJ_txt)&&
       (((struct tx*)*(obj+i))->txfnt == DEFAULT_FONT)
     ) continue;
   if(
      (*((char*)*(obj+i)) == OBJ_fld)
     ) continue;
*/
   if(
      (( *((char*)*(obj+i)) == OBJ_txt)&&
       (((struct tx*)*(obj+i))->txfnt == DEFAULT_FONT)
      )||
      (*((char*)*(obj+i)) == OBJ_fld)
     ) continue;
   predtime=biostime(0,0L);

   outpic_pn(obj,i);


  }/* for */
  return 0;

}/* outpic_Display */
/****************************************************************************/
int CompareName( const void *a, const void *b)
{
   return( strcmp((char *)a,(char *)b) );
}
/****************************************************************************/
