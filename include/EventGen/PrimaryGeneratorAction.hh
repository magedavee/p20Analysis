// Unrestricted Use - Property of AECL
// 
// PrimaryGeneratorAction.hh
// GEANT4 - geant4.9.3.p01
//
// Header File for Source and Initial Kinematics Specifications
//	Contains class functions/variables
//
// --------------------------------------------------------
//	Version 1.01 - 2011/04/29 - A. Ho
//  Edited for clarity 2014/07 M. P. Mendenhall
// --------------------------------------------------------

#ifndef PrimaryGeneratorAction_H
// Assure header is only loaded once during compilation
#define PrimaryGeneratorAction_H

// Specifies base class or parent class
#include "G4VUserPrimaryGeneratorAction.hh"

// Specifies the classes which contain structures called upon in this class
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"

// Specifies classes defining all global parameters and variable types
#include <vector>
#include <map>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "globals.hh"
#include "Randomize.hh"

class G4Event;
class G4ParticleGun;
class G4ParticleTable;
class G4GeneralParticleSource;
class PrimaryGeneratorMessenger;
class DetectorConstruction;

class CRYModule;
class IBDModule;
class FissionAntiNuModule;
class CosmicMuonModule;
class CosmicNeutronModule;

/// Specification for a primary particle to throw
struct primaryPtcl {
    int PDGid;  ///< PDG particle ID enumeration
    double KE;  ///< particle kinetic energy
    G4ThreeVector pos;  ///< vertex position
    G4ThreeVector mom;  ///< momentum direction
    double t;           ///< particle time
};

class PrimaryGeneratorAction;

/// Base class interface for alternate generator modules
class PrimaryGeneratorModule {
public:
    /// Constructor
    PrimaryGeneratorModule(PrimaryGeneratorAction* P): myPGA(P) { }
    /// Destructor
    virtual ~PrimaryGeneratorModule() { }
    
    /// throw event particles
    virtual void GeneratePrimaries(G4Event* anEvent) = 0;
    
protected:
    
    /// convenience function for throwing listed primaries
    void throwPrimaries(const std::vector<primaryPtcl>& v, G4Event* anEvent);
    
    PrimaryGeneratorAction* myPGA;      ///< PrimaryGeneratorAction this module runs for
};


/// Class for generating primary particles for event
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
    
friend class PrimaryGeneratorMessenger;

public:
    /// constructor
    PrimaryGeneratorAction();
    /// destructor
    virtual ~PrimaryGeneratorAction();

    /// Generates a particle and launches it into the geometry
    void GeneratePrimaries(G4Event* anEvent);

    G4ParticleGun* GetParticleGun() const { return particle_gun; };
    DetectorConstruction* GetDetector() const { return detect; }
    
    G4String GetDistribution() const { return fDistribution; };
    std::map<G4int,G4int>* GetHistogramData() const { return Histogram; };
    std::vector<G4double>* GetEnergyTable() const { return Energy; };
    G4int GetFunction() const { return fFunction; };
    G4int GetCalibrationSource() const { return fCalibration; };
    G4bool GetGunSettings() const { return fGun; };
    G4int GetVerbosity() const { return verbose; }

    void DisplayKinematicsInfo() const;
    
    /// load CRY as current generator
    void loadCRYModule();              
    /// load Inverse Beta Decay as current generator
    void loadIBDModule();
    /// load fission anti-neutrinos as current generator
    void loadFisAntNuModule();
    /// load cosmic muons as current generator
    void loadCosmicMuonModule();
    /// load cosmic neutrons as current generator
    void loadCosmicNeutronModule();
    
protected:
    
    void SetVerbosity(G4int);
    
    PrimaryGeneratorModule* genModule;          ///< generator module currently in use
    CRYModule* myCRYModule;                     ///< CRY generator module
    IBDModule* myIBDModule;                     ///< Inverse Beta Decay generator
    FissionAntiNuModule* myFisAntNuModule;      ///< Fission anti-neutrinos generator
    CosmicMuonModule* myCosmicMuonModule;       ///< Cosmic muons generator
    CosmicNeutronModule* myCosmicNeutronModule; ///< Cosmic neutrons generator
    
    void GenerateUserParticleKinematics(G4int);
    void GenerateFunctionParticleEnergy();
    void GenerateCustomParticleEnergy();
    void GenerateCalibratedSourceEnergy();

    void ToggleCalibrationMode(G4bool);
    void SetCalibrationSource(G4int);
    void SetCalibrationPosition(G4ThreeVector);
    void ResetAllSourceIntensities();

    void SetFunctionEnergySpectrum(G4int);
    void SetMinimumEnergyRange(G4double);
    void SetMaximumEnergyRange(G4double);
    void SetCustomEnergySpectrum(G4String);
    void InactivateUserEnergySpectrum(G4bool);
    G4bool Cf252Function(G4double,G4double);
    G4bool MaxwellFunction(G4double,G4double);
    void SetMaxwellianTemperature(G4double);
    G4bool NumericSpectrumGenerator(G4double,G4double);

    void SetGenerationWithGun(G4bool f) { fGun = f; };
    void SetCosmicNeutronFlag(G4bool);

private:

    G4int verbose;      ///< Verbosity (0 = silent, 1 = minimal, 2 = loud)

    G4ParticleGun* particle_gun;
    G4GeneralParticleSource* particle_source;
    DetectorConstruction* detect;
    PrimaryGeneratorMessenger* gun_messenger;

    G4String fDistribution;             ///< Flag to determine current distribution
    G4int fFunction;                    ///< Flag to determine current spectrum function, if any
    G4bool fCustom;                     ///< Flag to specify use of custom spectrum
    G4String customDist;
    G4bool fGun;                        ///< Flag to specify use of Particle Gun instead of default GPS
    G4int fCalibration;                 ///< Flag to determine which calibration source, if any
    G4String fDistStore;
    G4int fCalFunction;                 ///< Flag to determine which calibration function, if any

    std::vector<G4double>* Energy;
    std::vector<G4double>* Dist;
    std::map<G4int,G4int>* Histogram;

    G4int numSources, calSources;
    G4double min_energy;
    G4double max_energy;
    G4double max_dist;
    G4double eBin_width;
    G4double mTemp;                     ///< Temperature for Maxwellian spectrum
    std::ifstream* textfile;
};

#endif