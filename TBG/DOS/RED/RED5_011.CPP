
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <graphics.h>
#include <keyboard.h>
#include "red5.h"

//-------------------------------------------------------------------------
void BaseGraphElement::Move (void)//перемещение элемента попавшего в рамку
{
 long tmp;

      tmp= (long)(get_oldx()-rx_min) + (long)mmx(x_mouse);
      if(tmp > 32760L) put_x(32765);
        else           put_x((int)tmp);
      tmp= (long)(get_oldy()-ry_min) + (long)mmy(y_mouse);
      if(tmp > 32760L) put_y(32765);
        else           put_y((int)tmp);
}// Move
//-------------------------------------------------------------------------
void BaseGraphElement::Mash (void)//масштабирование элемента попавшего в рамку
{
 long tmp;
 int xc,yc,dxc,dyc;

      xc = (rx_max - rx_min)/2 + rx_min;
      yc = (ry_max - ry_min)/2 + ry_min;
      dxc= xc - get_oldx();
      dyc= yc - get_oldy();

      if(get_tip()==TIP_txt){
         tmp=(long)((double)get_oldmltx()*mx);
         if(tmp> 255) tmp=255L;
         put_mltx((unsigned char)tmp);

         tmp=(long)((double)get_oldmlty()*mx);
         if(tmp> 255) tmp=255L;
         put_mlty((unsigned char)tmp);

      }// if


      tmp=(long)((double)xc - (double)dxc*mx);
      if(tmp <=0L    ) put_x(0);
      if(tmp > 32760L) put_x(32765);
        else           put_x((int)tmp);

      tmp=(long)((double)yc - (double)dyc*mx);
      if(tmp <=0L    ) put_y(0);
      if(tmp > 32760L) put_y(32765);
        else           put_y((int)tmp);

      tmp=(long)((double)get_olddx()*mx);
      if(labs(tmp) > 32760L) {if(tmp < 0L) put_dx(-32765);
                                 else      put_dx( 32765);
                              }
        else           put_dx((int)tmp);

      tmp=(long)((double)get_olddy()*mx);
      if(labs(tmp) > 32760L) {if(tmp < 0L) put_dy(-32765);
                                 else      put_dy( 32765);
                              }
        else           put_dy((int)tmp);
}// Mash
//-------------------------------------------------------------------------
