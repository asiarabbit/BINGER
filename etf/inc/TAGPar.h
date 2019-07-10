/**
	\file TAGPar.h
	\class TAGPar
	\brief Class to store global parameters. Each parameter would be a
	TAParameter object.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/12/12 Last revised: 2018/6/7, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAGPAR_H_
#define _TAGPAR_H_

#include <array>

using std::array;

class TObject; // ROOT class
class TAParameter;
class TAParaManager;

class TAGPar{
public:
	static TAGPar *Instance();
	virtual ~TAGPar();

	double Val(unsigned int id) const;
	const TAParameter *Par(unsigned int id) const;
	void ShowPar(unsigned int id) const;
	short GetNParameter() const;
	TObject *Agent(unsigned int id) const;
	friend class TAParaManager;
	bool HasRead() const{ return fHasRead; }
	void Write(); ///< write Agent
protected:
	TAGPar();
	TAParameter *Parameter(unsigned int id) const; ///< for modify the parameters
	void DefineParameters();
	void DefineAgents();
	void SetHasRead(bool opt){ fHasRead = opt; }
	static TAGPar *fInstance;
	enum {kParSIZE = 250, kAgentSIZE = 20};
	array<TAParameter *, kParSIZE> fParVec;
	bool fHasRead; ///< has read the config file(s) or not
	array<TObject *, kAgentSIZE> fAgentVec; ///< agents in the Matrix, sneaking into codes for information
};

#endif
