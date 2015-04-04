#include "CollActionInitialization.hh"

#include "CollEventAction.hh"
#include "Source.hh"
#include "Tally.hh"
#include "SteppingAction.hh"

CollActionInitialization::CollActionInitialization():
    G4VUserActionInitialization()
{
}

CollActionInitialization::~CollActionInitialization()
{
}

void CollActionInitialization::BuildForMaster() const
{
    SetUserAction(new Tally);
}

void CollActionInitialization::Build() const
{
    SetUserAction(new Source());
    SetUserAction(new Tally());
    SetUserAction(new SteppingAction());
    // SetUserAction(new EventAction);
}  
