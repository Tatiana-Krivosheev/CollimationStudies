// -*- C++ -*-

#pragma once

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "tls.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4VisAttributes;

class G4Material;
class G4UserLimits;

class CollimatorMessenger;

/// Detector construction class to define materials & geometry

class CollimatorConstruction : public G4VUserDetectorConstruction
{
    public:          CollimatorConstruction();
    public: virtual ~CollimatorConstruction();

    public: virtual G4VPhysicalVolume* Construct() override;
    
    public: G4LogicalVolume* GetScoringVolume() const
    {
        return _scoringVolume;
    }

    // Set methods
    private: void DefineMaterials();
    private: void DefineColors();
    private: G4VPhysicalVolume* DefineVolumes();
    
    private: G4LogicalVolume* BuildPrimaryCollimator();
    private: G4LogicalVolume* BuildSecondaryCollimator();
  
#pragma region Data
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
    
    private: double                _sss_radius;  // source enclosure radius
    private: double                _sss_halfz;   // source enclosure half Z

    private: double                _opn_radius;  // opening radius
    private: double                _opn_halfz;   // opening half Z    

    private: double                _pcl_radius;  // primary collimator external radius, internal same as opening
    private: double                _pcl_halfz;   // primary collimator half Z    

    private: double                _oair_radius;  // open air ring radius at the end around SS shell
    private: double                _oair_halfz;   // open air ring halfz at the end around SS shell
    
    private: double                _sssair_halfz;   // SS shell air end halfz

    private: double                _air_gap;     // air gap between primary and secondary collimators

    private: double                _coll_radius; // scondary collimator enclosure radius
    private: double                _coll_halfz;  // scondary collimator enclosure halfz
    
    private: double                _scl_radius;
    private: double                _scl_holeA;
    private: double                _scl_holeB;
    private: double                _scl_halfz;
  
    private: bool                  _checkOverlaps;               // option to activate checking of volumes overlaps
    
    private: G4VisAttributes*      _turqIron;
    private: G4VisAttributes*      _blueCobalt;
    private: G4VisAttributes*      _graySS;
    private: G4VisAttributes*      _grayAir;
    private: G4VisAttributes*      _redTungsten;    
    
    private: G4LogicalVolume*      _scoringVolume;
    
    private: G4UserLimits*         _stepLimit;            // pointer to user step limits
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
    
    public: double sss_radius() const
    {
        return _sss_radius;
    }
    
    public: double sss_halfz() const
    {
        return _sss_halfz;
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

    public: double oair_radius() const
    {
        return _oair_radius;
    }

    public: double oair_halfz() const
    {
        return _oair_halfz;
    }

    public: double sssair_halfz() const
    {
        return _sssair_halfz;
    }

    public: double air_gap() const
    {
        return _air_gap;
    }

    public: double coll_radius() const
    {
        return _coll_radius;
    }

    public: double coll_halfz() const
    {
        return _coll_halfz;
    }
    
    public: double scl_radius() const
    {
        return _scl_radius;
    }
    
    public: double scl_holeA() const
    {
        return _scl_holeA;
    }

    public: double scl_holeB() const
    {
        return _scl_holeB;
    }

    public: double scl_halfz() const
    {
        return _scl_halfz;
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
    
    public: void set_sss_radius(double v)
    {
        _sss_radius = v;
    }
    
    public: void set_sss_halfz(double v)
    {
        _sss_halfz = v;
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

    public: void set_oair_radius(double v)
    {
        _oair_radius = v;
    }

    public: void set_oair_halfz(double v)
    {
        _oair_halfz = v;
    }

    public: void set_sssair_halfz(double v)
    {
        _sssair_halfz = v;
    }

    public: void set_air_gap(double v)
    {
        _air_gap = v;
    }

    public: void set_coll_radius(double v)
    {
        _coll_radius = v;
    }

    public: void set_coll_halfz(double v)
    {
        _coll_halfz = v;
    }  
    
    public: void set_scl_radius(double v)
    {
        _scl_radius = v;
    }
    
    public: void set_scl_holeA(double v)
    {
        _scl_holeA = v;
    }

    public: void set_scl_holeB(double v)
    {
        _scl_holeB = v;
    }

    public: void set_scl_halfz(double v)
    {
        _scl_halfz = v;
    }

    public: void set_checkOverlaps(bool v)
    {
        _checkOverlaps = v;
    }
    
    public: void set_maxStep(double step);
#pragma endregion
};

