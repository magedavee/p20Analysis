#include "GeomHelper.hh"
#include "SMExcept.hh"

#include <G4LogicalVolume.hh>
#include <G4VSolid.hh>
#include <G4ios.hh>
#include <G4UnitsTable.hh>

#include <cassert>

void GeomHelper::setParentChild(G4VPhysicalVolume* p, G4VPhysicalVolume* c) {
    P=p;
    C=c;
    if(!P) throw SMExcept("undefinedParentVolume");
    if(!C) throw SMExcept("undefinedChildVolume");
    calcChildOffset();
}

void GeomHelper::calcChildOffset() {
    rotCtoP = rotPtoC = G4RotationMatrix();
    transCtoP = G4ThreeVector();
    
    if(!P || !C) return;
    G4cout << "Calculating transform from " << P->GetName() << " to " << C->GetName() << G4endl;
    
    while(P != C) {
        G4LogicalVolume* currentLogical = P->GetLogicalVolume();
        G4int nC = currentLogical->GetNoDaughters();
        P = C;
        for(G4int i = 0; i < nC; i++) {
            P = currentLogical->GetDaughter(i);
            if(P->GetLogicalVolume()->IsAncestor(C) || P == C) break;
        }
        transCtoP += P->GetObjectTranslation();
        rotCtoP *= P->GetObjectRotationValue();
    }
    
    rotPtoC = rotCtoP.inverse();
    G4cout << rotCtoP << G4BestUnit(transCtoP,"Length") << G4endl;
}

G4ThreeVector GeomHelper::coordCtoP(G4ThreeVector x) const {
    x *= rotCtoP;
    x += transCtoP;
    return x;
}

G4ThreeVector GeomHelper::coordPtoC(G4ThreeVector x) const {
    x -= transCtoP;
    x *= rotPtoC;
    return x;
}

bool GeomHelper::intersects(G4ThreeVector x, G4ThreeVector d) {
    if(!C) return false;
    x = coordPtoC(x);
    d *= rotPtoC;
    return C->GetLogicalVolume()->GetSolid()->DistanceToIn(x, d) != kInfinity;
}

bool GeomHelper::intersectionDistances(G4ThreeVector x, G4ThreeVector d, double& dIn, double& dOut) {
    if(!C) { dIn = dOut = kInfinity; return false; }
    x = coordPtoC(x);
    d *= rotPtoC;
    dIn = C->GetLogicalVolume()->GetSolid()->DistanceToIn(x, d);
    if(dIn == kInfinity) { dOut = dIn; return false; }
    dOut = C->GetLogicalVolume()->GetSolid()->DistanceToIn(x+d*dIn, d);
    return true;
}
