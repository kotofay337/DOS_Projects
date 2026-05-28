/* - CLIENT ---------------------------------------------------------------- */
#include <windows.h>
#include <process.h>
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <string.h>
#include <stdio.h>

#include "nettools.h"
OSVERSIONINFO osi;
/* ------------------------------------------------------------------------- */
char buf[1024], *host;
char message[1024];
int fr, fw = sizeof(message), tr = 0, j, i = 0;
int sbuf = sizeof(buf);
/* ------------------------------------------------------------------------- */
void pb(char *c, int n){
   int i;
   for(i = 0; i < n; i++)
      printf("%2.2x ",c[i]);
}/* pb */
/* ------------------------------------------------------------------------- */
int main(int argc, char *argv[], char *env[]){
   char myhost[256];
   char myIPNumber[] = "000.000.000.000";
   char servIPNumber[] = "000.000.000.000";
   struct sockaddr_in sin;
   struct hostent* h;
   struct servent* sp;
   long int r, w;
   SOCKET s;
   int err;

   osi.dwOSVersionInfoSize = sizeof(osi);
   GetVersionEx(&osi);
   if(osi.dwMajorVersion <= (DWORD)4){
      SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
   }/* if */

   if(CLIENT_Init() == -1){
      printf("CLIENT_Init failed.\n");
      exit(-1);
   }/* if */

   if(gethostname(myhost, sizeof(myhost)) == 0)
      printf("myhost: >%s<\n", myhost);

   if(argc == 2)
      host = argv[1];
   else
      host = myhost;

   h = gethostbyname(myhost);
   CopyMemory(&sin.sin_addr, h->h_addr, h->h_length);
   strcpy(myIPNumber, inet_ntoa(sin.sin_addr));

   h = gethostbyname(host);

   if(h == NULL){
      pe("gethostbyname");
      exit(-1);
   }/* if */

   sp = getservbyname("ttkserv", "tcp");
   if(sp == 0){
      pe("getservbyname");
      exit(-1);
   }/* if */

   memset(&sin, 0, sizeof(sin));
   memset(buf, 0, sizeof(buf));

   CopyMemory(&sin.sin_addr, h->h_addr, h->h_length);
            /* FD_SET */
   sin.sin_family = h->h_addrtype;
   sin.sin_port = sp->s_port;
   printf("Connecting to host: %s\n", host);
   strcpy(servIPNumber, inet_ntoa(sin.sin_addr));

   SetConsoleTitle(host);

   s = socket(h->h_addrtype, SOCK_STREAM, 0);
   if(s == INVALID_SOCKET){
      pe("socket");
      exit(-1);
   }/* if */

   if(connect(s, (struct sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR){
      pe("connect");
      closesocket(s);
      exit(-1);
   }/* if */

/*
   FD_SET(s, &set);
   FD_ZERO(&set);
*/
   strcpy(message, "OpenConnect with ");
   strcat(message, myhost);
   strcat(message, ": ");
   strcat(message, myIPNumber);
   strcat(message, "\x7\x7\x7");

   if((w = lsend(s, message, fw, 0)) <= 0)
      CLIENT_Quit(s);

   for(;;){
      if((r = lrecv(s, buf, sizeof(buf), 0)) <= 0){
      }
      else{
         if(strstr(buf, "CloseConnect") != NULL)
            break;
         strcpy(buf, host);
         strcat(buf, " received OK!");
         SetConsoleTitle(buf);
      }

      strcpy(message, myhost);
      strcat(message, ": ");
      printf(".");
      strcat(message, gets(buf));
      message[fw - 1] = '\0';
      if((w = lsend(s, message, fw, 0)) <= 0)
         break;
      strcpy(buf, host);
      SetConsoleTitle(buf);
   }/* for */

   CLIENT_Quit(s);

   return 0;

}/* main */
/* ------------------------------------------------------------------------- */

