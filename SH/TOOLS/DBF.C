/* ----------------------------------------------------------------------- */
/*
*
*
*     DBF.C
*
*     (С) Чумак А.И. 2000
*     (╤) ╫єьръ └.╚. 2000
*
*/
/* ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <dos.h>
#include <dir.h>
#include "dbf.h"

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

void MessageBox(char *Title, char *MsgFmt, ...);
/* ----------------------------------------------------------------------- */
static DBFError DBFEr = DBF_OK;
static unsigned char StartData = 0x0D, EndData = 0x1A;
static unsigned char FieldValue[256];
static char PathToDBF[MAXPATH] = ".\\DB\\";
static unsigned char *DBFErrorString[] ={
#ifndef __WIN32__
      "Нет ошибок",
      "Нет открытой базы данных",
      "База пуста",
      "Файл базы не открывается или не существует",
      "Ошибка при чтении из базы",
      "Ошибка записи базы",
      "Ошибка перемещения указателя по базе",
      "Указатель в конце базы",
      "Ошибка распределения памяти",
      "Ошибка во внутренней структуре базы",
      "Поле не описано в данной базе",
      "Слишком длинное значение для записи в поле",
      "Не найдена запись по ключу",
      "Неправильный формат времени",
      "Неправильный формат даты"
#else
      "═хЄ ю°шсюъ",
      "═хЄ юЄъЁ√Єющ срч√ фрээ√ї",
      "┴рчр яєёЄр",
      "╘рщы срч√ эх юЄъЁ√трхЄё  шыш эх ёє∙хёЄтєхЄ",
      "╬°шсър яЁш ўЄхэшш шч срч√",
      "╬°шсър чряшёш срч√",
      "╬°шсър яхЁхьх∙хэш  єърчрЄхы  яю срчх",
      "╙ърчрЄхы№ т ъюэЎх срч√",
      "╬°шсър ЁрёяЁхфхыхэш  ярь Єш",
      "╬°шсър тю тэєЄЁхээхщ ёЄЁєъЄєЁх срч√",
      "╧юых эх юяшёрэю т фрээющ срчх",
      "╤ыш°ъюь фышээюх чэрўхэшх фы  чряшёш т яюых",
      "═х эрщфхэр чряшё№ яю ъы■ўє",
      "═хяЁртшы№э√щ ЇюЁьрЄ тЁхьхэш",
      "═хяЁртшы№э√щ ЇюЁьрЄ фрЄ√"
#endif
};
/* ----------------------------------------------------------------------- */
void SetPathToDBF (char *p){
   if(p)
      strncpy(PathToDBF, p, MAXPATH);
}/* SetPathToDBF */
/* ----------------------------------------------------------------------- */
#ifdef USE_ALIASES
DBF* DBFOpen(unsigned Alias){
#else
DBF* DBFOpen(unsigned char *FileName){
#endif

   FILE *fdbf = NULL;
   DBF *dbf   = NULL;
   DBFDefinition def;
   int i;

#ifdef USE_ALIASES
   unsigned char FileName[18];
   sprintf(FileName, "%s%3.3u.DBF", PathToDBF, Alias % 1000);
#endif

   DBFEr = DBF_OK;
   dbf = (DBF*)calloc(1U, sizeof(DBF));
   if(dbf == NULL){
      DBFEr = DBF_ERROR_MEMORY_ALLOCATE;
      return NULL;
   }/* if */

#ifdef DEBUG
   printf("\n--- DataBase: %s ---", FileName);
#endif

   fdbf = fopen(FileName, "r+b");
   if(fdbf == NULL){
      DBFEr = DBF_ERROR_FILE_OPEN;
      return NULL;
   }/* if */
   dbf->fdbf = fdbf;

   /* Read header */
   if(fread(&def, 1, sizeof(DBFDefinition), fdbf) != sizeof(DBFDefinition)){
      DBFEr = DBF_ERROR_FILE_READ;
      return NULL;
   }/* if */
   dbf->header = def.h;
   dbf->NumFields = def.h.SHeader / sizeof(DBFField) - 1;

   /* Read Field definition */
   dbf->fields = (LPDBFField)calloc(dbf->NumFields, sizeof(DBFField));
   if(dbf->fields == NULL){
      DBFEr = DBF_ERROR_MEMORY_ALLOCATE;
      return NULL;
   }/* if */

   if(fread(dbf->fields, sizeof(DBFDefinition), dbf->NumFields, fdbf) != dbf->NumFields){
      DBFEr = DBF_ERROR_FILE_READ;
      return NULL;
   }/* if */

   if(fread(&StartData, 1, 1, fdbf) != 1){
      DBFEr = DBF_ERROR_FILE_READ;
      return NULL;
   }/* if */

   if(StartData != 0x0D){
      DBFEr = DBF_ERROR_DATABASE_STRUCTURE;
      return NULL;
   }/* if */

   dbf->oStartData = ftell(fdbf);

   dbf->Record = (unsigned char*)malloc(dbf->header.SRecord);
   if(dbf->Record == NULL){
      DBFEr = DBF_ERROR_MEMORY_ALLOCATE;
      return NULL;
   }/* if */

   memset(dbf->Record, ' ', dbf->header.SRecord);

   fseek(fdbf, -1, SEEK_END);

   if(fread(&EndData, 1, 1, fdbf) != 1){
      DBFEr = DBF_ERROR_FILE_READ;
      return NULL;
   }/* if */

   if(EndData != 0x1A){
      DBFEr = DBF_ERROR_DATABASE_STRUCTURE;
      return NULL;
   }/* if */

/*
#ifdef DEBUG
   printf("\n MEMO:  %s"
          "\n Date:  %2.2d-%2.2d-%4.4d"
          "\n NRecs: %lu"
          "\n SHead: %d"
          "\n SRecd: %d"
          "\n Nf:    %d"
          ,dbf->header.Memo == 0x83 ? "YES" : "NO"
          ,dbf->header.D, dbf->header.M, dbf->header.Y + 1900
          ,dbf->header.NRecs
          ,dbf->header.SHeader
          ,dbf->header.SRecord
          ,dbf->NumFields);
   printf("\n --- Fields ---");
   for(i = 0;i < dbf->NumFields;i++){
      {// lockblock
         LPDBFField defr;
         defr = dbf->fields + i;
         printf("\n  %2.1d|%10.10s|%s|%3d"
         , i
         , defr->fName
         , defr->fType == 0x43 ? "Symbol" :
           defr->fType == 0x4E ? "Number" :
           defr->fType == 0x4D ? "MEMO  " :
           defr->fType == 0x4C ? "BOOL  " :
           defr->fType == 0x44 ? "DATE  " : "? Unknown or error !"
         , defr->fSize);
      }// lockblock end
   }/* for */

   fseek(fdbf, dbf->oStartData, SEEK_SET);
   printf("\n --- Records ---");
   for(i = 0;i < dbf->header.NRecs;i++){
      int nfield, j;
      j = 0;
      nfield = 0;
      fread(dbf->Record, dbf->header.SRecord, 1, fdbf);
      printf("\n|%1.1c", dbf->Record[j]);
      j++;

      for(nfield = 0;nfield < dbf->NumFields;nfield++){
         printf("|");

         switch(dbf->fields[nfield].fType){
            int k;
            case 0x43: // Chars
               for(k = 0;k < dbf->fields[nfield].fSize;k++){
                  printf("%c", dbf->Record[j++]);
               }/* for */
               break;
            case 0x4D: // MEMO
               for(k = 0;k < 10;k++){
                  printf("%c", dbf->Record[j++]);
               }/* for */
               break;
            case 0x4E: // Number
               {
                  unsigned char s[256];
                  memset(s, 0, sizeof(s));
                  for(k = 0;k < dbf->fields[nfield].fSize;k++){
                     s[k] = dbf->Record[j++];
                  }/* for */

                  if(dbf->fields[nfield].fFloat){
                     printf("%*f", dbf->fields[nfield].fSize, atof(s));
                  }/* if */
                  else{
                     if(dbf->fields[nfield].fSize <= 5){
                        printf("%*u", dbf->fields[nfield].fSize, atoi(s));
                     }/* if */
                     else{
                        printf("%*lu", dbf->fields[nfield].fSize, strtoul(s, NULL, 10));
                     }/* elif */
                  }/* elif */
               }
               break;
            case 0x4C: // BOOL
               printf("%s", dbf->Record[j] == 'T' ? "TRUE " :
                            dbf->Record[j] == 'F' ? "FALSE" : "     ");
               j++;
               break;
            case 0x44: // Date
               {
                  int M, D, Y;
                  if(sscanf(&(dbf->Record[j]), "%4d%2d%2d", &Y,&M,&D) == 3){
                     printf("%2.2d-%2.2d-%4.4d", D, M, Y);
                     j += 8;
                  }/* if */
               }
               break;
         }/* switch */
         printf("");
      }/* for */
      printf("|");
   }/* for */
#endif
*/
   fseek(fdbf, dbf->oStartData, SEEK_SET);

   dbf->Current = 1;

   return dbf;
}/* DBFOpen */
/* ----------------------------------------------------------------------- */
/*
   Пример создания базы: 5 полей

       DBFField flds[5];
       memset(flds, 0, sizeof(flds));
       // заполнение полей
       strcpy(flds[0].fName, "NAME1");
       strcpy(flds[1].fName, "NAME2");
       strcpy(flds[2].fName, "NAME3");
       strcpy(flds[3].fName, "NAME4");
       strcpy(flds[4].fName, "NAME5");

       flds[0].fType = 'C';
       flds[1].fType = 'C';
       flds[2].fType = 'C';
       flds[3].fType = 'C';
       flds[4].fType = 'C';

       flds[0].fSize = 32;
       flds[1].fSize = 24;
       flds[2].fSize = 32;
       flds[3].fSize = 24;
       flds[4].fSize = 32;

       flds[0].fOffset = 1;
       flds[1].fOffset = flds[0].fSize + 1;
       flds[2].fOffset = flds[0].fSize + flds[1].fSize + 1;
       flds[3].fOffset = flds[0].fSize + flds[1].fSize + flds[2].fSize + 1;
       flds[4].fOffset = flds[0].fSize + flds[1].fSize + flds[2].fSize + flds[3].fSize + 1;

       flds[0].fFloat = 0;
       flds[1].fFloat = 0;
       flds[2].fFloat = 0;
       flds[3].fFloat = 0;
       flds[4].fFloat = 0;

       DBFCreate(fndb, FALSE, 5, flds);
*/
int DBFCreate(unsigned char *FileName, int Memo, int NumField, LPDBFField flds){
   struct date dt;
   FILE *fdbf;
   DBFHeader dbh;
   int i;
   getdate(&dt);

   fdbf = fopen(FileName, "wb");
   if(fdbf == NULL){
      DBFEr = DBF_ERROR_FILE_OPEN;
      return FALSE;
   }/* if */

   // заполнение заголовка DBF
   memset(&dbh, 0, sizeof(DBFHeader));
   dbh.Memo = Memo == TRUE ? 0x83 : 0x03;
   dbh.Y = dt.da_year - 1900;
   dbh.M = dt.da_mon;
   dbh.D = dt.da_day;
   dbh.NRecs = 0;
   dbh.SHeader = sizeof(DBFHeader) + sizeof(DBFField) * NumField + 1;
   dbh.SRecord = 1; // + знак удаленной записи
   for(i = 0;i < NumField;i++){
      dbh.SRecord += flds[i].fSize;
   }/* for */
   fwrite(&dbh, 1, sizeof(DBFHeader), fdbf);

   // запись описаний полей
   for(i = 0;i < NumField;i++){
      fwrite(&(flds[i]), 1, sizeof(DBFField), fdbf);
   }/* for */

   fwrite(&StartData, 1, sizeof(StartData), fdbf);
   fwrite(&EndData, 1, sizeof(EndData), fdbf);

   fclose(fdbf);

   return TRUE;

}/* DBFCreate */
/* ----------------------------------------------------------------------- */
int DBFClose (DBF *dbf){
   if(dbf){
      fclose(dbf->fdbf);
      if(dbf->fields){
         free(dbf->fields);
      }/* if */
      if(dbf->Record){
         free(dbf->Record);
      }/* if */
      free(dbf);
#ifdef DEBUG
      printf("\n--- DataBase: close ---");
#endif
      return TRUE;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return FALSE;
}/* DBFClose */
/* ----------------------------------------------------------------------- */
int DBFEnd(LPDBF dbf){// Seek on end file
   DBFEr = DBF_OK;
   if(dbf){
      if(dbf->Record){
         // start data
         if(fseek(dbf->fdbf, dbf->oStartData, SEEK_SET) != 0){
            DBFEr = DBF_ERROR_FILE_SEEK;
            return FALSE;
         }/* if */
         // end data
         if(fseek(dbf->fdbf, dbf->header.SRecord * dbf->header.NRecs, SEEK_CUR) != 0){
            DBFEr = DBF_ERROR_FILE_SEEK;
            return FALSE;
         }/* if */
      }/* if */
#ifdef DEBUG
      printf("\n--- DataBase: TO END FILE ---");
#endif
      return TRUE;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return FALSE;
}/* DBFEnd */
/* ----------------------------------------------------------------------- */
int DBFStart(LPDBF dbf){// Seek on start file
   DBFEr = DBF_OK;
   if(dbf){
      if(dbf->Record){
         // start data
         if(fseek(dbf->fdbf, dbf->oStartData, SEEK_SET) != 0){
            DBFEr = DBF_ERROR_FILE_SEEK;
            return FALSE;
         }/* if */
      }/* if */
#ifdef DEBUG
      printf("\n--- DataBase: TO Start FILE ---");
#endif
      dbf->Current = 1;
      return TRUE;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return FALSE;
}/* DBFStart */
/* ----------------------------------------------------------------------- */
int DBFNext(LPDBF dbf){
   DBFEr = DBF_OK;
   if(dbf){
      if(dbf->Record){
         if(fseek(dbf->fdbf, dbf->header.SRecord, SEEK_CUR) != 0){
            DBFEr = DBF_ERROR_FILE_SEEK;
            return FALSE;
         }/* if */
         dbf->Current++;
         if(dbf->Current > dbf->header.NRecs){
            DBFEr = DBF_ERROR_FILE_END;
            dbf->Current--;
         }/* if */
      }/* if */
#ifdef DEBUG
      printf("\n--- DataBase: Next record ---");
#endif
      return TRUE;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return FALSE;
}/* DBFNext */
/* ----------------------------------------------------------------------- */
unsigned char* DBFGetRecord(LPDBF dbf){
   DBFEr = DBF_OK;
   if(dbf){
      if(dbf->Record){
         long s = ftell(dbf->fdbf);
         if((signed)fread(dbf->Record, 1, dbf->header.SRecord, dbf->fdbf) != dbf->header.SRecord){
            if(dbf->Record[0] == 0x1A){
               DBFEr = DBF_ERROR_EMPTY_DATABASE;
               return NULL;
            }/* if */
            DBFEr = DBF_ERROR_FILE_READ;
            return NULL;
         }/* if */
         if(fseek(dbf->fdbf, s, SEEK_SET) != 0){
            DBFEr = DBF_ERROR_FILE_SEEK;
            return NULL;
         }/* if */
      }/* if */
#ifdef DEBUG
      printf("\n--- DataBase: Get record ---");
#endif
      return dbf->Record;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return NULL;
}/* DBFGetRecord */
/* ----------------------------------------------------------------------- */
unsigned char* DBFPutRecord(LPDBF dbf){
   if(dbf){
      if(dbf->Record){
         long s = ftell(dbf->fdbf);
         if((signed)fwrite(dbf->Record, 1, dbf->header.SRecord, dbf->fdbf) != dbf->header.SRecord){
            DBFEr = DBF_ERROR_FILE_WRITE;
            return NULL;
         }/* if */
         if(fseek(dbf->fdbf, s, SEEK_SET) != 0){
            DBFEr = DBF_ERROR_FILE_SEEK;
            return NULL;
         }/* if */
      }/* if */
#ifdef DEBUG
      printf("\n--- DataBase: Put record ---");
#endif
      return dbf->Record;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return NULL;
}/* DBFPutRecord */
/* ----------------------------------------------------------------------- */
int DBFSetFieldAsInt(LPDBF dbf, unsigned char* FieldName, unsigned short int Value){
   char Buf[16];
   sprintf(Buf, "%u", Value);
   return DBFSetFieldAsString(dbf, FieldName, Buf);
}/* DBFSetFieldAsInt */
/* ----------------------------------------------------------------------- */
int DBFSetFieldAsLong(LPDBF dbf, unsigned char* FieldName, unsigned long int Value){
   char Buf[16];
   sprintf(Buf, "%lu", Value);
   return DBFSetFieldAsString(dbf, FieldName, Buf);
}/* DBFSetFieldAsLong */
/* ----------------------------------------------------------------------- */
int DBFSetFieldAsTime(LPDBF dbf, unsigned char* FieldName, struct time *pt){
   char Buf[16]; // getdate();
   sprintf(Buf, "%2.2d:%2.2d:%2.2d", pt->ti_hour, pt->ti_min, pt->ti_sec);
   return DBFSetFieldAsString(dbf, FieldName, Buf);
}/* DBFSetFieldAsTime */
/* ----------------------------------------------------------------------- */
int DBFSetFieldAsDate(LPDBF dbf, unsigned char* FieldName, struct date *pd){
   char Buf[16];
   sprintf(Buf, "%4.4d%2.2d%2.2d", pd->da_year, pd->da_mon, pd->da_day);
   return DBFSetFieldAsString(dbf, FieldName, Buf);
}/* DBFSetFieldAsTime */
/* ----------------------------------------------------------------------- */
int DBFSetFieldAsString(LPDBF dbf, unsigned char* FieldName, unsigned char* Value){
   int oField = 0, sField = 0, i;
   unsigned char* Record;
   DBFEr = DBF_OK;
   if(dbf){
      if(dbf->Record){
         // search Field
         oField++; // feed sign symbol

         for(i = 0;i < (signed)dbf->NumFields;i++){
            LPDBFField defr;
            defr = dbf->fields + i;
            sField = defr->fSize;
            if(strcmp(FieldName, defr->fName) == 0){
               // Field FOUND!
               break;
            }/* if */
            oField += sField;
         }/* for */
         if(i >= (signed)dbf->NumFields){
            DBFEr = DBF_ERROR_UNKNOWN_FIELD;
            return FALSE;
         }/* if */
         if(strlen(Value) > (unsigned)sField){
            DBFEr = DBF_ERROR_LONG_VALUE;
            return FALSE;
         }/* if */
         Record = dbf->Record;

         // clear field
         for(i = 0;i < sField;i++){
            Record[oField + i] = ' ';
         }/* for */

         // copy info into field
         for(i = 0;i < sField;i++){
            if(Value[i] != '\0'){
               Record[oField + i] = Value[i];
            }/* if */
            else{
               break;
            }/* elif */
         }/* for */
      }/* if */
#ifdef DEBUG
      printf("\n--- DataBase: SetFieldAsString ---");
#endif
      return TRUE;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return FALSE;
}/* DBFSetFieldAsString */
/* ----------------------------------------------------------------------- */
int DBFNumRecord(LPDBF dbf, unsigned char* NameKeyField, unsigned char* Criteria){
   int NumRec = 0;
   DBFStart(dbf);
   if(DBFEr == DBF_OK){
      while(TRUE){
         DBFFindRecord(dbf, NameKeyField, Criteria);
         if(DBFEr == DBF_OK){
            NumRec++;
         }/* if */
         else{
            if(DBFEr != DBF_ERROR_RECORD_NOT_FOUND){
               return -1;
            }/* if */
            DBFEr = DBF_OK;
            break;
         }/* elif */
      }/* while */
      return NumRec;
   }/* if */
   return -1;
}/* DBFNumRecord */
/* ----------------------------------------------------------------------- */
int DBFFindRecord(LPDBF dbf, unsigned char* NameKeyField, unsigned char* Criteria){
   unsigned char *Record, *Value;
   DBFEr = DBF_OK;
   if(dbf){
      do{
         Record = DBFGetRecord(dbf);
         if(DBFEr != DBF_OK){
            return FALSE;
         }/* if */
         if(Record[0] == ' '){
            Value = DBFGetFieldAsString(dbf, NameKeyField);
            if(DBFEr != DBF_OK){
               return FALSE;
            }/* if */
#ifdef DEBUG
            printf("\n >%s< ? >%s<", Value, Criteria);
#endif
            if(strcmp(Value, Criteria) == NULL){
               return TRUE;
            }/* if */
         }/* if */
         DBFNext(dbf);
         if((DBFEr != DBF_OK) && (DBFEr != DBF_ERROR_FILE_END)){
            return FALSE;
         }/* if */
      }while(DBFEr != DBF_ERROR_FILE_END);
#ifdef DEBUG
      printf("\n--- DataBase: FindRecord ---");
#endif
      DBFEr = DBF_ERROR_RECORD_NOT_FOUND;
      return FALSE;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return FALSE;
}/* DBFFindRecord */
/* ----------------------------------------------------------------------- */
int DBFGetMarkRecord(LPDBF dbf){// Space - OK, Asterisk - Deleted
   DBFEr = DBF_OK;
   if(dbf){
      if(dbf->Record){
#ifdef DEBUG
         printf("\n--- DataBase: Get Mark record ---");
#endif
         return dbf->Record[0];
      }/* if */
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return FALSE;
}/* DBFGetMarkRecord */
/* ----------------------------------------------------------------------- */
int DBFClearRecord(LPDBF dbf){
   DBFEr = DBF_OK;
   if(dbf){
      if(dbf->Record){
         memset(dbf->Record, ' ', dbf->header.SRecord);
#ifdef DEBUG
         printf("\n--- DataBase: Clear record ---");
#endif
         return TRUE;
      }/* if */
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return FALSE;
}/* DBFGetMarkRecord */
/* ----------------------------------------------------------------------- */
int DBFSetMarkRecord(LPDBF dbf, char Flag){// Space - OK, Asterisk - Deleted
   DBFEr = DBF_OK;
   if(dbf){
      if(dbf->Record){
         switch(Flag){
            case ' ':
               dbf->Record[0] = ' ';
               break;
            case '*':
               dbf->Record[0] = '*';
               break;
            default:
               return FALSE;
         }/* switch */
      }/* if */
#ifdef DEBUG
      printf("\n--- DataBase: Set Mark record ---");
#endif
      return TRUE;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return FALSE;
}/* DBFSetMarkRecord */
/* ----------------------------------------------------------------------- */
unsigned short int DBFGetFieldAsInt(LPDBF dbf, unsigned char* FieldName){
   return atoi(DBFGetFieldAsString(dbf, FieldName));
}/* DBFGetFieldAsInt */
/* ----------------------------------------------------------------------- */
unsigned long int DBFGetFieldAsLong(LPDBF dbf, unsigned char* FieldName){
   return atol(DBFGetFieldAsString(dbf, FieldName));
}/* DBFGetFieldAsInt */
/* ----------------------------------------------------------------------- */
double DBFGetFieldAsDouble(LPDBF dbf, unsigned char* FieldName){
   static char *endptr;
   return strtod(DBFGetFieldAsString(dbf, FieldName), &endptr);
}/* DBFGetFieldAsInt */
/* ----------------------------------------------------------------------- */
int DBFGetFieldAsTime(LPDBF dbf, unsigned char* FieldName, struct time *pt){
   unsigned int H, M, S;
   if(pt && (sscanf(DBFGetFieldAsString(dbf, FieldName), "%2d:%2d:%2d", &H, &M, &S) == 3)){
      pt->ti_hour = H;
      pt->ti_min  = M;
      pt->ti_sec  = S;
      return TRUE;
   }/* if */
   else{
      if(DBFEr != DBF_OK)
         return DBFEr;
      DBFEr = DBF_ERROR_WRONG_TIME_FORMAT;
      return FALSE;
   }/* elif */
}/* DBFGetFieldAsTime */
/* ----------------------------------------------------------------------- */
int DBFGetFieldAsTimeString(LPDBF dbf, unsigned char* FieldName, struct time *pt, char* pts){
   unsigned int H, M, S;
   if((sscanf(DBFGetFieldAsString(dbf, FieldName), "%2d:%2d:%2d", &H, &M, &S) == 3)){
      if(pt){
         pt->ti_hour = H;
         pt->ti_min  = M;
         pt->ti_sec  = S;
      }/* if */
      if(pts){
         sprintf(pts, "%2.1d:%2.2d:%2.2d", H, M, S);
      }/* if */
      return TRUE;
   }/* if */
   else{
      if(DBFEr != DBF_OK)
         return DBFEr;
      DBFEr = DBF_ERROR_WRONG_TIME_FORMAT;
      return FALSE;
   }/* elif */
}/* DBFGetFieldAsTimeString */
/* ----------------------------------------------------------------------- */
int DBFGetFieldAsDate(LPDBF dbf, unsigned char* FieldName, struct date *pd){
   unsigned int Y, M, D;
   if(pd && (sscanf(DBFGetFieldAsString(dbf, FieldName), "%4d%2d%2d", &Y,&M,&D) == 3)){
      pd->da_year = Y;
      pd->da_mon  = M;
      pd->da_day  = D;
      return TRUE;
   }/* if */
   else{
      if(DBFEr != DBF_OK)
         return DBFEr;
      DBFEr = DBF_ERROR_WRONG_DATE_FORMAT;
      return FALSE;
   }/* elif */
}/* DBFGetFieldAsDate */
/* ----------------------------------------------------------------------- */
int DBFGetFieldAsDateString(LPDBF dbf, unsigned char* FieldName, struct date *pd, char *pds){
   unsigned int Y, M, D;
   if(sscanf(DBFGetFieldAsString(dbf, FieldName), "%4d%2d%2d", &Y,&M,&D) == 3){
      if(pd){
         pd->da_year = Y;
         pd->da_mon  = M;
         pd->da_day  = D;
      }/* if */
      if(pds){
         sprintf(pds, "%2.d-%2.2d-%4.4d", D, M, Y);
      }/* if */
      return TRUE;
   }/* if */
   else{
      if(DBFEr != DBF_OK)
         return DBFEr;
      DBFEr = DBF_ERROR_WRONG_DATE_FORMAT;
      return FALSE;
   }/* elif */
}/* DBFGetFieldAsDateString */
/* ----------------------------------------------------------------------- */
unsigned char* DBFGetFieldAsString(LPDBF dbf, unsigned char* FieldName){
   int oField = 0, sField = 0, i;
   unsigned char* Record;
   DBFEr = DBF_OK;
   if(dbf){
      if(dbf->Record){
         // search Field
         oField++; // feed sign symbol
         for(i = 0;i < (signed)dbf->NumFields;i++){
            LPDBFField defr;
            defr = dbf->fields + i;
            sField = defr->fSize;
            if(strcmp(FieldName, defr->fName) == 0){
               break;
            }/* if */
            oField += sField;
         }/* for */
         if(i >= (signed)dbf->NumFields){
            DBFEr = DBF_ERROR_UNKNOWN_FIELD;
            return NULL;
         }/* if */
         memset(FieldValue, 0, sizeof(FieldValue));
         Record = dbf->Record;

         {// юЄЁхчрЄ№ їтюёЄют√х яЁюсхы√
            int s = oField + sField - 1;
            while(Record[s] == ' ' && s > oField){
               s--;
               sField--;
            }/* while */
         }

         {// юЄЁхчрЄ№ ышфшЁє■∙шх яЁюсхы√
            int s = oField;
            while(Record[s] == ' ' && sField > 0){
               s++;
               sField--;
               oField++;
            }/* while */
         }

         for(i = 0;i < sField;i++){
            FieldValue[i] = Record[oField + i];
         }/* for */
      }/* if */
#ifdef DEBUG
      printf("\n--- DataBase: GetFieldAsString ---");
#endif
      return FieldValue;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return NULL;
}/* DBFGetFieldAsString */
/* ----------------------------------------------------------------------- */
int DBFGetFieldLength(LPDBF dbf, unsigned char* FieldName){
   int sField = 0, i;
   unsigned char* Record;
   DBFEr = DBF_OK;
   if(dbf){
      if(dbf->Record){
         // search Field
         for(i = 0;i < (signed)dbf->NumFields;i++){
            LPDBFField defr;
            defr = dbf->fields + i;
            sField = defr->fSize;
            if(strcmp(FieldName, defr->fName) == 0){
               break;
            }/* if */
         }/* for */
         if(i >= (signed)dbf->NumFields){
            DBFEr = DBF_ERROR_UNKNOWN_FIELD;
            return -1;
         }/* if */
      }/* if */
#ifdef DEBUG
      printf("\n--- DataBase: GetFieldLength ---");
#endif
      return sField;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return -1;
}/* DBFGetFieldLength */
/* ----------------------------------------------------------------------- */
unsigned char* DBFAppendRecord(LPDBF dbf){
   DBFEr = DBF_OK;
   if(dbf){
      if(dbf->Record){

         DBFEnd(dbf);
         if(DBFEr != DBF_OK){
            return NULL;
         }/* if */

         dbf->Record[0] = ' ';

         if((signed)fwrite(dbf->Record, 1, dbf->header.SRecord, dbf->fdbf) != dbf->header.SRecord){
            DBFEr = DBF_ERROR_FILE_WRITE;
            return NULL;
         }/* if */

         if(fwrite(&EndData, 1, sizeof(EndData), dbf->fdbf) != sizeof(EndData)){
            DBFEr = DBF_ERROR_FILE_WRITE;
            return NULL;
         }/* if */

         dbf->header.NRecs++;
         if(fseek(dbf->fdbf, 0, SEEK_SET) != 0){
            DBFEr = DBF_ERROR_FILE_SEEK;
            return NULL;
         }/* if */
         if(fwrite(&(dbf->header), 1, sizeof(dbf->header), dbf->fdbf) != sizeof(dbf->header)){
            DBFEr = DBF_ERROR_FILE_WRITE;
            return NULL;
         }/* if */

         fflush(dbf->fdbf);

         DBFEnd(dbf);
         if(DBFEr != DBF_OK){
            return NULL;
         }/* if */

         if(fseek(dbf->fdbf, -(dbf->header.SRecord + 1), SEEK_CUR) != 0){
            DBFEr = DBF_ERROR_FILE_SEEK;
            return NULL;
         }/* if */
      }/* if */
#ifdef DEBUG
      printf("\n--- DataBase: Append record ---");
#endif
      return dbf->Record;
   }/* if */
   DBFEr = DBF_ERROR_NO_DATABASE;
   return NULL;
}/* DBFAppendRecord */
/* ----------------------------------------------------------------------- */
unsigned char* DBFGetErrorString(void){
   return DBFErrorString[DBFEr];
}/* DBFGetErrorString */
/* ----------------------------------------------------------------------- */
DBFError DBFGetErrorNum(void){
   return DBFEr;
}/* DBFGetErrorNum */
/* ----------------------------------------------------------------------- */

void InfoMsg(char*);

int DBFMsgError(int nAlias){
   char s[16 * 3];
   sprintf(s, "%3.3u.dbf ", nAlias);
   switch(DBFGetErrorNum()){
      case DBF_ERROR_NO_DATABASE:
         strcat(s, ":нет базы данных");
         break;
      case DBF_ERROR_EMPTY_DATABASE:
         strcat(s, ":база данных пустая");
         break;
      case DBF_ERROR_UNKNOWN_FIELD:
         strcat(s, ":неизвестное ключевое поле");
         break;
      case DBF_ERROR_MEMORY_ALLOCATE:
         strcat(s, ":ошибка при распределении памяти");
         break;
      case DBF_ERROR_DATABASE_STRUCTURE:
         strcat(s, ":ошибка структуры DBF файла");
         break;
      default:
         return -1;
   }/* switch */
   MessageBox(
#ifdef __WIN32__
   HWND_DESKTOP, s, "Ошибка работы с DBF", MB_OK
#else
   "Ошибка работы с DBF", s
#endif
   );
   return 0;
}/* DBFMsgError */
/* ----------------------------------------------------------------------- */
