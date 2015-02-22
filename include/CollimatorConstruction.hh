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
    private: double                _src_shiftz;  // source shift Z wrt enclosure
    
    private: double                _enc_radius;  // source enclosure radius
    private: double                _enc_halfz;   // source enclosure half Z
    
    private: double                _opn_radius;  // opening radius
    private: double                _opn_halfz;   // opening half Z    

    private: double                _pcl_radius;  // primary collimator external radius, internal same as opening
    private: double                _pcl_halfz;   // primary collimator half Z    

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
  
    public: double src_shiftz() const
    {
        return _src_shiftz;
    }

    public: double enc_radius() const
    {
        return _enc_radius;
    }
    
    public: double enc_halfz() const
    {
        return _enc_halfz;
    }
    
    public: double opn_radius() const
    {
        return _opn_radius;
    }

    public: double opn_halfz() const
    {
        return _opn_halfz;
    }
  
    public: double pcl_radius() const
    {
        return _pcl_radius;
    }

    public: double pcl_halfz() const
    {
        return _pcl_halfz;
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
#pragma endregion

#pragma region Mutators
    public: void set_src_radius(double v)
    {
        _src_radius = v;
    }

    public: void set_src_halfz(double v)
    {
        _src_halfz = v;
    }
  
    public: void set_src_shiftz(double v)
    {
        _src_shiftz = v;
    }

    public: void set_enc_radius(double v)
    {
        _enc_radius = v;
    }
    
    public: void set_enc_halfz(double v)
    {
        _enc_halfz = v;
    }

    public: void set_opn_radius(double v)
    {
        _opn_radius = v;
    }

    public: void set_opn_halfz(double v)
    {
        _opn_halfz = v;
    }
  
    public: void set_pcl_radius(double v)
    {
        _pcl_radius = v;
    }

    public: void set_pcl_halfz(double v)
    {
        _pcl_halfz = v;
    }

    public: void set_coll_radius(double v)
    {
        _coll_radius = v;
    }

    public: void set_coll_halfz(double v)
    {
        _coll_halfz = v;
    }
  
    public: void set_cout_radius(double v)
    {
        _cout_radius = v;
    }

    public: void set_checkOverlaps(bool v)
    {
        _checkOverlaps = v;
    }
#pragma endregion
};
