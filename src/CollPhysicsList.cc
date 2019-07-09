#include "CollPhysicsList.hh"
#include "CollPhysicsListMessenger.hh"

#include "G4RunManager.hh"

#include "G4PhysicalConstants.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4UnitsTable.hh"

#include "G4DecayPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4StoppingPhysics.hh"

#include "G4EmProcessOptions.hh"
#include "G4LossTableManager.hh"

#include "G4PhysListFactory.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

static constexpr char NL = '\n';

CollPhysicsList::CollPhysicsList(double cuts):
    G4VModularPhysicsList{},
    _emPhysicsList{nullptr},
    _decayPhysicsList{nullptr},
    _emExtraPhysicsList{nullptr},
    _messenger{nullptr}
{
    G4LossTableManager::Instance();

    defaultCutValue = cuts;

    std::cout << "Cut set to:" << G4BestUnit(cuts, "Length") << NL;

    _cutForGamma     = defaultCutValue;
    _cutForElectron  = defaultCutValue;
    _cutForPositron  = defaultCutValue;

    _messenger = new CollPhysicsListMessenger(this);

    int ver{1};
    SetVerboseLevel(ver);

    // EM physics
    _emPhysicsList = new G4EmLowEPPhysics(ver);
    _emName = std::string("emlowenergy");

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

void CollPhysicsList::AddPhysicsList(const std::string& name)
{
    if (verboseLevel>1)
       std::cout << "PhysicsList::AddPhysicsList: <" << name << ">" << NL;

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
       G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmStandardPhysics_option3" << NL;
       _emPhysicsList->ConstructProcess();
       goto end;
    }

    if (name == "emlowenergy")
    {
       _emName = name;
       delete _emPhysicsList;
       _emPhysicsList = new G4EmLowEPPhysics();
       G4RunManager::GetRunManager()->PhysicsHasBeenModified();
       G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLowEPPPhysics" << NL;
        _emPhysicsList->ConstructProcess();
        goto end;
    }

    if (name == "LowE_Livermore")
    {
       _emName = name;
       delete _emPhysicsList;
       _emPhysicsList = new G4EmLivermorePhysics();
       G4RunManager::GetRunManager()->PhysicsHasBeenModified();
       G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << NL;
        _emPhysicsList->ConstructProcess();
        goto end;
    }

    if (name == "LowE_Penelope")
    {
       _emName = name;
       delete _emPhysicsList;
       _emPhysicsList = new G4EmPenelopePhysics();
       G4RunManager::GetRunManager()->PhysicsHasBeenModified();
       G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << NL;
        _emPhysicsList->ConstructProcess();
        goto end;
    }

   G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
          << " is not defined"
          << NL;

    end:
    return;
}

void CollPhysicsList::SetCuts()
{
    if (verboseLevel > 0)
    {
        G4cout << "PhysicsList::SetCuts:";
        G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << NL;
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
