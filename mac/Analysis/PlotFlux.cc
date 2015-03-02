/////////////////////////////////////////////////////////////
// Standalone program for plotting flux counter distributions
// ./PlotFlux $PG4_OUTDIR/<directory>

#include "AnaUtils.hh"
#include "GoldhagenSpectrum.hh"

/// Collection of histograms to generate for a particle type
class FluxHistograms {
public:
    /// Constructor
    FluxHistograms(FileKeeper& F, int pid): PID(pid) {
        string pn = to_str(PID);
        
        p_dir = (TH2F*)F.add(new TH2F(("prim_p_"+pn).c_str(), "Momentum direction", 100,-1.2,1.2, 100,-1.2,1.2));
        
        E_hi = (TH1F*)F.add(new TH1F(("E_hi_"+pn).c_str(),"Energy spectrum", 500, 0, 5));
        E_hi->GetXaxis()->SetTitle("kinetic energy [GeV]");
        E_hi->GetYaxis()->SetTitle("Events per GeV");
        E_hi->GetYaxis()->SetTitleOffset(1.45);
        
        if(PID==2112) {
            hGoldhagen = loadGoldhagen();
            
            hNeutronE = (TH1*)hGoldhagen->Clone("hNeutronE");
            hNeutronE->SetLineColor(2);
            hNeutronE->Scale(0.);
            
            hNeutronIn = (TH1*)hNeutronE->Clone("hNeutronIn");
            hNeutronIn->SetLineColor(3);
            
        } else hGoldhagen = NULL;
    }
    
    /// Fill from particle flux data
    void Fill(ParticleVertex* pp) {
        p_dir->Fill(pp->p[0], pp->p[2]);
        E_hi->Fill(pp->E/1000.);
        if(hGoldhagen) {
            hNeutronE->Fill(pp->E);
            if(pp->p[2]<0) hNeutronIn->Fill(pp->E);
        }
    }
    
    /// Process and draw
    void Draw(const string& outpath, double t) {
        
        string pn = to_str(PID);
        
        gStyle->SetOptStat("");
        
        if(hGoldhagen) {
            scale_times_bin(hGoldhagen);
            hGoldhagen->GetYaxis()->SetTitle("E #times d#Phi/dE, [1/cm^{2}/s]");
            hGoldhagen->SetMaximum(1e-2);
            hGoldhagen->SetMinimum(1e-5);
            hGoldhagen->SetTitle("Surface neutron flux");
            hGoldhagen->Draw();
            
            double fluxscale = 1./(1000.*1000.*t);
            
            hNeutronE->Scale(fluxscale);
            scale_times_bin(hNeutronE);
            normalize_to_bin_width(hNeutronE);
            hNeutronE->Draw("Same");
            
            hNeutronIn->Scale(fluxscale);
            scale_times_bin(hNeutronIn);
            normalize_to_bin_width(hNeutronIn);
            hNeutronIn->Draw("Same");
            
            gPad->SetLogy(true);
            gPad->SetLogx(true);
            
            TLegend* leg = new TLegend(0.13,0.7, 0.48,0.895);
            leg->SetFillColor(0);       // white background
            leg->SetBorderSize(0);      // get rid of the box
            leg->SetTextSize(0.045);    // set text size
            leg->AddEntry(hGoldhagen,"Goldhagen (Watson roof)","l");
            //leg->AddEntry("hNeutronE","CRY + Geant4 backscatter","l");
            //leg->AddEntry("hNeutronIn","CRY incident","l");
            leg->AddEntry("hNeutronE","Sato-Niita + Geant4 backscatter","l");
            leg->AddEntry("hNeutronIn","Sato-Niita air","l");
            leg->Draw();
            
            gPad->Print((outpath+"/E_Goldhagen.pdf").c_str());
            
            gPad->SetLogy(false);
            gPad->SetLogx(false);
        }
        
        E_hi->Draw();
        gPad->SetLogy(true);
        gPad->Print((outpath+"/E_hi_"+pn+".pdf").c_str());
        
        gPad->SetLogy(false);
        gPad->SetCanvasSize(700,700);
        
        gPad->SetLogz(true);
        p_dir->Draw("Col Z");
        gPad->Print((outpath+"/p_Direction_"+pn+".pdf").c_str());
        gPad->SetLogz(false);

    }
    
    int PID;            ///< PDG particle ID
    TH2F* p_dir;        ///< momentum direction
    TH1F* E_hi;         ///< Energy, high-energy range
    TH1* hGoldhagen;    ///< Goldhagen binned neutron data
    TH1* hNeutronE;     ///< Neutron energy in Goldhagen binning
    TH1* hNeutronIn;    ///< Incident neutron energy in Goldhagen binning
};

int main(int argc, char** argv) {
    // load library describing data classes
    gSystem->Load("libEventLib.so");
    
    string inPath = ".";
    if(argc == 2) inPath = argv[1];
    string outpath = inPath + "/Plots/";
    
    mkdir(outpath.c_str(), 0755);
    FileKeeper f(outpath+"FluxOut.root");
    
    // load data into TChain
    OutDirLoader D(inPath);
    TChain* T = D.makeTChain();
    // set readout branches
    ParticleEvent* evt = new ParticleEvent();
    T->SetBranchAddress("Flux",&evt);
    
    map<Int_t, FluxHistograms> primHists;
    
    // scan events
    Long64_t nentries = T->GetEntries();
    std::cout << "Scanning " << nentries << " events...\n";
    for (Long64_t ev=0; ev<nentries; ev++) {
        if(!(ev % (nentries/20))) { cout << "*"; cout.flush(); }
        T->GetEntry(ev);
        
        Int_t nflux = evt->particles->GetEntriesFast();
        for(Int_t i=0; i<nflux; i++) {
            ParticleVertex* pp = (ParticleVertex*)evt->particles->At(i);
            
            Int_t PID = pp->PID;
            //if(PID != 2112) continue;
            auto it = primHists.find(PID);
            if(it == primHists.end())
                it = primHists.insert(pair<Int_t, FluxHistograms>(PID,FluxHistograms(f,PID))).first;
            it->second.Fill(pp);
        }
    }
    
    for(auto it = primHists.begin(); it !=primHists.end(); it++)
        it->second.Draw(outpath, D.genTime);
        
    return 0;
}