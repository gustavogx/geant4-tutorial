#include "MyHit.h"
#include "G4UnitsTable.hh"

MyHit::MyHit(G4Step*aStep) : 
    m_TrackID( aStep->GetTrack()->GetTrackID() ),
    m_PID(  aStep->GetTrack()->GetParticleDefinition()->GetParticleName() ),
    m_EnDep( aStep->GetTotalEnergyDeposit() ),
    m_Position( aStep->GetPreStepPoint()->GetPosition() ),
    m_Direction( aStep->GetPreStepPoint()->GetMomentumDirection() )
{}

void MyHit::Print(){

    G4cout 
    << "TrackID: " <<   m_TrackID 
    << " PID: "    <<   m_PID
    << " EnDep: "  <<   std::setw(7) << G4BestUnit( m_EnDep, "Energy" )
    << " Position: " << std::setw(7) << G4BestUnit( m_Position, "Length" )
    << " Directin: " << std::setw(7) << m_Direction
    << G4endl;
    
}