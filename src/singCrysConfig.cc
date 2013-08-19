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
    ("crysNumSides", po::value<G4int>()->default_value(4),
      "Number of sides of the crystal")
    ("crysSideLength", po::value<G4double>()->default_value(30.),
      "Length of flats")
    ("crysSizeZ", po::value<G4double>()->default_value(110.),
      "Z axis crystal length")
    ("layer1Thick", po::value<G4double>()->default_value(0.1),
      "Thickness of layer surrounding crystal")
    ("layer2Thick", po::value<G4double>()->default_value(0.1),
      "Thickness of layer surrounding layer1")
    ("AlCoating1Z", po::value<G4double>()->default_value(0.1),
      "Thickness of top Al APD case coating")
    ("AlCoating2Z", po::value<G4double>()->default_value(0.5),
      "Thickness of bottom Al APD case coating")
    ("siliconXY", po::value<G4double>()->default_value(10.),
      "XY dimension of silicon APD chip")
    ("siliconZ", po::value<G4double>()->default_value(0.6),
      "Thickness of silicon APD chip")
    ("casingX", po::value<G4double>()->default_value(13.7),
      "X dimension of APD ceramic casing")
    ("casingY", po::value<G4double>()->default_value(14.5),
      "Y dimension of APD ceramic casing")
    ("casingZ", po::value<G4double>()->default_value(1.78),
      "Thickness of APD ceramic casing")
    ("epoxyX", po::value<G4double>()->default_value(11.7),
      "X dimension of epoxy on APD")
    ("epoxyY", po::value<G4double>()->default_value(12.5),
      "Y dimension of epoxy on APD")
    ("epoxyZ", po::value<G4double>()->default_value(0.6),
      "Thickness of epoxy on APD")
    ("APDAlCaseThick", po::value<G4double>()->default_value(5.),
      "Thickness of rim on Al APD case")
    ("APDAlCaseZ", po::value<G4double>()->default_value(10.),
      "Thickness of Al APD case")
    ("APDSlotDepth", po::value<G4double>()->default_value(5.),
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
