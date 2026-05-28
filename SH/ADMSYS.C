/* -----------------------------------------------------------------------
*
*  AdmSys.C
*
*
*  ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <conio.h>
#include <string.h>
#include <alloc.h>
#include <process.h>
#include <dos.h>
#include <errno.h>
#include <io.h>
#include <bios.h>
#include <time.h>
#include <graphics.h>
#include <ctype.h>
#include "tbg_tc.h"
#include "dbf.h"
#include "data.h"
#include "editstr.h"
/* ----------------------------------------------------------------------- */
typedef
   enum{
      START,
      WAITSELECT,
      WORKABON,
      WORKPROP,
      COPYTODISK,
      COPYFROMDISK,
      EXITPROGRAM,
      REGOPERATOR,
      REGADMIN,
      REGADMINFR,
      CONFIGPROGRAM,
      WAITERROR,
      WAITERROROUT,
      STOPPROGRAM
   }STEP;

static STEP Step = START;
static char *sFROnOff[] = {
   "8. Включить ФР.",
   "8. Отключить ФР."
};
/* ----------------------------------------------------------------------- */
long CopyFile(char* What, char* To){
   FILE *fs, *fd;
   char* b;
   long i = 0;
   unsigned bs = 512;
   long fsz;
   OBJECT **pic = NULL;
   short int xs, ys, xe, ye;

   b = calloc(1U, (unsigned)bs);
   if(b == NULL){
      return -1;
   }/* if */

   fs = fopen(What, "rb");
   if(fs == NULL){
      free(b);
      return -1;
   }/* if */

   fd = fopen(To, "wb");
   if(fd == NULL){
      free(b);
      fclose(fs);
      return -1;
   }/* if */

   fsz = filelength(fileno(fs));

   load_file(&pic, "tbg\\workp.tbg");
   offset_pic(pic, 0, -2400);
   Store_Image(pic, "tbg\\copymsg1.img");
   outpic_all(pic);
   getsize_obj_num(pic, 1, 1, &xs, &ys, &xe, &ye);

   for(;!feof(fs);){
      bs = fread(b, 1, 512, fs);
      if(ferror(fs)){
         fclose(fs);
         fclose(fd);
         free(b);
         return -1;
      }/* if */
      bs = fwrite(b, 1, bs, fd);
      if(ferror(fd)){
         fclose(fs);
         fclose(fd);
         free(b);
         return -1;
      }/* if */
      i += bs;
      setfillstyle(SOLID_FILL, EGA_LIGHTBLUE);
      bar(xs, ys,
          xs+(int)((double)(xe-xs)*((double)(unsigned)i/(double)fsz)), ye);
   }/* for */

   fclose(fs);
   fclose(fd);
   free(b);
   Restore_Image(pic, "tbg\\copymsg1.img");
   del_pic(&pic);
   return i;
}/* CopyFile */
/* ----------------------------------------------------------------------- */
#pragma argsused
void cb(double pr){
   return ;
}/* CallBack */
/* ----------------------------------------------------------------------- */
int CopyFiles(char *What, char* To){

   char Command[256] = "";
   strcat(Command, "copy /B ");
   strcat(Command, What);
   strcat(Command, To);
   MessageBox("Копирование файлов",
              "команда: %s исполняется\n"
              "Ждите...", Command);
   if(system(Command) != 0)
      return -1;
   MessageBox(NULL, NULL);
   return 1;
/*
   struct ffblk ffblk;
   short int xs, ys, xe, ye;
   int done;
   long afs;
   OBJECT **pic = NULL;
   double p = 0;

   char WhatDrive[MAXDRIVE],
        WhatDir  [MAXDIR],
        WhatFile [_MAX_FNAME],
        WhatExt  [MAXEXT];

   char ToDrive  [MAXDRIVE],
        ToDir    [MAXDIR],
        ToFile   [_MAX_FNAME],
        ToExt    [MAXEXT];

   char Src[MAXDRIVE + MAXDIR + _MAX_FNAME + MAXEXT],
        Dst[MAXDRIVE + MAXDIR + _MAX_FNAME + MAXEXT];

   int bs;

   fnsplit(What, WhatDrive, WhatDir, WhatFile, WhatExt);
   fnsplit(To, ToDrive, ToDir, ToFile, ToExt);

   afs = 0;// All file sizes

   done = findfirst(What,&ffblk,0);
   if(done == -1 || errno == ENOENT || errno == EMFILE)
      return -1;
   while (!done){
      sprintf(Src, "%s%s%s", WhatDrive, WhatDir, ffblk.ff_name);
      afs += ffblk.ff_fsize;
      done = findnext(&ffblk);
      if(done == -1 || errno == ENOENT || errno == EMFILE)
         return -1;
   }/* while */

   load_file(&pic, "tbg\\workp.tbg");
   Store_Image(pic, "tbg\\copymsg2.img");
   outpic_all(pic);
   getsize_obj_num(pic, 1, 1, &xs, &ys, &xe, &ye);

   done = findfirst(What,&ffblk,0);
   if(done == -1 || errno == ENOENT || errno == EMFILE)
      return -1;
   while (!done){
      char* fullfmt = "%s%s%s";
      sprintf(Src, fullfmt, WhatDrive, WhatDir, ffblk.ff_name);
      sprintf(Dst, fullfmt, ToDrive, ToDir, ffblk.ff_name);
      p += (double)CopyFile(Src, Dst);
      setfillstyle(SOLID_FILL, EGA_LIGHTBLUE);
      bar(xs, ys,
          xs+(int)((double)(xe-xs)*(p/(double)afs)), ye);
      done = findnext(&ffblk);
      if(done == -1 || errno == ENOENT || errno == EMFILE)
         return -1;
   }/* while */

   Restore_Image(pic, "tbg\\copymsg2.img");
   del_pic(&pic);

   printf(sBell);

   return 1;
*/
}/* CopyFiles */
/* ----------------------------------------------------------------------- */
int AdmSys(void){

   if(Step == START){
      Step = WAITSELECT;
      load_file(&p, "tbg\\admsys.tbg");
      outtext_obj_num(p, 1, 8, EGA_LIGHTGRAY, sFROnOff[ FROnOff ], NO_SHOW);
      outpic_all(p);
      return 0;
   }/* if */

   if(Step == WAITSELECT){
      OutTime(p);
      if(Key || ExtKey){
         if(Key == '1'){
            Step = WORKABON;
            del_pic(&p);
         }/* if */
         if(Key == '2'){
            Step = WORKPROP;
            del_pic(&p);
         }/* if */
         if(Key == '3'){
            Step = COPYTODISK;
         }/* if */
         if(Key == '4'){
            Step = COPYFROMDISK;
         }/* if */
         if(Key == '5'){
            Step = CONFIGPROGRAM;
            del_pic(&p);
         }/* if */
         if(Key == '6'){
            Step = EXITPROGRAM;
            del_pic(&p);
         }/* if */
         if(Key == '7'){
            Step = STOPPROGRAM;
            del_pic(&p);
         }/* if */
         if(Key == '8'){
            FROnOff = !FROnOff;
            printf(sBell);
            outtext_obj_num(p, 1, 8, EGA_LIGHTGRAY, sFROnOff[ FROnOff ], NO_SHOW);
            outpic_all(p);
         }/* if */
         if(ExtKey == 63){// F5
            Step = REGOPERATOR;
            del_pic(&p);
         }/* if */
         if(ExtKey == 65){// F7
            Step = REGADMIN;
            del_pic(&p);
         }/* if */
         if(ExtKey == 67){// F9
            Step = REGADMINFR;
            del_pic(&p);
         }/* if */

      }/* if */
      return 0;
   }/* if */

   if(Step == WORKPROP){
      switch(NPost){
         case 4:
            switch(Prop4()){
               case 0:// wait
                  break;
               case 1:// return
                  Step = START;
                  break;
               case -1:// error
                  return -1;
               default:
                  break;
            }/* switch */
            break;
         case 1:
         case 2:
         case 3:
            switch(Propusk()){
               case 0:// wait
                  break;
               case 1:// return
                  Step = START;
                  break;
               case -1:// error
                  return -1;
               default:
                  break;
            }/* switch */
            break;
      }/* switch */
      return 0;
   }/* if */

   if(Step == WORKABON){
      switch(NPost){
         case 4:
            switch(Abon4(0)){
               case 0:// wait
                  break;
               case 1:// return
                  Step = START;
                  break;
               case -1:// error
                  return -1;
               default:
                  break;
            }/* switch */
            break;
         case 1:
         case 2:
         case 3:
            switch(Abonement()){
               case 0:// wait
                  break;
               case 1:// return
                  Step = START;
                  break;
               case -1:// error
                  return -1;
               default:
                  break;
            }/* switch */
            break;
      }/* switch */
      return 0;
   }/* if */

   if(Step == CONFIGPROGRAM){
      switch(ConfigProgram()){
         case 0:
            break;
         case 1:
            Step = START;
            break;
         case -1:
            return -1;
         default:
            break;
      }/* switch */
      return 0;
   }/* if */

   if(Step == REGOPERATOR || Step == REGADMIN || Step == REGADMINFR){
      switch(SetOperator(
         Step == REGADMINFR ? 98 :
         Step == REGADMIN   ? 99 : 00)){
         case 0:// wait
            break;
         case 1:// return
            Step = START;
            break;
         case -1:// error
            return -1;
         default:
            break;
      }/* switch */
      return 0;
   }/* if */

   if(Step == COPYTODISK){
      char pd[MAXDRIVE + MAXDIR + _MAX_FNAME + MAXEXT];
      OutTime(p);
      sprintf(pd, "A:\\%u ", NPost);
      mkdir(pd);
      sprintf(pd, "A:\\%u > NUL", NPost);
      if(CopyFiles("DB\\*.dbf ", pd) < 0){
         MessageBox("Ошибка копирования базы данных", "Ошибка копирования на дискету.");
         Step = WAITERROROUT;
         return 0;
      }/* if */

      if(CopyFiles("ShConfig.txt ", pd) < 0){
         MessageBox("Ошибка копирования кофигурационного файла", "Ошибка копирования на дискету.");
         Step = WAITERROROUT;
         return 0;
      }/* if */

      Step = WAITSELECT;
      return 0;
   }/* if */

   if(Step == COPYFROMDISK){
      char pd[MAXDRIVE + MAXDIR + _MAX_FNAME + MAXEXT];
      OutTime(p);
      sprintf(pd, "A:\\%u\\*.dbf ", NPost);
      if(CopyFiles(pd, "DB > NUL") < 0){
         MessageBox("Ошибка копирования базы данных", "Ошибка копирования с дискеты.");
         Step = WAITERROROUT;
         return 0;
      }/* if */
      Step = WAITSELECT;
      return 0;
         }/* if */

   if(Step == STOPPROGRAM){
      del_pic(&p);
      Step = START;
      return 1;
   }/* if */

   if(Step == EXITPROGRAM){
      return 2;
   }/* if */

   if(Step == WAITERROROUT){
      OutTime(p);
      if(Key == ENTER){
         Step = START;
         MessageBox(NULL, NULL);
         del_pic(&p);
         return -1;
      }/* if */
   }/* if */

   return -1;
}/* AdmSys */
/* ----------------------------------------------------------------------- */
