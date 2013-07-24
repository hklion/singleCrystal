#include "singCrysRunAction.hh"
#include "singCrysRun.hh"

// Constructor: nothing
singCrysRunAction::singCrysRunAction()
{;}

// Destructor: nothing
singCrysRunAction::~singCrysRunAction()
{;}

// Generates custom Run object
G4Run* singCrysRunAction::GenerateRun()
{
  return new singCrysRun;
}

// At the end of run, print the run's energy deposition and current tally.
void singCrysRunAction::EndOfRunAction(const G4Run* aRun)
{
  singCrysRun* theRun = (singCrysRun*)aRun;
  theRun->printEDepHits();
  theRun->printSurfCurrHits();
}
