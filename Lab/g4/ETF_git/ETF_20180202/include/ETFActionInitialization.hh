// ETFActionInitialization.hh -- brief definition of the class
// Created: Jan. 26, 2019, transcribed by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 26, 2019, by Asia SUN

#ifndef ETFActionInitialization_h
#define ETFActionInitialization_h

#include "G4VUserActionInitialization.hh"

class ETFActionInitialization : public G4VUserActionInitialization{
public:
	ETFActionInitialization();
	virtual ~ETFActionInitialization();
	
	virtual void BuildForMaster() const;
	virtual void Build() const;
};

#endif
