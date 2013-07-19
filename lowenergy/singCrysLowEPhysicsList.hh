#ifndef singCrysPhysicsList_h
#define singCrysPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
//class G4OpMieHG;
class G4OpBoundaryProcess;

class singCrysPhysicsList: public G4VUserPhysicsList
{
  public:
    singCrysPhysicsList();
    ~singCrysPhysicsList();

  protected:
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();
    // Helper functions for 'ConstructParticle'
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    // Helper functions for 'ConstructProcess
    void ConstructGeneral();
    void ConstructEM();
    void ConstructOp();
    void SetVerbose(G4int verbose);

  private:
    G4Cerenkov*          theCerenkovProcess;
    G4Scintillation*     theScintillationProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScatteringProcess;
//    G4OpMieHG*           theMieHGScatteringProcess;
    G4OpBoundaryProcess* theBoundaryProcess;

};

#endif
