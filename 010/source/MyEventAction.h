#pragma once
#include "G4UserEventAction.hh"

class MyEventAction : public G4UserEventAction{

    void BeginOfEventAction(const G4Event* anEvent) override;
    void EndOfEventAction(const G4Event* anEvent) override;


};