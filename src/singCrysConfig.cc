#include "singCrysConfig.hh"
#include <boost/program_options.hpp>
#include <string>
#include <fstream>
#include <iostream>
// Class implementation for the singCrysConfig class. This class reads and
// stores the contents of a configuration file.

G4bool singCrysConfig::constructed = false;
G4String singCrysConfig::filename = "";

singCrysConfig::singCrysConfig()
{
  // Open the config file, and read in its contents
  std::ifstream ini_file(filename); // Open stream
  po::options_description desc;     // Declare option description object
  desc.add_options()                // Add options
    ("crysNumSides", po::value<G4int>(), "Number of sides of the crystal")
    ("crysSideLength", po::value<G4double>(), "Length of flats")
    ("crysSideZ", po::value<G4double>(), "Z axis crystal length")
    ("layer1Thick", po::value<G4double>(),
      "Thickness of layer surrounding crystal")
    ("layer2Thick", po::value<G4double>(),
      "Thickness of layer surrounding layer1")
    ("AlCoating1Z", po::value<G4double>(),
      "Thickness of top Al APD case coating")
    ("AlCoating2Z", po::value<G4double>(),
      "Thickness of bottom Al APD case coating")
    ("siliconXY", po::value<G4double>(), "XY dimension of silicon APD chip")
    ("siliconZ", po::value<G4double>(), "Thickness of silicon APD chip")
    ("casingX", po::value<G4double>(), "X dimension of APD ceramic casing")
    ("casingY", po::value<G4double>(), "Y dimension of APD ceramic casing")
    ("casingZ", po::value<G4double>(), "Thickness of APD ceramic casing")
    ("epoxyX", po::value<G4double>(), "X dimension of epoxy on APD")
    ("epoxyY", po::value<G4double>(), "Y dimension of epoxy on APD")
    ("epoxyZ", po::value<G4double>(), "Thickness of epoxy on APD")
    ("APDAlCaseThick", po::value<G4double>(),
      "Thickness of rim on Al APD case")
    ("APDAlCaseZ", po::value<G4double>(), "Thickness of Al APD case")
    ("APDSlotDepth", po::value<G4double>(),
      "How much of the crystal is in the Al case")
    ("crysMat", po::value<std::string>()->default_value("LYSO"),
      "Crystal material")
    ("layer1Mat", po::value<std::string>()->default_value("G4_Galactic"),
      "Material of layer 1")
    ("layer2Mat", po::value<std::string>()->default_value("G4_Al"),
      "Material of layer 2")
    ("worldMat", po::value<std::string>()->default_value("G4_Galactic"),
      "Material of the world")
    ("checkOverlaps", po::value<G4bool>(), "Check overlaps in geometry?")
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
  if (constructed)
  {
    G4cerr << "Warning. File " << inFilename << " will not be read. Another"
      << " file has already been read in.";
  }
  filename = inFilename;
  constructed = true;
  return GetInstance();
}

//Returns a pointer to the variables_map storing the config options.
po::variables_map* singCrysConfig::GetMap()
{
  return &vm;
}
