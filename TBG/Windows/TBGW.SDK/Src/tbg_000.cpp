/* TBG_001.CPP
*
*   TBG v7.0
*   27-09-2001 3:33 pm msk
*
*/

#include <dir.h>
#include <math.h>
#include <vcl\Clipbrd.hpp>

#include "tbg.hpp"

#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#include "Screen.h"
#include "ToolBar.h"
#endif

#pragma hdrstop

typedef
   struct {
      char  *Name;
      int    Type;
   }NameTable;

static NameTable NameTbl[] = {
   {"TBG",         tZero},
   {"ГП",          tZero},
   {"Сглаживание", tSmooth},
   {"Размеры",     -2},
   {"SimplePoint", tPoint},
   {"ReperPoint",  tReperPoint},
   {"SimpleLine",  tLine},
   {"StyleLine",   tStyleLine},
   {"PolyLine",    tPolyLine},
   {"PipeLine",    tPipe},
   {"Polygon",     tPolygon},
   {"PolyFrameRectangle", tPolyRect},
   {"FrameRectangle",     tFrameRect},
   {"GradientFillRectangle", tGradientFillRect},
   {"FillRectangle",      tFillRect},
   {"FillPolyRectangle",  tFillPolyRect},
   {"FillArc",            tFillArc},
   {"Arc",                tArc},
   {"FillEllipse",        tFillEllipse},
   {"BGIText",            tBGIText},
   {"TrueTypeText",       tTrueTypeText},
   {"Macro",              tMacro}
};

static NameTable KeywordTbl[] = {
	{"-hide",    kwHide},
	{"-logic",   kwLogic},
	{"-coord",   kwCoords},
	{"-fgc",     kwFGColor},
	{"-bgc",     kwBGColor},
	{"-width",   kwWidth},
	{"-fs",      kwFillStyle},
	{"-sangle",  kwStartAngle},
	{"-eangle",  kwEndAngle},
	{"-angle",   kwAngle},
	{"-text",    kwText},
	{"-index",   kwIndex},
	{"-state",   kwState},
   {"-datapipe",kwDataPipe},
   {"-dataBGI", kwDataBGIText},
   {"-blur",    kwBlur},
   {"-grayscale", kwBlurToGrayscale},
   {"-filter",    kwFilter}
};


static char *sf[] = {
   "%d",	// rINT
   "%x",	// rHEX
   "%lf", // rDOUBLE
   "%hd"	// rSHORTINT
};

char *whitesp={" \t"};        /* blank and tab */
char *breakch={",\r"};        /* comma and carriage return */
char *quotech={"'\"|"};        /* single, double quote and vertical slash */
char escape='^';                /* "uparrow" is escape */

// ----------------------------------------------------------------------------
bool __fastcall ReadValue(char *s, int type, void *pv){
   bool ret = false;
   if((type >= rINT) && (type <= rSHORTINT))
      ret = sscanf(s, sf[type], pv) == 1;
   return ret;
}// Read
// ----------------------------------------------------------------------------
static TypeGP __fastcall SearchName(char *buf){
  for(int i = 0;i < sizeof(NameTbl) / sizeof(NameTbl[0]);i++){
     if(strstr(buf, NameTbl[i].Name) != NULL){
        return (TypeGP)(NameTbl[i].Type);
     }/* if */
  }/* for */
  return tSet;
}/* SearchName */
// ----------------------------------------------------------------------------
TypeKW __fastcall SearchKeyword(char *buf){
  for(int i = 0;i < sizeof(KeywordTbl) / sizeof(KeywordTbl[0]);i++){
     if(strstr(buf, KeywordTbl[i].Name) != NULL){
        return (TypeKW)(KeywordTbl[i].Type);
     }/* if */
  }/* for */
  return kwUnknown;
}/* SearchKW */
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// --- Базовый примитив -------------------------------------------------------
TBaseGP::~TBaseGP(){DeleteAllLE();};
__fastcall TBaseGP::TBaseGP(){Hide = false; Update = false;};
bool __fastcall TBaseGP::Hit(TWorkPlace*, TPoint){return false;};
#ifdef _EDITOR_
AnsiString __fastcall TBaseGP::GetName(void){return "";};
int __fastcall TBaseGP::MouseDown(TWorkPlace *wp, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y){return 0;};
int __fastcall TBaseGP::MouseMove(TWorkPlace *wp, TObject *Sender, TShiftState Shift, int X, int Y){return 0;};
// ----------------------------------------------------------------------------
bool __fastcall TBaseGP::Select(TWorkPlace* wp, TPoint p, TShiftState Shift){
   bool hit = Hit(wp, p);

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
   return IsSelect;
}// TLine::Select
// ----------------------------------------------------------------------------
bool __fastcall TBaseGP::Select(TWorkPlace* wp, TRect r, TShiftState Shift){
   toScreen(wp);
   bool isSelect = false;
   for(int i = 0;i < getSize();i++){
      if((isSelect = PtInRect(r, getps(i))) == false){
         break;
      }// if
   }// for
   if(isSelect){
      SetSelect(true);
   }/* if */
   else{
      if(!Shift.Contains(ssShift)){
         SetSelect(false);
      }/* if */
   }/* elif */
   return IsSelect;
}// TLine::Select
// ----------------------------------------------------------------------------
TPointf* __fastcall TBaseGP::HitToPoint(TPointf hp){
   for(int i = 0;i < getSize();i++)
      if(getpf(i).hit(hp, 0.006)){
         return GetCoordPtr(i);
      }// if
   return NULL;
}//
// ----------------------------------------------------------------------------
void __fastcall TBaseGP::ShowSelect(TWorkPlace *wp, SSMode m){
   TCanvas *cvs = wp->getCanvas();
   toScreen(wp);

   if((GetSelect() == true) && (m != SSSelect) && (m != SSIndicate)){
      cvs->Pen->Color = clBlack;
      cvs->Pen->Style = psSolid;
      cvs->Pen->Width = 1;
      cvs->Brush->Color = clLime;
      cvs->Brush->Style = bsSolid;
      int es = (wp->getMaxX() + wp->getMaxY()) / 2. / 300. + 1;
      for(int i = 0;i < getSize();i++)
         cvs->Ellipse(getps(i).x - es, getps(i).y - es, getps(i).x + es, getps(i).y + es);

   }/* if */

   if(m == SSSelect){
      ShowSelector(cvs, getps(0).x, getps(0).y, GetSelect());
   }/* if */

   if(m == SSIndicate){
     cvs->Pen->Mode = pmXor;
     Show(wp);
     cvs->Pen->Mode = pmCopy;
   }/* if */
};
// ----------------------------------------------------------------------------
TBaseGP* __fastcall TBaseGP::Copy(CopyGEMode m){
   return NULL;
};
// ----------------------------------------------------------------------------
void __fastcall TListLogic::CopyLE(TListLogic *c){
  if(FirstLE()){
     do{
        c->InsertLE(GetNo(), GetNe());
        c->SetID(GetID());
     }while(NextLE());
   }/* if */
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
bool __fastcall TListLogic::CheckNo(unsigned short No){
  bool ret = false;
  if(FirstLE()){
     do{
        if(GetNo() == No){
           ret = true;
           break;
        }/* if */
     }while(NextLE());
   }/* if */
   return ret;
};
// ----------------------------------------------------------------------------
bool __fastcall TListLogic::CheckNe(unsigned short No, unsigned short Ne){
  bool ret = false;
  if(FirstLE()){
     do{
        if((GetNo() == No) &&(GetNe() == Ne)){
           ret = true;
           break;
        }/* if */
     }while(NextLE());
   }/* if */
   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
AnsiString __fastcall TListLogic::GetStringAllLogic(void){
  AnsiString s = " ";
  if(FirstLE()){
     do{
        s += "[";
        s += GetNo();
        s += ", ";
        s += GetNe();
        if(!GetID().IsEmpty()){
           s += ", ";
           s += GetID();
        }// if
        s += "]";
     }while(NextLE());
   }/* if */
   return s;
};
#endif
// ----------------------------------------------------------------------------
bool __fastcall TListLogic::PrevLE(void){
  if(!ListLE.Empty())
     return ListLE.Prev();
  return false;
};
// ----------------------------------------------------------------------------
bool __fastcall TListLogic::NextLE(void){
  if(!ListLE.Empty())
     return ListLE.Next();
  return false;
};
// ----------------------------------------------------------------------------
bool __fastcall TListLogic::FirstLE(void){
  if(!ListLE.Empty())
     return ListLE.ToHead();
  return false;
};
// ----------------------------------------------------------------------------
bool __fastcall TListLogic::LastLE(void){
  if(!ListLE.Empty())
     return ListLE.ToTail();
  return false;
};
// ----------------------------------------------------------------------------
short __fastcall TListLogic::GetNo(void){
  if(!ListLE.Empty())
     return ListLE.GetData()->GetNo();
  return -1;
};
// ----------------------------------------------------------------------------
void __fastcall TListLogic::SetNo(int sNo){
  if(!ListLE.Empty())
     ListLE.GetData()->SetNo((unsigned short)sNo);
};/*  */
// ----------------------------------------------------------------------------
short __fastcall TListLogic::GetNe(void){
  if(!ListLE.Empty())
     return ListLE.GetData()->GetNe();
  return -1;
};
// ----------------------------------------------------------------------------
void __fastcall TListLogic::SetNe(int sNe){
  if(!ListLE.Empty())
     ListLE.GetData()->SetNe((unsigned short)sNe);
};/*  */
// ----------------------------------------------------------------------------
AnsiString __fastcall TListLogic::GetID(void){
  if(!ListLE.Empty())
     return ListLE.GetData()->GetID();
  return "";
};
// ----------------------------------------------------------------------------
void __fastcall TListLogic::SetID(AnsiString sId){
  if(!ListLE.Empty())
     ListLE.GetData()->SetID(sId);
};
// ----------------------------------------------------------------------------
int __fastcall TListLogic::IndexCurrentLE(void){
  return ListLE.IndexCurrent();
}/* IndexCurrent */
// ----------------------------------------------------------------------------
#ifdef _EDITOR_

void __fastcall TBaseGP::SaveAsBase(FILE *f){
   if(Hide)
      fprintf(f, " -hide");
   TListLogic::Save(f);
   TCoordSpace::Save(f);
};

void __fastcall TBaseGP::Save(FILE *f){};


int  __fastcall TListLogic::GetCount(void){return ListLE.GetCount();};

#endif

bool __fastcall TBaseGP::Load(char *buf){
   bool ret = false;

   char brkused, quoted, token[9*1024];
   int next = 0;

   Hide = false;

   // Read my name
   parser(0,token,sizeof(token) - 1, buf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, buf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
         case kwHide:
            Hide = true;
            break;
         case kwCoords:
            parser(0,token,sizeof(token) - 1, buf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
            ret |= TCoordSpace::LoadAsBase(token);
            break;
         case kwLogic:
            parser(0,token,sizeof(token) - 1, buf, whitesp, breakch,quotech,escape, &brkused, &next, &quoted);
            TListLogic::LoadAsBase(token);
            break;
      }// switch
   }// while
//   TListLogic::Load(buf);
//   ret |= TCoordSpace::Load(buf);
   Update = true;
   return ret;
};

 void __fastcall TBaseGP::Show(TWorkPlace*){};

 void __fastcall TBaseGP::ShowWithColor(TWorkPlace* c, TColor cl){};
 void __fastcall TBaseGP::ShowWithRotate(TWorkPlace* c, TPointf p, double a){};

 void   __fastcall TBaseGP::SetColor(TColor cl){};
 TColor __fastcall TBaseGP::GetColor(void){return clBlack;};

void __fastcall TBaseGP::SetWidthLine(unsigned w){};
unsigned __fastcall TBaseGP::GetWidthLine(void){ return 0; };

void __fastcall TBaseGP::SetFillColor(TColor cl){};
TColor __fastcall TBaseGP::GetFillColor(void){ return (TColor)0; };

void __fastcall TBaseGP::SetFillStyle(TBrushStyle cl){};
TBrushStyle __fastcall TBaseGP::GetFillStyle(void){ return (TBrushStyle)0; };

void __fastcall TBaseGP::SetPhiStart(TPointf &p){};
void __fastcall TBaseGP::SetPhiEnd(TPointf &p){};
TPointf& __fastcall TBaseGP::GetPhiStart(void){return *(TPointf*)NULL;};
TPointf& __fastcall TBaseGP::GetPhiEnd(void){return *(TPointf*)NULL;};

 void __fastcall TBaseGP::Smoothing(bool to){pSmooth = to;};
 bool __fastcall TBaseGP::Smoothable(void){return pSmooth;};

// --- Картинка TBG -----------------------------------------------------------
// ----------------------------------------------------------------------------
__fastcall TPictureTBG::TPictureTBG(void){
   OffsetX = 0;
   OffsetY = 0;
   SizeX = 640;
   SizeY = 480;
   Selected = 0;
   nCurrentGE = -1L;
   Change = false;
   Smooth = false;
};
// ----------------------------------------------------------------------------
__fastcall TPictureTBG::TPictureTBG(char *NameFile){
   OffsetX = 0;
   OffsetY = 0;
   SizeX = 640;
   SizeY = 480;
   Selected = 0;
   nCurrentGE = -1L;
   Change = false;
   Smooth = false;
   Load(NameFile);
};
// ----------------------------------------------------------------------------
TPictureTBG::~TPictureTBG(){
   DeleteAllGP();
};
// ----------------------------------------------------------------------------
void __fastcall TPictureTBG::setSizeScreen(int x, int y){
   if((wp.getMaxX() != (double)x) || (wp.getMaxY() != (double)y)){
      wp.setSizeScreen(x, y);
      wp.setUpdate(true);
   }// if
};
// ----------------------------------------------------------------------------
void __fastcall TPictureTBG::Load(char *NameFile){

   char drive[MAXDRIVE];
   char dir[MAXDIR];
   char file[MAXFILE];
   char ext[MAXEXT];
   TBaseGP *gp = NULL;

   fnsplit(NameFile, drive, dir, file, ext);
   strupr(ext);

   wp.setEditProcessing(false);
   wp.setShowHideState(false);

   if((strstr(ext, ".TBG") != 0) || (strstr(ext, ".TBL") != 0)){
      Convert(this, NameFile);
      if(!ListGP.Empty()){
         ListGP.ToTail();
      }/* if */
#ifdef _EDITOR_
      setChangeStatus(false);
#endif
   }/* if */
   else{
      if(FileExists(NameFile)){
         D(printf("\nLoad from:%s", NameFile));
         char Name[32];
         bool NewGPCreated = false;
         TStringList *pss = new TStringList;
         pss->LoadFromFile(NameFile);
         D(long ii = 0L);
         for(int i = 0;i < pss->Count;i++){
               char *buf = pss->Strings[i].c_str();
               if(strlen(buf) <= 1)
                  continue;
               switch(SearchName(buf)){
                  default:
                     if(Application->MessageBox(buf, "Строка не распознана, продолжить?", MB_OKCANCEL) == IDCANCEL){
                        goto Jmp;
                     }/* if */
                     break;
                  case tZero:
                     continue;
                  case -2:
                     sscanf(buf, "%s %d %d", Name, &SizeX, &SizeY);
                     break;
                  case tSmooth:
                     int sm;
                     sscanf(buf, "%s %d", Name, &sm);
                     wp.setEditProcessing(!sm);
                     break;
                  case tReperPoint:
                     gp = new TReperPoint;
                     NewGPCreated = true;
                     break;
                  case tLine:
                     gp = new TLine;
                     NewGPCreated = true;
                     break;
                  case tPolyLine:
                     gp = new TPolyLine;
                     NewGPCreated = true;
                     break;
                  case tPipe:
                     gp = new TPipeLine;
                     NewGPCreated = true;
                     break;
                  case tPolygon:
                     gp = new TPolygon;
                     NewGPCreated = true;
                     break;
                  case tFrameRect:
                     gp = new TRectangle;
                     NewGPCreated = true;
                     break;
                  case tPolyRect:
                     gp = new TPolyRect;
                     NewGPCreated = true;
                     break;
                  case tFillRect:
                     gp = new TFillRectangle;
                     NewGPCreated = true;
                     break;
                  case tGradientFillRect:
                     gp = new TGradientFillRectangle;
                     NewGPCreated = true;
                     break;
                  case tArc:
                     gp = new TArc;
                     NewGPCreated = true;
                     break;
                  case tFillArc:
                     gp = new TFillArc;
                     NewGPCreated = true;
                     break;
                  case tFillPolyRect:
                     gp = new TFillPolyRect;
                     NewGPCreated = true;
                     break;
                  case tBGIText:
                     gp = new TBGIText;
                     NewGPCreated = true;
                     break;
                  case tMacro:
                     gp = new TMacro;
                     NewGPCreated = true;
                     break;
               }/* switch */
               if(gp && NewGPCreated){
                  if(gp->Load(buf) == true){
                    NewGPCreated = false;

                    //TColorEx c1(gp->GetColor());
                    //gp->SetColor(c1.toGrayscale());
                    //TColorEx c2(gp->GetFillColor());
                    //gp->SetFillColor(c2.toGrayscale());

                    InsertGP(gp, ToTail);
                    D(ii++);
                  }/* if */
                  else{
                     delete gp;
                     gp = NULL;
                  }/* elif */
               }/* if */
         }/* for */
         Jmp:;
         delete pss;
         wp.setEditProcessing(false);
         if(!ListGP.Empty()){
            ListGP.ToTail();
         }/* if */
         D(printf("\nLoading:%ld GP's", ii));
#ifdef _EDITOR_
         setChangeStatus(false);
#endif
      }/* if */
   }/* elif */

#ifdef _EDITOR_
   if(!ListGP.Empty()){
      nCurrentGE = GetCount() - 1;
   }/* if */
#endif
   wp.setUpdate(true);
};
// ----------------------------------------------------------------------------
bool __fastcall TPictureTBG::IsUpdate(bool SetUpdate){
   if(SetUpdate)
      wp.setUpdate(true);
   return wp.getUpdate();
};
// ----------------------------------------------------------------------------
//static int i=0, ii=0;
void __fastcall TPictureTBG::Show(){

   if(!ListGP.Empty() && IsUpdate()){
      if(!wp.getBufCopy()){
         TCanvas *cvs = wp.getCanvas();
         cvs->Brush->Color = 0x00FDFEFF;
         cvs->Brush->Style = bsSolid;
         cvs->FillRect(Rect(0, 0, wp.getMaxX(), wp.getMaxY()));
      }// if
      ListGP.StoreCurrent();
      ListGP.ToHead();
      wp.updateGraphics();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            gp->Show(&wp);
         }// if
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();
/*
      {
         char s[] = "-\\|/";
         printf("Update:[%c]%2.2d\n", s[i], i);
         if(++i > 3)
            i = 0; 
      }
*/      
      wp.setUpdate(false);
   }/* if */

   if(!wp.getBufCopy()){
      if(TCanvas *cvs = wp.getCanvasOut()){
         cvs->Draw(0, 0, wp.getBuf());
/*
         {
            char s[] = "-\\|/";
            printf("Draw  :[%c]%2.2d\n", s[ii], ii);
            if(++ii > 3)
               ii= 0; 
         }
*/         
      }// if
   }// if
   
/*
//   TRect rr = wp.getCanvas()->ClipRect;
//   HRGN hRgn = CreateRectRgnIndirect(&rr);
//   HDC hDC = wp.getCanvas()->Handle;
//   SelectClipRgn(hDC, hRgn);

   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            gp->Show(&wp);
         }// if
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();
   }/* if */

//   SelectClipRgn(hDC, NULL);
//   DeleteObject(hRgn);
*/   
};
// ----------------------------------------------------------------------------
bool __fastcall TPictureTBG::MoveToAllGP(TPoint to, bool mode){
   bool ret = false;
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            gp->MoveTo(to.x / getMaxX(), to.y / getMaxY());
            ret = true;
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();
   }/* if */
#ifdef _EDITOR_
   setChangeStatus(ret);
#endif
   wp.setUpdate(ret);
   return ret;
}/* MoveToSelectedGP */
// ----------------------------------------------------------------------------
//
// Внимание !!!
// поиск ГЭ производится с конца списка
//
TBaseGP* __fastcall TPictureTBG::Hit(TPoint p){
   TBaseGP *ret = NULL;
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToTail();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            if(gp->Hit(&wp, p)){
               ret = gp;
               break;
            }/* if */
         }/* if */
      }while(ListGP.Prev());
      ListGP.ReStoreCurrent();
   }/* if */
   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TPictureTBG::ShowSelect(SSMode m){
   if(!ListGP.Empty()){
      if(m == SSNormal){
         // Show selected GP
         ListGP.StoreCurrent();
         ListGP.ToHead();
         do{
            TBaseGP *gp = ListGP.GetData();
            if(gp)
               gp->ShowSelect(&wp, SSNormal);
         }while(ListGP.Next());
         ListGP.ReStoreCurrent();
      }/* if */
      // Show current GP
      TBaseGP *gp = ListGP.GetData();
      if(gp)
         gp->ShowSelect(&wp, SSSelect);
      if(TCanvas *cvs = wp.getCanvasOut())
         cvs->Draw(0, 0, wp.getBuf());
   }/* if */
};
// ----------------------------------------------------------------------------
void __fastcall TPictureTBG::Save(char *NameFile){
   if(!ListGP.Empty()){
      char drive[MAXDRIVE];
      char dir[MAXDIR];
      char file[MAXFILE];
      char ext[MAXEXT];
      char NewNameFile[MAXDRIVE+MAXPATH];

      fnsplit(NameFile, drive, dir, file, ext);

      strcpy(NewNameFile, drive);
      strcat(NewNameFile, dir);
      strcat(NewNameFile, file);
      strcat(NewNameFile, ".wTBG");

      FILE *f;
      f = fopen(NewNameFile, "wb");
      if(f){
         D(printf("\nSave in:%s", NameFile));

         fprintf(f, "TBG 7.0\n");
         fprintf(f, "ГП: %lu\n", ListGP.GetCount());
         fprintf(f, "Сглаживание: %d\n", !wp.getEditProcessing());
         fprintf(f, "Размеры: %d %d\n", (int)getMaxX(), (int)getMaxY());

         ListGP.StoreCurrent();
         ListGP.ToHead();
         do{
            TBaseGP *gp = ListGP.GetData();
            if(gp){
               gp->Save(f);
               fprintf(f, "\n");
            }/* if */
         }while(ListGP.Next());
         ListGP.ReStoreCurrent();
         fclose(f);
         setChangeStatus(false);
      }/* if */
      else{
         char sm[256];
         sprintf(sm, "Ошибка открытия файла \"%s\".\nОтмена операции.", NameFile);
         Application->MessageBox(sm, "Сохранение файла", MB_OK | MB_ICONSTOP);
      }/* elif */
   }/* if */
};
// ----------------------------------------------------------------------------
//
// Внимание !!!
// поиск ГЭ производится с конца списка
//
TBaseGP* __fastcall TPictureTBG::Select(POINT p, TShiftState Shift){
   TBaseGP *ret = NULL;
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToTail();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            if(Shift.Contains(ssShift)){
               if(gp->Hit(&wp, p)){
                  gp->SetSelect(!gp->GetSelect());
               }// if
            }// if
            else{
               gp->SetSelect(false);
               if(gp->Hit(&wp, p) && (ret == NULL)){
                  ListGP.StoreCurrent();
                  gp->SetSelect(true);
                  ret = gp;
               }// if
            }// elif
         }/* if */
      }while(ListGP.Prev());
      ListGP.ReStoreCurrent();
   }/* if */
   return ret;
};
// ----------------------------------------------------------------------------
void __fastcall TPictureTBG::SelectAll(void){
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
       do{
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            gp->SetSelect(true);
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();
   }/* if */
   return ;
};
// ----------------------------------------------------------------------------
int __fastcall TPictureTBG::Select(TRect r, TShiftState Shift){
   int ret = 0;
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
       do{
         TBaseGP *gp = ListGP.GetData();
         if(gp && gp->Select(&wp, r, Shift)){
            ret++;
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();
   }/* if */
   return ret;
};
// ----------------------------------------------------------------------------
bool __fastcall TPictureTBG::MoveToSelectedGP(TPoint to, bool mode){
   bool ret = false;
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp && gp->GetSelect()){
            gp->MoveTo(to.x / getMaxX(), to.y / getMaxY());
            ret = true;
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();

      // ни один ГП не был выбран и не был обработан
      if(ret == false){
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            gp->MoveTo(to.x / getMaxX(), to.y / getMaxY());
            ret = true;
         }/* if */
      }/* if */
   }/* if */
   setChangeStatus(ret);
   wp.setUpdate(ret);
   return ret;
}/* MoveToSelectedGP */
// ----------------------------------------------------------------------------
TBaseGP* __fastcall TPictureTBG::HitToPointGP(TPointf hp){
 if(!ListGP.Empty()){
    ListGP.StoreCurrent();
    ListGP.ToTail();
    do{
       TBaseGP *gp = ListGP.GetData();
       if(gp && gp->GetSelect()){
          int numpts = gp->GetCoordCnt();
          for(int i = 0;i < numpts;i++){
             if(gp->getpf(i).hit(hp, 0.006))
                return gp;
          }/* for */
       }/* if */
    }while(ListGP.Prev());
    ListGP.ReStoreCurrent();
 }/* if */
 return NULL;
};
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool __fastcall TPictureTBG::RotateToSelectedGP(void){
   bool ret = false;
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp && gp->GetSelect()){
            TPointf c(Painter->PntCenterRotate.x / (double)wp.getMaxX(), Painter->PntCenterRotate.y / (double)wp.getMaxY());
            gp->RotateTo(c, Painter->AngleRotate);
            ret = true;
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();

      // ни один ГП не был выбран и не был обработан
      if(ret == false){
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            TPointf c(Painter->PntCenterRotate.x / (double)wp.getMaxX(), Painter->PntCenterRotate.y / (double)wp.getMaxY());
            gp->RotateTo(c, Painter->AngleRotate);
            ret = true;
         }/* if */
      }/* if */
   }/* if */
   setChangeStatus(ret);
   wp.setUpdate(ret);
   return ret;
}/* MoveToSelectedGP */
// ----------------------------------------------------------------------------
bool __fastcall TPictureTBG::CopySelectedGP(CopyGEMode m){
   bool ret = false;

   if(!ListGP.Empty()){
      LinkList<TBaseGP*> CopyList;
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp && gp->GetSelect()){
            gp->SetSelect(false);
            CopyList.AddToTail(gp->Copy(m));
            ret = true;
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();

      // ни один ГП не был выбран и не был обработан
      if(ret == false){
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            CopyList.AddToTail(gp->Copy(m));
            ret = true;
         }/* if */
      }/* if */

      if(!CopyList.Empty() && ret){
         CopyList.ToHead();
         do{
            TBaseGP *gp = CopyList.GetData();
            if(gp){
               gp->SetSelect(true);
               ListGP.AddToTail(gp);
            }/* if */
         }while(CopyList.Next());
      }/* if */
   }/* if */
   wp.setUpdate(ret);
   return ret;
}/* CopySelectedGP */
// ----------------------------------------------------------------------------
bool __fastcall TPictureTBG::CopySelectedGPIntoClipboard(CopyGEMode m){
   bool ret = false;

   ClearClipboard();

   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp && gp->GetSelect()){
            gp->SetSelect(false);
            cb.AddToTail(gp->Copy(m));
            ret = true;
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();

      // ни один ГП не был выбран и не был обработан
      if(ret == false){
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            cb.AddToTail(gp->Copy(m));
            ret = true;
         }/* if */
      }/* if */

      // Clipboard Windows
      if(!cb.Empty()){
        Graphics::TBitmap *Bitmap = new Graphics::TBitmap();
        Bitmap->Width = wp.getMaxX();
        Bitmap->Height = wp.getMaxY();
        wp.setCanvasOut(Bitmap->Canvas);
        cb.ToHead();
        do{
           TBaseGP *gp = cb.GetData();
           if(gp){
              gp->Show(&wp);
           }/* if */
        }while(cb.Next());
        Clipboard()->Assign(Bitmap);
        delete Bitmap;
      }/* if */

   }/* if */
   return ret;
}/* CopySelectedGPIntoClipboard */
// ----------------------------------------------------------------------------
bool __fastcall TPictureTBG::CopyGPFromClipboard(void){
  bool ret = false;
  if(!cb.Empty()){
     cb.ToHead();
     do{
        TBaseGP *gp = cb.GetData()->Copy(cmWithLogic);
        if(gp){
           gp->SetSelect(true);
           ListGP.AddToTail(gp);
           ret = true;
        }/* if */
     }while(cb.Next());
  }/* if */
  setChangeStatus(ret);
  wp.setUpdate(ret);
  return ret;
}/* CopyGPFromClipboard */
// ----------------------------------------------------------------------------
bool __fastcall TPictureTBG::ZoomSelectedGP(double Zk){
   bool ret = false;
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp && gp->GetSelect()){
            gp->Zoom(Zk);
            ret = true;
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();
   }/* if */
   setChangeStatus(ret);
   wp.setUpdate(ret);
   return ret;
};
// ----------------------------------------------------------------------------
bool __fastcall TPictureTBG::BringSelectedGP(Bring to){
   bool ret = false;
   LinkList<TBaseGP*> bl;

   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         Oops2:;
         TBaseGP *gp = ListGP.GetData();
         if(gp && gp->GetSelect()){
            ListGP.DeleteFromCurrent();
            ListGP.StoreCurrent();
            bl.AddToTail(gp);
            ret = true;
            if(!ListGP.Empty())
               goto Oops2;
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();

      // ни один ГП не был выбран и не был обработан
      if(ret == false){
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            ListGP.DeleteFromCurrent();
            bl.AddToTail(gp);
            ret = true;
         }/* if */
      }/* if */

      if(!bl.Empty()){
         //ListGP.StoreCurrent();
         do{
            TBaseGP *gp = bl.DeleteFromHead();
            if(gp){
               switch(to){
                  case ToFront:
                     ListGP.AddToTail(gp);
                     ListGP.ToTail();
                     break;
                  case ToBack:
                     ListGP.AddToHead(gp);
                     ListGP.ToHead();
                     break;
                  case ToStepUp:
                     ListGP.AddToCurrent(gp);
                     break;
                  case ToStepDown:
                     ListGP.Prev();
                     ListGP.Prev();
                     ListGP.AddToCurrent(gp);
                     break;
               }/* switch */
               ret = true;
            }/* if */
         }while(!bl.Empty());
         //ListGP.ReStoreCurrent();
      }/* if */
   }/* if */
   setChangeStatus(ret);
   wp.setUpdate(ret);
   return ret;
};
// ----------------------------------------------------------------------------
bool __fastcall TPictureTBG::DeleteSelectedGP(void){
   bool ret = false;
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
      Oops:;
         TBaseGP *gp = ListGP.GetData();
         if(gp && gp->GetSelect()){
            delete gp;
            ListGP.DeleteFromCurrent();
            ret = true;
            if(!ListGP.Empty())
               goto Oops;
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();
   }/* if */
   setChangeStatus(ret);
   wp.setUpdate(ret);
   return ret;
}/* DeleteSelectedGP */
// ----------------------------------------------------------------------------
bool __fastcall TPictureTBG::DeleteCurrentGP(void){
   bool ret = false;
   if(!ListGP.Empty()){
      delete ListGP.DeleteFromCurrent();
      ret = true;
   }/* if */
   setChangeStatus(ret);
   wp.setUpdate(ret);
   return ret;
}/* DeleteCurrentGP */
// ----------------------------------------------------------------------------
long __fastcall TPictureTBG::GetnCurrentGP(void){
   if(!ListGP.Empty()){
      if(nCurrentGE >= (long)GetCount()){
         nCurrentGE = GetCount() - 1;
      }/* if */
      return nCurrentGE;
   }/* if */
   else
      return -1L;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
void __fastcall TPictureTBG::InsertGP(TBaseGP *InsGP, int m){
   switch(m){
    case ToTail:
       ListGP.AddToTail(InsGP);
       ListGP.ToTail();
       break;
    case ToHead:
       ListGP.AddToHead(InsGP);
       ListGP.ToHead();
       break;
    case ToCurrent:
       ListGP.AddToCurrent(InsGP);
       break;
   }/* switch */
#ifdef _EDITOR_
   setChangeStatus(true);
#endif
   wp.setUpdate(true);
};
// ----------------------------------------------------------------------------

TBaseGP* __fastcall TPictureTBG::FindFirstNo(unsigned short No){
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP* gp = ListGP.GetData();
         if(gp && gp->CheckNo(No)){
            return gp;
         }/* if */
      }while(ListGP.Next());
   }/* if */
   return NULL;
};
// ----------------------------------------------------------------------------

TBaseGP* __fastcall TPictureTBG::FindNextNo(unsigned short No){
   if(!ListGP.Empty()){
      if(ListGP.Next()){
         do{
            TBaseGP* gp = ListGP.GetData();
            if(gp && gp->CheckNo(No)){
               return gp;
            }/* if */
         }while(ListGP.Next());
      }/* if */
      ListGP.ReStoreCurrent();
   }/* if */
   return NULL;
};
// ----------------------------------------------------------------------------

TBaseGP* __fastcall TPictureTBG::FindFirstNe(unsigned short No, unsigned short Ne){
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP* gp = ListGP.GetData();
         if(gp && gp->CheckNe(No, Ne)){
            return gp;
         }/* if */
      }while(ListGP.Next());
   }/* if */
   return NULL;
};
// ----------------------------------------------------------------------------

TBaseGP* __fastcall TPictureTBG::FindNextNe(unsigned short No, unsigned short Ne){
   if(!ListGP.Empty()){
      if(ListGP.Next()){
         do{
            TBaseGP* gp = ListGP.GetData();
            if(gp && gp->CheckNe(No, Ne)){
               return gp;
            }/* if */
         }while(ListGP.Next());
      }/* if */
      ListGP.ReStoreCurrent();
   }/* if */
   return NULL;
};
// ----------------------------------------------------------------------------
void __fastcall TPictureTBG::DeleteAllGP(void){
   if(!ListGP.Empty()){
      do{
         delete ListGP.DeleteFromHead();
      }while(!ListGP.Empty());
#ifdef _EDITOR_
      setChangeStatus(false);
#endif
   }/* if */
};
// ----------------------------------------------------------------------------
void __fastcall TPictureTBG::SetWorkPlace(TWorkPlace *wps){
   wp = *wps;
};
// ----------------------------------------------------------------------------
void __fastcall TPictureTBG::setOffset(TPoint po){
   if(!ListGP.Empty()){
      bool u = false;
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp && gp->setOffset(TPointf(po.x / (double)wp.getMaxX(), po.y / (double)wp.getMaxY()))){
            u = true;
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();
      wp.setUpdate(u);
   }/* if */
};
// ----------------------------------------------------------------------------
void __fastcall TPictureTBG::setOffset(TPointf po){
   if(!ListGP.Empty()){
      bool u = false;
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp && gp->setOffset(po)){
            u = true;
         }/* if */
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();
      wp.setUpdate(u);
   }/* if */
};
// ----------------------------------------------------------------------------
void __fastcall TPictureTBG::Smoothing(bool to){
   if(!ListGP.Empty()){
      ListGP.StoreCurrent();
      ListGP.ToHead();
      do{
         TBaseGP *gp = ListGP.GetData();
         if(gp){
            gp->Smoothing(to);
         }// if
      }while(ListGP.Next());
      ListGP.ReStoreCurrent();
      Smooth = to;
      wp.setUpdate(true);
   }/* if */
};
// ----------------------------------------------------------------------------
 int __fastcall TPictureTBG::load_file (char *fn)  {Load(fn); return 0;};
 int __fastcall TPictureTBG::del_pic   (void)      {DeleteAllGP(); return 0;};
 int __fastcall TPictureTBG::outpic_all(void)      {Show(); return 0;};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::outpic_obj(unsigned short numobj){
  bool ret = false;
  TBaseGP *gp = FindFirstNo(numobj);
  if(gp)
     ret = true;
  while(gp){
     gp->Show(&wp);
     gp = FindNextNo(numobj);
  }/* while */
  wp.setUpdate(ret);
  return ret;
};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::outpic_obj_num(unsigned short numobj, unsigned short num){
  bool ret = false;
  TBaseGP *gp = FindFirstNe(numobj, num);
  if(gp)
     ret = true;
  while(gp){
     gp->Show(&wp);
     gp = FindNextNe(numobj, num);
  }/* while */
  wp.setUpdate(ret);
  return ret;
};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::outpic_obj_color(unsigned short numobj, TColor color){
  bool ret = false;
  TBaseGP *gp = FindFirstNo(numobj);
  if(gp)
     ret = true;
  while(gp){
     gp->ShowWithColor(&wp, color);
     gp = FindNextNo(numobj);
  }/* while */
  wp.setUpdate(ret);
  return ret;
};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::outpic_obj_num_color(unsigned short numobj, unsigned short num, TColor color){
  bool ret = false;
  TBaseGP *gp = FindFirstNe(numobj, num);
  if(gp)
     ret = true;
  while(gp){
     gp->ShowWithColor(&wp, color);
     gp = FindNextNe(numobj, num);
  }/* while */
  wp.setUpdate(ret);
  return ret;
};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::setcolor_obj(unsigned short numobj, TColor color, TypeShow show){
  bool ret = false;
  TBaseGP *gp = FindFirstNo(numobj);
  if(gp)
     ret = true;
  while(gp){
     gp->SetColor(color);
     if(show == ::Show)
        gp->Show(&wp);
     gp = FindNextNo(numobj);
  }/* while */
  wp.setUpdate(ret);
  return ret;
};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::setcolor_obj_num(unsigned short numobj, unsigned short num, TColor color, TypeShow show){
  bool ret = false;
  TBaseGP *gp = FindFirstNe(numobj, num);
  if(gp)
     ret = true;
  while(gp){
     gp->SetColor(color);
     if(show == ::Show)
        gp->Show(&wp);
     gp = FindNextNe(numobj, num);
  }/* while */
  wp.setUpdate(ret);
  return ret;
};
// ----------------------------------------------------------------------------
TColor   __fastcall TPictureTBG::getcolor_obj_num(unsigned short numobj, unsigned short num){
  TColor ret = (TColor)-1;
  TBaseGP *gp = FindFirstNe(numobj, num);
  if(gp){
     ret = gp->GetColor();
     return gp->GetColor();
  }/* if */
  return ret;
};
// ----------------------------------------------------------------------------
TColor   __fastcall TPictureTBG::getcolorset_obj_num(unsigned short numobj, unsigned short num){return (TColor)0;};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::getsize_obj_num(unsigned short numobj, unsigned short num, TRect &r){return false;};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::getsize_obj_num_reset(void){return false;};
// ----------------------------------------------------------------------------
TypeGP   __fastcall TPictureTBG::gettip_obj_num(unsigned short numobj, unsigned short num){return tUnknown;};
// ----------------------------------------------------------------------------
int      __fastcall TPictureTBG::cmpcolor_obj_num(unsigned short numobj, unsigned short num, TColor color){return 0;};
// ----------------------------------------------------------------------------
int      __fastcall TPictureTBG::cmpcolorset_obj_num(unsigned short numobj, unsigned short num, TColor color){return 0;};
// ----------------------------------------------------------------------------
char*    __fastcall TPictureTBG::gettext_obj_num(unsigned short numobj, unsigned short num){return NULL;};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::outtext_obj_num(unsigned short numobj, unsigned short num, char *text, TypeShow show){return false;};
// ----------------------------------------------------------------------------
int      __fastcall TPictureTBG::strlen_obj_num(unsigned short numobj, unsigned short num){return 0;};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::getmax_obj(unsigned short numobj, unsigned *num_cou){return false;};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::getmin_obj(unsigned short numobj, unsigned *num_cou){return false;};
// ----------------------------------------------------------------------------
unsigned short __fastcall TPictureTBG::getpos_mouse_obj(unsigned short numobj, unsigned short StartNum){return 0;};
// ----------------------------------------------------------------------------
unsigned short __fastcall TPictureTBG::get_num_obj(unsigned short numobj, unsigned short num, unsigned short numobj2){return 0;};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::pren(unsigned short obj, unsigned short num, TPoint p){
  bool ret = false;
  TBaseGP *gp = FindFirstNe(obj, num);
  while(gp){
     if(gp->Hit(&wp, p)){
        ret = true;
        break;
     }// if
     gp = FindNextNe(obj, num);
  }/* while */
  return ret;
};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::check_obj_num(unsigned short numobj, unsigned short num){
   return FindFirstNe(numobj, num) != NULL;
};
// ----------------------------------------------------------------------------
bool     __fastcall TPictureTBG::outpic_obj_num_with_rotate(unsigned short numobj, unsigned short num,double a){
  bool ret = false;
  TReperPoint *rp = (TReperPoint*)FindFirstNe(numobj, 0);
  if(rp){
     TBaseGP *gp = FindFirstNe(numobj, num);
     TPointf cp = rp->getpf(0);
     while(gp){
        if(gp->GetType() == tLine){
           TLine *gpl = (TLine*)gp;
           gpl->ShowWithRotate(&wp, cp, a);
           ret = true;
        }// if
        gp = FindNextNe(numobj, num);
     }/* while */
  }// if
  wp.setUpdate(ret);
  return ret;
};
// ----------------------------------------------------------------------------
int __fastcall TPictureTBG::setstate_obj(unsigned short numobj,unsigned int state, int show){
  bool ret = false;
  TBaseGP *gp = FindFirstNo(numobj);
  if(gp)
     ret = true;
  while(gp){
     if(gp->GetType() == tMacro){
        TMacro *gpm = (TMacro*)gp;
        gpm->SetState(state);
        if(show == ::Show){
           gpm->Show(&wp);
        }// if
     }// if
     gp = FindNextNo(numobj);
  }/* while */
  wp.setUpdate(ret);
  return ret;
};
// ----------------------------------------------------------------------------
int __fastcall TPictureTBG::setstate_obj_num(unsigned short numobj,unsigned short num,unsigned int state, int show){
  bool ret = false;
  TBaseGP *gp = FindFirstNe(numobj, num);
  if(gp)
     ret = true;
  while(gp){
     if(gp->GetType() == tMacro){
        TMacro *gpm = (TMacro*)gp;
        gpm->SetState(state);
        if(show == ::Show){
           gpm->Show(&wp);
        }// if
     }// if
     gp = FindNextNe(numobj, num);
  }/* while */
  wp.setUpdate(ret);
  return ret;
};
// ----------------------------------------------------------------------------
unsigned int __fastcall TPictureTBG::getstate_obj_num(unsigned short numobj,unsigned short num){
  TBaseGP *gp = FindFirstNe(numobj, num);
  if(gp){
     if(gp->GetType() == tMacro){
        TMacro *gpm = (TMacro*)gp;
        return gpm->GetState();
     }// if
  }/* if */
  return 0;
};
// ----------------------------------------------------------------------------
int __fastcall TPictureTBG::setdata_obj(unsigned short numobj, void *ud){
  bool ret = false;
  TBaseGP *gp = FindFirstNo(numobj);
  while(gp){
     if(gp->GetType() == tMacro){
        TMacro *gpm = (TMacro*)gp;
        gpm->SetUserData(ud);
        ret = true;
     }// if
     gp = FindNextNo(numobj);
  }/* while */
  return ret;
};
// ----------------------------------------------------------------------------
int __fastcall TPictureTBG::setdata_obj_num(unsigned short numobj,unsigned short num, void *ud){
  bool ret = false;
  TBaseGP *gp = FindFirstNe(numobj, num);
  if(gp){
     if(gp->GetType() == tMacro){
        TMacro *gpm = (TMacro*)gp;
        gpm->SetUserData(ud);
        ret = true;
     }// if
  }/* if */
  return ret;
};
// ----------------------------------------------------------------------------
void* __fastcall TPictureTBG::getdata_obj_num(unsigned short numobj,unsigned char num){
  TBaseGP *gp = FindFirstNe(numobj, num);
  if(gp){
     if(gp->GetType() == tMacro){
        TMacro *gpm = (TMacro*)gp;
        return gpm->GetUserData();
     }// if
  }/* if */
  return NULL;
};
// ----------------------------------------------------------------------------
void __fastcall TPictureTBG::savestate(void){
};
// ----------------------------------------------------------------------------




