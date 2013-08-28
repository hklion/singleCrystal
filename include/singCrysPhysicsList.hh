/*!
 * \file singCrysPhysicsList.hh
 * \brief Header file for the singCrysPhysicsList class. Defines all
 * necessary physics processes
 */

// Physics list for the simulation of optical photons in a crystal. Also
// includes processes so that the particle's step length and energy can be
// limited.
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

/*!
 * \class singCrysPhysicsList
 * \brief Class for defining all physics processes.
 *
 * Defines all particles and physics processes. Different types of processes
 * are defined in different helper functions.
 */

class singCrysPhysicsList: public G4VUserPhysicsList
{
  public:
    //! Constructor
    /*!
     * Initializes all process pointer fields as NULL.
     */
    singCrysPhysicsList();
    //! Destructor
    ~singCrysPhysicsList();

  protected:
    //! Constructs all particles
    /*!
     * Calls helper functions to construct bosons, leptons, mesons, and
     * baryons.
     */
    void ConstructParticle();
    //! Constructs all processes
    /*!
     * Constructs and assigns all processes by calling helper functions for
     * transportation, general, electromagnetic, and optical processes.
     */
    void ConstructProcess();
    //! Set production cut values
    /*!
     * Sets particle production cuts and prints the cuts.
     */
    void SetCuts();
    // Helper functions for 'ConstructParticle'
    //! Helper function that constructs all bosons
    /*!
     * Constructs, geantino, gamma, and optical photon.
     */
    void ConstructBosons();
    //! Helper function that constructs all leptons
    /*!
     * Constructs electron, positron, mu+, mu-, electron and mu (anti)neutrino.
     */
    void ConstructLeptons();
    //! Helper function that constructs all mesons
    /*!
     * Constructs pi+, pi-, and pi0.
     */
    void ConstructMesons();
    //! Helper function that constructs all baryons
    /*!
     * Constructs proton, antiproton, neutron, and antineutron.
     */
    void ConstructBaryons();
    // Helper functions for 'ConstructProcess'
    //! Helper function that constructs general physics processes
    /*!
     * Adds decay process to the relevant particles.
     */
    void ConstructGeneral();
    //! Helper function that constructs electromagnetic physics processes
    /*!
     * Adds gamma conversion, Compton scattering, and the photoelctric effect
     * to gamma particles. Adds multiple scattering, ionization, and
     * Bremsstrahlung to electrons and positrons. Also adds annihilation to
     * positron. Adds multiple scattering, ionization, Brehmsstrahlung,
     * pair production, and muon capture to mu+ and mu-. Adds multiple
     * scattering and ionization to all other particles other than geantinos.
     */
    void ConstructEM();
    //! Helper function that constructs optical physics processes
    /*!
     * Adds Cerenkov radiation, scintillation, absorption, Rayleigh
     * scattering, and boundary handling. Also defines some Cerenkov and
     * scintillation parameters, as well as verbosity for optical processes.
     */
    void ConstructOp();
    //! Sets verbosity of optical processes
    void SetVerbose(G4int verbose);

  private:
    // Pointers to the various processes
    //! Pointer to the Cerenkov radiation process
    G4Cerenkov*             theCerenkovProcess;
    //! Pointer to the scintillation process
    G4Scintillation*        theScintillationProcess;
    //! Pointer to the optical absorption process
    G4OpAbsorption*         theAbsorptionProcess;
    //! Pointer to the Rayleigh scattering process
    G4OpRayleigh*           theRayleighScatteringProcess;
//    G4OpMieHG*            theMieHGScatteringProcess;
    //! Pointer to the optical boundary process
    G4OpBoundaryProcess*    theBoundaryProcess;
};

#endif
