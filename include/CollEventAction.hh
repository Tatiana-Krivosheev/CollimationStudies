#pragma once

#include "G4UserEventAction.hh"

class G4Event;

class CollEventAction : public G4UserEventAction
{
#pragma region Data
    private: double _end;
#pragma endregion
    
#pragma region Ctor/Dtor/ops
    public: CollEventAction(double end);
    public: virtual ~CollEventAction();
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
