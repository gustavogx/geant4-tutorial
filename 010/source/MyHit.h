#pragma once

#include "G4VHit.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"

class MyHit : public G4VHit{
public:

    MyHit( G4Step*aStep );

    void Print() override;

    inline int GetTrackID() { return m_TrackID; }
    inline G4String GetParticleID() { return m_PID; }
    inline double GetDepositedEnergy() { return m_EnDep; }
    inline G4ThreeVector GetPosition() { return m_Position; }
    inline G4ThreeVector GetDirection() { return m_Direction; }

private:

    int m_TrackID;
    G4String m_PID;
    double m_EnDep;
    G4ThreeVector m_Position, m_Direction;

};