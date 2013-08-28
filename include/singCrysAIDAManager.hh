/*!
 * \file singCrysAIDAManager.hh
 * \brief Header file for the singCrysAIDAManager class. Manages the AIDA-based
 * analysis.
 */

#ifndef singCrysAIDAManager_h
#define singCrysAIDAManager_h 1

#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include <AIDA/AIDA.h>

namespace AIDA{
class IAnalysisFactory;
class ITree;
class IHistogramFactory;
class ITupleFactory;
class IPlotter;
}

class G4Track;

/*!
 * \class singCrysAIDAManager
 * \brief AIDA manager class
 *
 * Singleton class that manages the objects used for AIDA-based analysis.
 * Handles the output to a .root file, as well as the various types of
 * objects and factories used. This class is only used if cmake was able to
 * find the AIDA libraries when it was last run. Otherwise, AIDA will not be
 * used, and all lines concerning its implementation will be excluded using
 * a preprocessor directive.
 */
class singCrysAIDAManager {
public:
  //! Destructor
  /*!
   * Attempts to write data to an AIDA file. Deletes all dynamically allocated
   * members.
   */
  virtual ~singCrysAIDAManager();
  //! Returns a pointer to the instance of the class
  /*!
   * If an instance of the class has not already been created, create one. In
   * either case, return the pointer to the class.
   * \return A pointer to the instance of the class.
   */
  static singCrysAIDAManager* getInstance();
  //! Destroys the instance of the class
  /*!
   * To be called from outside of the class. Deletes the instance of the class
   * if one has been created.
   */
  static void dispose();
  //! Accessor method for the histogram factory.
  /*!
   * Returns the pointer to the histogram factory used
   * \return A pointer to the histogram factory
   */
  AIDA::IHistogramFactory* getHistogramFactory();
  //! Accessor method for the tuple factory.
  /*!
   * Returns the pointer to the tuple factory used
   * \return A pointer to the tuple factory
   */
  AIDA::ITupleFactory* getTupleFactory();
  //! Accessor method for the plotter
  /*!
   * Returns the pointer to the plotter used
   * \return A pointer to the plotter
   */
  AIDA::IPlotter* getPlotter();

private:
  //! Constructor
  /*!
   * Hooks an AIDA compliant analysis system and creates the approprite
   * factories and plotters to do the analysis.
   */
  singCrysAIDAManager();
  //! Pointer to the singleton instance of this class
  static singCrysAIDAManager* fInstance;
  //! Pointer to the analysis factory
  AIDA::IAnalysisFactory* fAnalysisFactory;
  //! Pointer to the histogram factory
  AIDA::IHistogramFactory* fFactory;
  //! Pointer to the tuple factory
  AIDA::ITupleFactory* tFactory;
  //! Pointer to the plotter
  AIDA::IPlotter* fPlotter;
  //! Pointer to the tree
  AIDA::ITree* fTree;
};

#endif

