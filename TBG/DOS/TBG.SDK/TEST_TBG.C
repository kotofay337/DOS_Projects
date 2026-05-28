#ifdef __WIN32__
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include "dibutil.h"
#else
#include <graphics.h>
#endif
#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include "tbg_tc.h"
/* ----------------------------------------------------------------------------------------------- */
OBJECT **pic = NULL;
long coremem = 0L;
#ifdef __WIN32__

LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM);

static WORD wDeviceID, res;
static MCI_GENERIC_PARMS MciGenParm;
static MCI_OPEN_PARMS MciOpenParm;
static MCI_PLAY_PARMS MciPlayParm;
static MCI_ANIM_WINDOW_PARMS MciWndParm;
static MCI_ANIM_RECT_PARMS MciRectParm;

char szWinName[] = "TBG_TEST";
char str[256];
int maxX = 0, maxY = 0;

HDC memDC;
HWND hwnd;
short int xmin, ymin, xmax, ymax;

/* ----------------------------------------------------------------------------------------------- */
#else

int huge DetectSVGA256(void){
   return 0x102;
}

#endif

#pragma argsused
void main(int argc, char *argv[]){

#ifdef __WIN32__
   WNDCLASS wcl;
   MSG msg;

   HDC hdc;
   RECT dstrc, srcrc;
   SBMP sbmp;
   HBITMAP hbmp, hbmp2;
   WIN32_FIND_DATA fd;
   HANDLE hfile;
   BOOL bFlag = FALSE;

   hdc = GetDC(HWND_DESKTOP);
/*
   memset(&fd, 0, sizeof(fd));
   hfile = FindFirstFile("*.bmp", &fd);
   if(hfile != INVALID_HANDLE_VALUE)
      do{
         printf("<%s>\n", fd.cFileName);

         LoadDIBSection(0, fd.cFileName, &sbmp); //
         hbmp = sbmp.hBitmap;
         if(hbmp == NULL)
            MessageBox(0, "?", "LoadBitmap", MB_OK);

         dstrc.left = 0;
         dstrc.top = 0;
         dstrc.right  = 1024 / 2;
         dstrc.bottom = 768 / 2;
         FloodFillBMP(hdc, &dstrc, &sbmp);

         RemoveDIBSection(&sbmp);

         getch();

         bFlag = FindNextFile(hfile, &fd);
      }while(bFlag);

   FindClose(hfile);

   return;
*/
   CreateEGATools();

   RegisterBGIFont(SIMPLEX_FONT);
   RegisterBGIFont(TRIPLEX_FONT);
   RegisterBGIFont(SMALL_FONT);
   RegisterBGIFont(SANS_SERIF_FONT);
   RegisterBGIFont(GOTHIC_FONT);
   RegisterBGIFont(SCRIPT_FONT);
   RegisterBGIFont(TRIPLEX_SCR_FONT);
   RegisterBGIFont(COMPLEX_FONT);
   RegisterBGIFont(EUROPEAN_FONT);
   RegisterBGIFont(BOLD_FONT);

#else
   int gd = DETECT, gm;

//   installuserdriver("SVGA256", DetectSVGA256);

   initgraph(&gd, &gm, "c:\\tbg\\bin");
   setpalette_tbg();
   MPCSetPath("Z:\\RED");

   printf("MEM: %ld %ld %ld\n", coremem, coreleft(), coremem - coreleft());
/*
   mouse_ini();
   mouse_on();

   mouse_mode = MOUSE_ON;

   while(1){
      mouse_draw();
      if(kbhit()){
         switch(getch()){
            case '1':// on
               mouse_on();
               break;
            case '2':// off
               mouse_off();
               break;
            case '0':
               goto ext;
               break;
         }/* switch */
      }/* if */
   }/* while */
   ext:;
*/

   load_file(&pic, "testkey.tbg");

   if(pic)
      outpic_all(pic);
   else
      printf("pic == NULL ???\n");

/*
   {
      int a;
      for(a = 0.;a < 360.;a += 360. / 12.){
         getch();
         outpic_obj_num_color(pic, 2, 1, LIGHTGRAY);
         outpic_obj_num_with_rotate(pic, 1, 1, a);
      }/* for */
   }
*/
   {
      int s;
      for(s = 4;s < 20;s += 1){
         getch();
         setstate_obj_num(pic, 1, 1, s, SHOW);
      }/* for */
   }

#endif

#ifdef __WIN32__


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

   /* регистрация прошла - создадим окно */
   hwnd = CreateWindow(
      szWinName,
      "TBG TEST + Virtual Screen",
      WS_OVERLAPPEDWINDOW,
      0,
      0,
      640,
      480,
      HWND_DESKTOP,
      NULL,
      0,
      NULL);

   ShowWindow(hwnd, SW_SHOW);
   UpdateWindow(hwnd);

   load_file(&pic, argv[1] == NULL ? "testkey.tbg" : argv[1]);
   print_error_tbg(" load_file ");

/*
   {
      short int i = 0, x1, y1, x2, y2;
      while(getsize_obj_num(pic, 5, 1, &x1, &y1, &x2, &y2) == ER_OK){
         printf("[%d] - x1:%d y1:%d x2:%d y2:%d\n",i , x1, y1, x2, y2);
         i++;
      }/* while */
   }
   printf("Press any key...\n");
   getch();
   return;
*/

   /* главный цикл обработки сообщений */

   while( GetMessage(&msg, NULL, 0, 0) ){
      DispatchMessage(&msg);
   }/* while */

   ReleaseDC(HWND_DESKTOP, hdc);
   DeleteEGATools();

#else
//   getch();
//   setstate_obj_num(pic, 255, 2, 5, SHOW);
   getch();
   closegraph();
#endif
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
   HBRUSH hBrush;
   HPEN oldPen, oldBrush;
   char s[255];

   switch(mess){
      case WM_CREATE:
         hdc = GetDC(hwnd);
         memDC = CreateCompatibleDC(hdc);
         maxX = GetDeviceCaps(memDC, HORZRES);
         maxY = GetDeviceCaps(memDC, VERTRES);
         hVScr = CreateCompatibleBitmap(hdc, maxX, maxY);
         oldBMP = SelectObject(memDC, hVScr);
         oldBrush = SelectObject(memDC, cEGA[WHITE].hBrush[SOLID_FILL]);
         oldPen   = SelectObject(memDC, cEGA[WHITE].hPen[SOLID_LINE]);
         PatBlt(memDC, 0, 0, maxX, maxY, PATCOPY);
         ReleaseDC(hwnd, hdc);
         break;

      case WM_DESTROY:
         del_pic(&pic);
         print_error_tbg(" del_pic ");
         SelectObject(memDC, oldPen);
         SelectObject(memDC, oldBrush);
         SelectObject(memDC, oldBMP);
         DeleteObject(hVScr);
         DeleteDC(memDC);
         PostQuitMessage(0);
         break;

      case WM_PAINT:
         hdc = BeginPaint(hwnd, &ps);
         TBG_SetWND(pic, hwnd);
         print_error_tbg(" TBG_SetWND ");
         TBG_SetDC(pic, hdc);
         print_error_tbg(" TBGtoDC ");
//         outpic_all(pic);
         setcolor_obj_num(pic, 100, 100, -1, SHOW);
         EndPaint(hwnd, &ps);

         break;
      case WM_COMMAND:
         if(wParam == 108){
            exit(0);
         }/* if */
         break;

      default:
         return DefWindowProc(hwnd, mess, wParam, lParam);

   }/* switch */

   return 0;

}/* WindowFunc */
/* ----------------------------------------------------------------------------------------------- */
#endif

