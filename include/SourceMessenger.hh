// -*- C++ -*-

#pragma once

#include "globals.hh"
#include "G4UImessenger.hh"

class Source;
class G4UImessenger;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class SourceMessenger : public G4UImessenger 
{
    public:	SourceMessenger(Source* source);
	public: ~SourceMessenger();
	public: virtual void SetNewValue(G4UIcommand* cmd, G4String newValue) override;

#pragma region Data
    private: Source* _source;

    private: G4UIdirectory*           _detDirectory;

	private: G4UIcmdWithADoubleAndUnit* _radiusCmd;
	private: G4UIcmdWithADoubleAndUnit* _halzCmd;
#pragma endregion
};

