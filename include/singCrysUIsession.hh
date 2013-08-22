/*!
 * \file singCrysUIsession.hh
 * \brief Header file for the singCrysUIsession class. Diverts G4cout and
 * G4cerr to files.
 */

#ifndef singCrysUIsession_h
#define singCrysUIsession_h 1

#include "G4UIsession.hh"
#include <fstream>

using namespace std;

/*!
 * \class singCrysUIsession
 * \brief User-defined UI session class that diverts G4cout and G4cerr to
 * files.
 *
 * User-defined UI session class that user-defined files and diverts G4cout
 * and G4cerr to them.
 */

class singCrysUIsession : public G4UIsession
{
  public:
    //! Constructor
    /*!
     * Gets the file names from singCrysConfig and opens them.
     */
    singCrysUIsession();
    //! Destructor
    /*!
     * Closes the files opened in the constructor.
     */
    ~singCrysUIsession();
    //! Writes contents of G4cout to file
    virtual G4int ReceiveG4cout(const G4String& coutString);
    //! Writes contents of G4cerr to file
    virtual G4int ReceiveG4cerr(const G4String& cerrString);
  private:
    //! File to write G4cout to
    ofstream logfile;
    //! File to write G4cerr to
    ofstream errfile;
};
#endif
