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
/// \file singCrysAIDAManager.hh
/// \brief Definition of the singCrysAIDAManager class

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

class singCrysAIDAManager {
public:

  virtual ~singCrysAIDAManager();
  static singCrysAIDAManager* getInstance();
  static void dispose();

  AIDA::IHistogramFactory* getHistogramFactory();
  AIDA::ITupleFactory* getTupleFactory();
  AIDA::IPlotter* getPlotter();

private:
  singCrysAIDAManager();
  static singCrysAIDAManager* fInstance;

  AIDA::IAnalysisFactory* fAnalysisFactory;
  AIDA::IHistogramFactory* fFactory;
  AIDA::ITupleFactory* tFactory;
  AIDA::IPlotter* fPlotter;
  AIDA::ITree* fTree;
};

#endif
