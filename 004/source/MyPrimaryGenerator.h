#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"


class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction{
private:
	G4ParticleGun *m_ParticleGun;
public:

	MyPrimaryGenerator();
	~MyPrimaryGenerator(); 

    virtual void GeneratePrimaries(G4Event* anEvent) override;
};

