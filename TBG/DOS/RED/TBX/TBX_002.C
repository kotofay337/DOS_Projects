// tbx_002.c ==========================================
//
// Преобразователь картинок TBG.
//
//---------------------------------------------------------------------------
#include "tbx.h"

//---------------------------------------------------------------------------
void convert_file(FILE *tbg)
{

  struct HTBG_WR htb;

  int i,j,Itr=0,count_ei=0;
  char F_GE=0;
  static unsigned size;
  static char *vbuf;//буфер для setvbuf
  unsigned char no,ln,numobj,num;
  char tip;
  char tip_file;
//  ,nam[9],ext[5];
//  ,namef[15];
//  int obj,nm;
//  char ObjYes;
  int PosPred=-1,Pos;// для индикации

  long fpred; // позиция перед считыванием байта за ГЭ
  long length;// длинна файла

   /* ввод в буфер */
   size=16000;
   while(setvbuf(tbg,vbuf,_IOFBF,size)!=0){
      size-=100;
      if(size<=100)
         break;
   };

//  fnsplit(tbg,NULL,NULL,nam,ext);


  fseek(tbg, 0L, SEEK_END);

  length = ftell(tbg);//длинна файла

  fseek(tbg, 0L, SEEK_SET);

  if(fread(&tip_file,1,1,tbg) != 1) goto plx;
  fread(&htb,sizeof(htb),1,tbg);

  if(htb.Num_LE >= 0){

  }//if есть ЛЭ


//  printf("\n'load_file' Num_LE = %d, tip_file = %d",htb.Num_LE,tip_file);

     switch(tip_file) // тип файла
     {
      case 2:
      case 3:
      case 4:
            tip_txt=0;// подтип элемента текст
      break;
      case 5:
            tip_txt=1;// подтип элемента текст
      break;
     }//switch тип файла

  count_ei=htb.count_ei+1;//число элементов

   switch(mode){
      case TBG_TBX:
  fprintf(out,"\"TurBoGraphica\" v6.00 Файл текстового представления рисунка TBG\n");
  fprintf(out,"\nGRAPH          %d\nLOGIC          %d\nTEXT type      %d",count_ei,htb.Num_LE,tip_txt);
      break;
      case TBG_IVX:
  fprintf(out,"\"TurBoGraphica\" v6.00  Текстовое пpедставление IVF файла\n\n");
  fprintf(out,
  "HeadIVX\nFK         0\nKO         0\nXwcMin     0\nYwcMin     0\nXwcMax 32760\nYwcMax 32760\n");
    break;
   }//switch

  Itr=0;

     switch(mode){
      case TBG_IVX:
      fprintf(stderr," TBG->IVX:     ");
      break;
      case TBG_TBX:
      fprintf(stderr," TBG->TBX:     ");
      break;
      case C_convert:
      fprintf(stderr," TBG->TC:     ");
      break;
      case Pascal_convert:
      fprintf(stderr," TBG->TPascal:     ");
      break;
   }//switch

  for(i=1;i<=count_ei;i++)
  {// загрузка всех элементов
     switch(mode){
      case TBG_IVX:
      case TBG_TBX:
      case C_convert:
      case Pascal_convert:
      Pos=(int)((float)i/(float)count_ei*100.);
      if(Pos != PosPred){
        fprintf(stderr,"\b\b\b\b%3.1d%",Pos);
        PosPred = Pos;
//  int PosPred=-1,Pos;// для индикации
      }// if
      break;
    }//switch

     switch(tip_file) // тип файла
     {
      case 2://старые типы: принадлежность к одному обьекту
      case 3:

            if((fread(&ln,1,1,tbg)  != 1)||
               (fread(&no,1,1,tbg)  != 1)||
               (fread(&tip,1,1,tbg) != 1))
            goto plx;
        if(Read_GE(-tip,tbg) < 0)
           goto plx;//создать и загрузить ГЭ

        if((no==0)&&(ln==0)){
        }//нет ЛЭ
        else{
           numobj=no;
           num=ln;
           fprintf(out,"\n Logic  0\n#\n\n %3.1u,%3.1u",numobj,num);
/*           CheckObj[numobj][num]++;*/
        };//есть ЛЭ

      break; //старые типы: принадлежность к одному обьекту
      case 4://новые типы: принадлежность к нескольким обьектам
      case 5:
        if(F_GE==0)
           if(fread(&tip,1,1,tbg) != 1)
              goto plx;

         if(tip<0){  // ГЭ
           F_GE=0;

           if(Read_GE(-tip,tbg) < 0)
              goto plx;//создать и загрузить ГЭ

           fpred=ftell(tbg);//место байта ГЭ

           if(fpred==length)
              break;//следующий байт уже конец файла- не надо читать его
                    //это если за последним ГЭ нет ЛЭ

           if(fread(&tip,1,1,tbg) != 1)
              goto plx;//тип ГЭ
           if(tip<0){//тип
              F_GE=1;
           switch(mode)
           {
            case C_convert:
            case Pascal_convert:
               fprintf(out,"\n\n");
            break;
           }//switch

           }// if tip
           else{ // ЛЭ

           switch(mode)
           {
            case TBG_IVX:
               fprintf(out,"\nLog %5.1d",tip+1);
            break;
            case TBG_TBX:
               fprintf(out,"\n Logic  %d\n#",tip);
            break;
            case C_convert:
               fprintf(out,"/* Logic ");
            break;
            case Pascal_convert:
               fprintf(out,"{ Logic ");
            break;
           }//switch


              F_GE=0;

              for(j=0;j<=tip;j++,Itr++){
                 if((fread(&no,1,1,tbg)  != 1)
                  ||(fread(&ln,1,1,tbg)  != 1))
                    goto plx;

                  numobj=no;
                  num=ln;
                  switch(mode)
                  {
                   case TBG_IVX:
                        fprintf(out,"\n %4.1u%4.1u",numobj,num);
/*                        CheckObj[numobj][num]++;*/
                   break;
                   case TBG_TBX:
                        fprintf(out,"\n %3.1u,%3.1u",numobj,num);
/*                        CheckObj[numobj][num]++;*/
                   break;
                   case C_convert:
                   case Pascal_convert:
                        fprintf(out,"%u,%u ",numobj,num);
                   break;
                  }//switch

              }//for по ЛЭ

           switch(mode)
           {
            case C_convert:
               fprintf(out," */\n\n");
            break;
            case Pascal_convert:
               fprintf(out," }\n\n");
            break;
           }//switch


           }//if есть ЛЭ
        }// ГЭ
        else
        fprintf(stderr,"Options|Compiler|Code generation|Word aligment [ ] !!!\n");
      break; //новые типы: принадлежность к нескольким обьектам
      default:fprintf(stderr,"Неизвестный тип файла=%d\n",tip_file);
     }//switch тип файла

   }// for загрузка всех элементов

end:;

   switch(mode)
   {
    case TBG_IVX:
//    break;
    case TBG_TBX:
    fprintf(out,"\nКонец файла .TBX");
/*
    fprintf(out,"\n\nОб\"ект-элемент-ГЭ:");
    for(obj=0;obj<=255;obj++)
    {
     ObjYes=0;
     for(nm=0;nm<=255;nm++)
     {
      if(CheckObj[obj][nm] > 0){
        fprintf(out,"\n %3.1u,%3.1u #%4.1u",obj,nm,CheckObj[obj][nm]);
//        fprintf(out,"\n %3.1u,%3.1u",obj,nm);
        ObjYes=1;
      }// if
     }//for
     if(ObjYes==1) fprintf(out,"\n");// после обьекта
    }// for
*/
    break;
   }// switch

   fclose(tbg);
   return;

   plx:
      fprintf(stderr,"\nОшибка при чтении\n\007");
      fclose(tbg);
      return;

}//convert_file
//---------------------------------------------------------------------------
// end tbx_002.c ==========================================