/* - NetTools -------------------------------------------------------------- */
#ifdef _Windows
#include <winsock.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif
#include <stdio.h>
#include <string.h>

#include "nettools.h"
/* ------------------------------------------------------------------------- */
static struct sockaddr_in from;
static int len = sizeof(from);
#ifdef _Windows
static SOCKET f;
#else
static int f;
#endif
/* ------------------------------------------------------------------------- */
#ifdef _Windows
long int lsend(SOCKET f, const char *p, long int s, int flags){
#else
long int lsend(int f, char *p, long int s, int flags){
#endif

   long int All, SendUnit;/* Всего принято, принято элементов */
   char *tmp = (char*)p;/* указатель на текущий буфер размером = (s - All) */

   for(All = SendUnit = 0;All < s;){
      /* Передаём блок */
      SendUnit = send(f, tmp, s - All, flags);
#ifdef _Windows
      if((SendUnit == SOCKET_ERROR) || (SendUnit == 0))
#else
      if((SendUnit == -1) || (SendUnit == 0))
#endif
         return SendUnit;/* Нет ответа */
      tmp += SendUnit;/* Смещаем на сколько передано */
      All += SendUnit;/* Сколько передано? */
   }/* for */

   return All;

}/* lsend */
/* ----------------------------------------------------------------------- */
#ifdef _Windows
long int lrecv(SOCKET f, const char *p, long int s, int flags){
#else
long int lrecv(int f, char *p, long int s, int flags){
#endif

   long int All, ReceiveUnit;
   char *tmp = (char*)p;

   for(All = ReceiveUnit = 0;All < s;){
      ReceiveUnit = recv(f, tmp, s - All, flags);
#ifdef _Windows
      if((ReceiveUnit == SOCKET_ERROR) || (ReceiveUnit == 0))
#else
      if((ReceiveUnit == -1) || (ReceiveUnit == 0))
#endif
         return ReceiveUnit;
      tmp += ReceiveUnit;
      All += ReceiveUnit;
   }/* for */

   return All;

}/* lrecv */
/* ------------------------------------------------------------------------- */
void peWSAError(int WSAErr){
char *se = "WSA Not Errors.";
switch(WSAErr){
       case WSANOTINITIALISED: se = "A successful WSAStartup must occur before using this function. ";
break; case WSAENETDOWN: se = "The network subsystem has failed.";
break; case WSAEADDRINUSE: se = "The specified address is already in use.";
break; case WSAEINTR: se = "The (blocking) call was canceled through WSACancelBlockingCall.";
break; case WSAEINPROGRESS: se = "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.";
break; case WSAEALREADY: se = "A nonblocking connect call is in progress on the specified socket.Note  In order to preserve backward compatibility, this error is reported as break; case WSAEINVAL to Windows Sockets 1.1 applications that link to either WINSOCK.DLL or WSOCK32.DLL.";
break; case WSAEADDRNOTAVAIL: se = "The specified address is not available from the local machine.";
break; case WSAEAFNOSUPPORT: se = "Addresses in the specified family cannot be used with this socket.";
break; case WSAECONNREFUSED: se = "The attempt to connect was forcefully rejected.";
break; case WSAEFAULT: se = "The name or the namelen argument is not a valid part of the user address space, the namelen argument is too small, or the name argument contains incorrect address format for the associated address family.";
break; case WSAEINVAL: se = "The parameter s is a listening socket, or the destination address specified is not consistent with that of the constrained group the socket belongs to.";
break; case WSAEISCONN: se = "The socket is already connected (connection-oriented sockets only).";
break; case WSAENETUNREACH: se = "The network cannot be reached from this host at this time.";
break; case WSAENOBUFS: se = "No buffer space is available. The socket cannot be connected.";
break; case WSAENOTSOCK: se = "The descriptor is not a socket.";
break; case WSAETIMEDOUT: se = "Attempt to connect timed out without establishing a connection.";
break; case WSAEWOULDBLOCK: se = "The socket is marked as nonblocking and the connection cannot be completed immediately. It is possible to select the socket while it is connecting by selecting it for writing.";
break; case WSAEACCES: se = "Attempt to connect datagram socket to broadcast address failed because setsockopt SO_BROADCAST is not enabled.";
}
printf(se);
}
/* ------------------------------------------------------------------------- */
void pe(char *s){
   int e1 = errno,
       e2;
   perror(s);
#ifdef _Windows
   e2 = WSAGetLastError();
   printf("%s: errno: %d, WSA Error: ", s, e1);
   peWSAError(e2);
   printf("\n");
#endif
   if(e1 || e2){
#ifdef _Windows
      getch();
#else
      getchar();
#endif
   }/* if */
}/* pe */
/* ------------------------------------------------------------------------- */
int SERVER_Init(void){
#ifdef _Windows
   WORD wVersionRequested;
   WSADATA wsaData;
   int err;

   wVersionRequested = MAKEWORD(1, 1);
   err = WSAStartup(wVersionRequested, &wsaData);

   if (err != 0){
       printf("WSA not accessible.\n");
       return -1;
   }/* if */

   if ( LOBYTE( wsaData.wVersion ) != 1 ||
        HIBYTE( wsaData.wVersion ) != 1 ){
       printf("WSA low version!\n");
       WSACleanup();
       return -1;
   }/* if */
#endif
   return 0;
}/* SERVER_Init */
/* ------------------------------------------------------------------------- */
CLIENT_Init(void){
   return SERVER_Init();
}/* CLIENT_Init */
/* ------------------------------------------------------------------------- */
#ifdef _Windows
int SERVER_Quit(SOCKET f){
#else
int SERVER_Quit(int f){
#endif
   shutdown(f, 2);
#ifdef _Windows
   closesocket(f);
   WSACleanup();
#else
   close(f);
#endif
   return 0;
}/* SERVER_Quit */
/* ------------------------------------------------------------------------- */
#ifdef _Windows
int CLIENT_Quit(SOCKET f){
#else
int CLIENT_Quit(int f){
#endif
   return SERVER_Quit(f);
}/* CLIENT_Quit */
/* ------------------------------------------------------------------------- */
int SERVER_Setup(void){
   struct servent *sp;

   sp = getservbyname("ttkserv", "tcp");
   if(sp == 0){
      pe("getservbyname");
      return -1;
   }/* if */

#ifdef _Windows
   f = socket(AF_INET, SOCK_STREAM, 0);
   if(f == INVALID_SOCKET){
#else
   f = socket(AF_INET, SOCK_STREAM, 0);
   if(f == -1){
#endif
      pe("socket");
      return -1;
   }/* if */

   memset(&from, 0, sizeof(struct sockaddr_in));
#ifdef _Windows
   from.sin_family = AF_INET;
   from.sin_addr.s_addr = INADDR_ANY;
#endif
   from.sin_port = sp->s_port;

#ifdef _Windows
   gotoxy(1, 1);
#endif

   printf("port: >%u<\n", sp->s_port);

#ifdef _Windows
   if(bind(f, (struct sockaddr*)&from, sizeof(from)) == SOCKET_ERROR){
#else
   if(bind(f, (struct sockaddr*)&from, sizeof(from)) == -1){
#endif
      pe("bind");
      return -1;
   }/* if */

   listen(f, 1);

   return 0;
}/* SERVER_Setup */
/* ------------------------------------------------------------------------- */
/*
int CLIENT_Connect(char *host){
   h = gethostbyname(host);
#ifdef _Windows
   if(h == NULL){
#else
   if(h == 0){
#endif
      pe("gethostbyname");
      exit(-1);
   }/* if */

   sp = getservbyname("myserv", "tcp");
   if(sp == 0){
      pe("getservbyname");
      exit(-1);
   }/* if */

#ifdef _Windows
   memset(&sin, 0, sizeof(sin));
   memset(buf, 0, sizeof(buf));
#else
   bzero(&sin, sizeof(sin));
   bzero(buf, sizeof(buf));
#endif

#ifdef _Windows
   CopyMemory(&sin.sin_addr, h->h_addr, h->h_length);
#else
   bcopy(h->h_addr, (char*)&sin.sin_addr, h->h_length);
#endif
            /* FD_SET */
   sin.sin_family = h->h_addrtype;
   sin.sin_port = sp->s_port;

   printf("port: >%u<\n", sp->s_port);
   printf("addr: >");
   pb(h->h_addr, sizeof(struct in_addr));
   printf("<\n");
   printf("to host: %s\n", host);

   s = socket(h->h_addrtype, SOCK_STREAM, 0);
#ifdef _Windows
   if(s == INVALID_SOCKET){
#else
   if(s == -1){
#endif
      pe("socket");
      exit(-1);
   }/* if */

#ifdef _Windows
   if(connect(s, (struct sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR){
#else
   if(connect(s, (struct sockaddr*)&sin, sizeof(sin)) == -1){
#endif
      pe("connect");
#ifdef _Windows
      closesocket(s);
#else
      close(s);
#endif
      exit(-1);
   }/* if */
}/* CLIENT_Connect */
*/
/* ------------------------------------------------------------------------- */
int SERVER_Wait_Connection(void){
   memset(&from, 0, sizeof(struct sockaddr_in));
   return (int)accept(f, (struct sockaddr*)&from, &len);
}/* SERVER_Wait_Connection */
/* ------------------------------------------------------------------------- */
int SERVER_Close_Connection(int s){
#ifdef _Windows
   closesocket(s);
   return 0;
#else
   close(s);
   exit(0);
#endif
}/* SERVER_Close_Connection */
/* ----------------------------------------------------------------------- */

