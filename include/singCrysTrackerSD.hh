// User-defined sensitive detector class

#ifndef singCrysTrackerSD_h
#define singCrysTrackerSD_h 1

#include "G4VSensitiveDetector.hh"
#include "singCrysTrackerHit.hh"
#include <vector>

class G4Step;
class G4HCofThisEvent;

class singCrysTrackerSD : public G4VSensitiveDetector
{
  public:
    singCrysTrackerSD(const G4String& name,
               const G4String& hitsCollectionName);
    virtual ~singCrysTrackerSD();

    // methods from base classes
    virtual void Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    singCrysTrackerHitsCollection* fHitsCollection;
};

#endif
