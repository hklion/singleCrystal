#include "singCrysRunAction.hh"
#include "singCrysRun.hh"
#include "singCrysAnalysis.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

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

void singCrysRunAction::BeginOfRunAction(const G4Run* aRun)
{
  // Get/create analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(4);
  // Open output file
  G4String fileName = "singCrys";
  analysisManager->OpenFile(fileName);
  analysisManager->SetFirstHistoId(1);
  // Create histograms
  analysisManager->CreateH1("edep", "Edep in silicon", 100, 0., 100*keV);
  analysisManager->CreateH1("surfcurr", "Surface current", 100, 0., 400/mm2);
}

// At the end of run, print the run's energy deposition and current tally.
void singCrysRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  singCrysRun* theRun = (singCrysRun*)aRun;
  theRun->printEDepHits();
  theRun->printSurfCurrHits();
  delete G4AnalysisManager::Instance();
}
