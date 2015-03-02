#include "SphereShellBuilder.hh"

#include "ScintCellBuilder.hh"
#include "MaterialsHelper.hh"
#include "FluxCounterSD.hh"
#include "strutils.hh"

#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <G4Orb.hh>
#include <G4Sphere.hh>
#include <G4SDManager.hh>

SphereShellBuilder::SphereShellBuilder(): ScintSegVol("SphereShells"),
myMat(NULL), radius(100*cm), ndivs(100),
sphereDir("/geom/sphere/"),
radiusCmd("/geom/sphere/radius",this),
segCmd("/geom/sphere/nshells",this),
matCmd("/geom/sphere/material",this) {
    
    radiusCmd.SetGuidance("Set sphere radius.");
    radiusCmd.AvailableForStates(G4State_PreInit);
    
    segCmd.SetGuidance("Set number of spherical shells.");
    segCmd.AvailableForStates(G4State_PreInit);
    
    matCmd.SetGuidance("Set sphere material.");
    matCmd.AvailableForStates(G4State_PreInit);
    
    // special materials for study
    MaterialsHelper::M().get6LiLS(MaterialsHelper::M().Polyeth, 0.001, true);
}

void SphereShellBuilder::SetNewValue(G4UIcommand* command, G4String newValue) {
    if(command == &radiusCmd) radius = radiusCmd.GetNewDoubleValue(newValue);
    else if(command == &segCmd) ndivs = segCmd.GetNewIntValue(newValue);
    else if(command == &matCmd) myMat = G4Material::GetMaterial(newValue);
    else G4cout << "Unknown command!" << G4endl;
}

void SphereShellBuilder::construct() {
    
    if(!myMat) myMat = MaterialsHelper::M().get6LiLS(MaterialsHelper::M().EJ309, 0.001, true);
    
    dim = G4ThreeVector(2*radius, 2*radius, 2*radius);
    
    G4Orb* full_sphere = new G4Orb("full_sphere", radius);
    scint_log = main_log = new G4LogicalVolume(full_sphere, myMat, "SphereShells_main_log");
    
    // assign flux counter to outermost sphere
    FluxCounterSD* mySD = new FluxCounterSD("FluxSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(mySD);
    main_log->SetSensitiveDetector(mySD);
    
    if(ndivs<2) return;
    
    for(unsigned int i=0; i<ndivs; i++) {
        G4Sphere* shell_sphere = new G4Sphere("shell_sphere_"+to_str(i), i*radius/ndivs, (i+1)*radius/ndivs, 0, 2*M_PI, 0, M_PI);
        shells.push_back(new G4LogicalVolume(shell_sphere, myMat, "SphereShells_shell_log_"+to_str(i)));
        new G4PVPlacement(NULL, G4ThreeVector(), shells.back(), "SphereShells_shell_phys_"+to_str(i), main_log, false, 0, true);
    }
}

void SphereShellBuilder::setScintSD(G4VSensitiveDetector* SD) {
    for(auto it = shells.begin(); it != shells.end(); it++)
        (*it)->SetSensitiveDetector(SD);
}

void SphereShellBuilder::fillNode(TXMLEngine& E) {
    ScintSegVol::fillNode(E);
    addAttr(E, "r", G4BestUnit(radius,"Length"));
    addAttr(E, "n", ndivs);
}

int SphereShellBuilder::getSegmentNum(const G4ThreeVector& x) const {
    double r = x.mag();
    return int(r/(radius/ndivs));
}
