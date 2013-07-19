#ifndef singCrysDetectorConstruction_h
#define singCrysDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"

class G4VPhysicalVolume;
class G4UserLimits;

// Detector construction class that defines materials and geometry

class singCrysDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    singCrysDetectorConstruction();
    virtual ~singCrysDetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
  
  private:
    void DefineMaterials();
    G4MaterialPropertiesTable* generateLYSOTable();
    G4MaterialPropertiesTable* generateRIndexTable(G4double rindex);
    G4MaterialPropertiesTable* generateSiTable();

    G4UserLimits* fStepLimit;
};

#endif
