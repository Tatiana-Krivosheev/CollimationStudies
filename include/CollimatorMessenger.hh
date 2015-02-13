// -*- C++ -*-

#pragma once

#include "globals.hh"
#include "G4UImessenger.hh"

class CollimatorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

/// Messenger class that defines commands for CollimatorConstruction.
///
/// It implements commands:
/// - /GP/det/setTargetMaterial name
/// - /GP/det/setChamberMaterial name
/// - /GP/det/stepMax value unit

class CollimatorMessenger: public G4UImessenger
{
    public: CollimatorMessenger(CollimatorConstruction* );
    public: virtual ~CollimatorMessenger();
    
    public: virtual void SetNewValue(G4UIcommand*, G4String) override;
    
    private: CollimatorConstruction*  _collimator;

    private: G4UIdirectory*           _gpDirectory;
    private: G4UIdirectory*           _detDirectory;

    private: G4UIcmdWithADoubleAndUnit* _stepMaxCmdx;

    private: G4UIcmdWithADoubleAndUnit* _src_radiusCmd;
    private: G4UIcmdWithADoubleAndUnit* _src_halfzCmd;
    private: G4UIcmdWithADoubleAndUnit* _enc_radiusCmd;
    private: G4UIcmdWithADoubleAndUnit* _back_halfzCmd;
    private: G4UIcmdWithADoubleAndUnit* _coll_radiusCmd;
    private: G4UIcmdWithADoubleAndUnit* _coll_halfzCmd;
    private: G4UIcmdWithADoubleAndUnit* _cout_radiusCmd;
};
