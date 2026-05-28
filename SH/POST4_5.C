/* -----------------------------------------------------------------------
*
*  Post4_5.C
*
*  Модуль въезда по служебному стояночному талону
*  с его оформлением.
*
*  ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <bios.h>
#include <time.h>
#include <graphics.h>
#include <ctype.h>
#include "tbg_tc.h"
#include "dbf.h"
#include "com.h"
#include "data.h"
#include "editstr.h"

/* ----------------------------------------------------------------------- */
// состояния модуля(некоторые могут не использоваться)
typedef
   enum{
      DETECTFREEPLACE,      // Определить свободное место
      CARCATEGORY,          // Подготовить ввод категории автомобиля
      WAITINPUTCARCATTEGORY,// Ожидать ввод категории автомобиля
      INPUTPERSON,          // Подготовить ввод информации о посетителе
      WAITINPUTPERSON,      // Ожидать конец ввода информации
      NEXTFIELD,            // Переход на след. поле ввода
      CALCTARIF,            // .Вычислить тариф
      MONEYGET,             // .Подготовка к получению денег
      WAITMONEYGET,         // .Ожидать подтверждение получения денег
      PRINTBC,              // Печатать ШК-талон
      SHB,                  // Подготовка к работе ШБ
      WAITSHB,              // Ожидать отработку цикла ШБ
      WAITQSL,              // Ожидать подтверждения (общее)
      WAITINFO,             // Ожидать подтверждения (информационное сообщение)
      STOPPROGRAM           // Останов с выходом из цикла.
   }STEP;

static STEP Step = DETECTFREEPLACE, // Текущий шаг цикла
            SaveStep;               // для сохранения
static LPDBF db;                    // текущая база данных
static int NOs, NOe, NOc;           // Номера объектов
                                    // начального, конечного и текущего

static char *nfPlaceNum    = "NМЕСТА", // имена полей в таблицах
            *nfUseParking  = "НАСТОЯНКЕ",
            *nfInUse       = "ЗАНЯТО",
            *nfGRN         = "ГРН",
            *nfBC          = "ШК",
            *nfS           = "С",
            *nfSTIME       = "СВРЕМЯ",
            *nfPO          = "ПО",
            *nfPOTIME      = "ПОВРЕМЯ",
            *keyInFree     = "0",      // ключи поиска по полю nfInUse
            *keyInUse      = "1",
            *keyInMark     = "2",
             PlaceNum[ 6],             // номер места
                  GRN[16],             // ГРН
            *nfsInpPerson[]={          // Поля вводимые оператором
 "ФИО",      // 0
 "NУДЛ",     // 1
 "ГРН",      // 2
 "МАРКА",    // 3
 "СВД",      // 4
 "NПОДР"     // 5
};

#define FSIZE 4  // количество форматных ограничений для любого поля
static char* Formats[][FSIZE] = {
 {Info0, ABCRus, CtrKey, NULL  }, // 0 ФИО   
 {Info3, ABCRus, DecDig, CtrKey}, // 1 NУДЛ  
 {Info3, ABCRus, DecDig, CtrKey}, // 2 грн   
 {Info2, ABCRus, ABCEng, CtrKey}, // 3 Марка 
 {Info3, ABCRus, DecDig, CtrKey}, // 4 свд   
 {Info3, DecDig, ABCRus, CtrKey}  // 5 НПодр 
};
/* ----------------------------------------------------------------------- */
// функция проверки введённого символа формату поля ввода.
//
static BOOL CheckFieldsFormat (char *f[][FSIZE], int nf, int maxnf, int ikey){
   int j;
   if(nf < 0 || nf > maxnf){
      return FALSE;
   }/* if */
   for(j = 1;j < FSIZE;j++){
      if(f[nf][j]){
         if(strchr(f[nf][j], ikey) != NULL){
            return TRUE;
         }/* if */
      }/* if */
   }/* for */
   return FALSE;
}/* CheckFieldsFormat */
/* ----------------------------------------------------------------------- */
// служ. стояночный талон
// въезд по сл. удостоверению
int Post4_5(void){
   int i;
   char DateBuf[9],        // буфера ввода даты
        BCBuf[16],         //              штрих-кода
        NumRunningBuf[8],  //              колич. въездов
        FIO[32];           //              фио
   int D, M, Y;            //              день-мес-год

   if(Step == DETECTFREEPLACE){

      // установить февраль если високосный год
      if(dt.da_year % 4 == 0){   
         NumDayInMonth[1] = 29;
      }/* if */
      else{
         NumDayInMonth[1] = 28;
      }/* elif */

      // загрузка и вывод картинки тек. времени
      load_file(&p, "tbg\\timebar.tbg");
      outpic_all(p);

      // работа с базой Стоянка
      db = DBFOpen(3); // Стоянка

      // найти свободное место
      DBFFindRecord(db, nfInUse, keyInFree);
      if(DBFGetErrorNum() != DBF_OK){
         // не найдена запись с свободным местом
         if(DBFGetErrorNum() == DBF_ERROR_RECORD_NOT_FOUND){
            MessageBox("Стоянка", "Извините, но свободных мест НЕТ!");
            DBFClose(db);
            SaveStep = Step;
            Step = WAITQSL;
            return 0;
         }/* if */
         else{
            // при любых других ошибках выход безусловный 
            DBFClose(db);
            return -1;
         }/* elif */
      }/* if */
      else{
         // запомнить номер своб. места из тек. записи
         strcpy(PlaceNum, DBFGetFieldAsString(db, nfPlaceNum));
      }/* elif */
      DBFClose(db);

      Step = INPUTPERSON;
      return 0;
   }/* if */

   // Подготовить ввод инф-ции
   if(Step == INPUTPERSON){
      del_pic(&p);

      // работа с базой Талоны4
      db = DBFOpen(13); // Служебный стояночный талон

      // загружаем картинку в память
      load_file(&p, "tbg\\person45.tbg");

      // получаем макс. и мин. номера элементов объекта(99) полей вывода.
      getmin_obj(p, 99, (short int*)&NOs);
      getmax_obj(p, 99, (short int*)&NOe);
      // текущее поле ввода: 1-е
      // -1 делается для того чтобы правильно отработал шаг NEXTFIELD
      NOc = NOs - 1;

      // очищаем все поля ввода
      for(i = NOs;i <= NOe;i++){
         // строки ввода на картинке очищаем
         *(gettext_obj_num(p, i, 0)) = '\0';
      }/* for */

      // выводим картинку
      outpic_all(p);

      // вывести номер места
      outtext_obj_num(p, 200, 0, EGA_LIGHTGRAY, PlaceNum, SHOW);

      // продолжить работу с полями ввода
      Step = NEXTFIELD;
      return 0;
   }/* if */

   // ждем ввода текущего поля
   if(Step == WAITINPUTPERSON){
      // вывести тек. время
      OutTime(p);
      // была нажата расширенная клавиша (F1 etc.)?
      switch(ExtKey){
         // не нажата
         case 0:
            break;
         case F2:// конец ввода
            // все поля заполнены ?
            for(i = NOs;i <= NOe;i++){
               // не подсвечены или ничего не введено
               if((getcolor_obj_num(p, i, 0) != EGA_LIGHTGREEN) || (strlen(gettext_obj_num(p, i, 0)) == 0)){
                  MessageBox("Ввод формы", "Введите все поля формы");
                  SaveStep = Step;
                  Step = WAITINFO;
                  return 0;
               }/* if */
            }/* for */
            // копируем ФИО для печати на ШК-талоне
            strcpy(FIO, DBFGetFieldAsString(db, nfsInpPerson[0]));
            // пишем номер занимаемого места в Талоны4
            DBFSetFieldAsString(db, nfPlaceNum, PlaceNum);
            // вычисляем тек. ШК
            BC = CalcBC(15);
            // устанавливаем в новой записи поле ШК.
            DBFSetFieldAsLong(db, nfBC, BC);
            // добавляем запись
            DBFAppendRecord(db);
            // закрываем базу
            DBFClose(db);
            // заканчиваем редактирование тек. поля
            Edit_Stop(p,NOc,0);
            // уходим на печать талона
            Step = PRINTBC;
            break;
      }/* switch */
      // была нажата клавиша ('a' etc.)
      switch(Key){
         case ESC:// отказ от ввода
            DBFClose(db);
            // стоп текущего (!!!) модуля.
            Step = STOPPROGRAM;
            break;
         case TAB:// перейти на след. поле ввода
            Step = NEXTFIELD;
            // закончить редактирование тек. поля ввода
            Edit_Stop(p,NOc,0);
            break;
         // не нажата
         case 0:// чтобы не есть время процессора EditLin-ом выйдем здесь.
            if(ExtKey == 0)
               break;
         default:// редактируем поле
            // проверить вводимые символы на допустимость
            // и выдать ИнфоМсг если ошибка
            if(CheckFieldsFormat(Formats, NOc - NOs, sizeof(nfsInpPerson) / sizeof(nfsInpPerson[0]), Key) == FALSE){
               MessageBox("Фильтр ввода", Formats[NOc - NOs][0]);
               SaveStep = Step;
               Step = WAITINFO;
               break;
            }/* if */
            // отработать нажатую клавишу
            switch(EditLine_obj_num(p,NOc,0)){
               case OK      :
                  // Enter был нажат -> поле введено
                  // установить поле в базе введенным значением как строку
                  DBFSetFieldAsString(db, nfsInpPerson[NOc - NOs], Edit_Stop(p,NOc,0));
                  // Подсветить введенное поле на картинке
                  setcolor_obj_num(p, NOc, 0, EGA_LIGHTGREEN, SHOW);
                  // инициировать след. поле
                  Step = NEXTFIELD;
                  break;
               case CONTINUE:
                  // продолжить редактирование поля
                  break;
               case HELP    :
                  // пользователь нажал F1
                  break;
            }//switch
            break;
      }/* switch */
      // продолжить работу
      return 0;
   }/* if */

   // печатаем талон
   if(Step == PRINTBC){
      // следующий шаг: отработка ШБ
      Step = SHB;
      // отпечатать талон
      PrintBC(NPost, PlaceNum, "СЛУЖЕБНЫЙ", "суток", "", FIO, BC);
      return 0;
   }/* if */

   // подготовить к циклу ШБ
   if(Step == SHB){
      // удалить тек. картинку
      del_pic(&p);
      // начинаем отработку ШБ
      Step = WAITSHB;
      return 0;
   }/* if */

   // цикл отработки ШБ
   if(Step == WAITSHB){
      switch(WorkSHB()){
         case -1: // ошибка
            // модуль в исходное
            Step = DETECTFREEPLACE;
            return -1;
         case  0: // ждем завершения цикла
            // ШБ не закончил
            break;
         case  1: // все ОК, ШБ отработал без ошибок
            // правим базу Талоны4
            db = DBFOpen(13); // база Служебный стояночный талон
            // ищем ШК в базе
            sprintf(BCBuf, "%6.6lu", BC);
            DBFFindRecord(db, nfBC, BCBuf);
            // если в этом месте ошибка то дальше можно не продолжать
            if(DBFGetErrorNum() != DBF_OK){
               DBFClose(db);
               return -1;
            }/* if */
            else{
               // запоминаем ГРН для внесения в базу стоянки
               strcpy(GRN, DBFGetFieldAsString(db, nfGRN));
               // устанавливаем дату и время въезда текущими
               DBFSetFieldAsDate(db, nfS, &dt);
               DBFSetFieldAsTime(db, nfSTIME, &tm);
               // метим что место на стоянке мы занимаем
               DBFSetFieldAsString(db, nfUseParking, keyInUse);
               // пишем запись
               DBFPutRecord(db);
               // закрываем базу
               DBFClose(db);
            }/* elif */

            // правим базу Стоянка
            db = DBFOpen(3);// Стоянка
            DBFFindRecord(db, nfPlaceNum, PlaceNum);
            if(DBFGetErrorNum() != DBF_OK){
               DBFClose(db);
               return -1;
            }/* if */
            else{
               // метим место как "занятое"
               DBFSetFieldAsString(db, nfInUse, keyInUse);
               // кем занятое (ГРН)
               DBFSetFieldAsString(db, nfGRN, GRN);
               DBFPutRecord(db);
               DBFClose(db);
            }/* elif */
            Step = DETECTFREEPLACE;
            // выходим с ОК-ем
            return 1;
      }/* switch */
      return 0;
   }/* if */


   // перейти на сл. поле редактирования и начать редактирование
   if(Step == NEXTFIELD){
      NOc++;
      if(NOc > NOe){
         NOc = NOs;
      }/* if */
      // инициировать редактирование поля
      Edit_Init(p,NOc,0, DBFGetFieldAsString(db, nfsInpPerson[NOc - NOs]), DBFGetFieldLength(db, nfsInpPerson[NOc - NOs]) + 1);
      // отрисовать активное поле ввода
      Refresh_Display(p,NOc,0);
      // начать редактирование
      EditLine_obj_num(p,NOc,0);
      // следующий ввод произойдет в ожидании нажатии клавиатуры
      Step = WAITINPUTPERSON;
      return 0;
   }/* if */

   // ждать реакции пользователя на
   // информационное сообщение
   if(Step == WAITINFO){
      // показываем время
      OutTime(p);
      // нажал ENTER
      if(Key == ENTER){
         // вспомнить на каком шаге
         Step = SaveStep;
         // удалить окно сообщения с восстановлением под ним картинки
         MessageBox(NULL, NULL);
      }/* if */
      return 0;
   }/* if */

   // ждать подтверждения и выйти
   if(Step == WAITQSL){
      // показываем время
      OutTime(p);
      // нажал ENTER
      if(Key == ENTER){
         // остановить модуль
         Step = STOPPROGRAM;
         MessageBox(NULL, NULL);
      }/* if */
      return 0;
   }/* if */

   // останов модуля
   if(Step == STOPPROGRAM){
      // удалить картинку
      del_pic(&p);
      // в исходное
      Step = DETECTFREEPLACE;
      // модуль закончил работу без ошибок
      return 1;
   }/* if */
   // выход с ошибкой если ни один IF не обработал текущее состояние STEP.
   // это возможно если переменная STEP испорчена либо нет обработчика
   // этого состояния.
   return -1;
}/* Post4_5 */
/* ----------------------------------------------------------------------- */
