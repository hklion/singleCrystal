/*!
 * \file singCrysEventAction.hh
 * \brief Header file for the singCrysEventAction class. User defined event
 * action class.
 */

#ifndef singCrysEventAction_h
#define singCrysEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#ifdef ROOT_USE
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"
#endif // ROOT_USE

#ifdef AIDA_USE
#include "AIDA/AIDA.h"
using namespace AIDA;
#endif // AIDA_USE

class singCrysEventActionMessenger;

/*!
 * \class singCrysEventAction
 * \brief User-defined optional event action class. Defines events that occur
 * before and after each event.
 *
 * User-defined optional event action class. This class defines the actions
 * that occur before and after each event. This is used to store and process
 * data from the simulation. Either or both of ROOT or AIDA are used for
 * storing the data. The analysis will be done with the libraries that CMake
 * found the last time it was run. For instance, ROOT analysis will be included
 * if and only if CMake found the ROOT libraries. Otherwise, the code using
 * ROOT will be excluded using a preprocessor directive. Similar exclusion
 * is done for AIDA.
 *
 * The ROOT analysis outputs a ROOT file with two branches: event ID (int) and
 * energy (std::vector). The event ID is the ID used by GEANT, starts at 0 and
 * is incremented by one for each event. The energy vector is a vector
 * containing the amount of energy deposited by each hit of a given event.
 *
 * The AIDA analysis outputs a fle with three branches: event ID (int),
 * deposit ID (int), and energy (double). The event ID is determined as in the
 * ROOT analysis. The deposit ID is the index of the hit in the hits
 * collection. The deposit energy is the energy deposited by that hit.
 */
class singCrysEventAction : public G4UserEventAction
{
  public:
    //! Constructor
    /*!
     * Gets the hits collection, sets the verbosity, and starts the AIDA and/or
     * ROOT analysis. If AIDA is being used, it generates at tuple with the
     * appropriate data fields. If ROOT is being used, it generates a file and
     * tree with branches for the data members to be saved.
     */
    singCrysEventAction();
    //! Destructor
    /*!
     * If AIDA is being used, it deletes the instance of the
     * singCrysAIDAManager class (writitng the contents to a file in the
     * process). If ROOT is being used, it writes the data to file, and closes
     * it.
     */
    virtual ~singCrysEventAction();

    //! Actions to be carried out at the beginning of each event.
    virtual void BeginOfEventAction(const G4Event*);
    //! Actions to be carreid out at the end of each event
    /*!
     * Processes the hits, outputting the necessary data to the AIDA or ROOT
     * interface.
     */
    virtual void EndOfEventAction(const G4Event*);

  private:
    //! ID of the silicon hits collection
    G4int fSiHCID;
    //! Verbosity level
    G4int fVerboseLevel;
    
#ifdef ROOT_USE
    //! Pointer to the ROOT TFile object
    TFile *myFile;
    //! Pointer to the TTree
    TTree *myTree;
    //! ID number of the event 
    G4int eventID;
    //! Vector to store APD ID quantities
    std::vector<double> APDID;
    //! Vector to store energies of hits
    std::vector<double> energy;
    //! Vector to store x position of hits
    std::vector<double> xPos;
    //! Vector to store y position of hits
    std::vector<double> yPos;
    //! Vector to store z position of hits
    std::vector<double> zPos;
    //! Vector to store x momentum of hits
    std::vector<double> xPVec;
    //! Vector to store y momentum of hits
    std::vector<double> yPVec;
    //! Vector to store z momentum of hits
    std::vector<double> zPVec;
#endif // ROOT_USE

#ifdef AIDA_USE
    //! Tuple used in AIDA analysis
    ITuple* fTuple;
#endif // AIDA_USE

  public:
    //! Mutator method for the verbosity
    /*!
     * \param val Value to which to set the verbosity
     */
    inline void SetVerbose(G4int val) { fVerboseLevel = val; }
    //! Accessor method for the verbosity
    /*!
     * \return The verbosity
     */
    inline G4int GetVerbose() const { return fVerboseLevel; }
};

#endif
