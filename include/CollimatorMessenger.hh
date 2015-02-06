// -*- C++ -*-

#pragma once

#include "globals.hh"
#include "G4UImessenger.hh"

class CollimatorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

/// Messenger class that defines commands for B2aDetectorConstruction.
///
/// It implements commands:
/// - /B2/det/setTargetMaterial name
/// - /B2/det/setChamberMaterial name
/// - /B2/det/stepMax value unit

class CollimatorMessenger: public G4UImessenger
{
    public: CollimatorMessenger(CollimatorConstruction* );
    public: virtual ~CollimatorMessenger();
    
    public: virtual void SetNewValue(G4UIcommand*, G4String) override;
    
    private: CollimatorConstruction*  _collimator;

    G4UIdirectory*           fB2Directory;
    G4UIdirectory*           fDetDirectory;

    G4UIcmdWithAString*      fTargMatCmd;
    G4UIcmdWithAString*      fChamMatCmd;

    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
};
