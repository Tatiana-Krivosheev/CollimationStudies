#include <cmath>

#include "Source.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"

#include "Randomize.hh"

// here you set global source parameters, called once per run
Source::Source(double radius, double halfz):
    _particleGun(nullptr),
    _srcMessenger(nullptr),
    _radius(radius),
    _halfz(halfz)
{
    int nof_particles = 1;

    _particleGun = new G4ParticleGun(nof_particles);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "gamma";
    _particleGun->SetParticleDefinition(particleTable->FindParticle(particleName));

    _particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    _particleGun->SetParticleEnergy(0.0);
}

Source::~Source()
{
    delete _particleGun;
}

troika Source::sample_direction()
{
    double phi   = 2.0 * M_PI * G4UniformRand();

    double cos_z = 2.0 * G4UniformRand() - 1.0;
    double sin_z = sqrt( (1.0 - cos_z) * (1.0 + cos_z) );

    return troika{ sin_z * cos(phi), sin_z * sin(phi), cos_z };
}

double Source::sample_energy()
{
    return (G4UniformRand() < P_lo) ? E_lo : E_hi;
}

// source particle parameters, called per each source event
void Source::GeneratePrimaries(G4Event* anEvent)
{
    double z = _halfz * ( 2.0*G4UniformRand() - 1.0 );
    double r = _radius * sqrt(G4UniformRand());
    double phi = 2.0 * M_PI * G4UniformRand();
  
    _particleGun->SetParticlePosition(G4ThreeVector(r * cos(phi), r * sin(phi), z));

    auto dir = sample_direction();
    _particleGun->SetParticleMomentumDirection(G4ThreeVector(dir.wx, dir._wy, dir._wz));

    _particleGun->SetParticleEnergy(sample_energy());

    _particleGun->GeneratePrimaryVertex(anEvent);
}
