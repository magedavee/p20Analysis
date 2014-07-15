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
// --------------------------------------------------------

#ifndef PrimaryGeneratorAction_H		// Only carries out if object is undefined
#define PrimaryGeneratorAction_H 1		// Defines object

#include "G4VUserPrimaryGeneratorAction.hh"	// Specifies base class or parent class

#include "G4ThreeVector.hh"			// Specifies the classes which contain structures called upon in this class
#include "G4DataVector.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"

#include <vector>				// Specifies classes defining all global parameters and variable types
#include <map>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "globals.hh"
#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "CRYUtils.h"
#include "RNGWrapper.hh"
#include "Randomize.hh"
using namespace std;

class G4Event;					// Structures necessary for class definition
class G4ParticleGun;
class G4ParticleTable;
class G4GeneralParticleSource;
class PrimaryGeneratorMessenger;
class DetectorConstruction;
class CosmicNeutronGenerator;
class InverseBetaKinematics;
class FissionAntiNuGenerator;
class CosmicMuonGenerator;

/* -------- Class Definition --------- */

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction	// Class inherits functions from G4VUserPrimaryGeneratorAction
{
  friend class PrimaryGeneratorMessenger;	// Friend classes have access to all member data and internal methods of this class

  public:	// Accessible Data

    enum Module { None,
		  InverseBeta,
		  FissionAntinu,
		  CosmicNeutron,
		  CosmicMuon };		// Enumeration to ease transitions of models in coding (could be inefficient but is useful)

  public:	// Constructors and Destructors

    PrimaryGeneratorAction();						// Constructor
    virtual ~PrimaryGeneratorAction();					// Destructor

  public:	// Accessible Methods

    void GeneratePrimaries(G4Event* anEvent);			// Generates a particle and launches it into the geometry
  void InputCRY();
 
    void UpdateCRY(std::string* MessInput);
    void CRYFromFile(G4String newValue);
  
  G4ParticleGun* GetParticleGun() const { return particle_gun; };				// Get functions
  G4GeneralParticleSource* GetParticleSource() const { return particle_source; };
    G4String GetDistribution() const { return fDistribution; };
    std::map<G4int,G4int>* GetHistogramData() const { return Histogram; };
    std::vector<G4double>* GetEnergyTable() const { return Energy; };
    G4int GetFunction() const { return fFunction; };
  G4bool GetFile() const { return fFile; };  
  G4int GetCalibrationSource() const { return fCalibration; };
    G4String GetRunSettings() const { return sModes; };
    G4bool GetGunSettings() const { return fGun; };
    G4int GetModuleSettings() const { return (G4int)(fModule); };
    G4int GetVerbosity() const { return verbose; }

    void DisplayKinematicsInfo() const;

  protected:	// Internal Methods

    void SetVerbosity(G4int);

    void GenerateUserParticleKinematics(G4int);
    void GenerateModuleParticleKinematics(G4int);
    void GenerateFunctionParticleEnergy();
  void GenerateFileParticleEnergy();
  void GenerateCustomParticleEnergy();
    void GenerateCalibratedSourceEnergy();
    void GenerateEnergySpectrumWithoutSimulation(G4int n = 1);

  void SetCRY(G4bool);
 void SetCRYPoint(G4bool);
 void SetCRYZOffset(G4double);

    void ToggleCalibrationMode(G4bool);
    void SetCalibrationSource(G4int);
    void SetCalibrationPosition(G4ThreeVector);
    void ResetAllSourceIntensities();

    void SetFunctionEnergySpectrum(G4int);
  void SetFile(G4String);
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
    void SetInverseBetaFlag(G4bool);
    void SetFissionAntiNuFlag(G4bool);
    void SetCosmicMuonFlag(G4bool);
    void ResetSimulationModule();

    CosmicNeutronGenerator* GetNeutronGenerator() { return neutron_generator; };
    InverseBetaKinematics* GetInverseGenerator() { return inverse_beta; };
    FissionAntiNuGenerator* GetAntiNuGenerator() { return fission_spec; };
    CosmicMuonGenerator* GetMuonGenerator() { return muon_generator; };

  private:	// Member Data

    G4int verbose;			// Verbosity (0 = silent, 1 = minimal, 2 = loud)
    G4bool RawData;			// Outputs generated numbers, set with verbosity > 2

    G4ParticleGun* particle_gun;
  G4ParticleTable* particleTable;
  CRYGenerator* gen;			// Particle Gun
    G4GeneralParticleSource* particle_source;		// GPS
    DetectorConstruction* detect;
    PrimaryGeneratorMessenger* gun_messenger;

    CosmicNeutronGenerator* neutron_generator;
    InverseBetaKinematics* inverse_beta;
    FissionAntiNuGenerator* fission_spec;
    CosmicMuonGenerator* muon_generator;

    G4String fDistribution;		// Flag to determine current distribution
    Module fModule;			// Flag to determine current module, if any
    G4int fFunction;			// Flag to determine current spectrum function, if any
  G4bool fFile;                          // Flag to determine whether particles are generated from an input file
  G4bool fCry;                           // flag to determine whether particles are generated from the CRY package
  G4bool fCRYpoint;                           // flag to determine whether particles are generated from the CRY package selectively point at the detector
  G4bool fCustom;			// Flag to specify use of custom spectrum
    G4String customDist;
    G4bool fGun;			// Flag to specify use of Particle Gun instead of default GPS
    G4int fCalibration;			// Flag to determine which calibration source, if any
    G4String fDistStore;
    G4int fCalFunction;			// Flag to determine which calibration function, if any

    std::vector<G4double>* Energy;
    std::vector<G4double>* Dist;
    std::map<G4int,G4int>* Histogram;
  std::vector<CRYParticle*> *vect; // vector of generated particles

    G4int numSources, calSources;
    G4double cryZoffset;
    G4double min_energy;
    G4double max_energy;
    G4double max_dist;
    G4double eBin_width;
    G4double mTemp;			// Temperature for Maxwellian spectrum
    G4String sModes;			// Used to mark output files
  ifstream* textfile;
  G4int InputState;
};

/* ----------------------------------- */

#endif							// End of if clause

// EOF
