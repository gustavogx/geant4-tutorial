#include "MyPrimaryGenerator.h"

#include "G4Geantino.hh"

MyPrimaryGenerator::MyPrimaryGenerator() : 
	G4VUserPrimaryGeneratorAction(), 
	m_ParticleGun( new G4ParticleGun( G4Geantino::Definition() ) ) 
{

};

MyPrimaryGenerator::~MyPrimaryGenerator(){

	delete m_ParticleGun;
};

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent){

    m_ParticleGun->GeneratePrimaryVertex(anEvent);

};