#include "SourceMessenger.hh"
#include "Source.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

SourceMessenger::SourceMessenger(Source* source):
    _source{source},
    _srcDirectory{nullptr},
    _radius{nullptr},
    _halfz{nullptr}
{
    _srcDirectory = new G4UIdirectory("/GP/source/");
    _srcDirectory->SetGuidance("Source construction control");

    _src_radiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/source/radius", this);
    _src_radiusCmd->SetGuidance("Set source radius");
    _src_radiusCmd->SetParameterName("radius",false);
    _src_radiusCmd->SetUnitCategory("Length");
    _src_radiusCmd->SetRange("radius>0.0");
    _src_radiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    _src_halfzCmd = new G4UIcmdWithADoubleAndUnit("/GP/source/halfz", this);
    _src_halfzCmd->SetGuidance("Set source halfz");
    _src_halfzCmd->SetParameterName("halfz",false);
    _src_halfzCmd->SetUnitCategory("Length");
    _src_halfzCmd->SetRange("halfz>0.0");
    _src_halfzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

SourceMessenger::~SourceMessenger()
{
	delete _radiusCmd;
	delete _halfzCmd;
	
	delete _srcDirectory;
}

void SourceMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
	if (cmd == _radiusCmd)
	{
		_source->set_radius(_radiusCmd->GetNewDoubleValue(newValue));
		return;
	}

	if (cmd == _halfzCmd)
	{
		_source->set_halfz(_halfzCmd->GetNewDoubleValue(newValue));
		return;
	}
}

