#include "dibutil.h"
#ifdef __WIN32__
SBMP sbmp;
/* ------------------------------------------------------------------------ */
/*
   Загрузка BMP в память
*/
/* ------------------------------------------------------------------------ */
int LoadBMP(char *pdata){

   LoadDIBSection(0, padata, &sbmp); //
   if(sbmp.hBitmap == NULL)
      return 1;

   return 0;
}/* LoadBMP */
/* ------------------------------------------------------------------------ */
/*
   Вывод в HDC с координатами
*/
/* ------------------------------------------------------------------------ */
int OutBMP(char *pdata){

   RECT dstrc, srcrc;

   dstrc.top    = 0;
   dstrc.left   = 0;
   dstrc.bottom = 300;
   dstrc.right  = 300;

   srcrc.top = 0;
   srcrc.left = 0;
   srcrc.bottom = 480;
   srcrc.right = 640;

   PaintBitmap(hdc, &dstrc, sbmp.hBitmap, &srcrc, sbmp.hPalette);

   return 0;
}/* OutBMP */
/* ------------------------------------------------------------------------ */
/*
   Удаление из памяти
*/
/* ------------------------------------------------------------------------ */
int DelBMP(char *pdata){
   RemoveDIBSection(&sbmp);
   return 0;
}/* DelBMP */
/* ------------------------------------------------------------------------ */
#endif /* __WIN32__ */