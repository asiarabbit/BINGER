///////////////////////////////////////////////////////////////////////////////////////
// Theoretical Analysis Code for the External Target Facility, HIRFL-CSR, @IMP		 //
//																				     //
// BINGER/inc/TACS.C																 //
//   TACS.C -- source file for class TACS											 //
//   Introduction: to calculate reaction cross sections of direct reactions at high  //
// energies (several hundred MeV).													 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/11/26.															     //
// Last modified: 2018/11/28, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2018-2019, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "TACS.h"

using std::cout;
using std::endl;

static const double Pi = 3.14159265359;
static const double amu = 931.494; // atomic mass unit; unit: MeV
static const double hbarc = 197.327; // unit: MeV * fm
static const double e2 = 1.44; // unit: MeV * fm
static const double ALPHA = 1./137.036; // fine structure constant: e2/hbarc

TACS::TACS(int ZP, int NP, int ZT, int NT, double beta){
	fZP = ZP; fNP = NP; // projectile identification
	fZT = ZT; fNT = NT; // target identification
	fAP = fZP + fNP; fAT = fZT + fNT; // mass number
	fIPTRe = -9999.; fIPTIm = -9999.; fIPPT = -9999.; fIPTNo = -9999.;
	fBmax = 100.; // unit: fm
	fStep = 1e-4; // integration step for impact parameter, unit: fm

	fAlphaNN = -9999.;
	if(-1. == beta){
		fBeta_ = beta;
		fIncidentBeamEnergy = -9999.;
		fK = fBeta_ * fAP*fAT/(fAP+fAT) * amu/hbarc; // unit: fm
		fEta = fZP*fZT/fBeta_ * sqrt(2.)*ALPHA;
	} // end if
	else SetBeta(beta);
	
	fIsIntegrated = false;
} // end of the constructor

TACS::~TACS(){}

void TACS::SetBeta(double beta){
	// beta validity check //
	if(beta >= 1. || beta <= 0.){
		cout << "TACS::NNCS(): " << "The assignment of beta is improper." << endl;
		cout << "beta: " << beta << endl;
		cout << "Exiting...";
		getchar();
		exit(1);
	} // end if
	
	fBeta_ = beta;
	fIncidentBeamEnergy = (1./sqrt(1-fBeta_*fBeta_) - 1.) * amu;
	fK = fBeta_ * fAP*fAT/(fAP+fAT) * amu/hbarc; // unit: fm
	fEta = fZP*fZT/fBeta_ * sqrt(2.)*ALPHA;

	fIsIntegrated = false;
} // end of member function SetBeta

void TACS::SetIncidentBeamEnergy(double energy){ // in MeV/u
	// input energy validity check //
	if(energy <= 0){
		cout << "SetIncidentBeamEnergy: Abnormal value of input energy: ";
		cout << energy; getchar();
		exit(1);
	}

	const double x = 1. + energy/amu;
	fBeta_ = sqrt(x*x-1.) / x;
	fIncidentBeamEnergy = energy;
	fK = fBeta_ * fAP*fAT/(fAP+fAT) * amu/hbarc; // unit: fm
	fEta = fZP*fZT/fBeta_ * sqrt(2.)*ALPHA;

	fIsIntegrated = false;
}

void TACS::SetMaxImpactParameter(double bmax){
	if(bmax <= 0){
		cout << "TACS::SetMaxImpactParameter: abnormal input, bmax is " << bmax << endl;
		cout << "Exiting...";
		exit(1);
	} // end if
	fBmax = bmax;

	fIsIntegrated = false;
} // end of member function SetMaxImpactParameter

// total reaction cross section: el+inel
double TACS::TotalCS(){
	Initialize();

	return 2*Pi*2*(pow(fBmax, 2)/2. - fIPTRe) * 10.; // unit: mb
} // end of member function TotalCS

// elastic cross section
double TACS::ElasticCS(){
	Initialize();
	
	return 2*Pi*(pow(fBmax, 2)/2. - 2*fIPTRe + fIPTNo) * 10.; // unit: mb
}
// reaction cross section: non-elastic cross section
double TACS::ReactionCS(){
	// TotalCS() - ElasticCS();
	return 2*Pi*(pow(fBmax, 2)/2. - fIPTNo) * 10.; // unit: mb
}
// interaction cross section: nucleon removal from incident nuclei
// here it is calculated as C.S. that projectile not in ground state
double TACS::InteractionCS(){
	Initialize();

	return 2*Pi*(pow(fBmax, 2)/2. - fIPPT) * 10.; // unit: mb
}

void TACS::ReactionInfo(){
	cout << fIncidentBeamEnergy << "MeV/u" << " (" << fZP << ", " << fNP << ")";
	cout << " -> " << "(" << fZT << ", " << fNT << ")" << endl;

	cout << "cs_tot: " << TotalCS() << endl;
	cout << "cs_el: " << ElasticCS() << endl;
	cout << "cs_rec: " << ReactionCS() << endl;
	cout << "cs_int: " << InteractionCS() << endl;
} // end of member function ReactionInfo

void TACS::SetProjectileNucleonDensity(int len, double *alpha, double *miu){
	if(!len){
		cout << "TACS::SetProjectileNucleonDensity: Input array length is zero, exiting...";
		exit(1);
	}
	for(int i = 0; i < len; i++){
		fAlpha.push_back(alpha[i]);
		fMiu.push_back(miu[i]);
	} // end for over i
} // end of member function SetProjectileNucleonDensity
void TACS::SetTargetNucleonDensity(int len, double *beta, double *niu){
	if(!len){
		cout << "TACS::SetTargetNucleonDensity: Input array length is zero, exiting...";
		exit(1);
	}
	for(int i = 0; i < len; i++){
		fBeta.push_back(beta[i]);
		fNiu.push_back(niu[i]);
	} // end for over i
	
} // end of member function SetTargetNucleonDensity

inline double simpson(double *x){
	return (x[0]+4.*x[1]+x[2])/6.;
}

inline double cotes(double *x){
	return (7.*x[0]+32.*x[1]+12.*x[2]+32.*x[3]+7.*x[4])/90.;
}

// implement the numerical integration -- the most time-consuming one //
void TACS::Initialize(){
	if(fIsIntegrated) return;

	if(-9999. == fAlphaNN){
		cout << "fAlphaNN not set, exiting..." << endl;
		exit(1);
	}
	const int miuN = fMiu.size(), niuN = fNiu.size();
	if(!miuN || !niuN){
		cout << "Target or projectile nucleon density distribution is absent." << endl;
		cout << "Exiting...";
		getchar();
		exit(1);
	}

	const double nncs = NNCS(); // NN collision cross section
	// exponents and denominators
	double **expoPT = new double*[miuN], **denoPT = new double*[miuN];
	double ***expoPPT = new double**[miuN], ***denoPPT = new double**[miuN];
	double ***denoPPTex = new double**[miuN];
	for(int i = 0; i < miuN; i++){
		expoPT[i] = new double[niuN];
		denoPT[i] = new double[niuN];

		expoPPT[i] = new double*[miuN];
		denoPPT[i] = new double*[miuN];
		denoPPTex[i] = new double*[miuN];
		for(int j = 0; j < miuN; j++){
			expoPPT[i][j] = new double[niuN];
			denoPPT[i][j] = new double[niuN];
			denoPPTex[i][j] = new double[niuN];
		} // end for over j
	} // end for over i

	// calculate the general constants preemptively //
	for(int i = 0; i < miuN; i++){ // loop over Gauss of projectile density func.
		for(int k = 0; k < niuN; k++){ // loop over Gauss of target density func.
			const double a = pow(fAlpha[i], 2), b = pow(fBeta[k], 2), r = pow(fR0, 2);
			// preparation for IPT calculation //
			expoPT[i][k] = fMiu[i] * fNiu[k] * pow(fAlpha[i], 3) * pow(fBeta[k], 3);
			denoPT[i][k] = a + b + r;
			expoPT[i][k] *= pow(Pi, 2) * nncs / (2. * denoPT[i][k]);
			
			// preparation for IPPT calculation //
			for(int j = 0; j < miuN; j++){
				const double aj = pow(fAlpha[j], 2);
				expoPPT[i][j][k] = fMiu[i] * fMiu[j] * fNiu[k] *
	pow(fAlpha[i], 3) * pow(fAlpha[j], 3) * pow(fBeta[k], 3);
				denoPPT[i][j][k] = ((a+r)*(aj+r) + b*(a+aj+2*r));
				denoPPTex[i][j][k] = denoPPT[i][j][k]/(a+aj+2*r);
				expoPPT[i][j][k] *=
					pow(Pi, 2.5) * pow(nncs, 2) / (4. * denoPPT[i][j][k] * fAP);
				expoPPT[i][j][k] *= 1. + pow(fAlphaNN, 2);
				// XXX XXX XXX FIXME TODO XXX XXX XXX
//				expoPPT *= AP*AT; // according to my derivation result
			} // end for over projectile gaus
		} // end for over projectile gaus
	} // end for over target gaus
	
	// start the numerical integration //
	const double db = fStep;
	const int n = int(fBmax/fStep); // numerical integral, n is even
	cout << "################### TACS::Initialize: INFO ##################" << endl;
	cout << "Integration Step: " << fStep << endl;
	cout << "Integration times: " << n << endl;
	cout << "Integration range: " << "(0, " << fBmax << ")fm" << endl;
	if(n < 1000){
		cout << "TACS::Initialize: Integration times is wrong: n is " << n << endl;
		cout << "Exiting...";
		exit(1);
	}
	const double eta = fEta; // fEta 0;
	fIPTRe = 0.;  fIPTIm = 0.; fIPTNo = -0.; fIPPT = 0.;
	for(int l = 0; l <= n; l++){ // the numerical integral loop
		const double b0 = db*l; // 0~fBmax

		// interpolation in b and b+l //
		const int nn = 4;
		double IPTRe[16]{}, IPTIm[16]{}, IPTNo[16]{}, IPPT[16]{};
		for(int ll = 0; ll <= nn; ll++){
			const double b = b0+db*ll/nn;
			// Coulomb correction
			const double bp = eta/fK + sqrt(pow(eta/fK, 2) + pow(b, 2));
			double expoPTSum = 0., expoPPTSum = 0.;
			for(int i = 0; i < miuN; i++){
				for(int k = 0; k < niuN; k++){
					expoPTSum += expoPT[i][k] * exp(-pow(bp, 2) / denoPT[i][k]);
					for(int j = 0; j < miuN; j++){
						expoPPTSum +=
							expoPPT[i][j][k] * exp(-pow(bp, 2) / denoPPTex[i][j][k]);
					} // end for over j
				} // end for over k
			} // end for over i
			IPTRe[ll] += exp(-expoPTSum) * cos(expoPTSum*fAlphaNN) * b;
			IPTIm[ll] += exp(-expoPTSum) * sin(expoPTSum*fAlphaNN) * b;
			IPTNo[ll] += exp(-2*expoPTSum) * b;
			IPPT[ll] += exp(-2*expoPTSum + expoPPTSum) * b;
		} // end loop over ll
		
		fIPTRe += cotes(IPTRe); fIPTIm += cotes(IPTIm);
		fIPTNo += cotes(IPTNo); fIPPT += cotes(IPPT);
	} // end for over l
	fIPTRe *= db; fIPTIm *= db;
	fIPTNo *= db; fIPPT *= db;

	fIsIntegrated = true;
} // end of member function Initialize

// nucleon-nucleon scattering cross section //
double TACS::nnCS(double beta){
	return 13.73 - 15.04/beta + 8.76/pow(beta, 2) + 68.67*beta;
}
double TACS::ppCS(double beta){
	return 13.73 - 15.04/beta + 8.76/pow(beta, 2) + 68.67*beta;
}
double TACS::npCS(double beta){
	return -70.67 - 18.18/beta + 25.26/pow(beta, 2) + 113.85*beta;
}
// average over isospin
double TACS::NNCS(int ZP, int NP, int ZT, int NT, double beta){
	return (NP*NT*nnCS(beta)+ZP*ZT*ppCS(beta)+(NP*ZT+NT*ZP)*npCS(beta))/((ZP+NP)*(ZT+NT));
}
double TACS::NNCS(){
	// fBeta_ validity check //
	if(fBeta_ >= 1. || fBeta_ <= 0.){
		cout << "TACS::NNCS(): " << "The assignment of fBeta_ is improper." << endl;
		cout << "fBeta_: " << fBeta_;
		if(-1. == fBeta_) cout << "fBeta_ not assigned?" << endl;
		getchar();
		exit(1);
	} // end if

	return NNCS(fZP, fNP, fZT, fNT, fBeta_);
} // end of member function NNCS



