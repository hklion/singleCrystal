/*!
 * \file singCrysSiliconSD.cc
 * \brief Implementation file for the singCrysSiliconSD class. Defines a
 * sensitive detector.
 */

#include "singCrysSiliconSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

// Constructor
singCrysSiliconSD::singCrysSiliconSD(const G4String& name,
                                     const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name),
    fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

// Destructor
singCrysSiliconSD::~singCrysSiliconSD()
{}

// Initializes the hits collection associated with the detector
void singCrysSiliconSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection = new singCrysSiliconHitsCollection(SensitiveDetectorName,
    collectionName[0]);

  // Add this collection in hce
  G4int hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
}

// Processes the hits in the volume
G4bool singCrysSiliconSD::ProcessHits(G4Step* aStep,
                                      G4TouchableHistory*)
{
  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.) return false;
  // Define a new hit and pass it the appropriate values
  singCrysSiliconHit* newHit = new singCrysSiliconHit();
  newHit->SetTrackID (aStep->GetTrack()->GetTrackID());
  newHit->SetAPDNb(aStep->GetPreStepPoint()->GetTouchableHandle()
                                           ->GetCopyNumber());
  newHit->SetEdep(edep);
  newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());

  fHitsCollection->insert(newHit);
  newHit->Print();
  return true;
}

// Outputs information about the event
void singCrysSiliconSD::EndOfEvent(G4HCofThisEvent*)
{
  if (verboseLevel > 1)
  {
    G4int nofHits = fHitsCollection->entries();
    G4cout << "\n---------> In this event there are " << nofHits
           << " hits in the tracker chambers: " << G4endl;
    for (G4int i = 0; i < nofHits; i++) (*fHitsCollection)[i]->Print();
  }
}
