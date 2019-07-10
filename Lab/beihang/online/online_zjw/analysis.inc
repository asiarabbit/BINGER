//Set Analysis parameters
//	from 2018/March by S.T.
//
// paricle identification parameter
float brho = 5.1183;//Tm after F1 for 16O
float coe_aoq[2]={0.0000,1.0000};
float coe_zz[4]={1.0,1.0,1.0,1.0};//in1,in2,out1,out2
float off_zz[4]={0.0,0.0,0.0,0.0};//in1,in2,out1,out2

//start,stoL,stoR,Ve1L,Ve1R,Ve2L,Ve2R,PL2L,PL2R
float off_tof[9]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
float coe_tof=0.09765625;//almost fix for v1x90
//no 'start'
float off_sci[8]={0.,0.,0.,0.,0.,0.,0.,0.};//ch:pedestal
float gain_sci[8]={1.,1.,1.,1.,1.,1.,1.,1.};
//Musicf(4),Musicb(8),Open(3),PU(1)
float off_adc[16]={0.,0.,0.,0.,0.,0.,0.,0.,
               0.,0.,0.,0.,0.,0.,0.,0.};//ch
float gain_adc[16]={1.,1.,1.,1.,1.,1.,1.,1.,
               1.,1.,1.,1.,1.,1.,1.,1.};
//TAC calibration L,R,LR
float coe_tac[3]={1.05e-2,1.05e-2,1.05e-2};
float beta_par[3]={25.88,160.,160.};//Fligtpath[m], nsec(TAC), nsec(TDC)

//conversion QDC ch to PlasticID
int conv[8]={4,6,8,10,12,14,16,18};

//number of common for V1x90 0-origin
int cid = 9; //now in ana.cxx as cid1&cid2
// Time to Drift parameter for Ar-gas
// 'ch', 'cm/ch', 'cm/ch^2', 'cm/ch^3', 'cm/ch^4'  @1ch = 97.65psec = 25nsec/256
//float tpar[5]={1200.0,3.85e-4,0.0,0.0,0.0};//P10
//float tpar[5]={1650.,2.99e-4,0.,0.,0.};//another traial
//float tpar[5]={1650.,+7.44e-4,-3.21e-7,+8.12e-11,-7.53e-15};//another trail
//float tpar[5]={1650,+7.45e-4,-3.15e-07,+7.51e-11,-6.33e-15};
//float tpar[5]={1650.,0.000697693, -2.71205e-07, 6.25838e-11, -5.59645e-15};//RUN070 1.3kV
float tpar[5]={1550.,0.000697693, -2.71205e-07, 6.25838e-11, -5.59645e-15};//RUN037 1.2kV
//float tpar[5]={1650.,0.000697693, -2.71205e-07, 6.25838e-11, -5.59645e-15};//RUN036 1.3kV
//float tpar[5]={1700.,0.000697693, -2.71205e-07, 6.25838e-11, -5.59645e-15};//RUN035 1.4kV
//float tpar[5]={1750.,0.000697693, -2.71205e-07, 6.25838e-11, -5.59645e-15};//RUN034 1.5kV
//float tpar[5]={1850.,0.00045000, 0.0,0.0,0.0};// RUN033
// relative offset for all plane in 'ch'
float toff[16]={60.0,60.0,0.0,0.0,60.0,60.0,0.0,0.0
	       ,20.0,20.0,-40.0,-40.0,20.0,20.0,-40.0,-40.0};
//float toff[16]={0.0,0.0,20.0,10.0,0.0,-10.0,20.0,30.0,
//float toff[16]={0.0,0.0,50.0,40.0,0.0,-10.0,50.0,60.0,
//	0,0,20,10,0,0,10,30};

//Z-information in cm
//float leng[16]={-3.,-2.,+4.,+5.,-5.,-4.,+2.,+3.
//		,-3.,-2.,+4.,+5.,-5.,-4.,+2.,+3.};//RCNP one PDC X&Y
float leng[16]={-3.,-2.,+58.,+59.,-5.,-4.,+56.,+57.
		,-5.,-4.,+81.,+82.,-3.,-2.,+83.,+84.};//IMP 2018/05
//length of detectors XY1,XY2,tgt,XY3,XY4,--,--,--,--,-- in cm
float zz[10]={-67.8,-13.8,+0.0,+15.0,+94.0
                  ,0.0,0.0,0.0,0.0,0.0};

