#include "MyDetector.h"

#include "G4Box.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"

#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;

G4VPhysicalVolume* MyDetector::Construct(){

    auto lAr = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");
    auto air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    auto ssteel = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    // World volume
    auto worldBox = new G4Box("worldBox", 0.5*m, 0.5*m, 0.5*m); 
    auto logicalWorld = new G4LogicalVolume(worldBox, air, "LogicalWorld");
    auto physicalWorld = new G4PVPlacement(0, {0,0,0}, logicalWorld, "World", 0, false, 0);

    double lenght = 50.0*cm, width = 10.0*cm, skin = 3.0*mm;
    auto detectorExterior = new G4Box("detectorBoxE", lenght/2, width/2, width/2);
    auto detectorInterior = new G4Box("detectorBoxI", lenght/2-skin, width/2-skin, width/2-skin);
    auto detectorWall = new G4SubtractionSolid("detectorBox",detectorExterior,detectorInterior);
    auto logDetectorWall = new G4LogicalVolume(detectorWall, ssteel, "logDetectorWall");
    auto physDetectorWall = new G4PVPlacement(0, {0,0,0}, logDetectorWall, "DetectorWall", logicalWorld, false, 0);

    auto logDetectorVolume = new G4LogicalVolume(detectorInterior, lAr, "logDetectorVolume");
    auto physDetectorVolume = new G4PVPlacement(0, {0,0,0}, logDetectorVolume, "DetectorVolume", logicalWorld, false, 0);

    return physicalWorld;
}