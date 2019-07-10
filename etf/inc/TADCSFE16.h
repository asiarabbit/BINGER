/**
	\file TADCSFE16.h
	\class TADCSFE16
	\brief result of an elaborated classification of DC anodes, a container
	class, storing TAAnode objects that belongs to the same SFE16 Ampli-Shape-Discri
	chip, which is desigend to contain 16 channels.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/1 Last revised: 2017/10/12, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TADCSFE16_H_
#define _TADCSFE16_H_

#include "TAStuff.h"
#include <array>

using std::array;

class TAAnode;

class TADCSFE16 : public TAStuff{
public:
	TADCSFE16(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TADCSFE16();
	TAAnode *GetAnode(int n) const;
	int GetChipId() const;
	void SetAnode(int n, TAAnode *ano);
	void SetChipId(int id);

	virtual void Initialize();
	virtual void Configure();
protected:
	array<TAAnode *, 16> fAnodeArr;
	int fChipId;
};

#endif
