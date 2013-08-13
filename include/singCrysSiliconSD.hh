// User-defined sensitive detector class

#ifndef singCrysSiliconSD_h
#define singCrysSiliconSD_h 1

#include "G4VSensitiveDetector.hh"
#include "singCrysSiliconHit.hh"
#include <vector>

class G4Step;
class G4HCofThisEvent;

class singCrysSiliconSD : public G4VSensitiveDetector
{
  public:
    singCrysSiliconSD(const G4String& name,
               const G4String& hitsCollectionName);
    virtual ~singCrysSiliconSD();

    // methods from base classes
    virtual void Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    singCrysSiliconHitsCollection* fHitsCollection;
};

#endif
