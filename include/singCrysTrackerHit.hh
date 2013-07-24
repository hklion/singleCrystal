// Tracker hit class.
//
// Defines data members to store trackID, APDNb, energy deposit and position
// of particles in a selected volume

#ifndef singCrysTrackerHit_h
#define singCrysTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class singCrysTrackerHit : public G4VHit
{
  public:
    singCrysTrackerHit();
    singCrysTrackerHit(const singCrysTrackerHit& right);
    virtual ~singCrysTrackerHit();

    // operators
    const singCrysTrackerHit& operator=(const singCrysTrackerHit& right);
    G4int operator==(const singCrysTrackerHit& right) const;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID(G4int track)  {fTrackID = track;};
    void SetAPDNb(G4int APDNb)    {fAPDNb = APDNb;};
    void SetEdep(G4double de)     {fEdep = de;};
    void SetPos(G4ThreeVector xyz){fPos = xyz;};

    // Get methods
    G4int GetTrackID() const    {return fTrackID;};
    G4int GetAPDNb() const      {return fAPDNb;};
    G4double GetEdep() const    {return fEdep;};
    G4ThreeVector GetPos() const{return fPos;};

  private:
    G4int fTrackID;
    G4int fAPDNb;
    G4double fEdep;
    G4ThreeVector fPos;
};

typedef G4THitsCollection<singCrysTrackerHit> singCrysTrackerHitsCollection;

extern G4Allocator<singCrysTrackerHit> singCrysTrackerHitAllocator;

inline void* singCrysTrackerHit::operator new(size_t)
{
  void *hit;
  hit = (void *) singCrysTrackerHitAllocator.MallocSingle();
  return hit;
}

inline void singCrysTrackerHit::operator delete(void *hit)
{
  singCrysTrackerHitAllocator.FreeSingle((singCrysTrackerHit*) hit);
}

#endif
