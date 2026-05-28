#define OFF      0
#define ON       1
#define OK       2
#define CANCEL   3
#define HELP     4
#define CONTINUE 5

char *EditString(char *string);
char* EditString_obj_num(OBJECT **Edit_Pic,     // картинка
                         unsigned char editobj, // обьект
                         unsigned char editnum, // элемент типа "текст"
                         char *string,
                         unsigned char max);
int   EditLine_obj_num(OBJECT **PicTBG,
                       unsigned char editobj,
                       unsigned char editnum);
void Refresh_Display(OBJECT **Edit_Pic,
                     unsigned char editobj,
                     unsigned char editnum);
void Edit_Init(OBJECT **Edit_Pic,
               unsigned char editobj,
               unsigned char editnum,
               char *String,
               unsigned char max);
char* Edit_Stop(OBJECT **Edit_Pic,     // картинка
                         unsigned char editobj, // обьект
                         unsigned char editnum); // elem

extern char String_Edit       [64];
extern short int Bar_Color,Start;
