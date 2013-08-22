/*!
 * \file singCrysPrimaryGeneratorAction.hh
 * \brief Header file for the singCrysPrimaryGeneratorAction class. Generates a
 * particle with specified parameters.
 */

#ifndef singCrysPrimaryGeneratorAction_h
#define singCrysPrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

/*!
 * \class singCrysPrimaryGeneratorAction
 * \brief Mandatory user-defined primary generator action class
 * 
 * Creates the particles that cause events. Creates a particle of a specified
 * type and energy, and shoots it at a specified location in a specified
 * direction.
 */

class singCrysPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    //! Constructor
    /*!
     * Initializes a particle gun and defines the particle type, number,
     * and energy.
     */
    singCrysPrimaryGeneratorAction();
    //! Destructor
    /*!
     * Cleans up particle gun
     */
    ~singCrysPrimaryGeneratorAction();

  public:
    //! Function called by GEANT that shoots the particle gun
    /*!
     * Determines where the particle gun should go based on the world size.
     * The particle starting position and momentum vector are determined here.
     * The particle gun is then used to generate an event. This function is
     * called by GEANT.
     */
    void GeneratePrimaries(G4Event* anEvent);

  private:
    //! Particle gun used to 'shoot' particles to generate an event
    G4ParticleGun* particleGun;
};

#endif
