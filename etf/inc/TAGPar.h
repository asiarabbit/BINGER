///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAGPar.h															 //
//   TAGPar.h -- header file for class TAGPar										 //
//   Introduction: class to store global parameters. Each parameter would be a		 //
// TAParameter object.																 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/12/12.															     //
// Last modified: 2017/12/12, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAGPAR_H_
#define _TAGPAR_H_

#include <array>

using std::array;

class TAParameter;

class TAGPar{
public:
	static TAGPar *Instance();
	virtual ~TAGPar();

	double Val(unsigned int id) const;
	const TAParameter *Par(unsigned int id) const;
	void ShowPar(unsigned int id) const;
protected:
	TAGPar();
	static TAGPar *fInstance;
	static constexpr int kSIZE = 1000;
	array<TAParameter *, kSIZE> fParVec;
};

#endif
