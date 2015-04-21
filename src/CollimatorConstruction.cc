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
    
    _sss_radius{-1.0},
    _sss_halfz{-1.0},

    _opn_radius{-1.0},
    _opn_halfz{-1.0},

    _pcl_radius{-1.0},
    _pcl_halfz{-1.0},

    _oair_radius{-1.0},
    _oair_halfz{-1.0},
    
    _sssair_halfz{-1.0},
    
    _air_gap{-1.0},

    _coll_radius{-1.0},
    _coll_halfz{-1.0},

    _scl_radius{-1.0},
    _scl_holeA{-1.0},
    _scl_holeB{-1.0},
    _scl_halfz{-1.0},

    _checkOverlaps{true},

    _turqIron{nullptr},
    _blueCobalt{nullptr},
    _graySS{nullptr},
    _grayAir{nullptr},
    _redTungsten{nullptr},
    
    _scoringVolume{nullptr},
    _stepLimit{nullptr}
{
    _messenger = new CollimatorMessenger(this);
}

CollimatorConstruction::~CollimatorConstruction()
{
    delete _messenger;
    delete _stepLimit;
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
    _turqIron = new G4VisAttributes(G4Colour(0.25, 0.85, 0.65));
    _turqIron -> SetVisibility(true);
    _turqIron -> SetForceSolid(true);

    _blueCobalt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.7));
    _blueCobalt -> SetVisibility(true);
    _blueCobalt -> SetForceSolid(true);

    _graySS = new G4VisAttributes(G4Colour(0.9, 0.9, 0.9));
    _graySS -> SetVisibility(true);
    _graySS -> SetForceSolid(true);

    _grayAir = new G4VisAttributes(G4Colour(0.8, 0.8, 0.8));
    _grayAir -> SetVisibility(true);
    _grayAir -> SetForceSolid(true);

    _redTungsten = new G4VisAttributes(G4Colour(0.9, 0.2, 0.2));
    _redTungsten -> SetVisibility(true);
    _redTungsten -> SetForceSolid(true);
}

G4LogicalVolume* CollimatorConstruction::BuildPrimaryCollimator()
{
    const char* primary_enclosure  = "pri_enclosure";
    const char* primary_sss        = "pri_sss";
    const char* primary_source     = "pri_source";
    const char* primary_opening    = "pri_opening";
    const char* primary_collimator = "pri_col";
    const char* primary_outair     = "pri_outair";
    const char* primary_sssair     = "pri_sssair";

    // Enclosure around primary collimator
    auto encTube = new G4Tubs(primary_enclosure, 0.0, _enc_radius, _enc_halfz, 0.0*deg, 360.0*deg);
    auto encLV   = new G4LogicalVolume(encTube, _Tungsten, primary_enclosure, nullptr, nullptr, nullptr);
    encLV->SetVisAttributes(_redTungsten);
    
    // Stainless steel shell
    auto sssTube = new G4Tubs(primary_sss, 0.0, _sss_radius, _sss_halfz, 0.0*deg, 360.0*deg);
    auto sssLV   = new G4LogicalVolume(sssTube, _Iron, primary_sss, nullptr, nullptr, nullptr);
    sssLV->SetUserLimits(_stepLimit);
    
    new G4PVPlacement(nullptr,                              // no rotation
		              G4ThreeVector(0.0, 0.0, _enc_halfz - _sss_halfz), // at the end of the enclosure
		              sssLV,           // its logical volume
                      primary_sss,     // its name
                      encLV,           // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps
    sssLV->SetVisAttributes(_turqIron);

    // Source
    auto sourceTube = new G4Tubs(primary_source, 0.0, _src_radius, _src_halfz, 0.0*deg, 360.0*deg);
    auto sourceLV   = new G4LogicalVolume(sourceTube, _Nickel, primary_source, nullptr, nullptr, nullptr);
    sourceLV->SetUserLimits(_stepLimit);

    new G4PVPlacement(nullptr,                              // no rotation
		              G4ThreeVector(0.0, 0.0, _src_shiftz - (_enc_halfz - _sss_halfz)), // at (0,0,shiftz) relative to enclosure
		              sourceLV,        // its logical volume
                      primary_source,  // its name
                      sssLV,           // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps
    sourceLV->SetVisAttributes(_blueCobalt);

    // Primary opening
    auto opnTube = new G4Tubs(primary_opening, 0.0, _opn_radius, _opn_halfz, 0.0*deg, 360.0*deg);
    auto opnLV   = new G4LogicalVolume(opnTube, _Air, primary_opening, nullptr, nullptr, nullptr);
    opnLV->SetUserLimits(_stepLimit);
   
    new G4PVPlacement(nullptr,                             // no rotation
		              G4ThreeVector(0.0, 0.0, _sss_halfz - _opn_halfz), // opening at the end of the SS shell
		              opnLV,           // its logical volume
                      primary_opening, // its name
                      sssLV,           // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps
    opnLV->SetVisAttributes(_grayAir);

    // Primary tungsten collimator
    auto pclTube = new G4Tubs(primary_collimator, _opn_radius, _pcl_radius, _pcl_halfz, 0.0*deg, 360.0*deg);
    auto pclLV   = new G4LogicalVolume(pclTube, _Tungsten, primary_collimator, nullptr, nullptr, nullptr);
    pclLV->SetUserLimits(_stepLimit);

    new G4PVPlacement(nullptr,                             // no rotation
		              G4ThreeVector(0.0, 0.0, _sss_halfz - _pcl_halfz - 2.0*(_opn_halfz - _pcl_halfz)), // primary collimator starts at the same as opening position
		              pclLV,           // its logical volume
                      primary_collimator,  // its name
                      sssLV,           // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps
    pclLV->SetVisAttributes(_redTungsten);
    
    // SS shell end air ring
    auto sssairTube = new G4Tubs(primary_sssair, _opn_radius, _sss_radius, _sssair_halfz, 0.0*deg, 360.0*deg);
    auto sssairLV   = new G4LogicalVolume(sssairTube, _Air, primary_sssair, nullptr, nullptr, nullptr);

    new G4PVPlacement(nullptr,                             // no rotation
		              G4ThreeVector(0.0, 0.0, _sss_halfz - _sssair_halfz), // SS shell air end starts at the end of enclosure
		              sssairLV,        // its logical volume
                      primary_sssair,  // its name
                      sssLV,           // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps
    sssairLV->SetVisAttributes(_grayAir);

    // Outer air ring
    auto outairTube = new G4Tubs(primary_outair, _sss_radius, _oair_radius, _oair_halfz, 0.0*deg, 360.0*deg);
    auto outairLV   = new G4LogicalVolume(outairTube, _Air, primary_outair, nullptr, nullptr, nullptr);

    new G4PVPlacement(nullptr,                             // no rotation
		              G4ThreeVector(0.0, 0.0, _enc_halfz - _oair_halfz), // open air starts at the end of enclosure
		              outairLV,        // its logical volume
                      primary_outair,  // its name
                      encLV,           // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps
    outairLV->SetVisAttributes(_grayAir);

    return encLV;
}

G4LogicalVolume* CollimatorConstruction::BuildSecondaryCollimator()
{
    const char* secondary_air  = "sec_aircyl";
    const char* secondary_iron = "sec_irontube";
    const char* secondary_coll = "scl";

    // Air volume around secondary collimator
    auto airTube = new G4Tubs(secondary_air, 0.0, _coll_radius, _coll_halfz, 0.0*deg, 360.0*deg);
    auto airLV   = new G4LogicalVolume(airTube, _Air, secondary_air, nullptr, nullptr, nullptr);
    airLV->SetVisAttributes(_grayAir);
    airLV->SetUserLimits(_stepLimit);

    // Iron enclosure tube inside
    auto ironTube = new G4Tubs(secondary_iron, _scl_radius, _coll_radius, _coll_halfz, 0.0*deg, 360.0*deg);
    auto ironLV   = new G4LogicalVolume(ironTube, _Iron, secondary_iron, nullptr, nullptr, nullptr);
    new G4PVPlacement(nullptr,                      // no rotation
		              G4ThreeVector(0.0, 0.0, 0.0), // iron tube has same length, no shift
		              ironLV,               // its logical volume
                      secondary_iron,       // its name
                      airLV,                // its mother volume
                      false,                // no boolean operations
                      0,                    // copy number
                      _checkOverlaps);      // checking overlaps
    ironLV->SetVisAttributes(_turqIron);
    ironLV->SetUserLimits(_stepLimit);

    // tungsten secondary collimator
    auto sclCone = new G4Cons(secondary_coll, _scl_holeA, _scl_radius, _scl_holeB, _scl_radius, _scl_halfz, 0.0*deg, 360.0*deg);
    auto sclLV   = new G4LogicalVolume(sclCone, _Tungsten, secondary_coll, nullptr, nullptr, nullptr);
    new G4PVPlacement(0,                           // no rotation
                      G4ThreeVector(0.0, 0.0, -(_coll_halfz - _scl_halfz)), // shifted forward
                      sclLV,                       // its logical volume
                      secondary_coll,              // its name
                      airLV,                       // its mother volume
                      false,                       // no boolean operations
                      0,                           // copy number
                      _checkOverlaps);             // checking overlaps
    sclLV->SetVisAttributes(_redTungsten);
    sclLV->SetUserLimits(_stepLimit);

    return airLV;
}

// Definitions of Solids, Logical Volumes, Physical Volumes
G4VPhysicalVolume* CollimatorConstruction::DefineVolumes()
{
    // World
    double worldXY = 14.0*cm;
    double worldZ  = 60.0*cm;

    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldZ);

    std::cout << "Computed tolerance = "
              << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
              << " mm" << G4endl;
              
    double maxStep = 0.01*mm;
    _stepLimit = new G4UserLimits(maxStep);

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
    auto solidEnv = new G4Box("Envelope", 0.5*0.9*worldXY, 0.5*0.9*worldXY, 0.5*0.9*worldZ);

    auto logicEnv = new G4LogicalVolume(solidEnv,         //its solid
                                        _Air,             //its material
                                       "Envelope");       //its name
    logicEnv->SetUserLimits(_stepLimit);
                                       

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
                      
    // build scoring volume as thin (0.1mm) air-filled disk
    auto scorerTube = new G4Tubs("Scorer", 0.0, 30.0*mm, 0.02*mm, 0.0*deg, 360.0*deg);
    auto scorerVol  = new G4LogicalVolume(scorerTube,     //its solid
                                          _Air,           //its material
                                         "Scorer");       //its name
                                         
    new G4PVPlacement(nullptr,                             // no rotation
		              G4ThreeVector(0.0, 0.0, (_enc_halfz - _src_shiftz) + _air_gap + _coll_halfz + _coll_halfz + 10.0*mm), // secondary collimator center plus collimator half_z plus 10mm
		              scorerVol,       // its logical volume
                      "Scorer",        // its name
                      logicEnv,        // its mother volume
                      false,           // no boolean operations
                      0,               // copy number
                      _checkOverlaps); // checking overlaps    
                    
    _scoringVolume = scorerVol;
    _scoringVolume->SetVisAttributes(_blueCobalt);
    
    // Always return the physical world
    return worldPV;
}

void CollimatorConstruction::set_maxStep(double maxStep)
{
    if (_stepLimit != nullptr && maxStep > 0.0)
    {
        _stepLimit->SetMaxAllowedStep(maxStep);
    }
}

