///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPID.C															 //
//   TAPID.C -- source file for class TAPID											 //
//   Introduction: derived from TAMagnet, particle identification using magnetic	 //
// rigidity analysis. An interface class from TAMagnet to user.						 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/23.															     //
// Last modified: 2018/1/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <libgen.h> // dirname function
#include <cstring>

// ROOT include
#include "TGraph.h"

#include "TAPID.h"
#include "TAParaManager.h"
#include "TAMath.h"
#include "TACtrlPara.h"
#include "TADeployPara.h"
#include "TAPopMsg.h"
#include "TASiPMPlaArray.h"
#include "TAMWDCArray.h"
#include "TATOFWall.h"
#include "TAPlaStrip.h"
#include "TAPlaStripPara.h"
#include "TAT0_1.h"

//#define DEBUG

TAPID *TAPID::fInstance = nullptr;

TAPID::TAPID(const string &name, const string &title)
	: TAMagnet(name, title), fGCurve(0), fTOFWall{0}, fT0_1(0){
	fIsFlied = true; Initialize(); fIsFlied = false;
	fExB = -9999.;
}
TAPID *TAPID::Instance(){
	if(!fInstance) fInstance = new TAPID();
	return fInstance;
}
TAPID::~TAPID(){}

// l: x = k1*z+b1; y = k2*z+b2; ki:(k1, k2); bi(b1, b2);
// tof2: from target to TOFWall, length2: from exit of magnetic field to hit TOFWall strip
static double c0 = TAParaManager::Instance()->GetPhysConst("c0");
static double u0MeV = TAParaManager::Instance()->GetPhysConst("u0MeV");
// TaHit: target hit position
void TAPID::Fly(double tof2, double x0TaHit, const double *pOut, short dcArrId, const int option, const double *pIn){
	if(0 != dcArrId && 1 != dcArrId) TAPopMsg::Error("TAPID", "Fly: dcArrId neither 0 nor 1 (which is supposed to be a boolean)");

	const double h0 = GetIterationStep(), stepError = GetStepError();
	if(0 == option){
		SetIterationStep(h0*5.); // increment for z per step
		SetStepErrorTolerance(stepError*5.); // truncation error for RK method
	}

	TATOFWall *tofw = fTOFWall[dcArrId]; // 0 (faLse): L; 1 (true): R
	const double tofwPhi = tofw->GetDetPara()->GetPhi();
	const double tofwXc = tofw->GetDetPara()->GetX();
	const double tofwZc = tofw->GetDetPara()->GetZ();
	const double k_tofw = tan(tofwPhi+TAMath::Pi()/2.);
	const double b_tofw = tofwXc - k_tofw * tofwZc;
	const double k1 = pOut[0], k2 = pOut[1], b1 = pOut[2], b2 = pOut[3];
	// the central z coordinate of the target
	static const double z0_TA = TADeployPara::Instance()->GetTargetZ0();
	static const double z0_T0_1 = fT0_1->GetZ0();
	double y0_SiPMArr = 0.; // hit position in the target
	if(-9999. == x0TaHit) TAPopMsg::Error("TAPID", "GetAoQ: SiPM hit position not properly fired");
	// 1050.: z border of the magnetic field.  (xi,yi,zi): initial point to start RK propogation.
	const double zi = 1000., xi = k1*zi + b1, yi = k2*zi + b2;
	double y[2] = {xi, yi}; // start of the RK propagation
	double yp[2] = {k1, k2}; // k1 and k2

	double x_tofwHit = (b1*k_tofw - b_tofw*k1) / (k_tofw - k1);
	double y_tofwHit = (b1 - b_tofw) * k2 / (k_tofw - k1) + b2;
	double z_tofwHit = (b1 - b_tofw) / (k_tofw - k1);
	// (xe, ye, ze): exit point from Mag, treated as at the boundry of the magnetic field
	const double ze = 500., xe = k1*ze + b1, ye = k2*ze + b2;
	double trkL1 = sqrt(xe*xe + ye*ye + ze*ze); // half of the track in the Magnet
	double trkL2 = // from exit of the magnetic field to TOF wall
		sqrt(pow(x_tofwHit-xe, 2.) + pow(y_tofwHit-ye, 2.) + pow(z_tofwHit-ze, 2.));
	double totalTrackLength_t = trkL1+trkL2 - z0_T0_1;
	double beta = totalTrackLength_t / tof2 / c0;
	if(beta < 0. || beta >= 1.){
		fIsFlied = true;
		return;
	}
	// particle propagation in uniform magnetic field -- analytic solution exists
	if(2 == option){
		const double zMagIn = -500., zMagOut = ze;
		const double B = GetExB();
		double result[6]{}, dtheta, rho, ki; // dtheta: particle defelct angle by mag
		double zo, xo; // (zo, xo): rotating center of the arc
		TAMath::UniformMagneticSolution(k1, b1, zMagOut, zMagIn, z0_TA, x0TaHit, result);
		// no eligible solution found
		if(result[0] == result[1] &&result[1] == result[2] &&result[2] == result[3]){
			fIsFlied = true;
			// every element of result was set to be the number of valid solutions (0, 2, or 3)
//			cout << "\nresult[0]: " << result[0] << endl;
//			cout << "fAoZdmin: " << fAoZdmin << endl; getchar(); // DEBUG
			return;
		}
		dtheta = result[0]; rho = result[1]; ki = result[2];
		zo = result[4]; xo = result[5];
		const double d0 = (zMagIn-z0_T0_1)*sqrt(1.+ki*ki); // from T0_1 to Mag
		const double d1 = rho*dtheta; // arc in the Mag
		const double d2 = trkL2; // from Mag to TOF wall
		fTotalTrackLength = d0 + d1 + d2;
		fBeta = fTotalTrackLength / tof2 / c0;
		if(fBeta < 0. || fBeta >= 1.){ // bad beta, failed PID
			fBeta = -1.; fTotalTrackLength = -9999.;
			fIsFlied = true;
			return;
		}
		fGamma = TAMath::Gamma(fBeta);
		// 0.321840605 = e0/(u0*c0*1E6) SI unit
		fAoZ = B * (rho/1000.) * 0.321840605 / (fBeta * fGamma);
		fAoZdmin = 0.;
		fAngleTaOut[0] = atan(ki); fAngleTaOut[1] = 0.;
		fPoZ = fAoZ * fBeta * fGamma * u0MeV; // MeV/c
		fIsFlied = true; // fIsflied should be assigned immediately after flying
		// store the track
		if(fGCurve){
			fTrackVec.clear();
			tra_t tra;
			const int n = 2000;
			for(int i = 0; i <= n; i++){
				double ai = (1. - 2.*i/n)*TAMath::Pi();
				double zi = zo+fabs(rho)*cos(ai);
				double xi = xo+fabs(rho)*sin(ai);
				tra.x = xi; tra.y = 0.; tra.z = zi; tra.rho = rho; tra.brho = rho*B;
				fTrackVec.push_back(tra);
			} // end for
		} // end if(fGCurve)
		return;
	} // end if(2 == option)

	// particle propagation in nonuniform magnetic field //
	// after the initial beta was estimated, trkL2 is assigned with l from pi (not pe) to TOFWall
	trkL2 = // from exit of the magnetic field to TOF wall
		sqrt(pow(x_tofwHit-xi, 2.) + pow(y_tofwHit-yi, 2.) + pow(z_tofwHit-zi, 2.));

#ifdef DEBUG
	double ddmin[2]{}; // quality estimator
#endif
	double aoz, aozc = 1., d2; // aozc: the central aoz
	double span = 3.; // search scope, aozc-span ~ aozc+span
	int ln = 1, n = 60; if(0 == option){ n = 25; }
	for(int iter = 0; iter < 2; iter++){ // iteration to refine beta1
		if(1 == iter){
			// reset search domin, narrow the scope and coodinate the center
			n = 24; ln = 5; if(0 == option){ n = 10; ln = 3; }
			span = 1.; aozc = fAoZ;
			fAoZdmin = 9999.; // reset dmin
		}
		// aozc: center of the scope domain
		for(int l = 0; l < ln; l++){ // outer loop
			for(int ll = 0; ll <= n; ll++){ // interior loop
				aoz = aozc + (2.*ll/n - 1.)*span;
				SetOutOfRangeError(false);
				SetQoP(aoz, beta);
				y[0] = xi; y[1] = yi; // start of the RK propagation
				yp[0] = k1; yp[1] = k2; // k1 and k2
				TransportIon(y, yp, zi, z0_TA);
				if(IsOutOfRange()) continue; // ineligible aoz
				double dd[2] = {y[0] - x0TaHit, y[1] - y0_SiPMArr};
				// use track slope information
				if(!pIn){ // pIn[0-3]: {k1_Ta, b1_Ta, k2_Ta, b2_Ta}
					dd[0] = yp[0] - pIn[0];
					if(-9999. != pIn[2]) dd[1] = yp[1] - pIn[2];
				}
				d2 = sqrt(dd[0]*dd[0] + dd[1]*dd[1]); // ; 2017.7.5 20:27
				// use track position information
				if(0) if(!pIn){
					double dp[2] = {y[0] - pIn[0]*z0_TA+pIn[1], y[1] - pIn[2]*z0_TA+pIn[3]};
					if(-9999. == pIn[2]) dp[1] = 0.;
					d2 += sqrt(dp[0]*dp[0]+dp[1]*dp[1]) * 0.5;
				}
				if(d2 < fAoZdmin){
					fAoZdmin = d2; fAoZ = aoz; fBeta = beta;
					fTotalTrackLength = GetTrackLength() + trkL2;
#ifdef DEBUG
					ddmin[0] = dd[0]; ddmin[1] = dd[1];
#endif
					memcpy(fAngleTaOut, yp, sizeof(fAngleTaOut));
				} // end if
				if(fAoZdmin < 1E-2) break;
			} // end for over ll
			if(-9999. == fAoZ){ // failed, possibly fake track caused by chaos
				fAoZdmin = 9999.; break;
			}
			span = span / n * 2.2;
			aozc = fAoZ;
			if(fAoZdmin < 1E-2) break;
		} // end for over l
		if(-9999. == fAoZ){ // failed, possibly fake track caused by chaos
			fAoZdmin = 9999.; break;
		}
		beta = fTotalTrackLength/tof2/c0;
		if(beta < 0. || beta >= 1.){
			break;
		}
	} // end iteration to refine beta
	fIsFlied = true; // fIsflied should be assigned immediately after flying
	if(beta < 0. || beta >= 1.){ // bad beta, failed PID
		fBeta = -1.; fTotalTrackLength = -9999.;
		fAoZ = -9999.; fAoZdmin = 9999.;
		return;
	}
	if(9999. == fAoZdmin) return; // PID failure
	fGamma = TAMath::Gamma(fBeta);
	fPoZ = fAoZ * fBeta * fGamma * u0MeV; // MeV/c

#ifdef DEBUG
	cout << "___________________________________________________________\n";
	cout << "fBeta: " << fBeta << endl; // DEBUG
	cout << "ddmin[0]: " << ddmin[0] << "\tddmin[1]: " << ddmin[1] << endl; // DEBUG
	cout << "fAoZ: " << fAoZ << endl; // DEBUG
	cout << "fTotalTrackLength: " << fTotalTrackLength << endl; getchar(); // DEBUG
#endif

	if(fGCurve && -9999. != fAoZ){
		// Fill curved track
		SetOutOfRangeError(false);
		SetQoP(fAoZ, fBeta);
		y[0] = xi; y[1] = yi; // start of the RK propagation
		yp[0] = k1; yp[1] = k2; // k1 and k2
		TransportIon(y, yp, zi, z0_TA, true);
		FillGraphTrajectory();
	}

	if(0 == option){
		// recover precision level setttings
		SetIterationStep(h0); SetStepErrorTolerance(stepError);
	}
} // end member method Fly

void TAPID::FillGraphTrajectory() const{
	if(!fGCurve) return;
	const vector<tra_t> &trackVec = GetTrackVec();
//	cout << "trackVec.size(): " << fTrackVec.size() << endl; // DEBUG
	for(const tra_t &t : trackVec){
//		t.show(); getchar(); // DEBUG
		fGCurve->SetPoint(fGCurve->GetN(), t.z, t.x);
	} // end for
	char name[64];
	sprintf(name, "%s_%1.3f\n", fGCurve->GetName(), GetAoZ());
	fGCurve->SetName(name);
//	cout << "fGCurve->GetName(): " << fGCurve->GetName() << endl; getchar(); // DEBUG
}

double TAPID::GetAoZ() const{
	if(!IsFlied()) TAPopMsg::Error("TAPID", "GetAoZ: Particle Not Flied");
	return fAoZ;
}
double TAPID::GetBeta() const{
	if(!IsFlied()) TAPopMsg::Error("TAPID", "GetBeta: Particle Not Flied");
	return fBeta;
}
double TAPID::GetGamma() const{
	if(!IsFlied()) TAPopMsg::Error("TAPID", "GetGamma: Particle Not Flied");
	return fGamma;
}
double TAPID::GetPoZ() const{
	if(!IsFlied()) TAPopMsg::Error("TAPID", "GetPoZ: Particle Not Flied");
	return fPoZ;
}
double TAPID::GetChi() const{
	if(!IsFlied()) TAPopMsg::Error("TAPID", "GetChi: Particle Not Flied");
	return fAoZdmin;
}
void TAPID::GetTargetExitAngle(double *a) const{
	if(!IsFlied()) TAPopMsg::Error("TAPID", "GetTargetExitAngle: Particle Not Flied");
	memcpy(a, fAngleTaOut, sizeof(fAngleTaOut));
}
double TAPID::GetTotalTrackLength() const{
	if(!IsFlied()) TAPopMsg::Error("TAPID", "GetTotalTrackLength: Particle Not Flied");
	return fTotalTrackLength;
}
double TAPID::GetExB() const{
	if(-9999. == fExB) TAPopMsg::Warn("TAPID", "GetExB: fExB not assigned");
	return fExB;
}
void TAPID::Configure(){
	// assign the magnetic field table
	string magFile = TACtrlPara::Instance()->ConfigExpDir();
	char tmp[64]; strcpy(tmp, magFile.c_str());
	magFile = dirname(tmp);
	magFile += "/MagnetMap/magnetic.txt";
	LoadMagneticFieldFile(magFile.c_str());
	double p0[3]{}, B0[3]{}; p0[1] = 90.; GetMagneticIntensity(B0, p0);
	double B0m = TAMath::norm(B0);
	SetScale(GetExB() / B0m); // for extrapolation from B @1000A to B at other currents
//	cout << "fExB: " << fExB << "\tB0m: " << B0m << endl; // DEBUG
//	cout << "fScale: " << fScale << endl; getchar(); // DEBUG
	SetIterationStep(2E1); // increment for z per step
	SetStepErrorTolerance(1E1); // truncation error for RK method
	EnableEnergyLoss(false); // calculate energy loss in the magnetic field
	// kClassic, kNystrom. kNystrom is preferrable, well kClassic is considered normal and safe
	SetRKMethod(kNystrom);

	// assign TOF wall pionters
	TAParaManager::ArrDet_t &dec_vec = TAParaManager::Instance()->GetDetList();
	if(!dec_vec[3] && !dec_vec[4])
		TAPopMsg::Error("TAPID", "Configure: MWDC arrays in paraManger are null. Consider to put this TOFWall pointer assignment in TAPID::Configure(), and put TAPID::Configure in the last of TAEventProcessor::Configure to lick the problem");
	if(!dec_vec[1])
		TAPopMsg::Error("TAPID", "Configure: T0_1 in paraManager is null. Note that detector construction should be implemented prior to the current function in TAEventProcessor::Configure");
	TAMWDCArray *dcArr[2]{0};
	dcArr[0] = (TAMWDCArray*)dec_vec[3]; fTOFWall[0] = dcArr[0]->GetTOFWall(); // dc array L
	dcArr[1] = (TAMWDCArray*)dec_vec[4]; fTOFWall[1] = dcArr[1]->GetTOFWall(); // dc array R
	fT0_1 = (TAT0_1*)dec_vec[1];

//	p0[0] = 0.; p0[1] = 00.; p0[2] = 0.;
	GetMagneticIntensity(B0, p0); B0m = TAMath::norm(B0);
//	printf("Configure: B(0,0,0): |(%f, %f, %f)| = %f", B0[0], B0[1], B0[2], B0m); getchar(); // DEBUG
	TAPopMsg::ConfigInfo("TAPID", "Configure: magFile: %s", magFile.c_str());
	TAPopMsg::ConfigInfo("TAPID", "Configure: B(0,0,0): |(%f, %f, %f)| = %f", B0[0], B0[1], B0[2], B0m); // DEBUG
}

void TAPID::Initialize(){
	if(!IsFlied()) return;
	fAoZ = -9999.; fAoZdmin = 9999.;
	fBeta = -1.; fGamma = -1.; fPoZ = -9999.;
	fAngleTaOut[0] = -9999.; fAngleTaOut[1] = -9999.;
	fTotalTrackLength = -9999; fIsFlied = false;
}





