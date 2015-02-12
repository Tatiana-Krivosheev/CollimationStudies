// -*- C++ -*-

#pragma once

#include "G4VUserActionInitialization.hh"

class CollActionInitialization : public G4VUserActionInitialization
{
    public: CollActionInitialization();
    public: virtual ~CollActionInitialization();

    public: virtual void BuildForMaster() const override;
    public: virtual void Build() const;
};
