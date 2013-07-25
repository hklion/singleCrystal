#include "singCrysRun.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"
#include "singCrysAnalysis.hh"

// Constructor for 'singCrysRun' class
singCrysRun::singCrysRun() : nEvent(0)
{
  // Get collection IDs for the quantities we want.
  G4SDManager* SDM = G4SDManager::GetSDMpointer();
  totalSurfCurrID = SDM->GetCollectionID("SiliconAPD/FSC");
  totalEDepID = SDM->GetCollectionID("SiliconAPD/eDep");
}

// Destructor: do nothing
singCrysRun::~singCrysRun()
{
}

// Function called by GEANT4 that records the hits into members of this class
void singCrysRun::RecordEvent(const G4Event* evt)
{
  G4double EDep = 0.;
  G4double SurfCurr = 0.;
  nEvent++;
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // Get pointer to hits collection for this event. If NULL, there were no
  // hits and we should skip the rest of this to avoid segfaults.
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  if (HCE)
  {
    // Get events for desired quantities
    G4THitsMap<G4double>* eventTotalSurfCurr =
      (G4THitsMap<G4double>*)(HCE->GetHC(totalSurfCurrID));
    G4THitsMap<G4double>* eventTotalEDep =
      (G4THitsMap<G4double>*)(HCE->GetHC(totalEDepID));
    // Add them to the member hit maps
    totalSurfCurr += *eventTotalSurfCurr;
    totalEDep += *eventTotalEDep;
    if ((*(eventTotalEDep))[0])
    {
      EDep = *((*(eventTotalEDep))[0]);
    }
    if ((*(eventTotalSurfCurr))[0])
    {
      SurfCurr = *((*(eventTotalSurfCurr))[0]);
    }
  }
  analysisManager->FillH1(1, EDep);
  analysisManager->FillH1(2, SurfCurr);
}

// Prints the amount of energy deposited in the silicon.
void singCrysRun::printEDepHits()
{
  G4double energy = 0.0;
  // Only try to get the value if there actually was energy deposited...
  if (totalEDep.entries())
  {
    // Get energy deposit
    energy = *(totalEDep[0]);
  }
  // Print.
  G4cout << "energy deposited: " << energy / keV << " keV" << G4endl;
}

// Prints the number of tracks entering the silicon per detector area
void singCrysRun::printSurfCurrHits()
{
  G4double curr = 0.0;
  // Only try to get the value if there actually were any tracks
  if (totalSurfCurr.entries())
  {
    // Get current.
    curr = *(totalSurfCurr[0]);
  }
  // Print.
  G4cout << "surface current: " << curr << "/mm2" << G4endl;
}
