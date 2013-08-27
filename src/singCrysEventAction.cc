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
#ifdef G4ANALYSIS_USE
#include "singCrysAnalysisManager.hh"
#endif // G4ANALYSIS_USE

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

#ifdef G4ANALYSIS_USE
  //fPlotter = 0;
  fTuple = 0;
  //fDc1Hits = fDc2Hits = 0;
  //fDc1XY = fDc2XY = fEvstof = 0;

  // Do some analysis

  singCrysAnalysisManager* analysisManager = singCrysAnalysisManager::getInstance();
  IHistogramFactory* hFactory = analysisManager->getHistogramFactory();

//  if (hFactory)
//  {
    // Create some histograms
    //fDc1Hits = hFactory->createHistogram1D("Drift Chamber 1 # Hits",50,0,50);
    //fDc2Hits = hFactory->createHistogram1D("Drift Chamber 2 # Hits",50,0,50);

    // Create some clouds (Scatter Plots)
    //fDc1XY = hFactory->createCloud2D("Drift Chamber 1 X vs Y");
    //fDc2XY = hFactory->createCloud2D("Drift Chamber 2 X vs Y");
    //fEvstof = hFactory->createCloud2D("EDep vs Time-of-flight");

    //fPlotter = analysisManager->getPlotter();
    //if (fPlotter)
    //{
      // fPlotter->createRegions(3,2);
      // fPlotter->region(0)->plot(*fDc1Hits);
      // fPlotter->region(1)->plot(*fDc2Hits);
      // fPlotter->region(2)->plot(*fDc1XY);
      // fPlotter->region(3)->plot(*fDc2XY);
      // fPlotter->region(4)->plot(*fEvstof);
      // fPlotter->show();
    // }
//  }

  // Create a Tuple




  ITupleFactory* tFactory = analysisManager->getTupleFactory();
  if (tFactory)
  {
     //fTuple = tFactory->create("MyTuple","MyTuple","int fDc1Hits, fDc2Hits, double ECEnergy, HCEnergy, time1, time2","");
     fTuple = tFactory->create("MyTuple","MyTuple","int eventNumber, iDeposit, double Energy","");
  }
#endif // G4ANALYSIS_USE
}

singCrysEventAction::~singCrysEventAction()
{
#ifdef G4ANALYSIS_USE
  singCrysAnalysisManager::dispose();
#endif // G4ANALYSIS_USE
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

#ifdef G4ANALYSIS_USE
  // Fill some histograms

/*  if (fDHC1 && fDc1Hits)
  {
    int n_hit = fDHC1->entries();
    fDc1Hits->fill(n_hit);
    for(int i1=0;i1<n_hit;i1++)
    {
      singCrysDriftChamberHit* aHit = (*fDHC1)[i1];
      G4ThreeVector localPos = aHit->GetLocalPos();
      if (fDc1XY) fDc1XY->fill(localPos.y(), localPos.x());
    }
  }
  if (fDHC2 && fDc2Hits)
  {
    int n_hit = fDHC2->entries();
    fDc2Hits->fill(n_hit);
    for(int i1=0;i1<n_hit;i1++)
    {
      singCrysDriftChamberHit* aHit = (*fDHC2)[i1];
      G4ThreeVector localPos = aHit->GetLocalPos();
      if (fDc2XY) fDc2XY->fill(localPos.y(), localPos.x());
    }
  }*/

  // Fill the tuple

/*  if (fTuple)
  {
    if (fDHC1) fTuple->fill(0,fDHC1->entries());
    if (fDHC2) fTuple->fill(1,fDHC2->entries());
    if(ECHC)
    {
      int iHit = 0;
      double totalE = 0.;
      for(int i1=0;i1<80;i1++)
      {
        singCrysEmCalorimeterHit* aHit = (*ECHC)[i1];
        double eDep = aHit->GetEdep();
        if(eDep>0.)
        {
          iHit++;
          totalE += eDep;
        }
      }
      fTuple->fill(2,totalE);

          if (fHHC1 && fHHC2 && fHHC1->entries()==1 && fHHC2->entries()==1)
          {
             double tof = (*fHHC2)[0]->GetTime() - (*fHHC1)[0]->GetTime();
                 if (fEvstof) fEvstof->fill(tof,totalE);
          }
    }
    if(HCHC)
    {
      int iHit = 0;
      double totalE = 0.;
      for(int i1=0;i1<20;i1++)
      {
        singCrysHadCalorimeterHit* aHit = (*HCHC)[i1];
        double eDep = aHit->GetEdep();
        if(eDep>0.)
        {
          iHit++;
          totalE += eDep;
        }
      }
      fTuple->fill(3,totalE);
    }
    if (fHHC1 && fHHC1->entries()==1) fTuple->fill(4,(*fHHC1)[0]->GetTime());
    if (fHHC2 && fHHC2->entries()==1) fTuple->fill(5,(*fHHC2)[0]->GetTime());
        fTuple->addRow();
  }
*/  

  // Make sure there aren't going to be any issues with NULL pointers
  if (fTuple)
  {
    if (SiHC)
    {
      G4int hitID = 0;
      // Get the number of hits
      G4int nHits = SiHC->entries();
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

  
  
  //if (fPlotter) fPlotter->refresh();
#endif // G4ANALYSIS_USE



}
