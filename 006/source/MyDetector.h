#pragma once

// The Detector
#include "G4VUserDetectorConstruction.hh"

class MyDetector : public G4VUserDetectorConstruction {
public:
    virtual G4VPhysicalVolume* Construct() override;
};


