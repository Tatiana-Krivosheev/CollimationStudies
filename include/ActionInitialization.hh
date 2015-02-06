// -*- C++ -*-

#pragma once

#include "G4VUserActionInitialization.hh"

class CollimatorConstruction;

class ActionInitialization : public G4VUserActionInitialization
{
    public: ActionInitialization();
    public: virtual ~ActionInitialization();

    public: virtual void BuildForMaster() const;
    public: virtual void Build() const;
};
