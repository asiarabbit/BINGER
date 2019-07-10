// B1ActionInitialization.hh -- brief definition of the class
// transcribed by Asia SUN, asia.rabbit@163.com, Jan. 14, 2019
// Last modified: Jan. 14, by Asia SUN

#ifndef B1ActionInitialization_h
#define B1ActionInitialization_h

#include "G4VUserActionInitialization.hh"

// action initialization class
class B1ActionInitialization : public G4VUserActionInitialization{
public:
	B1ActionInitialization();
	virtual ~B1ActionInitialization();

	virtual void BuildForMaster() const;
	virtual void Build() const;
};

#endif
