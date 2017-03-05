#include "CollimatorMessenger.hh"
#include "CollimatorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

CollimatorMessenger::CollimatorMessenger(CollimatorConstruction* col):
    G4UImessenger(),
    _collimator(col),
    _stepMaxCmdx{nullptr},

    _src_radiusCmd{nullptr},
    _src_halfzCmd{nullptr},
    _src_shiftzCmd{nullptr},

    _enc_radiusCmd{nullptr},
    _enc_halfzCmd{nullptr},

    _sss_radiusCmd{nullptr},
    _sss_halfzCmd{nullptr},

    _opn_radiusCmd{nullptr},
    _opn_halfzCmd{nullptr},

    _pcl_radiusCmd{nullptr},
    _pcl_halfzCmd{nullptr},

    _oair_radiusCmd{nullptr},
    _oair_halfzCmd{nullptr},

    _sssair_halfzCmd{nullptr},

    _air_gapCmd{nullptr},

    _coll_radiusCmd{nullptr},
    _coll_halfzCmd{nullptr},

    _scl_radiusCmd{nullptr},
    _scl_holeACmd{nullptr},
    _scl_holeBCmd{nullptr},
    _scl_halfzCmd{nullptr},
    _checkOverlapsCmd{nullptr}
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

    _src_shiftzCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/src_shiftz", this);
    _src_shiftzCmd->SetGuidance("Set source shiftz");
    _src_shiftzCmd->SetParameterName("src_shiftz",false);
    _src_shiftzCmd->SetUnitCategory("Length");
    _src_shiftzCmd->SetRange("src_shiftz>0.0");
    _src_shiftzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _enc_radiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/enc_radius", this);
    _enc_radiusCmd->SetGuidance("Set enclosure radius");
    _enc_radiusCmd->SetParameterName("enc_radius",false);
    _enc_radiusCmd->SetUnitCategory("Length");
    _enc_radiusCmd->SetRange("enc_radius>0.0");
    _enc_radiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _enc_halfzCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/enc_halfz", this);
    _enc_halfzCmd->SetGuidance("Set enclosure halfz");
    _enc_halfzCmd->SetParameterName("enc_halfz",false);
    _enc_halfzCmd->SetUnitCategory("Length");
    _enc_halfzCmd->SetRange("enc_halfz>0.0");
    _enc_halfzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _sss_radiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/sss_radius", this);
    _sss_radiusCmd->SetGuidance("Set stainless steel shell radius");
    _sss_radiusCmd->SetParameterName("sss_radius",false);
    _sss_radiusCmd->SetUnitCategory("Length");
    _sss_radiusCmd->SetRange("sss_radius>0.0");
    _sss_radiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _sss_halfzCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/sss_halfz", this);
    _sss_halfzCmd->SetGuidance("Set stainless steel shell halfz");
    _sss_halfzCmd->SetParameterName("sss_halfz",false);
    _sss_halfzCmd->SetUnitCategory("Length");
    _sss_halfzCmd->SetRange("sss_halfz>0.0");
    _sss_halfzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _opn_radiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/opn_radius", this);
    _opn_radiusCmd->SetGuidance("Set opening radius");
    _opn_radiusCmd->SetParameterName("opn_radius",false);
    _opn_radiusCmd->SetUnitCategory("Length");
    _opn_radiusCmd->SetRange("opn_radius>0.0");
    _opn_radiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _opn_halfzCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/opn_halfz", this);
    _opn_halfzCmd->SetGuidance("Set opening halfz");
    _opn_halfzCmd->SetParameterName("opn_halfz",false);
    _opn_halfzCmd->SetUnitCategory("Length");
    _opn_halfzCmd->SetRange("opn_halfz>0.0");
    _opn_halfzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _pcl_radiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/pcl_radius", this);
    _pcl_radiusCmd->SetGuidance("Set primary collimator radius");
    _pcl_radiusCmd->SetParameterName("pcl_radius",false);
    _pcl_radiusCmd->SetUnitCategory("Length");
    _pcl_radiusCmd->SetRange("pcl_radius>0.0");
    _pcl_radiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _pcl_halfzCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/pcl_halfz", this);
    _pcl_halfzCmd->SetGuidance("Set primary collimator halfz");
    _pcl_halfzCmd->SetParameterName("pcl_halfz",false);
    _pcl_halfzCmd->SetUnitCategory("Length");
    _pcl_halfzCmd->SetRange("pcl_halfz>0.0");
    _pcl_halfzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _oair_radiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/oair_radius", this);
    _oair_radiusCmd->SetGuidance("Set primary open air radius");
    _oair_radiusCmd->SetParameterName("oair_radius",false);
    _oair_radiusCmd->SetUnitCategory("Length");
    _oair_radiusCmd->SetRange("oair_radius>0.0");
    _oair_radiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _oair_halfzCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/oair_halfz", this);
    _oair_halfzCmd->SetGuidance("Set primary open air halfz");
    _oair_halfzCmd->SetParameterName("oair_halfz",false);
    _oair_halfzCmd->SetUnitCategory("Length");
    _oair_halfzCmd->SetRange("oair_halfz>0.0");
    _oair_halfzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _sssair_halfzCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/sssair_halfz", this);
    _sssair_halfzCmd->SetGuidance("Set SS shell open air end halfz");
    _sssair_halfzCmd->SetParameterName("sssair_halfz",false);
    _sssair_halfzCmd->SetUnitCategory("Length");
    _sssair_halfzCmd->SetRange("sssair_halfz>0.0");
    _sssair_halfzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _air_gapCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/air_gap", this);
    _air_gapCmd->SetGuidance("Set air gap between primary and secondary collimator");
    _air_gapCmd->SetParameterName("air_gap",false);
    _air_gapCmd->SetUnitCategory("Length");
    _air_gapCmd->SetRange("air_gap>0.0");
    _air_gapCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

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

    _scl_radiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/scl_radius", this);
    _scl_radiusCmd->SetGuidance("Set secondary collimator radius");
    _scl_radiusCmd->SetParameterName("scl_radius",false);
    _scl_radiusCmd->SetUnitCategory("Length");
    _scl_radiusCmd->SetRange("scl_radius>0.0");
    _scl_radiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _scl_halfzCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/scl_halfz", this);
    _scl_halfzCmd->SetGuidance("Set secondary collimator halfz");
    _scl_halfzCmd->SetParameterName("scl_halfz",false);
    _scl_halfzCmd->SetUnitCategory("Length");
    _scl_halfzCmd->SetRange("scl_halfz>0.0");
    _scl_halfzCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _scl_holeACmd = new G4UIcmdWithADoubleAndUnit("/GP/det/scl_holeA", this);
    _scl_holeACmd->SetGuidance("Set secondary collimator in opening");
    _scl_holeACmd->SetParameterName("scl_holeA", false);
    _scl_holeACmd->SetUnitCategory("Length");
    _scl_holeACmd->SetRange("scl_holeA>0.0");
    _scl_holeACmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    _scl_holeBCmd = new G4UIcmdWithADoubleAndUnit("/GP/det/scl_holeB", this);
    _scl_holeBCmd->SetGuidance("Set secondary collimator out opening");
    _scl_holeBCmd->SetParameterName("scl_holeB", false);
    _scl_holeBCmd->SetUnitCategory("Length");
    _scl_holeBCmd->SetRange("scl_holeB>0.0");
    _scl_holeBCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

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
    delete _src_shiftzCmd;

    delete _enc_radiusCmd;
    delete _enc_halfzCmd;

    delete _sss_radiusCmd;
    delete _sss_halfzCmd;

    delete _opn_radiusCmd;
    delete _opn_halfzCmd;

    delete _pcl_radiusCmd;
    delete _pcl_halfzCmd;

    delete _oair_radiusCmd;
    delete _oair_halfzCmd;

    delete _sssair_halfzCmd;

    delete _air_gapCmd;

    delete _coll_radiusCmd;
    delete _coll_halfzCmd;

    delete _scl_radiusCmd;
    delete _scl_holeACmd;
    delete _scl_holeBCmd;
    delete _scl_halfzCmd;

    delete _checkOverlapsCmd;

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

    if (command == _src_shiftzCmd)
    {
        _collimator->set_src_shiftz(_src_shiftzCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _enc_radiusCmd)
    {
        _collimator->set_enc_radius(_enc_radiusCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _enc_halfzCmd)
    {
        _collimator->set_enc_halfz(_enc_halfzCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _sss_radiusCmd)
    {
        _collimator->set_sss_radius(_sss_radiusCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _sss_halfzCmd)
    {
        _collimator->set_sss_halfz(_sss_halfzCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _opn_radiusCmd)
    {
        _collimator->set_opn_radius(_opn_radiusCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _opn_halfzCmd)
    {
        _collimator->set_opn_halfz(_opn_halfzCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _pcl_radiusCmd)
    {
        _collimator->set_pcl_radius(_pcl_radiusCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _pcl_halfzCmd)
    {
        _collimator->set_pcl_halfz(_pcl_halfzCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _oair_radiusCmd)
    {
        _collimator->set_oair_radius(_oair_radiusCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _oair_halfzCmd)
    {
        _collimator->set_oair_halfz(_oair_halfzCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _sssair_halfzCmd)
    {
        _collimator->set_sssair_halfz(_sssair_halfzCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _air_gapCmd)
    {
        _collimator->set_air_gap(_air_gapCmd->GetNewDoubleValue(newValue));
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

    if (command == _scl_radiusCmd)
    {
        _collimator->set_scl_radius(_scl_radiusCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _scl_halfzCmd)
    {
        _collimator->set_scl_halfz(_scl_halfzCmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _scl_holeACmd)
    {
        _collimator->set_scl_holeA(_scl_holeACmd->GetNewDoubleValue(newValue));
        return;
    }

    if (command == _scl_holeBCmd)
    {
        _collimator->set_scl_holeB(_scl_holeBCmd->GetNewDoubleValue(newValue));
        return;
    }
}
