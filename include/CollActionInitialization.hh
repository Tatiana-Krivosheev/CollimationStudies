// -*- C++ -*-

#pragma once

#include "G4VUserActionInitialization.hh"

class CollActionInitialization : public G4VUserActionInitialization
{
#pragma region Ctor/Dtor
    public: CollActionInitialization();

    public: CollActionInitialization& operator=(const CollActionInitialization&) = delete;
    public: CollActionInitialization(const CollActionInitialization&) = delete;
    public: CollActionInitialization(CollActionInitialization&&) = delete;

    public: virtual ~CollActionInitialization();
#pragma endregion

#pragma region Overrides
    public: virtual void BuildForMaster() const override;
    public: virtual void Build() const override;
#pragma endregion
};
