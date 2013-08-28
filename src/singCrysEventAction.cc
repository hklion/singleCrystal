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
/// \file analysis/singCrys/src/singCrysEventAction.cc
/// \brief Implementation of the singCrysEventAction class
//
// $Id$
// --------------------------------------------------------------
//

#include "singCrysEventAction.hh"
#ifdef AIDA_USE
#include "singCrysAIDAManager.hh"
#endif // AIDA_USE

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "singCrysSiliconHit.hh"

singCrysEventAction::singCrysEventAction()
{
  G4String HCname;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  fSiHCID = SDman->GetCollectionID(HCname="SiliconHitsCollection");
  fVerboseLevel = 1;

  #ifdef AIDA_USE
  fTuple = 0;
  // Do some analysis

  singCrysAIDAManager* analysisManager =
      singCrysAIDAManager::getInstance();

  // Create a Tuple
  ITupleFactory* tFactory = analysisManager->getTupleFactory();
  if (tFactory)
  {
    fTuple = tFactory->
    create("MyTuple","MyTuple","int eventNumber, iDeposit, double Energy","");
  }
  #endif // AIDA_USE

  #ifdef ROOT_USE
  myFile = new TFile("output.root", "recreate");
  myTree = new TTree("ntp1", "Tree with vectors");
  myTree->Branch("eventID", &eventID);
  myTree->Branch("energy", &energy);
  #endif // ROOT_USE
}

singCrysEventAction::~singCrysEventAction()
{
  #ifdef AIDA_USE
  singCrysAIDAManager::dispose();
  #endif // AIDA_USE
  
  #ifdef ROOT_USE
  myFile->Write();
  myFile->Close();
  delete myFile;
  #endif
}

void singCrysEventAction::BeginOfEventAction(const G4Event*)
{
}

void singCrysEventAction::EndOfEventAction(const G4Event* evt)
{
  // Get event number
  G4int evtID = evt->GetEventID();
  // Get hits collection
  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  singCrysSiliconHitsCollection* SiHC = 0;
  if(HCE)
  {
    SiHC = (singCrysSiliconHitsCollection*)(HCE->GetHC(fSiHCID));
  }

  G4int nHits; // Number of hits
  #ifdef AIDA_USE
  // Fill the tuple

  // Make sure there aren't going to be any issues with NULL pointers
  if (fTuple)
  {
    if (SiHC)
    {
      G4int hitID = 0;
      // Get the number of hits
      nHits = SiHC->entries();
      // Loop through all of the hits.
      for (G4int i = 0; i < nHits; i++)
      {
        // If there is a nonzero energy deposit, 
        singCrysSiliconHit* hit = (*SiHC)[i];
        G4double eDep = hit->GetEdep();
        if (eDep > 0.)
        {
          fTuple->fill(0, evtID);
          fTuple->fill(1, hitID);
          fTuple->fill(2, eDep);
          G4cout << evtID << " " << hitID << " " << eDep << G4endl;
          fTuple->addRow();
          hitID++;
        }
      }
    }
  }
  #endif // AIDA_USE
  
  #ifdef ROOT_USE
  energy.clear();
  eventID = evtID;
  if (SiHC)
  {
    // Get the number of hits
    nHits = SiHC->entries();
    // Loop through all of the hits.
    for (G4int i = 0; i < nHits; i++)
    {
      singCrysSiliconHit* hit = (*SiHC)[i];
      G4double eDep = hit->GetEdep();
      if (eDep > 0.)
      {
        energy.push_back(eDep);
      }
    }
    myTree->Fill();
  }
  #endif // ROOT_USE

}
