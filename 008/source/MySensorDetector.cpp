#include "MySensorDetector.h"
#include <iostream>
#include <iomanip>
#include "G4UnitsTable.hh"

MySensorDetector::MySensorDetector(const G4String &name) : G4VSensitiveDetector(name) {
    ;
}

MySensorDetector::~MySensorDetector(){
    ;
}

G4bool MySensorDetector::ProcessHits(G4Step*aStep, G4TouchableHistory*ROhist){

    if (aStep->GetTotalEnergyDeposit() != 0) {
        G4cout 
        << "TrackID: " <<   aStep->GetTrack()->GetTrackID() 
        << " PID: "    <<   aStep->GetTrack()->GetParticleDefinition()->GetParticleName() 
        << " EnDep: "  <<   std::setw(7) << G4BestUnit( aStep->GetTotalEnergyDeposit(), "Energy" )
        << " Position: " << std::setw(7) << G4BestUnit( aStep->GetPreStepPoint()->GetPosition(), "Length" )
        << G4endl;
    }
    
}
