//---------------------------------------------------------------------------
#include <vcl.h>
#include <cstring.h>
#include <stdio.h>
#include <string.h>

#pragma hdrstop
//---------------------------------------------------------------------------
//AnsiString  ReadString(TFileStream *  pfs)
AnsiString  ReadString(TStream *  pfs)
{
 AnsiString str;
 char buf[500], ch;
 int i = 0;

 memset(buf,'\0',sizeof(buf));

 while( i < 500 )
  {
	 pfs->Read(&ch,sizeof(char));
    if(ch == '\0') break;
    if(ch == '\n') continue;
    buf[i++] = ch;
  }
  str = AnsiString(buf);
  return str;
} /* end  ReadString */
//---------------------------------------------------------------------------

 