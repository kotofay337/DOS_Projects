// red5_002.cpp

#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <conio.h>
#include <alloc.h>
#include <graphics.h>
#include <keyboard.h>

#include "red5.h"

void help_f1()
{
 OBJECT **tmp=NULL;

 char s[82];
 char str_search[100];
 char *chptr1,*chptr2,*chptr3;
 FILE *fp;
 int numstr,i;
 short int xmin1,ymin1,xmax1,ymax1;
 int col;

// имя файла подсказки/text/
char t[]="tbg_hlp.txt",BIOS_mode;

if(ARGV == 0) sprintf(s,"%s",t);
  else        sprintf(s,"%s\\%s",ARGV,t);

// sprintf(s,"%s\\tbg_hlp.txt",ARGV);

 if((fp=fopen(s,"r+t"))==NULL)
   { Warning("Не удалось открыть файл tbg_hlp.txt");return;}// if

 if(load_file(&tmp,"tbg_hlp.tbg")<0) {fclose(fp);return;}//не успех с файлом подсказки

 asm{
   mov ah,0Fh
   int 10h
   mov BIOS_mode,al
 }//asm



 if((BIOS_mode == 0x10)||
    (BIOS_mode == 0x12))
    setactivepage(1);
 else
    Store_Image(tmp,"image.tmp");// сохраняем место под Help

 outpic_all(tmp);

 rewind(fp);

 sprintf(str_search,"page_%d",pos_ms);//строка для поиска
// printf("%d  \r",pos_ms);


 do{
   if(fgets(s,81,fp)==NULL) goto end_f;
//   printf("%s\n",s);
 }while(strstr(s,str_search)==NULL);
// printf("%s\n",str_search);

 do{  /* ищем информацию help */
   if(fgets(s,81,fp)==NULL) goto end_f;
   chptr1=strtok(s,"#");/* первая подстрока */

   if(strstr(chptr1,"end")==NULL)
     {
      numstr=atoi(chptr1);
      chptr2=strtok(NULL,"#");/* цвет */
//      color=atoi(chptr2);
      chptr3=strtok(NULL,"#");/* текст */
      *(chptr3+((strlen(chptr3)-1)))=0;/* дописываем конец строки текста */

//      printf("qwqwq\n");
      getsize_obj_num_reset();
      getsize_obj_num(tmp,1,numstr,&xmin1,&ymin1,&xmax1,&ymax1);

      col=EGA_BLUE;
      if(strstr(chptr2,"BLACK")!=NULL) col=EGA_BLACK;
      if(strstr(chptr2,"BLUE")!=NULL) col=EGA_BLUE;
      if(strstr(chptr2,"GREEN")!=NULL) col=EGA_GREEN;
      if(strstr(chptr2,"CYAN")!=NULL) col=EGA_CYAN;
      if(strstr(chptr2,"RED")!=NULL) col=EGA_RED;
      if(strstr(chptr2,"MAGENTA")!=NULL) col=EGA_MAGENTA;
      if(strstr(chptr2,"BROWN")!=NULL) col=EGA_BROWN;
      if(strstr(chptr2,"LIGHTGRAY")!=NULL) col=EGA_LIGHTGRAY;
      if(strstr(chptr2,"DARKGRAY")!=NULL) col=EGA_DARKGRAY;
      if(strstr(chptr2,"LIGHTBLUE")!=NULL) col=EGA_LIGHTBLUE;
      if(strstr(chptr2,"LIGHTGREEN")!=NULL) col=EGA_LIGHTGREEN;
      if(strstr(chptr2,"LIGHTCYAN")!=NULL) col=EGA_LIGHTCYAN;
      if(strstr(chptr2,"LIGHTRED")!=NULL) col=EGA_LIGHTRED;
      if(strstr(chptr2,"LIGHTMAGENTA")!=NULL) col=EGA_LIGHTMAGENTA;
      if(strstr(chptr2,"YEELOW")!=NULL) col=EGA_YELLOW;
      if(strstr(chptr2,"WHITE")!=NULL) col=EGA_WHITE;

      setcolor(col);
      settextstyle(6,0,4);
      outtextxy(xmin1,ymax1,chptr3);
     }// if

 }while(strstr(chptr1,"end")==NULL);

// printf("%lu   \r",(unsigned long)coreleft());


 if((BIOS_mode == 0x10)||
    (BIOS_mode == 0x12)){

      setactivepage(0);
      Split(0);
      setvisualpage(1);
      for(i=0; i < 302; i+=3){
         Split(i);
      }// for

 }// if

 press_key();

 if((BIOS_mode == 0x10)||
    (BIOS_mode == 0x12)){
   for(; i >= 0; i-=6){
      Split(i);
   }// for
   setvisualpage(0);
   Split(0xffff);
 }// if


end_f:;

 fclose(fp);

 if((BIOS_mode == 0x10)||
    (BIOS_mode == 0x12)){
 }// if
 else{
    Restore_Image(tmp,"image.tmp");// восстанавливаем место под Help
 }// elif

 del_pic(&tmp);

} //help_f1
//-----------------------------------------------------------------------
/*
void attrxy(char x,char y,unsigned char lenght,
			  unsigned char bg,
			  unsigned char fg)
{
	unsigned char atr,cx=x;
	atr=(bg<<4)|fg;
	while(lenght--)
		pokeb(0xB800,((160*y)+(cx++*2))-1,atr);
}/* attrfxy */
/* ----------------------------------------------------------------------- */
void show(char line)
{
	unsigned i=0,a=9;
	int tst=0,l,r;
	long time;

	while(tst==0)
	{
         if(bioskey(1)!=0) tst=bioskey(0);
          mouse_lpk(&l,&r);/* какие кнопки были нажаты */
          if((l!=0)||(r!=0)) break;
/*
         mouse_getkey(&key,0);/* левая кнопка */
         if(key > 0) tst=key;
         mouse_getkey(&key,1);/* правая кнопка */
         if(key > 0) tst=key;
*/
       for(i=0;i!=26;i++)
		 {
        attrxy(i*3,line,3,BLACK,a);
		  if(a++>=15) a=9;
		 }// for

		  time=biostime(0,0L);
		  while((biostime(0,0L)-time) < 2L);

	};/* while */

}/* show */
*/
/* ----------------------------------------------------------------------- */
void press_key(void)
{
  short int l,r,tst;

  tst=0;
  while(tst==0){
   if(bioskey(1)!=0)
      tst=bioskey(0);
   mouse_lpk(&l,&r);/* какие кнопки были нажаты */
   if((l!=0)||(r!=0))
	   break;
  };/* while */

}/* press_key */
/****************************************************************************/
void Split(int i){

   unsigned index=*((unsigned*)MK_FP(0x40,0x63));
   char *EGApt=*(char far **)(*(void far **)MK_FP(0x0000,0x04a8)),
        OVr,
	LCr,
	MSLr,
        BIOS_mode;

   asm{
      mov ah,0Fh
      int 10h
      mov BIOS_mode,al
   }//asm

   switch(BIOS_mode){
      case 0x10://EGA HI color
         EGApt+=0x480;
         break;
      case 0x12://VGA Hi color
         EGApt+=0x6C0;
         break;
   }// switch

   OVr =*(EGApt+0x11);
   MSLr=*(EGApt+0x13);
   LCr =*(EGApt+0x22);

   asm{
      mov     dx,index
      add     dx,6
   }// asm
   L01:;
   asm{
      in	al,dx
      test	al,8
      jnz	L01
   }//asm
   L02:;
   asm{
      in	al,dx
      test	al,8
      jz	L02

      sub       dx,6

      mov	ax,i
      mov	bh,ah
      mov	bl,bh
      and	bx,0201h
      mov	cl,4
      shl	bx,cl
      shl	bh,1
      mov	ah,al
      mov       LCr,ah
      push      ax
      push      bx
}//asm
      *(EGApt+0x22)=LCr;
asm{
      pop       bx
      pop       ax
      mov	al,18h
      out	dx,ax
      mov       al,OVr
      mov	ah,al
      and	ah,11101111b
      or	ah,bl
      mov	al,7
      out	dx,ax
      mov       OVr,ah
      push      ax
      push      bx
}
      *(EGApt+0x11)=OVr;
asm{
      pop      bx
      pop      ax
      mov       al,MSLr
      mov	ah,al
      and	ah,10111111b
      or	ah,bh
      mov	al,9
      out	dx,ax
      mov       MSLr,ah
}//asm
      *(EGApt+0x13)=MSLr;

}// Split
//---------------------------------------------------------------------------
