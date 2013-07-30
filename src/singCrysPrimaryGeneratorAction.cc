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

#include "Randomize.hh"

// Constructor: create particle gun
singCrysPrimaryGeneratorAction::singCrysPrimaryGeneratorAction()
{
  // Declare the particle gun
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  
  // Choose the particle for the gun
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName = "e-";
  particleGun->
    SetParticleDefinition(particleTable->FindParticle(particleName));
  particleGun->SetParticleEnergy(511.*keV);
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
  particleGun->
    SetParticlePosition(G4ThreeVector(0.0, 0.0, -crystalZHalfLength * 1.1));
    //SetParticlePosition(G4ThreeVector(0.0, crystalXYHalfLength*1.8, gunZ0));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.0, 0.0, 1.0));
  particleGun->GeneratePrimaryVertex(anEvent);
}
