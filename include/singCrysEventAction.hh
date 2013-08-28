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
/// \file analysis/singCrys/include/singCrysEventAction.hh
/// \brief Definition of the singCrysEventAction class
//
// $Id$
// --------------------------------------------------------------
//
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

class singCrysEventAction : public G4UserEventAction
{
  public:
    singCrysEventAction();
    virtual ~singCrysEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

  private:
    G4int fSiHCID;
    G4int fVerboseLevel;
    
    #ifdef ROOT_USE
    TFile *myFile;
    TTree *myTree;
    int eventID;
    std::vector<double> energy;
    #endif // ROOT_USE

    #ifdef AIDA_USE
    ITuple* fTuple;
    #endif // AIDA_USE

  public:
    inline void SetVerbose(G4int val) { fVerboseLevel = val; }
    inline G4int GetVerbose() const { return fVerboseLevel; }
};

#endif
