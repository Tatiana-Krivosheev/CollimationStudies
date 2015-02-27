#include <iostream>

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
        _scoringVolume = nullptr; // coll->GetScoringVolume();   
    }

    // get volume of the current step
    G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
      
    // check if we are in scoring volume
    if (volume == nullptr)
        return;
    
    if (volume != _scoringVolume)
        return;

    // collect energy deposited in this step
    const G4StepPoint* pt = step->GetPreStepPoint();
    auto pos = pt->GetPosition();
    auto chg = pt->GetCharge();
    if (chg != 0.0)
        std::cout << "Electron: ";
    else
        std::cout << "Photon  : ";
    
    std::cout << pos.x() << " " << pos.y() << " " << pos.z() << std::endl;
}

