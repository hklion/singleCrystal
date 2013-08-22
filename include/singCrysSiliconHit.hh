/*!
 * \file singCrysSiliconHit.hh
 * \brief Header file for singCrysSiliconHit class. Defines an object for a
 * sensitive detector hit.
 */

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

/*!
 * \class singCrysSiliconHit
 * \brief User-defined hit class that contains information about energy,
 * position, etc.
 *
 * User-defined hit class. Instances of this class are generated when a hit
 * is recorded in the sensitive detector. They contain information about energy,
 * which APD the hit was recorded on, the position of the hit, and the track
 * ID.
 */

class singCrysSiliconHit : public G4VHit
{
  public:
    //! Constructor
    singCrysSiliconHit();
    //! Copy constructor
    /*!
     * All fields are copied
     */
    singCrysSiliconHit(const singCrysSiliconHit& right);
    //! Destructor
    virtual ~singCrysSiliconHit();

    // operators
    //! Overload of the assignment operator
    /*!
     * All fields are copied
     */
    const singCrysSiliconHit& operator=(const singCrysSiliconHit& right);
    //! Overlaod of the equals operator
    /*!
     * Returns true iff the pointers of the objects are the same.
     */
    G4int operator==(const singCrysSiliconHit& right) const;

    //! Overload of new operator
    inline void* operator new(size_t);
    //! Overload of delete operator
    inline void operator delete(void*);

    // methods from base class
    //! Draws hits in visualization
    virtual void Draw();
    //! Prints hits
    /*
     * Prints track ID, which APD the hit occurred in, the position, and
     * the energy.
     */
    virtual void Print();

    // Set methods
    //! Mutator method for the track ID
    /*!
     * \param track Track ID
     */
    void SetTrackID(G4int track)  {fTrackID = track;};
    //! Mutator method for the APD number where the hit occurred
    /*!
     * \param APDNb APD number
     */
    void SetAPDNb(G4int APDNb)    {fAPDNb = APDNb;};
    //! Mutator method for the energy deposited in the hit
    /*!
     * \param de Energy deposited
     */
    void SetEdep(G4double de)     {fEdep = de;};
    //! Mutator method for the position of the hit
    /*!
     * \param xyz Position
     */
    void SetPos(G4ThreeVector xyz){fPos = xyz;};

    // Get methods
    //! Accessor method for the track ID
    /*!
     * \return Track ID
     */
    G4int GetTrackID() const    {return fTrackID;};
    //! Accessor method for the APD number where the hit occurred
    /*!
     * \return APD number
     */
    G4int GetAPDNb() const      {return fAPDNb;};
    //! Accessor method for the energy deposited in the hit
    /*!
     * \return Deposited energy
     */
    G4double GetEdep() const    {return fEdep;};
    //! Accessor method for the position of the hit
    /*!
     * \return Position
     */
    G4ThreeVector GetPos() const{return fPos;};

  private:
    //! Track ID
    G4int fTrackID;
    //! APD number where the hit occurred
    G4int fAPDNb;
    //! Energy deposited in the hit
    G4double fEdep;
    //! Position of the hit
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
