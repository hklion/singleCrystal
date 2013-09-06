/*!
 * \file singCrysPrimaryGeneratorAction.cc
 * \brief Implementation file for the singCrysPrimaryGeneratorAction class. 
 * Generates a particle with specified parameters.
 */

#include "singCrysPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <boost/program_options.hpp>
#include "singCrysConfig.hh"
#include "singCrysPrimaryGeneratorMessenger.hh"

#include "Randomize.hh"

namespace po = boost::program_options;

// Constructor: create particle gun
singCrysPrimaryGeneratorAction::singCrysPrimaryGeneratorAction()
{
  po::variables_map config = *(singCrysConfig::GetInstance()->GetMap());
  messenger = new singCrysPrimaryGeneratorMessenger(this);
  // Declare the particle gun
  G4int n_particle = config["n_particle"].as<G4int>();
  particleGun = new G4ParticleGun(n_particle);

  // Choose the particle for the gun
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName = (G4String) config["particleName"].as<std::string>();
  particleGun->
    SetParticleDefinition(particleTable->FindParticle(particleName));
  G4double particleEnergy = config["particleEnergy"].as<G4double>();
  particleGun->SetParticleEnergy(particleEnergy * MeV);

  // Choose the default position and momentum direction as defined in the
  // config file
  gunPos = G4ThreeVector(config["particleXPos"].as<G4double>(),
                         config["particleYPos"].as<G4double>(),
                         config["particleZPos"].as<G4double>());
  gunPDir = G4ThreeVector(config["momentumX"].as<G4double>(),
                          config["momentumY"].as<G4double>(),
                          config["momentumZ"].as<G4double>());
}

// Destructor: delete the particle gun
singCrysPrimaryGeneratorAction::~singCrysPrimaryGeneratorAction()
{
  delete particleGun;
}

// Shoot the gun
void singCrysPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Set values for particle position and momentum direction
  particleGun->SetParticlePosition(gunPos);  
  particleGun->SetParticleMomentumDirection(gunPDir);
  particleGun->GeneratePrimaryVertex(anEvent);
}

// Sets the particle gun position to a new G4ThreeVector
void singCrysPrimaryGeneratorAction::setGunPos(G4ThreeVector newVal)
{
  gunPos = newVal;
}
