#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ios.hh"

#include "SourceMessenger.hh"
#include "Source.hh"

SourceMessenger::SourceMessenger(Source* src):
    _source(src)
{
    _radius_cmd = new G4UIcmdWithADoubleAndUnit("/phsource/radius", this);
    _radius_cmd->SetGuidance("Source radius");
    _radius_cmd->SetParameterName("R", true);
    _radius_cmd->SetRange("R>0.");
    _radius_cmd->SetDefaultValue(20.0);
    _radius_cmd->SetDefaultUnit("mm");

    _halfz_cmd = new G4UIcmdWithADoubleAndUnit("/phsource/halfz", this);
    _halfz_cmd->SetGuidance("Source half Z");
    _halfz_cmd->SetParameterName("Hz", true);
    _halfz_cmd->SetRange("Hz>0.");
    _halfz_cmd->SetDefaultValue(10.0);
    _halfz_cmd->SetDefaultUnit("mm");
}

SourceMessenger::~SourceMessenger()
{
    delete _radius_cmd;
    delete _halfz_cmd;
}

void SourceMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if( command == _radius_cmd )
    {
        _source->set_radius(_radius_cmd->GetNewDoubleValue(newValue));
	return;
    }

    if( command == _halfz_cmd )
    {
        _source->set_halfz(_halfz_cmd->GetNewDoubleValue(newValue));
	return;
    }
}

G4String SourceMessenger::GetCurrentValue(G4UIcommand * command)
{
    G4String cv;
    if( command == _radius_cmd )
    {
        cv = _radius_cmd->ConvertToString(_source->radius(), "mm");
	goto done;
    }

    if( command == _halfz_cmd )
    {
        cv = _halfz_cmd->ConvertToString(_source->half_z(), "mm");
	goto done;
    }

 done:
    return cv;
}
