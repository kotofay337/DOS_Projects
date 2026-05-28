// red5_005.cpp

#include <stdio.h>
#include <stdlib.h>
#include <bios.h>
#include <dos.h>
#include <dir.h>
#include <mem.h>
#include <string.h>
#include <conio.h>
#include <graphics.h>
#include "red5.h"

//---------------------------------------------------------------------------
void LIST::Korr(void)
{
 if(PN<0) {Warning("Нет элементов");return;}//список пуст

 switch(list[PN]->get_tip())
 {
  case TIP_line:
  case TIP_pipeline:
  case TIP_rect:
  case TIP_elip:
        if(pck.Mode_korr_lre[mode_korr_stl_lre] == ON) list[PN]->put_stl(pck.stl_l);
        if(pck.Mode_korr_lre[mode_korr_thi_lre] == ON) list[PN]->put_thi(pck.thi_l);
        if(pck.Mode_korr_lre[mode_korr_col_lre] == ON) list[PN]->put_col(pck.col_lt);
        if(pck.Mode_korr_lre[mode_korr_col_lre] == ON) list[PN]->put_col2(pck.col_f);
  break;
  case TIP_txt:
        if(pck.Mode_korr_t[mode_korr_stl_t] == ON) list[PN]->put_stl(pck.fnt_t);
        if(pck.Mode_korr_t[mode_korr_dir_t] == ON) list[PN]->put_dir(pck.dir_t);
        if(pck.Mode_korr_t[mode_korr_siz_t] == ON) {
          list[PN]->put_siz(pck.siz_t);
          list[PN]->put_mltx(Multx);
          list[PN]->put_mlty(Multy);
        }// if
        if(pck.Mode_korr_t[mode_korr_col_t] == ON) list[PN]->put_col(pck.col_lt);
        if(pck.Mode_korr_t[mode_korr_txt_t] == ON) {
        char *TmpPtr;
        if(*(TmpPtr=EditString(list[PN]->get_txt()))!='\0')
           list[PN]->put_txt(TmpPtr);
        }// if
  break;
  case TIP_bar:
  case TIP_fld:
        if(pck.Mode_korr_bzf[mode_korr_stl_bzf] == ON) list[PN]->put_stl(pck.stl_f);
        if(pck.Mode_korr_bzf[mode_korr_brd_bzf] == ON) list[PN]->put_brd(pck.col_lt);
  case TIP_bkgr:
        if(pck.Mode_korr_bzf[mode_korr_col_bzf] == ON) list[PN]->put_col(pck.col_f);
  break;
  case TIP_esc:
     list[PN]->put_index(pck.mc_index);
     list[PN]->put_state(pck.mc_state);
     hide_tbl();
     list[PN]->put_class(MPCLoad(pck.mc_index));
     restore_tbl();
     break;

 }// switch

 list[PN]->Show();// показать изменение

 SaveTest=ON;// признак необходимости сохранения

}// Korr
//---------------------------------------------------------------------------
