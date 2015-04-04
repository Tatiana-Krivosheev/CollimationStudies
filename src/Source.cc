#include <cmath>

#include "Source.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "SourceMessenger.hh"
#include "globals.hh"

#include "Randomize.hh"

// here you set global source parameters, called once per run
Source::Source():
    _particleGun{nullptr},
    _sourceMessenger{nullptr},
    _radius{-1.0},
    _halfz{-1.0}
{
    int nof_particles = 1;

    _particleGun = new G4ParticleGun(nof_particles);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "geantino";
    _particleGun->SetParticleDefinition(particleTable->FindParticle(particleName));

    _particleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    _particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    _particleGun->SetParticleEnergy(1000.0*MeV);
    
    _sourceMessenger = new SourceMessenger(this);
}

Source::~Source()
{
    delete _particleGun;
    delete _sourceMessenger;
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
    // here we sample spatial decay vertex uniformly in the cylinder
    double z   = _halfz * ( 2.0*G4UniformRand() - 1.0 );
    double phi = 2.0 * M_PI * G4UniformRand();
    double r   = _radius * sqrt(G4UniformRand());

    // _particleGun->SetParticlePosition(G4ThreeVector(r * cos(phi), r * sin(phi), z));

    // auto dir = sample_direction();
    // _particleGun->SetParticleMomentumDirection(G4ThreeVector(dir._wx, dir._wy, dir._wz));
    
    
    /*
    auto e = sample_energy();
    _particleGun->SetParticleEnergy(e);
    */

    _particleGun->GeneratePrimaryVertex(anEvent);
}

