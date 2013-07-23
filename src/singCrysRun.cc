#include "singCrysRun.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"

singCrysRun::singCrysRun() : nEvent(0)
{
  G4SDManager* SDM = G4SDManager::GetSDMpointer();
  totalSurfCurrID = SDM->GetCollectionID("SiliconAPD/FSC");
  totalEDepID = SDM->GetCollectionID("SiliconAPD/eDep");
}

singCrysRun::~singCrysRun()
{
}

void singCrysRun::RecordEvent(const G4Event* evt)
{
  nEvent++;
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  if (!HCE)
    return;
  G4THitsMap<G4double>* eventTotalSurfCurr =
    (G4THitsMap<G4double>*)(HCE->GetHC(totalSurfCurrID));
  G4THitsMap<G4double>* eventTotalEDep =
    (G4THitsMap<G4double>*)(HCE->GetHC(totalEDepID));
  totalSurfCurr += *eventTotalSurfCurr;
  totalEDep += *eventTotalEDep;
  G4cout << "Adding worked." << G4endl;
}

void singCrysRun::printEDepHits()
{
  G4double value = *(totalEDep[0]);
  G4cout << "edep: " << value / MeV << " MeV" << G4endl;
}

void singCrysRun::printSurfCurrHits()
{
  G4double value =  *(totalSurfCurr[0]);
  G4cout << "flux: " << value << "/mm2" << G4endl;
}
