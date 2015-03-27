#include <iostream>
#include <iomanip>

#include "SteppingAction.hh"
#include "CollimatorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

SteppingAction::SteppingAction(G4LogicalVolume* scoringVolume):
    G4UserSteppingAction(),
    _scoringVolume(scoringVolume)
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    if (_scoringVolume == nullptr)
    { 
        const CollimatorConstruction* coll = static_cast<const CollimatorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        _scoringVolume = coll->GetScoringVolume();   
    }

    // get step start point
    const G4StepPoint* pt = step->GetPreStepPoint();

    // get volume of the current step
    G4LogicalVolume* volume = pt->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
      
    if (volume == nullptr)
        return;
    
    // check if we are in scoring volume
    if (volume != _scoringVolume)
        return;
    
    // get all particle coordinates    
    auto pos = pt->GetPosition();
    auto dir = pt->GetMomentumDirection();
    auto ekn = pt->GetKineticEnergy();
    auto wgt = pt->GetWeight();
    
    auto chg = pt->GetCharge();
    
    if (chg == 0.0)
        std::cout << "G: "; // gamma, a.k.a. photon
    else
    {
        if (chg < 0.0)
            std::cout << "E: "; // electron
        else
            std::cout << "P: "; // positron
    }
    
    std::cout << std::scientific << std::setw(14) << std::setprecision(4);
    std::cout << wgt << ekn
              << pos.x() << pos.y() << pos.z()
              << dir.x() << dir.y() << dir.z() << "\n";
}

