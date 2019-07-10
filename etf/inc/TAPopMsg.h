/**
	\file TAPopMsg.h
	\class TAPopMsg
	\brief Print prompt message - warnings, debuggings, errors and plain message.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/9/24 Last revised: 2017/11/26, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


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
	static void ShowPiont(const char *msg, const double *p, const int len = 3);

	static void Verbose(bool opt = true){ fIsVerbose = opt; }
	static bool IsVerbose() { return fIsVerbose; }
	static void Silent(bool opt = true){ fIsSilent = opt; }
	static bool IsSilent() { return fIsSilent; }
	static void SetDebug(bool opt = true){ fIsDebug = opt; }
	static bool IsDebug() { return fIsDebug; }

	/// \param isName (true): no comma in the returned string, suitable for path and file name
	static const char *time0(bool isName = false); ///< current time year-month-day_hour-Min
protected:
	static bool fIsVerbose; ///< to switch ConfigInfo() method
	static bool fIsSilent; ///< to switch Info() method
	static bool fIsDebug; ///< to switch Debug() method
};

#endif
