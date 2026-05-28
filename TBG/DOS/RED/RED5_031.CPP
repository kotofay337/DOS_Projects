// red5_031.cpp

#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <conio.h>
#include <graphics.h>

#include "red5.h"
//--------------------------------------------------------------------------
Logic::Logic(){NO=0;LN=0;next=NULL;};//конструктор
Logic::Logic(unsigned char LNO,unsigned char LLN,Logic *L)
	    {NO=LNO;LN=LLN;next=L;};//конструктор

void Logic::set_NO(unsigned char sno){NO=sno;};//set
void Logic::set_LN(unsigned char sln){LN=sln;};
void Logic::set_next(Logic *n){next=n;};

unsigned char Logic::get_NO(void){return NO;};
unsigned char Logic::get_LN(void){return LN;};
Logic*        Logic::get_next(void){return next;};
//--------------------------------------------------------------------------