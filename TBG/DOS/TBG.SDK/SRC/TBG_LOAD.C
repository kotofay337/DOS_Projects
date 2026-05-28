/* tbg_load.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

#include <alloc.h>
#include <dir.h>
#include <string.h>
//#include <conio.h>

//#define DEBUG
int lo = 0;
/*-------------------------------------------------------------------------*/
/* (C) Чумак А.И. 1992 */
/*-------------------------------------------------------------------------*/
long ffind(char *name) /* Поиск файла в библиотеке */
{
 unsigned short int j;

   for(j=0;j!=iALLITEM;j++){
      if(strncmp(pitDir[j].baFName, name, 12) == 0){
         return pitDir[j].ulOffset;
      }/*if */
   }/*for */

   return -1L;
}/* ffind */
/*--------------------------------------------------------------------------- */
/* (C) Чумак А.И. 1992 */
/*-------------------------------------------------------------------------*/
static void sort(unsigned long *arr,short int dim){

   struct la{
      unsigned short int dl;
      unsigned short int dh;
   };
   struct la *a;
   register i,j,gap;

   a = calloc(dim, sizeof(struct la));
   if(a){
      for(i = 0;i < dim;i++){
         (a+i)->dl = ((struct la*)arr+i)->dh;
         (a+i)->dh = ((struct la*)arr+i)->dl;
      }/* for */
      for(gap = dim / 2; gap > 0; gap /= 2){
         for(i = gap; i < dim; i++){
            for(j = i - gap;
                (j >= 0) && ((unsigned long)((unsigned long *)a)[j] > (unsigned long)((unsigned long *)a)[j + gap]);
                j -= gap){
               swap(((unsigned long *)a)[j],((unsigned long *)a)[j + gap]);
           }/* for */
         }/* for */
      }/* for */
      for(i = 0;i < dim;i++){
         ((struct la*)arr+i)->dl = (a+i)->dh;
         ((struct la*)arr+i)->dh = (a+i)->dl;
      }/* for */
      free(a);
   }/* if */
}/* sort */
/*--------------------------------------------------------------------------- */
/* (C) Павельев П.И. 1992 */
/*--------------------------------------------------------------------------*/
/* загрузка примитива из файла */
short int crealo(char tip,FILE *fd,OBJECT **obj,short int pn)
{
  switch(tip)/* тип ГЭ */
  {
   case OBJ_line:/* линия */
             /* Память для ГЭ */
         ((struct ln*)*(obj+pn))=(struct ln *)malloc(sizeof(struct ln));

         if(((struct ln*)*(obj+pn))==NULL) return -1;

         if(fread( ((char*)*(obj+pn))+sizeof(char),
              sizeof(struct ln)-sizeof(char)-sizeof(char),1,fd) != 1) return -1;
         /*
         printf("\ncolor=%d,thi=%d,stl=%d x=%d y=%d ",
                      ((struct ln*)*(obj+pn))->lncol,
                      ((struct ln*)*(obj+pn))->lnthi,
                      ((struct ln*)*(obj+pn))->lnstl,
                      ((struct ln*)*(obj+pn))->lnx,
                      ((struct ln*)*(obj+pn))->lny);
         */
   break;

   case OBJ_rect:/* прямоугольник */
             /* Память для ГЭ */

         ((struct rc*)*(obj+pn))=(struct  rc*)malloc(sizeof(struct rc));

         if(((struct rc*)*(obj+pn))==NULL) return -1;

         if(fread( ((char*)*(obj+pn))+sizeof(char),
              sizeof(struct rc)-sizeof(char)-sizeof(char),1,fd) != 1) return -1;

   break;

   case OBJ_bar:/* закрашенный прямоугольник */
             /* Память для ГЭ */
         ((struct br*)*(obj+pn))=(struct  br*)malloc(sizeof(struct br));

         if(((struct br*)*(obj+pn))==NULL) return -1;

         if(fread( ((char*)*(obj+pn))+sizeof(char),
              sizeof(struct br)-sizeof(char)-sizeof(char),1,fd) != 1) return -1;
   break;

   case OBJ_elip:/* эллипс */
             /* Память для ГЭ */
         ((struct el*)*(obj+pn))=(struct  el*)malloc(sizeof(struct el));

         if(((struct el*)*(obj+pn))==NULL) return -1;

         if(fread( ((char*)*(obj+pn))+sizeof(char),
              sizeof(struct el)-sizeof(char)-sizeof(char),1,fd) != 1) return -1;
   break;

   case OBJ_felip:/* эллипс */
             /* Память для ГЭ */
         ((struct fel*)*(obj+pn))=(struct  fel*)malloc(sizeof(struct fel));

         if(((struct fel*)*(obj+pn))==NULL) return -1;

         if(fread( ((char*)*(obj+pn))+sizeof(char),
              sizeof(struct fel)-sizeof(char)-sizeof(char),1,fd) != 1) return -1;
   break;

   case OBJ_txt:/* текст */
         /* Память для ГЭ */
         ((struct tx*)*(obj+pn))=(struct  tx*)malloc(sizeof(struct tx));

         if(((struct tx*)*(obj+pn))==NULL) return -1;

         /*читаем нужное для данного подтипа элемента количество данных */
         switch(((struct HTBG*)*obj)->tip_txt) /* тип элемента текст */
         {
          case 0:if(fread(((char*)*(obj+pn))+sizeof(char),9,1,fd) != 1) return -1;
          break;
          case 1:if(fread(((char*)*(obj+pn))+sizeof(char),15,1,fd) != 1) return -1;
          break;
         }/*switch  тип элемента текст */

         /* С нулем строка хранится и без нуля */
         /* Память для строки */
         ((struct tx*)*(obj+pn))->txt=(char *)malloc((((struct tx*)*(obj+pn))->txmx)+1);
         if((((struct tx*)*(obj+pn))->txt)==NULL) return -1;
         /* строка */

         if(fread(((struct tx*)*(obj+pn))->txt,1,
                  ((struct tx*)*(obj+pn))->txmx,fd)
              !=  ((struct tx*)*(obj+pn))->txmx )
            return -1;

         /*завершающий '\0' */
         ((struct tx*)*(obj+pn))->txt[((struct tx*)*(obj+pn))->txmx]=0;

   break;

   case OBJ_fld:/* закраска */
         /* Память для ГЭ */
         ((struct fl*)*(obj+pn))=(struct  fl*)malloc(sizeof(struct fl));

         if(((struct el*)*(obj+pn))==NULL) return -1;

         if(fread( ((char*)*(obj+pn))+sizeof(char),
              sizeof(struct fl)-sizeof(char)-sizeof(char),1,fd) != 1) return -1;
   break;

   case OBJ_bkgr:/* фон */
         /* Память для ГЭ */
         ((struct bk*)*(obj+pn))=(struct  bk*)malloc(sizeof(struct bk));

         if(((struct bk*)*(obj+pn))==NULL) return -1;

         if(fread( ((char*)*(obj+pn))+sizeof(char),
              sizeof(struct bk)-sizeof(char)-sizeof(char),1,fd) != 1) return -1;
   break;

   case OBJ_esc:
      {/* lockblock */
         unsigned short int ret;
         if(( ((MP*)*(obj+pn)) = (MP*)calloc(1, sizeof(MP)) ) == NULL)
		      return -1;
         ret = fread( ((char*)*(obj+pn)) + sizeof(char),
                  1,
			         sizeof(MP) -
						sizeof(unsigned short int) - /* выведенное состояние */
						sizeof(MPClass*) -     /* указатель на класс */
						sizeof(char*) -        /* указатель на данные пользователя */
						sizeof(char),          /* тип ГП */
						fd);
//         printf("-crealo ret %u", ret);
         if(ret != sizeof(MP) -
						sizeof(unsigned short int) - /* выведенное состояние */
						sizeof(MPClass*) -     /* указатель на класс */
						sizeof(char*) -        /* указатель на данные пользователя */
						sizeof(char)){
		      return -1;
         }/* if */
/*
         printf("\n.crealo mp:index %d \n"
			                  "mcx  %x\n"
			                  "mcy  %x\n"
			                  "mode %x\n"
			                  "cs   %x\n"
                           ,
									((MP*)*(obj+pn))->index,
									((MP*)*(obj+pn))->mcx,
									((MP*)*(obj+pn))->mcy,
									((MP*)*(obj+pn))->mode,
									((MP*)*(obj+pn))->cs	);
*/
         ((MP*)*(obj+pn))->cl = MPCLoad(((MP*)*(obj+pn))->index);
         ((MP*)*(obj+pn))->ud = NULL;
/*
         printf("\n..crealo mp:index %d \n"
			                  "mcx  %x\n"
			                  "mcy  %x\n"
			                  "mode %x\n"
			                  "cs   %x\n"
                           ,
									((MP*)*(obj+pn))->index,
									((MP*)*(obj+pn))->mcx,
									((MP*)*(obj+pn))->mcy,
									((MP*)*(obj+pn))->mode,
									((MP*)*(obj+pn))->cs	);

         printf("pic  %s\n.end crealo\n", ((MP*)*(obj+pn))->cl->fname);
*/
		}/* lockblock */
   break;

   case OBJ_reper:/*  */
         /* Память для ГЭ */
         ((struct reper*)*(obj+pn))=(struct reper*)malloc(sizeof(struct reper));

         if(((struct reper*)*(obj+pn))==NULL) return -1;

         if(fread(((char*)*(obj+pn))+sizeof(char),
			         sizeof(struct reper) - sizeof(char),1,fd) != 1) return -1;
   break;
   case OBJ_pipeline:/* линия */
             /* Память для ГЭ */
         ((struct pln*)*(obj+pn))=(struct pln *)malloc(sizeof(struct pln));

         if(((struct pln*)*(obj+pn))==NULL) return -1;

         if(fread( ((char*)*(obj+pn))+sizeof(char),
              sizeof(struct pln)-sizeof(char)-sizeof(char),1,fd) != 1) return -1;
         ((struct pln*)*(obj+pn))->color = -1;
   break;


   default:
   error_tbg = ER_BAD_TYPE_GP; /* неизвестный тип графического примитива */
   return -1;
  };/* switch тип ГЭ */

  *((char*)*(obj+pn))=tip;/*тип ГЭ */

 return 0;

}/* crealo */
/*-------------------------------------------------------------------------*/
/* Павельев П.И.        */
/* (C) Чумак А.И. 1992  */
/* (C) Павельев П.И. 1995 (20 апреля)  */
/*-------------------------------------------------------------------------*/
short int load_file(OBJECT ***objret,char *fnam)
{
  struct HTBG_WR htb;
  OBJECT **ptr = *objret;
  OBJECT **obj;
  struct tag_Logic *ptr_Logic;

  short int i,j,Itr=0;
  char F_GE=0;
  long Offs=0L;
  unsigned char no,ln;
  char tip;
  char tip_file,nam[MAXFILE],ext[MAXEXT],namef[MAXDRIVE+MAXPATH];
  long fpred; // позиция перед считыванием байта за ГЭ
  long length;// длинна файла
  FILE *sfd = NULL;

  error_tbg = ER_OK; /* нет ошибок */

#ifdef DEBUG
   if(ptr)
      printf("\n  addr: %p load: %s ", ptr, hTbg(ptr)->PicName);
#endif

// printf("\n.in load_file <%s>", fnam);
  if((ptr != NULL) && (((struct HTBG*)*(ptr))->ident == 0x5a5a))
  {/* память занята,удалите картинку */
   error_tbg = ER_MEMNOTFULL;
   return -1;
  }/* if */

  if(Tbl_Present!=Ok){
     if((fd=fopen(fnam,"rb"))==NULL){/* не удалось открыть файл */
      error_tbg = ER_NOTFOPEN;
      return -1;
     }/* if */
  }/* if */

  fnsplit(fnam,NULL,NULL,nam,ext);
  if(Tbl_Present==Ok)
  {
   sprintf(namef,"%s%s",nam,ext);
   if((Offs=ffind(strupr(namef))) < 0L)
  {
   error_tbg = ER_TBLNOTFILE; /* в библиотеке нет файла  */
   return -1;
  }//if
  }//if Present

  if(Tbl_Present!=Ok)
     fseek(fd, 0L, SEEK_END);
  else
     fseek(fd,Offs,SEEK_SET);

  length = ftell(fd);//длина файла

  if(Tbl_Present!=Ok)
     rewind(fd);//файл в начало

#ifdef DEBUG
      printf("\n %d Load   %p name: %s ", lo, ptr, fnam);
#endif

  fread(&tip_file,1,1,fd);
  fread(&htb,sizeof(struct HTBG_WR),1,fd);

  if(Tbl_Present==Ok)
     //обман старой load_file:
     // -указатель после чтения структуры HTBG,значит,
     //  "length" это:(текущая позиция в файле + размер файла -
     //  - байт типа файла) - размер структуры HTBG.
     length=(ftell(fd)+htb.sr1.len-1L)-sizeof(htb);

  //память для указателей
  obj=(OBJECT**)calloc(htb.count_ei+2,sizeof(OBJECT*));

  if(obj==NULL)
  {
    error_tbg = ER_MEM; /* мало памяти (для файла) */
    goto err_end;
  }/* if */

  //место для хранения х-к файла
  //в первом элементе списка
  *obj=(OBJECT *)malloc(sizeof(struct HTBG));

  ((struct HTBG*)*(obj))->Num_LE=-1;
  ((struct HTBG*)*(obj))->Logic=NULL;

  if(htb.Num_LE >= 0)
  {

     ((struct HTBG*)*(obj))->Num_LE=htb.Num_LE;

     ptr_Logic = ((struct HTBG*)*(obj))->Logic =
	(struct tag_Logic*)calloc(((struct HTBG*)*(obj))->Num_LE+1,
				    sizeof(struct tag_Logic));

  if(((struct HTBG*)*(obj))->Logic == NULL)
  {
    error_tbg = ER_MEM; /* мало памяти (для ЛЭ) */
	 goto err_end;
  }/* if */
  }//if есть ЛЭ

  if(obj==NULL)
  {
    error_tbg = ER_MEM; /* мало памяти (для элементов) */
	 goto err_end;
  }/* if */

     switch(tip_file) // тип файла
     {
      case 2:
      case 3:
      case 4:
	    ((struct HTBG*)*(obj))->tip_txt=0;// подтип элемента текст
      break;
      case 5:
	    ((struct HTBG*)*(obj))->tip_txt=1;// подтип элемента текст
      break;
     }//switch тип файла

  ((struct HTBG*)*(obj))->count_ei=htb.count_ei+1;//число элементов

  /* Картинка при загрузке никуда не смещается */
  ((struct HTBG*)*(obj))->ofsx = 0;
  ((struct HTBG*)*(obj))->ofsy = 0;

  sprintf(hTbg(obj)->PicName,"%s",nam);

  Itr=0;

  for(i=1;i<=((struct HTBG*)*(obj))->count_ei;i++)
  {// загрузка всех элементов
	  switch(tip_file) // тип файла
	  {
		case 2://старые типы: принадлежность к одному обьекту
		case 3:

				if((fread(&ln,1,1,fd)  != 1)||
					(fread(&no,1,1,fd)  != 1)||
					(fread(&tip,1,1,fd) != 1))
            {
             error_tbg = ER_WRITE; /* ошибка при чтении */
//             printf("..e2\n");
				 goto err_end;
            }/* if */
        sfd = fd;
		  if(crealo(tip,fd,obj,i) != 0)
			  goto err_end;//создать и загрузить ГЭ
        fd = sfd;
		  if((no==0)&&(ln==0)){
		  }//нет ЛЭ
		  else{
			  Itr++;
			  (ptr_Logic+Itr)->Index_ei=i;
			  (ptr_Logic+Itr)->numobj=no;
			  (ptr_Logic+Itr)->num=ln;
		  };//есть ЛЭ

		break; //старые типы: принадлежность к одному обьекту
		case 4://новые типы: принадлежность к нескольким обьектам
		case 5:
		  if(F_GE==0)
			  if(fread(&tip,1,1,fd) != 1)
           {
             error_tbg = ER_WRITE; /* ошибка при чтении */
//             printf("..e1\n");
				 goto err_end;
           }/* if */

			if(tip<0)
         {  // ГЭ
			  F_GE=0;

           sfd = fd;

			  if(crealo(-tip,fd,obj,i) < 0)
				  goto err_end;//создать и загрузить ГЭ

           fd = sfd;

			  fpred=ftell(fd);//место байта ГЭ

			  if(fpred==length)
				  break;//следующий байт уже конец файла- не надо читать его
						  //это если за последним ГЭ нет ЛЭ

			  if(fread(&tip,1,1,fd) != 1)
           {
             error_tbg = ER_WRITE; /* ошибка при чтении */
//             printf("..e3 %s\n", fnam);
				 goto err_end;
           }/* if */

			  if(tip<0){//тип
				  F_GE=1;
			  }// if tip
			  else{ // ЛЭ
				  F_GE=0;
				  for(j=0;j<=tip;j++,Itr++){
					  if((fread(&no,1,1,fd)  != 1)||(fread(&ln,1,1,fd)  != 1)
                   )
                 {
                  error_tbg = ER_WRITE; /* ошибка при чтении */
//                  printf("..e4\n");
		      	   goto err_end;
                 }/* if */
					  (ptr_Logic+Itr)->Index_ei=i;
					  (ptr_Logic+Itr)->numobj=no;
					  (ptr_Logic+Itr)->num=ln;
				  }//for по ЛЭ

			  }//if есть ЛЭ
		  }// thif ГЭ
		  else
        {
         error_tbg = ER_ALIGNMENT; /* выравнивание при компиляции - до байта */
         goto err_end;
        }
		break; //новые типы: принадлежность к нескольким обьектам
		default:
       error_tbg = ER_BADTYPEFILE; /* неизвестный тип файла */
       goto err_end;
	  }//switch тип файла

	}// for загрузка всех элементов

end:;

	if(Tbl_Present!=Ok) fclose(fd);
/*------------------------- Тестовая печать
   { short int ii;
   printf("\n\nunsorted:");
   for(ii=0;ii < htb.Num_LE+1;ii++)
      printf("\n %lx",*(((long*)ptr_Logic)+ii));
-------------------------*/

	if( htb.Num_LE > 0)
           sort((unsigned long*)ptr_Logic,htb.Num_LE+1);

/*------------------------ Тестовая печать
   printf("\nsorted:");
   for(ii=0;ii < htb.Num_LE+1;ii++)
      printf("\n %lx",*(((long*)ptr_Logic)+ii));

   getch();

   }// lockblock
/* Пеpеполнение стека пpи соpтиpовке 1000 эл-тов                            */
/* qsort((void*)ptr_Logic,htb.Num_LE+1,sizeof(struct tag_Logic),CompareLE); */
------------------------*/

	((struct HTBG*)*(obj))->ident = 0x5a5a;
   ((struct HTBG*)*(obj))->SIZEX = 640;
   ((struct HTBG*)*(obj))->SIZEY = 480;
#ifdef __WIN32__
   ((struct HTBG*)*(obj))->NeedUpdateFlag = TRUE;
#endif
	*objret=obj;

// printf("\n.return load_file %p <%s>", *objret, hTbg(obj)->PicName);
#ifdef DEBUG
printf("OK! to %p", obj);
lo++;
#endif

	return 0;

err_end:

   if(Tbl_Present!=Ok) fclose(fd);
   return -1;

}/* load_file */
/*--------------------------------------------------------------------------*/
