#pragma once

#include "G4UserEventAction.hh"

class G4Event;

class EventAction : public G4UserEventAction
{
#pragma region Data
    private: double _end;
#pragma endregion
    
#pragma region Ctor/Dtor/ops
    public: EventAction(double end);
    public: virtual ~EventAction();
#pragma endregion

#pragma region Overrides
    public: virtual void BeginOfEventAction(const G4Event*);
    public: virtual void EndOfEventAction(const G4Event*);
#pragma endregion

    public: double end() const
    {
        return _end;
    }
};
