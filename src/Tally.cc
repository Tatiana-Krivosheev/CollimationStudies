#include "G4SystemOfUnits.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"

#include "Randomize.hh"

#include "Tally.hh"

Tally::Tally()
{
}

Tally::~Tally()
{
}

void Tally::BeginOfRunAction(const G4Run* /* aRun */)
{
#ifdef G4VIS_USE
    if (G4VVisManager::GetConcreteInstance())
    {
        G4UImanager::GetUIpointer()->ApplyCommand("/vis/scene/notifyHandlers");
    }
#endif
}

void Tally::EndOfRunAction(const G4Run* aRun)
{
    int nof_events = aRun->GetNumberOfEvent();
    nof_events++; nof_events--;

    // std::cout << "Nof events:" << nof_events << '\n';

#ifdef G4VIS_USE
    if (G4VVisManager::GetConcreteInstance())
    {
        G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
    }
#endif
}
