//---------------------------------------------------------------------------

#include <vcl.h>
#include "tbg_tc.h"
#pragma hdrstop

USEFORM("w_opros.cpp", Form1);
USEFORM("test_blt.cpp", Form2);
USEFORM("rtfOptions.cpp", Form4);
USEFORM("ObjectInspector.cpp", Form3);
USEFORM("btnOptions.cpp", Form5);
USEFORM("tbgOptions.cpp", Form6);
USEFORM("bmpOptions.cpp", Form7);
USEFORM("intOptions.cpp", Form8);
USEFORM("strOptions.cpp", Form9);
USEFORM("strOptionsEdit.cpp", Form10);
USEFORM("ansOptions.cpp", Form11);
USEFORM("BiletOption.cpp", Bilet_Option);
USEFORM("OprosOption.cpp", Opros_Option);
//---------------------------------------------------------------------------
char szMainPath[500];              // путь к каталогу откуда запущена программа
 char szPatName [500];               // путь к задаче
 char szMpcPath [500];               // путь к каталогу макросов
 int FlagKat = 0;
 int TestBlt = 1; /* флаг режима работы  1 - редактор опросов  0 - проигрыватель опросов  */
 AnsiString szNameKat_1 = "";
 AnsiString szNameKat_2 = "";
 AnsiString szNameKat_3 = "";
 AnsiString szNameKat_4 = "";
 AnsiString szNameKat_5 = "";
 AnsiString BiletCaption; /* заголовок окна билета  */
//---------------------------------------------------------------------------
int Read_Ini_File( char * name );
int get_str(FILE *name, char *str);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
//       char *TBGPath = "c:\\tbg\\bin\\";
//       MPCSetPath(TBGPath);
//       SetPathToBGIFont(TBGPath);
    memset(szMainPath,'\0', sizeof(szMainPath));
    memset(szPatName,'\0', sizeof(szPatName));
    memset(szMpcPath,'\0', sizeof(szMpcPath));



    if(Read_Ini_File("opros.cfg"))
     {
       char IniPath[500];
       /* имя каталога откуда запущена программа  */
       sprintf(szMainPath,"%s",(ExtractFilePath(ParamStr(0))).c_str());
       sprintf(IniPath,"%s%s",szMainPath,"opros.cfg");
       Read_Ini_File(IniPath); //"c:\\tbg\\bin\\opros.cfg"
     }

    if(strlen(szMpcPath) > 0)
     {
        MPCSetPath(szMpcPath); // szMpcPath заканчивается символом '\'
        SetPathToBGIFont(szMpcPath);
     }

    if(strlen(szPatName) > 0)
        chdir(szPatName);


       CreateEGATools();
       randomize();

       Application->Initialize();
       Application->CreateForm(__classid(TForm1), &Form1);
       Application->CreateForm(__classid(TForm4), &Form4);
       Application->CreateForm(__classid(TForm3), &Form3);
       Application->CreateForm(__classid(TForm5), &Form5);
       Application->CreateForm(__classid(TForm6), &Form6);
       Application->CreateForm(__classid(TForm7), &Form7);
       Application->CreateForm(__classid(TForm8), &Form8);
       Application->CreateForm(__classid(TForm9), &Form9);
       Application->CreateForm(__classid(TForm10), &Form10);
       Application->CreateForm(__classid(TForm11), &Form11);
       Application->CreateForm(__classid(TBilet_Option), &Bilet_Option);
       Application->CreateForm(__classid(TOpros_Option), &Opros_Option);
       Application->Run();

       DeleteEGATools();
   }
   catch (Exception &exception)
   {
       Application->ShowException(&exception);
   }
   return 0;
}
//---------------------------------------------------------------------------
/* ------------------------------------------------------------------------- */
int Read_Ini_File( char * name )
{
  FILE *fptr;
  char msg[500],msg1[250],msg2[250],msg3[250];


  if ((fptr = fopen(name, "rt")) != NULL)
   {/* открыли файл инициализации  */
    while(1)
     {
      memset(msg,  '\0', 449);
      memset(msg1, '\0', 249);
      memset(msg2, '\0', 249);
      memset(msg3, '\0', 249);

      if(get_str(fptr,msg)) break;

      sscanf(msg,"%s %s %s",msg1,msg2,msg3);

//MessageBox(NULL,msg,"msg",MB_OK|MB_ICONSTOP);

      if(!(strcmp(msg1,"MPC_PATH"))) // путь к каталогу макросов
		 {  /*szMpcPath */
        memset(szMpcPath,'\0', 255);
        sprintf(szMpcPath,"%s",msg3);
        strcat(szMpcPath, "\\"); // добавляем символ '\' в конце строки
// MessageBox(NULL,szMpcPath,"szMpcPath",MB_OK|MB_ICONSTOP);
		 } /*  */
      if(!(strcmp(msg1,"PATH"))) // путь к рабочему каталогу
		 {  /*szPatName*/
        memset(szPatName,'\0', 255);
        sprintf(szPatName,"%s",msg3);
// MessageBox(NULL,szPatName,"szPatName",MB_OK|MB_ICONSTOP);
		 } /*  */

      if(!(strcmp(msg1,"NameKat_1")))
		 {
        FlagKat = 1;
        szNameKat_1 = AnsiStrScan(msg,'=') + 1;
        Trim(szNameKat_1);
        OemToChar(szNameKat_1.c_str(),szNameKat_1.c_str());
// MessageBox(NULL,szNameKat_1.c_str(),"NameKat_1",MB_OK|MB_ICONSTOP);
		 } /*  */
      if(!(strcmp(msg1,"NameKat_2")))
		 {
        FlagKat = 1;
        szNameKat_2 = AnsiStrScan(msg,'=') + 1;
        Trim(szNameKat_2);
        OemToChar(szNameKat_2.c_str(),szNameKat_2.c_str());
// MessageBox(NULL,szNameKat_2.c_str(),"NameKat_2",MB_OK|MB_ICONSTOP);
		 } /*  */
      if(!(strcmp(msg1,"NameKat_3")))
		 {
        FlagKat = 1;
        szNameKat_3 = AnsiStrScan(msg,'=') + 1;
        Trim(szNameKat_3);
        OemToChar(szNameKat_3.c_str(),szNameKat_3.c_str());
// MessageBox(NULL,szNameKat_3.c_str(),"NameKat_3",MB_OK|MB_ICONSTOP);
		 } /*  */
      if(!(strcmp(msg1,"NameKat_4")))
		 {
        FlagKat = 1;
        szNameKat_4 = AnsiStrScan(msg,'=') + 1;
        Trim(szNameKat_4);
        OemToChar(szNameKat_4.c_str(),szNameKat_4.c_str());
// MessageBox(NULL,szNameKat_4.c_str(),"NameKat_4",MB_OK|MB_ICONSTOP);
		 } /*  */
      if(!(strcmp(msg1,"NameKat_5")))
		 {
        FlagKat = 1;
        szNameKat_5 = AnsiStrScan(msg,'=') + 1;
        Trim(szNameKat_5);
        OemToChar(szNameKat_5.c_str(),szNameKat_5.c_str());
// MessageBox(NULL,szNameKat_5.c_str(),"NameKat_5",MB_OK|MB_ICONSTOP);
		 } /*  */

     }/* end while */
   } /* end открыли файл инициализации  */
   else
   {
//    MessageBox(NULL,name,"Нет файла инициализации",MB_OK|MB_ICONSTOP);
    return 1;
   }

   if(fptr) fclose(fptr);
   return 0;
} /* end Read_Ini_File  */
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */

int get_str(FILE *name, char *str)
{
 int i,i1;
 char st[500];
 int fl = 0,F_coment = 0;

 while(1)
 {
  memset(st, '\0', sizeof(st));
  i1 = 0;
  if((fgets(st, sizeof(st), name)) == NULL) break;  /* зЁв _¬ бвpRЄг */

  i = 0;
  while(st[i]){  /* § ¬_-п_¬ в Ўг<пжЁо -  ЇаRЎ_<л */
     if(st[i] == '\t')  st[i] = ' ';
     i++;
   }/*end while  */

 i = 0;
 while(st[i]){ /* г¤ <п_¬ ў_¤гйЁЁ ЇаRЎ_<л */
    if(st[i] != ' ') break;
    i++;
  }/*end while  */

 while(st[i]){
       if((st[i] == '/')&&(st[i+1] == '*'))
	  {
	   F_coment++;
	   i+=2;
	   continue;
	  }
       if((st[i] == '*')&&(st[i+1] == '/'))
	  {
	   F_coment--;
	   i+=2;
	   continue;
	  }
       if(F_coment < 0){
	  return (1);
	  }
       if(F_coment) goto rep;

       if((st[i] == '\\')&&(st[i+1] == '"')) { i++; goto r;}

       if((st[i] == '"')&&(!fl)) { fl = 1; goto rep; }
       if((st[i] == '"')&&(fl))  { fl = 0; goto rep; }

       if((st[i] == ' ')&&(st[i+1] == ' ')&&(!fl)) goto rep;

       if((st[i] == ' ')&&(st[i+1] == '\n')) goto rep;
       if((st[i] == ' ')&&(st[i+1] == '\0')) goto rep;
r:     if((st[i] == '\n')||((st[i] == '/')&&(st[i+1] == '/'))||
	  (st[i] == '\0'))  break;
	  str[i1] = st[i];/* Ў_§ Ї_а_ўR¤  ў ў_ае-Ёc а__Ёбва */
       i1++;
rep:   i++;
  }/*end while  */

   if(i1 > 0){ /* бваRЄ  -_ Їгбв п  */
	       str[i1] = '\0';
	       return (0);  /* гбЇ_и-R_ § ў_аи_-Ё_  */
	     } /* if  */
 }/* end while */
 return (1);
}/* end get string */

/* ----------------------------------------------------------------------- */

