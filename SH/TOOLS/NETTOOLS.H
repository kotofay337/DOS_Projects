/* - NetTools.H ------------------------------------------------------------ */
#ifndef _NETTOOLS_H_
#define _NETTOOLS_H_

/* nettools.c */
/* Function block-oriented send & recv (BSD socket's) */
#ifdef _Windows
long int lsend(SOCKET f, const char *p, long int s, int flags);
long int lrecv(SOCKET f, const char *p, long int s, int flags);
#else
long int lsend(int f, char *p, long int s, int flags);
long int lrecv(int f, char *p, long int s, int flags);
#endif

void pe(char *s);
int SERVER_Init(void);
#ifdef _Windows
int SERVER_Quit(SOCKET f);
#else
int SERVER_Quit(int f);
#endif
int SERVER_Setup(void);
int SERVER_Wait_Connection(void);
int SERVER_Close_Connection(int s);
/* ------------------------------------------------------------------------- */
#endif

