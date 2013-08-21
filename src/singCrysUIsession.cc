#include "singCrysUIsession.hh"
#include "singCrysConfig.hh"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Constructor. Open files.
singCrysUIsession::singCrysUIsession() : G4UIsession()
{
  po::variables_map config = *(singCrysConfig::GetInstance()->GetMap());
  G4String logfileName = (G4String) config["logfileName"].as<std::string>();
  G4String errfileName = (G4String) config["errfileName"].as<std::string>();
  logfile.open(logfileName);
  errfile.open(errfileName);
}

// Destructor
singCrysUIsession::~singCrysUIsession()
{
  logfile.close();
  errfile.close();
}

// Diverts G4cout to file
G4int singCrysUIsession::ReceiveG4cout(const G4String& coutString)
{
  logfile << coutString << flush;
  return 0;
}

// Diverts G4cerr to file
G4int singCrysUIsession::ReceiveG4cerr(const G4String& cerrString)
{
  errfile << cerrString << flush;
  return 0;
}
