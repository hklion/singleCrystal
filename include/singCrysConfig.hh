#ifndef singCrysConfig_h
#define singCrysConfig_h 1

#include "globals.hh"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class singCrysConfig {

  public:
    static singCrysConfig* GetInstance();
    static G4String filename;
    po::variables_map* GetMap();

  protected:
    singCrysConfig();
    singCrysConfig(const singCrysConfig&);
    singCrysConfig& operator=(const singCrysConfig&);
    po::variables_map* vm;
};

#endif
