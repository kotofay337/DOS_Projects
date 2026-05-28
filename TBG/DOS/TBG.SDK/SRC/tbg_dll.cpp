//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

#include <windows.h>
#include "tbg_dll.h"
//---------------------------------------------------------------------------
#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------
short int DLL_EI Load_file(OBJECT ***objret,/* начало "картинки" */
                           char *fnam       /* имя загружаемого файла(с расширением) */
                           ){
   int ret;
   ret = load_file(objret, fnam);
   print_error_tbg("Load_file");
   SetPathToBGIFont("C:\\TBG\\BIN\\");
   MPCSetPath("C:\\TBG\\BIN\\");
   return ret;
}/* Load_file */
//---------------------------------------------------------------------------
short int DLL_EI Del_pic(OBJECT ***pp){
   return del_pic(pp);
}/* Del_pic */
//---------------------------------------------------------------------------
short int DLL_EI Outpic_all(OBJECT **p){
   return outpic_all(p);
}/* Outpic_all */
//---------------------------------------------------------------------------
HDC DLL_EI TBGToDC(OBJECT **p, HDC hdc){
   return TBGtoDC(p, hdc);
}/* TBGToDC */
//---------------------------------------------------------------------------
int DLL_EI dllCreateEGATools(void){
return CreateEGATools();
}
//---------------------------------------------------------------------------
int DLL_EI dllDeleteEGATools(void){
return DeleteEGATools();
}
//---------------------------------------------------------------------------
 
