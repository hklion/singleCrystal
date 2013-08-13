// Silicon hit class.
//
// Defines data members to store trackID, APDNb, energy deposit and position
// of particles in a selected volume

#ifndef singCrysSiliconHit_h
#define singCrysSiliconHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class singCrysSiliconHit : public G4VHit
{
  public:
    singCrysSiliconHit();
    singCrysSiliconHit(const singCrysSiliconHit& right);
    virtual ~singCrysSiliconHit();

    // operators
    const singCrysSiliconHit& operator=(const singCrysSiliconHit& right);
    G4int operator==(const singCrysSiliconHit& right) const;

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

typedef G4THitsCollection<singCrysSiliconHit> singCrysSiliconHitsCollection;

extern G4Allocator<singCrysSiliconHit> singCrysSiliconHitAllocator;

inline void* singCrysSiliconHit::operator new(size_t)
{
  void *hit;
  hit = (void *) singCrysSiliconHitAllocator.MallocSingle();
  return hit;
}

inline void singCrysSiliconHit::operator delete(void *hit)
{
  singCrysSiliconHitAllocator.FreeSingle((singCrysSiliconHit*) hit);
}

#endif
