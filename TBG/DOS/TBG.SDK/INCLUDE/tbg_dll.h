#ifndef  _TBG_DLL_H
#define  _TBG_DLL_H 

#include "tbg_tc.h"

#ifdef __DLL__
#define DLL_EI __declspec(dllexport)
#else
#define DLL_EI __declspec(dllimport)
#endif

/* загрузка графического файла в память */
short int DLL_EI Load_file(OBJECT ***objret,/* начало "картинки" */
                           char *fnam       /* имя загружаемого файла(с расширением) */
                           );/*   0 = при успешном завершении */
                             /* !=0 = при остальных ошибках   */

short int DLL_EI Del_pic(OBJECT ***pp);
short int DLL_EI Outpic_all(OBJECT **p);
HDC TBGToDC(OBJECT **p, HDC hdc);
int DLL_EI dllCreateEGATools(void);
int DLL_EI dllDeleteEGATools(void);


#endif
 