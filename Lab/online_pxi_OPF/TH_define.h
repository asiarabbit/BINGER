#ifndef TH_define
#define TH_define

#define hbook_init TH_init
#define hbook_fill TH_fill
#define analyze    Analyze

extern void HF1(int id, float x, float weight);
extern void HF2(int id, float x, float y, float weight);
extern void HBOOK1(int id, const char *title, int nxbin, float xlow, float xup, float vmx);
extern void HBOOK2(int id, const char *title, int nxbin, float xlow, float xup, int nybin, float ylow, float yup, float vmx);
extern bool HEXIST(int ihist);

#endif //#ifndef TH_define
