/*!
 * \file singCrysDetectorConstruction.cc
 * \brief Implementation file for the singCrysDetectorConstruction class.
 * Builds the necessary geometry and materials.
 */

#include "singCrysDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4Box.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"

#include "singCrysSiliconSD.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "singCrysConfig.hh"
#include "singCrysReadFile.hh"
#include <boost/program_options.hpp>

// Constructor: define materials
singCrysDetectorConstruction::singCrysDetectorConstruction()
: G4VUserDetectorConstruction()
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

// Returns the appropriate material properties table for a given material
G4MaterialPropertiesTable* singCrysDetectorConstruction::
  generateTable(G4String material)
{
  if (material.compareTo("G4_Galactic") == 0 || material.compareTo("G4_AIR") == 0)
    return generateRIndexTable(1.00);
  else if (material.compareTo("G4_Al") == 0)
    return generateAlTable();
  else if (material.compareTo("LYSO") == 0)
    return generateCrysTable();
  else if (material.compareTo("Epoxy") == 0)
    return generateRIndexTable(1.50);
  else
  {
    G4cerr << "Unable to find material " << material
      << ". Air table used.";
    return generateRIndexTable(1.00);
  }
}

// Determines whether to use a dielectric-dielectric or dielectric-metal
// boundary for a surface boundary
G4SurfaceType singCrysDetectorConstruction::surfaceType(G4String material)
{
  if (material.compareTo("G4_Si") == 0 || material.compareTo("G4_Al") == 0
    || material.compareTo("G4_ALUMINUM_OXIDE") == 0)
    return dielectric_metal;
  else
    return dielectric_dielectric;
}

// Determines whether to use a dielectric-dielectric or dielectric-metal
// boundary for a 
G4SurfaceType singCrysDetectorConstruction::
  surfaceType(G4String material1, G4String material2)
{
  if (material1.compareTo("G4_Si") == 0 || material1.compareTo("G4_Al") == 0
      || material1.compareTo("G4_ALUMINUM_OXIDE") == 0
      || material2.compareTo("G4_Si") == 0 || material2.compareTo("G4_Al") == 0
      || material2.compareTo("G4_ALUMINUM_OXIDE") == 0)
    return dielectric_metal;
  else
    return dielectric_dielectric;
}

// Constructs and returns material properties table for the crystal
G4MaterialPropertiesTable* singCrysDetectorConstruction::generateCrysTable()
{
  G4bool twoComponent = true; // Whether there are two scintillation components
  // Get config options map
  po::variables_map config = *(singCrysConfig::GetInstance()->GetMap());
  // Get relevant config options
  G4String dataPath = (G4String) config["dataPath"].as<std::string>();
  G4String fileRIndex = (G4String) config["crysRIndexFile"].as<std::string>();
  G4String fileAbs = (G4String) config["crysAbsFile"].as<std::string>();
  G4String fileRay = (G4String) config["crysRayFile"].as<std::string>();
  G4String fileFastScint =
    (G4String) config["crysFastScintFile"].as<std::string>();
  G4String fileSlowScint =
    (G4String) config["crysSlowScintFile"].as<std::string>();
  // Check whether there is a second scintillation component file. If there
  // isn't, assume that there is only one component.
  if (fileSlowScint.compareTo("") == 0)
    twoComponent = false;
  // Read in files
  singCrysReadFile RIndex = singCrysReadFile(dataPath + fileRIndex);
  singCrysReadFile Abs = singCrysReadFile(dataPath + fileAbs);
  singCrysReadFile Ray = singCrysReadFile(dataPath + fileRay);
  singCrysReadFile FastScint = singCrysReadFile(dataPath + fileFastScint);
  
  // Define table and add properties
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty("RINDEX", RIndex.GetEnergies(), RIndex.GetVals(),
    RIndex.GetNEntries())->SetSpline(true);
  table->AddProperty("ABSLENGTH", Abs.GetEnergies(), Abs.GetVals(),
    Abs.GetNEntries())->SetSpline(true);
  table->AddProperty("RAYLEIGH", Ray.GetEnergies(), Ray.GetVals(),
    Ray.GetNEntries())->SetSpline(true);
  table->AddProperty("FASTCOMPONENT", FastScint.GetEnergies(),
    FastScint.GetVals(), FastScint.GetNEntries())->SetSpline(true);

  G4double scintYield = config["scintYield"].as<G4double>();
  G4double resScale = config["resScale"].as<G4double>();
  G4double fastTimeConst = config["fastTimeConst"].as<G4double>();
  table->AddConstProperty("SCINTILLATIONYIELD", scintYield / keV);
  table->AddConstProperty("RESOLUTIONSCALE", resScale); //TODO: FIX
  table->AddConstProperty("FASTTIMECONSTANT", fastTimeConst * ns);

  // If there is a second scintillation component, add it
  if (twoComponent)
  {
    singCrysReadFile SlowScint = singCrysReadFile(dataPath + fileSlowScint);
    table->AddProperty("SLOWCOMPONENT", SlowScint.GetEnergies(),
      SlowScint.GetVals(), SlowScint.GetNEntries())->SetSpline(true);
    G4double slowTimeConst = config["slowTimeConst"].as<G4double>();
    G4double yieldRatio = config["yieldRatio"].as<G4double>();
    table->AddConstProperty("SLOWTIMECONSTANT", slowTimeConst * ns);
    table->AddConstProperty("YIELDRATIO", yieldRatio);
  }
  return table;
}

// Constructs and returns material properties table for silicon surface
G4MaterialPropertiesTable* singCrysDetectorConstruction::
  generateSiSurfaceTable()
{
  // Get config options map
  po::variables_map config = *(singCrysConfig::GetInstance()->GetMap());
  // Get relevant config options
  G4String dataPath = (G4String) config["dataPath"].as<std::string>();
  G4String fileEff = (G4String) config["SiQEffFile"].as<std::string>();
  G4String fileRefl = (G4String) config["SiReflFile"].as<std::string>();
  // Read in files
  singCrysReadFile Eff = singCrysReadFile(dataPath + fileEff);
  singCrysReadFile Refl = singCrysReadFile(dataPath + fileRefl);
  // Generate table and add properties
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty("EFFICIENCY", Eff.GetEnergies(), Eff.GetVals(),
    Eff.GetNEntries())->SetSpline(true);
  table->AddProperty("REFLECTIVITY", Refl.GetEnergies(), Refl.GetVals(),
    Refl.GetNEntries());
  return table;
}

// Constructs and returns material properties table for aluminum
G4MaterialPropertiesTable* singCrysDetectorConstruction::
  generateAlTable()
{
  // Get config options map
  po::variables_map config = *(singCrysConfig::GetInstance()->GetMap());
  // Get relevant config options
  G4String dataPath = (G4String) config["dataPath"].as<std::string>();
  G4String fileRIndexR = (G4String) config["AlRIndexRFile"].as<std::string>();
  G4String fileRIndexI = (G4String) config["AlRIndexIFile"].as<std::string>();
  // Read in files
  singCrysReadFile RIndexR = singCrysReadFile(dataPath + fileRIndexR);
  singCrysReadFile RIndexI = singCrysReadFile(dataPath + fileRIndexI);
  // Construct table and add refractive index property.
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty("REALRINDEX", RIndexR.GetEnergies(), RIndexR.GetVals(),
    RIndexR.GetNEntries())->SetSpline(true);
  table->AddProperty("IMAGINARYRINDEX", RIndexI.GetEnergies(),
    RIndexI.GetVals(), RIndexI.GetNEntries())->SetSpline(true);
  return table;
}

G4MaterialPropertiesTable* singCrysDetectorConstruction::
  generateCeramicTable()
{
  // Get config options map
  po::variables_map config = *(singCrysConfig::GetInstance()->GetMap());
  // Get reflectivity
  G4double ceramicRefl = config["ceramicRefl"].as<G4double>();
  const G4int nEntries = 1;
  // Define photon energy
  G4double PhotonEnergy[nEntries] = {3.*eV};
  // Define reflectivity
  G4double Reflectivity[nEntries] = {ceramicRefl};
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
  const G4int nEntries = 1; 
  // Define energies and refractive indices (all unity)
  G4double PhotonEnergy[nEntries] = {3.*eV};
  G4double RefractiveIndex[nEntries] = {rindex}; 
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

  // Check overlaps in volumes
  G4bool checkOverlaps = config["checkOverlaps"].as<G4bool>();

//====================NUMBERS=============================
  // Crystal parameters: assumes a regular 'crysNumSides'-gonal prism
  // Length along flats
  G4double crysSideLength = config["crysSideLength"].as<G4double>();
  G4double crysSizeZ = config["crysSizeZ"].as<G4double>();  // Z axis length
  G4int crysNumSides = config["crysNumSides"].as<G4int>(); // Number of sides
  // Thickness of layer surrounding crystal
  G4double layer1Thick = config["layer1Thick"].as<G4double>();
  // Thickness of layer surrounding layer1
  G4double layer2Thick = config["layer2Thick"].as<G4double>();
  // Thickness of top Al APD case coating
  G4double AlCoating1Z = config["AlCoating1Z"].as<G4double>();
  // Thickness of botom Al APD case coating
  G4double AlCoating2Z = config["AlCoating2Z"].as<G4double>();
  // Parameters for APD
  // XY dimension of silicon APD chip
  G4double siliconXY = config["siliconXY"].as<G4double>();
  // Thickness of silicon APD chip
  G4double siliconZ = config["siliconZ"].as<G4double>();
  // Dimensions of APD ceramic casing
  G4double casingX = config["casingX"].as<G4double>();
  G4double casingY = config["casingY"].as<G4double>();
  G4double casingZ = config["casingZ"].as<G4double>();
  // Dimensions of epoxy on APD
  G4double epoxyX = config["epoxyX"].as<G4double>();
  G4double epoxyY = config["epoxyY"].as<G4double>();
  G4double epoxyZ = config["epoxyZ"].as<G4double>();
  // thickness of rim on Al APD case
  G4double APDAlCaseThick = config["APDAlCaseThick"].as<G4double>();
  // Thickness of Al APD case
  G4double APDAlCaseZ = config["APDAlCaseZ"].as<G4double>();
  // How much fo the crystal is in the Al case
  G4double APDSlotDepth = config["APDSlotDepth"].as<G4double>();
  // Length from the center of the polygonal face to the middle of one of
  // the polygon's sides.
  G4double crysRadLen = crysSideLength / (2 * std::tan(pi / crysNumSides));
  G4double crysMaxXYRad = crysSideLength / (2 * std::sin(pi / crysNumSides));
  // World parameters
  G4double worldSizeXY = 5 * crysMaxXYRad; // Maximum width is twice radius
  G4double worldSizeZ = 5 * crysSizeZ;

  // Parameters for the crystal geometry
  G4double crysZPlaneCoords[2] = {-0.5 * crysSizeZ, 0.5 * crysSizeZ};
  G4double crysRInner[2] = {0, 0};
  G4double crysROuter[2] = {crysRadLen, crysRadLen};
  // Angle by which the crystal should be rotated so that the top of
  // the world is always parallel to a face. It is equal to the sum of
  // the interior angles of the polygon divided by the number of sides,
  // divided by two.
  G4double rotation = (crysNumSides - 2) * pi / (2 * crysNumSides);
  // Parameters for two other polyhedra to serve as layers surrounding the
  // crystal. The first will be 'layer1Thick' larger than the crystal, and the
  // second will be 'layer2Thick' larger than the first. The crystal will be
  // placed in the first, and the first will be placed in the second. This will
  // create layers.
  // Radial lengths of the n-gonal faces
  G4double layer1RadLen = crysRadLen + layer1Thick;
  G4double layer2RadLen = layer1RadLen + layer2Thick;
  // Assign those sizes as the radial lengths of both faces
  G4double layer1ROuter[2] = {layer1RadLen, layer1RadLen};
  G4double layer2ROuter[2] = {layer2RadLen, layer2RadLen};
  // Placement of the n-gonal faces in z coordinate space
  G4double layer1ZPlaneCoords[2] =
    {0.5 * crysSizeZ + layer1Thick, -0.5 * crysSizeZ};
  G4double layer2ZPlaneCoords[2] =
    {0.5 * crysSizeZ + layer1Thick + layer2Thick, -0.5 * crysSizeZ};

  // Make aluminum casing for APD. It is made by making a G4Polyhedra with
  // the same number of sides as the crystal. Then the translated cyrstal
  // is subtracted in order to make a dent for the coatings and crystal.
  // Another subtraction is done to make a place for the APD.
  // Radial length of the n-gonal face of the case
  G4double APDAlCaseRadLen = layer2RadLen + APDAlCaseThick;
  // Assign that size as the radial length of both faces
  G4double APDAlCaseROuter[2] = {APDAlCaseRadLen, APDAlCaseRadLen};
  // Placement of the n-gonal faces in z coordinate space
  G4double APDAlCaseZPlaneCoords[2] = {-0.5 * APDAlCaseZ, 0.5 * APDAlCaseZ};
  // Translation of APD case for the subtraction of the crystal from the case
  G4ThreeVector translCrysAPDCase = G4ThreeVector(0., 0.,
    0.5 * (crysSizeZ + APDAlCaseZ) - APDSlotDepth - AlCoating1Z - AlCoating2Z);
  // Translation of APD case for the subtraction of the APD from the case
  G4ThreeVector translAPDCase = G4ThreeVector(0., 0., 0.5 * APDAlCaseZ
    - 0.5 * casingZ - APDSlotDepth - AlCoating1Z - AlCoating2Z);
  // Placement of the silicon in the silicon APD chip with respect to the
  // whole APD
  G4double siliconPlaceZ = (0.5 * (casingZ - siliconZ) - epoxyZ);
  // Make coatings for aluminum casing. Define the z coordinates of their
  // n-gonal faces
  G4double AlCoating1ZPlaneCoords[2] = {-0.5 * crysSizeZ,
    -(0.5 * crysSizeZ + AlCoating1Z)};
  G4double AlCoating2ZPlaneCoords[2] = {-(0.5 * crysSizeZ + AlCoating1Z),
    -(0.5 * crysSizeZ + AlCoating1Z + AlCoating2Z)};
  // APD placement along the z axis relative to the world volume
  G4double APDPlaceZ = -(0.5 * (crysSizeZ + casingZ) +
    AlCoating1Z + AlCoating2Z);
  // Placement of aluminum APD case along the Z axis
  G4double APDCasePlacementZ = -(0.5 * (crysSizeZ + APDAlCaseZ) - APDSlotDepth);  // Translation of the epoxy and silcon for placement in the APD solid
  G4ThreeVector translEpoxy = G4ThreeVector(0.0*mm,0.4*mm,
    0.5 * (casingZ - epoxyZ));
  G4ThreeVector translSilicon = G4ThreeVector(0.0*mm, 0.4*mm,
    0.5 * (casingZ - siliconZ) - epoxyZ);


  // Define material strings. APD materials are hard coded, but other
  // materials are read in from the config file.
  G4String crysMatStr = (G4String) config["crysMat"].as<std::string>();
  G4String layer1MatStr = (G4String) config["layer1Mat"].as<std::string>();
  G4String layer2MatStr = (G4String) config["layer2Mat"].as<std::string>();
  G4String worldMatStr = (G4String) config["worldMat"].as<std::string>();
  G4String APDMatStr = "G4_AIR";
  G4String casingMatStr = "G4_ALUMINUM_OXIDE";
  G4String epoxyMatStr = "Epoxy";
  G4String siliconMatStr = "G4_Si";
  G4String APDAlCaseMatStr = "G4_Al";
  G4String coating1MatStr = (G4String) config["coating1Mat"].as<std::string>();
  G4String coating2MatStr = (G4String) config["coating2Mat"].as<std::string>();

  // Define materials from the previously defined strings.
  G4Material* crysMat = nist->FindOrBuildMaterial(crysMatStr);
  G4Material* layer1Mat = nist->FindOrBuildMaterial(layer1MatStr);
  G4Material* layer2Mat = nist->FindOrBuildMaterial(layer2MatStr);
  G4Material* worldMat = nist->FindOrBuildMaterial(worldMatStr);
  G4Material* APDMat = nist->FindOrBuildMaterial(APDMatStr);
  G4Material* casingMat = nist->FindOrBuildMaterial(casingMatStr);
  G4Material* epoxyMat = nist->FindOrBuildMaterial(epoxyMatStr);
  G4Material* siliconMat = nist->FindOrBuildMaterial(siliconMatStr);
  G4Material* APDAlCaseMat = nist->FindOrBuildMaterial(APDAlCaseMatStr);
  G4Material* coating1Mat = nist->FindOrBuildMaterial(coating1MatStr);
  G4Material* coating2Mat = nist->FindOrBuildMaterial(coating2MatStr);

  // Get the material properties tables for all of the materials that
  // interact with optical photons.
  crysMat->SetMaterialPropertiesTable(generateTable(crysMatStr));
  layer1Mat->SetMaterialPropertiesTable(generateTable(layer1MatStr));
  layer2Mat->SetMaterialPropertiesTable(generateTable(layer2MatStr));
  worldMat->SetMaterialPropertiesTable(generateTable(worldMatStr));
  epoxyMat->SetMaterialPropertiesTable(generateTable(epoxyMatStr));
  APDAlCaseMat->SetMaterialPropertiesTable(generateTable(APDAlCaseMatStr));
  coating1Mat->SetMaterialPropertiesTable(generateTable(coating1MatStr));
  coating2Mat->SetMaterialPropertiesTable(generateTable(coating2MatStr));

  // Define the necessary solids for the geometry.
  // Define world
  G4Box* solidWorld = new G4Box("World",
                                0.5 * worldSizeXY,
                                0.5 * worldSizeXY,
                                0.5 * worldSizeZ);
  // Define crystal
  G4Polyhedra* solidCrys = new G4Polyhedra("Crystal",
                                           rotation,
                                           2*pi + rotation,
                                           crysNumSides,
                                           2,
                                           crysZPlaneCoords,
                                           crysRInner,
                                           crysROuter);
  // Layers surroudning crystals
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

  // Define solids for APD
  G4Box* solidAPD = new G4Box("APD",
                              casingX * 0.5,
                              casingY * 0.5,
                              casingZ * 0.5);
  G4Box* solidEpoxy = new G4Box("Epoxy",
                                epoxyX * 0.5,
                                epoxyY * 0.5,
                                epoxyZ * 0.5);
  G4Box* solidSilicon = new G4Box("SiDetector",
                                  siliconXY * 0.5,
                                  siliconXY * 0.5,
                                  siliconZ * 0.5);
  // Make casing by subtracting the epoxy and silicon layers from the
  // full APD solid.
  G4SubtractionSolid* solidCasingMid = new G4SubtractionSolid("CasingMid",
                                                              solidAPD,
                                                              solidEpoxy,
                                                              0,
                                                              translEpoxy);
  G4SubtractionSolid* solidCasing = new G4SubtractionSolid("Casing",
                                                           solidCasingMid,
                                                           solidSilicon,
                                                           0,
                                                           translSilicon);

  // Aluminum casing for APD. Make it by subtracting out the overlap with
  // the crystal and the APD itself.
  G4Polyhedra* solidAlAPDCaseFull = new G4Polyhedra("AlAPDCaseFull",
                                                    rotation,
                                                    2*pi + rotation,
                                                    crysNumSides,
                                                    2,
                                                    APDAlCaseZPlaneCoords,
                                                    crysRInner,
                                                    APDAlCaseROuter);
  G4SubtractionSolid* solidAlAPDCaseMid =
    new G4SubtractionSolid("AlAPDCaseMid",
                           solidAlAPDCaseFull,
                           solidLayer2,
                           0,
                           translCrysAPDCase);
  G4SubtractionSolid* solidAlAPDCase = new G4SubtractionSolid("AlAPDCase",
                                                              solidAlAPDCaseMid,
                                                              solidAPD,
                                                              0,
                                                              translAPDCase);
  // Solids for the coatings for the APD case
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

  // Define the logical volumes from the solids created above
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,
                                                    worldMat,                                                                       "World");
  G4LogicalVolume* logicCrys = new G4LogicalVolume(solidCrys,
                                                   crysMat,
                                                   "Crystal");
  G4LogicalVolume* logicLayer1 = new G4LogicalVolume(solidLayer1,
                                                     layer1Mat,
                                                     "Layer 1");
  G4LogicalVolume* logicLayer2 = new G4LogicalVolume(solidLayer2,
                                                     layer2Mat,
                                                     "Layer 2");
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
  G4LogicalVolume* logicAlAPDCase = new G4LogicalVolume(solidAlAPDCase,
                                                       APDAlCaseMat,
                                                       "AlAPDCase");
  G4LogicalVolume* logicAlCoating1 = new G4LogicalVolume(solidAlCoating1,
                                                         coating1Mat,
                                                         "AlCoating1");
  G4LogicalVolume* logicAlCoating2 = new G4LogicalVolume(solidAlCoating2,
                                                         coating2Mat,
                                                         "AlCoating2");

  // Define and place physical volumes
  // Place the world
  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,
                      G4ThreeVector(),
                      logicWorld,
                      "World",
                      0,
                      false,
                      0,
                      checkOverlaps); 

  // Place crystal and layers 
  G4VPhysicalVolume* physCrys = new G4PVPlacement(0,
                                                  G4ThreeVector(),
                                                  logicCrys,
                                                  "Crystal",
                                                  logicLayer1,
                                                  false,
                                                  0,
                                                  checkOverlaps);

  G4VPhysicalVolume* physLayer1 = new G4PVPlacement(0,
                                                    G4ThreeVector(),
                                                    logicLayer1,
                                                    "Layer 1",
                                                    logicLayer2,
                                                    false,
                                                    0,
                                                    checkOverlaps);
  G4VPhysicalVolume* physLayer2 = new G4PVPlacement(0,
                                                    G4ThreeVector(),
                                                    logicLayer2,
                                                    "Layer 2",
                                                    logicWorld,
                                                    false,
                                                    0,
                                                    checkOverlaps);
  // Place epoxy, casing, and silicon in APD.
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
  // Place coatings on aluminum APD case
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
  // Place APD in the world volume
  G4VPhysicalVolume* physAPD =
    new G4PVPlacement(0,
                      G4ThreeVector(0.0*mm, 0.0*mm, APDPlaceZ),
                      logicAPD,
                      "APD",
                      logicWorld,
                      false,
                      0,
                      checkOverlaps);
  // Place Al APD case in the world volume.
  G4VPhysicalVolume* physAlAPDCase =
    new G4PVPlacement(0,
                      G4ThreeVector(0.,0.,APDCasePlacementZ),
                      logicAlAPDCase,
                      "AlAPDCase",
                      logicWorld,
                      false,
                      0,
                      checkOverlaps);

  // Define the optical boundaries between physical volumes
  // Define the aluminum-layer1 boundary.
  G4OpticalSurface* OpLayer1AlSurface = new G4OpticalSurface("Layer1AlSurface");
  OpLayer1AlSurface->SetModel(unified);
  OpLayer1AlSurface->SetType(surfaceType(layer1MatStr, layer2MatStr));
  OpLayer1AlSurface->SetFinish(ground);
  OpLayer1AlSurface->SetSigmaAlpha(0.1);
  G4LogicalBorderSurface* Layer1AlSurface = new
    G4LogicalBorderSurface("Layer1AlSurface", physLayer1, physLayer2,
                           OpLayer1AlSurface);

  // Define the aluminum-world boundary.
  G4OpticalSurface* OpWorldAlSurface = new G4OpticalSurface("WorldAlSurface");
  OpWorldAlSurface->SetModel(unified);
  OpWorldAlSurface->SetType(surfaceType(worldMatStr, layer2MatStr));
  OpWorldAlSurface->SetFinish(ground);
  OpWorldAlSurface->SetSigmaAlpha(0.1);
  G4LogicalBorderSurface* WorldAlSurface = new
    G4LogicalBorderSurface("WorldAlSurface", physWorld, physLayer2,
                           OpWorldAlSurface);

  // Make skin surface on silicon with a certain efficiency.
  G4OpticalSurface* optSilicon = new G4OpticalSurface("optSilicon");
  optSilicon->SetModel(glisur);
  optSilicon->SetFinish(polished);
  optSilicon->SetType(surfaceType(siliconMatStr));
  optSilicon->SetMaterialPropertiesTable(generateSiSurfaceTable());
  G4LogicalSurface* skinSilicon = new G4LogicalSkinSurface("skinSilicon",
    logicSilicon, optSilicon);


  // Make a surface surounding the casing with a certain relfectivity
  G4OpticalSurface* optCasing = new G4OpticalSurface("optCasing");
  optCasing->SetModel(unified);
  optCasing->SetFinish(ground);
  optCasing->SetSigmaAlpha(0.2);
  optCasing->SetType(surfaceType(casingMatStr));
  optCasing->SetMaterialPropertiesTable(generateCeramicTable());
  G4LogicalSkinSurface* skinCasing = new G4LogicalSkinSurface("optCasing",
    logicCasing, optCasing);

  // Now define the aluminum casing-coating2 boundary.
  G4OpticalSurface* OpCoat2APDCaseSurface = 
    new G4OpticalSurface("Coating2APDCaseSurface");
  OpCoat2APDCaseSurface->SetModel(glisur);
  OpCoat2APDCaseSurface->SetType(surfaceType(APDAlCaseMatStr,
    coating2MatStr));
  OpCoat2APDCaseSurface->SetFinish(polished);
  G4LogicalBorderSurface* Coat2APDCaseSurface = new
  G4LogicalBorderSurface("Coating2APDCaseSurface", physAlCoating2,
    physAlAPDCase, OpCoat2APDCaseSurface);
  
  // Define a sensitive detector and assign it to epoxy.
  singCrysSiliconSD* siliconSD = new singCrysSiliconSD("singCrys/siliconSD",
    "SiliconHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(siliconSD);
  logicEpoxy->SetSensitiveDetector(siliconSD);

  return physWorld;
}
