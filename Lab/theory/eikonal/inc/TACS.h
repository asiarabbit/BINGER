///////////////////////////////////////////////////////////////////////////////////////
// Theoretical Analysis Code for the External Target Facility, HIRFL-CSR, @IMP		 //
//																				     //
// BINGER/inc/TACS.h																 //
//   TACS.h -- header file for class TACS											 //
//   Introduction: to calculate reaction cross sections of direct reactions at high  //
// energies (several hundred MeV).													 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2018/11/26.															     //
// Last modified: 2018/11/26, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2018-2019, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef TACS_H_
#define TACS_H_

#include <vector>

using std::vector;

class TACS{
public:
	TACS(int ZP, int NP, int ZT, int NT, double beta = -1.);
	virtual ~TACS();

	void SetBeta(double beta); // set fBeta
	void SetIncidentBeamEnergy(double energy); // set fBeta with beam energy, unit: MeV/u
	void SetAlphaNN(double alphaNN){ fAlphaNN = alphaNN; }
	double TotalCS(); // total reaction cross section
	double ElasticCS(); // elastic cross section
	double ReactionCS(); // reaction cross section: non-elastic cross section
	double InteractionCS(); // interaction cross section: nucleon removal from incident nuclei
	void ReactionInfo(); // projectile+energy+target
	void SetProjectileNucleonDensity(int len, double *alpha, double *miu);
	void SetTargetNucleonDensity(int len, double *beta, double *niu);
	void SetR0(double r0){ fR0 = r0; }
	void SetIntegrationStep(double step){ fStep = step; }
	void SetMaxImpactParameter(double bmax);

private:
	void Initialize(); // implement the numerical integration

	static double nnCS(double beta); // neutron-neutron scattering cross section
	static double ppCS(double beta); // proton-proton scattering cross section
	static double npCS(double beta); // neutron-proton scattering cross section
	// nucleon-nucleon scattering cross section: average over isospin
	// N(Z): neutron(proton) number; P, T: projectile, target; beta: v/c
	static double NNCS(int ZP, int NP, int ZT, int NT, double beta);
	double NNCS();

	// projectile and target identification, USER INPUT
	int fZP, fNP, fZT, fNT, fAP, fAT;
	double fK; // wave number: khbar = miu*v
	double fEta; // Sommerfield constant for Coulomb correction, ZP*ZT/beta * sqrt(2)*alpha;
	double fBeta_, fIncidentBeamEnergy; // reaction energy
	double fAlphaNN; // Re/Im of f_{NN}(0), the scattering amplitude at 0 angle
	double fR0; // finite-range correction
	double fBmax; // the integral range: 0~fBmax
	// nucleon density distribution function, USER INPUT
	vector<double> fMiu, fAlpha; // rho_P(vec_r) = sum_i miu_i*exp(-(r/alpha_i)^2)
	vector<double> fNiu, fBeta; // rho_T(vec_eta) = sum_k niu_k*exp(-(eta/beta_k)^2)

	// intermediate results //
	double fIPTRe, fIPTIm, fIPTNo; // integral for integrand exp(i*chi_PT), no: norm
	// integral for integrand exp(ichi_PT-ichi_PT^* + chi_PPT)
	double fIPPT;
	double fStep; // integration step
	
	bool fIsIntegrated; // if initialize() is called
};

#endif
