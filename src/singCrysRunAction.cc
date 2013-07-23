#include "singCrysRunAction.hh"
#include "singCrysRun.hh"

singCrysRunAction::singCrysRunAction()
{;}

singCrysRunAction::~singCrysRunAction()
{;}

G4Run* singCrysRunAction::GenerateRun()
{
  return new singCrysRun;
}

void singCrysRunAction::EndOfRunAction(const G4Run* aRun)
{
  singCrysRun* theRun = (singCrysRun*)aRun;
  theRun->printEDepHits();
  theRun->printSurfCurrHits();
}
