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

  // Choose the default position as defined in the config file
  gunPos = G4ThreeVector(config["particleXPos"].as<G4double>(),
                         config["particleYPos"].as<G4double>(),
                         config["particleZPos"].as<G4double>());
}

// Destructor: delete the particle gun
singCrysPrimaryGeneratorAction::~singCrysPrimaryGeneratorAction()
{
  delete particleGun;
}

// Shoot the gun
void singCrysPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Called at the beginning of each event. First get some information about
  // the geometry in order to figure out where to place the gun.
  G4double worldZHalfLength = 0;
  G4double worldXYHalfLength = 0;
  // Get the world logical volume and its solid, which is a G4Box.
  G4LogicalVolume* worldLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox = NULL;
  if (worldLV)
    worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  if (worldBox)
  {
    // Get dimensions of the G4Box
    worldZHalfLength = worldBox->GetZHalfLength();
    worldXYHalfLength = worldBox->GetYHalfLength();
  }
  else
  {
    // Something went wrong!
    G4cerr << "World volume of box not found." << G4endl;
    G4cerr << "Perhaps the geometry has changed." << G4endl;
    G4cerr << "The gun will be placed in the center." << G4endl;
  }  
  // Determine crystal dimensions.
  G4double crystalZHalfLength = worldZHalfLength / 5;
  G4double crystalXYHalfLength = worldXYHalfLength / 5;
  // Choose random position along crystal length for z coordinate of gun
  G4double gunZ0 = crystalZHalfLength * (G4UniformRand() * 2 - 1);
  // Place gun at random z coordinate along crystal length and at a position
  // known to be outside of the crystal.
  particleGun->SetParticlePosition(gunPos);  
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.0, -1.0, 0.0));
  particleGun->GeneratePrimaryVertex(anEvent);
}

// Sets the particle gun position to a new G4ThreeVector
void singCrysPrimaryGeneratorAction::setGunPos(G4ThreeVector newVal)
{
  gunPos = newVal;
}
