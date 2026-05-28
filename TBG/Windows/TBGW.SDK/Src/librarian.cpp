#include <vcl.h>
#include <stdio.h>
#include "DList.HPP"
#pragma hdrstop
// ----------------------------------------------------------------------------
struct LibRecord {
   AnsiString Name;
   void *Data;
};
// ----------------------------------------------------------------------------
bool CreateLib(const AnsiString fname){
   if(FileExists(fname)){
      return false;
   }/* if */   
   FILE *fl = NULL;
   fl = fopen(fname.c_str(), "w+b");
   if(fl){
      fprintf(fl, "TBG Library file.\n");
      fprintf(fl, "Blocks: 0\n");
      fclose(fl);
      return true;
   }/* if */
   return false;
}/* CreateLib */
// ----------------------------------------------------------------------------
bool OpenLib(const AnsiString fname){
   FILE *fl = NULL;
   if(!FileExists(fname)){
      return false;
   }/* if */   
   fl = fopen(fname.c_str(), "r+b");
   if(fl){
      char buf[256];
      unsigned long Blocks;
      fscanf(fl, "%s%s%s", buf, buf, buf);
      fscanf(fl, "%s%lU", buf, &Blocks);
      fclose(fl);
      return true;
   }/* if */
   return false;
}/* OpenLib */
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

 