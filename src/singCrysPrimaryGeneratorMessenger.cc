/*!
 * \file singCrysPrimaryGeneratorMessenger.cc
 * \brief Implementation file for singCrysPrimaryGeneratorMessenger class.
 * Handles UI commands specific to the singCrysPrimaryGeneratorAction class.
 */

#include "singCrysPrimaryGeneratorMessenger.hh"
#include "singCrysPrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

// Constructor: construct the necessary commands and directories
singCrysPrimaryGeneratorMessenger::singCrysPrimaryGeneratorMessenger
  (singCrysPrimaryGeneratorAction* PGA) : G4UImessenger(),
                                          PrimaryGeneratorAction(PGA)
{
  // Define directories
  singCrysDirectory = new G4UIdirectory("/singCrys/");
  singCrysDirectory->SetGuidance("UI commands specific to singCrys");
  PrimaryGeneratorDirectory = new G4UIdirectory("/singCrys/PGA/");
  PrimaryGeneratorDirectory->SetGuidance("Primary generator action control");

  // Define command to update the position of the particle gun.
  setPosCmd = new G4UIcmdWith3VectorAndUnit("/singCrys/PGA/pos", this);
  setPosCmd->SetGuidance("Define the position of the particle gun");
  setPosCmd->SetParameterName("posX", "posY", "posZ", false);
  setPosCmd->SetUnitCategory("Length");
  setPosCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

// Destructor: delete the dynamically allocated commands and directories
singCrysPrimaryGeneratorMessenger::~singCrysPrimaryGeneratorMessenger()
{
  delete singCrysDirectory;
  delete PrimaryGeneratorDirectory;
  delete setPosCmd;
}

// Updates values from UI commands
void singCrysPrimaryGeneratorMessenger::
  SetNewValue(G4UIcommand* command, G4String newVal)
{
  // Updates the position of the particle gun
  if (command == setPosCmd)
  {
    PrimaryGeneratorAction->setGunPos(setPosCmd->GetNew3VectorValue(newVal));
  }
}
