#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include <time.h>
#include <tbg_tc.h>
/* ----------------------------------------------------------------------------------------------- */
LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM);

char szWinName[] = "TEST";
char str[256] = "";

int maxX = 0, maxY = 0;

HDC memDC;
HWND hWnd, hedit;
short int xmin, ymin, xmax, ymax;
OBJECT **pic = NULL;
BOOL StopPipe = FALSE;
/* ----------------------------------------------------------------------------------------------- */
#pragma argsused
void main(int argc, char *argv[]){

   WNDCLASS wcl;
   MSG msg;

   HDC hdc;
   RECT dstrc, srcrc;
   HBITMAP hbmp, hbmp2;
   WIN32_FIND_DATA fd;
   HANDLE hfile;
   BOOL bFlag = FALSE;

   hdc = GetDC(HWND_DESKTOP);

   /* определить класс окна */
   wcl.hInstance = 0;
   wcl.lpszClassName = szWinName;
   wcl.lpfnWndProc = WindowFunc;
   wcl.style = 0;

   wcl.cbClsExtra = 0;
   wcl.cbWndExtra = 0;

   wcl.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
   wcl.lpszMenuName = MAKEINTRESOURCE(1);
   /* зарегистрируем класс окна */
   if( !RegisterClass(&wcl) )
      return;

   CreateEGATools();

   /* регистрация прошла - создадим окно */
   hWnd = CreateWindow(
      szWinName,
      "Show TBG",
      WS_OVERLAPPEDWINDOW,
      0,
      0,
      640,
      480,
      HWND_DESKTOP,
      NULL,
      0,
      NULL);

   hedit = CreateWindow("edit", "edit", WS_CHILD |
   ES_MULTILINE | ES_WANTRETURN
   , 100, 100, 100, 80, hWnd, NULL, NULL, NULL);

   ShowWindow(hWnd, SW_SHOW);
   ShowWindow(hedit, SW_SHOW);
   UpdateWindow(hWnd);
   UpdateWindow(hedit);
   if(argc > 1){
      strcpy(str, argv[1]);
   }/* if */
   /* главный цикл обработки сообщений */

   MPCSetPath("C:\\TBG\\BIN\\");
   load_file(&pic, str);

   while( GetMessage(&msg, NULL, 0, 0) ){
     /* if(msg.hwnd == hedit && msg.message == WM_KEYDOWN && (int)msg.wParam == 13){
         DestroyWindow(hWnd);
      }/* if */*/
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }/* while */

   ReleaseDC(HWND_DESKTOP, hdc);
   DeleteEGATools();
}/* main */
/* -------------------------------------------------------------------------- */
#ifdef __WIN32__
LRESULT CALLBACK WindowFunc(HWND hwnd, UINT mess,
                            WPARAM wParam,
                            LPARAM lParam)
{
   int x1, y1, x2, y2;
   HDC hdc;
   PAINTSTRUCT ps;
   RECT r;
   HRGN hrgn;
   HBITMAP hVScr, oldBMP;
   HPEN oldPen, oldBrush;
   char s[255];
   int i;
   clock_t ct;

   switch(mess){
      case WM_CREATE:
         hdc = GetDC(hwnd);
         // создать контекст в памяти
         memDC = CreateCompatibleDC(hdc);
         maxX = GetDeviceCaps(memDC, HORZRES);
         maxY = GetDeviceCaps(memDC, VERTRES);
         // создать BITMAP
         hVScr = CreateCompatibleBitmap(hdc, maxX, maxY);
         // выбрать BITMAP  в контекст памяти (область рисования)
         oldBMP = SelectObject(memDC, hVScr);

         oldBrush = SelectObject(memDC, cEGA[BLACK].hBrush[SOLID_FILL]);
         oldPen   = SelectObject(memDC, cEGA[WHITE].hPen[SOLID_LINE]);

         PatBlt(memDC, 0, 0, maxX, maxY, PATCOPY);
         ReleaseDC(hwnd, hdc);
         PostMessage(hwnd, WM_KEYDOWN, 0, 0);
         break;
      case WM_KEYDOWN:{
         if(hwnd == hedit){
            Beep(1000, 220);
            Beep(600, 120);
         }/* if */

         if(27 == (int)wParam){
            DestroyWindow(hWnd);
            break;
         }/* if */

         if(32 == (int)wParam){
            StopPipe = ~StopPipe;
            break;
         }/* if */

         SmoothEnable = ~SmoothEnable;
         {
            TBGtoDC(pic, memDC);
            outpic_all(pic);
         }

         // Обновить окно
         InvalidateRect(hwnd, NULL, TRUE);
         }
         break;
      case WM_TIMER:
         if(StopPipe){
            setcolor_obj_num(pic, 10, 0, LIGHTBLUE, SHOW);
            KillTimer(hwnd, 1);
         }/* if */
         else{
            setcolor_obj_num(pic, 10, 0, -2, SHOW);
         }/* elif */
         InvalidateRect(hwnd, NULL, TRUE);
         break;
      case WM_DESTROY:
         del_pic(&pic);
         SelectObject(memDC, oldBMP);
         DeleteObject(hVScr);
         DeleteDC(memDC);
         KillTimer(hwnd, 1);
         PostQuitMessage(0);
         break;

      case WM_PAINT:
         hdc = BeginPaint(hwnd, &ps);
//         printf("\n Start Paint");
         BitBlt(hdc, 0, 0, maxX, maxY, memDC, 0, 0, SRCCOPY);
         EndPaint(hwnd, &ps);
//         printf("\n End Paint");
         break;
      default:
         return DefWindowProc(hwnd, mess, wParam, lParam);

   }/* switch */

   return 0;

}/* WindowFunc */
/* ----------------------------------------------------------------------------------------------- */
#endif

