#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <conio.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"
//--------------------------------------------------------------------
int otsek(int x1,int y1,int x2,int y2)//отсечение окном
{
  int xa_max = win.xa_min + win.size_xya;
  int ya_max = win.ya_min + win.size_xya;

  if(((x1 < win.xa_min)||(x1 > xa_max))||
     ((x2 < win.xa_min)||(x2 > xa_max))||
     ((y1 < win.ya_min)||(y1 > ya_max))||
     ((y2 < win.ya_min)||(y2 > ya_max))
    ){}  //отрезок возможно невидим
    else return Ok; //отрезок полностью видим

  if(((x1 < win.xa_min)&&(x2 < win.xa_min))||
     ((x1 >     xa_max)&&(x2 >     xa_max))||
     ((y1 >     ya_max)&&(y2 >     ya_max))||
     ((y1 < win.ya_min)&&(y2 < win.ya_min))
    )    return Fu;  //отрезок невидим
    else return Ok; //отрезок частично видим
}// otsek
//--------------------------------------------------------------------
/* (C) 1992 Чумак А.И. */
/* (C) 1992 Павельев П.И. */

#define ABORT   -1  //!!!!!!!!!!!

#pragma warn -par

int handler(int errval,int ax,int bp,int si)// обработчик ошибок железа
{
   printf("\007");
#ifndef DOSX286
   hardretn(ABORT);
#endif
   return ABORT;
}// handler

#pragma warn +par
//---------------------------------------------------------------------------
void refr(void) //перевывод экрана без отключения мыши и рамки
{
 if(pck.Fn_fon_pcx[0] == NULL)
   {// фона PCX нет
       cleardevice();
   }// фона PCX нет
   else{// фон PCX есть

        PCXHEADER hdr;

        if (pcxGetFileHeader(pck.Fn_fon_pcx,&hdr) == pcxSUCCESS) {
            if((hdr.x1!=0) || (hdr.y1!=0)||
               (hdr.x2!=MaxX) || (hdr.y2!=MaxY)
              ) cleardevice();
/*
    printf("\n\nHeader structure for %s:\n\n",s);
    printf("    manuf: %d\n",hdr.  manuf);
    printf("     hard: %d\n",hdr.   hard);
    printf("    encod: %d\n",hdr.  encod);
    printf("бит-на-px: %d\n",hdr.  bitpx);
    printf("       x1: %d\n",hdr.     x1);
    printf("       y1: %d\n",hdr.     y1);
    printf("       x2: %d\n",hdr.     x2);
    printf("       y2: %d\n",hdr.     y2);
    printf("     hres: %d\n",hdr.   hres);
    printf("     vres: %d\n",hdr.   vres);
    printf("    vmode: %d\n",hdr.  vmode);
    printf("плоскости: %d\n",hdr.nplanes);
    printf("лин-стрке: %d\n",hdr.  bplin);

    getch();
*/

        if((hdr.bitpx==8)&&(hdr.nplanes==1)&&(GraphDriver!=134)){
               Warning("Файл фона=VGA256 не соответсвует режиму редактора");
               pck.Fn_fon_pcx[0] = NULL;
               }
          else{// режим VGA256

      if (pcxGetFilePalette(pcxtype,pck.Fn_fon_pcx,pcxpal) == pcxSUCCESS) {
        /* Now set it */
        if (pcxSetDisplayPalette(pcxpal) != pcxSUCCESS) {
          printf("pcxSetDisplayPalette error: \n");
        }// if
      }// if

            if(pcxFileDisplay(pck.Fn_fon_pcx,hdr.x1,hdr.y1,0) != pcxSUCCESS) {
//                Warning("Что-то с файлом фона");
                cleardevice();
            }// if
        }// режим VGA256
        }//if
        else{
          pck.Fn_fon_pcx[0]=NULL; // удаляем фон PCX при неуспешной загрузке
          Warning("Что-то с файлом фона");
          cleardevice();
        }//else if
   }// фон PCX есть

   if(FonTbg != NULL){
      outpic_all(FonTbg);
     } // картинка фона TBG есть
     else{ // картинки фона TBG нет
      if(pck.Fn_fon_tbg[0] != NULL){ // имя файла есть
         hide_tbl();   // временное отключение библиотеки
         if(load_file(&FonTbg,pck.Fn_fon_tbg) < 0){
            pck.Fn_fon_tbg[0]=NULL;
           } // удаляем фон TBG при неуспешной загрузке
           else{outpic_all(FonTbg);
           }// else if
         restore_tbl();// восстановление библиотеки
      }// имя файла есть
     }// картинки фона TBG нет

/*
           hide_tbl();   // временное отключение библиотеки
           if(FonTbg != NULL) del_pic(&FonTbg); // удаляем предыдущий фон

*/

//long tim=biostime(0,0L);
	  if(pck.Mode_show==mode_show_all) pic.ShowAll();
	  if(pck.Mode_show==mode_show_no ) pic.ShowNO();
	  if(pck.Mode_show==mode_show_ln ) pic.ShowLN();

      if(pck.Mode_grid[mode_grid] == ON){

      int step_grid=STEP_GRID_DEFAULT;

      for(int y=0;y <= getmaxy();y+=step_grid)
         for(int x=0;x <= getmaxx();x+=step_grid)
            putpixel(x,y,EGA_DARKGRAY);

      }// if
//printf("%lf      \r",(double)(biostime(0,0L)-tim)/18.2);

}// refr
//---------------------------------------------------------------------------
void refr_t(void) //перевывод экрана с отключением мышы и рамки
{
	  mouse_off();
	  if(ind_move == 0) ybcur();

      refr(); //перевывод экрана
      if((pck.Mode_save == mode_save_all_pcx)||
         (pck.Mode_save == mode_save_ram_pcx))
      {
         pck.Mode_save = mode_save_all_tbg;
         char s[80];
         sprintf(s,"%s\\%s",pck.div_path,pck.Fn_work_pcx);
         pcxDisplayFile(s,xmin_pcx,ymin_pcx,xmax_pcx,ymax_pcx,0);
      }//if

      Rulers();

      // запоминаем место под главным меню
      getimage(xmin_m,ymin_m,xmax_m,ymax_m,menu);

	  if(ind_move == 0) ybcur();
	  if(ente > 0) ramka0();
	  pos_ms_pred=-1;//смена сообщения
	  mn1_out=OFF;
	  mouse_on();

}// refr_t
//---------------------------------------------------------------------------
void risp(int x,int y)
{
  if(ind_move==0) {if(xs<650)putimage(xs,ys,oper,XOR_PUT);};
  if(x<=3) x=8;/* 3 */
  if(y<=5) y=8;/* 5 */
  if(x>=MaxX) x=MaxX-8;/* 3 */
  if(y>=MaxY) y=MaxY-8;/* 5 */
  xs=x-3;ys=y-5;/* 4  5 */
  if(ind_move==0) putimage(xs,ys,oper,XOR_PUT);
}
/****************************************************************************/
void ybcur() { putimage(xs,ys,oper,XOR_PUT);}
/****************************************************************************/
void set_mode_xor(void)
{
		outportb(0x3ce,3);
		outportb(0x3cf,030);
}/* set_mode_xor */
/****************************************************************************/

