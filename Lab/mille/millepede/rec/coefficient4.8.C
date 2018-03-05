// coefficient.C -- coefficients for fun function
// control variables and other global variables defined here.
#include "TAMatrix.h"
#include "TAMatrixSym.h"
#include "TAFun.h"

// ROOT header files
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

const int NG = 18, NL = 4, NT = NG + NL; // Number of Global, Local and Total parameters
const int NO = 18; // Number of Observation points
const int NC = 6; // Number of equality Constraints (which reduces NG to (NG-NC))
const int NITERGLB = 3; // iteration times to fit global parameters
const int NITERLCL = 3; // iteration times to fit local parameters

bool INV_TEST = false; // test invert method mode

// constraints: offsets of position and orientation of the first DC is fixed to zero
// X*6-U*6-V*6
const double offset[NO] = {
	0., 0., 0., 0., 0., 0., // DC0: (dx, dy, dz, phi, theta, psi)
	0., 0., 0., 0., 0., 0., // DC1: (dx, dy, dz, phi, theta, psi)
	0., 0., 0., 0., 0., 0.  // DC2: (dx, dy, dz, phi, theta, psi)
};
const double res[NO] = {
	0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3
};

// DC0, All[0](X, Y, Z): 1162.02, 600, 1312.24
// DC1, All[1](X, Y, Z): 1414.97, 600, 1755.38
// DC2, All[2](X, Y, Z): 1570.79, 600, 2190.56
static const double Al[3][3] = { // a point in MWDC0-1-2 : (x, y, z)
	{1162.02, 600, 1312.24}, {1414.97, 600, 1755.38}, {1570.79, 600, 2190.56}
};
double cons[NG*NC] = {
// ----	// // // // // // constraints: overall rotation is zero // // // // // // ---- //
// -dx0 -dx1 -dx2 +Aly0*dalpha0 +Aly1*dalpha1 +Aly2*dalpha2 -Alz0*dgamma0 -Alz1*dgamma1 -Alz2*dgamma2 
// -dy0 -dy1 -dy2 -Alx0*dalpha0 -Alx1*dalpha1 -Alx2*dalpha2 +Alz0*dbeta0  +Alz1*dbeta1  +Alz2*dbeta2 
// -dz0 -dz1 -dz2 -Aly0*dbeta0  -Aly1*dbeta1  -Aly2*dbeta2  +Alx0*dgamma0 +Alx1*dgamma1 +Alx2*dgamma2 
// {dx, dy, dz, alpha, beta, gamma},{dx, dy, dz, alpha, beta, gamma},{dx, dy, dz, alpha, beta, gamma}
	-1., 0., 0.,   Al[0][1], 0., -Al[0][2],       -1., 0., 0.,   Al[1][1], 0., -Al[1][2],       -1., 0., 0.,   Al[2][1], 0., -Al[2][2],
	0., -1., 0.,   -Al[0][0], Al[0][2], 0.,       0., -1., 0.,   -Al[1][0], Al[1][2], 0.,       0., -1., 0.,   -Al[2][0], Al[2][2], 0.,
	0., 0., -1.,   0., -Al[0][1], Al[0][0],       0., 0., -1.,   0., -Al[1][1], Al[1][0],       0., 0., -1.,   0., -Al[2][1], Al[2][0],
// ----	// // // // // // constraints: overall translation is zero // // // // // // ---- //
	1., 0., 0.,    0., 0., 0.,                    0., 0., 0.,    0., 0., 0.,                    0., 0., 0.,    0., 0., 0.,
	0., 1., 0.,    0., 0., 0.,                    0., 0., 0.,    0., 0., 0.,                    0., 0., 0.,    0., 0., 0.,
	0., 0., 1.,    0., 0., 0.,                    0., 0., 0.,    0., 0., 0.,                    0., 0., 0.,    0., 0., 0.
};
const double consc[NC] = {0., 0., 0., 0., 0., 0.};
const double phiAvrg = (ccLayout_DCArrR.MWDC[1].phi+ccLayout_DCArrR.MWDC[1].phi+ccLayout_DCArrR.MWDC[1].phi) * DEGREE / 3.;

////////################ CONFIGURE AND DEFINE THE OBJECTIVE FUNCTION ##################////////
// p[22]: (x,y,z,phi,theta,psi)*3=18:(0~17), (k1,k2,b1,b2)=4:(18~21);
double fun(const double *a, const double *p, void *ptr){
	/////// constants for the function ///////
	static const double sec30 = 1. / cos(30. * DEGREE);
	static const double A0[6][3] = { // 6 wires : X-U-V
		{-393.,       0.,   0.}, {-397.,       0.,  10.}, // X
		{-393.*sec30, 0., -40.}, {-397.*sec30, 0., -30.}, // U
		{-397.*sec30, 0.,  40.}, {-393.*sec30, 0.,  50.}, // V
	};
	// detector angle
	static const double phi[3] = {0., -30. * DEGREE, 30. * DEGREE}; // X-U-V
	// sense wire orientation
	static const double al[3][3] = {{0., 1., 0.}, {1./2., sqrt(3.)/2., 0.}, {-1./2., sqrt(3.)/2., 0.}}; // X-U-V
	static const double Bz = ccLayout_DCArrR.MWDC[1].zc; // DC[1]-X1-central z
	TAMWDCArray* DCArrR = (TAMWDCArray*)ptr;


	// transform the first input parameter
	// the point where the measurement happens
	int LAYER = int(a[0]); // 0~17: nuX, nuU+6, nuV+6
	int DCid = (LAYER % 6) / 2; // 0-1-2: DC0-DC1-DC2
	int dcType = LAYER / 6; // 0-1-2: X-U-V
	int layerType = LAYER % 2 + 1; // 1-2: anodeL1-anode L2
	int kk = dcType * 2 + layerType - 1; // DC#X1-X2--DC#U1-U2--DC#V1-V2
	int nu = int(a[1]); // 0~80; anode id in the layer

//	cout << "a[0]: " << a[0] << "\ta[1]: " << a[1] << endl; // DEBUG
//	cout << "LAYER: " << LAYER << "\tDCid: " << DCid << "\tdcType: " << dcType << endl; // DEBUG
//	cout << "layerType: " << layerType << "\tkk:" << kk << "\tnu: " << nu << endl; // DEBUG
//	getchar(); // DEBUG

	// 3-D track line parameters. B[3]: one point in the line; b[3]: direction vector
	double B[3] = {0., 0., Bz}, b[3] = {0., 0., 1.}; // B[2] = 0. and b[2] = 1.; preset values.
	B[0] = p[20]+B[2]*p[18]; // B[0] = b1+B[2]*k1;
	B[1] = p[21]+B[2]*p[19]; // B[1] = b2+B[2]*k2;
	b[0] = b[2]*p[18]; // b[0] = b[2]*k1;
	b[1] = b[2]*p[19]; // b[1] = b[2]*k2;
	
	double bb = sqrt(b[0]*b[0]+b[1]*b[1]+b[2]*b[2]); // DEBUG
//	cout << "B[0]: " << B[0] << "\tB[1]: " << B[1] << "\tB[2]: " << B[2] << endl; getchar(); // DEBUG
//	cout << "After normalized, b[0]: " << b[0] / bb << "\tb[1]: " << b[1] / bb << "\tb[2]: " << b[2] / bb << endl; getchar(); // DEBUG


	// 3-D sense wire line parameters. Ag[3]: one point in the line; ag[3]: direction vector
	double Ag[3], ag[3], a_temp[3]; // ag: dR.R.a
	double Al[3] = // local coordinates of anode k. X-U-V
	 {A0[kk][0]+10.*nu/cos(phi[dcType]), A0[kk][1], A0[kk][2]};
	TAMWDC *mwdc = DCArrR->GetMWDC(DCid);
	mwdc->SetPositionOffset(&p[DCid*6]); // apply the input position offset parameters (dr0)
	mwdc->SetAngleOffset(&p[DCid*6]+3); // apply the input global rotation offset parameters (dR)
	mwdc->GetGlobalPosition(Al, Ag); // dR.R.Al+r0+dr0
	mwdc->GetGlobalRotation(al[kk/2], ag); // dR.R.al

	double ab[3] = // cross product of vector ag and b.  (dR.R.al)×b
	 {ag[1]*b[2]-ag[2]*b[1], ag[2]*b[0]-ag[0]*b[2], ag[0]*b[1]-ag[1]*b[0]};
	// |[B-(dR.R.Al+r0+dr0)]| * [(dR.R.al)×b] / |(dR.R.al)×b|
//	double d = fabs((B[0]-Ag[0])*ab[0]+(B[1]-Ag[1])*ab[1]+(B[2]-Ag[2])*ab[2])/ // DEBUG
//	 sqrt(ab[0]*ab[0]+ab[1]*ab[1]+ab[2]*ab[2]); // DEBUG
//	cout << "The distance to be returned is " << d << endl; getchar(); // DEBUG
	return fabs((B[0]-Ag[0])*ab[0]+(B[1]-Ag[1])*ab[1]+(B[2]-Ag[2])*ab[2])/
	 sqrt(ab[0]*ab[0]+ab[1]*ab[1]+ab[2]*ab[2]);
} // end of function fun.


////////############## END OF CONFIGURE AND DEFINE THE OBJECTIVE FUNCTION ################////////











