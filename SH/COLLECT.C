/* -----------------------------------------------------------------------
*
*  COLLECT.C
*
*  Collect write & read data
*  Working with hardware
*  ----------------------------------------------------------------------- */
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <bios.h>
#include <time.h>
#include "data.h"

#define DEBUG

/* ----------------------------------------------------------------------- */
typedef
   union{
     struct{
      unsigned RS:1; // RShift
      unsigned LS:1; // LShift
      unsigned CT:1; // Ctrl
      unsigned AT:1; // Alt
      unsigned SL:1; // Scroll Lock
      unsigned NL:1; // Num Lock
      unsigned CL:1; // Caps Lock
      unsigned b7:1; // ?
     }bf;
     unsigned char byte;
   }KS;
/* ----------------------------------------------------------------------- */
enum{
   _UNKNOWN,
   _CUP,
   _WUP,
   _UP,
   _CDN,
   _WDN,
   _DN,
   _IR1ON,
   _IR1OFF,
   _IR2ON,
   _IR2OFF,
   _IR12ON,
   _IR12OFF,
   _STOP
};
static int SHB = _DN, IR1 = _IR1OFF, IR2 = _IR2OFF;
static long btime;
/* ----------------------------------------------------------------------- */
unsigned ReadBit(EQ *eq){
   ER erRead;
   if(eq->nP == 0){
      return 0xFFFF;
   }/* if */
   erRead.word = inport(eq->nP);
   return eq->Bit ==  0 ? erRead.bf.b0 :
          eq->Bit ==  1 ? erRead.bf.b1 :
          eq->Bit ==  2 ? erRead.bf.b2 :
          eq->Bit ==  3 ? erRead.bf.b3 :
          eq->Bit ==  4 ? erRead.bf.b4 :
          eq->Bit ==  5 ? erRead.bf.b5 :
          eq->Bit ==  6 ? erRead.bf.b6 :
          eq->Bit ==  7 ? erRead.bf.b7 :
          eq->Bit ==  8 ? erRead.bf.b8 :
          eq->Bit ==  9 ? erRead.bf.b9 :
          eq->Bit == 10 ? erRead.bf.b10 :
          eq->Bit == 11 ? erRead.bf.b11 :
          eq->Bit == 12 ? erRead.bf.b12 :
          eq->Bit == 13 ? erRead.bf.b13 :
          eq->Bit == 14 ? erRead.bf.b14 :
          eq->Bit == 15 ? erRead.bf.b15 : 0xFFFF;
}/* ReadBit */
/* ----------------------------------------------------------------------- */
int WriteBit(EQ *eq, unsigned Val){
   ER erWrite;
   erWrite.word = 0;
   if(eq->nP == 0){
      return -1;
   }/* if */
   switch(eq->Bit){
      case  0: erWrite.bf.b0  = Val; break;
      case  1: erWrite.bf.b1  = Val; break;
      case  2: erWrite.bf.b2  = Val; break;
      case  3: erWrite.bf.b3  = Val; break;
      case  4: erWrite.bf.b4  = Val; break;
      case  5: erWrite.bf.b5  = Val; break;
      case  6: erWrite.bf.b6  = Val; break;
      case  7: erWrite.bf.b7  = Val; break;
      case  8: erWrite.bf.b8  = Val; break;
      case  9: erWrite.bf.b9  = Val; break;
      case 10: erWrite.bf.b10 = Val; break;
      case 11: erWrite.bf.b11 = Val; break;
      case 12: erWrite.bf.b12 = Val; break;
      case 13: erWrite.bf.b13 = Val; break;
      case 14: erWrite.bf.b14 = Val; break;
      case 15: erWrite.bf.b15 = Val; break;
      default: return -1;
   }/* switch */
   outport(eq->nP, erWrite.word);
   return Val;
}/* WriteBit */
/* ----------------------------------------------------------------------- */
void collect(void){

   int  NumRecv;
   KS ks;

   gettime(&tm);
   getdate(&dt);
   bt = biostime(0, 0L);

   Key = ExtKey = 0;
   if(kbhit()){
      if((Key = getch()) == 0){
         ExtKey = getch();
      }/* if */
   }/* if */

   if(Emulate){

      ks.byte = peek(0x0040, 0x0017); // Read KeyStat from BIOS area
      if(ks.bf.NL){
         shbS1 = ks.bf.RS == 1 ? shbsDOWN :
                 ks.bf.LS == 1 ? shbsUP   : shbsUNKNOWN;

         ir1 = ks.bf.CT ? irON : irOFF;
         ir2 = ks.bf.AT ? irON : irOFF;
      }/* if */
      else{/* autotest */
         shbS1 = shbsUNKNOWN;
         switch(SHB){
            case _UP:
               shbS1 = shbsUP; // UP
               break;
            case _DN:
               shbS1 = shbsDOWN; // DN
               break;
         }/* switch */

         switch(IR1){
            case _IR1ON:
               ir1 = irON; // IR1
               break;
            case _IR1OFF:
               ir1 = irOFF;// IR1
               break;
         }/* switch */

         switch(IR2){
            case _IR2ON:
               ir2 = irON;// IR2
               break;
            case _IR2OFF:
               ir2 = irOFF;// IR2
               break;
         }/* switch */

         if(shbC1 == shbcUP){// work to UP
            btime = bt;
            SHB = _CUP;
         }/* if */

         if(SHB == _CUP){
            if(((bt - btime) / CLK_TCK) > 9.0F){
               btime = bt;
               SHB = _UP;
            }/* if */
         }/* if */

         if(SHB == _UP && IR1 == _IR1OFF && IR2 == _IR2OFF){
            if(((bt - btime) / CLK_TCK) > .2F){
               btime = bt;
               IR1 = _IR1ON;
            }/* if */
         }/* if */

         if(SHB == _UP && IR1 == _IR1ON && IR2 == _IR2OFF){
            if(((bt - btime) / CLK_TCK) > .1F){
               btime = bt;
               IR2 = _IR2ON;
            }/* if */
         }/* if */

         if(SHB == _UP && IR1 == _IR1ON && IR2 == _IR2ON){
            if(((bt - btime) / CLK_TCK) > 1.3F){
               btime = bt;
               IR1 = _IR1OFF;
            }/* if */
         }/* if */

         if(SHB == _UP && IR1 == _IR1OFF && IR2 == _IR2ON){
            if(((bt - btime) / CLK_TCK) > .1F){
               btime = bt;
               IR2 = _IR2OFF;
            }/* if */
         }/* if */

         if(shbC1 == shbcDOWN){// DN
            btime = bt;
            SHB = _CDN;
         }/* if */

         if(SHB == _CDN){
            if(((bt - btime) / CLK_TCK) > 9.0F){
               btime = bt;
               SHB = _DN;
            }/* if */
         }/* if */

      }/* if */

   }/* if */

   else{ // Работаем с реальным железом
      if(iSHBOnOff == shbsOn){

         shbS1 = shbsUNKNOWN;
         if(ReadBit(&(cfg.SwUp)) == cfg.SwUp.Val){
            shbS1 = shbsUP;
         }/* if */
         if(ReadBit(&(cfg.SwDown)) == cfg.SwDown.Val){
            shbS1 = shbsDOWN;
         }/* if */

         switch(shbC1){
            case shbcSTOP:
               WriteBit(&(cfg.CmStop), cfg.CmStop.Val);
               break;
            case shbcUP:
               WriteBit(&(cfg.CmUp), cfg.CmUp.Val);
               break;
            case shbcDOWN:
               WriteBit(&(cfg.CmDown), cfg.CmDown.Val);
               break;
            case shbcOK:
               WriteBit(&(cfg.CmStop), !cfg.CmStop.Val);
               WriteBit(&(cfg.CmUp),   !cfg.CmUp.Val);
               WriteBit(&(cfg.CmDown), !cfg.CmDown.Val);
               break;
         }/* switch */


         ir1 = ir2 = irUNKNOWN;
         if(ReadBit(&(cfg.IR1)) == cfg.IR1.Val){
            ir1 = irON;
         }/* if */
         if(ReadBit(&(cfg.IR1)) != cfg.IR1.Val){
            ir1 = irOFF;
         }/* if */

         if(ReadBit(&(cfg.IR2)) == cfg.IR2.Val){
            ir2 = irON;
         }/* if */
         if(ReadBit(&(cfg.IR2)) != cfg.IR2.Val){
            ir2 = irOFF;
         }/* if */

      }/* if */

      if(iSHBOnOff == shbsOn){

         shbS2 = shbsUNKNOWN;
         if(ReadBit(&(cfg4.SwUp)) == cfg4.SwUp.Val){
            shbS2 = shbsUP;
         }/* if */
         if(ReadBit(&(cfg4.SwDown)) == cfg4.SwDown.Val){
            shbS2 = shbsDOWN;
         }/* if */

         switch(shbC2){
            case shbcSTOP:
               WriteBit(&(cfg4.CmStop), cfg4.CmStop.Val);
               break;
            case shbcUP:
               WriteBit(&(cfg4.CmUp), cfg4.CmUp.Val);
               break;
            case shbcDOWN:
               WriteBit(&(cfg4.CmDown), cfg4.CmDown.Val);
               break;
            case shbcOK:
               WriteBit(&(cfg4.CmStop), !cfg4.CmStop.Val);
               WriteBit(&(cfg4.CmUp),   !cfg4.CmUp.Val);
               WriteBit(&(cfg4.CmDown), !cfg4.CmDown.Val);
               break;
         }/* switch */


         ir1out = ir2out = irUNKNOWN;
         if(ReadBit(&(cfg4.IR1)) == cfg4.IR1.Val){
            ir1out = irON;
         }/* if */
         if(ReadBit(&(cfg4.IR1)) != cfg4.IR1.Val){
            ir1out = irOFF;
         }/* if */

         if(ReadBit(&(cfg4.IR2)) == cfg4.IR2.Val){
            ir2out = irON;
         }/* if */
         if(ReadBit(&(cfg4.IR2)) != cfg4.IR2.Val){
            ir2out = irOFF;
         }/* if */

      }/* if */
   }/* elif */

   return;
}/* collect */
/* ----------------------------------------------------------------------- */
