// Mandatory user action class. Generates a specified partcle at a specified
// energy.
#ifndef singCrysPrimaryGeneratorAction_h
#define singCrysPrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class singCrysPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    singCrysPrimaryGeneratorAction();
    ~singCrysPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent);

  private:
    G4ParticleGun* particleGun;
};

#endif
