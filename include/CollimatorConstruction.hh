// -*- C++ -*-

#pragma once

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "tls.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;

class CollimatorMessenger;

/// Detector construction class to define materials, geometry
/// and global uniform magnetic field.

class CollimatorConstruction : public G4VUserDetectorConstruction
{
    public: CollimatorConstruction();
    public: virtual ~CollimatorConstruction();

    public: virtual G4VPhysicalVolume* Construct() override;
    public: virtual void ConstructSDandField() override;

    // Set methods
    public: void SetTargetMaterial(G4String );
    public: void SetChamberMaterial(G4String );
    public: void SetMaxStep ( double );
    public: void SetCheckOverlaps( bool );

    
    private: void DefineMaterials();
    private: G4VPhysicalVolume* DefineVolumes();
  
#pragma region Data
    private: G4LogicalVolume*   fLogicTarget;     // pointer to the logical Target

    private: G4Material*        _Nickel;         // pointer to the source material
    private: G4Material*        _Tungsten;       // pointer to the collimator material
    private: G4Material*        _Iron;           // pointer to the shielding material

    private: G4UserLimits*      _stepLimit;            // pointer to user step limits

    private: CollimatorMessenger*  _messenger;   // messenger

    private: bool  fCheckOverlaps; // option to activate checking of volumes overlaps
#pragma endregion
};
