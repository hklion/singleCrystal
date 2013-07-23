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
    virtual void EndOfRunAction(const G4Run*);
};

#endif
