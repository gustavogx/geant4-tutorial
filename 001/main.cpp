/* ========================================================================

	TUTORIAL 001 - The Simplest Simulation 

		This is the shortest working simulation using Geant4, v11.2.2.
	It has all the basic elements to be considerer a simulation. 
	Right now, it does nothing more than shoot 1 GeV electrons through 
	10m of liquid argon. Look for the next tutorial to see how this 
	can be expanded.

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
#include "G4Electron.hh"
#include "G4Event.hh"

// This is YOUR source of particles. Primaries are the first
// particles entering your detector. We must tell Geant4
// which particles we want, their origin and momentum (including energy).
class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
	virtual void GeneratePrimaries(G4Event* anEvent) override;
};

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent){

	auto particleGun = new G4ParticleGun( G4Electron::Definition() ); 
	particleGun->GeneratePrimaryVertex(anEvent);

	// We will rely on the default values for the particle's momentum and vertex (its origin).
	// Not the best way of doing it, but let's ask a few things from the Primary Generator
    G4cout << "Event ID      " << anEvent->GetEventID() << G4endl;
    G4cout << "Particle Name " << particleGun->GetParticleDefinition()->GetParticleName() << G4endl;
    G4cout << "Energy (MeV)  " << particleGun->GetParticleEnergy() << G4endl;
};

// We need some action! The only action we will perform in this
// first simulation is to generate our particles.
class MyActionInitialization: public G4VUserActionInitialization{
public:
	virtual void Build() const override;
};

void MyActionInitialization::Build() const {

	// Today, the only action we want o initialize is our particle source
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

	// The Detector "Geometry" = The Geometrical Model + The Material Model
	// Ask for the NIST Manager for a material ready to be used
	// You can find more at:
	// https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html
	auto lAr = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");
	
	// We need a solid for our lab (the world volume)
	// This one is a Box, but you can learn more about solids here:
	// https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Detector/Geometry/geomSolids.html
	auto worldBox = new G4Box("worldBox", 10000, 10000, 10000);

	// A logical volume is the union of a geometry and a material
	auto logicalWorld = new G4LogicalVolume(worldBox, lAr, "logicalWorld");

	// A physical volume is a realization of a logical volume. It's something that has a position and
	// orientation in space.
	auto physicalWorld = new G4PVPlacement(0, {0,0,0}, logicalWorld, "physicalWorld", 0, false, 0);

	return physicalWorld;
};


// The Manager
#include "G4RunManager.hh"
#include "G4RunManagerFactory.hh"

// The Physics List
#include "G4VUserPhysicsList.hh"
#include "G4PhysListFactory.hh"
// We are going to import a pre-made physics list called "Shielding"
// You can find other options at 
// https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/TrackingAndPhysics/physicsProcess.html

// This is the Main code.
int main(){

	auto manager = G4RunManagerFactory::CreateRunManager();

	// We can get a pointer to an existing Physics List from the Factory
	auto physicsListFactory = new G4PhysListFactory();
	auto physicsList = physicsListFactory->GetReferencePhysList("Shielding");
	delete physicsListFactory; // Don't forget to delete the Factory

	manager->SetUserInitialization( new MyDetector );
	manager->SetUserInitialization( physicsList );
	manager->SetUserInitialization( new MyActionInitialization );
	manager->Initialize();
	manager->BeamOn(1000);

	delete manager; // The manager will delete all other pointers owned by it.
	
	return 0;
}