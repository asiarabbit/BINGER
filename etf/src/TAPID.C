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
// Last modified: 2018/9/18, SUN Yazhou.										     //
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
#include "TADetectorPara.h"
#include "TASiPMPlaArray.h"
#include "TAMWDCArray.h"
#include "TATOFWall.h"
#include "TAPlaStrip.h"
#include "TAPlaStripPara.h"
#include "TAT0_1.h"

//#define DEBUG

TAPID *TAPID::fInstance = nullptr;

TAPID::TAPID(const string &name, const string &title)
	: TAMagnet(name, title), fGCurve(0), fTOFWall{0}, fT0_1(0), fIsFlied(1){
	Initialize();
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
// Fly function dedicated for pion experiment
void TAPID::FlyPion(double tof2, double x0TaHit, const double *pOut_, short dcArrId, const OPTION option){
	if(kOpt0 != option && kOpt1 != option && kOpt2 != option && kOpt3 != option)
		TAPopMsg::Error("TAPID", "FlyPion: invalid option value: only kOpt0, kOpt1, kOpt2 or kOpt3 is allowed");
	if(0 != dcArrId && 1 != dcArrId)
		TAPopMsg::Error("TAPID", "FlyPion: dcArrId neither 0 nor 1 (which is supposed\
 to be a boolean-like integer)");

	const double h0 = GetIterationStep(), stepError = GetStepError();
	if(kOpt0 == option){
		SetIterationStep(h0*5.); // increment for z per step
		SetStepErrorTolerance(stepError*5.); // truncation error for RK method
	}

	if(!fTOFWall[dcArrId])
		TAPopMsg::Error("TAPID", "FlyPion: fTOFWall is null pointer - The parent MWDC array not created");
	TATOFWall *tofw = fTOFWall[dcArrId]; // 0 (faLse): L; 1 (true): R
	const double tofwPhi = tofw->GetDetPara()->GetPhi();
	const double tofwXc = tofw->GetDetPara()->GetX();
	const double tofwZc = tofw->GetDetPara()->GetZ();
	const double k_tofw = tan(tofwPhi+TAMath::Pi()/2.);
	const double b_tofw = tofwXc - k_tofw * tofwZc;
	// particle trajectory function pre-post-target and post-mag; 0-1-2-3: k1, k2, b1, b2
	double pOut[4] = {pOut_[0], pOut_[1], pOut_[2], pOut_[3]}; // track after the dipole magnet //
	if(-9999. == pOut[1]) pOut[1] = 0.; // k2_out
	if(-9999. == pOut[3]) pOut[3] = 0.; // b2_out
	double k1 = pOut[0], k2 = pOut[1], b1 = pOut[2], b2 = pOut[3];

	// the central z coordinate of the target
	static const double z0_TA = TADeployPara::Instance()->GetTargetZ0(); // 
	static const double z0_T0_1 = fT0_1->GetZ0(); // -2699.08
	double y0_SiPMArr = 0.; // hit position in the target
	if(-9999. == x0TaHit)
		TAPopMsg::Error("TAPID", "GetAoQ: SiPM hit position not properly fired");
	// 1050.: z border of the magnetic field.  P_i: (xi,yi,zi): initial point to start RK propogation
	const double zi = 1000., xi = pOut[0]*zi + pOut[2], yi = pOut[1]*zi + pOut[3];
	const double yp0[2] = {pOut[0], pOut[1]}; // magF initial entrance direction
	double y[2] = {xi, yi}; // start of the RK propagation
	double yp[2] = {yp0[0], yp0[1]}; // k1_in and k2_in

	/////////////// COARSE ESTIMATE FOR BETA AFTER THE TARGET ///////////////
	double x_tofwHit = (b1*k_tofw - b_tofw*k1) / (k_tofw - k1);
	double y_tofwHit = (b1 - b_tofw) * k2 / (k_tofw - k1) + b2;
	double z_tofwHit = (b1 - b_tofw) / (k_tofw - k1);
	// P_e(xe, ye, ze): exit point from Mag, treated as at the boundry of the magnetic field
	const double ze = 500., xe = k1*ze + b1, ye = k2*ze + b2;
	const double trkL0 = 0. - z0_T0_1; // from TOF stop to the center of the dipole magnet (the origin)
	const double trkL1 = sqrt(xe*xe + ye*ye + ze*ze); // from the origin to P_e
	const double trkL2 = // from P_e to the hit point of the TOF wall
		sqrt(pow(x_tofwHit-xe, 2.) + pow(y_tofwHit-ye, 2.) + pow(z_tofwHit-ze, 2.));
	const double totalTrackLength_t = trkL0 + trkL1 + trkL2;
	double beta = totalTrackLength_t / tof2 / c0;
	if(beta < 2.) beta = beta < 1. ? beta : 0.9; // toleration for beta error (from tof2 or trkLenT)
	if(beta < 0. || beta >= 1.){
		fIsFlied = true;
		return;
	}
	///////////////////////////////////////////////////////////////////////////

	// particle propagation in uniform magnetic field -- analytic solution exists
	// 2: uniform Mag - point+trk
	if(kOpt2 == option){
		const double B = GetExB();
		double dtheta, rho, ki; // ki: x=ki z+bi, track after Target, before Mag
		double zo, xo; // (zo, xo): rotating center of the arc
		const double zMagIn = -500., zMagOut = ze;
		double result[6]{}; // dtheta: particle defelct angle by mag
		TAMath::UniformMagneticSolution(k1, b1, zMagOut, zMagIn, z0_TA, x0TaHit, result);
		// no eligible solution found
		if(result[0] == result[1] && result[1] == result[2] && result[2] == result[3]){
			fIsFlied = true;
			// every element of result was set to be the number of valid solutions (0, 2, or 3)
//			cout << "\nresult[0]: " << result[0] << endl;
//			cout << "fAoZdmin: " << fAoZdmin << endl; getchar(); // DEBUG
			return;
		}
		dtheta = result[0]; rho = result[1]; ki = result[2];
		zo = result[4]; xo = result[5];
		const double d0 = (zMagIn - z0_T0_1)*sqrt(1.+ki*ki); // from T0_1 to Mag
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
//		cout << "fBeta: " << fBeta << "\tfTotalTrackLength: " << fTotalTrackLength << endl;
//		getchar(); // DEBUG
		// 0.321840605 = e0/(u0*c0*1E6) SI unit
		fAoZ = B * (rho/1000.) * 0.321840605 / (fBeta * fGamma);
		fAoZdmin = 0.;
		fAngleTaOut[0] = atan(ki); fAngleTaOut[1] = atan(k2);
		fPoZ = B * (rho/1000.) * 0.321840605 * u0MeV; // MeV/c
		fBrho = B * rho / 1000.; // T.m
		fIsFlied = true; // fIsflied should be assigned immediately after flying
//		cout << "fAoZ: " << fAoZ << "\tfBrho: " << fBrho << endl; // DEBUG
//		getchar(); // DEBUG
		// store the track
		if(fGCurve){
			fTrackVec.clear();
			tra_t tra;
			const int n = 20000;
			for(int i = 0; i <= n; i++){
				double ai = (1. - 2.*i/n)*TAMath::Pi();
				double zi = zo+fabs(rho)*cos(ai);
				double xi = xo+fabs(rho)*sin(ai);
				tra.x = xi; tra.y = 0.; tra.z = zi; tra.rho = rho; tra.brho = rho*B;
				fTrackVec.push_back(tra);
			} // end for
		} // end if(fGCurve)
		if(kOpt2 == option) return;
	} // end if(kOpt2 == option || 3 = option)

	// particle propagation in nonuniform magnetic field //
	double pT0_1[3] = {0., 0., z0_T0_1}, pTaHit[3] = {0., 0., z0_TA};
	double pMagOut[3] = {0., 0., zi};
	const double pTOFWHit[3] = {x_tofwHit, y_tofwHit, z_tofwHit};
	pT0_1[0] = 0.; // T0_1 hit x
	pT0_1[1] = 0.; // T0_1 hit y
	pTaHit[0] = 0.; // Target hit x
	pTaHit[1] = 0.; // Target hit y
	pMagOut[0] = pOut[0] * pMagOut[2] + pOut[2]; // MagIn hit x
	pMagOut[1] = pOut[1] * pMagOut[2] + pOut[3]; // MagIn hit y
	// trajectoryLength0-1-2: T0_1->Ta; mag arc; exit magF->TOFW
	const double trajectoryLength0 = TAMath::L(pT0_1, pTaHit); // from TOFStop to Target
	const double trajectoryLength2 = TAMath::L(pMagOut, pTOFWHit); // from Mag Field to TOFW

#ifdef DEBUG
	double ddmin[2]{}; // quality estimator
#endif
	const double zf = z0_TA; // the end of the RK propogation
	double aoz, aozc = 2.2, d2; // aozc: the central aoz; d2: LSM Qsquare
	if(kOpt3 == option){ aozc = fAoZ; }
	const double span0 = 1.;
	double span = span0; // search scope, aozc-span ~ aozc+span
	int ln = 3, n = 40;
	// loop laps for iter==0, which is to refine beta
	if(kOpt0 == option){ n = 15; }
	for(int iter = 0; iter < 2; iter++){ // iteration to refine beta2
		if(1 == iter){
			// reset search domin, narrow the scope and coodinate the center
			span = span0 / n * 3.;
			n = 20; ln = 3;
			aozc = fAoZ;
			fAoZdmin = 9999.; // reset dmin
			if(kOpt0 == option){ n = 5; ln = 3; }
			if(kOpt3 == option){ n = 5; ln = 3; span = 0.5; }
//			SetIterationStep(h0*0.5);
		}
		// aozc: center of the scope domain
		for(int l = 0; l < ln; l++){ // outer loop
//			cout << "l: " << l << "\tln: " << ln << "\titer: " << iter << endl; // DEBUG
//			cout << "\tspan: " << span << "\taozc: " << aozc << "\tn: " << n << endl; // DEBUG
//			getchar(); // DEBUG
			for(int ll = 0; ll <= n; ll++){ // interior loop
//				cout << "ll: " << ll << "\taoz: " << aoz << endl; // DEBUG
//				getchar(); // DEBUG
				aoz = aozc + (2.*ll/n - 1.)*span;
				if(fabs(aoz) > 5.) continue;
				// initialize the RK propagation
				SetOutOfRangeError(false);
				SetQoP(aoz, beta);
				y[0] = xi; y[1] = yi;
				yp[0] = yp0[0]; yp[1] = yp0[1];

				TransportIon(y, yp, zi, zf);

				if(IsOutOfRange()) continue; // ineligible aoz
				double dd[2]{};
				dd[0] = y[0] - x0TaHit; dd[1] = y[1] - y0_SiPMArr;
				d2 = sqrt(dd[0]*dd[0] + dd[1]*dd[1]);

				if(d2 < fAoZdmin){
					fAoZdmin = d2; fAoZ = aoz; fBeta = beta;

					/////////////// CALCULATE TOTAL TRACK LENGTH ////////////////
					const double trajectoryLength1 = GetTrackLength(); // arc length in the Mag Field
					fTotalTrackLength = trajectoryLength0 + trajectoryLength1 + trajectoryLength2;
					memcpy(fAngleTaOut, yp, sizeof(fAngleTaOut));
					//////////////////////////////////////////////////////////////
#ifdef DEBUG
					ddmin[0] = dd[0]; ddmin[1] = dd[1];
					cout << "fAoZdmin: " << fAoZdmin << "\tdd[0]: " << dd[0] << "\tdd[1]: "; // DEBUG
					cout << dd[1] << "\tfAoZ: " << fAoZ << endl; // DEBUG

					cout << "dd[0]*dd[0] + dd[1]*dd[1]: " << dd[0]*dd[0] + dd[1]*dd[1] << endl; // DEBUG
					cout << "d2: " << d2 << endl; // DEBUG

					getchar(); // DEBUG
#endif
				} // end if
				if(fAoZdmin < 1E-8) break;
			} // end for over ll
			if(-9999. == fAoZ){ // failed, possibly fake track caused by chaos
				fAoZdmin = 9999.; break;
			}
#ifdef DEBUG
			cout << "\033[31;1m______________l:" << l << "___________________\033[0m\n" << endl; // DEBUG
#endif
			span = span / n;
			aozc = fAoZ;
			if(fAoZdmin < 1E-8) break;
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
//		cout << "tof2: " << tof2 << "\tfTotalTrackLength: " << fTotalTrackLength << endl; // DEUBG
//		cout << "beta: " << beta << endl; getchar(); // DEBUG
		return;
	}
	if(9999. == fAoZdmin){
		return; // PID failure
	}
	fGamma = TAMath::Gamma(fBeta);
	fPoZ = fAoZ * fBeta * fGamma * u0MeV; // MeV/c
	fBrho = fPoZ / (0.321840605 * u0MeV);

#ifdef DEBUG
	cout << "___________________________________________________________\n";
	cout << "fBeta: " << fBeta << endl; // DEBUG
	cout << "fBrho: " << fBrho << endl; // DEBUG
	cout << "ddmin[0]: " << ddmin[0] << "\tddmin[1]: " << ddmin[1] << endl; // DEBUG
	cout << "fAoZ: " << fAoZ << endl; // DEBUG
	cout << "fAoZdmin: " << fAoZdmin << endl; // DEBUG
	cout << "tof2: " << tof2 << endl; // DEBUG
	cout << "fTotalTrackLength: " << fTotalTrackLength << endl; // DEBUG
	getchar(); // DEBUG
#endif

	if(fGCurve && -9999. != fAoZ){
		SetIterationStep(h0/10); SetStepErrorTolerance(stepError/10); // increase RK precision //
		// initialize the RK propagation
		SetOutOfRangeError(false);
		SetQoP(fAoZ, fBeta);
		y[0] = xi; y[1] = yi;
		yp[0] = yp0[0]; yp[1] = yp0[1];
		TransportIon(y, yp, zi, zf, true);
		SetIterationStep(h0); SetStepErrorTolerance(stepError); // recover RK precision //
//		FillGraphTrajectory();
	}

	if(kOpt0 == option){
		// recover precision level setttings
		SetIterationStep(h0); SetStepErrorTolerance(stepError);
	}

} // end of member function FlyPion

// TaHit: target hit position
// see TAPID.h for the interpretation of option
void TAPID::Fly(double tof2, double x0TaHit, const double *pOut_, short dcArrId, const OPTION option,
		const double *pIn_, const double *pIn0_){
	if(0 != dcArrId && 1 != dcArrId)
		TAPopMsg::Error("TAPID", "Fly: dcArrId neither 0 nor 1 (which is supposed to be a boolean-like integer)");

	const double h0 = GetIterationStep(), stepError = GetStepError();
	if(kOpt0 == option){
		SetIterationStep(h0*5.); // increment for z per step
		SetStepErrorTolerance(stepError*5.); // truncation error for RK method
	}

	if(!fTOFWall[dcArrId])
		TAPopMsg::Error("TAPID", "Fly: fTOFWall is null pointer - The parent MWDC array not created");
	TATOFWall *tofw = fTOFWall[dcArrId]; // 0 (faLse): L; 1 (true): R
	const double tofwPhi = tofw->GetDetPara()->GetPhi();
	const double tofwXc = tofw->GetDetPara()->GetX();
	const double tofwZc = tofw->GetDetPara()->GetZ();
	const double k_tofw = tan(tofwPhi+TAMath::Pi()/2.);
	const double b_tofw = tofwXc - k_tofw * tofwZc;
	// particle trajectory function pre-post-target and post-mag; 0-1-2-3: k1, k2, b1, b2
	double pOut[4] = {pOut_[0], pOut_[1], pOut_[2], pOut_[3]}; // track after the dipole magnet //
	if(-9999. == pOut[1]) pOut[1] = 0.; // k2_out
	if(-9999. == pOut[3]) pOut[3] = 0.; // b2_out
	double k1 = pOut[0], k2 = pOut[1], b1 = pOut[2], b2 = pOut[3];
	double pIn[4] = {pIn_[0], pIn_[1], pIn_[2], pIn_[3]}; // track between target and the mag // 
	if(-9999. == pIn[1]) pIn[1] = 0.; // k2_in
	if(-9999. == pIn[3]) pIn[3] = 0.; // b2_in
	double pIn0[4] = {pIn0_[0], pIn0_[1], pIn0_[2], pIn0_[3]}; // track before the target //
	if(-9999. == pIn0[0]) pIn0[0] = 0.; // k1_in0
	if(-9999. == pIn0[2]) pIn0[2] = 0.; // b1_in0
	if(-9999. == pIn0[1]) pIn0[1] = 0.; // k2_in0
	if(-9999. == pIn0[3]) pIn0[3] = 0.; // b2_in0

	// the central z coordinate of the target
	static const double z0_TA = TADeployPara::Instance()->GetTargetZ0(); // 
	static const double z0_T0_1 = fT0_1->GetZ0(); // -2699.08
	double y0_SiPMArr = 0.; // hit position in the target
	if(-9999. == x0TaHit && !pIn)
		TAPopMsg::Error("TAPID", "GetAoQ: Target hit position not properly assigned");
	// 1050.: z border of the magnetic field.  P_i: (xi,yi,zi): initial point to start RK propogation
	const double zi = -1000., xi = pIn[0]*zi + pIn[2], yi = pIn[1]*zi + pIn[3];
	const double yp0[2] = {pIn[0], pIn[1]}; // magF initial entrance direction
	double y[2] = {xi, yi}; // start of the RK propagation
	double yp[2] = {yp0[0], yp0[1]}; // k1_in and k2_in

	/////////////// COARSE ESTIMATE FOR BETA AFTER THE TARGET ///////////////
	double x_tofwHit = (b1*k_tofw - b_tofw*k1) / (k_tofw - k1);
	double y_tofwHit = (b1 - b_tofw) * k2 / (k_tofw - k1) + b2;
	double z_tofwHit = (b1 - b_tofw) / (k_tofw - k1);
	// P_e(xe, ye, ze): exit point from Mag, treated as at the boundry of the magnetic field
	const double ze = 500., xe = k1*ze + b1, ye = k2*ze + b2;
	const double trkL0 = 0. - z0_T0_1; // from TOF stop to the center of the dipole magnet (the origin)
	const double trkL1 = sqrt(xe*xe + ye*ye + ze*ze); // from the origin to P_e
	const double trkL2 = // from P_e to the hit point of the TOF wall
		sqrt(pow(x_tofwHit-xe, 2.) + pow(y_tofwHit-ye, 2.) + pow(z_tofwHit-ze, 2.));
	const double totalTrackLength_t = trkL0 + trkL1 + trkL2;
	double beta = totalTrackLength_t / tof2 / c0;
	if(beta < 2.) beta = beta < 1. ? beta : 0.9; // toleration for beta error (from tof2 or trkLenT)
	if(beta < 0. || beta >= 1.){
		fIsFlied = true;
		return;
	}
	///////////////////////////////////////////////////////////////////////////

	// particle propagation in uniform magnetic field -- analytic solution exists
	// 2: uniform Mag - point+trk; 3: hybrid of uni-and-nonuni-Mag; 4: uni-Mag - trk+trk
	if(kOpt2 == option || kOpt3 == option || kOpt4 == option){
		const double B = GetExB();
		double dtheta, rho, ki; // ki: x=ki z+bi, track after Target, before Mag
		double zo, xo; // (zo, xo): rotating center of the arc
		if(kOpt2 == option || kOpt3 == option){
			const double zMagIn = -500., zMagOut = ze;
			double result[6]{}; // dtheta: particle defelct angle by mag
			TAMath::UniformMagneticSolution(k1, b1, zMagOut, zMagIn, z0_TA, x0TaHit, result);
			// no eligible solution found
			if(result[0] == result[1] && result[1] == result[2] && result[2] == result[3]){
				fIsFlied = true;
				// every element of result was set to be the number of valid solutions (0, 2, or 3)
//				cout << "\nresult[0]: " << result[0] << endl;
//				cout << "fAoZdmin: " << fAoZdmin << endl; getchar(); // DEBUG
				return;
			}
			dtheta = result[0]; rho = result[1]; ki = result[2];
			zo = result[4]; xo = result[5];
			const double d0 = (zMagIn - z0_T0_1)*sqrt(1.+ki*ki); // from T0_1 to Mag
			const double d1 = rho*dtheta; // arc in the Mag
			const double d2 = trkL2; // from Mag to TOF wall
			fTotalTrackLength = d0 + d1 + d2;
		} // end if
		if(kOpt4 == option){
			if(-9999. == pIn[0] || -9999. == pIn[2] || -9999. == pOut[0] || -9999. == pOut[2]){
				TAPopMsg::Error("TAPID", "Fly: deficient incident XTrk or(and) exit XTrk.\
					\nOption: %d, kin: %f, bin: %f, kout: %f, bOut: %f", option, pIn[0], pIn[2], pOut[0], pOut[2]);
			}
			rho = TAMath::rho(pIn[0], pIn[2], pOut[0], pOut[2], &zo, &xo, fX2Arr);
//			cout << "fX2Arr[0]: " << fX2Arr[0] << "\tfX2Arr[1]: " << fX2Arr[1] << endl; getchar(); // DEBUG
			dtheta = atan(pOut[0]) - atan(pIn[0]); ki = pIn[0];
			double pT0_1[3] = {0., 0., z0_T0_1}, pTaHit[3] = {0., 0., z0_TA};
			double pMagIn[3] = {0., 0., TAMath::kzMagIn}, pMagOut[3] = {0., 0., TAMath::kzMagOut};
			const double pTOFWHit[3] = {x_tofwHit, y_tofwHit, z_tofwHit};
			pT0_1[0] = pIn0[0] * pT0_1[2] + pIn0[2]; // T0_1 hit x
			pT0_1[1] = pIn0[1] * pT0_1[2] + pIn0[3]; // T0_1 hit y
			pTaHit[0] = pIn[0] * pTaHit[2] + pIn[2]; // Target hit x
			pTaHit[1] = pIn[1] * pTaHit[2] + pIn[3]; // Target hit y
			pMagIn[0] = pIn[0] * pMagIn[2] + pIn[2]; // MagIn hit x
			pMagIn[1] = pIn[1] * pMagIn[2] + pIn[3]; // MagIn hit y
			pMagOut[0] = pOut[0] * pMagOut[2] + pOut[2]; // MagIn hit x
			pMagOut[1] = pOut[1] * pMagOut[2] + pOut[3]; // MagIn hit y
			const double trajectoryLength0 = TAMath::L(pT0_1, pTaHit); // from TOFStop to Target
			const double trajectoryLength1 = TAMath::L(pTaHit, pMagIn); // from Target to Mag Field
			const double trajectoryLength2 = rho * dtheta; // arc length in the Mag Field
			const double trajectoryLength3 = TAMath::L(pMagOut, pTOFWHit); // from Mag Field to TOFW
			fTotalTrackLength = trajectoryLength0 + trajectoryLength1 + trajectoryLength2 + trajectoryLength3;
		} // end if(kOpt4 == option)


		fBeta = fTotalTrackLength / tof2 / c0;
		if(fBeta < 0. || fBeta >= 1.){ // bad beta, failed PID
			fBeta = -1.; fTotalTrackLength = -9999.;
			fIsFlied = true;
			return;
		}
		fGamma = TAMath::Gamma(fBeta);
//		cout << "fBeta: " << fBeta << "\tfTotalTrackLength: " << fTotalTrackLength << endl;
//		getchar(); // DEBUG
		// 0.321840605 = e0/(u0*c0*1E6) SI unit
		fAoZ = B * (rho/1000.) * 0.321840605 / (fBeta * fGamma);
		fAoZdmin = 0.;
		fAngleTaOut[0] = atan(ki); fAngleTaOut[1] = 0.;
		fPoZ = B * (rho/1000.) * 0.321840605 * u0MeV; // MeV/c
		fBrho = B * rho / 1000.; // T.m
		fIsFlied = true; // fIsflied should be assigned immediately after flying
//		cout << "fAoZ: " << fAoZ << "\tfBrho: " << fBrho << endl; // DEBUG
//		getchar(); // DEBUG
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
		if(kOpt2 == option || kOpt4 == option) return;
	} // end if(kOpt2 == option || 3 = option)

	// particle propagation in nonuniform magnetic field //
	const double zf = 1000.; // the zmax border of the magnetic field
	double pT0_1[3] = {0., 0., z0_T0_1}, pTaHit[3] = {0., 0., z0_TA};
	double pMagIn[3] = {0., 0., zi}, pMagOut[3] = {0., 0., zf};
	const double pTOFWHit[3] = {x_tofwHit, y_tofwHit, z_tofwHit};
	pT0_1[0] = pIn0[0] * pT0_1[2] + pIn0[2]; // T0_1 hit x
	pT0_1[1] = pIn0[1] * pT0_1[2] + pIn0[3]; // T0_1 hit y
	pTaHit[0] = pIn[0] * pTaHit[2] + pIn[2]; // Target hit x
	pTaHit[1] = pIn[1] * pTaHit[2] + pIn[3]; // Target hit y
	pMagIn[0] = pIn[0] * pMagIn[2] + pIn[2]; // MagIn hit x
	pMagIn[1] = pIn[1] * pMagIn[2] + pIn[3]; // MagIn hit y
	pMagOut[0] = pOut[0] * pMagOut[2] + pOut[2]; // MagIn hit x
	pMagOut[1] = pOut[1] * pMagOut[2] + pOut[3]; // MagIn hit y
	// trajectoryLength0-1-2-3: T0_1->TA; TA->magF; magF->exit of magF; exit of magF->TOFW
	const double trajectoryLength0 = TAMath::L(pT0_1, pTaHit); // from TOFStop to Target
	const double trajectoryLength1 = TAMath::L(pTaHit, pMagIn); // from Target to Mag Field
	const double trajectoryLength3 = TAMath::L(pMagOut, pTOFWHit); // from Mag Field to TOFW
#ifdef DEBUG
	double ddmin[2]{}; // quality estimator
#endif
	double aoz, aozc = 2.0, d2; // aozc: the central aoz; d2: LSM Qsquare
	if(kOpt3 == option){ aozc = fAoZ; }
	const double span0 = 1.5;
	double span = span0; // search scope, aozc-span ~ aozc+span
	int ln = 1, n = 30;
	// loop laps for iter==0, which is to refine beta
	if(kOpt0 == option){ n = 15; }
	if(kOpt3 == option){ span = 0.8; n = 10; } // aoz from uniform magField is precise enough
	for(int iter = 0; iter < 2; iter++){ // iteration to refine beta2
		if(1 == iter){
			// reset search domin, narrow the scope and coodinate the center
			span = span0 / n * 3.;
			n = 10; ln = 2;
			aozc = fAoZ;
			fAoZdmin = 9999.; // reset dmin
			if(kOpt0 == option){ n = 5; ln = 3; }
			if(kOpt3 == option){ n = 5; ln = 3; span = 0.5; }
//			SetIterationStep(h0*0.5);
		}
		// aozc: center of the scope domain
		for(int l = 0; l < ln; l++){ // outer loop
//			cout << "l: " << l << "\tln: " << ln << "\titer: " << iter << endl; // DEBUG
//			cout << "\tspan: " << span << "\taozc: " << aozc << "\tn: " << n << endl; // DEBUG
//			getchar(); // DEBUG
			for(int ll = 0; ll <= n; ll++){ // interior loop
//				cout << "ll: " << ll << "\taoz: " << aoz << endl; // DEBUG
//				getchar(); // DEBUG
				aoz = aozc + (2.*ll/n - 1.)*span;
//				aoz = 2.15; // DEBUG
				if(aoz < 0.03) continue;
				if(fabs(aoz) > 5.) continue;
				// initialize the RK propagation
				SetOutOfRangeError(false);
				SetQoP(aoz, beta);
				y[0] = xi; y[1] = yi;
				yp[0] = yp0[0]; yp[1] = yp0[1];

				TransportIon(y, yp, zi, zf);

				if(IsOutOfRange()) continue; // ineligible aoz
				double dd[2]{}, dp[2]{}; // dd: slope or position deviation; dp: position deviation
				if(-9999!= x0TaHit){ // pion experiment pid mode
					dd[0] = y[0] - x0TaHit; dd[1] = y[1] - y0_SiPMArr;
				}
				else if(pIn){ // pIn[0-3]: {k1_Ta, k2_Ta, b1_Ta, b2_Ta}, TWO-TRACK mode
					dd[0] = yp[0] - pOut[0]; // x trk
					if(0. != pIn[1] && 0. != pOut[1]) dd[1] = yp[1] - pOut[1]; // y trk
					else dd[1] = 0.; // y trk not known, so it contributes nothing to aoz estimator
				}
				else TAPopMsg::Error("TAPID", "Fly: neither pion mode nor TWO-TRACK mode ...");
				d2 = sqrt(dd[0]*dd[0] + dd[1]*dd[1]);

				// use track position information -- EXPERIMENTAL //
				if(pIn){
					dp[0] = y[0] - pOut[0]*zf + pOut[2]; dp[1] = y[1] - pOut[1]*zf + pOut[3];
					if(0. == pOut[1]) dp[1] = 0.; // y trk not assigned
					fX2Arr[0] = dp[0]; fX2Arr[1] = dp[1];
//					dx2 = sqrt(dp[0]*dp[0]+dp[1]*dp[1]); // XXX still experimental XXX
				}
				if(d2 < fAoZdmin){
					fAoZdmin = d2; fAoZ = aoz; fBeta = beta;

					/////////////// CALCULATE TOTAL TRACK LENGTH ////////////////
					const double trajectoryLength2 = GetTrackLength(); // arc length in the Mag Field
					fTotalTrackLength = trajectoryLength0 + trajectoryLength1 + trajectoryLength2 + trajectoryLength3;
					memcpy(fAngleTaOut, yp, sizeof(fAngleTaOut));
					//////////////////////////////////////////////////////////////
#ifdef DEBUG
					ddmin[0] = dd[0]; ddmin[1] = dd[1];
					cout << "fAoZdmin: " << fAoZdmin << "\tdd[0]: " << dd[0] << "\tdd[1]: "; // DEBUG
					cout << dd[1] << "\tfAoZ: " << fAoZ << endl; // DEBUG

					cout << "dd[0]*dd[0] + dd[1]*dd[1]: " << dd[0]*dd[0] + dd[1]*dd[1] << endl; // DEBUG
					cout << "dp[0]*dp[0]+dp[1]*dp[1]: " << dp[0]*dp[0]+dp[1]*dp[1] << endl; // DEBUG
					cout << "d2: " << d2 << endl; // DEBUG

					getchar(); // DEBUG
#endif
				} // end if
				if(fAoZdmin < 1E-8) break;
			} // end for over ll
			if(-9999. == fAoZ){ // failed, possibly fake track caused by chaos
				fAoZdmin = 9999.; break;
			}
#ifdef DEBUG
			cout << "\033[31;1m______________l:" << l << "___________________\033[0m\n" << endl; // DEBUG
#endif
			span = span / n;
			aozc = fAoZ;
			if(fAoZdmin < 1E-8) break;
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
//		cout << "tof2: " << tof2 << "\tfTotalTrackLength: " << fTotalTrackLength << endl; // DEUBG
//		cout << "beta: " << beta << endl; getchar(); // DEBUG
		return;
	}
	if(9999. == fAoZdmin){
		return; // PID failure
	}
	fGamma = TAMath::Gamma(fBeta);
	fPoZ = fAoZ * fBeta * fGamma * u0MeV; // MeV/c
	fBrho = fPoZ / (0.321840605 * u0MeV);

#ifdef DEBUG
	cout << "___________________________________________________________\n";
	cout << "fBeta: " << fBeta << endl; // DEBUG
	cout << "fBrho: " << fBrho << endl; // DEBUG
	cout << "ddmin[0]: " << ddmin[0] << "\tddmin[1]: " << ddmin[1] << endl; // DEBUG
	cout << "fAoZ: " << fAoZ << endl; // DEBUG
	cout << "fAoZdmin: " << fAoZdmin << endl; // DEBUG
	cout << "tof2: " << tof2 << endl; // DEBUG
	cout << "fTotalTrackLength: " << fTotalTrackLength << endl; // DEBUG
	getchar(); // DEBUG
#endif

	if(fGCurve && -9999. != fAoZ){
		SetIterationStep(h0/10); SetStepErrorTolerance(stepError/10); // increase RK precision //
		// initialize the RK propagation
		SetOutOfRangeError(false);
		SetQoP(fAoZ, fBeta);
		y[0] = xi; y[1] = yi;
		yp[0] = yp0[0]; yp[1] = yp0[1];
		TransportIon(y, yp, zi, zf, true);
		SetIterationStep(h0); SetStepErrorTolerance(stepError); // recover RK precision //
//		FillGraphTrajectory();
	}

	if(kOpt0 == option){
		// recover precision level setttings
		SetIterationStep(h0); SetStepErrorTolerance(stepError);
	}
} // end member method Fly

void TAPID::FillGraphTrajectory() const{
	if(!fGCurve) return;
	const vector<tra_t> &trackVec = GetTrackVec();
	if(0 == trackVec.size()) return;
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
double TAPID::GetBrho() const{
	if(!IsFlied()) TAPopMsg::Error("TAPID", "GetBrho: Particle Not Flied");
	return fBrho;
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
	SetStepErrorTolerance(5E0); // truncation error for RK method
	EnableEnergyLoss(false); // calculate energy loss in the magnetic field
	// kClassic, kNystrom. kNystrom is preferrable, well kClassic is considered normal and safe
	SetRKMethod(kNystrom);

	// assign TOF wall pionters
	TAParaManager::ArrDet_t &dec_vec = TAParaManager::Instance()->GetDetList();
	if(!dec_vec[3] && !dec_vec[4])
		TAPopMsg::Error("TAPID", "Configure: MWDC arrays in paraManger are null. Consider to put\
 this TOFWall pointer assignment in TAPID::Configure(), and put TAPID::Configure in the last\
 of TAEventProcessor::Configure to lick the problem");
	if(!dec_vec[1])
		TAPopMsg::Error("TAPID", "Configure: T0_1 in paraManager is null. Note that detector construction\
 should be implemented prior to the current function in TAEventProcessor::Configure");

	// assign TOFWall and T0_1
	TAMWDCArray *dcArr[2]{0};
	dcArr[0] = (TAMWDCArray*)dec_vec[3];
	if(dcArr[0]) fTOFWall[0] = dcArr[0]->GetTOFWall();
	dcArr[1] = (TAMWDCArray*)dec_vec[4];
	if(dcArr[1]) fTOFWall[1] = dcArr[1]->GetTOFWall();
	fT0_1 = (TAT0_1*)dec_vec[1];

	p0[0] = 0.; p0[1] = 0.; p0[2] = 0.;
	GetMagneticIntensity(B0, p0); B0m = TAMath::norm(B0);
//	printf("Configure: B(%f,%f,%f): |(%f, %f, %f)| = %f", p0[0], p0[1], p0[2], B0[0], B0[1], B0[2], B0m); getchar(); // DEBUG
	TAPopMsg::ConfigInfo("TAPID", "Configure: magFile: %s", magFile.c_str());
	TAPopMsg::ConfigInfo("TAPID", "Configure: B(0,0,0): |(%f, %f, %f)| = %f", B0[0], B0[1], B0[2], B0m); // DEBUG
}

void TAPID::GetX2Arr(double *x2) const{
	if(!x2) return;
	if(!IsFlied()) TAPopMsg::Error("TAPID", "GetX2: Particle Not Flied");
	memcpy(x2, fX2Arr, sizeof(fX2Arr));
}

void TAPID::Initialize(){
	TAMagnet::Initialize();
//	if(!IsFlied()) return;
	fAoZ = -9999.; fAoZdmin = 9999.;
	fBeta = -1.; fGamma = -1.; fPoZ = -9999.; fBrho = -9999.;
	fAngleTaOut[0] = -9999.; fAngleTaOut[1] = -9999.;
	fTotalTrackLength = -9999; fIsFlied = false;
	fX2Arr[0] = -9999.; fX2Arr[1] = -9999.;
}





