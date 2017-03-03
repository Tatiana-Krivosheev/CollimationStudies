// -*- C++ -*-

#pragma once

#include "globals.hh"
#include "G4UserRunAction.hh"

class G4LogicalVolume;
class G4Run;

class Tally : public G4UserRunAction
{
#pragma region Data
    private: double _sum_real;
    private: double _sum_user;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public:          Tally();
    public: virtual ~Tally();
#pragma endregion

#pragma region Overrides
    public: virtual void BeginOfRunAction(const G4Run*);
    public: virtual void EndOfRunAction(const G4Run*);
#pragma endregion

#pragma region Observers
#pragma endregion
};
