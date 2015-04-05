#include <iostream>
#include <iomanip>
#include <mutex>

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

static std::mutex mtx;

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    static const char* PHOTON   = "GGG: ";
    static const char* ELECTRON = "EEE: ";
    static const char* POSITRON = "PPP: ";

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
    
    const char* tag = PHOTON;
    if (chg != 0.0)
    {
        tag = ELECTRON;
        if (chg > 0.0)
            tag = POSITRON;
    }
    
    mtx.lock();

    std::cout << tag
              << std::scientific << std::setw(15) << std::setprecision(4) << wgt
              << std::scientific << std::setw(15) << std::setprecision(4) << ekn
              << std::scientific << std::setw(15) << std::setprecision(4) << pos.x()
              << std::scientific << std::setw(15) << std::setprecision(4) << pos.y()
              << std::scientific << std::setw(15) << std::setprecision(4) << pos.z()
              << std::scientific << std::setw(15) << std::setprecision(4) << dir.x()
              << std::scientific << std::setw(15) << std::setprecision(4) << dir.y()
              << std::scientific << std::setw(15) << std::setprecision(4) << dir.z() << std::endl;
              
    mtx.unlock();
}

