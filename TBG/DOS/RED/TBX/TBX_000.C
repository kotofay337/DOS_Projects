// tbx_000.c ==========================================
//
// Преобразователь картинок TBG.
//
//---------------------------------------------------------------------------
#include "tbx.h"

//---------------------------------------------------------------------------
#define OK  1
#define NO  0

char Cache[1024],String[100];

int XmaxPlt=32760,XminPlt=0,YmaxPlt=32760,YminPlt=0;

struct ln ln;
struct rc rc;
struct br br;
struct el el;
struct tx tx;
struct fl fl;
struct bk bk;

int lstl=0,lthi=0,
    col=1,
    fstl=1,fcol=1,
    tfnt=0,tdir=0,tsiz=1,
    thor=0,tver=0,
    tmltx=10,tmlty=10;
//    bcol=0;

double MaxX=639.,MaxY=349.;// EGAHI

char text[255];

char argv1[3],
     argv2[13];

char ErOpenFile[]={"\nОшибка при открытии или не существует\007"};
char sfmt_01 []={"\n%s:%s"};
char sfmt_02 []={"%s"};
char sfmt_021[]={"%-2.2s"};
char ErKey   []={"\nОшибочный ключ.\n\007"};
char Title   []={
"\n\
TBX 1.01 \"TurBoGraphica\" (C) Павельев П.И.,Чумак А.И. 1992-94 /31 мая/\n"};

char Usage   []={"Преобразователь TBG файлов (принтер:9 игл).\n\n\
Формат вызова: TBX -<Ключ> имя_файла.расширение -[Режим_графики]\n\
<Ключи>  v: просмотр;\n\
	 1: печать /цвета задаются в tbx.cfg текущего каталога/;\n\
	 c: преобразование TBG -> TC;\n\
	 p: преобразование TBG -> TPascal;\n\
	 t: преобразование TBG -> TBX;\n\
	 b: преобразование TBX -> TBG;\n\
	 h: преобразование PLT -> TBG.\n\
[Режим_графики]\n\
	 умолчание:наибольшее pазpешение;\n\
	 c: 320*200-4  /пpоcмотp/;\n\
	 e: 640*350-16 /пpоcмотp,печать/;\n\
	 v: 640*480-16 /пpоcмотp,печать/.\n"};

int tip_txt,
    mode;

FILE *out,*in;
int GD,GM;
//---------------------------------------------------------------------------
void main(int argc,char *argv[]){

   char s[60];
   char outname[13];
   char fun_name[13];
   struct ffblk fb;
   int done;

   fprintf(stderr,sfmt_02,Title);

//-- Разбор командной строки ------------------------------------------------
   if((argc > 4)||(argc < 3)){// не более 3 и не менее 2
	  fprintf(stderr,sfmt_02,Usage);
	  return;
   }//if

   if((argv[1][0]=='-' )||
      (argv[1][0]=='\\')||
      (argv[1][0]=='/')){
	  switch(toupper(argv[1][1])){// Заглавные буквы
	  case 'T':
         mode=TBG_TBX;
         break;
	  case 'B':
         mode=TBX_TBG;
         break;
	  case 'H':
         mode=PLT_TBG;
         break;
	  case 'V':
         mode=VIEW;
         break;
	  case '1':
	 mode=PRINT;
	 if(argv[1][1]=='1') mode_key='1';
	 break;
	  case 'C':
         mode=C_convert;
         break;
	  case 'P':
         mode=Pascal_convert;
         break;

	  default :
              fprintf(stderr,sfmt_02,Usage);
              fprintf(stderr,sfmt_02,ErKey);
              return;
	  }//switch key
   }//if
   else{
	  fprintf(stderr,sfmt_02,Usage);
	  fprintf(stderr,sfmt_02,ErKey);
	  return;
   }//if else
   switch(mode){
      case VIEW:
      case PRINT:
      if((argv[3][0]=='-' )||
         (argv[3][0]=='\\')||
         (argv[3][0]=='/')){

         switch(toupper(argv[3][1])){// Заглавные буквы
	      case 'C':if(mode==VIEW) {GD=CGA;GM=CGAC0;}
         break;
	      case 'E':GD=EGA;GM=EGAHI;
         break;
	      case 'V':
                  detectgraph(&GD,&GM);
                  if(GD == EGA) {GD=EGA;GM=EGAHI;}
                    else        {GD=VGA;GM=VGAHI;}
         break;
         }// switch
      }// if
      break;
   }// switch

//---------------------------------------------------------------------------
   sprintf(argv1,sfmt_021,       argv[1]);
   sprintf(argv2,sfmt_02 ,strupr(argv[2]));
   *strrchr(*argv,'\\')='\0';// удаляем имя из строки пути
   sprintf(s,"%s\\",*argv);/* путь к файлам BGI,CHR */
//---------------------------------------------------------------------------
   fnsplit(argv2,NULL,NULL,outname,NULL);
   sprintf(fun_name,sfmt_02,outname);


   switch(mode){
      case TBG_TBX:
         done = findfirst(argv2,&fb,0);
         while(!done){
            fnsplit(fb.ff_name,NULL,NULL,outname,NULL);
            sprintf(fun_name,sfmt_02,outname);
            if((in=fopen(fb.ff_name,"rb"))==NULL){
               fprintf(stderr,sfmt_01,ErOpenFile,fb.ff_name);
               return;
            }//if
            strcat(outname,".TBX");
            if((out=fopen(outname,"wt"))==NULL){
               fprintf(stderr,sfmt_01,ErOpenFile,outname);
               return;
            }//if
            convert_file(in);
            fclose(in);
            fclose(out);
            fprintf(stderr," : %s\n",fb.ff_name);
            done=findnext(&fb);
         }// while
      break;


      case TBX_TBG:
      case PLT_TBG:
/*
         if((in=fopen(argv2,"rt"))==NULL){
            fprintf(stderr,sfmt_01,ErOpenFile,argv2);
            return;
         }//if
         strcat(outname,".TBG");
         if((out=fopen(outname,"wb"))==NULL){
            fprintf(stderr,sfmt_01,ErOpenFile,outname);
            return;
         }//if
         restore_file(in);
*/
         done = findfirst(argv2,&fb,0);
         while(!done){
            fnsplit(fb.ff_name,NULL,NULL,outname,NULL);
            sprintf(fun_name,sfmt_02,outname);
            if((in=fopen(fb.ff_name,"rt"))==NULL){
               fprintf(stderr,sfmt_01,ErOpenFile,fb.ff_name);
               return;
            }//if
            strcat(outname,".TBG");
            if((out=fopen(outname,"wb"))==NULL){
               fprintf(stderr,sfmt_01,ErOpenFile,outname);
               return;
            }//if
            restore_file(in);
            fclose(in);
            fclose(out);
            fprintf(stderr," : %s\n",fb.ff_name);
            done=findnext(&fb);
         }// while
      break;

      case VIEW:
         initgraph(&GD,&GM,s); /* инициализация графики */
         setpalette_tbg();
         MaxX=(double)getmaxx();
         MaxY=(double)getmaxy();
         if((in=fopen(argv2,"rb"))==NULL){
            fprintf(stderr,sfmt_01,ErOpenFile,argv2);
            return;
         }//if
         convert_file(in);
         getch();
         closegraph();
     break;

      case PRINT:
         initgraph(&GD,&GM,s); /* инициализация графики */
         setpalette_tbg();
         MaxX=(double)getmaxx();
         MaxY=(double)getmaxy();
         if((in=fopen(argv2,"rb"))==NULL){
            fprintf(stderr,sfmt_01,ErOpenFile,argv2);
            return;
         }//if
         mode=VIEW;
         convert_file(in);// вывод на экpан
         tbxp();// печать экpана поточечно
         closegraph();
     break;

      case C_convert:
         if((in=fopen(argv2,"rb"))==NULL){
            fprintf(stderr,sfmt_01,ErOpenFile,argv2);
            return;
         }//if
         strcat(outname,".C");
         if((out=fopen(outname,"wt"))==NULL){
            fprintf(stderr,sfmt_01,ErOpenFile,outname);
            return;
         }//if
//---------
   fprintf(out,"/* %s*/\n\n",Title);

   fprintf(out,"#include <stdio.h>\n");
   fprintf(out,"#include <conio.h>\n");
   fprintf(out,"#include <graphics.h>\n\n");
   fprintf(out,"void tbg_%s(void);\n",fun_name);
   fprintf(out,"int Ky(int y);\n\n");

   fprintf(out,"int Ky(int y)\n");
   fprintf(out,"{\n return((int) ((double)y* (double)getmaxy() /349. ));\n}\n\n");

   fprintf(out,"int GraphDriver=DETECT,GraphMode;\n");

   fprintf(out,"void main(void)\n{\n");
   fprintf(out," initgraph(&GraphDriver, &GraphMode,%c%c);\n",'"','"');
   fprintf(out," /* setpalette_tbg();  установка \"улучшенной\" палитры из tbg_tc.lib */\n");
   fprintf(out," tbg_%s();\n",fun_name);
   fprintf(out," getch();\n");
   fprintf(out," closegraph();\n");
   fprintf(out,"}/* main */\n\n");

   fprintf(out,"void tbg_%s(void) /* исходный файл %s */\n{\n",fun_name,argv2);

   fprintf(out," setlinestyle(%d,0,%d);\n",lstl,lthi);
   fprintf(out," setcolor(%d);\n",col);
   fprintf(out," setbkcolor(0);\n");
   fprintf(out," setfillstyle(%d,%d);\n",fstl,fcol);
   fprintf(out," settextjustify(%d,%d);\n",thor,tver);
   fprintf(out," settextstyle(%d,%d,%d);\n\n",tfnt,tdir,tsiz);
//   fprintf(out,"/*%s",ms1);
//   fprintf(out,"%s*/\n\n",ms2);

         convert_file(in);
//---------
         fprintf(out,"\n}/* tbg_%s */\n",fun_name);
      break;

      case Pascal_convert:
         if((in=fopen(argv2,"rb"))==NULL){
            fprintf(stderr,sfmt_01,ErOpenFile,argv2);
            return;
         }//if
         strcat(outname,".PAS");
         if((out=fopen(outname,"wt"))==NULL){
            fprintf(stderr,sfmt_01,ErOpenFile,outname);
            return;
         }//if
//---------
	fprintf(out,"{ %s}\n\n",Title);
	fprintf(out,"Program tbgdemopas(input,output);\n\n");

	fprintf(out,"uses Graph;\n\n");
	fprintf(out,"VAR	 Driver,Mode:integer;\n\n");

	fprintf(out,"FUNCTION Ky(y:integer):integer;\n");
	fprintf(out,"BEGIN\n Ky:=Round(y/349.*GetMaxY);\nEND;\n\n");


	fprintf(out,"PROCEDURE tbg_%s; { исходный файл %s }\n BEGIN\n\n",fun_name,argv2);

    fprintf(out," setlinestyle(%d,0,%d);\n",lstl,lthi);
    fprintf(out," setcolor(%d);\n",col);
    fprintf(out," setbkcolor(0);\n");
    fprintf(out," setfillstyle(%d,%d);\n",fstl,fcol);
    fprintf(out," settextjustify(%d,%d);\n",thor,tver);
    fprintf(out," settextstyle(%d,%d,%d);\n\n",tfnt,tdir,tsiz);
//    fprintf(out,"{%s",ms1);
//    fprintf(out,"%s}\n\n",ms2);

         convert_file(in);
//---------
	  fprintf(out,"\nEND;{ tbg_%s }\n\n",fun_name);
	  fprintf(out,"BEGIN\n\n  Driver:=Detect;\n");
	  fprintf(out,"  InitGraph(Driver,Mode,'');\n");
	  fprintf(out,"  tbg_%s;\n  readln;\n\n",fun_name);
  	  fprintf(out,"  CloseGraph;\n");
	  fprintf(out,"END.{ tbgdemopas }\n");

      break;

   }//switch

   fclose(out);
//   getch();

}// main
//---------------------------------------------------------------------------
// end tbx_000.c ==========================================