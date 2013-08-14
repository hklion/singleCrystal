#include "singCrysConfig.hh"
#include <boost/program_options.hpp>
#include <string>
#include <fstream>
#include <iostream>
// Class implementation for the singCrysConfig class. This class reads and
// stores the contents of a configuration file.

G4String singCrysConfig::filename = "config.txt";

singCrysConfig::singCrysConfig()
{
  // Open the config file, and read in its contents
  std::ifstream ini_file(filename); // Open stream
  po::options_description desc;     // Declare option description object
  desc.add_options()                // Add options
    ("crys_num_sides", po::value<G4int>()->default_value(4),
      "Number of sides of the crystal");
  // Add to map of stored options
  po::variables_map vm;
  po::store(parse_config_file(ini_file, desc), vm);
  po::notify(vm);
}

// Returns the pointer to the singleton class.
singCrysConfig* singCrysConfig::GetInstance()
{
  static singCrysConfig pInstance;
  return &pInstance;
}

//Returns a pointer to the variables_map storing the config options.
variables_map* singCrysConfig::GetMap()
{
  return &variables_map;
}
