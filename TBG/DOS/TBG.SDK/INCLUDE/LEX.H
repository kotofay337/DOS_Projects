#define ECHO

#define NUMINT  1
#define NUMFLT  8
#define TXT  2
#define SEP  3
#define COMS 4 
#define COME 5
#define KEYWORD 6
#define COMCPP 7
#define NEWLINE 9
extern long int vali;
extern double vald;
extern int comment;
extern char buftxt[256];
extern int  maxbuftxt;
extern int*  pnumln;
extern FILE *yyin;

typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern YY_BUFFER_STATE yy_current_buffer;