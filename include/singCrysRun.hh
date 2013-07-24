// User-defined run action class. Stores scores for energy deposition and
// current through the APD. Also contains methods to output data.
#ifndef singCrysRun_h
#define singCrysRun_h 1

#include "globals.hh"
#include "G4Run.hh"

#include "G4THitsMap.hh"

class G4Event;

class singCrysRun : public G4Run
{
  public:
    singCrysRun();
    virtual ~singCrysRun();

  public:
    virtual void RecordEvent(const G4Event*);

  private:
    G4int nEvent;
    G4int totalEDepID, totalSurfCurrID;
    G4THitsMap<G4double> totalEDep;
    G4THitsMap<G4double> totalSurfCurr;
  public:
    void printEDepHits();
    void printSurfCurrHits();
};

#endif
