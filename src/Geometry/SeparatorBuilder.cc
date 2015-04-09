#include "SeparatorBuilder.hh"
#include "MaterialsHelper.hh"

#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <G4Box.hh>
#include <cassert>

SeparatorBuilder::SeparatorBuilder(const string& uiname): Builder("Separator"),
width(0), length(0), totalThick(1.5*mm), cfThick(0.5*mm), sep_vis(G4Colour(0.9,0.9,0.9)),
ui_dir(("/geom/"+uiname+"/").c_str()),
thick_cmd(("/geom/"+uiname+"/thick").c_str(),this) {
    addChild(&myOptSurf);
    myOptSurf.refl = 0.96;
    
    ui_dir.SetGuidance("Separator panel settings");
    ui_dir.AvailableForStates(G4State_PreInit);
    
    thick_cmd.SetGuidance("Set the total thickness of the separator panels");
    thick_cmd.SetDefaultValue(totalThick);
    thick_cmd.AvailableForStates(G4State_PreInit);
}

void SeparatorBuilder::SetNewValue(G4UIcommand* command, G4String newValue) {
    if(command == &thick_cmd)  {
        double newThick = thick_cmd.GetNewDoubleValue(newValue);
        cfThick *= newThick/totalThick;
        totalThick = newThick;
    }
}

void SeparatorBuilder::construct() {
    myOptSurf.construct();
    
    dim = G4ThreeVector(totalThick, width, length);
    if(totalThick <= 0) return;
    assert(cfThick <= totalThick);
    
    G4Box* main_box = new G4Box("SeparatorMainBox", dim[0]/2., dim[1]/2., dim[2]/2.);    
    main_log = new G4LogicalVolume(main_box, MaterialsHelper::M().PMMA, "Separator_main_Log");
    main_log->SetVisAttributes(&sep_vis);
}
