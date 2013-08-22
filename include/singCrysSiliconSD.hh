/*!
 * \file singCrysSiliconSD.hh
 * \brief Header file for the singCrysSiliconSD class. Defines a sensitive
 * detector
 */

#ifndef singCrysSiliconSD_h
#define singCrysSiliconSD_h 1

#include "G4VSensitiveDetector.hh"
#include "singCrysSiliconHit.hh"
#include <vector>

class G4Step;
class G4HCofThisEvent;

/*!
 * \class singCrysSiliconSD
 * \brief User-defined sensitive detector class.
 * 
 * Processes hits that occur in the volume it is assigned to, and creates the
 * appropriate singCrysSiliconHit objects.
 */

class singCrysSiliconSD : public G4VSensitiveDetector
{
  public:
    //! Constructor
    singCrysSiliconSD(const G4String& name,
               const G4String& hitsCollectionName);
    //! Destructor
    virtual ~singCrysSiliconSD();

    // methods from base classes
    //! Initializes hit collections
    /*!
     * Initializes the singCrysSiliconHitsCollection and adds it to the hit
     * collection of this event.
     * \param hce The hit collection of this event
     */
    virtual void Initialize(G4HCofThisEvent* hce);
    //! Process hits
    /*!
     * Function called by GEANT4 to proceses the hits. Makes singCrysSiliconHit
     * objects and puts in the relevant information about the hit. Also prints
     * the hit information.
     * \param step The step of this event
     * \param history The touchable history
     */
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    //! Prints summary information about the hits
    /*!
     * Called by GEANT4 at the end of the event. Prints the number of hits
     * recorded during the event.
     * \param hitCollection The hit collection of this event
     */
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    //! Hit collection object
    singCrysSiliconHitsCollection* fHitsCollection;
};

#endif
