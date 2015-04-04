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

        // std::cout << "Scoring volume assigned: " << (void*)_scoringVolume << std::endl;
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
        std::cout << "GGG: "; // gamma, a.k.a. photon
    else
    {
        if (chg < 0.0)
            std::cout << "EEE: "; // electron
        else
            std::cout << "PPP: "; // positron
    }
    
    std::cout << std::scientific << std::setw(15) << std::setprecision(4) << wgt
              << std::scientific << std::setw(15) << std::setprecision(4) << ekn
              << std::scientific << std::setw(15) << std::setprecision(4) << pos.x()
              << std::scientific << std::setw(15) << std::setprecision(4) << pos.y()
              << std::scientific << std::setw(15) << std::setprecision(4) << pos.z()
              << std::scientific << std::setw(15) << std::setprecision(4) << dir.x()
              << std::scientific << std::setw(15) << std::setprecision(4) << dir.y()
              << std::scientific << std::setw(15) << std::setprecision(4) << dir.z() << std::endl;
}

