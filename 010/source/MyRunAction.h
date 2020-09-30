#pragma once
#include "G4UserRunAction.hh"

class MyRunAction :  public G4UserRunAction {

    void BeginOfRunAction(const G4Run* aRun) override;
    void EndOfRunAction(const G4Run* aRun) override;


};