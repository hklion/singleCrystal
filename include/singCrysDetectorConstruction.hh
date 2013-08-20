// Mandatory user detector construction class. Builds a LYSO crystal with a
// simplified silicon APD on one end. Silicon chip in APD is a
// MultiFunctionalDetector that collects surface current and energy deposits.
// Optical photon tracks are killed in the detector to get them to
// automatically deposit energy.

#ifndef singCrysDetectorConstruction_h
#define singCrysDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"

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
    //Materials and optical properties generators
    void DefineMaterials();
    G4MaterialPropertiesTable* generateTable(G4String);
    G4MaterialPropertiesTable* generateCrysTable();
    G4MaterialPropertiesTable* generateRIndexTable(G4double);
    G4MaterialPropertiesTable* generateSiSurfaceTable();
    G4MaterialPropertiesTable* generateCeramicTable();
    G4MaterialPropertiesTable* generateAlTable();
    G4SurfaceType surfaceType(G4String);
    G4SurfaceType surfaceType(G4String, G4String);
    
    // Energy/length limits
    G4UserLimits* fLimit;
};

#endif
