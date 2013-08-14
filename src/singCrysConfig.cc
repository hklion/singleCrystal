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
  std::ifstream ini_file("config.txt");
  po::options_description desc;
  desc.add_options()
    ("crys_num_sides", po::value<int>()->default_value(4),
      "Number of sides of the crystal");
  po::variables_map vm;
  po::store(parse_config_file(ini_file, desc), vm);
  po::notify(vm);

}

singCrysConfig* singCrysConfig::GetInstance()
{
  static singCrysConfig pInstance;
  return &pInstance;
}

// Returns a pointer to the variables_map storing the config options.
//variables_map* singCrysConfig::GetMap()
//{
  //return &variables_map;
//}
