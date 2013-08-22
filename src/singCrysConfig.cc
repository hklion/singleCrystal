/*!
 * \file singCrysConfig.cc
 * \brief Implementation file for the singCrysConfig class. Read and process
 * configuration file
 */

#include "singCrysConfig.hh"
#include <boost/program_options.hpp>
#include <string>
#include <fstream>
#include <iostream>

// Initialize static members.
G4bool singCrysConfig::constructed = false;
G4String singCrysConfig::filename = "";

// Constructor. Reads in the file and stores the config options in 'vm'.
singCrysConfig::singCrysConfig()
{
  // Open the config file, and read in its contents
  std::ifstream ini_file(filename); // Open stream
  po::options_description desc;     // Declare option description object
  desc.add_options()                // Add options
    // Geometry options
    ("crysNumSides", po::value<G4int>()->default_value(4),
      "Number of sides of the crystal")
    ("crysSideLength", po::value<G4double>()->default_value(30.),
      "Length of flats (mm)")
    ("crysSizeZ", po::value<G4double>()->default_value(110.),
      "Z axis crystal length (mm)")
    ("layer1Thick", po::value<G4double>()->default_value(0.1),
      "Thickness of layer surrounding crystal(mm)")
    ("layer2Thick", po::value<G4double>()->default_value(0.1),
      "Thickness of layer surrounding layer1 (mm)")
    ("AlCoating1Z", po::value<G4double>()->default_value(0.1),
      "Thickness of top Al APD case coating (mm)")
    ("AlCoating2Z", po::value<G4double>()->default_value(0.5),
      "Thickness of bottom Al APD case coating (mm)")
    ("siliconXY", po::value<G4double>()->default_value(10.),
      "XY dimension of silicon APD chip (mm)")
    ("siliconZ", po::value<G4double>()->default_value(0.6),
      "Thickness of silicon APD chip (mm)")
    ("casingX", po::value<G4double>()->default_value(13.7),
      "X dimension of APD ceramic casing (mm)")
    ("casingY", po::value<G4double>()->default_value(14.5),
      "Y dimension of APD ceramic casing (mm)")
    ("casingZ", po::value<G4double>()->default_value(1.78),
      "Thickness of APD ceramic casing (mm)")
    ("epoxyX", po::value<G4double>()->default_value(11.7),
      "X dimension of epoxy on APD (mm)")
    ("epoxyY", po::value<G4double>()->default_value(12.5),
      "Y dimension of epoxy on APD (mm)")
    ("epoxyZ", po::value<G4double>()->default_value(0.6),
      "Thickness of epoxy on APD (mm)")
    ("APDAlCaseThick", po::value<G4double>()->default_value(5.),
      "Thickness of rim on Al APD case (mm)")
    ("APDAlCaseZ", po::value<G4double>()->default_value(10.),
      "Thickness of Al APD case (mm)")
    ("APDSlotDepth", po::value<G4double>()->default_value(5.),
      "How much of the crystal is in the Al case (mm)")
    // Materials
    ("crysMat", po::value<std::string>()->default_value("LYSO"),
      "Crystal material")
    ("layer1Mat", po::value<std::string>()->default_value("G4_Galactic"),
      "Material of layer 1")
    ("layer2Mat", po::value<std::string>()->default_value("G4_Al"),
      "Material of layer 2")
    ("worldMat", po::value<std::string>()->default_value("G4_Galactic"),
      "Material of the world")
    ("coating1Mat", po::value<std::string>()->default_value("G4_Galactic"),
      "Material of the top Al APD case coating")
    ("coating2Mat", po::value<std::string>()->default_value("Epoxy"))
    // Whether to check for volume overlaps
    ("checkOverlaps", po::value<G4bool>()->default_value(true),
      "Check overlaps in geometry?")
    // Single-value physics parameters
    ("ceramicRefl", po::value<G4double>()->default_value(0.9),
      "Reflectance of ceramic")
    ("scintYield", po::value<G4double>()->default_value(26.),
      "Scintillation yield (/MeV)")
    ("resScale", po::value<G4double>()->default_value(1.0),
      "Resolution scale")
    ("fastTimeConst", po::value<G4double>()->default_value(40.),
      "Time constant for fast component of scintillation (ns)")
    ("slowTimeConst", po::value<G4double>()->default_value(0.),
      "Time constant for slow component of scintillation (ns)")
    ("yieldRatio", po::value<G4double>()->default_value(1.),
      "Relative strength of fast component as fraction of total scint yeild")
    // Data files
    ("dataPath", po::value<std::string>()->default_value(""),
      "Path to data files")
    ("crysRIndexFile", po::value<std::string>()
      ->default_value("LYSO_RIndex.dat"), "File name for crys refractive index")
    ("crysAbsFile", po::value<std::string>()->default_value("LYSO_Abs.dat"),
      "File name for crys absorption length (values in mm)")
    ("crysRayFile", po::value<std::string>()->default_value("LYSO_Ray.dat"),
      "File name for crys Rayleigh scattering length (values in mm)")
    ("crysFastScintFile",
      po::value<std::string>()->default_value("LYSO_FastScint.dat"),
      "File name for crys fast component scintillation intensity")
    ("crysSlowScintFile",
      po::value<std::string>()->default_value(""),
      "File name for crys slow component scintillation intensity")
    ("SiQEffFile", po::value<std::string>()->default_value("Si_QEff.dat"),
      "File name for silicon quantum efficiency")
    ("SiReflFile", po::value<std::string>()->default_value("Si_Refl.dat"),
      "File name for silicon reflectivity")
    ("AlRIndexRFile", po::value<std::string>()->default_value("Al_RIndexR.dat"),
      "File name for real component of aluminum refractive index")
    ("AlRIndexIFile", po::value<std::string>()->default_value("Al_RIndexI.dat"),
      "File name for imaginary component of aluminum refractive index")
    // Options for singCrysPrimaryGeneratorAction
    ("n_particle", po::value<G4int>()->default_value(1),
      "Number of particles per event")
    ("particleName", po::value<std::string>()->default_value("e-"),
      "Type of particle")
    ("particleEnergy", po::value<G4double>()->default_value(105),
      "Energy of particle (MeV)")
    // Options for singCrysUIsession
    ("logfileName",
      po::value<std::string>()->default_value("singleCrystal.log"),
      "Name of log file")
    ("errfileName",
      po::value<std::string>()->default_value("singleCrystal.err"),
      "Name of error file")
    // Options for singCrysPhysicsList
    ("optVerbosity", po::value<G4int>()->default_value(0),
      "Verbosity for optical processes") 
    ;
  // Add to map of stored options
  po::store(parse_config_file(ini_file, desc), vm);
  po::notify(vm);
}

// Returns the pointer to the singleton class.
singCrysConfig* singCrysConfig::GetInstance()
{
  static singCrysConfig pInstance;
  return &pInstance;
}

// Wrapper function that takes the file name and returns the pointer to the
// singleton class.
singCrysConfig* singCrysConfig::LoadFile(G4String inFilename)
{
  // This function can only be called once, so if the class has already
  // been constructed, throw an error.
  if (constructed)
  {
    G4cerr << "Warning. File " << inFilename << " will not be read. Another"
      << " file has already been read in.";
  }
  // Update fields
  filename = inFilename;
  constructed = true;
  // Return the instance of the class
  return GetInstance();
}

//Returns a pointer to the variables_map storing the config options.
po::variables_map* singCrysConfig::GetMap()
{
  return &vm;
}
