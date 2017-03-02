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
#pragma region Ctor/Dtor
    public: CollimatorMessenger(CollimatorConstruction* );

    public: CollimatorMessenger& operator=(const CollimatorMessenger&) = delete;
    public: CollimatorMessenger(const CollimatorMessenger&) = delete;
    public: CollimatorMessenger(CollimatorMessenger&&) = delete;

    public: virtual ~CollimatorMessenger();
#pragma endregion

    public: virtual void SetNewValue(G4UIcommand*, G4String) override;

#pragma region Data
    private: CollimatorConstruction*  _collimator;

    private: G4UIdirectory*           _gpDirectory;
    private: G4UIdirectory*           _detDirectory;

    private: G4UIcmdWithADoubleAndUnit* _stepMaxCmdx;

    private: G4UIcmdWithADoubleAndUnit* _src_radiusCmd;
    private: G4UIcmdWithADoubleAndUnit* _src_halfzCmd;
    private: G4UIcmdWithADoubleAndUnit* _src_shiftzCmd;

    private: G4UIcmdWithADoubleAndUnit* _enc_radiusCmd;
    private: G4UIcmdWithADoubleAndUnit* _enc_halfzCmd;

    private: G4UIcmdWithADoubleAndUnit* _sss_radiusCmd;
    private: G4UIcmdWithADoubleAndUnit* _sss_halfzCmd;

    private: G4UIcmdWithADoubleAndUnit* _opn_radiusCmd;
    private: G4UIcmdWithADoubleAndUnit* _opn_halfzCmd;

    private: G4UIcmdWithADoubleAndUnit* _pcl_radiusCmd;
    private: G4UIcmdWithADoubleAndUnit* _pcl_halfzCmd;

    private: G4UIcmdWithADoubleAndUnit* _oair_radiusCmd;
    private: G4UIcmdWithADoubleAndUnit* _oair_halfzCmd;

    private: G4UIcmdWithADoubleAndUnit* _sssair_halfzCmd;

    private: G4UIcmdWithADoubleAndUnit* _air_gapCmd;

    private: G4UIcmdWithADoubleAndUnit* _coll_radiusCmd;
    private: G4UIcmdWithADoubleAndUnit* _coll_halfzCmd;

    private: G4UIcmdWithADoubleAndUnit* _scl_radiusCmd;
    private: G4UIcmdWithADoubleAndUnit* _scl_holeACmd;
    private: G4UIcmdWithADoubleAndUnit* _scl_holeBCmd;
    private: G4UIcmdWithADoubleAndUnit* _scl_halfzCmd;

    private: G4UIcmdWithADoubleAndUnit* _checkOverlapsCmd;
#pragma endregion
};

