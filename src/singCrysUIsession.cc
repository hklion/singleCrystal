#include "singCrysUIsession.hh"

// Constructor. Open files.
singCrysUIsession::singCrysUIsession() : G4UIsession()
{
  logfile.open("singleCrystal.log");
  errfile.open("singleCrystal.err");
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
