// red5_022.cpp

#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <alloc.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <conio.h>
#include <graphics.h>

#include "red5.h"

//-----------------------------------------------------------------------
int LIST::CreateGE(int tip)//создание GE заданного типа
{

if(coreleft() < 30000L)
  {
    Warning("Нет памяти для создания ГЭ");
    return -1;
  }// if
  /* Корректируем порядковый номер вершины */
  if(PN==PN_TOP)
    { PN_TOP++;
      if(PN_TOP >=MAX_BGE)
	    {PN_TOP=MAX_BGE-1;Warning("CreateGE:Нет памяти для указателя ГЭ");
         return -1;
        };/* if */
	  PN=PN_TOP;
	}/* if then */
	else
	{ PN_TOP++;
      if(PN_TOP >=MAX_BGE)
	    {PN_TOP=MAX_BGE-1;Warning("CreateGE:Нет памяти для указателя ГЭ");
         return -1;
        };/* if */
	  /* сдвиг всех обьектов вправо */
	  for(int Index=PN_TOP;Index > PN;Index--)
        list[Index]=list[Index-1];
	};/* if else */


 switch(tip)
 {
  case TIP_line:
     // printf("create Line:");
		if((list[PN]=new Line)==0)
		  {
//           Warning("CreateGE:Нет  памяти для ГЭ");
		   return -1;
		  };//if
  break;
  case TIP_rect:
     // printf("create Rctangle:");
        if((list[PN]=new Rectangle)==0)
          {
//          Warning("CreateGE:Нет  памяти для ГЭ");
           return -1;
          };//if
  break;
  case TIP_bar:
     // printf("create Bar:");
        if((list[PN]=new Bar)==0)
          {
//           Warning("CreateGE:Нет  памяти для ГЭ");
           return -1;
          };//if
  break;
  case TIP_elip:
     // printf("create Ellipse:");
        if((list[PN]=new Ellipse)==0)
          {
//           Warning("CreateGE:Нет  памяти для ГЭ");
           return -1;
          };//if
  break;
  case TIP_felip:
     // printf("create FEllipse:");
        if((list[PN]=new FEllipse)==0)
          {
//           Warning("CreateGE:Нет  памяти для ГЭ");
           return -1;
          };//if
  break;
  case TIP_txt:
     // printf("create Text:");
        if((list[PN]=new Text)==0)
          {
//           Warning("CreateGE:Нет  памяти для ГЭ");
           return -1;
	  };//if
  break;
  case TIP_fld:
     // printf("create Fill:");
        if((list[PN]=new Floodfill)==0)
          {
//           Warning("CreateGE:Нет  памяти для ГЭ");
           return -1;
          };//if
  break;
  case TIP_bkgr:
     // printf("create BackGr:");
        if((list[PN]=new Background)==0)
          {
//           Warning("CreateGE:Нет  памяти для ГЭ");
           return -1;
          };//if
     break;
  case TIP_esc:
     // printf("create Macro:");
     if((list[PN] = new Macro) == NULL){
//           Warning("CreateGE:Нет  памяти для ГЭ");
        return -1;
     };//if
     break;
  case TIP_reper:
     // printf("create Reper:");
     if((list[PN] = new Reper) == NULL){
//           Warning("CreateGE:Нет  памяти для ГЭ");
        return -1;
     };//if
//     // printf("create Reper\n");
     break;
  case TIP_pipeline:
     // printf("create PipeLine:");
     if((list[PN] = new PipeLine) == NULL){
//           Warning("CreateGE:Нет  памяти для ГЭ");
        return -1;
     };//if
//     // printf("create Pipeline\n");
     break;
   default:
     // printf("Unknown type: %u ", tip);
      break;
 }// switch(tip)

 // printf("Ok! PN:%u\n", PN);

 SaveTest=ON;// признак необходимости сохранения
 return 0;

};// CreateGE
//---------------------------------------------------------------------------
