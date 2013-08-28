//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file analysis/singCrys/src/singCrysAIDAManager.cc
/// \brief Implementation of the singCrysAIDAManager class

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

singCrysAIDAManager* singCrysAIDAManager::fInstance = 0;

singCrysAIDAManager::singCrysAIDAManager()
:fAnalysisFactory(0), fFactory(0), tFactory(0), fPlotter(0)
{
  // Hooking an AIDA compliant analysis system.
  fAnalysisFactory = AIDA_createAnalysisFactory();
  if(fAnalysisFactory)
  {
    AIDA::ITreeFactory* treeFactory = fAnalysisFactory->createTreeFactory();
    fTree = treeFactory->create("singCrys.root","root",false,true,"compress=no");
    fFactory = fAnalysisFactory->createHistogramFactory(*fTree);
    tFactory = fAnalysisFactory->createTupleFactory(*fTree);
    AIDA::IPlotterFactory* pf = fAnalysisFactory->createPlotterFactory(0,0);
    if (pf) {
      fPlotter = pf->create("Plotter");
      delete pf;
    }
    delete treeFactory; // Will not delete the ITree.
  }
}

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
AIDA::IHistogramFactory* singCrysAIDAManager::getHistogramFactory()
{
  return fFactory;
}
AIDA::ITupleFactory* singCrysAIDAManager::getTupleFactory()
{
  return tFactory;
}
AIDA::IPlotter* singCrysAIDAManager::getPlotter()
{
  return fPlotter;
}

singCrysAIDAManager* singCrysAIDAManager::getInstance()
{
  if (fInstance == 0) fInstance = new singCrysAIDAManager();
  return fInstance;
}

void singCrysAIDAManager::dispose()
{
  if (fInstance != 0)
  {
    delete fInstance;
    fInstance = 0;
  }
}

#endif // AIDA_USE

