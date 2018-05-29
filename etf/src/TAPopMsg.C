///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAPopMsg.C													     //
//   TAPopMsg.C -- source file for class TAPopMsg								     //
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

#include <iostream> // getline
#include <ctime>
#include <stdarg.h> // variable arguments handle function declarations.
#include <cstdlib>
#include "TAPopMsg.h"

using std::cout;

// assignment and definition
bool TAPopMsg::fIsVerbose = true;
bool TAPopMsg::fIsSilent = false;
bool TAPopMsg::fIsDebug = true;

void TAPopMsg::Info(const char *cname, const char *fmt, ...){
	if(fIsSilent) return;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	char msg[1024], omsg[1024];
	sprintf(msg, "  BINGER-Info: \033[32;1m[%s]\033[33;1m %s\n\033[0m", cname, fmt);
	vsprintf(omsg, msg, arg_ptr);
	va_end(arg_ptr);
	cout << omsg;
}

void TAPopMsg::Error(const char *cname, const char *fmt, ...){
	static bool nomore = false;
	if(nomore) return;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	char msg[1024], omsg[1024];
	sprintf(msg, "\033[31;1m  BINGER-Error: \033[0m\033[33;1m[%s]\033[0m\033[31;1m %s\n\033[0m", cname, fmt);
	vsprintf(omsg, msg, arg_ptr);
	va_end(arg_ptr);
	cout << omsg;

	// select repeatability
	if(!nomore){
		cout << "[q]: abort, [n]: do not stop any more, others: continue > " << std::flush;
		std::string str;
		std::getline(std::cin, str);
		if(!str.empty() && (str.c_str())[0] == 'q'){
			cout << "Aborting BINGER...\n";
			exit(1);
		}
		else if(!str.empty() && (str.c_str())[0] == 'n'){
			nomore = true;
		}
		cout << std::endl;
	}
}

void TAPopMsg::Warn(const char *cname, const char *fmt, ...){
	static bool nomore = false;
	if(nomore) return;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	char msg[1024], omsg[1024];
	sprintf(msg, "\033[36;1m  BINGER-Warning: \033[0m\033[33;1m[%s]\033[0m\033[36;1m %s\n\033[0m", cname, fmt);
	vsprintf(omsg, msg, arg_ptr);
	va_end(arg_ptr);
	cout << omsg;

	// select repeatability
	if(!nomore){
		cout << "[q]: abort, [n]: do not stop any more, others: continue > " << std::flush;
		std::string str;
		std::getline(std::cin, str);
		if(!str.empty() && (str.c_str())[0] == 'q'){
			cout << "Aborting BINGER...\n";
			exit(1);
		}
		else if(!str.empty() && (str.c_str())[0] == 'n'){
			nomore = true;
		}
		cout << std::endl;
	}
}

void TAPopMsg::Debug(const char *cname, const char *fmt, ...){
	if(!fIsDebug) return;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	char msg[1024], omsg[1024];
	sprintf(msg, "  BINGER-Debug: \033[33;1m[%s]\033[0m %s\n", cname, fmt);
	vsprintf(omsg, msg, arg_ptr);
	va_end(arg_ptr);
	cout << omsg;

	cout << "Press ENTER to continue...";
	getchar();
}

void TAPopMsg::ConfigInfo(const char *cname, const char *fmt, ...){
	if(!fIsVerbose) return;

	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	char msg[1024], omsg[1024];
	sprintf(msg, "\033[32;1m  BINGER-ConfigInfo: \033[0m\033[33;1m[%s]\033[0m\033[32;1m %s\n\033[0m", cname, fmt);
	vsprintf(omsg, msg, arg_ptr);
	va_end(arg_ptr);
	cout << omsg;
}
void TAPopMsg::ShowPiont(const char *msg, const double *p, const int len){
	for(int i = 0; i < len; i++)
		cout << msg << "[" << i << "]: " << p[i] << "\t";
}

// isName: no comma in the returned string, suitable for path and file name
const char *TAPopMsg::time0(bool isName){
	time_t tt = ::time(NULL);
	tm *t = localtime(&tt);
	static char ttt[128];
	if(!isName) sprintf(ttt, "%d-%02d-%02d_%02d:%02d", t->tm_year + 1900,
	 t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
	else sprintf(ttt, "%d%02d%02d_%02d%02d", t->tm_year + 1900,
	 t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
	return ttt;
}




