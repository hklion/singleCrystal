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
  analysisManager->CreateH1("edep", "Energy deposited", 100, 0., 0.001);
  analysisManager->CreateH1("nphotons", "Number of Photons", 100, 0., 500);
  analysisManager->CreateH1("crysEdep", "Energy deposited in crystal", 1000,
    0.51099, 0.511111);
}

// At the end of run, print the run's energy deposition and current tally.
void singCrysRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  singCrysRun* theRun = (singCrysRun*)aRun;
  theRun->printEDepHits();
  theRun->printNPhotonsHits();
  delete G4AnalysisManager::Instance();
}
