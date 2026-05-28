/* tbg_tbl1.c ---------------------------------------------------------------
                  Функции интерфейса Си с TBG-файлами

                    Графический пакет "ТурБоГрафика"
                      (C) Павельев П.И. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* обьявления функций и данных */

#include <string.h>
#include <stdlib.h>
#include <dir.h>
#include <io.h>

/*--------------------------------------------------------------------------*/
/* (C) Чумак А.И. 1992 */
/*--------------------------------------------------------------------------*/
short int CreateDir(FILE *lf,char *name_tbl) /* Завести каталог библиотеки */
{

FILE *flash;
short int done=0;
fpos_t fp;
struct ffblk ff;
char stmp[13];
long len_lf=filelength(fileno(lf));

fnsplit(name_tbl,NULL,NULL,stmp,NULL);
strcat(stmp,".FLH");

done=findfirst(stmp,&ff,0);

if(done==0){

   if((flash=fopen(stmp,"rb"))==NULL)
      return -1;

   fseek(flash,0L,SEEK_SET);
   fread(&iALLITEM,2,1,flash);

   //выделяем память для массива элементов каталога.
   pitDir=(struct ITEM *)calloc(iALLITEM,sizeof(struct ITEM));

   if(pitDir == NULL)
      return -1;

   iCountITEM=0;

   while(!feof(flash)){

      fread( pitDir[iCountITEM].baFName,13,1,flash);
      fread(&pitDir[iCountITEM].ulOffset,4,1,flash);

      if(iCountITEM == iALLITEM-1)
         break;

      iCountITEM++;

   }//while

   fclose(flash);

}//if
else{

   fseek(lf,0L,SEEK_SET);
   iCountITEM=0;

   while(!feof(lf)){
      fseek(lf,+Offset_to_TBL,SEEK_CUR);
      fread(&hl,sizeof(hl),1,lf);
      fseek(lf,+(long)(hl.len-sizeof(hl)-Offset_to_TBL),SEEK_CUR);
      iCountITEM++;
      fgetpos(lf, &fp);
      if(fp>=len_lf)
         break;
   }//while

   iALLITEM=iCountITEM;

   //выделяем память для массива элементов каталога.
   pitDir=(struct ITEM *)calloc(iALLITEM,sizeof(struct ITEM));

   if(pitDir == NULL)
      return -1;

   iCountITEM=0;

   fseek(lf,0L,SEEK_SET);

   while(!feof(lf)){
      fseek(lf,+Offset_to_TBL,SEEK_CUR);
      fread(&hl,sizeof(hl),1,lf);
      fseek(lf,+(long)(hl.len-sizeof(hl)-Offset_to_TBL),SEEK_CUR);

      strcpy(pitDir[iCountITEM].baFName,hl.name);
      pitDir[iCountITEM].ulOffset=ftell(lf)-hl.len;

      iCountITEM++;

      fgetpos(lf, &fp);
      if(fp>=len_lf)
         break;
   }//while

}//if else

  /*****************************************************************
   iCountITEM=0;
   while(1){
      printf("\n'%13.13s' - %10.10ld",pitDir[iCountITEM].baFName,
                                      pitDir[iCountITEM].ulOffset);
      if(iCountITEM++ == iALLITEM-1)
         break;
   }//while
   printf("\n %d",iALLITEM);
   getch();
  *****************************************************************/

return 0;

}//CreateDir
/*--------------------------------------------------------------------------*/