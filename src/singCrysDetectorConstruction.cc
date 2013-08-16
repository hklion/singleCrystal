#include "singCrysDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "singCrysSiliconSD.hh"
#include "G4UserLimits.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSFlatSurfaceCurrent.hh"
#include "G4PSEnergyDeposit.hh"
#include "singCrysPSNPhotons.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SubtractionSolid.hh"
#include "singCrysConfig.hh"
#include <boost/program_options.hpp>

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
    {0.002, 0.002, 0.003, 0.004, 0.008, 0.018, 0.045, 0.090, 0.118, 0.116,
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

// Constructs and returns material properties table for silicon surface
G4MaterialPropertiesTable* singCrysDetectorConstruction::
  generateSiSurfaceTable()
{
  const G4int nEntries = 12;
  G4double PhotonEnergy[nEntries] = {1.378*eV, 1.459*eV, 1.550*eV, 1.653*eV,
    1.771*eV, 1.908*eV, 2.067*eV, 2.254*eV, 2.480*eV, 2.755*eV, 3.100*eV,
    3.543*eV};
  G4double Efficiency[nEntries] = {0.69, 0.78, 0.82, 0.85, 0.85, 0.86, 0.85,
    0.84, 0.81, 0.75, 0.65, 0.50};
  //G4double Efficiency[nEntries] = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};
  G4double Reflectivity[nEntries] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
    0., 0.};
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty("EFFICIENCY", PhotonEnergy, Efficiency, nEntries);
  table->AddProperty("REFLECTIVITY", PhotonEnergy, Reflectivity, nEntries);
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
  G4double RefractiveIndexReal[nEntries] =
  {2.767, 2.367, 1.921, 1.558, 1.262, 1.015, 0.8126, 0.6332, 0.4879,
  0.3667, 0.26418};
  G4double RefractiveIndexImaginary[nEntries] =
  {8.3543, 8.4177, 8.1420, 7.7124, 7.1855, 6.6273, 6.0481, 5.4544, 4.8355,
   4.2127, 3.5787};
  // Construct table and add refractive index property.
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty("REALRINDEX", PhotonEnergy, RefractiveIndexReal,
                     nEntries)->SetSpline(true);
  table->AddProperty("IMAGINARYRINDEX", PhotonEnergy, RefractiveIndexImaginary,
                     nEntries)->SetSpline(true);
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

G4MaterialPropertiesTable* singCrysDetectorConstruction::
  generateCeramicTable()
{
  const G4int nEntries = 1;
  // Define photon energy
  G4double PhotonEnergy[nEntries] = {3.*eV};
  // Define reflectivity
  G4double Reflectivity[nEntries] = {0.9};
  G4double Efficiency[nEntries] = {0.};
  // Construct table and add reflectivity property
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty("REFLECTIVITY", PhotonEnergy, Reflectivity, nEntries);
  table->AddProperty("EFFICIENCY", PhotonEnergy, Efficiency, nEntries);
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
  // Also get config file parameters
  po::variables_map config = *(singCrysConfig::GetInstance()->GetMap());

  // Crystal parameters: assumes a regular 'crysNumSides'-gonal prism
  //G4double crysSideLength = sqrt(18/(3*sqrt(3)))*cm;
  G4double crysSideLength = 3.0*cm;  // Length along flats
  G4double crysSizeZ = 11*cm;       // Z axis length
  G4int crysNumSides = config["crysNumSides"].as<G4int>(); // Number of sides
  G4double layer1Thick = 100*um;    // Thickness of layer surrounding crystal
  G4double layer2Thick = 100.*um;     // Thickness of layer surrounding layer1
  G4double AlCoating1Z = 0.1*mm;    // Thickness of top Al APD case coating
  G4double AlCoating2Z = 0.5*mm;    // Thickenss of bottom Al APD case coating
  // Parameters for APD
  G4double siliconXY = 10*mm;       // XY dimension of silicon APD chip
  G4double siliconZ = 0.6*mm;       // Thickness of silicon APD chip
  G4double casingX = 13.7*mm;       // X dimension of APD ceramic casing
  G4double casingY = 14.5*mm;       // Y dimension of APD ceramic casing
  G4double casingZ = 1.78*mm;       // Thickness of APD ceramic casing
  G4double epoxyX = 11.7*mm;        // X dimension of epoxy on APD
  G4double epoxyY = 12.5*mm;        // Y dimension of epoxy on APD
  G4double epoxyZ = 0.6*mm;         // Thickness of epoxy on APD
  G4double APDAlCaseThick = 5.*mm;  // Thickness of rim on Al APD case
  G4double APDAlCaseZ = 10.*mm;     // Thickness of Al APD case
  G4double APDSlotDepth = 5.*mm;    // How much the crystal is in the Al case
  // Length from the center of the polygonal face to the middle of one of
  // the polygon's sides.
  G4double crysRadLen = crysSideLength / (2 * std::tan(pi / crysNumSides));
  G4double crysMaxXYRad = crysSideLength / (2 * std::sin(pi / crysNumSides));
  
  // Define materials for crystals and layering
  G4Material* crysMat = nist->FindOrBuildMaterial("LYSO");
  crysMat->SetMaterialPropertiesTable(generateLYSOTable());
  G4Material* layer1Mat = nist->FindOrBuildMaterial("G4_Galactic");
  layer1Mat->SetMaterialPropertiesTable(generateRIndexTable(1.00));
  G4Material* layer2Mat = nist->FindOrBuildMaterial("G4_Al");
  layer2Mat->SetMaterialPropertiesTable(generateAlTable());
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
  // Place the world
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
  // Parameters for the crystal geometry
  G4double crysZPlaneCoords[2] = {-0.5 * crysSizeZ, 0.5 * crysSizeZ};
  G4double crysRInner[2] = {0, 0};
  G4double crysROuter[2] = {crysRadLen, crysRadLen};

  // Angle by which the crystal should be rotated so that the top of
  // the world is always parallel to a face. It is equal to the sum of
  // the interior angles of the polygon divided by the number of sides,
  // divided by two.
  G4double rotation = (crysNumSides - 2) * pi / (2 * crysNumSides);
  // Define crystal
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
  G4double layer1ZPlaneCoords[2] = {0.5 * crysSizeZ + layer1Thick,
                                    -0.5 * crysSizeZ};
  G4double layer2ZPlaneCoords[2] =
    {0.5 * crysSizeZ + layer1Thick + layer2Thick, -0.5 * crysSizeZ};
  G4Polyhedra* solidLayer1 = new G4Polyhedra("Layer 1",
                                            rotation,
                                            2*pi + rotation,
                                            crysNumSides,
                                            2,
                                            layer1ZPlaneCoords,
                                            crysRInner,
                                            layer1ROuter);

  G4Polyhedra* solidLayer2 = new G4Polyhedra("Layer 2",
                                            rotation,
                                            2*pi + rotation,
                                            crysNumSides,
                                            2,
                                            layer2ZPlaneCoords,
                                            crysRInner,
                                            layer2ROuter); 

  G4LogicalVolume* logicLayer1 = new G4LogicalVolume(solidLayer1,   // solid
                                                     layer1Mat,     // material
                                                     "Layer 1");  // name

  G4LogicalVolume* logicLayer2 = new G4LogicalVolume(solidLayer2,   // solid
                                                     layer2Mat,     // material
                                                     "Layer 2");  // name
  // Place crystal and layers 
  G4VPhysicalVolume* physCrys = new G4PVPlacement(0,  // no rotation
                    G4ThreeVector(),  // at the origin
                    logicCrys,        // logical volume
                    "Crystal",        // name
                    logicLayer1,      // mother volume
                    false,            // no boolean operation
                    0,                // copy number
                    checkOverlaps);   // overlaps checking

  G4VPhysicalVolume* physLayer1 = new G4PVPlacement(0,   // no rotation
                    G4ThreeVector(),  // at the origin
                    logicLayer1,      // logical volume
                    "Layer 1",        // name
                    logicLayer2,      // mother volume
                    false,            // no boolean operation
                    0,                // copy number
                    checkOverlaps);   // overlaps checking

  G4VPhysicalVolume* physLayer2 = new G4PVPlacement(0,   // no rotation
                    G4ThreeVector(),  // at the origin
                    logicLayer2,      // logical volume
                    "Layer 2",        // name
                    logicWorld,       // mother volume
                    false,            // no boolean operation
                    0,                // copy number
                    checkOverlaps);   // overlaps checking

  // Now define the aluminum-layer1 boundary.
  G4OpticalSurface* OpLayer1AlSurface = new G4OpticalSurface("Layer1AlSurface");
  OpLayer1AlSurface->SetModel(unified);
  OpLayer1AlSurface->SetType(dielectric_metal);
  OpLayer1AlSurface->SetFinish(ground);
  OpLayer1AlSurface->SetSigmaAlpha(0.1);
  G4LogicalBorderSurface* Layer1AlSurface = new
    G4LogicalBorderSurface("Layer1AlSurface", physLayer1, physLayer2,
                           OpLayer1AlSurface);

  // Define the aluminum-world boundary.
  G4OpticalSurface* OpWorldAlSurface = new G4OpticalSurface("WorldAlSurface");
  OpWorldAlSurface->SetModel(unified);
  OpWorldAlSurface->SetType(dielectric_metal);
  OpWorldAlSurface->SetFinish(ground);
  OpWorldAlSurface->SetSigmaAlpha(0.1);
  G4LogicalBorderSurface* WorldAlSurface = new
    G4LogicalBorderSurface("WorldAlSurface", physWorld, physLayer2,
                           OpWorldAlSurface);

  // Define solids for APD
  G4Box* solidAPD = new G4Box("APD",
    casingX * 0.5, casingY * 0.5, casingZ * 0.5);
  G4Box* solidEpoxy = new G4Box("Epoxy",
    epoxyX * 0.5, epoxyY * 0.5, epoxyZ * 0.5);
  G4Box* solidSilicon = new G4Box("SiDetector",
    siliconXY * 0.5, siliconXY * 0.5, siliconZ * 0.5);
  G4ThreeVector translEpoxy = G4ThreeVector(0.0*mm ,0.4*mm,
    0.5 * (casingZ - epoxyZ));
  G4ThreeVector translSilicon = G4ThreeVector(0.0*mm, 0.4*mm,
    0.5 * (casingZ - siliconZ) - epoxyZ);
  G4SubtractionSolid* solidCasingMid = new G4SubtractionSolid("CasingMid",
    solidAPD, solidEpoxy, 0, translEpoxy);
  G4SubtractionSolid* solidCasing = new G4SubtractionSolid("Casing",
    solidCasingMid, solidSilicon, 0, translSilicon);

  // Define materials for APD
  G4Material* APDMat = nist->FindOrBuildMaterial("G4_AIR");
  APDMat->SetMaterialPropertiesTable(generateRIndexTable(1.00));
  G4Material* casingMat = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  casingMat->SetMaterialPropertiesTable(generateCeramicTable());
  G4Material* epoxyMat = nist->FindOrBuildMaterial("Epoxy");
  epoxyMat->SetMaterialPropertiesTable(generateRIndexTable(1.5));
  G4Material* siliconMat = nist->FindOrBuildMaterial("G4_Si");
  siliconMat->SetMaterialPropertiesTable(generateSiTable());

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
  
  // Define a sensitive detector and assign it to epoxy.
  singCrysSiliconSD* siliconSD = new singCrysSiliconSD("singCrys/siliconSD",
    "SiliconHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(siliconSD);
  logicEpoxy->SetSensitiveDetector(siliconSD);

  // Make skin surface on silicon with a certain efficiency.
  G4OpticalSurface* optSilicon = new G4OpticalSurface("optSilicon");
  optSilicon->SetModel(glisur);
  optSilicon->SetFinish(polished);
  optSilicon->SetType(dielectric_metal);
  optSilicon->SetMaterialPropertiesTable(generateSiSurfaceTable());
  G4LogicalSurface* skinSilicon = new G4LogicalSkinSurface("skinSilicon",
    logicSilicon, optSilicon);

  // Place epoxy and silicon in casing.
  G4double siliconPlaceZ = (0.5 * (casingZ - siliconZ) - epoxyZ);
  G4VPhysicalVolume* physEpoxy = new G4PVPlacement(0,
                    translEpoxy,
                    logicEpoxy,
                    "Epoxy",
                    logicAPD,
                    false,
                    0,
                    checkOverlaps);
  G4VPhysicalVolume* physSilicon = new G4PVPlacement(0,
                    translSilicon,
                    logicSilicon,
                    "Silicon",
                    logicAPD,
                    false,
                    0,
                    checkOverlaps);
  G4VPhysicalVolume* physCasing = new G4PVPlacement(0,
                    G4ThreeVector(),
                    logicCasing,
                    "Casing",
                    logicAPD,
                    false,
                    0,
                    checkOverlaps);

  // Make a surface surounding the casing with a certain relfectivity
  G4OpticalSurface* optCasing = new G4OpticalSurface("optCasing");
  optCasing->SetModel(unified);
  optCasing->SetFinish(ground);
  optCasing->SetSigmaAlpha(0.2);
  optCasing->SetType(dielectric_metal);
  optCasing->SetMaterialPropertiesTable(generateCeramicTable());
  G4LogicalSkinSurface* skinCasing = new G4LogicalSkinSurface("optCasing", logicCasing, optCasing);

  // Make aluminum casing for APD. It is made by making a G4Polyhedra with
  // the same number of sides as the crystal. Then the translated cyrstal
  // is subtracted in order to make a dent for the coatings and crystal.
  // Another subtraction is done to make a place for the APD.
  G4double APDAlCaseRadLen = layer2RadLen + APDAlCaseThick;
  G4double APDAlCaseZPlaneCoords[2] = {-0.5 * APDAlCaseZ, 0.5 * APDAlCaseZ};
  G4double APDAlCaseROuter[2] = {APDAlCaseRadLen, APDAlCaseRadLen};
  G4ThreeVector translCrysAPDCase(0.0, 0.0,
    0.5 * (crysSizeZ + APDAlCaseZ) - APDSlotDepth - AlCoating1Z - AlCoating2Z);
  G4double casingPlaceZ = -(0.5 * APDAlCaseZ - 0.5 * casingZ - APDSlotDepth
    - AlCoating1Z - AlCoating2Z);
  G4Polyhedra* solidAlAPDCaseFull = new G4Polyhedra("AlAPDCaseFull",
                                     rotation,
                                     2*pi + rotation,
                                     crysNumSides,
                                     2,
                                     APDAlCaseZPlaneCoords,
                                     crysRInner,
                                     APDAlCaseROuter);
  G4SubtractionSolid* solidAlAPDCaseMid = new G4SubtractionSolid("AlAPDCaseMid",
                                        solidAlAPDCaseFull, solidLayer2, 0,
                                        translCrysAPDCase);
  G4SubtractionSolid* solidAlAPDCase = new G4SubtractionSolid("AlAPDCase",
                                      solidAlAPDCaseMid, solidAPD, 0,
                                      G4ThreeVector(0.0, 0.0, -casingPlaceZ));
  // Generate aluminum material
  G4Material* APDAlCaseMat = nist->FindOrBuildMaterial("G4_Al");
  APDAlCaseMat->SetMaterialPropertiesTable(generateAlTable());
  G4LogicalVolume* logicAlAPDCase = new G4LogicalVolume(solidAlAPDCase,
                                                       APDAlCaseMat,
                                                       "AlAPDCase");
  // Make coatings for aluminum casing.
  G4double AlCoating1ZPlaneCoords[2] = {-0.5 * crysSizeZ,
    -(0.5 * crysSizeZ + AlCoating1Z)};
  G4double AlCoating2ZPlaneCoords[2] = {-(0.5 * crysSizeZ + AlCoating1Z),
    -(0.5 * crysSizeZ + AlCoating1Z + AlCoating2Z)};
  G4Polyhedra* solidAlCoating1 = new G4Polyhedra("AlCoating1",
                                                  rotation,
                                                  2*pi + rotation,
                                                  crysNumSides,
                                                  2,
                                                  AlCoating1ZPlaneCoords,
                                                  crysRInner,
                                                  layer2ROuter);
  G4Polyhedra* solidAlCoating2 = new G4Polyhedra("AlCoating1",
                                                  rotation,
                                                  2*pi + rotation,
                                                  crysNumSides,
                                                  2,
                                                  AlCoating2ZPlaneCoords,
                                                  crysRInner,
                                                  layer2ROuter);
  G4Material* coating1Mat = worldMat;
  G4Material* coating2Mat = nist->FindOrBuildMaterial("Epoxy");
  coating2Mat->SetMaterialPropertiesTable(generateRIndexTable(1.5));
  G4LogicalVolume* logicAlCoating1 = new G4LogicalVolume(solidAlCoating1,
                                                         coating1Mat,
                                                         "AlCoating1");
  G4LogicalVolume* logicAlCoating2 = new G4LogicalVolume(solidAlCoating2,
                                                         coating2Mat,
                                                         "AlCoating2");
  G4VPhysicalVolume* physAlCoating1 = new G4PVPlacement(0,
                                                       G4ThreeVector(),
                                                       logicAlCoating1,
                                                       "AlCoating1",
                                                       logicWorld,
                                                       false,
                                                       0,
                                                       checkOverlaps);

  G4VPhysicalVolume* physAlCoating2 = new G4PVPlacement(0,
                                                       G4ThreeVector(),
                                                       logicAlCoating2,
                                                       "AlCoating2",
                                                       logicWorld,
                                                       false,
                                                       0,
                                                       checkOverlaps);


  // Place APD casing in the world volume
  G4double APDPlaceZ = -(0.5 * (crysSizeZ + casingZ) +
    AlCoating1Z + AlCoating2Z);
  G4VPhysicalVolume* physAPD = new G4PVPlacement(0,
                    G4ThreeVector(0.0*mm, 0.0*mm, APDPlaceZ),
                    logicAPD,
                    "APD",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);
  G4double APDCasePlacement = -(0.5 * (crysSizeZ + APDAlCaseZ) - APDSlotDepth); 
  G4VPhysicalVolume* physAlAPDCase = new G4PVPlacement(0,
                                         G4ThreeVector(0.,0.,APDCasePlacement),
                                         //G4ThreeVector(0., 0., 15.*cm),
                                         logicAlAPDCase,
                                         "AlAPDCase",
                                         logicWorld,
                                         false,
                                         0,
                                         checkOverlaps);
  // Now define the aluminum casing-coating2 boundary.
  G4OpticalSurface* OpCoat2APDCaseSurface = 
    new G4OpticalSurface("Coating2APDCaseSurface");
  OpCoat2APDCaseSurface->SetModel(glisur);
  OpCoat2APDCaseSurface->SetType(dielectric_metal);
  OpCoat2APDCaseSurface->SetFinish(polished);
  G4LogicalBorderSurface* Coat2APDCaseSurface = new
  G4LogicalBorderSurface("Coating2APDCaseSurface", physAlCoating2,
    physAlAPDCase, OpCoat2APDCaseSurface);

  // Set energy limits. If particle below energy limit, track is
  // killed and energy is deposited.
  fLimit = new G4UserLimits(DBL_MAX, DBL_MAX, DBL_MAX, 0.);
  logicSilicon->SetUserLimits(fLimit);
 
  return physWorld;
}
