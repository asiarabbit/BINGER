/**
	\file TADCSuperLayer.h
	\class TADCSuperLayer
	\brief A super layer is the combination of two adjacent layers in a DC,
	which of course share the same wire-orientation. This is a container class, which
	accommodates TADCCable objects.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/3 Last revised: 2018/4/26, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TADCSUPERLAYER_H_
#define _TADCSUPERLAYER_H_

#include <vector>
#include "TAStuff.h"

using std::vector;

class TADCCable;
class TAMWDCTa;

class TADCSuperLayer : public TAStuff{
public:
	TADCSuperLayer(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TADCSuperLayer();
	short GetSLayerId() const;
	short GetNCable() const;
	const double *GetGlobalDirection() const;
	TADCCable *GetCable(int n) const;
	void SetSLayerId(int id);
	void SetGlobalDirection(const double *ag);

	virtual void Initialize();
	virtual void Configure();
	
	friend class TAMWDCTa;
protected:
	void SetNDCCable(int n);

	vector<TADCCable *> fDCCableArr;
	short fSLayerId;
	short fNCable;
	/// global orientation vector, pointing to the Mother MWDC direction array
	double fGlobalDirection[3];
};

#endif
