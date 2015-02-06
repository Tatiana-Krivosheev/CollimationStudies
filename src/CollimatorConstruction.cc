// -*- C++ -*-

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

ColimatorConstruction::CollimatorConstruction():
    G4VUserDetectorConstruction(),
    fNbOfChambers(0),
    fLogicTarget(NULL), fLogicChamber(NULL), 
    fTargetMaterial(NULL),
    fChamberMaterial(NULL),
    fStepLimit(NULL),
    fCheckOverlaps(true)
{
    _messenger = new CollimatorMessenger(this);

    fNbOfChambers = 5;
    fLogicChamber = new G4LogicalVolume*[fNbOfChambers];
}

 
ColimatorConstruction::~ColimatorConstruction()
{
    delete [] fLogicChamber; 
    delete fStepLimit;
    delete fMessenger;
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
    nistManager->FindOrBuildMaterial("G4_AIR");
  
    // Lead defined using NIST Manager
    fTargetMaterial = nistManager->FindOrBuildMaterial("G4_Pb");

    // Xenon gas defined using NIST Manager
    fChamberMaterial = nistManager->FindOrBuildMaterial("G4_Xe");

    // Print materials
    xG4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* CollimatorConstruction::DefineVolumes()
{
    G4Material* air  = G4Material::GetMaterial("G4_AIR");

    // Sizes of the principal geometrical components (solids)
  
  G4double chamberSpacing = 80*cm; // from chamber center to center!

  G4double chamberWidth = 20.0*cm; // width of the chambers
  G4double targetLength =  5.0*cm; // full length of Target
  
  G4double trackerLength = (fNbOfChambers+1)*chamberSpacing;

  G4double worldLength = 1.2 * (2*targetLength + trackerLength);

  G4double targetRadius  = 0.5*targetLength;   // Radius of Target
  targetLength = 0.5*targetLength;             // Half length of the Target  
  G4double trackerSize   = 0.5*trackerLength;  // Half length of the Tracker

  // Definitions of Solids, Logical Volumes, Physical Volumes

  // World

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  G4Box* worldS
    = new G4Box("world",                                    //its name
                worldLength/2,worldLength/2,worldLength/2); //its size
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,   //its solid
                 air,      //its material
                 "World"); //its name
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps 

  // Target
  
  G4ThreeVector positionTarget = G4ThreeVector(0,0,-(targetLength+trackerSize));

  G4Tubs* targetS
    = new G4Tubs("target",0.,targetRadius,targetLength,0.*deg,360.*deg);
  fLogicTarget
    = new G4LogicalVolume(targetS, fTargetMaterial,"Target",0,0,0);
  new G4PVPlacement(0,               // no rotation
                    positionTarget,  // at (x,y,z)
                    fLogicTarget,    // its logical volume
                    "Target",        // its name
                    worldLV,         // its mother volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps 

  G4cout << "Target is " << 2*targetLength/cm << " cm of "
         << fTargetMaterial->GetName() << G4endl;

  // Tracker
 
  G4ThreeVector positionTracker = G4ThreeVector(0,0,0);

  G4Tubs* trackerS
    = new G4Tubs("tracker",0,trackerSize,trackerSize, 0.*deg, 360.*deg);
  G4LogicalVolume* trackerLV
    = new G4LogicalVolume(trackerS, air, "Tracker",0,0,0);  
  new G4PVPlacement(0,               // no rotation
                    positionTracker, // at (x,y,z)
                    trackerLV,       // its logical volume
                    "Tracker",       // its name
                    worldLV,         // its mother  volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps 

  // Visualization attributes

  G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));

  worldLV      ->SetVisAttributes(boxVisAtt);
  fLogicTarget ->SetVisAttributes(boxVisAtt);
  trackerLV    ->SetVisAttributes(boxVisAtt);

  // Tracker segments

  G4cout << "There are " << fNbOfChambers << " chambers in the tracker region. "
         << G4endl
         << "The chambers are " << chamberWidth/cm << " cm of "
         << fChamberMaterial->GetName() << G4endl
         << "The distance between chamber is " << chamberSpacing/cm << " cm" 
         << G4endl;
  
  G4double firstPosition = -trackerSize + chamberSpacing;
  G4double firstLength   = trackerLength/10;
  G4double lastLength    = trackerLength;

  G4double halfWidth = 0.5*chamberWidth;
  G4double rmaxFirst = 0.5 * firstLength;

  G4double rmaxIncr = 0.0;
  if( fNbOfChambers > 0 ){
    rmaxIncr =  0.5 * (lastLength-firstLength)/(fNbOfChambers-1);
    if (chamberSpacing  < chamberWidth) {
       G4Exception("B2aDetectorConstruction::DefineVolumes()",
                   "InvalidSetup", FatalException,
                   "Width>Spacing");
    }
  }

  for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {

      G4double Zposition = firstPosition + copyNo * chamberSpacing;
      G4double rmax =  rmaxFirst + copyNo * rmaxIncr;

      G4Tubs* chamberS
        = new G4Tubs("Chamber_solid", 0, rmax, halfWidth, 0.*deg, 360.*deg);

      fLogicChamber[copyNo] =
              new G4LogicalVolume(chamberS,fChamberMaterial,"Chamber_LV",0,0,0);

      fLogicChamber[copyNo]->SetVisAttributes(chamberVisAtt);

      new G4PVPlacement(0,                            // no rotation
                        G4ThreeVector(0,0,Zposition), // at (x,y,z)
                        fLogicChamber[copyNo],        // its logical volume
                        "Chamber_PV",                 // its name
                        trackerLV,                    // its mother  volume
                        false,                        // no boolean operations
                        copyNo,                       // copy number
                        fCheckOverlaps);              // checking overlaps 

  }

  // Example of User Limits
  //
  // Below is an example of how to set tracking constraints in a given
  // logical volume
  //
  // Sets a max step length in the tracker region, with G4StepLimiter

  G4double maxStep = 0.5*chamberWidth;
  fStepLimit = new G4UserLimits(maxStep);
  trackerLV->SetUserLimits(fStepLimit);
 
  /// Set additional contraints on the track, with G4UserSpecialCuts
  ///
  /// G4double maxLength = 2*trackerLength, maxTime = 0.1*ns, minEkin = 10*MeV;
  /// trackerLV->SetUserLimits(new G4UserLimits(maxStep,
  ///                                           maxLength,
  ///                                           maxTime,
  ///                                           minEkin));

  // Always return the physical world

  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void B2aDetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  G4String trackerChamberSDname = "B2/TrackerChamberSD";
  B2TrackerSD* aTrackerSD = new B2TrackerSD(trackerChamberSDname,
                                            "TrackerHitsCollection");
  // Setting aTrackerSD to all logical volumes with the same name 
  // of "Chamber_LV".
  SetSensitiveDetector("Chamber_LV", aTrackerSD, true);

  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void B2aDetectorConstruction::SetTargetMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial = 
              nistManager->FindOrBuildMaterial(materialName);

  if (fTargetMaterial != pttoMaterial) {
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
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CollimatorConstruction::SetChamberMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial =  nistManager->FindOrBuildMaterial(materialName);

  if (fChamberMaterial != pttoMaterial)
    {
     if ( pttoMaterial ) {
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
    if ((fStepLimit)&&(maxStep>0.))
        fStepLimit->SetMaxAllowedStep(maxStep);
}

void CollimatorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
    fCheckOverlaps = checkOverlaps;
}  
