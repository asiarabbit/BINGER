///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPopMsg.h													     //
//   TAPopMsg.h -- header file for class TAPopMsg								     //
//   Introduction: print prompt message - warnings, debuggings, errors and plain	 //
// message.																			 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/24.															     //
// Last modified: 2017/10/24, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAPOPMSG_H_
#define _TAPOPMSG_H_

class TAPopMsg{
public:
	TAPopMsg() {};
	~TAPopMsg() {};
	static void Info(const char *cname, const char *fmt, ...);
	static void Error(const char *cname, const char *fmt, ...);
	static void Warn(const char *cname, const char *fmt, ...);
	static void Debug(const char *cname, const char *fmt, ...);
	static void ConfigInfo(const char *cname, const char *fmt, ...);

	static void Verbose(bool opt = true){ fIsVerbose = opt; }
	static bool IsVerbose() { return fIsVerbose; }
	static void Silent(bool opt = true){ fIsSilent = opt; }
	static bool IsSilent() { return fIsSilent; }
	static void SetDebug(bool opt = true){ fIsDebug = opt; }
	static bool IsDebug() { return fIsDebug; }

	static const char *time0(); // current time year-month-day_hour-Min
protected:
	static bool fIsVerbose; // to switch ConfigInfo() method
	static bool fIsSilent; // to switch Info() method
	static bool fIsDebug; // to swithc Debug() method
};

#endif
