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

    // Set methods
    private: void DefineMaterials();
    private: G4VPhysicalVolume* DefineVolumes();
  
#pragma region Data
    private: G4LogicalVolume*   _logicTarget;    // pointer to the logical Target

    private: G4Material*        _Nickel;         // pointer to the source material
    private: G4Material*        _Tungsten;       // pointer to the collimator material
    private: G4Material*        _Iron;           // pointer to the shielding material
    private: G4Material*        _Air;            // pointer to the air

    private: CollimatorMessenger*  _messenger;   // messenger

    private: double                _src_radius;  // source radius
    private: double                _src_halfz;   // source half Z

    private: double                _enc_radius;  // enclosure radius

    private: double                _back_halfz;  // back shielding length

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
  
    public: double enc_radius() const
    {
        return _enc_radius;
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
  
    public: double cout_radius() const
    {
        return _cout_radius;
    }

    public: bool checkOverlaps() const
    {
        return _checkOverlaps;
    }

    public: double set_src_radius(double v)
    {
        _src_radius = v;
    }

    public: double set_src_halfz(double v)
    {
        _src_halfz = v;
    }
  
    public: double set_enc_radius(double v)
    {
        _enc_radius = v;
    }
    
    public: double set_back_halfz(double v)
    {
        _back_halfz = v;
    }

    public: double set_coll_radius(double v)
    {
        _coll_radius = v;
    }

    public: double set_coll_halfz(double v)
    {
        _coll_halfz = v;
    }
  
    public: double set_cout_radius(double v)
    {
        _cout_radius;
    }

    public: void set_checkOverlaps(bool v)
    {
        _checkOverlaps = v;
    }
#pragma endregion
};
