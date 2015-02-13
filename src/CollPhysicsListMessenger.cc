#include "CollPhysicsListMessenger.hh"
#include "CollPhysicsList.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh"

CollPhysicsListMessenger::CollPhysicsListMessenger(CollPhysicsList* physList):
    _physicsList(physList)
{  
    _listDir = new G4UIdirectory("/GP/physics");

    // Building modular PhysicsList

    _physicsListCmd = new G4UIcmdWithAString("/GP/physics/addPhysics",this);  
    _physicsListCmd->SetGuidance("Add chunks of PhysicsList.");
    _physicsListCmd->SetParameterName("physList",false);
    _physicsListCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
	
    _gammaCutCmd = new G4UIcmdWithADoubleAndUnit("/GP/physics/setGCut",this);  
    _gammaCutCmd->SetGuidance("Set gamma cut.");
    _gammaCutCmd->SetParameterName("Gcut",false);
    _gammaCutCmd->SetUnitCategory("Energy");
    _gammaCutCmd->SetRange("Gcut>0.0");
    _gammaCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _electCutCmd = new G4UIcmdWithADoubleAndUnit("/GP/physics/setECut",this);  
    _electCutCmd->SetGuidance("Set electron cut.");
    _electCutCmd->SetParameterName("Ecut",false);
    _electCutCmd->SetUnitCategory("Energy");
    _electCutCmd->SetRange("Ecut>0.0");
    _electCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
    _positCutCmd = new G4UIcmdWithADoubleAndUnit("/GP/physics/setPCut",this);  
    _positCutCmd->SetGuidance("Set positron cut.");
    _positCutCmd->SetParameterName("Pcut",false);
    _positCutCmd->SetUnitCategory("Energy");
    _positCutCmd->SetRange("Pcut>0.0");
    _positCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

    _allCutCmd = new G4UIcmdWithADoubleAndUnit("/GP/physics/setAllCuts",this);  
    _allCutCmd->SetGuidance("Set cut for all.");
    _allCutCmd->SetParameterName("cut",false);
    _allCutCmd->SetUnitCategory("Energy");
    _allCutCmd->SetRange("cut>0.0");
    _allCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
}

CollPhysicsListMessenger::~CollPhysicsListMessenger()
{
    delete _physicsListCmd;
    
    delete _gammaCutCmd;
    delete _electCutCmd;
    delete _positCutCmd;

    delete _allCutCmd;

    delete _listDir;
}

void CollPhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == _physicsListCmd)
    { 
		_physicsList->AddPhysicsList(newValue);
        return;
	}
    
    if( command == _gammaCutCmd )
    {
        _physicsList->SetCutForGamma(_gammaCutCmd->GetNewDoubleValue(newValue));
        return;
    }
    
    if( command == _electCutCmd )
    {
        _physicsList->SetCutForElectron(_electCutCmd->GetNewDoubleValue(newValue));
        return;
    }
    
    if( command == _positCutCmd )
    {
        _physicsList->SetCutForPositron(_positCutCmd->GetNewDoubleValue(newValue));
        return;
    }
    
    if( command == _allCutCmd )
    {
        double cut = _allCutCmd->GetNewDoubleValue(newValue);
        _physicsList->SetCutForGamma(cut);
        _physicsList->SetCutForElectron(cut);
        _physicsList->SetCutForPositron(cut);
    }
}
