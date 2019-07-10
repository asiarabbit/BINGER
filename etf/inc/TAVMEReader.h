/**
	\file TAVMEReader.h
	\class TAVMEReader
	\brief read binary data file from VME Daq and transform it into root tree.
	\deprecated This class is for an old experiments carried out in 2016 and should be deprecated.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/1/9 Last revised: 2018/1/10, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAVMEREADER_H_
#define _TAVMEREADER_H_

#include <string>

using std::string;

class TAVMEReader{
public:
	TAVMEReader(const string &datafile = "");
	virtual ~TAVMEReader();
	virtual void ReadVME();
	void SetDataFile(const string &datafile);
	/// match vme and PXI data tree event by event
	virtual void Match(const string &PXIROOTFile);
	static void ReadVME(const string &vmeDataFile, const string &vmerootfile);
	static void Match(const string &PXIROOTFile, const string &VMEROOTFile);
private:
	string fDataFile;
	string fROOTFile;
};

#endif
