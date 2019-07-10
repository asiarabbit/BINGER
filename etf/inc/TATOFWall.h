/**
	\file TATOFWall.h
	\class TATOFWall
	\brief TOF wall class. A container class, accommodating plastic scintillator
	strips and choosing fired strip for a given track.
	for the DC array upstream of the target and upstream of the magnet.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/9/30 Last revised: 2018/4/13, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TATOFWALL_H_
#define _TATOFWALL_H_

#include <vector>
#include "TADetector.h"

using std::vector;

class TAPlaStrip;
class TAChannel;

class TATOFWall : public TADetector{
public:
	TATOFWall(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TATOFWall();
	
	TAPlaStrip *GetStrip(int stripId) const;
	vector<TAPlaStrip *> &GetStripArr(){ return fStripArr; }
	/// assign the input array with the list of ids of fired strips
	void GetFiredStripArr(int &multi, int *idLs, short *staLs = nullptr, double *uvlTLs = nullptr, double *dvlTLs = nullptr) const; ///< sta == 4 is mandatory
	unsigned short GetNFiredStrip() const; ///< \return count of strips with fired status being 4
	/// \return number of strips on aggregate
	unsigned short GetNStrip() const{ return fStripArr.size(); }
	double GetStripTime(int i, double t0 = -9999., double t1 = -9999., double t2 = -9999.) const;
	/// Get TOF time from strip specified by a track with linear parameter kl and bl
	/// \param firedStripId: serial id of the fired strip for the track specifically
	/// \return If the supposed fired strip is not fired, return -9999.
	/// t0, t1 and t2 are set for choosing time over edges
	/// (time-t0) within t1 and t2 is chosen\n
	/// t0, t1 and t2 using default values, choose the 1st edge
	double GetTime(double kl, double bl, double &nstripsStray, int &firedStripId, double t0 = -9999., double t1 = -9999., double t2 = -9999.) const;
	virtual void AssignStripPosition();
	virtual void GetStripProjection(int serialId, double *p) const;
	double Acceptance() const; ///< detector acceptance
	double GetDelayAvrg() const;
	double GetDelay(int stripId) const;
	void SetDelayAvrg(double delay) { fDelayAvrg = delay; }
	void SetNStrip(int nstrip);

	virtual void Configure();
	virtual void Initialize() override;
	virtual void Info() const;
protected:
	vector<TAPlaStrip *> fStripArr; ///< 30 plastic scintillator strips
	int fNStrip; ///< number of plastic strips in the TOF Wall
	double fDelayAvrg; ///< base delay for every strip
};

#endif
