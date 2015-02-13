#include "G4RunManager.hh"
#include "globals.hh"
#include "G4ProcessManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4PhysListFactory.hh"
#include "CollPhysicsList.hh"
#include "CollPhysicsListMessenger.hh"
#include "G4SystemOfUnits.hh"

// Physic lists (contained inside the Geant4 source code, in the 'physicslists folder')
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4LossTableManager.hh"


CollPhysicsList::CollPhysicsList(double cuts):
    G4VModularPhysicsList(),
    _emPhysicsList(nullptr),
    _decayPhysicsList(nullptr)
{
    G4LossTableManager::Instance();

    defaultCutValue = cuts;

    std::cout << "Cut set to:" << G4BestUnit(cuts,"Length") << std::endl;

    _cutForGamma     = defaultCutValue;
    _cutForElectron  = defaultCutValue;
    _cutForPositron  = defaultCutValue;

    _messenger = new CollPhysicsListMessenger(this);

    int ver{1};
    SetVerboseLevel(ver);

    // EM physics
    _emPhysicsList = new G4EmStandardPhysics_option3(ver);
    _emName = G4String("standard_opt3");

    // Decay physics and all particles
    _decayPhysicsList = new G4DecayPhysics(ver);
}

CollPhysicsList::~CollPhysicsList()
{ 
    delete _messenger;

    delete _emPhysicsList;
    delete _decayPhysicsList;
}

void CollPhysicsList::ConstructParticle()
{
    _decayPhysicsList->ConstructParticle();
    _emPhysicsList->ConstructParticle();
}

void CollPhysicsList::ConstructProcess()
{
    AddTransportation();

    _emPhysicsList->ConstructProcess();
    _decayPhysicsList->ConstructProcess();
}

void CollPhysicsList::AddPhysicsList(const G4String& name)
{
    if (verboseLevel>1)
	   G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;

    if (name == _emName)
        return;

    /////////////////////////////////////////////////////////////////////////////
    //   ELECTROMAGNETIC MODELS
    /////////////////////////////////////////////////////////////////////////////
    if (name == "standard_opt3")
    {
	   _emName = name;
	   delete _emPhysicsList;
	   _emPhysicsList = new G4EmStandardPhysics_option3();
	   G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	   G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmStandardPhysics_option3" << G4endl;
    }
    else if (name == "LowE_Livermore")
    {
	   _emName = name;
	   delete _emPhysicsList;
	   _emPhysicsList = new G4EmLivermorePhysics();
	   G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	   G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << G4endl;
    }
    else if (name == "LowE_Penelope")
    {
	   _emName = name;
	   delete _emPhysicsList;
	   _emPhysicsList = new G4EmPenelopePhysics();
	   G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	   G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << G4endl;
    } 
    else
    {
	   G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
	          << " is not defined"
	          << G4endl;
    }
}

void CollPhysicsList::SetCuts()
{  
    if (verboseLevel > 0)
    {
        G4cout << "PhysicsList::SetCuts:";
        G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
    }

    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    SetCutValue(_cutForGamma, "gamma");
    SetCutValue(_cutForElectron, "e-");
    SetCutValue(_cutForPositron, "e+");

    if (verboseLevel > 0)
        DumpCutValuesTable();
}

void CollPhysicsList::SetCutForGamma(double cut)
{
    _cutForGamma = cut;
    SetParticleCuts(_cutForGamma, G4Gamma::Gamma());
}

void CollPhysicsList::SetCutForElectron(double cut)
{
    _cutForElectron = cut;
    SetParticleCuts(_cutForElectron, G4Electron::Electron());
}

void CollPhysicsList::SetCutForPositron(double cut)
{
    _cutForPositron = cut;
    SetParticleCuts(_cutForPositron, G4Positron::Positron());
}
