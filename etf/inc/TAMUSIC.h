/**
	\file TAMUSIC.h
	\class TAMUSIC
	\brief A class for multi-sampleing ionization chamber. It is for the
	measurement of particle charge information.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/4/22 Last revised: 2018/4/26, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAMUSIC_H_
#define _TAMUSIC_H_

#include <vector>

#include "TAStuff.h"
#include "TADetUnion.h"

using std::vector;

class TAChannel;

class TAMUSIC : public TAStuff, public TADetUnion{
public:
	TAMUSIC(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMUSIC() = 0;

	void SetBeta(const double beta);
	double GetBeta() const;
	int GetNChannel() const;
	const vector<TAChannel *> &GetChArr() const{ return fChArr; }
	double GetPileUp() const; ///< return the value of pileUp channel
	bool IsPileUp() const{ return fIsPileUp; }
	void SetPileUp(bool opt) { fIsPileUp = opt; }
	bool GetFiredStatus() const;
	int GetNFiredChannel() const;
	virtual double GetDeltaE() = 0;
	virtual double GetZ() = 0; ///< particle charge
	
	virtual void Initialize();
	virtual TAStuff *GetChannel(unsigned uid) const;
protected:
	vector<TAChannel *> fChArr;
	double fBeta; ///< particle velocity
	double fDeltaE; ///< measured energy deposit in MUSIC
	double fZ; ///< measured particle charge number
	int fNChannel; ///< number of channels in the MUSIC
	bool fIsPileUp;
};

#endif
