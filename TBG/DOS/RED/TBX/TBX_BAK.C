// tbx_bak.c =========================================
/*
#include "tbx.h"

#define esc 27
#define LF  10
#define CR  13
#define pmax 480

static int i;
static int n1,n2;
static int n_md1=24;/* 24 mode=1 */
static int x,y;
static int colpix;
static int lin;
static unsigned char G1[pmax];
static unsigned char G2[pmax];
static unsigned char G3[pmax];

static unsigned char T[pmax];

char mode_key='?';

//------------------------------------------------------
static int Ky(int y)
{
 return((int)((double)y * 479. / (double)getmaxy()));
}// Ky
//------------------------------------------------------
void tbxp(void)
{

 biosprint(1,0,0);
 biosprint(2,0,0);//статус ?

 biosprint(0,esc,0);/* между линиями n/216 */
 biosprint(0,'3',0);
 biosprint(0,n_md1,0);

 biosprint(0,'=',0);
 biosprint(0,'T',0);
 biosprint(0,'B',0);
 biosprint(0,'X',0);
 biosprint(0,'=',0);
 biosprint(0,' ',0);
 biosprint(0,'1',0);
 biosprint(0,'.',0);
 biosprint(0,'0',0);
 biosprint(0,' ',0);
 biosprint(0,'T',0);
 biosprint(0,'u',0);
 biosprint(0,'r',0);
 biosprint(0,'B',0);
 biosprint(0,'o',0);
 biosprint(0,'G',0);
 biosprint(0,'r',0);
 biosprint(0,'a',0);
 biosprint(0,'f',0);
 biosprint(0,'i',0);
 biosprint(0,'c',0);
 biosprint(0,'a',0);

 biosprint(0,0x0a,0);/* LF */
 biosprint(0,0x0a,0);/* LF */
 biosprint(0,0x0a,0);/* LF */


lin=0;
for(i=0;i<pmax;i++) {G1[i]=G2[i]=G3[i]=T[i]=0;};

for(x=getmaxx();x>=0;x--) {// for x
               for(y=0;y<=getmaxy();y++)
               {// for y
			      if(kbhit()!=0) { if(getch()==esc) goto end; };
			      colpix=getpixel(x,y);
			      T[Ky(y)]=1;
			      if(mode_key=='1'){
			      switch(lin)
			      {
			       case 7:
				 if(colpix>0) G1[Ky(y)]|=01;
			       break;/* 7 lin */
			       case 6:
				 if(colpix>0) G1[Ky(y)]|=02;
			       break;/* 6 lin */
			       case 5:
				 if(colpix>0) G1[Ky(y)]|=04;
			       break;/* 5 lin */
			       case 4:
				 if(colpix>0) G1[Ky(y)]|=010;
			       break;/* 4 lin */
			       case 3:
				 if(colpix>0) G1[Ky(y)]|=020;
			       break;/* 3 lin */
			       case 2:
				 if(colpix>0) G1[Ky(y)]|=040;
			       break;/* 2 lin */
			       case 1:
				 if(colpix>0) G1[Ky(y)]|=0100;
			       break;/* 1 lin */
			       case 0:
				 if(colpix>0) G1[Ky(y)]|=0200;
			       break;/* 0 lin */
			      };/* switch(lin) */

			      };/* if mode_key=='1' */

		  if(mode_key=='3'){
			      switch(lin)
			      {
			       case 7:
				switch(colpix)
				{
				 case 1:
				 case 2:
				 case 3:
				 case 4:
				 case 5:
				 case 6:
				 case 12:
				 case 13:
				 case 11:
				       G1[Ky(y)]|=01;
				 break;

				 case 9:
				 case 10:
				       G1[Ky(y)]|=01;
				       G2[Ky(y)]|=01;
				 break;

				 case 15:
				 case 14:
				 case 7:
				       G1[Ky(y)]|=01;
				       G2[Ky(y)]|=01;
				       G3[Ky(y)]|=01;
				 break;

				 case 0:
				 case 8:
				 break;
				};/* switch(colpix) */
			    break;/* 7 lin */
			       case 6:
				switch(colpix)
				{
				 case 1:
				 case 2:
				 case 3:
				 case 4:
				 case 5:
				 case 6:
				 case 12:
				 case 13:
				 case 11:
				       G1[Ky(y)]|=02;
				 break;

				 case 9:
				 case 10:
				       G1[Ky(y)]|=02;
				       G2[Ky(y)]|=02;
				 break;

				 case 15:
				 case 14:
				 case 7:
				       G1[Ky(y)]|=02;
				       G2[Ky(y)]|=02;
				       G3[Ky(y)]|=02;
				 break;

				 case 0:
				 case 8:
				 break;
				};/* switch(colpix) */
			       break;/* 6 lin */
			       case 5:
				switch(colpix)
				{
				 case 1:
				 case 2:
				 case 3:
				 case 4:
				 case 5:
				 case 6:
				 case 12:
				 case 13:
				 case 11:
				       G1[Ky(y)]|=04;
				 break;

				 case 9:
				 case 10:
				       G1[Ky(y)]|=04;
				       G2[Ky(y)]|=04;
				 break;

				 case 15:
				 case 14:
				 case 7:
				       G1[Ky(y)]|=04;
				       G2[Ky(y)]|=04;
				       G3[Ky(y)]|=04;
				 break;

				 case 0:
				 case 8:
				 break;
				};/* switch(colpix) */
			       break;/* 5 lin */
			       case 4:
				switch(colpix)
				{
				 case 1:
				 case 2:
				 case 3:
				 case 4:
				 case 5:
				 case 6:
				 case 12:
				 case 13:
				 case 11:
				       G1[Ky(y)]|=010;
				 break;

				 case 9:
				 case 10:
				       G1[Ky(y)]|=010;
				       G2[Ky(y)]|=010;
				 break;

				 case 15:
				 case 14:
				 case 7:
				       G1[Ky(y)]|=010;
				       G2[Ky(y)]|=010;
				       G3[Ky(y)]|=010;
				 break;

				 case 0:
				 case 8:
				 break;
				};/* switch(colpix) */
			       break;/* 4 lin */
			       case 3:
				switch(colpix)
				{
				 case 1:
				 case 2:
				 case 3:
				 case 4:
				 case 5:
				 case 6:
				 case 12:
				 case 13:
				 case 11:
				       G1[Ky(y)]|=020;
				 break;

				 case 9:
				 case 10:
				       G1[Ky(y)]|=020;
				       G2[Ky(y)]|=020;
				 break;

				 case 15:
				 case 14:
				 case 7:
				       G1[Ky(y)]|=020;
				       G2[Ky(y)]|=020;
				       G3[Ky(y)]|=020;
				 break;

				 case 0:
				 case 8:
				 break;
				};/* switch(colpix) */
			       break;/* 3 lin */

			       case 2:
				switch(colpix)
				{
				 case 1:
				 case 2:
				 case 3:
				 case 4:
				 case 5:
				 case 6:
				 case 12:
				 case 13:
				 case 11:
				       G1[Ky(y)]|=040;
				 break;

				 case 9:
				 case 10:
				       G1[Ky(y)]|=040;
				       G2[Ky(y)]|=040;
				 break;

				 case 15:
				 case 14:
				 case 7:
				       G1[Ky(y)]|=040;
				       G2[Ky(y)]|=040;
				       G3[Ky(y)]|=040;
				 break;

				 case 0:
				 case 8:
				 break;
				};/* switch(colpix) */
			       break;/* 2 lin */
			       case 1:
				switch(colpix)
				{
				 case 1:
				 case 2:
				 case 3:
				 case 4:
				 case 5:
				 case 6:
				 case 12:
				 case 13:
				 case 11:
				       G1[Ky(y)]|=0100;
				 break;

				 case 9:
				 case 10:
				       G1[Ky(y)]|=0100;
				       G2[Ky(y)]|=0100;
				 break;

				 case 15:
				 case 14:
				 case 7:
				       G1[Ky(y)]|=0100;
				       G2[Ky(y)]|=0100;
				       G3[Ky(y)]|=0100;
				 break;

				 case 0:
				 case 8:
				 break;
				};/* switch(colpix) */
			       break;/* 1 lin */
			       case 0:
				switch(colpix)
				{
				 case 1:
				 case 2:
				 case 3:
				 case 4:
				 case 5:
				 case 6:
				 case 12:
				 case 13:
				 case 11:
				       G1[Ky(y)]|=0200;
				 break;

				 case 9:
				 case 10:
				       G1[Ky(y)]|=0200;
				       G2[Ky(y)]|=0200;
				 break;

				 case 15:
				 case 14:
				 case 7:
				       G1[Ky(y)]|=0200;
				       G2[Ky(y)]|=0200;
				       G3[Ky(y)]|=0200;
				 break;

				 case 0:
				 case 8:
				 break;
				};/* switch(colpix) */
			       break;/* 0 lin */
			      };/* switch(lin) */
				  };/* if mode_key=='3' */
/*		      printf("x=%d y=%d lin=%d  \r",x,y,lin);*/
			     }// for y

			     if(lin == 7)
			       {

				for(i=1;i<=478;i++){ /* пропуск строки */
				   if(T[i]==0){ /* пропуск строки */
	     if(((G1[i-1]&01  ) != 0)&&((G1[i+1]&01  ) != 0)) G1[i]|=01;
	     if(((G1[i-1]&02  ) != 0)&&((G1[i+1]&02  ) != 0)) G1[i]|=02;
	     if(((G1[i-1]&04  ) != 0)&&((G1[i+1]&04  ) != 0)) G1[i]|=04;
	     if(((G1[i-1]&010 ) != 0)&&((G1[i+1]&010 ) != 0)) G1[i]|=010;
	     if(((G1[i-1]&020 ) != 0)&&((G1[i+1]&020 ) != 0)) G1[i]|=020;
	     if(((G1[i-1]&040 ) != 0)&&((G1[i+1]&040 ) != 0)) G1[i]|=040;
	     if(((G1[i-1]&0100) != 0)&&((G1[i+1]&0100) != 0)) G1[i]|=0100;
	     if(((G1[i-1]&0200) != 0)&&((G1[i+1]&0200) != 0)) G1[i]|=0200;

	     if(((G2[i-1]&01  ) != 0)&&((G2[i+1]&01  ) != 0)) G2[i]|=01;
	     if(((G2[i-1]&02  ) != 0)&&((G2[i+1]&02  ) != 0)) G2[i]|=02;
	     if(((G2[i-1]&04  ) != 0)&&((G2[i+1]&04  ) != 0)) G2[i]|=04;
	     if(((G2[i-1]&010 ) != 0)&&((G2[i+1]&010 ) != 0)) G2[i]|=010;
	     if(((G2[i-1]&020 ) != 0)&&((G2[i+1]&020 ) != 0)) G2[i]|=020;
	     if(((G2[i-1]&040 ) != 0)&&((G2[i+1]&040 ) != 0)) G2[i]|=040;
	     if(((G2[i-1]&0100) != 0)&&((G2[i+1]&0100) != 0)) G2[i]|=0100;
	     if(((G2[i-1]&0200) != 0)&&((G2[i+1]&0200) != 0)) G2[i]|=0200;

	     if(((G3[i-1]&01  ) != 0)&&((G3[i+1]&01  ) != 0)) G3[i]|=01;
	     if(((G3[i-1]&02  ) != 0)&&((G3[i+1]&02  ) != 0)) G3[i]|=02;
	     if(((G3[i-1]&04  ) != 0)&&((G3[i+1]&04  ) != 0)) G3[i]|=04;
	     if(((G3[i-1]&010 ) != 0)&&((G3[i+1]&010 ) != 0)) G3[i]|=010;
	     if(((G3[i-1]&020 ) != 0)&&((G3[i+1]&020 ) != 0)) G3[i]|=020;
	     if(((G3[i-1]&040 ) != 0)&&((G3[i+1]&040 ) != 0)) G3[i]|=040;
	     if(((G3[i-1]&0100) != 0)&&((G3[i+1]&0100) != 0)) G3[i]|=0100;
	     if(((G3[i-1]&0200) != 0)&&((G3[i+1]&0200) != 0)) G3[i]|=0200;
				   };/* if пропуск строки */
				/*   printf("%d %d %d\n",i-1,i,i+1);*/
				};/* for пропуск строки */

			      if(mode_key=='1'){
				n2=pmax/256; n1=pmax-n2*256;
				biosprint(0,esc,0);
				biosprint(0,'*',0);
				biosprint(0,  5,0);/* 72 т/дюйм */
				biosprint(0, n1,0);
				biosprint(0, n2,0);

				for(i=0;i<pmax;i++)biosprint(0,G1[i],0);
				biosprint(0, LF,0);

				for(i=0;i<pmax;i++) {G1[i]=T[i]=0;};
				};/* if mode_key=='1' */

			  if(mode_key=='3'){
				n2=pmax/256; n1=pmax-n2*256;
				biosprint(0,esc,0);
				biosprint(0,'*',0);
				biosprint(0,  5,0);/* 72 т/дюйм */
				biosprint(0, n1,0);
				biosprint(0, n2,0);
				for(i=0;i<pmax;i++)biosprint(0,G1[i],0);
				biosprint(0, CR,0);

				biosprint(0,esc,0);
				biosprint(0,'*',0);
				biosprint(0,  5,0);/* 72 т/дюйм */
				biosprint(0, n1,0);
				biosprint(0, n2,0);
				for(i=0;i<pmax;i++)biosprint(0,G2[i],0);
				biosprint(0, CR,0);

				biosprint(0,esc,0);
				biosprint(0,'*',0);
				biosprint(0,  5,0);/* 72 т/дюйм */
				biosprint(0, n1,0);
				biosprint(0, n2,0);
				for(i=0;i<pmax;i++)biosprint(0,G3[i],0);

				biosprint(0,esc,0);/* между линиями n/216 */
				biosprint(0,'3',0);
				biosprint(0,n_md1,0);
				biosprint(0, LF,0);

				for(i=0;i<pmax
				;i++) {G1[i]=G2[i]=G3[i]=T[i]=0;};
				};/* if mode_key=='3' */


			       };/* if lin==7 */
			     lin+=1;if(lin > 7) lin=0;
			 }// for x
 biosprint(0,0x0a,0);/* LF */

 printf("\007");

end:;

}//  tbxp
//------------------------------------------------------
/***************************************************************************/
int DMX(long x)
{
 long tmp;

 tmp= (0.5 + ((double)labs(x)*(MaxX / 32760.)));
 if(tmp< 0L) return -(int)tmp;
   else      return  (int)tmp;
}//DMx
/***************************************************************************/
int DMY(long y)
{
 long tmp;

 tmp= (0.5 + ((double)labs(y)*(MaxY / 32760.)));
 if(tmp< 0L) return -(int)tmp;
   else      return  (int)tmp;
}//DMy
/***************************************************************************/
int  FindLex(char *search,char *strings[],int Count)
{//                где ?        что ?
   int i;
//   printf("\n");
   for(i=0;i<=Count;i++){
//      printf("\n   %s %s  ",search,strings[i]);
      if(strstr(search,strings[i])!=NULL)
         return i;
   }//for

   return -1;

}//  FindLex
/****************************************************************************/
int mmx(int x)
{
 return (int) ((double)x/((double)(XmaxPlt-XminPlt) / 32760.)+0.5);
}
/****************************************************************************/
int mmy(int y)
{
 return (int) ((double)(YmaxPlt-y)/((double)(YmaxPlt-YminPlt) / 32760.)+0.5);
}// mmy
/***************************************************************************/
void setpalette_tbg(void) // установка "улучшеной" палитры
{
 struct palettetype pal={15,
  {EGA_BLACK,
   EGA_BLUE,
   EGA_GREEN,
   EGA_CYAN,
   EGA_RED,
   38,// EGA_MAGENTA оранжевый GOLD
   14,// EGA_BROWN болото BOLOTO
   EGA_LIGHTGRAY,
   EGA_DARKGRAY,
   25,// EGA_LIGHTBLUE поярче
   EGA_LIGHTGREEN,
   27,// EGA_LIGHTCYAN поярче
   36,// EGA_LIGHTRED  поярче
   39,// EGA_LIGHTMAGENTA пастельный PASTEL
   54,// EGA_YELLOW    поярче
   EGA_WHITE
  }
 };//суперпалитра

  setallpalette(&pal);
}// setpalette_tbg
/***************************************************************************/
*/
// end tbx_bak.c ==========================================





