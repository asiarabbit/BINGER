
int poln = 6;
int STRid = 9; // 9 ~ 14
int STRarr = STRid - 9;
const char *dir = "STR_1000V";
//#define FIX // to fix the deviation brought about by fitting

// dividing points for STR fitting
const double ccx0[6] = {0., 0., 0., 0., 0., 0.};
const double ccx1[6] = {180., 180., 150., 180., 180., 180.};
const double ccx2[6] = {400., 400., 400., 400., 400., 400.};

#include "../src/fitSTR_core.C"

