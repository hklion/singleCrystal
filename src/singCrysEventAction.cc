/*!
 * \file singCrysEventAction.cc
 * \brief Implementation file for the singCrysEventAction class. User defined
 * event action class.
 */

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
#include "singCrysConfig.hh"
#include <boost/program_options.hpp>

#include "singCrysSiliconHit.hh"

namespace po = boost::program_options;

// Constructor: gets the hits collection and sets up the analysis interface
// for ROOT and/or AIDA.
singCrysEventAction::singCrysEventAction()
{
  // Get the variables map from the configuration file
  po::variables_map config = *(singCrysConfig::GetInstance()->GetMap());
  // Get the hits collection
  G4String HCname;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  fSiHCID = SDman->GetCollectionID(HCname="SiliconHitsCollection");
  fVerboseLevel = 1;

  #ifdef AIDA_USE
  fTuple = 0;
  // Get the analysis manager  
  singCrysAIDAManager* analysisManager =
      singCrysAIDAManager::getInstance();
  // Create a Tuple. It contains the event number, the index of the deposit,
  // and the energy of the deposit. 
  ITupleFactory* tFactory = analysisManager->getTupleFactory();
  if (tFactory)
  {
    fTuple = tFactory->
    create("MyTuple","MyTuple","int eventNumber, iDeposit, double Energy, xPos, yPos, zPos, xMomentum, yMomentum, zMomentum","");
  }
  #endif // AIDA_USE

  #ifdef ROOT_USE
  // Create a file and a tree
  G4String rootOutfile = (G4String) config["rootOutfile"].as<std::string>();
  myFile = new TFile(rootOutfile, "recreate");
  myTree = new TTree("ntp1", "Tree with vectors");
  // Create branches, one for the event ID, and one for the energy of the hits
  myTree->Branch("eventID", &eventID);
  myTree->Branch("energy", &energy);
  myTree->Branch("xPos", &xPos);
  myTree->Branch("yPos", &yPos);
  myTree->Branch("zPos", &zPos);
  myTree->Branch("xMomentum", &xPVec);
  myTree->Branch("yMomentum", &yPVec);
  myTree->Branch("zMomentum", &zPVec);
  #endif // ROOT_USE
}

// Destructor: Writes data to file and cleans up analysis interface.
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

// Actions to be carried out at the beginning of each event
void singCrysEventAction::BeginOfEventAction(const G4Event*)
{
}

// Actions to be carried out at the end of each event: process hits and write
// the data to the ROOT and/or AIDA interface.
void singCrysEventAction::EndOfEventAction(const G4Event* evt)
{
  // Get event number. Print it if modulo a user-specified number
  po::variables_map config = *(singCrysConfig::GetInstance()->GetMap());
  G4int printEvery = config["printEvery"].as<G4int>();
  G4int evtID = evt->GetEventID();
  if (evtID % printEvery == 0)
  {
    G4cout << evtID << " events completed." << G4endl;
  }
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
        // If there is a nonzero energy deposit, store information about the
        // hit in the tuple. 
        singCrysSiliconHit* hit = (*SiHC)[i];
        G4double eDep = hit->GetEdep();
        G4ThreeVector position = hit->GetPos();
        G4ThreeVector momentum = hit->GetPVec();
        if (eDep > 0.)
        {
          fTuple->fill(0, evtID);
          fTuple->fill(1, hitID);
          fTuple->fill(2, eDep);
          fTuple->fill(3, position.x());
          fTuple->fill(4, position.y());
          fTuple->fill(5, position.z());
          fTuple->fill(6, momentum.x());
          fTuple->fill(7, momentum.y());
          fTuple->fill(8, momentum.z());
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
      // Get the energy deposit from the hit. If it is nonzero, store
      // the data in the appropriate vectors, to be added to the file.
      singCrysSiliconHit* hit = (*SiHC)[i];
      G4double eDep = hit->GetEdep();
      G4ThreeVector position = hit->GetPos();
      G4ThreeVector momentum = hit->GetPVec();
      if (eDep > 0.)
      {
        energy.push_back(eDep);
        xPos.push_back(position.x());
        yPos.push_back(position.y());
        zPos.push_back(position.z());
        xPVec.push_back(momentum.x());
        yPVec.push_back(momentum.y());
        zPVec.push_back(momentum.z());
      }
    }
    // After all hits have been processed, add the event ID and energy vector
    // to the tree.
    myTree->Fill();
  }
  #endif // ROOT_USE

}
