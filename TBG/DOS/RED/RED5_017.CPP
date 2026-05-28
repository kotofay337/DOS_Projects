//  red5_017.c

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <dos.h>
#include <dir.h>
#include <math.h>
#include <conio.h>

#include "red5.h"
/*
#define TIP_line      1
#define TIP_rect      2
#define TIP_bar       3
                      4
#define TIP_elip      5
#define TIP_txt       6
#define TIP_fld       7
#define TIP_bkgr      8
                      9 - 12
#define TIP_felip    13
                     14 - 18
#define TIP_esc      19
#define TIP_reper    20
#define TIP_bitmap   21
#define TIP_pipeline 22

#define max_tip TIP_pipeline
*/
//-------------------------------------------------------------------------
void DecTipSearch(void){
	pck.tip_search--;
	if(pck.tip_search <   1) pck.tip_search=TIP_pipeline;
	if(pck.tip_search ==  4) pck.tip_search=TIP_bar;
	if(pck.tip_search == 12) pck.tip_search=TIP_bkgr;
	if(pck.tip_search == 18) pck.tip_search=TIP_felip;
	if(pck.tip_search == TIP_bitmap) pck.tip_search=TIP_reper;
}
//-------------------------------------------------------------------------
void IncTipSearch(void){
	pck.tip_search++;
	if(pck.tip_search == 4)      pck.tip_search=TIP_elip;  // 5
	if(pck.tip_search == 9)      pck.tip_search=TIP_felip; // 13
	if(pck.tip_search == 14)     pck.tip_search=TIP_esc;   // 19
	if(pck.tip_search == TIP_bitmap) pck.tip_search=TIP_pipeline;  // 22
	if(pck.tip_search > max_tip) pck.tip_search=TIP_line;  // 1
}
//-------------------------------------------------------------------------
