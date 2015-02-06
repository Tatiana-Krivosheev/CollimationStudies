#pragma once

#include <cassert>

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include "troika.hh"

class G4ParticleGun;
class G4Event;
class SourceMessenger;

class Source : public G4VUserPrimaryGeneratorAction
{
    public: constexpr double E_lo = 1.17*MeV;
    public: constexpr double E_hi = 1.33*MeV;
 
    public: constexpr double P_lo = 0.5;
    public: constexpr double P_hi = 0.5;
  
#pragma region Data
    private: G4ParticleGun*             _particleGun;
    private: SourceMessenger*           _srcMessenger;

    private: double _radius;
    private: double _half_z;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: Source(double radius, double half_z);
    public: ~Source();
#pragma endregion

#pragma region Mutators
    protected: troika sample_direction(double x0, double y0, double z0) const;

    public: void GeneratePrimaries(G4Event* anEvent);

    public: void set_radius(double val)
    {
        _radius = val;
    }

    public: void set_fullz(double val)
    {
        _half_z = 0.5*val;
    }
#pragma endregion

#pragma region Observers
    public: double radius() const
    {
        return _radius;
    }

    public: double half_z() const
    {
        return _half_z;
    }
#pragma endregion
};
