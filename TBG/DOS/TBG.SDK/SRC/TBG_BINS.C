/* tbg_bins.c ---------------------------------------------------------------
                  Ôóíêöèè èíòåğôåéñà Ñè ñ TBG-ôàéëàìè

                    Ãğàôè÷åñêèé ïàêåò "ÒóğÁîÃğàôèêà"
                      (C) Ïàâåëüåâ Ï.È. 1990-95
---------------------------------------------------------------------------*/

#include "tbg_tc.h" /* îáüÿâëåíèÿ ôóíêöèé è äàííûõ */

/*--------------------------------------------------------------------------*/
/* (C) ×óìàê À.È. 1993 */
/*--------------------------------------------------------------------------*/
short int BinSearch(unsigned short int Search,
              unsigned long *array,
              unsigned short int Size,unsigned short int Mask)
{

	register unsigned short int
		 Lim_Max=Size+1,
		 Lim_Min=0,
		 Value=0;
	register short int
		 Current=0;

	Search &= Mask;

	while(1){
		Current=(unsigned short int)((Lim_Max - Lim_Min) >> 1);
		Current+=Lim_Min;
		Value=((unsigned short int)*((unsigned long*)array+Current)) & Mask;
		if( Value == Search )
         break;
 		if( (Current-Lim_Min) == 0 )
			return -1;
		if( Value > Search )
			Lim_Max=Current;
		else
			Lim_Min=Current;
	}/*while */

  	for(;((((unsigned short int)*((unsigned long*)array+Current)) & Mask) == Search) && (Current >= 0);
		  Current--);

	return Current+1;

}/* BinSearch */
/*--------------------------------------------------------------------------*/