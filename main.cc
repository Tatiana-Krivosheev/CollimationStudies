#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "G4SystemOfUnits.hh"

#include "Randomize.hh"

#include "CollimatorConstruction.hh"
#include "CollPhysicsList.hh"
#include "CollActionInitialization.hh"

static constexpr char NL = '\n';

int main(int argc, char* argv[])
{
    // CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

    G4UIExecutive* ui = nullptr;

    if ( argc == 1 )
    {
        ui = new G4UIExecutive(argc, argv);
    }

    // Choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Construct the default run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager{};
#else
    G4RunManager*   runManager = new G4RunManager{};
#endif

    // Set mandatory initialization classes

    // Detector construction
    runManager->SetUserInitialization(new CollimatorConstruction{});

    // Physics list
    runManager->SetUserInitialization(new CollPhysicsList{});

    // User action initialization
    runManager->SetUserInitialization(new CollActionInitialization{});

    // Initialize visualization
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    G4VisManager* visManager = new G4VisExecutive{"Quiet"};
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    if ( ui )
    {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else
    {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    std::cout << "DONE" << NL;

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !
    delete visManager;
    delete runManager;

    return 0;
}
