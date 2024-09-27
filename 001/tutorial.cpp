/* ========================================================================

		This is the shortest working simulation using Geant4, v11.2.2.
	It has all the basic elements to be considerer a simulation. 
	Right now, it does nothing more than shoot 1 GeV electrons through 
	10m of liquid argon. Look for the next tutorial to see how this 
	can be expanded.

	How to compile it:
	
	1) Using the compiler directly:

	$ g++ tutorial.cpp `geant4-config --libs -cflags` -I${G4INCLUDES} -o tutorial

	Where you have to define ${G4INCLUDES} to point to the include files in 
	your local installation.

	2) Using cmake and make:

	Create a directory to hold the build, run cmake from it:

	$ mkdir build && cd build
	$ cmake ..
	$ make


	Gustavo Valdiviesso 2024
   ======================================================================== */

#include <iostream>

// The Manager
#include "G4RunManager.hh"

// The Detector
#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

// This is YOUR Detector. It can have many attributes but for
// now Geant4 only needs to know how to construct it.
class MyDetector : public G4VUserDetectorConstruction {
public:
	virtual G4VPhysicalVolume* Construct() override;
};

// This method is mandatory and it returns the detector's geometry
G4VPhysicalVolume* MyDetector::Construct(){

	// This is just a Liquid-Argon filled box
	auto lAr = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");
	auto worldBox = new G4Box("worldBox", 10000, 10000, 10000);
	auto logicalWorld = new G4LogicalVolume(worldBox, lAr, "Logical World");
	auto physicalWorld = new G4PVPlacement(0, {0,0,0}, logicalWorld, "Physical World", 0, false, 0);

	return physicalWorld;
}

// The Physics List
#include "G4VUserPhysicsList.hh"
#include "G4PhysListFactory.hh"
// We are going to import a pre-made physics list called "Shielding"
// You can find other options at https://geant4.web.cern.ch/node/302

// The Action Initialization (particle source)
#include "G4VUserActionInitialization.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Electron.hh"

// This is YOUR source of particles. Primaries are the first
// particles entering your detector. We must tell Geant4
// which particles we want, their origin and momentum.
class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction{
public:
	virtual void GeneratePrimaries(G4Event* anEvent) override;
};

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent){

	auto *particleGun = new G4ParticleGun( G4Electron::Definition() );
	particleGun->GeneratePrimaryVertex(anEvent);

	// We will rely on the default values for the particle's momentum 
	// and vertex (its origin).
	// Not the best way of doing it, but let's ask a few things from the Primary Generator
	std::cout << "Run ID      " << anEvent->GetEventID() << std::endl;
	std::cout << "Primary     " << particleGun->GetParticleDefinition()->GetParticleName() << std::endl;
	std::cout << "Enery (MeV) " << particleGun->GetParticleEnergy() << std::endl;

};

// We need some action! The only action we will perform in this
// first simulation is to generate our particles.
class MyActionInitialization : public G4VUserActionInitialization{
public:
	virtual void Build() const override;
};

void MyActionInitialization::Build() const {
	SetUserAction( new MyPrimaryGenerator );
};

#include "G4RunManagerFactory.hh"

// This is the Main code.
int main(){

	// We can order a manager from the RunManager factory
	auto manager = G4RunManagerFactory::CreateRunManager();

	// We can get a pointer to an existing Physics List from the factory
	auto physicsListFactory = new G4PhysListFactory();
	auto physicsList = physicsListFactory->GetReferencePhysList("Shielding");

	manager->SetUserInitialization( new MyDetector() );
	manager->SetUserInitialization( physicsList );
	manager->SetUserInitialization( new MyActionInitialization() );
	manager->Initialize();

	manager->BeamOn(10);

	delete manager; // The manager will delete all other pointers owned by it.
};