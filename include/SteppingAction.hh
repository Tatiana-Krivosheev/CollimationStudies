// -*- C++ -*-

#pragma once

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4LogicalVolume;

class SteppingAction : public G4UserSteppingAction
{
#pragma region Data
    private: G4LogicalVolume* _scoringVolume;
#pragma endregion

#pragma region Ctor/Dtor
    public: SteppingAction(G4LogicalVolume* scoringVolume = nullptr);

    public: SteppingAction& operator=(const SteppingAction&) = delete;
    public: SteppingAction(const SteppingAction&) = delete;
    public: SteppingAction(SteppingAction&&) = delete;

    public: virtual ~SteppingAction();
#pragma endregion

#pragma region Overrides
    public: virtual void UserSteppingAction(const G4Step*) override;
#pragma endregion
};
