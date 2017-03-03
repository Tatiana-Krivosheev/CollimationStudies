#pragma once

#include "globals.hh"
#include "G4UserRunAction.hh"

class G4Run;

class Tally : public G4UserRunAction
{
#pragma region Data
#pragma endregion

#pragma region Ctor/Dtor/ops
    public:  Tally();
    public: ~Tally();
#pragma endregion

#pragma region Overrides
    public: virtual void BeginOfRunAction(const G4Run*) override;
    public: virtual void EndOfRunAction(const G4Run*) override;
#pragma endregion

#pragma region Observers
#pragma endregion

#pragma region I/O
#pragma endregion
};
