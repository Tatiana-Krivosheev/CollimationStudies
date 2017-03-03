#pragma once

#include "globals.hh"
#include "G4VModularPhysicsList.hh"
#include "G4SystemOfUnits.hh"

class CollPhysicsListMessenger;
class G4VPhysicsConstructor;

class CollPhysicsList: public G4VModularPhysicsList
{
#pragma region Data
    private: double _cutForGamma;
    private: double _cutForElectron;
    private: double _cutForPositron;

    private: std::string             _emName;

    private: G4VPhysicsConstructor*  _emPhysicsList;
    private: G4VPhysicsConstructor*  _decayPhysicsList;
    private: G4VPhysicsConstructor*  _emExtraPhysicsList;

    private: CollPhysicsListMessenger*       _messenger;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: CollPhysicsList(double cuts = 0.1*mm);

    public: CollPhysicsList& operator=(const CollPhysicsList&) = delete;
    public: CollPhysicsList(const CollPhysicsList&) = delete;
    public: CollPhysicsList(CollPhysicsList&&) = delete;

    public: virtual ~CollPhysicsList();
#pragma endregion

#pragma region Overrides
    public: virtual void ConstructParticle();
    public: virtual void ConstructProcess();
#pragma endregion

#pragma region Helpers
    public: void SetCuts();

    public: void SetCutForGamma(double cut);
    public: void SetCutForElectron(double cut);
    public: void SetCutForPositron(double cut);

    public: void AddPhysicsList(const std::string& name);
#pragma endregion
};
