#include "singCrysSiliconHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4Allocator<singCrysSiliconHit> singCrysSiliconHitAllocator;

// Empty constructor
singCrysSiliconHit::singCrysSiliconHit()
  : G4VHit(),
    fTrackID(-1),
    fAPDNb(-1),
    fEdep(0.),
    fPos(G4ThreeVector())
{}

// Destructor
singCrysSiliconHit::~singCrysSiliconHit() {}

// Constructor from other 'singCrysSiliconHit' object
singCrysSiliconHit::singCrysSiliconHit(const singCrysSiliconHit& right)
  : G4VHit()
{
  fTrackID = right.fTrackID;
  fAPDNb = right.fAPDNb;
  fEdep = right.fEdep;
  fPos = right.fPos;
}

// Overloading for assignment operator
const singCrysSiliconHit& singCrysSiliconHit::
  operator=(const singCrysSiliconHit& right)
{
  fTrackID = right.fTrackID;
  fAPDNb = right.fAPDNb;
  fEdep = right.fEdep;
  fPos = right.fPos;
  return *this;
}

// Overloading for equals operator
G4int singCrysSiliconHit::operator==(const singCrysSiliconHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

// Draw hit
void singCrysSiliconHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if (pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1., 0., 0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

// Print properties of hit
void singCrysSiliconHit::Print()
{
  G4cout
    << "  trackID: " << fTrackID << " APDNb: " << fAPDNb
    << "Edep: "
    << std::setw(7) << G4BestUnit(fEdep, "Energy")
    << " Position: "
    << std::setw(7) << G4BestUnit(fPos, "Length")
    << G4endl;
}
