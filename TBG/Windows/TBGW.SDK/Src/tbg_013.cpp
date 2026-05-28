/* TBG_013.CPP
*
*   TBG v7.0
*   04-03-2003 18:41 msk
*
*/

#include <math.h>
#include <string.h>

#ifdef _EDITOR_
#include "TBG_Editor.hpp"
#include "Screen.h"
#endif

#include "tbg.hpp"
#include "tbg_tc.h"
#pragma hdrstop

// ----------------------------------------------------------------------------
typedef
   struct {
      char* str;
      int id;
      int cntarg;
   }Cmd;

typedef
   struct {
      int id;
      unsigned short obj, num;
      TColor color1, color2;
      int type_show;
      double angle;
      unsigned state;
      TTreeNode *pnstate;
   }CmdData;

static Cmd Commands[] = {
   {"показать"                     , Coutpic_all          , 0},
   {"показать_объект"              , Coutpic_obj          , 1},
   {"показать_элемент"             , Coutpic_obj_num      , 2},
   {"показать_элемент_вращением"   , Coutpic_obj_num_with_rotate    , 3},
   {"установить_цвет_элемента"     , Csetcolor_obj_num    , 4},
   {"установить_цвет_объекта"      , Csetcolor_obj        , 3},
   {"установить_состояние_элемента", Csetstate_obj_num    , 3},
   {"установить_состояние_объекта" , Csetstate_obj        , 2},
   {"установить_текст_объекта"     , Couttext_obj_num     , 5},
   {"исполнить_состояние"          , CexecState           , 1},
   {"показать_элемент_цветом"      , Coutpic_obj_num_color, 3},
   {"показать_объект_цветом"       , Coutpic_obj_color    , 2},
   {"outpic_all"                   , Coutpic_all          , 0},
   {"outpic_obj"                   , Coutpic_obj          , 1},
   {"outpic_obj_num"               , Coutpic_obj_num      , 2},
   {"outpic_obj_num_with_rotate"   , Coutpic_obj_num_with_rotate    , 3},
   {"setcolor_obj_num"             , Csetcolor_obj_num    , 4},
   {"setcolor_obj"                 , Csetcolor_obj        , 3},
   {"setstate_obj_num"             , Csetstate_obj_num    , 3},
   {"setstate_obj"                 , Csetstate_obj        , 2},
   {"outtext_obj_num"              , Couttext_obj_num     , 5},
   {"execState"                    , CexecState           , 1},
   {"outpic_obj_num_color"         , Coutpic_obj_num_color, 3},
   {"outpic_obj_color"             , Coutpic_obj_color    , 2}};

TMacroTreeView *mtv;

// --- TMacro -----------------------------------------------------------------
// ----------------------------------------------------------------------------
bool __fastcall TMacro::Load(char *rbuf){
   bool ret = false;

   char brkused, quoted, token[32];
   int i = 0,next = 0;

   ret = TBaseGP::Load(rbuf);

   // Read my name
   parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
         case kwState:
            parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
            ReadValue(token, rINT, &State);
            break;
         case kwIndex:
            parser(0,token,sizeof(token) - 1, rbuf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);
            ReadValue(token, rINT, &Index);
            break;
      }// switch
   }// while

   if(Node == NULL)
      Node = mtv->SearchOnIndex(Index);
   if(Node){
      Pic = mtv->LoadPic(Node);
      SetState(State);
   }// if

   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TMacro::Save(FILE *f){
   fprintf(f, "Macro");
   fprintf(f, " -index %u", Index);
   fprintf(f, " -state %u", State);
   TBaseGP::SaveAsBase(f);
};
#endif
// ----------------------------------------------------------------------------
void __fastcall TMacro::Show(TWorkPlace *wp){
   if((GetHideState() == true) && (wp->getShowHideState() == false))
      return;

   if(Node == NULL){
      if(mtv){
         Node = mtv->SearchOnIndex(Index);
      }// if
      if(Node){
         Pic = (TPictureTBG*)(Node->Data);
      }// if
      if(Node && (Pic == NULL) && mtv){
         Pic = mtv->LoadPic(Node);
      }// if
   }// if

   if(Pic){
      Pic->SetWorkPlace(wp);
      // смещение картинки макропримитива
      // (может отсутствовать)
      TBaseGP *gp = Pic->FindFirstNe(0, 0);
      if(gp){
         // offset = myxycoord + mypicoffset - repermacroxy
         Pic->setOffset(getpf(0) + getOffset() - gp->getpf(0));
      }// if
      if(mtv){
         if(Indicate){
            switch(Indicate){
               case 2:
                  mtv->ExecuteState(IndicateOnNode, Pic);
                  break;
               case 3:
                  mtv->ExecuteState(IndicateOffNode, Pic);
                  break;
            }// switch
            Indicate = 0;
         }// if
         else{
            Pic->WorkPlace()->setEditProcessing(wp->getEditProcessing());
            mtv->ExecuteState(NodeState, Pic);
         }// elif
      }// if
   }// if
};
// ----------------------------------------------------------------------------
void __fastcall TMacro::SetState(unsigned state){
   if(Node == NULL){
      if(mtv){
         Node = mtv->SearchOnIndex(Index);
      }// if
   }// if
   if((state != 2) && (state != 3)){
      State = state;
      NodeState = mtv->SearchState(Node, State);
   }// if
   else{
      Indicate = state;
      if(IndicateOnNode == NULL)
         IndicateOnNode = mtv->SearchState(Node, 2);
      if(IndicateOffNode == NULL)
         IndicateOffNode = mtv->SearchState(Node, 3);
   }// elif
};
// ----------------------------------------------------------------------------
unsigned __fastcall TMacro::GetState(void){
   return State;
};
// ----------------------------------------------------------------------------
void __fastcall TMacro::SaveState(void){
   StateSave = State;
};
// ----------------------------------------------------------------------------
unsigned __fastcall TMacro::GetSaveState(void){
   return StateSave;
};
// ----------------------------------------------------------------------------
void __fastcall TMacro::SetUserData(void *p){
   pData = p;
};
// ----------------------------------------------------------------------------
void *__fastcall TMacro::GetUserData(void){
   return pData;
};
// ----------------------------------------------------------------------------
bool __fastcall TMacro::Hit(TWorkPlace* wp, TPoint p){
   bool ret = false;

   toScreen(wp);

   if(Node == NULL){
      if(mtv){
         Node = mtv->SearchOnIndex(Index);
      }// if
      if(Node){
         Pic = (TPictureTBG*)(Node->Data);
      }// if
      if(Node && (Pic == NULL) && mtv){
         Pic = mtv->LoadPic(Node);
      }// if
   }// if

   if(Pic){
      TBaseGP *gp      = Pic->FindFirstNe(0, 10),
      		  *gpReper = Pic->FindFirstNe(0,  0);
      if(gp && gpReper){
         // offset = myxycoord + mypicoffset - repermacroxy
         gp->setOffset(getpf(0) + getOffset() - gpReper->getpf(0));
         gp->toScreen(wp);
         ret = gp->Hit(wp, p);
      }// if
   }// if

   return ret;
};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
// ----------------------------------------------------------------------------
int __fastcall TMacro::MouseDown(TWorkPlace *wp, TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y){
   if(Creation()){
      // ГП создаётся
      if (Shift.Contains(ssLeft)){
         if(Op == Nop){
            Op = Start;
            SetStartXY(X / wp->getMaxX(), Y / wp->getMaxY());
            D(printf("\n Macro: Start created from: %d, %d", X, Y));
         }/* if */
         else{
            Op = Nop;
            return -1;
         }// elif
      }/* if */

      if (Shift.Contains(ssRight)){
         if(Op == Start){
            Op = Nop;
            D(printf("\n Macro: Stop created to: %d, %d", X, Y));
            return 1;
         }/* if */
      }/* if */
   }/* if */
   return 0;
};
// ----------------------------------------------------------------------------
int __fastcall TMacro::MouseMove(TWorkPlace *wp, TObject *Sender, TShiftState Shift, int X, int Y){
   if(Creation()){
      // ГП создаётся
      if(Op == Start){
        SetStartXY(X / wp->getMaxX(), Y / wp->getMaxY());
        D(printf("\n Macro: Continued Create to: %d, %d", X, Y));
      }/* if */
   }/* if */
   return 0;
};
#endif
// ----------------------------------------------------------------------------
__fastcall TMacro::TMacro(){
          newSize(1);
          SetType(tMacro);
          Pic = NULL;
          Node = NodeState = NULL;
          IndicateOnNode = NULL;
          IndicateOffNode = NULL;
          Index = 0;
          Indicate = 0;
          SetStartXY(0.0, 0.0);
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TMacro::TMacro(unsigned i, unsigned s, TPointf p){
          newSize(1);
          SetType(tMacro);
          Index = i;
          Node = NULL;
          if(mtv)
             Node = mtv->SearchOnIndex(Index);
          if(Node){
             NodeState = mtv->SearchState(Node, s);
             IndicateOnNode = mtv->SearchState(Node, 2);
             IndicateOffNode = mtv->SearchState(Node, 3);
             Pic = mtv->LoadPic(Node);
          }// if
          State = s;
          Indicate = 0;
          SetStartXY(p.getx(), p.gety());
          Smoothing(false);
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

__fastcall TMacro::TMacro(TMacro &s){
          //*this = s;
          Smoothing(s.Smoothable());
#ifdef _EDITOR_
          Op = Nop;
          SetSelect(false);
#endif
       };

AnsiString __fastcall TMacro::GetName(void){return "макропримитив";};

#ifdef _EDITOR_

TMacro& __fastcall TMacro::operator=(TMacro& R){
   newSize(R.getSize());
   SetType(R.GetType());
   Pic = R.Pic;
   Node = R.Node;
   NodeState = R.NodeState;
   Index = R.Index;
   State = R.State;
   Indicate = 0;
   IndicateOnNode = R.IndicateOnNode;
   IndicateOffNode = R.IndicateOffNode;
   for(int i = 0;i < getSize();i++){
      getpf(i) = R.getpf(i);
   }/* for */
   TBaseGP::Smoothing(R.Smoothable());
   return *this;
};

TBaseGP* __fastcall TMacro::Copy(CopyGEMode m){
   TMacro *l = new TMacro(*this);
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
#endif


// ----------------------------------------------------------------------------
__fastcall TMacroTreeView::TMacroTreeView(TComponent *AOwner){
   SetPathToMacro("C:\\TBG\\BIN\\");
   tv = NULL;
   TVExtern = false;
   Hash = new THashedStringList;
   if(Hash){
      int Index;
      Hash->CaseSensitive = true;
      Hash->Sorted = true;
      for(int i = 0;i < (sizeof(Commands)/sizeof(Commands[0]));i++){
         Hash->AddObject(Commands[i].str, (TObject*)i);
      }// for
   }// if
   if(AOwner){
      tv = new TTreeView(AOwner);
      if(tv){
         tv->Visible = false;
         tv->Parent = (TWinControl*)AOwner;
         TVExtern = true;
      }// if
   }// if
};
// ----------------------------------------------------------------------------
TMacroTreeView::~TMacroTreeView(void){
   if(Hash)
      delete Hash;
   if(TVExtern)
      delete tv;
};
// ----------------------------------------------------------------------------
const TMacroTreeView& __fastcall TMacroTreeView::operator=(const TMacroTreeView& r){
   if(&r != this){
      tv = r.tv;
      TVExtern = r.TVExtern;
      PathToMacro = r.PathToMacro;
   }// if
   return *this;
};
// ----------------------------------------------------------------------------
void __fastcall TMacroTreeView::SetTreeView(TTreeView* stv){
   if(stv && (tv == NULL))
      tv = stv;
};
// ----------------------------------------------------------------------------
bool __fastcall TMacroTreeView::LoadMacroTree(AnsiString FileName){
   if(tv){
      tv->LoadFromFile(PathToMacro+FileName);
   }// if
};
// ----------------------------------------------------------------------------
void __fastcall TMacroTreeView::SetPathToMacro(AnsiString Str){
   PathToMacro = Str;
};
// ----------------------------------------------------------------------------
TTreeNode* __fastcall TMacroTreeView::SearchOnIndex(unsigned Index){
   if(this && tv){
      TTreeNodes* Nodes = tv->Items;
      for(int i = 0;i < Nodes->Count;i++){
         TTreeNode *Node = Nodes->Item[i];
         if(Node->Level == 0){
            char *StringBuf = Node->Text.c_str();
            unsigned ScanIndex;
            if((sscanf(StringBuf, "Макро : %u", &ScanIndex) == 1) && ScanIndex == Index){
               return Node;
            }// if
         }// if
      }// for
   }// if
   return NULL;
};
// ----------------------------------------------------------------------------
TTreeNode* __fastcall TMacroTreeView::SearchMacroAsParent(TTreeNode* N){
   if(this && N){
      TTreeNode* Node = N;
      if(Node->Level > 0){
         for(;Node->Level > 0;){
            Node = Node->Parent;
         }// for
      }// if
      return Node;
   }// if
   return NULL;
};
// ----------------------------------------------------------------------------
unsigned __fastcall TMacroTreeView::SearchMacroAsIndex(TTreeNode* N){
   if(this && N){
      TTreeNode* Node = SearchMacroAsParent(N);
      if(Node->Level == 0){
         char *StringBuf = Node->Text.c_str();
         unsigned ScanIndex;
         if(sscanf(StringBuf, "Макро : %u", &ScanIndex) == 1){
            return ScanIndex;
         }// if
      }// if
   }// if
   return 0U;
};
// ----------------------------------------------------------------------------
TTreeNode* __fastcall TMacroTreeView::SearchStateAsParent(TTreeNode* N){
   if(this && N){
      TTreeNode* Node = N;
      if(Node->Level > 1){
         for(;Node->Level > 1;){
            Node = Node->Parent;
         }// for
      }// if
      return Node;
   }// if
   return NULL;
};
// ----------------------------------------------------------------------------
unsigned __fastcall TMacroTreeView::SearchStateAsIndex(TTreeNode* N){
   if(this && N){
      TTreeNode* Node = SearchStateAsParent(N);
      if(Node->Level == 1){
         char *StringBuf = Node->Text.c_str();
         unsigned ScanIndex;
         if(sscanf(StringBuf, "Состояние : %u", &ScanIndex) == 1){
            return ScanIndex;
         }// if
      }// if
   }// if
   return 0U;
};
// ----------------------------------------------------------------------------
TPictureTBG* __fastcall TMacroTreeView::LoadPic(TTreeNode* N){
   if(this && N && (N->Level == 0)){
      if(N->Data)
         return (TPictureTBG*)(N->Data);
      for(int i = 0;i < N->Count;i++){
         TTreeNode *Node = N->Item[i];
         if(Node->Level == 1){
            char *StringBuf = Node->Text.c_str();
            char PicName[1024], ms = '\"';
            if(sscanf(StringBuf, "Картинка : \" %s \" ", &PicName) == 1){
               strcpy(PicName, StringBuf);
               *strrchr(PicName, ms) = 0;
               char *fn = strchr (PicName, ms);
               if(fn){
                  AnsiString FileName;
                  FileName = PathToMacro;
                  FileName += fn + 1;
                  TPictureTBG* ret = new TPictureTBG(FileName.c_str());
                  if(ret){
                     N->Data = ret;
                     ret->WorkPlace()->setEditProcessing(false);
                  }// if
                  return ret;
               }// if
            }// if
         }// if
      }// for
   }// if
   return NULL;
};
// ----------------------------------------------------------------------------
TTreeNode* __fastcall TMacroTreeView::SearchState(TTreeNode* N, unsigned Index){
   if(this && N){
      for(int i = 0;i < N->Count;i++){
         TTreeNode *Node = N->Item[i];
         if(Node->Level == 1){
            char *StringBuf = Node->Text.c_str();
            unsigned ScanIndex;
            if((sscanf(StringBuf, "Состояние : %u", &ScanIndex) == 1) && ScanIndex == Index){
               return Node;
            }// if
         }// if
      }// for
   }// if
   return NULL;
};
// ----------------------------------------------------------------------------
TColor __fastcall SearchColor(char *s){
   int i;
   for(i = 0;i < 16;i++){
      if(strcmp(key_colors[i].str, s) == 0)
         break;
   }// for
   if(i < 16){
      return EGAColors[key_colors[i].id];
   }// if

   TColor c;
   if(sscanf(s, "0x%x", &c) == 1)
      return c;

   return clWhite;
};
// ----------------------------------------------------------------------------
TypeShow __fastcall SearchTypeShow(char *s){
   int i;
   for(i = 0;i < 2;i++){
      if(strcmp(key_modes[i].str, s) == 0)
         break;
   }// for
   if(i < 2){
      switch(key_modes[i].id){
         case SHOW:
            return ::Show;
         case NO_SHOW:
            return ::NoShow;
      }// switch
   }// if
   return Show;
};
// ----------------------------------------------------------------------------
void __fastcall TMacroTreeView::ExecuteState(TTreeNode* N, TPictureTBG *pic){
   if(this && N){
      for(int i = 0;i < N->Count;i++){
         TTreeNode *Node = N->Item[i];
         if(Node->Level == 2){
            int index = 0;
            if(Node->Data == NULL){
               char *StringBuf = Node->Text.c_str();
               char Command[256];
               unsigned ScanIndex;
               if(sscanf(StringBuf, "%s :", Command) == 1){
                  if(Hash){
                     int Ind;
                     if(Hash->Find(Command, Ind)){
                        index = (int)(Hash->Objects[Ind]);
                     }// if
                  }// if
                  else{
                     for(index = 0;index < 50;index++){
                        if(strcmp(Commands[index].str, Command) == 0)
                           break;
                     }// for
                     if(index >= 50){
                        index = -1;
                     }// if
                  }// elif
               }// if

               if(index >= 0){
                  int sret;
                  unsigned short obj, num;
                  unsigned tState;
                  char sColor[64], sMode[64];
                  CmdData *cd = new CmdData;
                  cd->id = Commands[index].id;
                  Node->Data = cd;

                  switch(Commands[index].id){
                     case Coutpic_all:
                        break;
                     case CexecState:
                        sret = sscanf(StringBuf, "%s : %u", Command, &tState);
                        if(sret == Commands[index].cntarg + 1){
                           //mtv->ExecuteState(mtv->SearchState(mtv->SearchMacroAsParent(N), tState), pic);
                           cd->pnstate = mtv->SearchState(mtv->SearchMacroAsParent(N), tState);
                        }// if
                        break;
                     case Coutpic_obj_num:
                        sret = sscanf(StringBuf, "%s : %hu %hu", Command, &obj, &num);
                        if(sret == Commands[index].cntarg + 1){
                           //pic->outpic_obj_num(obj, num);
                           cd->obj = obj;
                           cd->num = num;
                        }// if
                        break;
                     case Coutpic_obj_num_with_rotate:
                        double angle;
                        sret = sscanf(StringBuf, "%s : %hu %hu %lf", Command, &obj, &num, &angle);
                        if(sret == Commands[index].cntarg + 1){
                           //pic->outpic_obj_num_with_rotate(obj, num, angle);
                           cd->obj = obj;
                           cd->num = num;
                           cd->angle = angle;
                        }// if
                        break;
                     case Coutpic_obj:
                        sret = sscanf(StringBuf, "%s : %hu", Command, &obj);
                        if(sret == Commands[index].cntarg + 1){
                           //pic->outpic_obj(obj);
                           cd->obj = obj;
                        }// if
                        break;
                     case Coutpic_obj_num_color:
                        sret = sscanf(StringBuf, "%s : %hu %hu %s", Command, &obj, &num, sColor);
                        if(sret == Commands[index].cntarg + 1){
                           //pic->outpic_obj_num_color(obj, num, SearchColor(sColor));
                           cd->obj = obj;
                           cd->num = num;
                           cd->color1 = SearchColor(sColor);
                        }// if
                        break;
                     case Coutpic_obj_color:
                        sret = sscanf(StringBuf, "%s : %hu %s", Command, &obj, sColor);
                        if(sret == Commands[index].cntarg + 1){
                           //pic->outpic_obj_color(obj, SearchColor(sColor));
                           cd->obj = obj;
                           cd->color1 = SearchColor(sColor);
                        }// if
                        break;
                     case Csetcolor_obj_num:
                        sret = sscanf(StringBuf, "%s : %hu %hu %s %s", Command, &obj, &num, sColor, sMode);
                        if(sret == Commands[index].cntarg + 1){
                           //pic->setcolor_obj_num(obj, num, SearchColor(sColor), SearchTypeShow(sMode));
                           cd->obj = obj;
                           cd->num = num;
                           cd->color1 = SearchColor(sColor);
                           cd->type_show = SearchTypeShow(sMode);
                        }// if
                        break;
                     case Csetcolor_obj:
                        sret = sscanf(StringBuf, "%s : %hu %s %s", Command, &obj, sColor, sMode);
                        if(sret == Commands[index].cntarg + 1){
                           //pic->setcolor_obj(obj, SearchColor(sColor), SearchTypeShow(sMode));
                           cd->obj = obj;
                           cd->color1 = SearchColor(sColor);
                           cd->type_show = SearchTypeShow(sMode);
                        }// if
                        break;
                     case Couttext_obj_num:
                        //pic->outtext_obj_num( d->obj, d->num, d->Colorbg, d->txt, d->Mode);
                        break;
                     case Csetstate_obj_num:
                        sret = sscanf(StringBuf, "%s : %hu %hu %u", Command, &obj, &num, &tState);
                        if(sret == Commands[index].cntarg + 1){
                           //pic->setstate_obj_num(obj, num, tState, Show);
                           cd->obj = obj;
                           cd->num = num;
                           cd->state = tState;
                           cd->type_show = Show;
                        }// if
                        break;
                     case Csetstate_obj:
                        sret = sscanf(StringBuf, "%s : %hu %u", Command, &obj, &tState);
                        if(sret == Commands[index].cntarg + 1){
                           //pic->setstate_obj(obj, tState, Show);
                           cd->obj = obj;
                           cd->state = tState;
                           cd->type_show = Show;
                        }// if
                        break;
                     default:
                        Node->Data = NULL;
                        delete cd;
                        break;
                  }// switch
               }// if
            }// if

            if(Node->Data != NULL){
               CmdData *cd = (CmdData*)(Node->Data);
               switch(cd->id){
                  case Coutpic_all:
                     pic->Show();
                     break;
                  case CexecState:
                     mtv->ExecuteState(cd->pnstate, pic);
                     break;
                  case Coutpic_obj_num:
                     pic->outpic_obj_num(cd->obj, cd->num);
                  case Coutpic_obj_num_with_rotate:
                     pic->outpic_obj_num_with_rotate(cd->obj, cd->num, cd->angle);
                     break;
                  case Coutpic_obj:
                     pic->outpic_obj(cd->obj);
                     break;
                  case Coutpic_obj_num_color:
                     pic->outpic_obj_num_color(cd->obj, cd->num, cd->color1);
                     break;
                  case Coutpic_obj_color:
                     pic->outpic_obj_color(cd->obj, cd->color1);
                     break;
                  case Csetcolor_obj_num:
                     pic->setcolor_obj_num(cd->obj, cd->num, cd->color1, cd->type_show);
                     break;
                  case Csetcolor_obj:
                     pic->setcolor_obj(cd->obj, cd->color1, cd->type_show);
                     break;
                  case Couttext_obj_num:
//                           pic->outtext_obj_num( d->obj, d->num, d->Colorbg, d->txt, d->Mode);
                     break;
                  case Csetstate_obj_num:
                     pic->setstate_obj_num(cd->obj, cd->num, cd->state, Show);
                     break;
                  case Csetstate_obj:
                     pic->setstate_obj(cd->obj, cd->state, Show);
                     break;
               }// switch
            }// if
         }// if
      }// for
   }// if
};
// ----------------------------------------------------------------------------
void __fastcall TMacroTreeView::DeleteAllPic(void){
   if(this && tv){
      TTreeNodes* Nodes = tv->Items;
      for(int i = 0;i < Nodes->Count;i++){
         TTreeNode *Node = Nodes->Item[i];
         if((Node->Level == 0) && Node->Data){
            TPictureTBG* p = (TPictureTBG*)(Node->Data);
            if(p){
               delete p;
               Node->Data = NULL;
            }// if
         }// if
         if((Node->Level == 2) && Node->Data){
            CmdData* p = (CmdData*)(Node->Data);
            if(p){
               delete p;
               Node->Data = NULL;
            }// if
         }// if
      }// for
   }// if
};
// ----------------------------------------------------------------------------

