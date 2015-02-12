#pragma once

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "troika.hh"

class G4ParticleGun;
class G4Event;

class Source : public G4VUserPrimaryGeneratorAction
{
    public: constexpr double E_lo = 1.17*MeV;
    public: constexpr double E_hi = 1.33*MeV;
 
    public: constexpr double P_lo = 0.5;
    public: constexpr double P_hi = 0.5;
  
#pragma region Data
    private: G4ParticleGun*             _particleGun;

    private: double _radius;
    private: double _halfz;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: Source(double radius, double halfz);
    public: ~Source();
#pragma endregion

#pragma region Mutators
    protected: troika sample_direction(double x0, double y0, double z0) const;

    public: void GeneratePrimaries(G4Event* anEvent);

    public: void set_radius(double val)
    {
        _radius = val;
    }

    public: void set_halfz(double v)
    {
        _half_z = v;
    }
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
};
