// ETFActionInitialization.c -- implementation of the class
// Created: Jan. 26, 2019, transcribed by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 28, 2019, by Asia SUN

#include "ETFActionInitialization.hh"

#include "ETFPrimaryGeneratorAction.hh"
#include "ETFTrackingAction.hh"
#include "ETFEventAction.hh"
#include "ETFRunAction.hh"
#include "G4RunManager.hh"

ETFActionInitialization::ETFActionInitialization()
 : G4VUserActionInitialization(){}

ETFActionInitialization::~ETFActionInitialization(){}

void ETFActionInitialization::BuildForMaster() const{
	SetUserAction(new ETFRunAction());
}

void ETFActionInitialization::Build() const{
	SetUserAction(new ETFPrimaryGeneratorAction);

	ETFRunAction *runAction = new ETFRunAction();
	ETFEventAction *eventAction = new ETFEventAction(runAction);
	ETFTrackingAction *trackingAction = new ETFTrackingAction(eventAction);
	SetUserAction(trackingAction);
	SetUserAction(eventAction);
	SetUserAction(runAction);
}

