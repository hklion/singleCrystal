#include "G4UImanager.hh"
#include "G4RunManager.hh"
#include "singCrysUIsession.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "singCrysDetectorConstruction.hh"
#include "singCrysPhysicsList.hh"
#include "singCrysPrimaryGeneratorAction.hh"
#include "singCrysRunAction.hh"
#include "singCrysConfig.hh"

#include "G4StepLimiterBuilder.hh"
#include "G4VModularPhysicsList.hh"

#include "Randomize.hh"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
  // Choose the random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Set detector construction class
  runManager->SetUserInitialization(new singCrysDetectorConstruction());
  // Set physics list
//  runManager->SetUserInitialization(new LBE);
  runManager->SetUserInitialization(new singCrysPhysicsList());

  // Add manatory user action class
  runManager->SetUserAction(new singCrysPrimaryGeneratorAction());

  // Add optional run action class
//  runManager->SetUserAction(new singCrysRunAction());
  po::options_description desc;
  desc.add_options()
    ("config,c", po::value<std::string>()->default_value("config.ini"),
      "configuration fle")
    ("script", po::value<std::string>()->default_value(""),
      "script to run in batch mode");
  po::positional_options_description pos_options;
  pos_options.add("script", 1);
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).
    positional(pos_options).run(), vm);
  po::notify(vm);
  singCrysConfig::LoadFile((G4String) vm["config"].as<std::string>());

  // Initialize kernel
  runManager->Initialize();

  #ifdef G4VIS_USE
  // Visualization initialization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  #endif

  // Get pointer to UI manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  singCrysUIsession* loggedSession = new singCrysUIsession;
  UImanager->SetCoutDestination(loggedSession);
 
  // If 'script' is not the empty string, batch mode
  if (std::strcmp((G4String)vm["script"].as<std::string>(), "") != 0)
  {
    G4String command = "/control/execute ";

    G4String fileName = (G4String) vm["script"].as<std::string>();
    UImanager->ApplyCommand(command + fileName);
  }

  // Otherwise, interactive mode
  else
  {
    // Define UI session
    #ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    #ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    #else
    UImanager->ApplyCommand("/control/execute init.mac");
    #endif
    ui->SessionStart();
    delete ui;
    #endif
  }

  // Free everything
  #ifdef G4VIS_USE
  delete visManager;
  #endif
  delete runManager;
  delete loggedSession;

  return 0;
}
