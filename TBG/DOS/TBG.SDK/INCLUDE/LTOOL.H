/* --------------------------------------------------------------------------
   LTOOL.H
   Simple LIST header file,
   implementation in LTOOL.C
   10:04:39 2-DEC-1997
   (C) Чумак А.И.
   (C) —г¬ Є Ђ.€.
-------------------------------------------------------------------------- */
/* Типовые структуры - заготовки */
typedef
   struct GENERIC_LIST {
      struct GENERIC_LIST *next;
   }GENERIC_LIST, *LPGENERIC_LIST;

typedef
   struct GENERIC_HEADER {
      LPGENERIC_LIST head;
      LPGENERIC_LIST tail;
      LPGENERIC_LIST current;
   }GENERIC_HEADER, *LPGENERIC_HEADER;
/* ----------------------------------------------------------------------- */
/* Добавить в голову списка */
int ladd_head(LPGENERIC_HEADER lph, LPGENERIC_LIST lpladd);
/* ----------------------------------------------------------------------- */
/* Добавить в хвост списка */
int ladd_tail(LPGENERIC_HEADER lph, LPGENERIC_LIST lpladd);
/* ----------------------------------------------------------------------- */
/* Удалить из головы списка */
LPGENERIC_LIST ldel_head(LPGENERIC_HEADER lph);
/* ----------------------------------------------------------------------- */
/* Удалить текущий */
LPGENERIC_LIST ldel_cur(LPGENERIC_HEADER lph);
/* ----------------------------------------------------------------------- */
/* Переместить текущий в голову */
void *ltohead(LPGENERIC_HEADER lph);
/* ----------------------------------------------------------------------- */
/* Переместить текущий в хвост */
void *ltotail(LPGENERIC_HEADER lph);
/* ----------------------------------------------------------------------- */
/* Переместить текущий вперёд  */
void *lnext(LPGENERIC_HEADER lph);
/* ----------------------------------------------------------------------- */
/* Получить указатель текущего */
void *lcur(LPGENERIC_HEADER lph);
/* ----------------------------------------------------------------------- */
/* Установить указатель текущего,
   будьте осторожны с этой функцией ! */
int lsetcur(LPGENERIC_HEADER lph, LPGENERIC_LIST lpsc);
/* ----------------------------------------------------------------------- */
/* Переместить текущий назад */
void *lprev(LPGENERIC_HEADER lph);
/* ----------------------------------------------------------------------- */
/* Найти по ключу */
void *lfinds(LPGENERIC_HEADER lph, LPGENERIC_LIST lplkey,
            int (*fcmp)(void *s1, void *s2));

/* Сортировать список */
void lsort(LPGENERIC_HEADER lph, int (*fcmp)(void *s1, void *s2));

/* Sample fcmp function

for lfinds:

 s1 == s2 ? rezult: 1
 s1 != s2 ? rezult: 0
int fcmp(LPTARGT s1, LPTARGT s2){
   if(s1->NF == s2->NF)
      return 1;
   else
      return 0;
}

for lsort:

s1  > s2 ? result:  1
s1 == s2 ? result:  0
s1  < s2 ? result: -1
int fcmp(LPTARGT s1, LPTARGT s2){
   if(s1->NF > s2->NF)
      return 1;
   if(s1->NF == s2->NF)
      return 0;
   if(s1->NF < s2->NF)
      return -1;
}

*/
/* ----------------------------------------------------------------------- */
