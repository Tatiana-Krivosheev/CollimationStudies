// -*- C++ -*-

#include <iostream>

#include "CollimatorConstruction.hh"
#include "CollimatorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

ColimatorConstruction::CollimatorConstruction(double src_radius,
					                          double src_halfz):
    G4VUserDetectorConstruction(),
    _Nickel(nullptr),
    _Tungsten(nullptr),
    _Iron(nullptr),
    _Air(nullptr),
    _checkOverlaps(true)
{
    _messenger = new CollimatorMessenger(this);
}
 
ColimatorConstruction::~ColimatorConstruction()
{
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
    double wl = 10.0*cm;
  
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
                      G4TheeVector(0.0, 0.0, -_src_halfz - _back_halfz), // shifted back
                      backLV,                      // its logical volume
                      "Back",                      // its name
                      worldLV,                     // its mother volume
                      false,                       // no boolean operations
                      0,                           // copy number
                      _checkOverlaps);             // checking overlaps

    // tungsten collimator
    auto collW = new G4Tubs("collimator", 0.0, _coll_radius, _coll_halfz, 0.0*deg, 360.0*deg);
    G4LogicalVolume* collLV = new G4LogicalVolume(collW, _Tungsten, "Collimator", 0, 0, 0);
    new G4PVPlacement(0,                           // no rotation
                      G4TheeVector(0.0, 0.0,  +_src_halfz + _coll_halfz), // shifted forward
                      collLV,                      // its logical volume
                      "Back",                      // its name
                      worldLV,                     // its mother volume
                      false,                       // no boolean operations
                      0,                           // copy number
                      _checkOverlaps);             // checking overlaps
    
    // air cone in collimator
    auto collOpen = new G4Cons("hole", 0.0, _src_radius, 0.0, _cout_radius, coll_halfz, 0.0*deg, 360.0*deg);
    G4LogicalVolume* choleLV = new G4LogicalVolume(collOpen, _Air, "hole", 0, 0, 0);
    new G4PVPlacement(0,                           // no rotation
                      G4TheeVector(0.0, 0.0, 0.0), // not shifted
                      choleLV,                     // its logical volume
                      "Back",                      // its name
                      collLV,                      // its mother volume, which is collimator tube
                      false,                       // no boolean operations
                      0,                           // copy number
                      _checkOverlaps);             // checking overlaps

    // Visualization attributes
    // Visualisation attributes of all elements colours 
    G4VisAttributes * grayIron = new G4VisAttributes(G4Colour(0.5 ,0.5 ,0.5));
    grayIron -> SetVisibility(true);
    grayIron -> SetForceSolid(true);

    G4VisAttributes * blueCobalt = new G4VisAttributes(G4Colour(0. ,0. ,0.7));
    blueCobalt -> SetVisibility(true);
    blueCobalt -> SetForceSolid(true);

    G4VisAttributes * graySS = new G4VisAttributes(G4Colour(0.9 ,0.9 ,0.9));
    graySS -> SetVisibility(true);
    graySS -> SetForceSolid(true);

    G4VisAttributes * grayAl = new G4VisAttributes(G4Colour(0.7 ,0.7 ,0.7));
    grayAl -> SetVisibility(true);
    grayAl -> SetForceSolid(true);

    G4VisAttributes * blackLead = new G4VisAttributes(G4Colour(0.2 ,0.2 ,0.2));
    blackLead -> SetVisibility(true);
    blackLead -> SetForceSolid(true);
 
    G4VisAttributes * colorTungsten = new G4VisAttributes(G4Colour(0.3 ,0.3 ,0.3));
    colorTungsten -> SetVisibility(true);
    colorTungsten -> SetForceSolid(true);
 
    // Always return the physical world

    return worldPV;
}

/*
void CollimatorConstruction::SetMaxStep(G4double maxStep)
{
    if (_stepLimit != nullptr && maxStep > 0.0)
        _stepLimit->SetMaxAllowedStep(maxStep);
}
*/
