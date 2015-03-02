// -*- C++ -*-

#include <iostream>

#include "globals.hh"
#include "CollimatorConstruction.hh"
#include "CollimatorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4ThreeVector.hh"

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

CollimatorConstruction::CollimatorConstruction():
    G4VUserDetectorConstruction(),
    _Nickel{nullptr},
    _Tungsten{nullptr},
    _Iron{nullptr},
    _Air{nullptr},
    
    _src_radius{-1.0},
    _src_halfz{-1.0},
    _src_shiftz{-1.0},

    _enc_radius{-1.0},
    _enc_halfz{-1.0},

    _opn_radius{-1.0},
    _opn_halfz{-1.0},

    _pcl_radius{-1.0},
    _pcl_halfz{-1.0},
    
    _air_gap{-1.0},

    _coll_radius{-1.0},
    _coll_halfz{-1.0},
    
    _scl_radius{-1.0},
    _scl_holeA{-1.0},
    _scl_holeB{-1.0},
    _scl_halfz{-1.0},
    
    _checkOverlaps{true},
    
    _grayIron{nullptr},
    _blueCobalt{nullptr},
    _graySS{nullptr},
    _grayAl{nullptr},
    _blackLead{nullptr},
    _clrTungsten{nullptr}
    
{
    _messenger = new CollimatorMessenger(this);
}
 
CollimatorConstruction::~CollimatorConstruction()
{
    delete _messenger;
}

G4VPhysicalVolume* CollimatorConstruction::Construct()
{
    DefineMaterials();
    DefineColors();

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

void CollimatorConstruction::DefineColors()
{
    _grayIron = new G4VisAttributes(G4Colour(0.5 ,0.5 ,0.5));
    _grayIron -> SetVisibility(true);
    _grayIron -> SetForceSolid(true);

    _blueCobalt = new G4VisAttributes(G4Colour(0. ,0. ,0.7));
    _blueCobalt -> SetVisibility(true);
    _blueCobalt -> SetForceSolid(true);

    _graySS = new G4VisAttributes(G4Colour(0.9 ,0.9 ,0.9));
    _graySS -> SetVisibility(true);
    _graySS -> SetForceSolid(true);

    _grayAl = new G4VisAttributes(G4Colour(0.7 ,0.7 ,0.7));
    _grayAl -> SetVisibility(true);
    _grayAl -> SetForceSolid(true);

    _blackLead = new G4VisAttributes(G4Colour(0.2 ,0.2 ,0.2));
    _blackLead -> SetVisibility(true);
    _blackLead -> SetForceSolid(true);
 
    _clrTungsten = new G4VisAttributes(G4Colour(0.3 ,0.3 ,0.6));
    _clrTungsten -> SetVisibility(true);
    _clrTungsten -> SetForceSolid(true);
}

G4LogicalVolume* CollimatorConstruction::BuildPrimaryCollimator()
{
    // Enclosure around primary collimator
    auto encTube = new G4Tubs("enclosure", 0.0, _enc_radius, _enc_halfz, 0.0*deg, 360.0*deg);
    auto encLV   = new G4LogicalVolume(encTube, _Iron, "enclosure", 0, 0, 0);
    encLV->SetVisAttributes(_grayIron);
    
    // Source
    auto sourceTube = new G4Tubs("source", 0.0, _src_radius, _src_halfz, 0.0*deg, 360.0*deg);
    auto sourceLV   = new G4LogicalVolume(sourceTube, _Nickel, "source", 0, 0, 0);
    
    new G4PVPlacement(nullptr,                              // no rotation
		              G4ThreeVector(0.0, 0.0, _src_shiftz), // at (0,0,shiftz)
		              sourceLV,        // its logical volume
                      "source",        // its name
                      encLV,           // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps 
    sourceLV->SetVisAttributes(_blueCobalt);
                      
    // Primary opening
    auto opnTube = new G4Tubs("opening", 0.0, _opn_radius, _opn_halfz, 0.0*deg, 360.0*deg);
    auto opnLV   = new G4LogicalVolume(opnTube, _Air, "opening", 0, 0, 0);
    
    new G4PVPlacement(nullptr,                             // no rotation
		              G4ThreeVector(0.0, 0.0, _enc_halfz - _opn_halfz), // opening at the end of the enclosure
		              opnLV,           // its logical volume
                      "opening",       // its name
                      encLV,           // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps 
    opnLV->SetVisAttributes(_grayAl);
    
    // Primary tungsten collimator
    auto pclTube = new G4Tubs("PCL", _opn_radius, _pcl_radius, _pcl_halfz, 0.0*deg, 360.0*deg);
    auto pclLV   = new G4LogicalVolume(pclTube, _Tungsten, "PCL", 0, 0, 0);
    
    new G4PVPlacement(nullptr,                             // no rotation
		              G4ThreeVector(0.0, 0.0, _enc_halfz - _pcl_halfz - 0.5*(_opn_halfz - _pcl_halfz)), // primary collimator starts at the same as opening position
		              pclLV,           // its logical volume
                      "PCL",           // its name
                      encLV,           // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps 
    pclLV->SetVisAttributes(_clrTungsten);
    
    return encLV;
}

G4LogicalVolume* CollimatorConstruction::BuildSecondaryCollimator()
{
    // Air volume around secondary collimator
    auto airTube = new G4Tubs("aircyl", 0.0, _coll_radius, _coll_halfz, 0.0*deg, 360.0*deg);
    auto airLV   = new G4LogicalVolume(airTube, _Air, "aircyl", 0, 0, 0);
    
    // Iron enclosure tube inside
    auto ironTube = new G4Tubs("irontube", _scl_radius, _coll_radius, _coll_halfz, 0.0*deg, 360.0*deg);
    auto ironLV   = new G4LogicalVolume(ironTube, _Iron, "irontube", 0, 0, 0);
    new G4PVPlacement(nullptr,                      // no rotation
		              G4ThreeVector(0.0, 0.0, 0.0), // iron tube has same length, no shift
		              ironLV,               // its logical volume
                      "irontube",           // its name
                      airLV,                // its mother volume
                      false,                // no boolean operations
                      0,                    // copy number
                      _checkOverlaps);      // checking overlaps 
    ironLV->SetVisAttributes(_grayIron);
    
    // tungsten secondary collimator
    auto sclCone = new G4Cons("scl", _scl_holeA, _scl_radius, _scl_holeB, _scl_radius, _scl_halfz, 0.0*deg, 360.0*deg);
    auto sclLV   = new G4LogicalVolume(sclCone, _Tungsten, "scl", 0, 0, 0);
    new G4PVPlacement(0,                           // no rotation
                      G4ThreeVector(0.0, 0.0, -(_coll_halfz - _scl_halfz)), // shifted forward
                      sclLV,                       // its logical volume
                      "scl",                       // its name
                      airLV,                       // its mother volume
                      false,                       // no boolean operations
                      0,                           // copy number
                      _checkOverlaps);             // checking overlaps
    sclLV->SetVisAttributes(_clrTungsten);
    
    return airLV;
}

// Definitions of Solids, Logical Volumes, Physical Volumes
G4VPhysicalVolume* CollimatorConstruction::DefineVolumes()
{    
    // World
    double worldXY = 10.0*cm;
    double worldZ  = 60.0*cm;
    
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldZ);
    
    std::cout << "Computed tolerance = "
              << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
              << " mm" << G4endl;

    auto worldBox = new G4Box("World",  0.5*worldXY, 0.5*worldXY, 0.5*worldZ);
    auto worldLV  = new G4LogicalVolume( worldBox,  // its solid
                                         _Air,      // its material
                                        "World" );  // its name
  
    G4VPhysicalVolume* worldPV = new G4PVPlacement(
                 nullptr,         // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 nullptr,         // its mother volume
                 false,           // no boolean operations
                 0,               // copy number
                 _checkOverlaps); // checking overlaps
                 
    //     
    // Envelope
    //
    auto solidEnv = new G4Box("Envelope",                    
                               0.5*0.9*worldXY, 0.5*0.9*worldXY, 0.5*0.9*worldZ);
      
    auto logicEnv = new G4LogicalVolume(solidEnv,         //its solid
                                        _Air,             //its material
                                       "Envelope");       //its name
               
    new G4PVPlacement(0,                       //no rotation
                      G4ThreeVector(),         //at (0,0,0)
                      logicEnv,                //its logical volume
                      "Envelope",              //its name
                      worldLV,                 //its mother  volume
                      false,                   //no boolean operation
                      0,                       //copy number
                      _checkOverlaps);          //overlaps checking
  
    auto priColl = BuildPrimaryCollimator();
     
    new G4PVPlacement(nullptr,                               // no rotation
		              G4ThreeVector(0.0, 0.0, -_src_shiftz), // position primary collimator such that center is at source
		              priColl,         // its logical volume
                      "PCL",           // its name
                      logicEnv,        // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps 

    auto secColl = BuildSecondaryCollimator(); 
    new G4PVPlacement(nullptr,                             // no rotation
		              G4ThreeVector(0.0, 0.0, (_enc_halfz - _src_shiftz) + _air_gap + _coll_halfz), // secondary collimator after primary with air gap in between
		              secColl,         // its logical volume
                      "SCL",           // its name
                      logicEnv,        // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps 

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
