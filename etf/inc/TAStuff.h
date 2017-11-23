///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAStuff.h														     //
//   TAStuff.h -- header file for class TAStuff									     //
//   Introduction: This is a top base class supposed to be inherited by all classes  //
// in the code project, providing UID and interpretation of UID, togehter with name  //
// and title for object identification in the code.								     //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created time: 2017/9/24.														     //
// Last modified: 2017/10/13, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TASTUFF_H_
#define _TASTUFF_H_

#include <string>

// for debugging purposeses
#include <iostream>
#include <cstdlib>
using std::cout;
using std::endl;

using std::string;

class TAChPara;
class TAChData;

// abstract base class
class TAStuff{
public:
	TAStuff(const string &name = "", const string &title = "", unsigned uid = 999999999)
		 : fName(name), fTitle(title), fUID(uid){
	} // end of constructor
	virtual ~TAStuff() = 0; // the destructor

	// Get functions
	const string &GetName() const{ return fName; }
	const string &GetTitle() const{ return fTitle; }
	unsigned GetUID() const;

	// Set functions
	void SetName(const string &name){ fName = name; }
	void SetName(const char *name){ fName = name; }
	void AppendName(const string &tail){ fName += tail; }
	void AppendName(const char *tail){ fName += tail; }
	void SetUID(unsigned uid){ fUID = uid; }

	void SetTitle(const string &title){ fTitle = title; }
	void SetTitle(const char *title){ fTitle = title; }
	void AppendTitle(const string &tail){ fTitle += tail; }
	void AppendTitle(const char *tail){ fTitle += tail; }

	// to mark the shared part of TAChannel and TAAnode
	virtual TAChPara *GetPara() const;
	virtual TAChData *GetData() const;
protected:
	string fName;
	string fTitle;
	unsigned fUID;
};

#endif






