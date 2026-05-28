/* tbg_rtt1.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/


#include <math.h>
#ifndef __WIN32__
#include <graphics.h>
#endif
#include "tbg_tc.h" /* обьявления функций и данных */
/*--------------------------------------------------------------------------*/
/* (C) Чумак А.И. 1994 */
/*--------------------------------------------------------------------------*/
#ifndef __WIN32__
void WaitVert(void){ // ожидание обратного хода луча ЭЛТ

   unsigned short int index=*((unsigned short int*)MK_FP(0x40,0x63));

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
   };

}// WaitVert
#endif
/*--------------------------------------------------------------------------*/
/* (C) Чумак А.И. 1994 */
/*--------------------------------------------------------------------------*/
// ф-ия вращения точки
static void rotate_point(double a,double *Xt,double *Yt,double Xc,double Yc)
{

#ifndef __WIN32__
  static short int xa=0,ya=0;
#endif
  double dXt=(*Xt)-Xc,
         dYt=(*Yt)-Yc,
         sina,cosa,
         tx,ty;

  a /= 57.29577951472;

  sina = sin(a);
  cosa = cos(a);

  tx = dXt;
  ty = dYt;

  dXt = (tx * cosa) - (ty * sina);
  dYt = (tx * sina) + (ty * cosa);

#ifndef __WIN32__
  if(!ya){ // взять отношение сторон дисплея
     getaspectratio((int far *)&xa, (int far *)&ya);
  }// if

  // скорректировать "неквадратность" точки
  if((xa != 10000)||(ya != 10000)){
     dYt /= ((double)ya / 10000);
     dXt /= ((double)xa / 10000);
  }// if
#endif

  *Xt = Xc + dXt + .5;
  *Yt = Yc + dYt + .5;

}// rotate_point
/*--------------------------------------------------------------------------*/
/* (C) Чумак А.И. 1994 */
/*--------------------------------------------------------------------------*/
#ifndef __WIN32__
short int rotate_obj_num
  (OBJECT **obj,unsigned char numobj,unsigned char num,double a){
  /*
     a     -> угол поворота от исходного состояния
     num   -> центр вращения(нужен эллипс)
     num+1 -> исходное состояние
     num+2 -> состояние после поворота и показ
  */

  short int i,i1,i2,tst=-2,
      Ind,
      Ind1,
      Ind2;
  unsigned short int Search,
           Search1,
           Search2;
  short int xmax,ymax,xmin,ymin,xc,yc;
  double maxx=(double)getmaxx();
  double maxy=(double)getmaxy();
  struct tag_Logic *ptr_Logic;//=((struct HTBG*)*(obj))->Logic;
  struct ln *ptr_ln;

  if(obj == NULL){
     error_tbg = ER_NOTPIC;
     goto ret;
  }
  setstateTBG(obj, TBG_BUSY);

  error_tbg = ER_OK; /* нет ошибок */

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
  {/* нет картинки */
   tst = error_tbg = ER_NOTPIC;
   goto ret;
  }/* if */

  if(((struct HTBG*)*(obj))->Num_LE==-1)
  {/* нет логических элементов */
   tst = error_tbg = ER_NOTLE;
   goto ret;
  }/* if */

  set_MaxXY(obj);// установка параметров для вывода в окно

  Search =(unsigned short int)((numobj << 8)|(num++));
  Search1=(unsigned short int)((numobj << 8)|(num++));
  Search2=(unsigned short int)((numobj << 8)|(num  ));
  ptr_Logic=((struct HTBG*)*(obj))->Logic;
  i =BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  i1=BinSearch(Search1,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  i2=BinSearch(Search2,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);

  if((i != -1)&&(i1 != -1)&&(i2 != -1)){

	  Ind=ptr_Logic[i].Index_ei;
	  switch(*((char*)*(obj+Ind))){
           // получить координаты центра,
	     case OBJ_elip:
		     xc= DMX((long)((struct el*)*(obj+Ind))->elx);
		     yc= DMY((long)((struct el*)*(obj+Ind))->ely);
			  break;
	     case OBJ_line:
		     xc= DMX((long)((struct ln*)*(obj+Ind))->lnx);
		     yc= DMY((long)((struct ln*)*(obj+Ind))->lny);
			  break;
        default:
	        printf("<%s>'rotate_obj_num' центр поворота не эллипс\n\007",hTbg(obj)->PicName);
	        tst = (-1);
           goto ret;
	  };/* switch(tipobj) */

     setwritemode(XOR_PUT);
     WaitVert();
//     outpic_obj_num(obj,numobj,num);

	  for(;(((unsigned short int)*((unsigned long*)ptr_Logic+i1) == Search1)&&
           ((unsigned short int)*((unsigned long*)ptr_Logic+i2) == Search2));i1++,i2++)
        {

	  if(
        (i1 > ((struct HTBG*)*(obj))->Num_LE)||
        (i2 > ((struct HTBG*)*(obj))->Num_LE)
       )
	     break;

		  Ind1=ptr_Logic[i1].Index_ei;
		  Ind2=ptr_Logic[i2].Index_ei;

		  switch(*((char*)*(obj+Ind1))){
			  case OBJ_line:
              // получить координаты линии,исходные
              ptr_ln=((struct ln*)*(obj+Ind1));
		        xmin= DMX((long)ptr_ln->lnx);
		        ymin= DMY((long)ptr_ln->lny);
		        xmax= DMX((long)ptr_ln->lnx+
                        (long)ptr_ln->lndx);
		        ymax= DMY((long)ptr_ln->lny+
                        (long)ptr_ln->lndy);
			     break;
           default:
	           printf("<%s>'rotate_obj_num+1' исходные данные для поворота: не линия\n\007",hTbg(obj)->PicName);
	           tst = (-1);
              goto ret;
		  };/* switch(tipobj) */
        /*
        rp(xmin,ymin,a,xc,yc);
        rp(xmax,ymax,a,xc,yc);
        */
		  switch(*((char*)*(obj+Ind2))){
			  case OBJ_line:
              // изменить координаты в num+2
              //
              ptr_ln=((struct ln*)*(obj+Ind2));
              setcolor(ptr_ln->lncol);
              setlinestyle(  ptr_ln->lnstl,0,
                             ptr_ln->lnthi);
              line(DMX((long)ptr_ln->lnx),
                   DMY((long)ptr_ln->lny),
                   DMX((long)ptr_ln->lnx+
                       (long)ptr_ln->lndx),
                   DMY((long)ptr_ln->lny+
                       (long)ptr_ln->lndy));
              rp(xmin,ymin,a,xc,yc);
              rp(xmax,ymax,a,xc,yc);
              line(xmin,ymin,xmax,ymax);
              //
              xmax-=xmin;
              ymax-=ymin;
		        ptr_ln->lnx =(short int)(0.5+(xmin/maxx)*SIZE_XY);
		        ptr_ln->lny =(short int)(0.5+(ymin/maxy)*SIZE_XY);
		        ptr_ln->lndx=(short int)(0.5+(xmax/maxx)*SIZE_XY);
		        ptr_ln->lndy=(short int)(0.5+(ymax/maxy)*SIZE_XY);
			     break;
           default:
	           printf("<%s>'rotate_obj_num+2' целевые данные для поворота: не линия\n\007",hTbg(obj)->PicName);
	           tst = (-1);
              goto ret;
		  };/* switch(tipobj) */

	  }// for

	  tst=0;/* элемент обьекта найден */

//     outpic_obj_num(obj,numobj,num);

     setwritemode(COPY_PUT);
  }//if

 if(tst==-2)
 {/* нет элемента объекта */
   tst = error_tbg = ER_NOTELEMOBJ;

 }/* if */

ret:
 setstateTBG(obj, TBG_FREE);
 return tst;

}/* rotate_obj_num */
/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/
/* (C) Чумак А.И. 2000 */
/*--------------------------------------------------------------------------*/
short int outpic_obj_num_with_rotate
  (OBJECT **obj,unsigned char numobj,unsigned char num,double a){
  /*
     a        -> угол поворота от исходного состояния
     obj, num -> центр вращения(нужен эллипс)
     obj, 0   -> GP type:reper, point center rotation
  */

  short int i,i1,tst=-2,
      Ind,
      Ind1;
  unsigned short int Search,
                     Search1;
  short int xmax,ymax,xmin,ymin,xc,yc;
  struct tag_Logic *ptr_Logic=((struct HTBG*)*(obj))->Logic;
  struct ln *ptr_ln;

  int ofsx;
  int ofsy;

#ifdef __WIN32__
 HDC hdc;
#endif

  if(obj == NULL){
     error_tbg = ER_NOTPIC;
     goto ret;
  }
  setstateTBG(obj, TBG_BUSY);

  error_tbg = ER_OK; /* нет ошибок */

  if(((struct HTBG*)*(obj))->ident != 0x5a5a)
  {/* нет картинки */
   tst = error_tbg = ER_NOTPIC;
   goto ret;
  }/* if */

  if(((struct HTBG*)*(obj))->Num_LE==-1)
  {/* нет логических элементов */
   tst = error_tbg = ER_NOTLE;
   goto ret;
  }/* if */

  set_MaxXY(obj);// установка параметров для вывода в окно

 /* Устанавливаем смещение от начала координат */
 ofsx = ((struct HTBG*)*(obj))->ofsx;
 ofsy = ((struct HTBG*)*(obj))->ofsy;

#ifdef __WIN32__
 hdc  = ((struct HTBG*)*(obj))->hdc;
#endif

  Search =(unsigned short int)((numobj << 8)|(0)); // srch center (reper)
  Search1=(unsigned short int)((numobj << 8)|(num)); // srch lines in obj's

  i =BinSearch(Search,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);
  i1=BinSearch(Search1,(unsigned long*)ptr_Logic,
				  ((struct HTBG*)*(obj))->Num_LE,OBJ_NUM);

  if((i != -1)&&(i1 != -1)){

	  Ind=ptr_Logic[i].Index_ei;
	  switch(*((char*)*(obj+Ind))){
           // получить координаты центра,
	     case OBJ_reper:
		     xc= DMX((long)((struct reper*)*(obj+Ind))->x + ofsx);
		     yc= DMY((long)((struct reper*)*(obj+Ind))->y + ofsy);
			  break;
        default:
	        printf("<%s>'rotate_obj_num' GP must be reper to center rotation\n",hTbg(obj)->PicName);
	        tst = (-1);
           goto ret;
	  };/* switch(tipobj) */

	  for(;(((unsigned short int)*((unsigned long*)ptr_Logic+i1) == Search1));i1++)
        {

	  if(
        (i1 > ((struct HTBG*)*(obj))->Num_LE)
       )
	     break;

		  Ind1=ptr_Logic[i1].Index_ei;

		  switch(*((char*)*(obj+Ind1))){
			  case OBJ_line:
              // получить координаты линии,исходные
              ptr_ln=((struct ln*)*(obj+Ind1));
		        xmin= DMX((long)ptr_ln->lnx + ofsx);
		        ymin= DMY((long)ptr_ln->lny + ofsy);
		        xmax= DMX((long)ptr_ln->lnx+
                        (long)ptr_ln->lndx + ofsx);
		        ymax= DMY((long)ptr_ln->lny+
                        (long)ptr_ln->lndy + ofsy);
              rp(xmin,ymin,a,xc,yc);
              rp(xmax,ymax,a,xc,yc);
#ifdef __WIN32__
              SelectObject(hdc,
               cEGA[ptr_ln->lncol].hPen[(ptr_ln->lnthi == THICK_WIDTH) ? USERBIT_LINE : ptr_ln->lnstl]);
              Line(hdc, xmin,ymin,xmax,ymax);
#else  /* DOS16 */
              setcolor(ptr_ln->lncol);
              setlinestyle(  ptr_ln->lnstl,0,
                             ptr_ln->lnthi);
              line(xmin,ymin,xmax,ymax);
#endif /* DOS16 */

              break;

           default:
	           printf("<%s>'outpic_obj_num_with_rotate' supported only line's rotate\n",hTbg(obj)->PicName);
	           tst = (-1);
              goto ret;
		  };/* switch(tipobj) */

	  }// for

	  tst=0;/* элемент обьекта найден */

  }//if

 if(tst==-2)
 {/* нет элемента объекта */
   error_tbg = ER_NOTELEMOBJ;
 }/* if */

ret:
 setstateTBG(obj, TBG_FREE);
 return tst;

}/* outpic_obj_num_with_rotate */
