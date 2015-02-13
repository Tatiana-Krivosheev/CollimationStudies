#include "CollimatorMessenger.hh"
#include "CollimatorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

CollimatorMessenger::CollimatorMessenger(CollimatorConstruction* col):
    G4UImessenger(),
    _collimator(col),
    _stepMaxCmdx{nullptr},
    _src_radiusCmd(nullptr),
    _src_halfzCmd(nullptr),
    _enc_radiusCmd(nullptr),
    _back_halfzCmd(nullptr),
    _coll_radiusCmd(nullptr),
    _coll_halfzCmd(nullptr),
    _cout_radiusCmd(nullptr)
{
    _gpDirectory = new G4UIdirectory("/GP/");
    _gpDirectory->SetGuidance("Place where all GP commands are living.");

    _detDirectory = new G4UIdirectory("/GP/det/");
    _detDirectory->SetGuidance("Detector construction control");

    _src_radiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/src_radius", this);
    _src_radiusCmd->SetGuidance("Set source radius");
    _src_radiusCmd->SetParameterName("src_radius",false);
    _src_radiusCmd->SetUnitCategory("Length");
    _src_radiusCmd->SetRange("src_radius>0.0");
    _src_radiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    _src_halfzCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/src_halfz", this);
    _src_halfzCmd->SetGuidance("Set source halfz");
    _src_halfzCmd->SetParameterName("src_halfz",false);
    _src_halfzCmd->SetUnitCategory("Length");
    _src_halfzCmd->SetRange("src_halfz>0.0");
    _src_halfzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    _enc_radiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/enc_radius", this);
    _enc_radiusCmd->SetGuidance("Set enclosure radius");
    _enc_radiusCmd->SetParameterName("enc_radius",false);
    _enc_radiusCmd->SetUnitCategory("Length");
    _enc_radiusCmd->SetRange("enc_radius>0.0");
    _enc_radiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
    _back_halfzCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/back_halfz", this);
    _back_halfzCmd->SetGuidance("Set back halfz");
    _back_halfzCmd->SetParameterName("back_halfz", false);
    _back_halfzCmd->SetUnitCategory("Length");
    _back_halfzCmd->SetRange("back_halfz>0.0");
    _back_halfzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _coll_radiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/coll_radius", this);
    _coll_radiusCmd->SetGuidance("Set collimator radius");
    _coll_radiusCmd->SetParameterName("coll_radius",false);
    _coll_radiusCmd->SetUnitCategory("Length");
    _coll_radiusCmd->SetRange("coll_radius>0.0");
    _coll_radiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  
    
    _coll_halfzCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/coll_halfz", this);
    _coll_halfzCmd->SetGuidance("Set collimator halfz");
    _coll_halfzCmd->SetParameterName("coll_halfz", false);
    _coll_halfzCmd->SetUnitCategory("Length");
    _coll_halfzCmd->SetRange("coll_halfz>0.0");
    _coll_halfzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  
    
    _cout_radiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/cout_radius", this);
    _cout_radiusCmd->SetGuidance("Set collimator opening radius");
    _cout_radiusCmd->SetParameterName("cout_radius",false);
    _cout_radiusCmd->SetUnitCategory("Length");
    _cout_radiusCmd->SetRange("cout_radius>0.0");
    _cout_radiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

    /*
    _stepMaxCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/stepMax",this);
    _stepMaxCmd->SetGuidance("Define a step max");
    _stepMaxCmd->SetParameterName("stepMax",false);
    _stepMaxCmd->SetUnitCategory("Length");
    _stepMaxCmd->SetRange("stepMax>0.0");
    _stepMaxCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    */
}

CollimatorMessenger::~CollimatorMessenger()
{
    delete _stepMaxCmdx;
    
    delete _src_radiusCmd;
    delete _src_halfzCmd;
    delete _enc_radiusCmd;
    delete _back_halfzCmd;
    delete _coll_radiusCmd;
    delete _coll_halfzCmd;
    delete _cout_radiusCmd;
    
    delete _detDirectory;
    delete _gpDirectory;
}

void CollimatorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == _stepMaxCmdx)
    {
        // _collimator->SetMaxStep(_stepMaxCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _src_radiusCmd)
    {
        _collimator->set_src_radius(_src_radiusCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _src_halfzCmd)
    {
        _collimator->set_src_halfz(_src_halfzCmd->GetNewDoubleValue(newValue));
        return;
    }
    
    if (command == _enc_radiusCmd)
    {
        _collimator->set_enc_radius(_enc_radiusCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _back_halfzCmd)
    {
        _collimator->set_back_halfz(_back_halfzCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _coll_radiusCmd)
    {
        _collimator->set_coll_radius(_coll_radiusCmd->GetNewDoubleValue(newValue));
        return;
    }
        
    if (command == _coll_halfzCmd)
    {
        _collimator->set_coll_halfz(_coll_halfzCmd->GetNewDoubleValue(newValue));
        return;
    }
        
    if (command == _cout_radiusCmd)
    {
        _collimator->set_cout_radius(_cout_radiusCmd->GetNewDoubleValue(newValue));
        return;
    }    
}
