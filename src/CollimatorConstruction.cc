// -*- C++ -*-

#include <iostream>

#include "CollimatorConstruction.hh"
#include "CollimatorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

ColimatorConstruction::CollimatorConstruction(double src_radius,
					      double src_halfz):
    G4VUserDetectorConstruction(),
    _Nickel(nullptr),
    _Tungsten(nullptr),
    _Iron(nullptr),
    _Air(nullptr),
    _stepLimit(nullptr),
    _
    _checkOverlaps(true)
{
    _messenger = new CollimatorMessenger(this);
}
 
ColimatorConstruction::~ColimatorConstruction()
{
    delete _stepLimit;
    delete _messenger;
}

G4VPhysicalVolume* ColimatorConstruction::Construct()
{
    DefineMaterials();

    return DefineVolumes();
}

void CollimatorConstruction::DefineMaterials()
{
    // Material definition 
    G4NistManager* nistManager = G4NistManager::Instance();

    // Air defined using NIST Manager
    _Air = nistManager->FindOrBuildMaterial("G4_AIR");
  
    // Iron defined using NIST Manager
    _Iron = nistManager->FindOrBuildMaterial("G4_Fe");

    // Wolfram defined using NIST Manager
    _Tungsten = nistManager->FindOrBuildMaterial("G4_W");
    
    // Nickel defined using NIST Manager
    _Nickel = nistManager->FindOrBuildMaterial("G4_Ni");

    // Print materials
    std::cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* CollimatorConstruction::DefineVolumes()
{
    // Definitions of Solids, Logical Volumes, Physical Volumes

    // World
    double wl = 50.0*cm;
  
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(wl);

    std::cout << "Computed tolerance = "
              << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
              << " mm" << G4endl;

    G4Box* worldS = new G4Box("world",  0.5*wl, 0.5*wl, 0.5*wl);
    G4LogicalVolume* worldLV = new G4LogicalVolume( worldS,   //its solid
                                                    _Air,     //its material
                                                  "World" ); //its name
  
    G4VPhysicalVolume* worldPV = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother volume
                 false,           // no boolean operations
                 0,               // copy number
                 _checkOverlaps); // checking overlaps 

    // Source
    auto sourceTube = new G4Tubs("source", 0.0, _src_radius, _src_halfz, 0.0*deg, 360.0*deg);
  
    _source  = new G4LogicalVolume(sourceTube, _Nickel, "source", 0, 0, 0);
    
    new G4PVPlacement(0,               // no rotation
		      G4ThreeVector(), // at (0,0,0)
		      _source,         // its logical volume
                      "source",        // its name
                      worldLV,         // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps 

    // Source enclosure 
    auto srcEnclosure = new G4Tubs("enclosure", _src_radius, _enc_radius, _src_halfz, 0.0*deg, 360.0*deg);
    G4LogicalVolume* encLV = new G4LogicalVolume(srcEnclosure, _Iron, "Enclosure", 0, 0, 0);
    new G4PVPlacement(0,                           // no rotation
                      G4TheeVector(0.0, 0.0, 0.0), // at (0, 0, 0)
                      encLV,                       // its logical volume
                      "Enclosure",                 // its name
                      worldLV,                     // its mother volume
                      false,                       // no boolean operations
                      0,                           // copy number
                      _checkOverlaps);             // checking overlaps

    // source backside shielding
    auto srcBack = new G4Tubs("enclosure", 0.0, _enc_radius, _back_halfz, 0.0*deg, 360.0*deg);
    G4LogicalVolume* backLV = new G4LogicalVolume(srcBack, _Iron, "Back", 0, 0, 0);
    new G4PVPlacement(0,                           // no rotation
                      G4TheeVector(0.0, 0.0, -0.5*_src_halfz - 0.5*_back_halfz), // shifted back
                      backLV,                      // its logical volume
                      "Back",                      // its name
                      worldLV,                     // its mother volume
                      false,                       // no boolean operations
                      0,                           // copy number
                      _checkOverlaps);             // checking overlaps

    // tungsten collimator
    auto srcBack = new G4Tubs("collimator", 0.0, _coll_radius, _coll_halfz, 0.0*deg, 360.0*deg);
    G4LogicalVolume* collLV = new G4LogicalVolume(srcBack, _Tungsten, "Collimator", 0, 0, 0);
    new G4PVPlacement(0,                           // no rotation
                      G4TheeVector(0.0, 0.0,  + 0.5*_src_halfz + 0.5*_coll_halfz), // shifted forward
                      collLV,                      // its logical volume
                      "Back",                      // its name
                      worldLV,                     // its mother volume
                      false,                       // no boolean operations
                      0,                           // copy number
                      _checkOverlaps);             // checking overlaps
    
    // air cone in collimator
    auto srcBack = new G4Cone("chole", 0.0, _src_radius, _cout_radius, coll_halfz, 0.0*deg, 360.0*deg);
    G4LogicalVolume* choleLV = new G4LogicalVolume(srcBack, _Air, "Chole", 0, 0, 0);
    new G4PVPlacement(0,                           // no rotation
                      G4TheeVector(0.0, 0.0, 0.0), // not shifted
                      choleLV,                     // its logical volume
                      "Back",                      // its name
                      collLV,                      // its mother volume, which is collimator tube
                      false,                       // no boolean operations
                      0,                           // copy number
                      _checkOverlaps);             // checking overlaps

    // Visualization attributes
    G4VisAttributes* boxVisAtt     = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));

    worldLV      ->SetVisAttributes(boxVisAtt);
    fLogicTarget ->SetVisAttributes(boxVisAtt);
    trackerLV    ->SetVisAttributes(boxVisAtt);

    // Example of User Limits
    //
    // Below is an example of how to set tracking constraints in a given
    // logical volume
    //
    // Sets a max step length in the tracker region, with G4StepLimiter

    double maxStep = 0.5*chamberWidth;
    _stepLimit     = new G4UserLimits(maxStep);
    trackerLV->SetUserLimits(fStepLimit);
 
    // Always return the physical world

    return worldPV;
}

void CollimatorConstruction::SetTargetMaterial(G4String materialName)
{
    G4NistManager* nistManager = G4NistManager::Instance();

    G4Material* pttoMaterial = nistManager->FindOrBuildMaterial(materialName);

    if (fTargetMaterial != pttoMaterial)
    {
      if ( pttoMaterial ) {
        fTargetMaterial = pttoMaterial;
        if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
        G4cout 
          << G4endl 
          << "----> The target is made of " << materialName << G4endl;
     } else {
        G4cout 
          << G4endl 
          << "-->  WARNING from SetTargetMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
}

void CollimatorConstruction::SetChamberMaterial(G4String materialName)
{
    G4NistManager* nistManager = G4NistManager::Instance();

    G4Material* pttoMaterial =  nistManager->FindOrBuildMaterial(materialName);

    if (fChamberMaterial != pttoMaterial)
    {
        if ( pttoMaterial )
	{
        fChamberMaterial = pttoMaterial;
        for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {
            if (fLogicChamber[copyNo]) fLogicChamber[copyNo]->
                                               SetMaterial(fChamberMaterial);
        }
        G4cout 
          << G4endl 
          << "----> The chambers are made of " << materialName << G4endl;
     } else {
        G4cout 
          << G4endl 
          << "-->  WARNING from SetChamberMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
}

void CollimatorConstruction::SetMaxStep(G4double maxStep)
{
    if (_stepLimit != nullptr && maxStep > 0.0)
        _stepLimit->SetMaxAllowedStep(maxStep);
}
