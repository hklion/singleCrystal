#include "singCrysUIsession.hh"

// Constructor
singCrysUIsession::singCrysUIsession() : G4UIsession()
{
  logfile.open("singleCrystal.log");
  errfile.open("singleCrystal.err");
}

singCrysUIsession::~singCrysUIsession()
{
  logfile.close();
  errfile.close();
}

G4int singCrysUIsession::ReceiveG4cout(const G4String& coutString)
{
  logfile << coutString << flush;
  return 0;
}

G4int singCrysUIsession::ReceiveG4cerr(const G4String& cerrString)
{
  errfile << cerrString << flush;
  return 0;
}
