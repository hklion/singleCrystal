/*!
 * \file singCrysAIDAManager.cc
 * \brief Implementation file for the singCrysAIDAManager class. Manages the
 * AIDA-based analysis.
 */

#ifdef AIDA_USE

#include <fstream>
#include "G4ios.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4VVisManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"

#include <AIDA/AIDA.h>

#include "singCrysAIDAManager.hh"

// Initialize the pointer to the class as NULL
singCrysAIDAManager* singCrysAIDAManager::fInstance = 0;

// Constructor: initialize analysis system and plotter
singCrysAIDAManager::singCrysAIDAManager()
:fAnalysisFactory(0), fFactory(0), tFactory(0), fPlotter(0)
{
  // Hooking an AIDA compliant analysis system.
  fAnalysisFactory = AIDA_createAnalysisFactory();
  if(fAnalysisFactory)
  {
    // Get tree for file output
    AIDA::ITreeFactory* treeFactory = fAnalysisFactory->createTreeFactory();
    fTree = treeFactory->
      create("singCrys.root","root",false,true,"compress=no");
    // Create factories
    fFactory = fAnalysisFactory->createHistogramFactory(*fTree);
    tFactory = fAnalysisFactory->createTupleFactory(*fTree);
    // Create plotter
    AIDA::IPlotterFactory* pf = fAnalysisFactory->createPlotterFactory(0,0);
    if (pf) {
      fPlotter = pf->create("Plotter");
      delete pf;
    }
    delete treeFactory; // Will not delete the ITree.
  }
}

// Destructor: attempt to write data to file, and delete dynamically allocated
// analysis tools.
singCrysAIDAManager::~singCrysAIDAManager()
{
  if (fAnalysisFactory)
  {
    if (!fTree->commit()) G4cout <<
        "Commit failed: no AIDA file produced!" << G4endl;
    delete fTree;
    delete tFactory;
    delete fFactory;
    delete fPlotter;
    G4cout << "Warning: In case of working with JAS-AIDA, Geant4 will NOT exit unless you close the JAS-AIDA window." << G4endl;
    delete fAnalysisFactory;
  }
}

// Accessor method for the histogram factory
AIDA::IHistogramFactory* singCrysAIDAManager::getHistogramFactory()
{
  return fFactory;
}

// Accessor method for the tuple factory
AIDA::ITupleFactory* singCrysAIDAManager::getTupleFactory()
{
  return tFactory;
}

// Accessor method for the plotter
AIDA::IPlotter* singCrysAIDAManager::getPlotter()
{
  return fPlotter;
}

// Returns the pointer to the class, if the class has been initialized.
// Otherwise creates an instance.
singCrysAIDAManager* singCrysAIDAManager::getInstance()
{
  if (fInstance == 0) fInstance = new singCrysAIDAManager();
  return fInstance;
}

// Deletes the instance of the class
void singCrysAIDAManager::dispose()
{
  if (fInstance != 0)
  {
    delete fInstance;
    fInstance = 0;
  }
}

#endif // AIDA_USE

