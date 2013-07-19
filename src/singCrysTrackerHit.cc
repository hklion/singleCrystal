#include "singCrysTrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4Allocator<singCrysTrackerHit> singCrysTrackerHitAllocator;

// Empty constructor
singCrysTrackerHit::singCrysTrackerHit()
  : G4VHit(),
    fTrackID(-1),
    fAPDNb(-1),
    fEdep(0.),
    fPos(G4ThreeVector())
{}

// Destructor
singCrysTrackerHit::~singCrysTrackerHit() {}

// Constructor from other 'singCrysTrackerHit' object
singCrysTrackerHit::singCrysTrackerHit(const singCrysTrackerHit& right)
  : G4VHit()
{
  fTrackID = right.fTrackID;
  fAPDNb = right.fAPDNb;
  fEdep = right.fEdep;
  fPos = right.fPos;
}

// Overloading for assignment operator
const singCrysTrackerHit& singCrysTrackerHit::
  operator=(const singCrysTrackerHit& right)
{
  fTrackID = right.fTrackID;
  fAPDNb = right.fAPDNb;
  fEdep = right.fEdep;
  fPos = right.fPos;
  return *this;
}

// Overloading for equals operator
G4int singCrysTrackerHit::operator==(const singCrysTrackerHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

// Draw hit
void singCrysTrackerHit::Draw()
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
void singCrysTrackerHit::Print()
{
  G4cout
    << "  trackID: " << fTrackID << " APDNb: " << fAPDNb
    << "Edep: "
    << std::setw(7) << G4BestUnit(fEdep, "Energy")
    << " Position: "
    << std::setw(7) << G4BestUnit(fPos, "Length")
    << G4endl;
}
