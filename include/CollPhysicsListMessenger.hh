#pragma once

#include "globals.hh"
#include "G4UImessenger.hh"

class CollPhysicsList;

class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

class CollPhysicsListMessenger: public G4UImessenger
{
#pragma region Data
    private: CollPhysicsList*    _physicsList;
    private: G4UIdirectory*      _listDir;
    private: G4UIcmdWithAString* _physicsListCmd;

    private: G4UIcmdWithADoubleAndUnit* _gammaCutCmd;
    private: G4UIcmdWithADoubleAndUnit* _electCutCmd;
    private: G4UIcmdWithADoubleAndUnit* _positCutCmd;

    private: G4UIcmdWithADoubleAndUnit* _allCutCmd;
#pragma endregion

#pragma region Ctor/Dtor
    public: CollPhysicsListMessenger(CollPhysicsList* physList);
    public: ~CollPhysicsListMessenger();
#pragma endregion

#pragma region Overrides
    public: virtual void SetNewValue(G4UIcommand*, G4String) override;
#pragma endregion
};
