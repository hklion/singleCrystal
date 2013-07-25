// User-defined run action class. Allows for information from the scorer to be
// dumped.
#ifndef singCrysRunAction_h
#define singCrysRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class singCrysRunAction : public G4UserRunAction
{
  public:
    singCrysRunAction();
    ~singCrysRunAction();

  public:
    G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
};

#endif
