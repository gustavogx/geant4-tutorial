/* ========================================================================

        This is the shortest working simulation on Geant4, which has 
    all the basic elements to be considerer a simulation. Right now, it 
    does nothing more than shoot 1 GeV electrons through 10m of liquid argon.
    Look for the next tutorial to see how this can be expanded.

    Compile it with
    g++ tutorial001.cpp `geant4-config --libs` -I${G4INCLUDES} -o tutorial001

    Where you have to define ${G4INCLUDES} to point to the include files in 
    your local installation.

    On linux, you can just source the compile.sh script.

    Gustavo Valdiviesso 2020

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

class MyDetector : public G4VUserDetectorConstruction {
public:
    virtual G4VPhysicalVolume* Construct() override;
};

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

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction{
public:
    virtual void GeneratePrimaries(G4Event* anEvent) override;
};

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent){

    auto *particleGun = new G4ParticleGun( G4Electron::Definition() );
    particleGun->GeneratePrimaryVertex(anEvent);

    // Not the best way of doing it, but let's ask a few things from the Primary Generator
    std::cout << "Run ID      " << anEvent->GetEventID() << std::endl;
    std::cout << "Primary     " << particleGun->GetParticleDefinition()->GetParticleName() << std::endl;
    std::cout << "Enery (MeV) " << particleGun->GetParticleEnergy() << std::endl;

};

class MyActionInitialization : public G4VUserActionInitialization{
public:
    virtual void Build() const override;
};

void MyActionInitialization::Build() const {
    SetUserAction( new MyPrimaryGenerator );
};

// This is the Main code.
int main(){

    auto *manager = new G4RunManager();

    auto *factory = new G4PhysListFactory();
    auto *physicsList = factory->GetReferencePhysList("Shielding");

    manager->SetUserInitialization( new MyDetector() );
    manager->SetUserInitialization( physicsList );
    manager->SetUserInitialization( new MyActionInitialization() );
    manager->Initialize();

    manager->BeamOn(100);

    delete manager; // The manager will delete all other pointers owned by it.
};