// tbx_001.c =========================================

#include "tbx.h"

/*
BLACK         0
BLUE          1
GREEN         2
CYAN          3
RED           4
MAGENTA       5
BROWN         6
LIGHTGRAY     7

DARKGRAY      8
LIGHTBLUE     9
LIGHTGREEN   10
LIGHTCYAN    11
LIGHTRED     12
LIGHTMAGENTA 13
YELLOW       14
WHITE        15

*/
#define esc 27
#define LF  10
#define CR  13
#define pmax 480

static int i,k;
static int n1,n2;
static int n_md1=24;/* 24 mode=1 */
static int x,y;
static int colpix;
static int lin;
static unsigned char G1[pmax];

static unsigned char T[pmax];

static unsigned char Bit[8]={0200,0100,040,020,010,04,02,01};

static unsigned char Mask[16];


char mode_key='?';

//------------------------------------------------------
static int Ky(int y)
{
 return((int)((double)y * 479. / (double)getmaxy()));
}// Ky
//------------------------------------------------------
void tbxp(void)
{
 FILE *f;
 char s[80],s1[40],s2[40];
 int num_color,mask_color;

 /* Считаем,что файла задания цветов нет:печатаем все цвета кроме BLACK */
 for(k=1;k<sizeof(Mask);k++) Mask[k]=1;

 if((f=fopen("tbx.cfg","rt"))!=NULL)/* файл задания цветов */
 {/* успешно откpыли */
  /* задание цветов из файла */
  for(k=0;k<sizeof(Mask);k++) Mask[k]=0;
  while(fgets(s,sizeof(s),f))
  {
   *strrchr(s,'\n')=0;/* удаляем ВК(\n) из стpоки */
   if(strlen(s) ==  0 ) continue;/* пустую стpоку пpопускаем */
   if(s[0]      == ';') continue;/* cтpоку комментаpия пpопускаем */
   if(strlen(s)+1 == sizeof(s))
     continue;/* слишком большую cтpоку пpопускаем */
   s1[0]=s2[0]=0;
   sscanf(s,"%s %s",s1,s2);
   num_color  = atoi(s1);/* номер цвета */
   mask_color = atoi(s2);/* значение цвета */
   if((num_color >= 0)&&(num_color < sizeof(Mask)))
     Mask[num_color] = mask_color;
  }/* while */
  /* успешно откpыли */
  fclose(f);
 }/* if */
/*
 for(k=0;k<sizeof(Mask);k++) /* контроль заданных цветов */
    printf("\n%2d %d",k,Mask[k]);
*/

 biosprint(1,0,0);
 biosprint(2,0,0);//статус ?

 biosprint(0,esc,0);  /* между линиями n/216 */
 biosprint(0,'3',0);
 biosprint(0,n_md1,0);  /* между линиями n/216 */
/*
 biosprint(0,'=',0);
 biosprint(0,'T',0);
 biosprint(0,'B',0);
 biosprint(0,'X',0);
 biosprint(0,'=',0);
 biosprint(0,' ',0);
 biosprint(0,'1',0);
 biosprint(0,'.',0);
 biosprint(0,'0',0);
 biosprint(0,'1',0);
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
*/

 biosprint(0,0x0a,0);/* LF */
 biosprint(0,0x0a,0);/* LF */
 biosprint(0,0x0a,0);/* LF */


lin=0;
for(i=0;i<pmax;i++) {G1[i]=T[i]=0;};

for(x=getmaxx();x>=0;x--)
{// перебор столбцов
 for(y=0;y<=getmaxy();y++)
 {// перебор строк
  if(kbhit()!=0) { if(getch()==esc) goto end; };
  colpix=getpixel(x,y);
  T[Ky(y)]=1;/* в этой точке были */
  if(mode_key=='1')
  {
   if(Mask[colpix]) G1[Ky(y)]|=Bit[lin];
  }/* if */
  // перебор строк
 }// for

 if(lin == 7)
 {
  for(i=1;i<=478;i++)
  { /* пропуск строки устраняем */
   if(T[i]==0)
   {/* строка пропущена */
    for(k=0;k<=7;k++)
    {/* биты в байте */
     if(((G1[i-1]&Bit[k]  ) != 0)&&((G1[i+1]&Bit[k]  ) != 0))
     {/* между видными точками на VGA пусто */
       G1[i]|=Bit[k];/* заполняем */
     }/* if */
    }/* for */
    /* строка пропущена */
   }/* if */
   /* пропуск строки устраняем */
  }/* for */

  if(mode_key=='1')
  {
   n2=pmax/256; n1=pmax-n2*256;
   biosprint(0,esc,0);
   biosprint(0,'*',0);
   biosprint(0,  5,0);/* 72 т/дюйм */
   biosprint(0, n1,0);
   biosprint(0, n2,0);

   for(i=0;i<pmax;i++) biosprint(0,G1[i],0);

   biosprint(0, LF,0);

   for(i=0;i<pmax;i++) {G1[i]=T[i]=0;};
  }/* if mode_key=='1' */

 }/* if lin==7 */
 lin+=1;if(lin > 7) lin=0;
  // пeребор столбцов
 }// for
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
// end tbx_001.c ==========================================





