/* ========================================================================

	TUTORIAL 002 - The Basics of User Interface

	We added a User Interface (UI) to the 001 tutorial.
	We also recoded the particle gun in order to be persistent across runs.

	How to compile it:
	
	1) Using the compiler directly:

	$ g++ main.cpp -o tutorial `geant4-config --libs -cflags` 

	2) Using cmake and make:

	Create a directory to hold the build, run cmake from it:

	$ mkdir build && cd build
	$ cmake ..
	$ make

	Gustavo Valdiviesso 2024
   ======================================================================== */


// The Particle Source
#include "G4VUserActionInitialization.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

// This is the proper design of a particle generator (not the one done in 001)
// The particle gun is declared as a member object.
class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:

	MyPrimaryGenerator();
	~MyPrimaryGenerator();
	virtual void GeneratePrimaries(G4Event* anEvent) override;

private:

	G4ParticleGun *mParticleGun = nullptr;
};

MyPrimaryGenerator::MyPrimaryGenerator(){
	
	mParticleGun = new G4ParticleGun(); 

}

MyPrimaryGenerator::~MyPrimaryGenerator(){
	
	delete mParticleGun;

}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent){

	
	mParticleGun->GeneratePrimaryVertex(anEvent);

	// We will rely on the default values for the particle's momentum and vertex (its origin).
	// Not the best way of doing it, but let's ask a few things from the Primary Generator
    G4cout << "Particle Name " << mParticleGun->GetParticleDefinition()->GetParticleName() << G4endl;
    G4cout << "Energy (MeV)  " << mParticleGun->GetParticleEnergy() << G4endl;
	G4cout << "Who Many?	 " << mParticleGun->GetNumberOfParticles() << G4endl;

};

// This action initialization remains unchanded from 001
class MyActionInitialization: public G4VUserActionInitialization{
public:

	virtual void Build() const override;

};

void MyActionInitialization::Build() const {

	SetUserAction( new MyPrimaryGenerator );

};

// The Detector
#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

class MyDetector: public G4VUserDetectorConstruction {
public:

	virtual G4VPhysicalVolume* Construct() override;

};

G4VPhysicalVolume* MyDetector::Construct() {

	// Material
	auto lAr = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");
	
	// Geometry
	auto worldBox = new G4Box("worldBox", 10000, 10000, 10000);

	// Logical volume
	auto logicalWorld = new G4LogicalVolume(worldBox, lAr, "logicalWorld");

	// Physical volume 
	auto physicalWorld = new G4PVPlacement(0, {0,0,0}, logicalWorld, "physicalWorld", 0, false, 0);

	return physicalWorld;
};


// The Manager
#include "G4RunManager.hh"
#include "G4RunManagerFactory.hh"

// The Physics List
#include "G4VUserPhysicsList.hh"
#include "G4PhysListFactory.hh"

// The User Interface
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4String.hh"

// This is the Main code.
// We now accept command-line parameters
int main(int argc, char** argv){

	auto manager = G4RunManagerFactory::CreateRunManager();

	// We can get a pointer to an existing Physics List from the Factory
	auto physicsListFactory = new G4PhysListFactory();
	auto physicsList = physicsListFactory->GetReferencePhysList("Shielding");
	delete physicsListFactory; // Don't forget to delete the Factory

	manager->SetUserInitialization( new MyDetector );
	manager->SetUserInitialization( physicsList );
	manager->SetUserInitialization( new MyActionInitialization );
	manager->Initialize();

	// THIS IS NEW: We will decide if we run a MACRO file or open the UI prompt
	if(argc>1) {

		// If there's any number of parameters, try using the first as MACRO file.
		G4String macroFile(argv[1]);
		auto uiManager = G4UImanager::GetUIpointer();
		uiManager->ApplyCommand("/control/execute "+macroFile);
	} else {

		// If there are no parameters, open the UI prompt
		auto uiExecutive = new G4UIExecutive(argc,argv,"tcsh");
		uiExecutive->SessionStart();
		delete uiExecutive;
	}

	delete manager; // The manager will delete all other pointers owned by it.
	
	return 0;
}