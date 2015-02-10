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

/// Detector construction class to define materials & geometry

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
    
    private: void DefineMaterials();
    private: G4VPhysicalVolume* DefineVolumes();
  
#pragma region Data
    private: G4LogicalVolume*   _logicTarget;    // pointer to the logical Target

    private: G4Material*        _Nickel;         // pointer to the source material
    private: G4Material*        _Tungsten;       // pointer to the collimator material
    private: G4Material*        _Iron;           // pointer to the shielding material
    private: G4Material*        _Air;            // pointer to the air

    private: G4UserLimits*      _stepLimit;      // pointer to user step limits

    private: CollimatorMessenger*  _messenger;   // messenger

    private: double                _src_radius;
    private: double                _src_halfz;

    private: double                _enc_radius;

    private: double                _back_halfz;

    private: double                _coll_radius;
    private: double                _coll_halfz;

    private: double                _cout_radius;
  
    private: bool                  _checkOverlaps;               // option to activate checking of volumes overlaps
#pragma endregion
  
#pragma region Observers
    public: double src_radius() const
    {
        return _src_radius;
    }

    public: double src_halfz() const
    {
        return _src_halfz;
    }
  
    public: double back_halfz() const
    {
        return _back_halfz;
    }

    public: double coll_radius() const
    {
        return _coll_radius;
    }

    public: double coll_halfz() const
    {
        return _coll_halfz;
    }
  
    public: void SetCheckOverlaps(bool checkOverlaps)
    {
        _checkOverlaps = checkOverlaps;
    }
#pragma endregion
};
