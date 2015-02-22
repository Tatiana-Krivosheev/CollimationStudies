#pragma once

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"

#include "troika.hh"

class G4ParticleGun;
class G4Event;

class Source : public G4VUserPrimaryGeneratorAction
{
    public: constexpr static double E_lo = 1.17*MeV;
    public: constexpr static double E_hi = 1.33*MeV;
 
    public: constexpr static double P_lo = 0.5;
    public: constexpr static double P_hi = 0.5;
  
#pragma region Data
    private: G4ParticleGun*             _particleGun;

    private: double _radius;
    private: double _halfz;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: Source();
    public: ~Source();
#pragma endregion

#pragma region Observers
    public: double radius() const
    {
        return _radius;
    }

    public: double halfz() const
    {
        return _halfz;
    }
#pragma endregion

    public: static troika sample_direction();
    public: static double sample_energy();

#pragma region Mutators
    public: void GeneratePrimaries(G4Event* anEvent);

    public: void set_radius(double val)
    {
        _radius = val;
    }

    public: void set_halfz(double v)
    {
        _halfz = v;
    }
#pragma endregion
};
