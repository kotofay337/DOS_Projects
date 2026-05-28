/* TBG_010.CPP
*
*   TBG v7.0
*   13-05-2002 21:43 msk
*
*/

#include <math.h>

#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#include "Screen.h"
#include "TextBGI_unt.h"
#endif

#include "tbg.hpp"
#include "tbg_tc.h"

// --- Текст TrueType ------------------------------------------------------------------
// --------------------------------------------------------------------------------
bool TTrueTypeText::Load(char *rbuf){
   bool ret;
   char Name[32];
   double sX, sY;
   int mltx, mlty;
   ret = sscanf(rbuf, "%s %lf %lf 0x%x %d %d %d %d %d %d %d %d %d %d %lf",
                   &Name,
                   &sX,
                   &sY,
                   &Color,
                   &Direction,
                   &Size,
                   &HorJustify,
                   &VerJustify,
                   &Angle
                   ) ==  15 ? true : false;
   if(ret){
      pt.moveto(sX, sY);
      char *s = strchr(rbuf, '#') + 1;
      char *e = strrchr(rbuf, '\n');
      if(s && e){
         *e = 0;
      }/* if */
      lenStr = strlen(s) + 1;
      pStr = new char[lenStr];
      if(pStr){
         strcpy(pStr, s);
      }/* if */
   }/* if */
   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void TTrueTypeText::Save(FILE *f){
   fprintf(f, "TrueTypeText");
   fprintf(f, " %lf", pt.getx());
   fprintf(f, " %lf", pt.gety());
   fprintf(f, " 0x%x", Color);
   fprintf(f, " %d", Direction);
   fprintf(f, " %d", Size);
   fprintf(f, " %d", HorJustify);
   fprintf(f, " %d", VerJustify);
   fprintf(f, " %lf", Angle);
   fprintf(f, "#%s\n", pStr);
   SaveLE(f);
};
#endif
// ----------------------------------------------------------------------------
void TTrueTypeText::SetColor(TColor color){
   Color = color;
}
// ----------------------------------------------------------------------------
void TTrueTypeText::SetFont(int ifont){
   TrueTypeFont = ifont;
}
// ----------------------------------------------------------------------------
void TTrueTypeText::SetDirection(int dir){
   Direction = dir;
}
// ----------------------------------------------------------------------------
void TTrueTypeText::SetSize(short int size){
   Size= size;
}
// ----------------------------------------------------------------------------
void TTrueTypeText::SetAngle(double ang){
   Angle = 360. - ang;
}
// ----------------------------------------------------------------------------
void TTrueTypeText::SetJustify(short int hj, short int vj){
   HorJustify = hj;
   VerJustify = vj;
}
// ----------------------------------------------------------------------------
void TTrueTypeText::SetExt(short int sda, unsigned char mltx, unsigned char mlty){
}
// ----------------------------------------------------------------------------
void TTrueTypeText::SetSS(short int sx, short int sy){
}
// ----------------------------------------------------------------------------
void TTrueTypeText::SetText(char *t){
   if(pStr)
      delete pStr;
   lenStr = strlen(t) + 1;
   pStr = new char[lenStr];
   if(pStr)
      strcpy(pStr, t);
}
// ----------------------------------------------------------------------------
char* TTrueTypeText::GetText(void){
   return pStr;
}
// ----------------------------------------------------------------------------
void TTrueTypeText::Show(TCanvas *cvs){
    LOGFONT lf; // Windows native font structure

    cvs->Pen->Color = Color;
    cvs->Brush->Style = bsClear; // set the brush style to transparent
    ZeroMemory(&lf, sizeof(LOGFONT));

    lf.lfHeight = 20;
    lf.lfEscapement = Angle * 10.; // degrees to rotate
    lf.lfOrientation = Angle * 10.;
    lf.lfCharSet = DEFAULT_CHARSET;
    strcpy(lf.lfFaceName, "Times New Roman");

    cvs->Font->Handle = CreateFontIndirect(&lf);

    int X = pt.getx() * SizeScreenX + 0.5;
    int Y = pt.gety() * SizeScreenY + 0.5;
    cvs->TextOut(X, Y, pStr); // output the rotated font
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
bool TTrueTypeText::Hit(POINT p){
   int sX, sY, eX, eY;
   sX = pt.getx() * SizeScreenX + 0.5;
   sY = pt.gety() * SizeScreenY + 0.5;
   return false;//Cross(sX, sY, eX, eY, p.x, p.y, Width > 0 ? Width : 1) == 1 ? true : false;
}// TTrueTypeText::Hit
// ----------------------------------------------------------------------------
bool TTrueTypeText::Select(POINT p, TShiftState Shift){
   bool hit = Hit(p);

   if(Shift.Contains(ssShift)){
      if(hit){
         SetSelect(!GetSelect());
      }/* if */
   }/* if */
   else{
      if((hit) && (GetSelect() == true)){
         SetSelect(false);
      }/* if */
      else{
         SetSelect(hit);
      }/* elif */
   }/* elif */
   return GetSelect();
}// TTrueTypeText::Select
// ----------------------------------------------------------------------------
bool TTrueTypeText::Select(TRect r, TShiftState Shift){
   int sx, sy, ex, ey;
   sx = pt.getx() * SizeScreenX + 0.5;
   sy = pt.gety() * SizeScreenY + 0.5;

   if((sx >= r.left) && (sx <= r.right)  &&
      (sy >= r.top)  && (sy <= r.bottom) ){
      SetSelect(true);
   }/* if */
   else{
      if(!Shift.Contains(ssShift)){
         SetSelect(false);
      }/* if */
   }/* elif */

   return GetSelect();
}// TTrueTypeText::Select
// ----------------------------------------------------------------------------
void TTrueTypeText::ShowSelect(TCanvas *cvs, SSMode m){
   int X, Y;

   if((GetSelect() == true) && (m != SSSelect) && (m != SSIndicate)){
     X = pt.getx() * SizeScreenX + 0.5;
     Y = pt.gety() * SizeScreenY + 0.5;
     ShowCoordSelector(cvs, X, Y);
   }/* if */

   if(m == SSSelect){
      X = pt.getx() * SizeScreenX + 0.5;
      Y = pt.gety() * SizeScreenY + 0.5;
      ShowSelector(cvs, X, Y, GetSelect());
   }/* if */

   if(m == SSIndicate){
     cvs->Pen->Mode = pmXor;
     Show(cvs);
     cvs->Pen->Mode = pmCopy;
   }/* if */
}/* ShowSelect */
// ----------------------------------------------------------------------------
int TTrueTypeText::MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y){

   if(Editing()){
      switch(pc){
        case cMove:
           // переместить ГП
           if (Shift.Contains(ssLeft)){
             MoveTo(X / SizeScreenX, Y / SizeScreenY);
             D(printf("\n TrueTypeText: Start Moved from: %d, %d", X, Y));
           }/* if */
           else
              return 1;
           break;
        case cZoom:
           // увеличить ГП
           break;
        case cEdit:
           // изменить какие-то свойства ГП
           break;
      }/* switch */
   }// if

   if(Creation()){
      // ГП создаётся
      if (Shift.Contains(ssLeft)){
         if(Op == Nop){
            Op = Start;
            SetStartXY(X, Y);
            D(printf("\n TrueTypeText: Start created from: %d, %d", X, Y));
         }/* if */
         else{
            Op = Nop;
            return -1;
         }// elif
      }/* if */

      if (Shift.Contains(ssRight)){
         if(Op == Start){
            Op = Nop;
            D(printf("\n TrueTypeText: Stop created to: %d, %d", X, Y));
            return 1;
         }/* if */
      }/* if */
   }/* if */
   return 0;
};
// ----------------------------------------------------------------------------
int TTrueTypeText::MouseMove(TObject *Sender, TShiftState Shift, int X, int Y){

   if(Editing()){
      switch(pc){
        case cMove:
           if (Shift.Contains(ssLeft)){
             MoveTo(X / SizeScreenX, Y / SizeScreenY);
             D(printf("\n TrueTypeText: Continued Moved to: %d, %d", X, Y));
           }/* if */
           break;
        case cZoom:
           // увеличить ГП
           break;
        case cEdit:
           // изменить какие-то свойства ГП
           break;
      }/* switch */
   }// if

   if(Creation()){
      // ГП создаётся
      if(Op == Start){
        if (Shift.Contains(ssLeft)){
           SetStartXY(X, Y);
        }/* if */
        if (Shift.Contains(ssAlt)){
           double x1, y1, x2, y2, a;
           x1 = GetStartX();
           y1 = GetStartY();
           x2 = X / SizeScreenX;
           y2 = Y / SizeScreenY;
           if((x2 - x1)){
              a = atan((y2 - y1)/(x2 - x1)) * RAD;
              if((x2 - x1) < 0.0){
                 a = 180 + a;
              }/* if */
              dAngleText = a;
              Angle = 360. - a;
           }/* if */
        }/* if */
        D(printf("\n TrueTypeText: Continued Create to: %d, %d", X, Y));
      }/* if */
   }/* if */
   return 0;
};
#endif
// ----------------------------------------------------------------------------
TTrueTypeText::TTrueTypeText(){  
          Type = tTrueTypeText;
          SetStartXY(0.0, 0.0);
          Color = (TColor)0;
          pStr = NULL;
          lenStr = 0;
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

TTrueTypeText::~TTrueTypeText(){  
   delete pStr;
};

TTrueTypeText::TTrueTypeText(double ssX, double ssY,
      TColor sColor){
         Type = tTrueTypeText;
         SetStartXY(ssX, ssY);
         Color = sColor;
         pStr = NULL;
         lenStr = 0;
         Smoothing(false);
#ifdef _EDITOR_
         Op = Nop;
         SetSelect(false);
#endif
      };

AnsiString TTrueTypeText::GetName(void){return "текст TrueType";};

void TTrueTypeText::SetStartXY(int ssX, int ssY){
   pt.moveto(ssX / SizeScreenX, ssY / SizeScreenY);
};

void TTrueTypeText::SetStartXY(double ssX, double ssY){
   pt.moveto(ssX, ssY);
};

double TTrueTypeText::GetStartX(void){ return pt.getx(); };
double TTrueTypeText::GetStartY(void){ return pt.gety(); };

#ifdef _EDITOR_

int TTrueTypeText::GetCoordCnt(void){return 1;};
   
TPointf *TTrueTypeText::GetCoordPtr(int i){return &pt;};

void TTrueTypeText::MirrorX(double ax){
};
   
void TTrueTypeText::MirrorY(double ay){
};
   
TTrueTypeText& TTrueTypeText::operator=(TTrueTypeText& R){
   if(this != &R){
      Type = R.Type;
      pt = R.pt;
      Color = R.Color;
      SetText(R.GetText());
   }/* if */
   return *this;
};

TBaseGP* TTrueTypeText::Copy(CopyGEMode m){
   TTrueTypeText *l = new TTrueTypeText;
   *l = *this;
   switch(m){
      case cmWithLogic:
         this->CopyLE(l);
         break;
      case cmMirror:
         l->MirrorX(0.5);
         l->MirrorY(0.5);
         break;
      case cmMirrorX:
         l->MirrorX(0.5);
         break;
      case cmMirrorY:
         l->MirrorY(0.5);
         break;
   }/* switch */
   return l;
};

int TTrueTypeText::RotateTo(TPoint cp, double ang){
   Op = Start;
   TPointf c;
   c.moveto(cp.x / SizeScreenX, cp.y / SizeScreenY);
   pt.rotate(ang, c);
   Angle += ang;
   Op = Stop;
   return 0;
};

int TTrueTypeText::Zoom(double Zk){
   SetStartXY(GetStartX() * Zk, GetStartY() * Zk);
   return 0;
};
   
#endif
int TTrueTypeText::MoveTo(double toX, double toY, bool m){
//   Op = Start;
   SetStartXY(GetStartX() - toX, GetStartY() - toY);
//   Op = Stop;
   return 0;
};
void TTrueTypeText::Smoothing(bool to){
};

 