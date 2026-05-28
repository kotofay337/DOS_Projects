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

#define WAIT 150

static int duga=OFF;//работа с дугой:нужно для перевывода
//static int fnt_ind=1;
static int old_pos=0;

#define SZ 24
static int Zoom = 0,
           XZoom = 2;

#define XSIZERULER 20
#define YSIZERULER 20

int sRulers = 0;
/* ----------------------------------------------------------------------- */
void ZoomOn(int xzoom){
   XZoom = xzoom;
   Zoom = 1;
}/* ZoomOn */
/* ----------------------------------------------------------------------- */
void ZoomOff(void){
   XZoom = 2;
   Zoom = 0;
}/* ZoomOff */
/* ----------------------------------------------------------------------- */
void ZoomDisplay (void){
   static int x_mouse_old, y_mouse_old, xp, yp;

   if((y_mouse != y_mouse_old) || (x_mouse != x_mouse_old)){

      if(sRulers){
         mouse_off();
         setwritemode(XOR_PUT);
         setcolor(EGA_YELLOW);
         setlinestyle(SOLID_LINE, 0, 1);
         line(x_mouse_old, 0, x_mouse_old, YSIZERULER - 10);
         line(0, y_mouse_old, XSIZERULER - 10, y_mouse_old);
         line(x_mouse, 0, x_mouse, YSIZERULER - 10);
         line(0, y_mouse, XSIZERULER - 10, y_mouse);
         setwritemode(COPY_PUT);
         mouse_on();
      }/* if */

      y_mouse_old = y_mouse;
      x_mouse_old = x_mouse;

      if(Zoom){
         for(yp = 0;yp < SZ;yp++){
            for(xp = 0;xp < SZ;xp++){
               mouse_getxy(&x_mouse,&y_mouse);
               if((y_mouse != y_mouse_old) || (x_mouse != x_mouse_old)){
                  goto dn;
               }/* if */
               setfillstyle(SOLID_FILL, getpixel(xp + (x_mouse - SZ / 2),
                                                 yp + (y_mouse - SZ / 2)));
               bar(xp * XZoom, yp * XZoom,
                   xp * XZoom + XZoom, yp * XZoom + XZoom);
            }/* for */
         }/* for */
      }/* if */
   }/* if */
   dn:;
}/* ZoomDisplay */
/* ----------------------------------------------------------------------- */
void HorizRuler(void){
   int maxx = getmaxx();
   int maxy = YSIZERULER;
   int x;
   setcolor(EGA_DARKGRAY);
   setlinestyle(SOLID_LINE, 0, 1);
   setfillstyle(SOLID_FILL, EGA_LIGHTGRAY);
   settextjustify(CENTER_TEXT, TOP_TEXT);
   settextstyle(SMALL_FONT, HORIZ_DIR, 2);
   mouse_off();
   bar(0, 0, maxx, maxy);
   for(x = 0;x < maxx;x++){
      if(x % 20 == 0){
         char s[16];
         line(x, 0, x, maxy - 10);
         sprintf(s, "%d", x);
         outtextxy(x, maxy - 9, s);
      }/* if */
      else{
         if(x % 10 == 0){
            line(x, 0, x, maxy / 3);
         }/* if */
         else{
            if(x % 2 == 0){
               line(x, 0, x, maxy / 4);
            }/* if */
         }/* elif */
      }/* elif */
   }/* for */
   setwritemode(XOR_PUT);
   setcolor(EGA_YELLOW);
   line(x_mouse, 0, x_mouse, YSIZERULER - 10);
   setwritemode(COPY_PUT);
   mouse_on();
}/* HorizRuler */
/****************************************************************************/
void VertRuler(void){
   int maxy = getmaxy();
   int maxx = XSIZERULER;
   int y;
   setcolor(EGA_DARKGRAY);
   setlinestyle(SOLID_LINE, 0, 1);
   setfillstyle(SOLID_FILL, EGA_LIGHTGRAY);
   settextjustify(LEFT_TEXT, CENTER_TEXT);
   settextstyle(SMALL_FONT, HORIZ_DIR, 2);
   mouse_off();
   bar(0, 0, maxx + 3, maxy);
   for(y = 0;y < maxy;y++){
      if(y % 20 == 0){
         char s[16];
         line(0, y, maxx - 10, y);
         sprintf(s, "%d", y);
         outtextxy(maxx - 8, y, s);
      }/* if */
      else{
         if(y % 10 == 0){
            line(0, y, maxx / 3, y);
         }/* if */
         else{
            if(y % 2 == 0){
               line(0, y, maxx / 4, y);
            }/* if */
         }/* elif */
      }/* elif */
   }/* for */
   setwritemode(XOR_PUT);
   setcolor(EGA_YELLOW);
   line(0, y_mouse, XSIZERULER - 10, y_mouse);
   setwritemode(COPY_PUT);
   mouse_on();
}/* VertRuler */
/****************************************************************************/
/* ----------------------------------------------------------------------- */
void Rulers(void){
   if(sRulers == 0){
      return;
   }/* if */
   HorizRuler();
   VertRuler();
}/* Rulers */
/* ----------------------------------------------------------------------- */
int obrab_mouse()
{
 skankod=0;

 mouse_getxy(&x_mouse,&y_mouse);/* берем координаты мыши */
 mouse_cpk(&key_mouse);/* какие кнопки сейчас нажаты */
 mouse_lpk(&getkey_left,&getkey_right);/* какие кнопки были нажаты */

/* printf("\r %x %x   %x  ",getkey_left,getkey_right,key_mouse);*/

 pos_ms=pos_mouse();/* вычисление номера позиции */

 // printf("\r %u  ",pos_ms);

    if(pos_ms == 0){ // В окне pисования будет кpестик а в меню стpелка.
       if(old_pos != CROSS){
	       mouse_mask((unsigned short int*)markers[CROSS],7,7);
	       old_pos=CROSS;
       }//if
    }//if
    else{
       if(old_pos != ARROW){
	       mouse_mask((unsigned short int*)markers[ARROW],0,0);
	       old_pos=ARROW;
       }//if
    }// elif

 if((komand==KOM_elip) || (komand==KOM_felip)){
    if(pren(menu_1,12,1,x_mouse,y_mouse) == ON) pos_ms=161;
    if(pren(menu_1,12,2,x_mouse,y_mouse) == ON) pos_ms=162;
 }// if

// printf("%d   \r",pos_ms);
indic_mess();//вывод сообщений

// визуализация нажатия кнопки
if(getkey_left > 0){
   switch(pos_ms)
   {
    case  4:
    case  2:
    case  6:
    case 30:
    case 31:
    case 32:
    case 33:
    case 37:
    case 38:
    // средняя строка
    case 43:
    case 51:
    case 53:
         visual_on(menu_1,pos_ms,98);
    break;
   }// switch
}// if left

// визуализация нажатия кнопки
if(getkey_right > 0){
   switch(pos_ms)
   {
    case  1:
    case  5:
    case  8:
         visual_on(menu_1,pos_ms,98);
    break;
   }// switch
}// if right

// визуализация нажатия кнопки
if((getkey_right > 0 )||(getkey_left > 0)){
   switch(pos_ms)
   {
    case 10:
    case 12:
    // верхняя строка
    case 40:
    case 48:
    case 50:
    case 54:
    case 49:
    case 46:
    case 47:
    // средняя строка
//    case 41:
//    case 42:
    // нижняя  строка
    case 55:
    case 52:
    case 44:
    case 45:
         visual_on(menu_1,pos_ms,98);
    break;
   }// switch
}// if left || right


switch(pos_ms)
{
 case 8:/* выравнивание */
        if(getkey_right>0){
          panel_mode(&pn_v_t);
          return NORESTART;
       }// if
	  break;
 case 10:/* стиль */
       if(getkey_right > 0)
       {
          panel_mode(&pn_mf_t);
          return RESTART;
       }// if
       if(getkey_left > 0)// ориентация
	   {
		  if(++pck.dir_t > 1) pck.dir_t=0;
		  return RESTART;
	   }// if
	  break;
 case 12:/* размер шрифта */
       if(getkey_left>0) /*  - */
		 {
		  if(--pck.siz_t < 0) pck.siz_t=10;
		  return(RESTART);
		 };
	   if(getkey_right>0) /*  + */
		 {
		  if(++pck.siz_t > 10) pck.siz_t=0;
		  return(RESTART);
		 };
	  break;
};/* switch(pos_ms) */

switch(pos_ms)
{
 case 0:
       ris_ramka();
      break;

 case 2:  /* Copyright */
       if(getkey_left>0){
          panel_mode(&pn_mr);
          return NORESTART;
       }// if
      break;
 case 1: /* цвет линии */
       if(getkey_right>0){
          panel_mode(&pn_lst);
          return NORESTART;
       }// if
      break;
 case 4: /* помощь */
       if(getkey_left>0)
	 {skankod=_F1_;return(RESTART);
	 };
      break;
 case 5: /* тип закраски */
       if(getkey_right>0){
          panel_mode(&pn_fst);
          return NORESTART;
       }// if
      break;
 case 6:/* выход в Dos */
       if(getkey_left>0)
         {
          skankod=_alt_x_;
          return RESTART;
         }//if
      break;
 case 13:
       /* линия */
        if(getkey_right > 0){
            pck.Mode_line = pck.Mode_line == KOM_line ? KOM_pipeline : KOM_line;
            mouse_off();
            outtext_obj_num(menu_1, 3, 10, EGA_LIGHTGRAY, pck.Mode_line == KOM_line ? "Ln" : "P", SHOW);
            mouse_on();
        }/* if */
        if(getkey_left > 0)
         {
			   if(komand == pck.Mode_line)
			      komand = OFF;
            else
				   komand = pck.Mode_line;

          off_komand();//действия при отключении команд
          return RESTART;/* перевывод меню */
       }//if
      break;
 case 14:
       /* прямоугольник */
       if(getkey_left>0)
         {if(komand==KOM_rect) komand=OFF;
            else               komand=KOM_rect;
          off_komand();//действия при отключении команд
          return RESTART;/* перевывод меню */
       };
      break;
 case 15:
       /* закрашенный прямоугольник */
       if(getkey_left > 0)
         {if(komand==KOM_bar) komand=OFF;
            else               komand=KOM_bar;
          off_komand();//действия при отключении команд
          return RESTART;/* перевывод меню */
       }//if
      break;
 case 16:
       /* эллипс */
       if(getkey_right > 0){
           pck.Mode_elip = pck.Mode_elip == KOM_elip ? KOM_felip : KOM_elip;
           mouse_off();
           outtext_obj_num(menu_1, 3, 50, EGA_LIGHTGRAY, pck.Mode_elip == KOM_elip ? "El" : "Fe", SHOW);
           mouse_on();
       }/* if */
       if(getkey_left>0){

		    if(komand == pck.Mode_elip)
			    komand = OFF;
          else
			    komand = pck.Mode_elip;

          off_komand();//действия при отключении команд
          return RESTART;/* перевывод меню */
	    };
       break;
 case 17:
       /* текст */
       if(getkey_left>0)
	 {if(komand==KOM_txt) komand=OFF;
            else               komand=KOM_txt;
          off_komand();//действия при отключении команд
          return RESTART;/* перевывод меню */
	 };
      break;
 case 18:
       /* заливка */
       if(getkey_left>0)
	 {if(komand==KOM_fld) komand=OFF;
            else               komand=KOM_fld;
          off_komand();//действия при отключении команд
          return RESTART;/* перевывод меню */
	 };
      break;
 case 19:
       /* фон */
       if(getkey_left>0)
         {if(komand==KOM_bkgr) komand=OFF;
            else               komand=KOM_bkgr;
          off_komand();//действия при отключении команд
          return RESTART;/* перевывод меню */
	 };
      break;
 case 20:
       /* макропримитив */
       if(getkey_left>0)
         {if(komand==KOM_macro) komand=OFF;
            else               komand=KOM_macro;
          off_komand();//действия при отключении команд
          return RESTART;/* перевывод меню */
	 };
      break;
 case 30:
       /* сохранение в файле на диске */
       if(getkey_left>0)  if(pp_pos30()<0) {skankod=_F10_;return(RESTART);}
       if(getkey_right>0){
          panel_mode(&pn_ms);
          return RESTART;
       }// if
      break;
 case 31:
       /* загрузка файла изображения с диска */
       if(getkey_left>0)
		    if(pp_pos31() < 0){
			    skankod=_F10_;
				 return(RESTART);
			 }
       if(getkey_right>0){
          panel_mode(&pn_ml);
          return RESTART;
       }// if
      break;
 case 32:
       if(getkey_left >0) {ybcur();pic.Korr();ybcur();return NORESTART;
       }//if
       if(getkey_right>0){
          panel_mode(&pn_mk);
          return RESTART;
       }// if
      break;
 case 33:
       /* удаление */
       if(getkey_left>0)
         if(pp_pos33()<0)
           {skankod=_F10_;return(RESTART);};
       if(getkey_right>0){
          panel_mode(&pn_md);
          return RESTART;
       }// if
      break;
 case 34:
       /* копировать */
       if(getkey_left>0)
         {if(komand==KOM_copy)
            {
             if(ente1==ON)/* было рисование:отключены указатель и мышь */
               {ybcur();/* показываем указатель */
                mouse_on();/* включаем мышь */
               };
			 ente1=OFF;komand=OFF;
            }
		    else{komand=KOM_copy;};
          return RESTART;/* перевывод меню */
         };//левая
       if(getkey_right>0){
          panel_mode(&pn_mc);
          return RESTART;
       }// if
      break;
 case 35:
       /* сдвинуть */
       if(getkey_left>0)
	 {if(komand==KOM_move)
            {
             if(ente1==ON)/* было рисование:отключены указатель и мышь */
               {ybcur();/* показываем указатель */
                mouse_on();/* включаем мышь */
               };
			 ente1=OFF;komand=OFF;
            }
		    else{komand=KOM_move;};
          return RESTART;/* перевывод меню */
         };//левая
       if(getkey_right>0){
          panel_mode(&pn_mm);
          return RESTART;
       }// if
      break;
 case 36:
       /* Масштабирование */
       if(getkey_left>0)
	 {if(komand==KOM_mash)
	    {
	     if(ente1==ON)/* было рисование:отключены указатель и мышь */
	       {ybcur();/* показываем указатель */
            mouse_on();/* включаем мышь */
	       };
			ente1=OFF;komand=OFF;
	    }
	 else{komand=KOM_mash;};
	  return RESTART;/* перевывод меню */
	 };
      break;
 case 37:
       /* Перевывод изображения */
       if(getkey_left>0)
         {skankod=_F10_;
//         pos_ms_pred-=1;mn1_out=0;
          return(RESTART);
       }// if
       if(getkey_right>0){
          panel_mode(&pn_mh);
          return RESTART;
       }// if
      break;
 case 38:
       /* Индикация */
       if(getkey_left>0) {pp_indic();return NORESTART;
       }// if
       if(getkey_right>0){
          panel_mode(&pn_mi);
          return RESTART;
       }// if
 case 39: // ЛУПА
       if(getkey_left>0)
	 {if(komand==KOM_raio)
            {
             if(ente1==ON)/* было рисование:отключены указатель и мышь */
               {ybcur();/* показываем указатель */
                mouse_on();/* включаем мышь */
               };
			 ente1=OFF;komand=OFF;
            }
		    else{komand=KOM_raio;};
          return RESTART;/* перевывод меню */
         };//левая
       if(getkey_right > 0) { // экран полностью
	 win.xa_min = 0;
	 win.ya_min = 0;
	 win.size_xya = SIZE_XY;
	 kf_size = 51;

         skankod=_F10_;// перевывод изображения
         return RESTART;/* перевывод меню */
       }// правая
      break; // ЛУПА

 case 40://Выбор типа элемента для поиска
      if(getkey_left >0){ DecTipSearch();  return RESTART; };
      if(getkey_right>0){ IncTipSearch();  return RESTART; };
      break;
 case 41:
 	   if((key_mouse!=0)&&(pic.GetMaxLE()<0)) pic.AddLE(0,0);
       // изменение NO
	   if(key_mouse==1)// left
         {
          visual_on(menu_1,pos_ms,98);
          pic.DI_NO_CLE(DEC);
//          delay(WAIT);
          return(RESTART);
         }// if
	   if(key_mouse==2)// right
         {
          visual_on(menu_1,pos_ms,98);
          pic.DI_NO_CLE(INC);
//          delay(WAIT);
          return(RESTART);
         }// if
	  break;
 case 42:
 	   if((key_mouse!=0)&&(pic.GetMaxLE()<0)) pic.AddLE(0,0);
       // изменение LN
       if(key_mouse==1)// left
         {
          visual_on(menu_1,pos_ms,98);
          pic.DI_LN_CLE(DEC);
//          delay(WAIT);
          return(RESTART);
         }// if
       if(key_mouse==2)// right
         {
          visual_on(menu_1,pos_ms,98);
          pic.DI_LN_CLE(INC);
//          delay(WAIT);
          return(RESTART);
         }// if
	  break;

/*
 case 41:
 	   if((key_mouse!=0)&&(pic.GetMaxLE()<0)) pic.AddLE(0,0);
       // изменение NO
	   if(key_mouse==1)// left
         { pic.DI_NO_CLE(DEC);delay(WAIT);return(RESTART);};
	   if(key_mouse==2)// right
         { pic.DI_NO_CLE(INC);delay(WAIT);return(RESTART);};
	  break;
 case 42:
 	   if((key_mouse!=0)&&(pic.GetMaxLE()<0)) pic.AddLE(0,0);
       // изменение LN
       if(key_mouse==1)// left
         { pic.DI_LN_CLE(DEC);delay(WAIT);return(RESTART);};
       if(key_mouse==2)// right
         { pic.DI_LN_CLE(INC);delay(WAIT);return(RESTART);};
	  break;
*/
 case 43: /* Установка масштаба окна по тексту */
       if(getkey_left  > 0) {
         pic.Set_kf_size();
         skankod=_F10_;// перевывод изображения
         return(RESTART);
       }// if
      break;
 case 44: /* изменение номера текущего графического элемента */
       if(getkey_left  > 0) {
/*
          if(pic.GetTip() == TIP_pipeline){
             pic.SetPipeLineColor(pck.col_lt);
             pic.ShowCur();
             pic.SetPipeLineColor(-1);
          }/* if */
*/
		    pic.DecPN();return(RESTART);
		 };
       if(getkey_right > 0) {
/*
          if(pic.GetTip() == TIP_pipeline){
             pic.SetPipeLineColor(pck.col_lt);
             pic.ShowCur();
             pic.SetPipeLineColor(-1);
          }/* if */
*/
		    pic.IncPN();return(RESTART);
		 };
      break;
 case 45:
       /* всего элементов */
       if(getkey_left  > 0) { pic.StartPN();return(RESTART); };
       if(getkey_right > 0) { pic.EndPN();  return(RESTART); };
      break;
 case 49://поиск NO в списке LGE
       if(getkey_left>0 ){ pic.Search_NO_Pred_LGE();return RESTART;};
       if(getkey_right>0){ pic.Search_NO_Succ_LGE();return RESTART;};
       break;
 case 46://поиск LN в списке CGE
       if(getkey_left>0 ){ pic.Search_LN_Pred_LGE();return RESTART;};
       if(getkey_right>0){ pic.Search_LN_Succ_LGE();return RESTART;};
       break;
 case 47: //поиск LN равного текущему в списке CGE
       if(getkey_left>0 ){ pic.Search_All_LN_Pred_LGE();return RESTART;};
       if(getkey_right>0){ pic.Search_All_LN_Succ_LGE();return RESTART;};
       break;
 case 48://поиск заданного типа
      if(getkey_left >0){ pic.Search_Tip_Pred_LGE();  return RESTART; };
      if(getkey_right>0){ pic.Search_Tip_Succ_LGE();  return RESTART; };
      break;
 case 55://перемещение по списку LE
      if(getkey_left >0){ pic.Succ();  return RESTART; };
      if(getkey_right>0){ pic.Pred();  return RESTART; };
      break;
 case 50://поиск очередного NO в списке LE
      if(getkey_left >0){ pic.Search_NO_Succ();  return RESTART; };
      if(getkey_right>0){ pic.Search_NO_Pred();  return RESTART; };
      break;
 case 54://поиск очередного LN для NO в списке LE
      if(getkey_left >0){ pic.Search_LN_Succ();  return RESTART; };
      if(getkey_right>0){ pic.Search_LN_Pred();  return RESTART; };
      break;
 case 52://переход в начало-конец списка LE
      if(getkey_left >0){ pic.End_LLE();  return RESTART; };
      if(getkey_right>0){ pic.Start_LLE();return RESTART; };
      break;
 case 51://удалить LE
      if(getkey_left >0){ pic.DelCLE();  return RESTART; };
      break;
 case 53://добавить LE
      if(getkey_left >0){ pic.AddLE(0,0);  return RESTART; };
      break;
 default:
      break;
};/* switch(pos_ms) */

set_attrib();

switch(komand)
{
 case KOM_line:
 case KOM_pipeline:
 case KOM_rect:
 case KOM_bar:
       if(pp_pos13()<0) return(RESTART);
      break;
 case KOM_elip:
 case KOM_felip:
       if(pos_ms==161)//начало дуги
         {
          if(key_mouse==1) {pic.DI_fi1(DEC);delay(WAIT);duga=ON;}//уменьшаем
          if(key_mouse==2) {pic.DI_fi1(INC);delay(WAIT);duga=ON;}//увеличиваем
          break;
         };
       if(pos_ms==162)//конец дуги
         {
          if(key_mouse==1) {pic.DI_fi2(DEC);delay(WAIT);duga=ON;}//уменьшаем
          if(key_mouse==2) {pic.DI_fi2(INC);delay(WAIT);duga=ON;}//увеличиваем
          break;
         };
      switch(komand){
         case KOM_elip:
            if(pp_pos16()<0) {return RESTART;};
            break;
         case KOM_felip:
            if(pp_pos161()<0) {return RESTART;};
            break;
      }/* switch */
//         {skankod=_F10_;pos_ms_pred=0;mn1_out=OFF;return RESTART;};
      break;
 case KOM_txt:
       if(pp_pos17()<0)
         {
          komand=OFF;
          off_komand();
          return(RESTART);
         };
      break;
 case KOM_fld:
       if(pp_pos18()<0)
         {
          skankod=_F10_; //перевывод изображения
          return(RESTART);
         };
      break;
 case KOM_bkgr:
       if(pp_pos19()<0)
         {
          skankod=_F10_; //перевывод изображения
          return(RESTART);
         };
      break;
 case KOM_macro:
       if(pp_pos20() < 0)
         {
          skankod=_F10_; //перевывод изображения
          return(RESTART);
         };
      break;
 case KOM_copy:
       if(getkey_left>0)  if(pp_pos34()<0) {skankod=_F10_;return(RESTART);};
      break;
 case KOM_move:
       if(pp_pos35()<0)
	 {
	  skankod=_F10_; //перевывод изображения
	  return(RESTART);
	 };
      break;
 case KOM_mash:
       if(pp_pos36()<0)
	 {
	  skankod=_F10_; //перевывод изображения
	  return(RESTART);
	 };
      break;

 case KOM_raio:
       if(pp_pos39()<0)
	 {
	  skankod=_F10_; //перевывод изображения
	  return(RESTART);
	 };
      break;

 default:/*Warning("Неизвестная команда.'obrab_mouse'");*/
      break;
};/* switch(komand) */

 return NORESTART;

}/* obrab_mouse */
/****************************************************************************/
void off_komand()
{
 if(korr_txt==ON) ybcur();
	korr_txt=OFF;//текст не введен
 if(ente1==ON)/* было рисование:отключены указатель и мышь */
   {ybcur();/* показываем указатель */
    mouse_on();/* включаем мышь */
    ente1=OFF;
   };

 if((ente_fld==ON)||(ente_bkgr==ON)||(duga==ON))
   {                                          //рисовали в момент отключения
    skankod=_F10_; //перевывод изображения
//    pos_ms_pred=-1;//смена сообщения
//    mn1_out=OFF;   //полный перевывод меню
    ente_fld=OFF; // не рисовали
    ente_bkgr=OFF;// не рисовали
    duga=OFF;     // не рисовали
   };
};// off_komand
/****************************************************************************/