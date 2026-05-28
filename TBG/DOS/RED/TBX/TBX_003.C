// tbx_003.c ==========================================
//
// Преобразователь картинок TBG.
//
//---------------------------------------------------------------------------
#include "tbx.h"

//---------------------------------------------------------------------------
int Read_GE(char tip,FILE *fd)
{
  double mw=1.0;
  int mxn,myn;

//  int point;

  switch(tip)// тип ГЭ
  {
   case OBJ_line:// линия

      if(fread(&ln.lnstl,sizeof(struct ln)-sizeof(char),1,fd) != 1) return -1;

      ln.lncol=over_color(ln.lncol);
        switch(mode)
        {
         case TBG_IVX:
 fprintf(out,
 "\n\n_Line\n LT    %2.1d\n LU     0\n LW    %2.1d\n LC    %2.1d\n xB %5.1d\n yB %5.1d\n xE %5.1d\n yE %5.1d",
            ln.lnstl,
            ln.lnthi,
            ln.lncol,
				ln.lnx,
				ln.lny,
				ln.lnx+ln.lndx,
				ln.lny+ln.lndy);
         break;
         case TBG_TBX:
            fprintf(out,"\n\nLine\n Style  %s\n Widths %s\n Color  %s\n X      %5.1d\n Y      %5.1d\n Dx     %5.1d\n Dy     %5.1d",
            Styles_line[ln.lnstl],
            Widths_line[ln.lnthi],
            Colors[ln.lncol],ln.lnx,ln.lny,ln.lndx,ln.lndy);
         break;
         case VIEW:
            setcolor(ln.lncol);
            setlinestyle(ln.lnstl,0,ln.lnthi);
            line(DMX((long)ln.lnx),
                 DMY((long)ln.lny),
                 DMX((long)ln.lnx+(long)ln.lndx),
                 DMY((long)ln.lny+(long)ln.lndy)
                );
         break;
         case C_convert:
         case Pascal_convert:
	if((lstl!=ln.lnstl)||
	   (lthi!=ln.lnthi))
	  {
	   lstl=ln.lnstl;
	   lthi=ln.lnthi;
	   fprintf(out," setlinestyle(%d,0,%d);\n",lstl,lthi);
	  };
	if(col!=ln.lncol)
	  {
	   col=ln.lncol;
	   fprintf(out," setcolor(%d);\n",col);
	  };
	fprintf(out," line(%d,Ky(%d),%d,Ky(%d));",
            DMX((long)ln.lnx),
            DMY((long)ln.lny),
            DMX((long)ln.lnx+(long)ln.lndx),
            DMY((long)ln.lny+(long)ln.lndy)
           );

         break;
        }//switch
   break;

   case OBJ_rect:/* прямоугольник */

      if(fread(&rc.rcstl,sizeof(struct rc)-sizeof(char),1,fd) != 1) return -1;

      rc.rccol=over_color(rc.rccol);
        switch(mode)
        {
         case TBG_IVX:
 fprintf(out,
 "\n\n_Rectangle\n LT    %2.1d\n LU     0\n LW    %2.1d\n LC    %2.1d\n xB %5.1d\n yB %5.1d\n xE %5.1d\n yE %5.1d",
            rc.rcstl,
            rc.rcthi,
            rc.rccol,
				rc.rcx,
				rc.rcy,
				rc.rcx+rc.rcdx,
				rc.rcy+rc.rcdy);
/*
            setcolor(rc.rccol);
            setlinestyle(rc.rcstl,0,rc.rcthi);
            rectangle
                (DMX((long)rc.rcx),
                 DMY((long)rc.rcy),
                 DMX((long)rc.rcx+(long)rc.rcdx),
                 DMY((long)rc.rcy+(long)rc.rcdy)
                );
*/
         break;

         case TBG_TBX:
            fprintf(out,"\n\nRectangle\n Style  %s\n Widths %s\n Color  %s\n X      %5.1d\n Y      %5.1d\n Dx     %5.1d\n Dy     %5.1d",
            Styles_line[rc.rcstl],
            Widths_line[rc.rcthi],
            Colors[rc.rccol],rc.rcx,rc.rcy,rc.rcdx,rc.rcdy);
         break;
         case VIEW:
            setcolor(rc.rccol);
            setlinestyle(rc.rcstl,0,rc.rcthi);
            rectangle
                (DMX((long)rc.rcx),
                 DMY((long)rc.rcy),
                 DMX((long)rc.rcx+(long)rc.rcdx),
                 DMY((long)rc.rcy+(long)rc.rcdy)
                );
         break;
         case C_convert:
         case Pascal_convert:
	if((lstl!=rc.rcstl)||
	   (lthi!=rc.rcthi))
	  {
	   lstl=rc.rcstl;
	   lthi=rc.rcthi;
	   fprintf(out," setlinestyle(%d,0,%d);\n",lstl,lthi);
	  };
	if(col!=rc.rccol)
	  {
	   col=rc.rccol;
	   fprintf(out," setcolor(%d);\n",col);
	  };

	fprintf(out," rectangle(%d,Ky(%d),%d,Ky(%d));",
            DMX((long)rc.rcx),
            DMY((long)rc.rcy),
            DMX((long)rc.rcx+(long)rc.rcdx),
            DMY((long)rc.rcy+(long)rc.rcdy)
           );


         break;
        }//switch
   break;

   case OBJ_bar:/* закрашенный прямоугольник */

      if(fread(&br.brstl,sizeof(struct br)-sizeof(char),1,fd) != 1) return -1;

      br.brcol=over_color(br.brcol);
        switch(mode)
        {
         case TBG_IVX:
 fprintf(out,
 "\n\n_BAR\n FT    %2.1d\n FC    %2.1d\n FB     0\n xB %5.1d\n yB %5.1d\n xE %5.1d\n yE %5.1d",
            br.brstl,
            br.brcol,
				br.brx,
				br.bry,
				br.brx+br.brdx,
				br.bry+br.brdy);
         break;

         case TBG_TBX:
            fprintf(out,"\n\nBar\n Style  %s\n Color  %s\n X      %5.1d\n Y      %5.1d\n Dx     %5.1d\n Dy     %5.1d",
            Styles_flood[br.brstl],
            Colors[br.brcol],br.brx,br.bry,br.brdx,br.brdy);
         break;
         case VIEW:
            setfillstyle(br.brstl,br.brcol);
            bar (DMX((long)br.brx),
                 DMY((long)br.bry),
                 DMX((long)br.brx+(long)br.brdx),
                 DMY((long)br.bry+(long)br.brdy)
                );
         break;
         case C_convert:
         case Pascal_convert:
	if((fstl!=br.brstl)||
	   (fcol!=br.brcol))
	  {
	   fstl=br.brstl;
	   fcol=br.brcol;
	   fprintf(out," setfillstyle(%d,%d);\n",fstl,fcol);
     };
	fprintf(out," bar(%d,Ky(%d),%d,Ky(%d));",
            DMX((long)br.brx),
            DMY((long)br.bry),
            DMX((long)br.brx+(long)br.brdx),
            DMY((long)br.bry+(long)br.brdy)
           );
         break;
        }//switch
   break;

   case OBJ_elip:/* эллипс */

      if(fread(&el.elstl,sizeof(struct el)-sizeof(char),1,fd) != 1) return -1;

      el.elcol=over_color(el.elcol);
        switch(mode)
        {
         case TBG_IVX:
/*
_Ellipse
 LT  1
 LU  1
 LW  1
 LC  1
 xC  100
 yC  100
 xR  20
 yR  20
 sA  36000
 eA  0
 rA  0

*/
 fprintf(out,
 "\n\n_Ellipse\n LT    %2.1d\n LU     0\n LW    %2.1d\n LC    %2.1d\n xC %5.1d\n yC %5.1d\n xR %5.1d\n yR %5.1d\n sA %5.1u\n eA %5.1u\n rA     0",
            el.elstl,
            el.elthi,
            el.elcol,
				el.elx,
				el.ely,
				el.eldx,
				el.eldy,
				el.elfi1*100,
				el.elfi2*100
				);

/*
            fprintf(out,"\n\nEllipse\n Style  %s\n Widths %s\n Color  %s\n X      %5.1d\n Y      %5.1d\n Dx     %5.1d\n Dy     %5.1d\n Start  %5.1d\n End    %5.1d",
            Styles_line[el.elstl],
            Widths_line[el.elthi],
            Colors[el.elcol],el.elx,el.ely,el.eldx,el.eldy,el.elfi1,el.elfi2);
*/
         break;
         case TBG_TBX:
            fprintf(out,"\n\nEllipse\n Style  %s\n Widths %s\n Color  %s\n X      %5.1d\n Y      %5.1d\n Dx     %5.1d\n Dy     %5.1d\n Start  %5.1d\n End    %5.1d",
            Styles_line[el.elstl],
            Widths_line[el.elthi],
            Colors[el.elcol],el.elx,el.ely,el.eldx,el.eldy,el.elfi1,el.elfi2);
         break;
         case VIEW:
            setcolor(el.elcol);
            setlinestyle(el.elstl,0,el.elthi);
            ellipse
                (DMX((long)el.elx),
                 DMY((long)el.ely),
                 el.elfi1,el.elfi2,
                 DMX((long)el.eldx),
                 DMY((long)el.eldy)
                );
         break;
         case C_convert:
//         break;
         case Pascal_convert:
	if((lstl!=el.elstl)||
	   (lthi!=el.elthi))
	  {
	   lstl=el.elstl;
	   lthi=el.elthi;
	   fprintf(out," setlinestyle(%d,0,%d);\n",lstl,lthi);
	  };
	if(col!=el.elcol)
	  {
	   col=el.elcol;
	   fprintf(out," setcolor(%d);\n",col);
	  };
	fprintf(out," ellipse(%d,Ky(%d),%d,%d,%d,Ky(%d));",
            DMX((long)el.elx),
            DMY((long)el.ely),
            el.elfi1,el.elfi2,
            DMX((long)el.eldx),
            DMY((long)el.eldy)
           );

         break;
        }//switch
   break;

   case OBJ_txt:/* текст */

      //читаем нужное для данного подтипа элемента количество данных
      switch(tip_txt){ // тип элемента текст

       case 0:if(fread(&tx.txfnt,9,1,fd) != 1) return -1;
       break;
       case 1:if(fread(&tx.txfnt,15,1,fd) != 1) return -1;
       break;

      }//switch  тип элемента текст

      tx.txcol=over_color(tx.txcol);

      tx.txt=text;

      if(fread(text,1,tx.txmx,fd)
           !=  tx.txmx )
         return -1;

      //завершающий '\0'
      tx.txt[tx.txmx]=0;

      switch(tip_txt){ // тип элемента текст

       case 0:
        switch(mode)
        {
         case TBG_IVX:
/*
          fprintf(out,"\n\nText\n Font   %s\n Direct %s\n Size   %5.1d\n Color  %s\n X      %5.1d\n Y      %5.1d\n Text:%s",
          Fonts[tx.txfnt],
          Directions[tx.txdir],
          tx.txsiz,
          Colors[tx.txcol],
          tx.txx,tx.txy,
          tx.txt);
*/
         break;
         case TBG_TBX:
          fprintf(out,"\n\nText\n Font   %s\n Direct %s\n Size   %5.1d\n Color  %s\n X      %5.1d\n Y      %5.1d\n Text:%s",
          Fonts[tx.txfnt],
          Directions[tx.txdir],
          tx.txsiz,
          Colors[tx.txcol],
          tx.txx,tx.txy,
          tx.txt);
         break;
         case VIEW:
            settextjustify(0,0);
            settextstyle(tx.txfnt,tx.txdir,tx.txsiz);
            setcolor(tx.txcol);
            outtextxy(DMX((long)tx.txx),DMY((long)tx.txy),tx.txt);
         break;
         case C_convert:
         case Pascal_convert:
           thor=0;
	        tver=0;
            fprintf(out," settextjustify(%d,%d);\n",thor,tver);

            if((tfnt !=tx.txfnt)||
               (tdir !=tx.txdir)||
               (tsiz !=tx.txsiz)
              )
            {
             tfnt=tx.txfnt;
             tdir=tx.txdir;
             tsiz=tx.txsiz;
             fprintf(out," settextstyle(%d,%d,%d);\n",tfnt,tdir,tsiz);
            }//if fnt,dir,siz,mltx,mlty

            if(col!=tx.txcol)
            {
             col=tx.txcol;
             fprintf(out," setcolor(%d);\n",col);
            }// if col

         break;
        }//switch тип текста = 0
       break;

       case 1:
        switch(mode)
        {
         case TBG_IVX:
 {/* локальный блок */
  int h=0,w=0,tv=0;
  int he,we;
  int i;
  char m[2]={0,0};

  settextjustify(tx.txhor,tx.txver);
  if(tx.txdir == VERT_DIR) tv=9000;

  if(tx.txfnt == 0) settextstyle(0,tx.txdir,tx.txsiz);
  else
  {
   settextstyle(tx.txfnt,tx.txdir,0);
   mw=(double)tx.txsda / 32760.;
   mxn=(int)((double)(tx.mltx)*mw);
   myn=(int)((double)(tx.mlty)*mw);
   setusercharsize(mxn,10,myn,10);
  }// if

  for(i=0;i<strlen(tx.txt);i++)  // шиpина точная !!!
  {
   m[0]=*(tx.txt+i);
   w+=textwidth(m);
  }/* for */
//  w=textwidth(tx.txt)-textwidth("M");// шиpина не точная

  h=textheight(tx.txt);// высота

//  printf("\nW=%2.0d H=%2.0d\n",w,h);
//  getch();
/*
{
int maxx=639;
int maxy=349;

  if(tx.txdir == HORIZ_DIR) {
     if(h>maxy) h=maxy;
     if(w>maxx) w=maxx;
     he=(int)((double)h*(32760./(double)maxy)+0.5);
     we=(int)((double)w*(32760./(double)maxx)+0.5);
  }else{
     if(h>maxx) h=maxx;
     if(w>maxy) w=maxy;
     he=(int)((double)h*(32760./(double)maxx)+0.5);
     we=(int)((double)w*(32760./(double)maxy)+0.5);
  }// elif
}
*/

  if(tx.txdir == HORIZ_DIR) {
     if(h>getmaxy()) h=getmaxy();
     if(w>getmaxx()) w=getmaxx();
     he=(int)((double)h*(32760./(double)getmaxy())+0.5);
     we=(int)((double)w*(32760./(double)getmaxx())+0.5);
  }else{
     if(h>getmaxx()) h=getmaxx();
     if(w>getmaxy()) w=getmaxy();
     he=(int)((double)h*(32760./(double)getmaxx())+0.5);
     we=(int)((double)w*(32760./(double)getmaxy())+0.5);
  }// elif



  fprintf(out,"\n\n_Text\n TF  %5.1d\n THc %5.1d\n TMc %5.1d\n TMs     0\n TD      0\n TjY %5.1d\n TjX %5.1d\n TH   9000\n TV  %5.1d\n TC  %5.1d\n xT  %5.1d\n yT  %5.1d\nStr:%s\n",
                   tx.txfnt,           he,         we,                           tx.txver,   tx.txhor,                    tv,   tx.txcol,     tx.txx,     tx.txy,     tx.txt);
/*
// показ того что пpеобpазуем (пpи инициализации)
  setcolor(tx.txcol);
  outtextxy(DMX((long)tx.txx),DMY((long)tx.txy),tx.txt);

  setcolor(LIGHTRED);
  if(tx.txdir == HORIZ_DIR){
    rectangle(DMX((long)tx.txx)+w,DMY((long)tx.txy),
              DMX((long)tx.txx),  DMY((long)tx.txy)-h);
  }else{
    rectangle(DMX((long)tx.txx),DMY((long)tx.txy),
              DMX((long)tx.txx)+h,DMY((long)tx.txy)-w);
  }// elif
*/
  }/* локальный блок */
         break;

         case TBG_TBX:
          fprintf(out,"\n\nText\n Font   %s\n Direct %s\n Color  %s\n Size   %5.1d\n X      %5.1d\n Y      %5.1d\n Just_H %s\n Just_V %s\n Sda    %5.1d\n Mult_X %5.1d\n Mult_Y %5.1d\n Text:%s",
          Fonts[tx.txfnt],
          Directions[tx.txdir],
          Colors[tx.txcol],
          tx.txsiz,
          tx.txx,tx.txy,
          Justify[tx.txhor],
          Justify[tx.txver],
          tx.txsda,
          tx.mltx,
          tx.mlty,
          tx.txt);
         break;
         case VIEW:
               settextjustify(tx.txhor,tx.txver);

               if(tx.txfnt == 0)
                  settextstyle(0,tx.txdir,tx.txsiz);
               else{
                  settextstyle(tx.txfnt,tx.txdir,0);
                  mw=(double)tx.txsda / 32760.;
                  mxn=(int)((double)(tx.mltx)*mw);
                  myn=(int)((double)(tx.mlty)*mw);
                  setusercharsize(mxn,10,myn,10);
/*
                  if(tx.txdir == HORIZ_DIR) setusercharsize(mxn,10,myn,10);
                    else                    setusercharsize(myn,10,mxn,10);
*/
               }// if

               setcolor(tx.txcol);
               outtextxy(DMX((long)tx.txx),DMY((long)tx.txy),tx.txt);

/*
{//---------------------------
  int h,w;

  h=textheight(tx.txt);// высота
  w=textwidth (tx.txt) - textwidth ("M");// шиpина

  printf("\nH=%3.1d W=%3.1d",h,w);


  setcolor(tx.txcol);
  outtextxy(DMX((long)tx.txx),DMY((long)tx.txy),tx.txt);

  setcolor(LIGHTRED);
  if(tx.txdir == HORIZ_DIR){
    rectangle(DMX((long)tx.txx)+w,DMY((long)tx.txy),
              DMX((long)tx.txx),  DMY((long)tx.txy)-h);
  }else{
    rectangle(DMX((long)tx.txx),DMY((long)tx.txy),
              DMX((long)tx.txx)+h,DMY((long)tx.txy)-w);
  }// elif
}//---------------------------
*/
         break;
         case C_convert:
         case Pascal_convert:
           if((thor!=tx.txhor)||
              (tver!=tx.txver)
             )
           {
            thor=tx.txhor;
	        tver=tx.txver;
            fprintf(out," settextjustify(%d,%d);\n",thor,tver);
	       }//if hor,ver


	if((tfnt !=tx.txfnt)||
	   (tdir !=tx.txdir)||
	   (tsiz !=tx.txsiz)||
       (tmltx!=tx.mltx )||
	   (tmlty!=tx.mlty )
     )
	  {

	   tfnt=tx.txfnt;
	   tdir=tx.txdir;
	   tsiz=tx.txsiz;
       tmltx=tx.mltx;
	   tmlty=tx.mlty;

      fprintf(out," settextstyle(%d,%d,%d);\n",tfnt,tdir,tsiz);

      if(tfnt > 0){
         mw=(double)(tx.txsda) / 32760.;

         mxn=(int)(((double)(tx.mltx)*mw));
         myn=(int)(((double)(tx.mlty)*mw));
         if(mxn > 639) mxn=639;
         if(myn > 349) myn=349;
         fprintf(out," setusercharsize(%d,10,%d,10);\n",mxn,myn);
      }//if tfnt

	  }//if fnt,dir,siz,mltx,mlty

	if(col!=tx.txcol)
	  {
	   col=tx.txcol;
	   fprintf(out," setcolor(%d);\n",col);
	  }// if col

         break;
        }//switch

       break; // тип текста = 1

      }//switch  тип элемента текст

      switch(mode)
      {
       case C_convert:
          fprintf(out," outtextxy(%d,Ky(%d),%c%s%c);",
                  DMX((long)tx.txx),
                  DMY((long)tx.txy),
                  '"',tx.txt,'"');
       break;
       case Pascal_convert:
          fprintf(out," outtextxy(%d,Ky(%d),'%s');",
                  DMX((long)tx.txx),
                  DMY((long)tx.txy),
                  tx.txt);
       break;
      }//switch

   break;
   case OBJ_fld:/* закраска */

      if(fread(&fl.flstl,sizeof(struct fl)-sizeof(char),1,fd) != 1) return -1;

      fl.flcol=over_color(fl.flcol);
      fl.flbrd=over_color(fl.flbrd);
        switch(mode)
        {
         case TBG_IVX:
 fprintf(out,
 "\n\n_FloodFill\n FT    %2.1d\n FC    %2.1d\n FB     0\n CB    %2.1d\n xB %5.1d\n yB %5.1d\n",
            fl.flstl,
            fl.flcol,
            fl.flbrd,
				fl.flx,
				fl.fly);
         break;
         case TBG_TBX:
            fprintf(out,"\n\nFloodFill\n Style  %s\n Color  %s\n Border %s\n X      %5.1d\n Y      %5.1d",
            Styles_flood[fl.flstl],
            Colors[fl.flcol],
            Colors[fl.flbrd],
            fl.flx,fl.fly);
         break;
         case VIEW:
            setfillstyle(fl.flstl,fl.flcol);
            setcolor(fl.flbrd);
            floodfill(DMX((long)fl.flx),DMY((long)fl.fly),fl.flbrd);
         break;
         case C_convert:
         case Pascal_convert:
            if((fstl!=fl.flstl)||
               (fcol!=fl.flcol))
            {
             fstl=fl.flstl;
             fcol=fl.flcol;
             fprintf(out," setfillstyle(%d,%d);\n",fstl,fcol);
            };
           if(col!=fl.flbrd)
           {
            col=fl.flbrd;
            fprintf(out," setcolor(%d);\n",col);
           };
           fprintf(out," floodfill(%d,Ky(%d),%d);",
                   DMX((long)fl.flx),
                   DMY((long)fl.fly),
                   fl.flbrd);
         break;
        }//switch
   break;
   case OBJ_bkgr:/* фон */

      if(fread(&bk.bkcol,sizeof(struct bk)-sizeof(char),1,fd) != 1) return -1;

      bk.bkcol=over_color(bk.bkcol);
        switch(mode)
        {
         case TBG_IVX:
            fprintf(out,"\n\nBackGround\n Color  %s",Colors[bk.bkcol]);
         break;
         case TBG_TBX:
            fprintf(out,"\n\nBackGround\n Color  %s",Colors[bk.bkcol]);
         break;
         case VIEW:
            setbkcolor(bk.bkcol);
         break;
         case C_convert:
         case Pascal_convert:
            fprintf(out," setbkcolor(%d);",bk.bkcol);
         break;
        }//switch
   break;

   default:  fprintf(stderr,"load_file Неизвестный тип ГЭ\n\007");return -1;
  }// switch тип ГЭ

 return 0;

}//Read_GE
//---------------------------------------------------------------------------

// end tbx_003.c ==========================================