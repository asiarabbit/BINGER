/**
	\file TAChannel.h
	\class TAChannel
	\brief Representating an electronic channel. A container class.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/9/30 Last revised: 2017/11/29, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/

#ifndef _TACHANNEL_H_
#define _TACHANNEL_H_

#include "TAStuff.h"
#include "TAChPara.h"
#include "TAChData.h"

struct tEntry;

class TAChannel : public TAStuff{
public:
	TAChannel(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAChannel();

	virtual TAChPara *GetPara() const override;
	virtual TAChData *GetData() const override;
	bool GetFiredStatus() const;
	double GetLeadingTime(int n = 0) const; ///< get the n-th pulse
	/// t0, t1 and t2 are set for choosing ch->GetLT over edges
	/// (ch->GetLT-t0) within t1 and t2 is chosen
	/// t0, t1 and t2 using default values, choose the 1st edge
	double GetLT(double t0 = -9999., double t1 = -9999., double t2 = -9999.) const;
	double GetTOT(int n = 0) const;
	double GetTime(int i = 0) const; /// Get meaningful timing information for users
	int GetSerialId() const;
	void SetPara(TAChPara *para){ fPara = para; }
	void SetData(TAChData *data){ fData = data; }
	void SetSerialId(int id){ fSerialId = id; }
	virtual void SetChId(int id);
	virtual bool Assign(tEntry *entry);

	virtual void Initialize();
protected:
	TAChPara *fPara;
	TAChData *fData;
	int fSerialId;
};

#endif
