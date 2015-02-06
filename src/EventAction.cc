#include "EventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"

EventAction::EventAction(double end):
    _end{end}
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event*)
{
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
    int event_id = evt->GetEventID();
  
    // periodic printing
    if (event_id < 10 || event_id % 10000 == 0)
    {
        G4cout << ">>> Event " << evt->GetEventID() << G4endl;
    }
}
