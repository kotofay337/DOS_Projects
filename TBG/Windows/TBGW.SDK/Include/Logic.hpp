

// Тип логический элемент
//  хранит номер объекта и номер элемента,
//  строку текстового идентификатора
class TLogic {
   unsigned short No, Ne; // Номер объекта, номер элемента
   AnsiString id;
public:
   __fastcall TLogic();
   __fastcall TLogic(unsigned short sNo, unsigned short sNe);
   __fastcall TLogic(char *sId);
   void __fastcall SetNo(unsigned short sNo);
   void __fastcall SetNe(unsigned short sNe);
   void __fastcall SetID(char *sid);
   void __fastcall SetID(AnsiString &sid);
   unsigned short __fastcall GetNo(void);
   unsigned short __fastcall GetNe(void);
   AnsiString __fastcall GetID(void);
#ifdef _EDITOR_
   void __fastcall Save(FILE *out);
#endif
   bool __fastcall Load(char *buf);
};

//
// Тип список логических элементов
// управляет списком логических элементов
class TListLogic {
   LinkList<TLogic*> ListLE; // все Логические элементы для данного примитива
public:
   void __fastcall LoadAsBase(char *buf);
   void __fastcall Load(char *buf);
#ifdef _EDITOR_
   void __fastcall CopyLE(TListLogic *c);
   int  __fastcall GetCount(void);
   void __fastcall Save(FILE *f);
   void __fastcall DeleteLE(void);
   void __fastcall InsertID(char *sId);
   AnsiString __fastcall GetStringAllLogic(void);
#endif
   bool  __fastcall PrevLE(void);
   bool  __fastcall NextLE(void);
   bool  __fastcall FirstLE(void);
   bool  __fastcall LastLE(void);
   short __fastcall GetNo(void);
   void  __fastcall SetNo(int sNo);
   short __fastcall GetNe(void);
   void  __fastcall SetNe(int sNe);
   AnsiString __fastcall GetID(void);
   void  __fastcall SetID(AnsiString sId);
   int   __fastcall IndexCurrentLE(void);
   void  __fastcall DeleteAllLE(void);
   void  __fastcall InsertLE(unsigned short sNo, unsigned short sNe);

   bool  __fastcall CheckNo(unsigned short No);
   bool  __fastcall CheckNe(unsigned short No, unsigned short Ne);
   bool  __fastcall CheckID(AnsiString sId);

};


 