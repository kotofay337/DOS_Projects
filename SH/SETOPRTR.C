/* -----------------------------------------------------------------------
*
*  SetOperator.C
*
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
typedef
   enum{
      START,
      INPUTPERSON,
      WAITINPUTPERSON,
      NEXTFIELD,
      PRINTBC,
      WAITINFO,
      STOPPROGRAM
   }STEP;

static STEP Step = START, SaveStep;
static LPDBF db;
static int NOs, NOe, NOc;

static char *nfBC          = "òä",
            *nfCategory    = "äÄíÖÉéêàü",
            *nfsInpPerson[]={
"îàé"      // 0
};

static long BC;
/* ----------------------------------------------------------------------- */
int SetOperator(int OpKat){
   int i;

   if(Step == START){

      load_file(&p, "tbg\\timebar.tbg");
      outpic_all(p);

      Step = INPUTPERSON;
      return 0;
   }/* if */

   if(Step == INPUTPERSON){
      del_pic(&p);

      db = DBFOpen(16); // ä†‚•£Æ‡®® ØÆ´ÏßÆ¢†‚•´•©

      load_file(&p, "tbg\\setoprtr.tbg");

      getmin_obj(p, 99, (short int*)&NOs);
      getmax_obj(p, 99, (short int*)&NOe);
      NOc = NOs;

      // all field clear
      for(i = NOs;i <= NOe;i++){
         *(gettext_obj_num(p, i, 0)) = '\0';
      }/* for */

      outpic_all(p);

      Edit_Init(p,NOc,0, DBFGetFieldAsString(db, nfsInpPerson[NOc - NOs]), DBFGetFieldLength(db, nfsInpPerson[NOc - NOs]) + 1);
      Refresh_Display(p,NOc,0);
      EditLine_obj_num(p,NOc,0);

      Step = WAITINPUTPERSON;
      return 0;
   }/* if */

   if(Step == WAITINPUTPERSON){
      OutTime(p);
      switch(ExtKey){
         case 0:
            break;
         case F2:
            // all field set?
            for(i = NOs;i <= NOe;i++){
               if((getcolor_obj_num(p, i, 0) != EGA_LIGHTGREEN) || (strlen(gettext_obj_num(p, i, 0)) == 0)){
                  MessageBox("Ç¢Æ§ ‰Æ‡¨Î", "Ç¢•§®‚• ¢·• ØÆ´Ô ‰Æ‡¨Î");
                  SaveStep = Step;
                  Step = WAITINFO;
                  return 0;
               }/* if */
            }/* for */
            {
               char sBC[16];
               BC = CalcBC(17);
               sprintf(sBC, "%7.7lu", BC);
               DBFSetFieldAsString(db, nfBC, sBC);
               DBFSetFieldAsInt(db, nfCategory, OpKat);
               DBFAppendRecord(db);
               Edit_Stop(p,NOc,0);
               Step = PRINTBC;
            }
            break;
      }/* switch */
      switch(Key){
         case ESC:
            DBFClose(db);
            Step = STOPPROGRAM;
            break;
         case TAB:
            Step = NEXTFIELD;
            Edit_Stop(p,NOc,0);
            break;
         case 0:
            if(ExtKey == 0)
               break;
         default:
            switch(EditLine_obj_num(p,NOc,0)){
               case OK      :
                     DBFSetFieldAsString(db, nfsInpPerson[NOc - NOs], Edit_Stop(p,NOc,0));
                  // Mark input field
                  setcolor_obj_num(p, NOc, 0, EGA_LIGHTGREEN, SHOW);
                  Step = NEXTFIELD;
                  break;
               case CONTINUE:
                  break;
               case HELP    :
                  break;
            }//switch
            break;
      }/* switch */
      return 0;
   }/* if */

   if(Step == PRINTBC){
      char dts[16];
      Step = STOPPROGRAM;
      PrintBCAdmin(NPost, OpKat == 99 ? " ÄÑåàçàëíêÄíéê  " :
                          OpKat == 98 ? "ÄÑåàçàëíêÄíéê îê" :
                                        "   éèÖêÄíéê     "
                        , DBFGetFieldAsString(db, "îàé"), BC);
      DBFClose(db);
      return 0;
   }/* if */

   if(Step == NEXTFIELD){
      NOc++;
      if(NOc > NOe){
         NOc = NOs;
      }/* if */
      switch(NOc - NOs){
         default:
            Edit_Init(p,NOc,0, DBFGetFieldAsString(db, nfsInpPerson[NOc - NOs]), DBFGetFieldLength(db, nfsInpPerson[NOc - NOs]) + 1);
            break;
      }/* switch */
      Refresh_Display(p,NOc,0);
      EditLine_obj_num(p,NOc,0);
      Step = WAITINPUTPERSON;
      return 0;
   }/* if */


   if(Step == WAITINFO){
      OutTime(p);
      if(Key == ENTER){
         Step = SaveStep;
         MessageBox(NULL, NULL);
      }/* if */
      return 0;
   }/* if */


   if(Step == STOPPROGRAM){
      del_pic(&p);
      Step = START;
      return 1;
   }/* if */

   return -1;
}/* SetOperator */
/* ----------------------------------------------------------------------- */
