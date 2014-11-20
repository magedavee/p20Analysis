#include "SurfaceThrower.hh"
#include "SMExcept.hh"

#include <G4LogicalVolume.hh>
#include <G4VSolid.hh>
#include <Randomize.hh>
#include <G4UnitsTable.hh>

SurfaceThrower::SurfaceThrower(G4VPhysicalVolume* w, G4VPhysicalVolume* SS, G4VPhysicalVolume* TT):
VertexPositioner("SurfaceThrower"), outer(true), fromVolume(false), W(w) {
    if(!W) throw SMExcept("undefinedWorldVolume");
    setSourceTarget(SS,TT);
}

void SurfaceThrower::setSourceTarget(G4VPhysicalVolume* SS, G4VPhysicalVolume* TT) {
    S = SS;
    if(!S) { S = W; outer = fromVolume = false; }
    W2S.setParentChild(W,S);
    sourceExtent = S->GetLogicalVolume()->GetSolid()->GetExtent();
    
    T = TT;
    if(T) W2T.setParentChild(W,T);
}

G4ThreeVector pointInSolid(const G4VisExtent& E, const G4VSolid* S) {
    assert(S);
    G4ThreeVector v;
    do {
        double x = G4UniformRand(); x = E.GetXmin()*(1-x) + E.GetXmax()*x;
        double y = G4UniformRand(); y = E.GetYmin()*(1-y) + E.GetYmax()*y;
        double z = G4UniformRand(); z = E.GetZmin()*(1-z) + E.GetZmax()*z;
        v = G4ThreeVector(x,y,z);
    } while (S->Inside(v) != kInside);
    return v;
}

void SurfaceThrower::proposePosition() {
    if(fromVolume) pos = pointInSolid(sourceExtent, S->GetLogicalVolume()->GetSolid());
    else  {
        // Note: this won't be right if solid doesn't return uniformly distributed points!
        pos = S->GetLogicalVolume()->GetSolid()->GetPointOnSurface();
        snorm = S->GetLogicalVolume()->GetSolid()->SurfaceNormal(pos);
    }
    pos = W2S.coordCtoP(pos);
}

bool SurfaceThrower::tryMomentum() {
    mom = proposeDirection();
    bool passNormal = true;
    if(!fromVolume) {
        double x = snorm.dot(mom);
        passNormal = ((outer && x>0) || (!outer && x<0)) && G4UniformRand() < fabs(x);
    }
    nSurfaceThrows += passNormal;
    return passNormal && (!T || W2T.intersects(pos,mom));
}

void SurfaceThrower::genThrow() {
    assert(S);
    if(!S) return;
    
    do { proposePosition(); nAttempts++; }
    while(!tryMomentum());

    nHits++;
}

bool SurfaceThrower::tryVertex(vector<primaryPtcl>& v) {
    if(!v.size()) return false;
    
    vector<primaryPtcl> thrown;
    nAttempts++;
    proposePosition();
    for(auto it = v.begin(); it != v.end(); it++) {
        if(tryMomentum()) {
            it->pos = pos;
            it->mom = mom;
            thrown.push_back(*it);
        }
    }
    
    nHits += thrown.size();
    if(thrown.size()) v = thrown;
    else return false;
    
    return true;
}

void SurfaceThrower::fillNode(TXMLEngine& E) {
    VertexPositioner::fillNode(E);
    addAttr(E, "s_area", G4BestUnit(getOriginArea(),"Surface"));
    addAttr(E, "s_volume", G4BestUnit(getOriginVolume(),"Volume"));
    addAttr(E, "nAttempts", nAttempts);
    addAttr(E, "nSurfaceThrows", nSurfaceThrows);
    addAttr(E, "nHits", nHits);    
}
