#include "CollActionInitialization.hh"

#include "CollEventAction.hh"
#include "Source.hh"
#include "Tally.hh"

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
    SetUserAction(new Source(1.0*mm, 10.0*mm));
    SetUserAction(new Tally);
    // SetUserAction(new EventAction);
}  
