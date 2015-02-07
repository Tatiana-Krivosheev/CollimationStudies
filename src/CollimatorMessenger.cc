#include "CollimatorMessenger.hh"
#include "CollimatorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

CollimatorMessenger::CollimatorMessenger(CollimatorConstruction* col):
    G4UImessenger(),
    _collimator(col)
{
    _gpDirectory = new G4UIdirectory("/GP/");
    _gpDirectory->SetGuidance("Place where all GP commands are living.");

    _detDirectory = new G4UIdirectory("/GP/det/");
    _detDirectory->SetGuidance("Detector construction control");

    // target material
    _TargMatCmd = new G4UIcmdWithAString("/GP/det/setTargetMaterial", this);
    _TargMatCmd->SetGuidance("Select Material of the Target.");
    _TargMatCmd->SetParameterName("choice", false);
    _TargMatCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // 
    fChamMatCmd = new G4UIcmdWithAString("/GP/det/setChamberMaterial",this);
    fChamMatCmd->SetGuidance("Select Material of the Chamber.");
    fChamMatCmd->SetParameterName("choice",false);
    fChamMatCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/B2/det/stepMax",this);
    fStepMaxCmd->SetGuidance("Define a step max");
    fStepMaxCmd->SetParameterName("stepMax",false);
    fStepMaxCmd->SetUnitCategory("Length");
    fStepMaxCmd->AvailableForStates(G4State_Idle);
}

CollimatorMessenger::~CollimatorMessenger()
{
    delete fTargMatCmd;
    delete fChamMatCmd;
    delete fStepMaxCmd;
    delete fB2Directory;
    delete fDetDirectory;
}

void CollimatorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if( command == fTargMatCmd )
    {
	fDetectorConstruction->SetTargetMaterial(newValue);
	return;
    }

    if( command == fChamMatCmd )
    {
	fDetectorConstruction->SetChamberMaterial(newValue);
	return;
    }

    if( command == fStepMaxCmd )
    {
	fDetectorConstruction->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
	return;
    }
}
