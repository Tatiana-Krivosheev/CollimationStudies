// -*- C++ -*-

#pragma once

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4LogicalVolume;

class SteppingAction : public G4UserSteppingAction
{
    public: SteppingAction(G4LogicalVolume* scoringVolume = nullptr);
    public: virtual ~SteppingAction();

    // method from the base class
    public: virtual void UserSteppingAction(const G4Step*) override;

    private: G4LogicalVolume* _scoringVolume;
};

