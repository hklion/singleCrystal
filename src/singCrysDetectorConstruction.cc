#include "singCrysDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "singCrysTrackerSD.hh"
#include "G4UserLimits.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSFlatSurfaceCurrent.hh"
#include "G4PSEnergyDeposit.hh"

// Constructor: define materials
singCrysDetectorConstruction::singCrysDetectorConstruction()
: G4VUserDetectorConstruction(),
  fLimit(NULL)
{ 
  DefineMaterials();
}

// Destructor: nothing to delete
singCrysDetectorConstruction::~singCrysDetectorConstruction()
{ }

// Defines LYSO material
void singCrysDetectorConstruction::DefineMaterials()
{
  G4NistManager* nist = G4NistManager::Instance();

  G4bool isotopes = false;
  
  // Define LYSO
  G4Element* Lu = nist->FindOrBuildElement("Lu", isotopes);
  G4Element* Y = nist->FindOrBuildElement("Y", isotopes);
  G4Element* Si = nist->FindOrBuildElement("Si", isotopes);
  G4Element* O = nist->FindOrBuildElement("O", isotopes);

  // TODO: check density
  G4Material* LYSO = new G4Material("LYSO", 7.4*g/cm3, 4);
  // For now, assume that x = 0.2. Making all subscripts integers...
  LYSO->AddElement(Lu, 9);
  LYSO->AddElement(Y, 1);
  LYSO->AddElement(Si, 5);
  LYSO->AddElement(O, 25);
  
  // Define epoxy
  G4Element* C = nist->FindOrBuildElement("C", isotopes);
  G4Element* H = nist->FindOrBuildElement("H", isotopes);
  // Oxygen is already defined
  G4Material* epoxy = new G4Material("Epoxy", 1.0*g/cm3, 3);
  epoxy->AddElement(C, 1);
  epoxy->AddElement(H, 1);
  epoxy->AddElement(O, 1);

  // Define optical grease
  G4Material* opticalGrease = new G4Material("opticalgrease", 1.0*g/cm3, 2);
  opticalGrease->AddElement(Si, 1);
  opticalGrease->AddElement(O, 1); 
}

// Constructs and returns material properties table for LYSO
G4MaterialPropertiesTable* singCrysDetectorConstruction::generateLYSOTable()
{
  const G4int nEntries = 7;

  // Define energies and corresponding quantities
  G4double PhotonEnergy[nEntries] =
      {2.271*eV, 2.403*eV, 2.551*eV, 2.690*eV, 2.844*eV, 2.952*eV, 3.062*eV};

  G4double RefractiveIndex[nEntries] =
      {1.806, 1.810, 1.813, 1.818, 1.822, 1.827, 1.833};

  G4double Absorption[nEntries] = 
      {40*cm, 40*cm, 40*cm, 40*cm, 40*cm, 40*cm, 40*cm};

  G4double Rayleigh[nEntries] = 
      {260*cm, 260*cm, 260*cm, 260*cm, 260*cm, 260*cm, 260*cm};

  const G4int nEntriesScint = 11;
  G4double ScintilPhotonEnergy[nEntriesScint] = 
    {2.138*eV, 2.214*eV, 2.296*eV, 2.385*eV, 2.480*eV, 2.583*eV, 2.696*eV,
     2.818*eV, 2.952*eV, 3.100*eV, 3.263*eV};
  G4double Scintil[nEntriesScint] = 
    {0.002, 0.116, 0.118, 0.090, 0.045, 0.018, 0.008, 0.004, 0.003, 0.002,
     0.002};

  // Define table and add properties
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex, nEntries)
      ->SetSpline(true);
  table->AddProperty("ABSLENGTH", PhotonEnergy, Absorption, nEntries)
      ->SetSpline(true);
  table->AddProperty("RAYLEIGH", PhotonEnergy, Rayleigh, nEntries)
      ->SetSpline(true);
  table->AddProperty("FASTCOMPONENT", ScintilPhotonEnergy, Scintil,
      nEntriesScint)->SetSpline(true);

  table->AddConstProperty("SCINTILLATIONYIELD", 26./keV);
  table->AddConstProperty("RESOLUTIONSCALE", 1.0); //TODO: FIX
  table->AddConstProperty("FASTTIMECONSTANT", 40.*ns);

  return table;
}

// Constructs and returns material properties table for aluminum
G4MaterialPropertiesTable* singCrysDetectorConstruction::
  generateAlTable()
{
  const G4int nEntries = 11;
  // Define photon energies
  G4double PhotonEnergy[nEntries] =
  {1.550*eV, 1.653*eV, 1.771*eV, 1.908*eV, 2.067*eV, 2.254*eV, 2.480*eV,
   2.755*eV, 3.100*eV, 3.543*eV, 4.133*eV};
  // Define refractive index
  G4double RefractiveIndex[nEntries] =
  {2.767, 2.367, 1.921, 1.558, 1.262, 1.015, 0.8126, 0.6332, 0.4879,
  0.3667, 0.26418};        
  // Construct table and add refractive index property.
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex, nEntries);
  
  return table;
}

// Constructs and returns material properties table for crystalline silicon
G4MaterialPropertiesTable* singCrysDetectorConstruction::
  generateSiTable()
{
  const G4int nEntries = 11;
  // Define photon energies
  G4double PhotonEnergy[nEntries] =
  {1.550*eV, 1.653*eV, 1.771*eV, 1.908*eV, 2.067*eV, 2.254*eV, 2.480*eV,
   2.755*eV, 3.100*eV, 3.543*eV, 4.133*eV};
  // Define refractive index
  G4double RefractiveIndex[nEntries] = 
  {3.696, 3.734, 3.783, 3.849, 3.939, 4.072, 4.286, 4.687, 5.646,
   5.446, 4.966};

  // Construct table and add refractive index property.
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex, nEntries);

  return table;
}

// Constructs and returns material properties table for a material of
// arbitrary constant refractive index
G4MaterialPropertiesTable* singCrysDetectorConstruction::
  generateRIndexTable(G4double rindex)
{
  const G4int nEntries = 7;
  
  // Define energies and refractive indices (all unity)
  G4double PhotonEnergy[nEntries] =
    {3.060*eV, 2.951*eV, 2.843*eV, 2.689*eV, 2.550*eV, 2.402*eV, 2.270*eV};
  G4double RefractiveIndex[nEntries] = 
    {rindex, rindex, rindex, rindex, rindex, rindex, rindex};

  // Construct table and add refractive index
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex, nEntries);

  return table;
}

G4VPhysicalVolume* singCrysDetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Crystal parameters: assumes a regular 'crysNumSides'-gonal prism
  //G4double crysSideLength = sqrt(18/(3*sqrt(3)))*cm;
  G4double crysSideLength = 3.*cm;
  G4double crysSizeZ = 11*cm;
  G4int crysNumSides = 4;
  G4double layer1Thick = 1*mm;
  G4double layer2Thick = 1*mm;
  // Parameters for APD
  G4double siliconXY = 10*mm;
  G4double siliconZ = 0.4*mm;
  G4double casingX = 13.7*mm;
  G4double casingY = 14.5*mm;
  G4double casingZ = 1.78*mm;
  G4double epoxyX = 11.7*mm;
  G4double epoxyY = 12.5*mm;
  G4double epoxyZ = 0.6*mm;
  G4double mountingZ = 0.3*mm; // Thickness of mounting
  G4double APDZ = mountingZ + casingZ;
  // Length from the center of the polygonal face to the middle of one of
  // the polygon's sides.
  G4double crysRadLen = crysSideLength / (2 * std::tan(pi / crysNumSides));
  G4double crysMaxXYRad = crysSideLength / (2 * std::sin(pi / crysNumSides));
  G4Material* crysMat = nist->FindOrBuildMaterial("LYSO");
  crysMat->SetMaterialPropertiesTable(generateLYSOTable());
  G4Material* layer1Mat = nist->FindOrBuildMaterial("G4_Al");
  layer1Mat->SetMaterialPropertiesTable(generateAlTable());
  G4Material* layer2Mat = nist->FindOrBuildMaterial("G4_Galactic");
  layer2Mat->SetMaterialPropertiesTable(generateRIndexTable(1.00));
  // Check overlaps in volumes
  G4bool checkOverlaps = true;

  // World parameters
  G4double worldSizeXY = 5 * crysMaxXYRad; // Maximum width is twice radius
  G4double worldSizeZ = 5 * crysSizeZ;
  G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");
  worldMat->SetMaterialPropertiesTable(generateRIndexTable(1.00));

  // Define world
  G4Box* solidWorld = new G4Box("World",
    0.5 * worldSizeXY, 0.5 * worldSizeXY, 0.5 * worldSizeZ);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, // solid
                                                    worldMat,   // material
                                                    "World");   // name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                // no rotation
                      G4ThreeVector(),  // at the origin
                      logicWorld,       // logical volume
                      "World",          // name
                      0,                // mother volume
                      false,            // no boolean operation
                      0,                // copy number
                      checkOverlaps);   // overlaps checking


  // Define crystal: for the sake of simplicity, just a box for now
//  G4Box* solidCrys = new G4Box("Crystal",
//    0.5 * crysSideLength, 0.5 * crysSideLength, 0.5 * crysSizeZ);

  G4double crysZPlaneCoords[2] = {-0.5 * crysSizeZ, 0.5 * crysSizeZ};
  G4double crysRInner[2] = {0, 0};
  G4double crysROuter[2] = {crysRadLen, crysRadLen};

  // Angle by which the crystal should be rotated so that the top of
  // the world is always parallel to a face. It is equal to the sum of
  // the interior angles of the polygon divided by the number of sides,
  // divided by two.
  G4double rotation = (crysNumSides - 2) * pi / (2 * crysNumSides);

  G4Polyhedra* solidCrys = new G4Polyhedra("Crystal",
                                     rotation,
                                     2*pi + rotation,
                                     crysNumSides,
                                     2,
                                     crysZPlaneCoords,
                                     crysRInner,
                                     crysROuter);

  G4LogicalVolume* logicCrys = new G4LogicalVolume(solidCrys,   // solid
                                                   crysMat,     // material
                                                   "Crystal");  // name

  // Define two other polyhedra to serve as layers surrounding the crystal.
  // The first will be a user-defined amount larger than the crystal, and
  // the second will be a user-defined amount larger than the first. The
  // crystal will be placed in the first, and the first will be placed in
  // the second. This will create layers.

  G4double layer1RadLen = crysRadLen + layer1Thick;
  G4double layer2RadLen = layer1RadLen + layer2Thick;
  G4double layer1ROuter[2] = {layer1RadLen, layer1RadLen};
  G4double layer2ROuter[2] = {layer2RadLen, layer2RadLen};

  G4Polyhedra* solidLayer1 = new G4Polyhedra("Layer 1",
                                            rotation,
                                            2*pi + rotation,
                                            crysNumSides,
                                            2,
                                            crysZPlaneCoords,
                                            crysRInner,
                                            layer1ROuter);

  G4Polyhedra* solidLayer2 = new G4Polyhedra("Layer 2",
                                            rotation,
                                            2*pi + rotation,
                                            crysNumSides,
                                            2,
                                            crysZPlaneCoords,
                                            crysRInner,
                                            layer2ROuter); 

  G4LogicalVolume* logicLayer1 = new G4LogicalVolume(solidLayer1,   // solid
                                                     layer1Mat,     // material
                                                     "Layer 1");  // name

  G4LogicalVolume* logicLayer2 = new G4LogicalVolume(solidLayer2,   // solid
                                                     layer2Mat,     // material
                                                     "Layer 2");  // name
  
  new G4PVPlacement(0,                // no rotation
                    G4ThreeVector(),  // at the origin
                    logicCrys,        // logical volume
                    "Crystal",        // name
                    logicLayer1,      // mother volume
                    false,            // no boolean operation
                    0,                // copy number
                    checkOverlaps);   // overlaps checking

  new G4PVPlacement(0,                // no rotation
                    G4ThreeVector(),  // at the origin
                    logicLayer1,      // logical volume
                    "Layer 1",        // name
                    logicLayer2,      // mother volume
                    false,            // no boolean operation
                    0,                // copy number
                    checkOverlaps);   // overlaps checking

  new G4PVPlacement(0,                // no rotation
                    G4ThreeVector(),  // at the origin
                    logicLayer2,      // logical volume
                    "Layer 2",        // name
                    logicWorld,       // mother volume
                    false,            // no boolean operation
                    0,                // copy number
                    checkOverlaps);   // overlaps checking

  // Define solids for APD
  G4Box* solidAPD = new G4Box("APD",
    casingX * 0.5, casingY * 0.5, APDZ * 0.5);
  G4Box* solidCasing = new G4Box("CasingFull",
    casingX * 0.5, casingY * 0.5, casingZ * 0.5);
  G4Box* solidEpoxy = new G4Box("Epoxy",
    epoxyX * 0.5, epoxyY * 0.5, epoxyZ * 0.5);
  G4Box* solidSilicon = new G4Box("SiDetector",
    siliconXY * 0.5, siliconXY * 0.5, siliconZ * 0.5);
  G4Box* solidMounting = new G4Box("Mounting",
    casingX * 0.5, casingY * 0.5, mountingZ * 0.5);  

  // Define materials for APD
  G4Material* APDMat = nist->FindOrBuildMaterial("G4_AIR");
  APDMat->SetMaterialPropertiesTable(generateRIndexTable(1.00));
  G4Material* casingMat = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  G4Material* epoxyMat = nist->FindOrBuildMaterial("Epoxy");
  epoxyMat->SetMaterialPropertiesTable(generateRIndexTable(1.5));
  G4Material* siliconMat = nist->FindOrBuildMaterial("G4_Si");
  siliconMat->SetMaterialPropertiesTable(generateSiTable());
  G4Material* mountingMat = nist->FindOrBuildMaterial("opticalgrease");
  mountingMat->SetMaterialPropertiesTable(generateRIndexTable(1.00));

  // Define logical volumes
  G4LogicalVolume* logicAPD = new G4LogicalVolume(solidAPD,
                                                  APDMat,
                                                  "APD");
  G4LogicalVolume* logicCasing = new G4LogicalVolume(solidCasing,
                                                     casingMat,
                                                     "Casing");
  G4LogicalVolume* logicEpoxy = new G4LogicalVolume(solidEpoxy,
                                                    epoxyMat,
                                                    "Epoxy");
  G4LogicalVolume* logicSilicon = new G4LogicalVolume(solidSilicon,
                                                      siliconMat,
                                                      "Silicon");
  G4LogicalVolume* logicMounting = new G4LogicalVolume(solidMounting,
                                                       mountingMat,
                                                       "Mounting");
  
  // Add multifunctional detector and scorers
  G4String siliconAPDname = "SiliconAPD";
  G4MultiFunctionalDetector* SiliconMFD = 
    new G4MultiFunctionalDetector(siliconAPDname);
  // Add scorers
  G4VPrimitiveScorer* primitive;
  primitive = new G4PSEnergyDeposit("eDep");
  SiliconMFD->RegisterPrimitive(primitive);
  // Gives the number of tracks that pass through the -Z surface of the solid.
  // The second argument tells the scorer to record tracks going in to the
  // solid (1). 0 means both in and out. 2 means out.
  primitive = new G4PSFlatSurfaceCurrent("FSC", 1);
  SiliconMFD->RegisterPrimitive(primitive);
  // Assign detector to silicon logical volume
  G4SDManager::GetSDMpointer()->AddNewDetector(SiliconMFD);
  logicSilicon->SetSensitiveDetector(SiliconMFD);
  
  // Place epoxy and silicon in casing.
  G4double epoxyPlaceZ = -0.5 * (casingZ - epoxyZ);
  G4double siliconPlaceZ = -(0.5 * (casingZ - siliconZ) - epoxyZ);
  new G4PVPlacement(0,
                    G4ThreeVector(0.0*mm ,0.4*mm, epoxyPlaceZ),
                    logicEpoxy,
                    "Epoxy",
                    logicCasing,
                    false,
                    0,
                    checkOverlaps);
  new G4PVPlacement(0,
                    G4ThreeVector(0.0*mm, 0.4*mm, siliconPlaceZ),
                    logicSilicon,
                    "Silicon",
                    logicCasing,
                    false,
                    0,
                    checkOverlaps);

  // Place casing and mounting in APD
  G4double mountingPlaceZ = -0.5 * (APDZ - mountingZ);
  G4double casingPlaceZ = -0.5 * (casingZ - APDZ);
  new G4PVPlacement(0,
                    G4ThreeVector(0.0*mm, 0.0*mm, mountingPlaceZ),
                    logicMounting,
                    "Mounting",
                    logicAPD,
                    false,
                    0,
                    checkOverlaps);
  new G4PVPlacement(0,
                    G4ThreeVector(0.0*mm, 0.0*mm, casingPlaceZ),
                    logicCasing,
                    "Casing",
                    logicAPD,
                    false,
                    0,
                    checkOverlaps);

  // Place APD
  G4double APDZPlacement = 0.5 * (crysSizeZ + APDZ);
  new G4PVPlacement(0,
                    G4ThreeVector(0.0*cm, 0.0*cm, APDZPlacement),
                    logicAPD,
                    "APD",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);
  // Set energy limits. If particle below energy limit, track is
  // killed and energy is deposited.
  fLimit = new G4UserLimits(DBL_MAX, DBL_MAX, DBL_MAX, 1000*MeV);
  logicSilicon->SetUserLimits(fLimit);
 
  return physWorld;
}
