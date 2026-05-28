//---------------------------------------------------------------------------
// tbl.h
//---------------------------------------------------------------------------
#define Offset_to_TBL   16
#define ADD           0x10
#define DELETE        0x11
#define EXTRACT       0x12
#define LIST          0x13
#define UPDATE        0x14
#define CREATE_FLASH  0x15
//---------------------------------------------------------------------------
struct HTBG{ // Заголовок для элемента библиотеки TBG.
    char name[15];// имя файла картинки
    long len;     // длина файла картинки в байтах
};
//---------------------------------------------------------------------------
long ffile(FILE *lf,char *name);
char *NextFile(FILE *lf);
int  add (void);
int  ext (void);
void list(void);
int  del (void);
int  create_flash_dir(void);
//---------------------------------------------------------------------------
