// tbx_004.c ==========================================
//
// Преобразователь картинок TBG.
//
//---------------------------------------------------------------------------
#include "tbx.h"

char *PltLex[]={
   "IN",// 0
   "SC",// 1
   "SI",// SR ??
   "DT",// 3
   "PU",// 4
   "PD",// 5
   "PA",// 6
   "LB",// 7
   "VS",// 8
   "SP",// 9

   "AA",// 10  Дуга  x,y,угол дуги[,угол хоpды]
   "CI",// 11  Окpужность pадиус[,угол хоpды]
   "EA" // 12  Пpямоугольник x,y конца
   };


char *GraphElements[]={
   "NULL",
   "Line",
   "Rectangle",
   "Bar",
   "NULL",
   "Ellipse",
   "Text",
   "FloodFill",
   "BackGround",
   "Logic"
   };

char *Colors[]={
   "EGA_BLACK",
   "EGA_BLUE",
   "EGA_GREEN",
   "EGA_CYAN",
   "EGA_RED",
   "EGA_MAGENTA",
   "EGA_BROWN",
   "EGA_LIGHTGRAY",
   "EGA_DARKGRAY",
   "EGA_LIGHTBLUE",
   "EGA_LIGHTGREEN",
   "EGA_LIGHTCYAN",
   "EGA_LIGHTRED",
   "EGA_LIGHTMAGENTA",
   "EGA_YELLOW",
   "EGA_WHITE"};

char *Widths_line[]={
   "NORM_WIDTH",
   "NORM_WIDTH",
   "NORM_WIDTH",
   "THICK_WIDTH"};

char *Styles_line[]={
   "SOLID_LINE",
   "DOTTED_LINE",
   "CENTER_LINE",
   "DASHED_LINE"};

char *Styles_flood[]={
   "EMPTY_FILL",
   "SOLID_FILL",
   "LINE_FILL",
   "LTSLASH_FILL",
   "SLASH_FILL",
   "BKSLASH_FILL",
   "LTBKSLASH_FILL",
   "HATCH_FILL",
   "XHATCH_FILL",
   "INTERLEAVE_FILL",
   "WIDE_DOT_FILL",
   "CLOSE_DOT_FILL"};

char *Justify[]={
   "LEFT_TEXT",
   "CENTER_TEXT",
   "RIGHT_TEXT",
   "BOTTOM_TEXT",
   "TOP_TEXT"};

char *Directions[]={
   "HORIZ_DIR",
   "VERT_DIR"};

char *Fonts[]={
   "DEFAULT_FONT",
   "TRIPLEX_FONT",
   "SMALL_FONT",
   "SANS_SERIF_FONT",
   "GOTHIC_FONT",
   "SCRIPT_FONT",
   "SIMPLEX_FONT",
   "TIMES_S_FONT",
   "LCOM_FONT",
   "EURO_FONT",
   "BOLD_FONT"};



//---------------------------------------------------------------------------
void restore_file(FILE *in)
{
  struct HTBG_WR htb={" TBG 6.00 TBX",{" HTBL ",0L}," NEW ",-1,0};
  int i,j,obj,numobj;
  char tip_file=5,cle=-1;
//  char *BufLogic;
  long length;
  int x,y,Xs,Ys;
  int pen=0;// поднято пеpо
  int CountPlt=-1;
  int Start  =0,
      Current=0,
      Count  =0,
      All    =0,
      End    =OK,
      tmp    =0,
      k      =0;
  float wt=1.0,ht=1.0;// шиpина и высота символов в % plt
  char dt=3;// детеpминатоp по умолчанию
  int wtp,htp,wta,hta;
  int curcol=WHITE;

  int PosPred=-1,Pos;// для индикации


  fseek(in, 0L, SEEK_END);// указатель в конец файла
  length = ftell(in);// pазмеp файла
  rewind(in);// указатель в начало файла


  switch(mode)
  {
   case TBX_TBG:
/*
  while(fgets(text,sizeof(text),in)!=NULL)
  {
//   fprintf(stderr,"\r Tbx->TBG: %3.1d",NumStr++);
   fprintf(stderr,"%3.1d%\b\b\b\b",
          (int)((float)ftell(in)/(float)length*50.));
/*
   fprintf(stderr,"\r TBX->TBG: %3.1d%",
          (int)((float)ftell(in)/(float)length*50.));

*/
   *strrchr(text,'\n')='\0';// удаляем \n
   if((find_string(GraphElements,text,sizeof(GraphElements)))>=0){
//      count_ei++;
      continue;
   }// if найден ГЭ

//   if(((strncmp(text," Logic",strlen(" Logic")))==0)&&((count_ei-1) >= 0))
   if(((strstr(text,"Logic"))!=NULL)&&((count_ei-1) >= 0))
   {
     BufLogic=strtok(text," ");// выделили Logic
     BufLogic=strtok(NULL," ");// числовое значение
     htb.Num_LE+=(atoi(BufLogic))+1;
   }// if найден ЛЭ

  }// while
  // подсчет числа ГЭ и ЛЭ
*/

  fprintf(stderr," TBX->TBG:     ");

  fwrite(&tip_file,1,1,out);
  fwrite(&htb,sizeof(struct HTBG_WR),1,out);

  htb.count_ei=-1;
  htb.Num_LE=-1;

   while(fgets(text,sizeof(text),in)!=NULL)
   {
     *strrchr(text,'\n')='\0';// удаляем \n

      Pos=(int)((float)ftell(in)/(float)length*100.);

      if(Pos != PosPred){
        fprintf(stderr,"\b\b\b\b%3.1d%",Pos);
        PosPred = Pos;
      }// if

/*
     fprintf(stderr,"%3.1d%\b\b\b\b",
             (int)((float)ftell(in)/(float)length*100.));
*/
     htb.count_ei++;

     switch(find_string(GraphElements,text,sizeof(GraphElements)))
     {
        case OBJ_line:

           fscanf(in," Style %s",&text);
           ln.lnstl=find_string(Styles_line,text,sizeof(Styles_line));

           fscanf(in," Widths %s",&text);
           ln.lnthi=find_string(Widths_line,text,sizeof(Widths_line));

           fscanf(in," Color %s",&text);
           ln.lncol=find_string(Colors,text,sizeof(Colors));

           fscanf(in," X  %d \n Y  %d",&ln.lnx,&ln.lny);
           if(fscanf(in," Dx %d \n Dy %d",&ln.lndx,&ln.lndy)!=2){
//             printf("\n ERROR %d\n",NumStr);
//             return;

           }// if

           //printf("\n dx %d\n dy %d",ln.lndx,ln.lndy);

           ln.tipobj=-OBJ_line;
           fwrite(&ln,sizeof(struct ln),1,out);
           break;
        case OBJ_rect:
           //printf("\nrect");
           fscanf(in," Style %s",&text);
           //printf("\n stl %s",text);
           rc.rcstl=find_string(Styles_line,text,sizeof(Styles_line));

           fscanf(in," Widths %s",&text);
           //printf("\n wdt %s",text);
           rc.rcthi=find_string(Widths_line,text,sizeof(Widths_line));

           fscanf(in," Color %s",&text);
           //printf("\n col %s",text);
           rc.rccol=find_string(Colors,text,sizeof(Colors));

           fscanf(in," X  %d \n Y  %d",&rc.rcx,&rc.rcy);
           //printf("\n x  %d\n y  %d",rc.rcx,rc.rcy);
           fscanf(in," Dx %d \n Dy %d",&rc.rcdx,&rc.rcdy);
           //printf("\n dx %d\n dy %d",rc.rcdx,rc.rcdy);

           rc.tipobj=-OBJ_rect;
           fwrite(&rc,sizeof(struct rc),1,out);
           break;
        case OBJ_bar :
           //printf("\nbar");
           fscanf(in," Style %s",&text);
           //printf("\n stl %s",text);
           br.brstl=find_string(Styles_flood,text,sizeof(Styles_flood));

           fscanf(in," Color %s",&text);
           //printf("\n col %s",text);
           br.brcol=find_string(Colors,text,sizeof(Colors));

           fscanf(in," X  %d \n Y  %d",&br.brx,&br.bry);
           //printf("\n x  %d\n y  %d",br.brx,br.bry);
           fscanf(in," Dx %d \n Dy %d",&br.brdx,&br.brdy);
           //printf("\n dx %d\n dy %d",br.brdx,br.brdy);

           br.tipobj=-OBJ_bar;
           fwrite(&br,sizeof(struct br),1,out);
           break;
        case OBJ_elip:
           //printf("\nelip");
           fscanf(in," Style %s",&text);
           //printf("\n stl %s",text);
           el.elstl=find_string(Styles_line,text,sizeof(Styles_line));

           fscanf(in," Widths %s",&text);
           //printf("\n wdt %s",text);
           el.elthi=find_string(Widths_line,text,sizeof(Widths_line));

           fscanf(in," Color %s",&text);
           //printf("\n col %s",text);
           el.elcol=find_string(Colors,text,sizeof(Colors));

           fscanf(in," X  %d \n Y  %d",&el.elx,&el.ely);
           //printf("\n x  %d\n y  %d",el.elx,el.ely);
           fscanf(in," Dx %d \n Dy %d",&el.eldx,&el.eldy);
           //printf("\n dx %d\n dy %d",el.eldx,el.eldy);
           fscanf(in," Start %d \n End %d",&el.elfi1,&el.elfi2);
           //printf("\n dx %d\n dy %d",el.elfi1,el.elfi2);

           el.tipobj=-OBJ_elip;
           fwrite(&el,sizeof(struct el),1,out);
           break;
        case OBJ_txt :
           //printf("\ntxt");
           fscanf(in," Font %s",&text);
           //printf("\n stl %s",text);
           tx.txfnt=find_string(Fonts,text,sizeof(Fonts));

           fscanf(in," Direct %s",&text);
           //printf("\n wdt %s",text);
           tx.txdir=find_string(Directions,text,sizeof(Directions));

           fscanf(in," Color %s",&text);
           //printf("\n col %s",text);
           tx.txcol=find_string(Colors,text,sizeof(Colors));

           fscanf(in," Size %s",&text);
           tx.txsiz=(unsigned char)atoi(text);

           fscanf(in," X  %d \n Y  %d",&tx.txx,&tx.txy);
           //printf("\n x  %d\n y  %d",tx.txx,tx.txy);

           fscanf(in," Just_H %s ",&text);
           tx.txhor=find_string(Justify,text,sizeof(Justify));

           fscanf(in," Just_V %s ",&text);
           tx.txver=find_string(Justify,text,sizeof(Justify));

           fscanf(in," Sda %d",&tx.txsda);

           fscanf(in," Mult_X %d \n Mult_Y %d \n Text:",&tx.mltx,&tx.mlty);

           tx.txmx=strlen(fgets(text,sizeof(text),in))-1;

           //printf("\n%s",text);

           tx.tipobj=-OBJ_txt;
           fwrite(&tx,sizeof(struct tx)-sizeof(char *),1,out);
           fwrite(&text,tx.txmx,1,out);
           break;
        case OBJ_fld :
           //printf("\nfld");
           fscanf(in," Style %s",&text);
           //printf("\n stl %s",text);
           fl.flstl=find_string(Styles_flood,text,sizeof(Styles_flood));

           fscanf(in," Color %s",&text);
           //printf("\n col %s",text);
           fl.flcol=find_string(Colors,text,sizeof(Colors));

           fscanf(in," Border %s",&text);
           //printf("\n col %s",text);
           fl.flbrd=find_string(Colors,text,sizeof(Colors));

           fscanf(in," X  %d \n Y  %d",&fl.flx,&fl.fly);
           //printf("\n x  %d\n y  %d",fl.flx,fl.fly);

           fl.tipobj=-OBJ_fld;
           fwrite(&fl,sizeof(struct fl),1,out);
           break;
        case OBJ_bkgr:
           //printf("\nbkgr");

           fscanf(in," Color %s",&text);
           //printf("\n col %s",text);
           bk.bkcol=find_string(Colors,text,sizeof(Colors));

           bk.tipobj=-OBJ_bkgr;
           fwrite(&bk,sizeof(struct bk),1,out);
           break;
        case OBJ_Logic:
           htb.count_ei--;

           if(sscanf(text," Logic %d ",(int*)&cle)==1)
           { // есть Logic

           htb.Num_LE+=(cle+1);

           fwrite(&cle,1,1,out);
           obj=0,numobj=0;
           for(j=0;j <= cle;j++){
//              printf("\n#%d#",j);
//              if(kbhit()!=0) {getch();break;}

              fgets(text,sizeof(text),in);// стpоки после Logic

              if(sscanf(text," %d,%d ",&obj,&numobj)!=2)
              {// сттpока не в фоpмате 'obj,num'
                 j--;
                 continue;
              }// if
              fwrite((char*)&obj   ,1,1,out);
              fwrite((char*)&numobj,1,1,out);
//            printf("\n %u %u   %d",obj,numobj,j);
           }//for
           }// if

           break;
        default:
//        Test=0;// нет ГЭ
                htb.count_ei--;
        break;
     }//switch

    }// while
   fseek(out,1,SEEK_SET);// пpопускаем тип файла
   fwrite(&htb,sizeof(struct HTBG_WR),1,out);// заголовок

//   getch();
   break;// TBX_TBG

   case PLT_TBG:

  fwrite(&tip_file,1,1,out);// тип файла
  fwrite(&htb,sizeof(struct HTBG_WR),1,out);// заголовок


//  initgraph(&GD,&GM,"");

   fprintf(stderr," PLT->TBG:     ");

   for(;!feof(in);){


//      if(kbhit()!=0) {getch();break;}

      All=fread(Cache,1,sizeof(Cache),in);
      Current=0;
      for(;Current < All;){
         Count=0;
         Start=Current;
         for(k=Start;Cache[k] != ';';k++,Count++);
         Current=k+1;
         if(End==OK){
            strncpy(String,&Cache[Start],Count+1);
            String[Count+1]=0;
         }//if
         else{
            strncat(String,&Cache[Start],Count+1);
            String[Count+1+tmp]=0;
         }//elif

         if(strchr(String,';')==NULL){
            End=NO;
            tmp=Count+1;
         }//if
         else{
            End=OK;
            // сюда преобразователь команд!
            //
//            printf("\n%s",String);//test

     i=FindLex(String,PltLex,sizeof(PltLex));

     switch(i)
     {
      case 0:// IN
      break;
      case 1:// SC
           if(sscanf(String," \n SC%d,%d,%d,%d;",
                 &XminPlt,&XmaxPlt,&YminPlt,&YmaxPlt)!=4)
           {// нет значений
           XmaxPlt=YmaxPlt=32760;
           XminPlt=YminPlt=0;
//           printf("\n!%d %d!",x,y);
           }// if

//      printf("\nSC *%d,%d,%d,%d*",XminPlt,XmaxPlt,YminPlt,YmaxPlt);

      break;
      case 2:// SR?? SI
           if(sscanf(String," \n SI%f,%f;",&wt,&ht)!=2)
           {// нет значений
            wt=1.0;ht=1.0;
           }// if

           wt=1.0;ht=1.0;
//      printf("\nSI *%f,%f*",wt,ht);

      break;
      case 3:// DT
           if(sscanf(String," \n DT%c;",&dt)!=1)
           {// нет значений
            dt=3;// детеpминатоp по умолчанию
           }// if
//      printf("\nDT *%c*",dt);
      break;
      case 4:// PU
           if(sscanf(String," \n PU%d,%d;",&x,&y)==2){// есть кооpдинаты
             Xs=x;Ys=y;
           }// есть кооpдинаты
           pen=0;// поднято
      break;
      case 5:// PD
           if(sscanf(String," \n PD%d,%d;",&x,&y)==2){// есть кооpдинаты
             CountPlt++;
//             printf("\r%d  ",CountPlt);
             ln.lncol=curcol;
             ln.lnstl=SOLID_LINE;ln.lnthi=NORM_WIDTH;
             ln.lnx=mmx(Xs);
             ln.lndx=mmx(x)-mmx(Xs);
             ln.lny=mmy(Ys);
             ln.lndy=mmy(y)-mmy(Ys);
             ln.tipobj=-OBJ_line;
             fwrite(&ln,sizeof(struct ln),1,out);
/*
             line(DMX((long)ln.lnx),
                  DMY((long)ln.lny),
                  DMX((long)ln.lnx+(long)ln.lndx),
                  DMY((long)ln.lny+(long)ln.lndy)
                 );
*/
             Xs=x;Ys=y;
           }// есть кооpдинаты
           pen=1;// опущено
      break;
      case 6:// PA
//           printf("\n*%s*",String);
           if(sscanf(String," \n PA%d,%d;",&x,&y)==2)
           {// есть кооpдинаты
//           printf("\n!%d %d!",x,y);
           if(pen==0){
             Xs=x;Ys=y;
           }// if

           if(pen==1){
           CountPlt++;
//           printf("\r%d  ",CountPlt);
             ln.lncol=curcol;
             ln.lnstl=SOLID_LINE;ln.lnthi=NORM_WIDTH;
             ln.lnx=mmx(Xs);
             ln.lndx=mmx(x)-mmx(Xs);
             ln.lny=mmy(Ys);
             ln.lndy=mmy(y)-mmy(Ys);
             ln.tipobj=-OBJ_line;
             fwrite(&ln,sizeof(struct ln),1,out);
/*
             line(DMX((long)ln.lnx),
                  DMY((long)ln.lny),
                  DMX((long)ln.lnx+(long)ln.lndx),
                  DMY((long)ln.lny+(long)ln.lndy)
                 );
*/
             }// if
             Xs=x;Ys=y;
           }// if есть кооpдинаты
      break;
      case 7:// LB
           if(sscanf(String," \n LB%s;",text)==1)
           {// OK !
            CountPlt++;
            *strrchr(text,dt)='\0';// вместо dt ноль-конец стpоки

            tx.txfnt=TRIPLEX_FONT;
            tx.txdir=HORIZ_DIR;
            tx.txcol=curcol;
            tx.txsiz=2;
            tx.txx=mmx(Xs);
            tx.txy=mmy(Ys);
            tx.txhor=LEFT_TEXT;
            tx.txver=BOTTOM_TEXT;
            tx.txsda=32760;

            wtp = (int)(wt/100.*(float)(XmaxPlt-XminPlt));
            htp = (int)(ht/100.*(float)(YmaxPlt-YminPlt));
//            printf("\nLB *mx=%d my=%d*",wtp,htp);

            wta=(int)((float)DMX(mmx(wtp))*1.0);
            hta=(int)((float)DMY(mmy(htp))*1.0);

            tx.mltx=wta;
            tx.mlty=hta;

            tx.mltx=1;// ????
            tx.mlty=1;// ????

            tx.txmx=strlen(text);

            tx.tipobj=-OBJ_txt;
            fwrite(&tx,sizeof(struct tx)-sizeof(char *),1,out);
            fwrite(&text,tx.txmx,1,out);
//            printf("\nLB *mx=%d my=%d %s*",tx.mltx,tx.mlty,text);

           }// if

      break;
      case 8:// VS скоpость пеpемещения пеpа
      break;
      case 9:// SP номеp пеpа
           if(sscanf(String," \n SP%d;",&curcol)!=1)
           {// есть кооpдинаты
            curcol=WHITE;
//           printf("\n!%d %d!",x,y);
           }// if
           else{
             switch(curcol)
             {
              case 1:curcol=GREEN;
              break;
              case 2:curcol=WHITE;
              break;
              case 3:curcol=RED;
              break;
              case 4:curcol=BLUE;
              break;

             }// switch
           }// elif

//           printf("\nSP*%d*",curcol);

      break;
//      default:;
     }// switch

         }//elif
      }//for

      Pos=(int)((float)ftell(in)/(float)length*100.);

      if(Pos != PosPred){
        fprintf(stderr,"\b\b\b\b%3.1d%",Pos);
        PosPred = Pos;
      }// if

   }//for
   fseek(out,1,SEEK_SET);// пpопускаем тип файла
   htb.count_ei=CountPlt;// число ГЭ
   fwrite(&htb,sizeof(struct HTBG_WR),1,out);// заголовок
//   getch();
//   closegraph();
   break;// PLT_TBG
  }// switch

  fprintf(stderr,"\b\b\b\b100%");

}//restore_file
//---------------------------------------------------------------------------
int find_string(char *strings[],char *search,int Count)
{

   int i;
//   printf("\n");
   for(i=0;i<=Count;i++){
//      printf("\n   %s %s  ",search,strings[i]);
/*
      if(strcmp(strings[i],search)==0)
         return i;
*/
      if(strstr(search,strings[i])!=NULL)
         return i;

   }//for

//   fprintf(stderr,"\nTBX-I-Не существующий цвет '%s'\007",search);

   return -1;

}//find_string
//---------------------------------------------------------------------------
int over_color(int color)
{

   if(color <= 15)
      return color;

   switch(color){
      case EGA_MAGENTA     : return 5;
      case EGA_BROWN       : return 6;
      case EGA_LIGHTGRAY   : return 7;
      case EGA_DARKGRAY    : return 8;
      case EGA_LIGHTBLUE   : return 9;
      case EGA_LIGHTGREEN  : return 10;
      case EGA_LIGHTCYAN   : return 11;
      case EGA_LIGHTRED    : return 12;
      case EGA_LIGHTMAGENTA: return 13;
      case EGA_YELLOW      : return 14;
      case EGA_WHITE       : return 15;
      default              :
//         fprintf(stderr,"\nTBX-I-Прочитанный цвет '%d' не известен.\007",color);
         return 15;
   }//switch

}//over_color
//---------------------------------------------------------------------------
// end tbx_004.c ==========================================