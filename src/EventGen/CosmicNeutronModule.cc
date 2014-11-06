#include <TH1F.h>
#include "CosmicNeutronModule.hh"
#include "DetectorConstruction.hh"

#include <G4Neutron.hh>
#include <G4ParticleGun.hh>
#include <G4ios.hh>
#include <G4UnitsTable.hh>
#include <G4VSolid.hh>
#include <Randomize.hh>

#include <cassert>

G4ThreeVector CosmicFluxThrower::proposeDirection() {
    double phi = 2.*M_PI*G4UniformRand();
    double costheta;
    do { costheta = -G4UniformRand(); }
    while(G4UniformRand() > pow(fabs(costheta),3.));
    double sintheta = sqrt(1.-costheta*costheta);
    return G4ThreeVector(cos(phi)*sintheta, sin(phi)*sintheta, costheta);
}

CosmicNeutronModule::CosmicNeutronModule(PrimaryGeneratorAction* P):
PrimaryGeneratorModule(P, "CosmicNeutron"),
myThrower(myPGA->GetDetector()->theWorld), myDist(NULL) {
    myThrower.setSourceTarget(NULL, myPGA->GetDetector()->ptclTrg);
    
    //setParameters(0.5*GV, 3.47*GeV, 1016*g/cm2);        // Nashville, TN
    setParameters(s_min, 2.00*GeV, 1010.9*g/cm2, 0.2);       // Goldhagen "Watson Roof"
}

void CosmicNeutronModule::GeneratePrimaries(G4Event* anEvent) {    
    primaryPtcl p;
    p.PDGid = 2112;
    if(!myDist) makeDistribution();
    p.KE = myDist->GetRandom();
    p.t = 0;
    
    vector<primaryPtcl> v;
    v.push_back(p);
    myThrower.setVertex(v);
    throwPrimaries(v,anEvent);
}

void CosmicNeutronModule::makeDistribution() {
    if(myDist) delete myDist;
    
    const unsigned int nBins = 1000;
    const double eMin = 1e-3*eV;
    const double eMax = 1e10*eV;
    Double_t binEdges[nBins+1];
    for(unsigned int i=0; i<=nBins; i++)
        binEdges[i] = exp((nBins-i)*log(eMin)/nBins + i*log(eMax)/nBins);
    myDist = new TH1F("hSatoNiita","Neutron energy spectrum",nBins,binEdges);
    
    /*
    for(int i=1; i<=11; i++) G4cout << "a_" << i << " = " << a[i] << "\n";
    for(int i=1; i<=5; i++) { G4cout << "\tb_" << i << "1 = " << b_i1[i]; } G4cout << "\n";
    for(int i=1; i<=5; i++) { G4cout << "\tb_" << i << "2 = " << b_i2[i]; } G4cout << "\n";
    for(int i=1; i<=5; i++) { G4cout << "\tb_" << i << "3 = " << b_i3[i]; } G4cout << "\n";
    for(int i=1; i<=5; i++) { G4cout << "\tb_" << i << "4 = " << b_i4[i]; } G4cout << "\n";
    */
    
    for(unsigned int i=1; i<=nBins; i++) {
        double E = myDist->GetBinCenter(i);
        //double EdPhidE = calcGroundSpectrum(E);
        double EdPhidE = calcAirSpectrum(E);
        //G4cout << g_1 << "\t" << g_2 << "\t" << G4BestUnit(g_3,"Energy") << "\t" << g_4 << "\t"<< G4BestUnit(g_5,"Energy") << "\n";
        assert(EdPhidE >= 0.);
        myDist->SetBinContent(i,EdPhidE*myDist->GetBinWidth(i)/E);
        //G4cout << "E = " << G4BestUnit(E,"Energy") << "\tphi_B = " << phi_B << "\tphi_L = " << phi_L
        //    << "\tphi_T = " << phi_T << "\tf_G = " << f_G << "\tE*dPhi/dE = " << EdEdPhi << G4endl;
    }
    netFlux = myDist->Integral();
    area = myThrower.getSourceArea();
}


G4double CosmicNeutronModule::GetGeneratorTime() const {
    return 1.0; //double(nAttempts) / area / netFlux;
}

void CosmicNeutronModule::fillNode(TXMLEngine& E) {
    addAttr(E, "flux", to_str(netFlux*(1000*s*cm2))+" mHz/cm2");
    addAttr(E, "area", G4BestUnit(area,"Surface"));
    //addAttr(E, "a_frac", double(nSurfaceThrows)/double(nAttempts));
    //addAttr(E, "t_frac", double(nHits)/double(nSurfaceThrows));
    addAttr(E, "s_mod", G4BestUnit(s_mod,"Electric potential"));
    addAttr(E, "depth", G4BestUnit(depth,"Mass/Surface"));
    addAttr(E, "r_c", G4BestUnit(r_c,"Energy"));
    addAttr(E, "w", waterFrac);
}
