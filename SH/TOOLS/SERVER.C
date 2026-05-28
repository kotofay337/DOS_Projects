/* - SERVER ---------------------------------------------------------------- */
#include <winsock.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <stdio.h>
#include <string.h>

#include "nettools.h"

OSVERSIONINFO osi;
BOOL Stop = FALSE;
int trailer(char* out, char *str, double t, unsigned int d);

/* ------------------------------------------------------------------------- */
#pragma argsused
DWORD user(LPVOID pd){
   char buf[256];
   for(;;){
      if(trailer(buf, "Сервер сообщений работает...", 10., 25)){
         if(osi.dwMajorVersion > (DWORD)4){
            AnsiToOem(buf, buf);
         }/* if */
         SetConsoleTitle(buf);
      }// if
      if((kbhit()) && (getch() == 27)) /* ESC */
         break;
   }/* for */
   Stop = TRUE;
   ExitProcess(0);
   return 0;
}/* user */
/* ------------------------------------------------------------------------- */
#define _1Mb (1024)
DWORD work(LPVOID pd){
   char myhost[64];
   char *buf;
   long int sbuf = _1Mb;
   char *message;
   long int smessage = _1Mb;
   long int  w, r, i = 0, j,
             cw, cr;
   int f = 1;
   SOCKET s = *((SOCKET*)pd);
   u_long l;

   if((buf = (char*)malloc(_1Mb)) == NULL){
      closesocket(s);
      return 0;
   }/* if */

   if((message = (char*)calloc(1, _1Mb)) == NULL){
      free(buf);
      closesocket(s);
      return 0;
   }/* if */

   if(ioctlsocket(s, FIONBIO, &f) != 0){
      pe("ioctl");
   }/* if */

   gethostname(myhost, sizeof(myhost));

   cw = cr = 0;

   for(;;){
      if(Stop)
         break;
      r = lrecv(s, buf, _1Mb, 0);
      if((r == -1)||(r == 0)){
      }/* if */
      else{
         printf("%s\n", buf);
         if(strstr(buf, "CloseConnect") != NULL)
            SetConsoleTitle(buf);
         if(strstr(buf, "OpenConnect") != NULL)
            SetConsoleTitle(buf);
         cr = 0;

         w = lsend(s, buf, _1Mb, 0);
         if((w == -1)||(w == 0)){
         }/* if */
         else{
         }/* elif */
      }/* elif */
   }/* for */

   printf("\nSession terminate.\n");

   free(buf);
   free(message);

   shutdown(s, 2);
   closesocket(s);
   return 0;
}/* work */
/* ------------------------------------------------------------------------- */
int main(void){
   int iret;
   HANDLE hThUser;
   DWORD  IDUserTh;
   HANDLE hThWork;
   DWORD  IDWorkTh;
   SOCKET g;

   osi.dwOSVersionInfoSize = sizeof(osi);
   GetVersionEx(&osi);

   if(SERVER_Init() == -1){
      printf("SERVER_Init failed.\n");
      exit(-1);
   }/* if */

   if(SERVER_Setup() == -1){
      printf("SERVER_Setup failed.\n");
      exit(-1);
   }/* if */

   hThUser = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)user,
                          NULL, 0, &IDUserTh);
   if(hThUser == NULL){
      pe("CreateThread \"user\"");
      exit(-1);
   }/* if */

   for(;;){
      g = SERVER_Wait_Connection();
      if(g == INVALID_SOCKET){
         pe("accept");
         exit(-1);
      }/* if */
      else{
         hThWork = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)work,
                                (LPVOID)&g, 0, &IDWorkTh);
         if(hThWork == NULL){
            pe("CreateThread \"work\"");
            exit(-1);
         }/* if */
      }/* elif */
   }/* for */
}/* main */
/* ------------------------------------------------------------------------- */

