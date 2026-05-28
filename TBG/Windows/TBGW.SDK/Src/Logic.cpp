#include "tbg.hpp"


#ifdef _EDITOR_
// --- Логические элементы TBG ------------------------------------------------
// ----------------------------------------------------------------------------
void __fastcall TLogic::Save(FILE *f){
   fprintf(f, " [%u,%u", No, Ne);
   if(!id.IsEmpty()){
      fprintf(f, ",%s", id.c_str());
   }/* if */
   fprintf(f, "]");
}/* TLogic::Save */
#endif
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
bool __fastcall TLogic::Load(char *buf){
   bool ret = false;
   char sId[256];
   int scret = sscanf(buf, "[%u,%u,%s]", &No, &Ne, sId);
   switch(scret){
      case 2:
         ret = true;
         break;
      case 3:
         char *lastbr = strrchr(sId, ']');
         if(lastbr){
            *lastbr = '\0';
            id = sId;
            ret = true;
         }/* if */
         break;
   }/* switch */
   return ret;
}/* TLogic::Load */
// ----------------------------------------------------------------------------
__fastcall TLogic::TLogic(){No = Ne = 0;};
__fastcall TLogic::TLogic(unsigned short sNo, unsigned short sNe){
   No = sNo; Ne = sNe;
};
__fastcall TLogic::TLogic(char *sId){
   No = Ne = 0;
   id = sId;
};
void __fastcall TLogic::SetNo(unsigned short sNo){No = sNo;};
void __fastcall TLogic::SetNe(unsigned short sNe){Ne = sNe;};
void __fastcall TLogic::SetID(char *sid){id = sid;};
void __fastcall TLogic::SetID(AnsiString &sid){id = sid;};
unsigned short __fastcall TLogic::GetNo(void){return No;};
unsigned short __fastcall TLogic::GetNe(void){return Ne;};
AnsiString __fastcall TLogic::GetID(void){return id;};
// ----------------------------------------------------------------------------
#ifdef _EDITOR_
void __fastcall TListLogic::Save(FILE *f){
   if(!ListLE.Empty()){
      ListLE.StoreCurrent();
      ListLE.ToHead();
      fprintf(f, " -logic \"");
      do{
         TLogic *le = ListLE.GetData();
         if(le){
            le->Save(f);
         }/* if */
      }while(ListLE.Next());
      ListLE.ReStoreCurrent();
      fprintf(f, "\"");
   }/* if */
}/* TBaseGP::SaveLE */
// ----------------------------------------------------------------------------
void __fastcall TListLogic::DeleteLE(void){
   ListLE.DeleteFromCurrent();
}/* TBaseGP::InsertLE */
// ----------------------------------------------------------------------------
void __fastcall TListLogic::InsertID(char *sId){
   TLogic *l = new TLogic(sId);
   ListLE.AddToTail(l);
   ListLE.ToTail();
}/* TBaseGP::InsertLE */
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
void __fastcall TListLogic::DeleteAllLE(void){
   if(!ListLE.Empty()){
      do{
         delete ListLE.DeleteFromHead();
      }while(!ListLE.Empty());
   }/* if */
}/* TBaseGP::DeleteAllLE */
// ----------------------------------------------------------------------------
void __fastcall TListLogic::InsertLE(unsigned short sNo, unsigned short sNe){
   TLogic *l = new TLogic(sNo, sNe);
   ListLE.AddToTail(l);
   ListLE.ToTail();
}/* TBaseGP::InsertLE */
// ----------------------------------------------------------------------------
void __fastcall TListLogic::LoadAsBase(char *buf){
   char brkused, quoted;
   char t[32];
   int nx = 0;
   while(parser(0,t,sizeof(t) - 1, buf, whitesp, "\r",quotech,escape, &brkused, &nx, &quoted) == 0){
      TLogic *le;
      le = new TLogic;
      if(le && le->Load(t))
         ListLE.AddToTail(le);
   }// while
}/* TBaseGP::LoadLE */
// ----------------------------------------------------------------------------
void __fastcall TListLogic::Load(char *buf){
   char brkused, quoted, token[1792]; // max 128 LE
   int next = 0;

   // Read my name
   parser(0,token,sizeof(token) - 1, buf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted);

   // Read args
   while(parser(0,token,sizeof(token) - 1, buf, whitesp,breakch,quotech,escape, &brkused, &next, &quoted)==0){
      TypeKW t = SearchKeyword(token);
      switch (t){
          case kwLogic:
            parser(0,token,sizeof(token) - 1, buf, whitesp, breakch,quotech,escape, &brkused, &next, &quoted);
            LoadAsBase(token);
            break;
      }// switch
   }// while
}/* TBaseGP::LoadLE */
// ----------------------------------------------------------------------------

 