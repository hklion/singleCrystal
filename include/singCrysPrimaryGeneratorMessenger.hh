/*!
 * \file singCrysPrimaryGeneratorMessenger.hh
 * \brief Header file for singCrysPrimaryGeneratorMessenger class. Handles UI
 * commands specific to the singCrysPrimaryGeneratorAction class
 */ 

#ifndef singCrysPrimaryGeneratorMessenger_h
#define singCrysPrimaryGeneratorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class singCrysPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;

/*!
 * \class singCrysPrimaryGeneratorMessenger
 * \brief Handles UI commands specific to the singCrysPrimaryGeneratorAction
 * class
 *
 * Constructs directories and commands for the interactive input of the
 * position of the particle gun. Also passes the udpated quantities to the
 * singCrysPrimaryGeneratorAction class.
 */

class singCrysPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    //! Constructor
    /*!
     * Defines the directories and commands needed to update the desired
     * quantities.
     * \param PGA Pointer to the singCrysPrimaryGeneratorAction class, which
     * is then stored in the relevant field.
     */
    singCrysPrimaryGeneratorMessenger(singCrysPrimaryGeneratorAction* PGA);
    //! Destructor
    /*!
     * Deletes directories and commands created in constructor
     */
    virtual ~singCrysPrimaryGeneratorMessenger();
    //! Updates the values of the interactively changed properties
    /*!
     * Called when a new UI command occurs. If the command matches a known
     * command, parse the input and update the corresponding quantity.
     * \param command The command passed by the UI manager.
     * \param newVal The string inputted along with the command.
     */
    virtual void SetNewValue(G4UIcommand* command, G4String newVal);

  private:
    //! Pointer to the affiliatd singCrysPrimaryGeneratorAction class
    singCrysPrimaryGeneratorAction* PrimaryGeneratorAction;

    //! Pointer to the directory for the UI commands for singCrys
    G4UIdirectory* singCrysDirectory;
    //! Pointer to the directory for the UI commands for PrimaryGeneratorAction
    G4UIdirectory* PrimaryGeneratorDirectory;

    //! Command to update the particle gun position
    G4UIcmdWith3VectorAndUnit* setPosCmd;
};

#endif
