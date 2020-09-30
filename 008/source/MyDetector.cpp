#include "MyDetector.h"

#include "G4Box.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4VisAttributes.hh"

#include "MySensorDetector.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include "G4MaterialPropertiesTable.hh"

using namespace CLHEP;

G4VPhysicalVolume* MyDetector::Construct(){

    const int N_DATA = 2;
    double photonEneries[N_DATA] = { 1.65*eV, 3.10*eV };

    auto air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    auto airPropertiesTable = new G4MaterialPropertiesTable();
    air->SetMaterialPropertiesTable( airPropertiesTable );

    double airRIndex[N_DATA] = { 1.0, 1.0 };
    airPropertiesTable->AddProperty("RINDEX", photonEneries, airRIndex, N_DATA);

    auto water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
    auto waterPropertiesTable = new G4MaterialPropertiesTable();
    water->SetMaterialPropertiesTable( waterPropertiesTable );

    double waterRIndex[N_DATA] = { 1.33, 1.33 };
    waterPropertiesTable->AddProperty("RINDEX", photonEneries, waterRIndex, N_DATA);

    double waterAbsLength[N_DATA] = { 10*m, 10*m };
    waterPropertiesTable->AddProperty("ABSLENGTH", photonEneries, waterAbsLength, N_DATA);

    auto glass = G4NistManager::Instance()->FindOrBuildMaterial("G4_GLASS_PLATE");
    auto glassPropertiesTable = new G4MaterialPropertiesTable();
    glass->SetMaterialPropertiesTable( glassPropertiesTable );

    double glassRIndex[N_DATA] = { 2.50, 2.50 };
    glassPropertiesTable->AddProperty("RINDEX", photonEneries, glassRIndex, N_DATA);

    double glassAbsLength[N_DATA] = { 50*cm, 50*cm };
    glassPropertiesTable->AddProperty("ABSLENGTH", photonEneries, glassAbsLength, N_DATA);

    // World volume
    auto worldBox = new G4Box("worldBox", 0.5*m, 0.5*m, 0.5*m); 
    auto logicalWorld = new G4LogicalVolume(worldBox, air, "LogicalWorld");
    auto physicalWorld = new G4PVPlacement(0, {0,0,0}, logicalWorld, "World", 0, false, 0);
    logicalWorld->SetVisAttributes( new G4VisAttributes( false ) );

    double sideWaterBox = 20.0*cm;
    auto waterBox = new G4Box("worldBox", sideWaterBox/2, sideWaterBox/2, sideWaterBox/2); 
    auto logWater = new G4LogicalVolume(waterBox, water, "Logical Water Box");
    auto physWater = new G4PVPlacement(0, {0,0,0}, logWater, "World/WaterBox", logicalWorld, false, 0);
    logWater->SetVisAttributes( new G4VisAttributes( G4Colour::Blue() ) );

    double sideGlassBox = 10.0*cm;
    auto glassBox = new G4Box("worldBox", sideGlassBox/2, sideGlassBox/2, sideGlassBox/2); 
    auto logGlass = new G4LogicalVolume(glassBox, glass, "Logical Glass Box");
    auto physGlass = new G4PVPlacement(0, {0,0,0}, logGlass, "World/WaterBox/GlassBox", logWater, false, 0);
    logGlass->SetVisAttributes( new G4VisAttributes( G4Colour::Cyan() ) );

    // Air Water Optical Properties
    auto surfaceAirWater = new G4OpticalSurface("Air/Water Surface");
    surfaceAirWater->SetModel(unified);
    surfaceAirWater->SetType(dielectric_dielectric);
    surfaceAirWater->SetFinish(polished);

    auto surfaceAirWaterProp = new G4MaterialPropertiesTable();
    surfaceAirWater->SetMaterialPropertiesTable(surfaceAirWaterProp);

    auto logSurfaceAirWater = new G4LogicalBorderSurface("Air->Water Interface", physicalWorld, physWater, surfaceAirWater);
    auto logSurfaceWaterAir = new G4LogicalBorderSurface("Water->Air Interface", physWater, physicalWorld, surfaceAirWater);

    double reflectAirWater[N_DATA] = { 0.7, 0.7 };
    surfaceAirWaterProp->AddProperty("REFLECTIVITY", photonEneries, reflectAirWater, N_DATA);

    // Water Glass Optical Properties
    auto surfaceWaterGlass = new G4OpticalSurface("Water/Glass Surface");
    surfaceWaterGlass->SetModel(unified);
    surfaceWaterGlass->SetType(dielectric_metal);
    surfaceWaterGlass->SetFinish(polished);
    surfaceWaterGlass->SetSigmaAlpha(5*degree);

    auto surfaceWaterGlassProp = new G4MaterialPropertiesTable();
    surfaceWaterGlass->SetMaterialPropertiesTable(surfaceWaterGlassProp);

    auto logsurfaceWaterGlass = new G4LogicalBorderSurface("Water->Glass Interface", physWater, physGlass, surfaceWaterGlass);
    auto logSurfaceGlassWater = new G4LogicalBorderSurface("Glass->Water Interface", physGlass, physWater, surfaceWaterGlass);

    double reflectWaterGlass[N_DATA] = { 1, 1 };
    surfaceWaterGlassProp->AddProperty("REFLECTIVITY", photonEneries, reflectWaterGlass, N_DATA);

    return physicalWorld;
}