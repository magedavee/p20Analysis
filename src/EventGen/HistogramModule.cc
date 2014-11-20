#include "HistogramModule.hh"
#include <TH1.h>
#include <TFile.h>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>

HistogramModule::HistogramModule(PrimaryGeneratorAction* P):
PrimaryGeneratorModule(P, "Histogram"), myDist(NULL),
hist_dir("/generator/histogram/"),
file_cmd("/generator/histogram/file",this),
hname_cmd("/generator/histogram/hname",this),
ptcl_cmd("/generator/histogram/ptcl",this) {
    
    file_cmd.SetGuidance("Set .root filename for histogram.");
    file_cmd.AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);
    
    hname_cmd.SetGuidance("Set name of histogram to load.");
    hname_cmd.AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);
    
    ptcl_cmd.SetGuidance("Set particle PDG PID to throw.");
    ptcl_cmd.AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);
}

void HistogramModule::resetDistribution() {
    if(myDist) delete myDist;
    myDist = NULL;
    netRate = 0;
}

void HistogramModule::SetNewValue(G4UIcommand* command, G4String newValue) {
    if(command == &file_cmd) { fname = newValue; resetDistribution(); }
    else if(command == &hname_cmd) { hname = newValue; resetDistribution(); }
    else if(command == &ptcl_cmd) { ptcl = ptcl_cmd.GetNewIntValue(newValue); }
}

void HistogramModule::makeDistribution() {
    resetDistribution();
    TFile f(fname.c_str(),"READ");
    if(f.IsZombie()) return;
    myDist = (TH1*)f.Get(hname.c_str());
    if(myDist) {
        myDist->SetDirectory(NULL);
        netRate = myDist->Integral() / s;
    } else G4cout << "Histogram '" << hname << "' not found in file '" << fname << "'!\n";
}

void HistogramModule::GeneratePrimaries(G4Event* anEvent) {
    if(!myDist) makeDistribution();
    if(!myDist) return;
        
    primaryPtcl p;
    p.PDGid = ptcl;
    p.KE = myDist->GetRandom() * MeV;
    p.t = 0;
    
    vector<primaryPtcl> v;
    v.push_back(p);
    setVertices(v);
    throwPrimaries(v,anEvent);
}

G4double HistogramModule::GetGeneratorTime() const {
    return myPGA->GetPositioner()->getAttempts() / netRate;
}

void HistogramModule::fillNode(TXMLEngine& E) {
    addAttr(E, "rate", G4BestUnit(netRate,"Frequency"));
    addAttr(E, "file", fname);
    addAttr(E, "histogram", hname);
    addAttr(E, "particle", ptcl);
}
