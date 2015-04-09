/// \file NuclEvtGen.hh \brief Configurable nuclear decay chain event generator.
#ifndef NUCLEVTGEN_HH
#define NUCLEVTGEN_HH

#include "ElectronBindingEnergy.hh"
#include "BetaSpectrum.hh"
#include "FloatErr.hh"
#include <TF1.h>
#include <TArrayD.h>
#include <vector>
#include <map>
#include <set>
#include <climits>
#include <float.h>
#include <stdio.h>

using std::set;

/// random event selector
class PSelector {
public:
    /// constructor
    PSelector() { cumprob.push_back(0); }
    /// add a probability
    void addProb(double p) { cumprob.push_back(p+cumprob.back()); }
    /// select partition for given input (random if not specified); re-scale input to partition range to pass along to sub-selections
    unsigned int select(double* x = NULL) const;
    /// get cumulative probability
    double getCumProb() const { return cumprob.back(); }
    /// get number of items
    unsigned int getN() const { return cumprob.size()-1; }
    /// get probability of numbered item
    double getProb(unsigned int n) const;
    /// scale all probabilities
    void scale(double s);
    
protected:
    vector<double> cumprob;        ///< cumulative probabilites
};

/// Quantiles (inverse CDF) distribution from a TF1
/// based on ROOT's TF1::GetQuantiles(...) function
class TF1_Quantiles {
public:
    /// constructor
    TF1_Quantiles(TF1& f);
    /// return quantile for 0 <= p <= 1
    double eval(double p) const;
    
protected:
    
    const unsigned int npx;
    const Double_t xMin;
    const Double_t xMax;
    const Double_t dx;
    TArrayD integral;
    TArrayD alpha;
    TArrayD beta;
    TArrayD gamma;
};

/// generate an isotropic random direction, from optional random in [0,1]^2
void randomDirection(double& x, double& y, double& z, double* rnd = NULL);

/// Nuclear energy level
class NucLevel {
public:
    /// constructor
    NucLevel(const Stringmap& m);
    /// print info
    void display(bool verbose = false) const;
    /// scale probabilities
    void scale(double s) { fluxIn *= s; fluxOut *= s; }
    
    string name;        ///< name for this level
    unsigned int A;     ///< nucleus A
    unsigned int Z;     ///< nucleus Z
    unsigned int n;     ///< level number
    double E;           ///< energy
    double hl;          ///< half-life
    string jpi;         ///< spin/parity
    double fluxIn;      ///< net flux into level
    double fluxOut;     ///< net flux out of level
};

/// primary particle types
/// using PDG numbering scheme, http://pdg.lbl.gov/2012/reviews/rpp2012-rev-monte-carlo-numbering.pdf
enum DecayType {
    D_GAMMA = 22,
    D_ELECTRON = 11,
    D_POSITRON = -11,
    D_NEUTRINO = -12, // anti-nu_e
    D_NONEVENT = 0
};

/// string name of particle types
string particleName(DecayType t);
/// decay type from particle name
DecayType particleType(const std::string& s);


/// specification for a decay particle
class NucDecayEvent {
public:
    /// constructor
    NucDecayEvent(): eid(0), E(0), d(D_NONEVENT), t(0), w(1.) {}
    /// randomize momentum direction
    void randp(double* rnd = NULL) { randomDirection(p[0],p[1],p[2],rnd); }
    
    unsigned int eid;   ///< event ID number
    double E;           ///< particle energy
    double p[3];        ///< particle momentum direction
    double x[3];        ///< vertex position
    DecayType d;        ///< particle type
    double t;           ///< time of event
    double w;           ///< weighting for event
};

/// Atom/electron information
class DecayAtom {
public:
    /// constructor
    DecayAtom(BindingEnergyTable const* B);
    /// load Auger data from Stringmap
    void load(const Stringmap& m);
    /// generate Auger K probabilistically
    void genAuger(vector<NucDecayEvent>& v);
    /// display info
    void display(bool verbose = false) const;
    
    BindingEnergyTable const* BET;      ///< binding energy table
    double Eauger;                      ///< Auger K energy
    double Iauger;                      ///< intensity of Auger electron emissions
    double Ikxr;                        ///< intensity of k X-Ray emissions
    double ICEK;                        ///< intensity of CE K events
    double IMissing;                    ///< intensity of un-accounted for Augers (from initial capture)
    double pAuger;                      ///< probability of auger given any opening
};

/// Transition base class
class TransitionBase {
public:
    /// constructor
    TransitionBase(NucLevel& f, NucLevel& t): from(f), to(t), Itotal(0) {}
    /// destructor
    virtual ~TransitionBase() {}
    /// display transition line info
    virtual void display(bool verbose = false) const;
    
    /// select transition outcome
    virtual void run(vector<NucDecayEvent>&, double* = NULL) { }
    
    /// return number of continuous degrees of freedom needed to specify transition
    virtual unsigned int getNDF() const { return 2; }
    
    /// scale probability
    virtual void scale(double s) { Itotal *= s; }
    
    /// get probability of removing an electron from a given shell
    virtual double getPVacant(unsigned int) const { return 0; }
    /// how many of given electron type were knocked out
    virtual unsigned int nVacant(unsigned int) const { return 0; }
    
    DecayAtom* toAtom;  ///< final state atom info
    NucLevel& from;     ///< level this transition is from
    NucLevel& to;       ///< level this transition is to
    double Itotal;      ///< total transition intensity
};


/// Gamma transition with possible conversion electrons
class ConversionGamma: public TransitionBase {
public:
    /// constructor
    ConversionGamma(NucLevel& f, NucLevel& t, const Stringmap& m);
    /// select transition outcome
    virtual void run(vector<NucDecayEvent>& v, double* rnd = NULL);
    /// display transition line info
    virtual void display(bool verbose = false) const;
    /// get total conversion efficiency
    double getConversionEffic() const;
    /// get probability of knocking conversion electron from a given shell
    virtual double getPVacant(unsigned int n) const { return n<shells.getN()-1?shells.getProb(n):0; }
    /// get whether said electron was knocked out
    virtual unsigned int nVacant(unsigned int n) const { return int(n)==shell; }
    /// shell weighted average energy
    double shellAverageE(unsigned int n) const;
    /// line weighted average
    float_err averageE() const;
    /// scale probability
    virtual void scale(double s);
    
    double Egamma;      ///< gamma energy
    int shell;          ///< selected conversion electron shell
    int subshell;       ///< selected conversion electron subshell
    double Igamma;      ///< total gamma intensity
    
protected:
    PSelector shells;                   ///< conversion electron shells
    vector<float> shellUncert;     ///< uncertainty on shell selection probability
    vector<PSelector> subshells;   ///< subshell choices for each shell
};

/// electron capture transitions
class ECapture: public TransitionBase {
public:
    /// constructor
    ECapture(NucLevel& f, NucLevel& t): TransitionBase(f,t) {}
    /// select transition outcome
    virtual void run(vector<NucDecayEvent>&, double* rnd = NULL);
    /// display transition line info
    virtual void display(bool verbose = false) const { printf("Ecapture "); TransitionBase::display(verbose); }
    /// get probability of removing an electron from a given shell
    virtual double getPVacant(unsigned int n) const { return n==0?toAtom->IMissing:0; }
    /// get whether said electron was knocked out
    virtual unsigned int nVacant(unsigned int n) const { return n==0?isKCapt:0; }
    
    /// return number of continuous degrees of freedom needed to specify transition
    virtual unsigned int getNDF() const { return 0; }
    
    bool isKCapt;                       ///< whether transition was a K capture
};

/// beta decay transitions
class BetaDecayTrans: public TransitionBase {
public:
    /// constructor
    BetaDecayTrans(NucLevel& f, NucLevel& t, bool pstrn = false, unsigned int forbidden = 0);
    /// destructor
    ~BetaDecayTrans();
    /// select transition outcome
    virtual void run(vector<NucDecayEvent>& v, double* rnd = NULL);
    /// display transition line info
    virtual void display(bool verbose = false) const { printf("Beta(%.1f) ",from.E-to.E); TransitionBase::display(verbose); }
    
    /// return number of continuous degrees of freedom needed to specify transition
    virtual unsigned int getNDF() const { return 3; }
    
    bool positron;                      ///< whether this is positron decay
    BetaSpectrumGenerator BSG;          ///< spectrum shape generator
    
protected:
    /// evaluate beta spectrum probability
    double evalBeta(double* x, double*);
    TF1 betaTF1;                        ///< TF1 for beta spectrum shape
    TF1_Quantiles* betaQuantiles;       ///< inverse CDF of beta spectrum shape for random point selection
};

/// Decay system
class NucDecaySystem {
public:
    /// constructor from specification file
    NucDecaySystem(const SMFile& Q, const BindingEnergyLibrary& B, double t = DBL_MAX);
    /// destructor
    ~NucDecaySystem();
    /// set cutoff lifetime for intermediate states
    void setCutoff(double t);
    /// display transitions summary
    void display(bool verbose = false) const;
    /// display list of levels
    void displayLevels(bool verbose = false) const;
    /// display list of transitions
    void displayTransitions(bool verbose = false) const;
    /// display list of atoms
    void displayAtoms(bool verbose = false) const;
    /// generate a chain of decay events starting from level n
    void genDecayChain(vector<NucDecayEvent>& v, double* rnd = NULL, unsigned int n = UINT_MAX);
    /// rescale all probabilities
    void scale(double s);
    
    /// return number of degrees of freedom needed to specify decay from given level
    unsigned int getNDF(unsigned int n = UINT_MAX) const;
    
    /// LaTeX name for generator
    string fancyname;
    
protected:
    /// get index for named level
    unsigned int levIndex(const std::string& s) const;
    /// get atom info for given Z
    DecayAtom* getAtom(unsigned int Z);
    /// add a transition
    void addTransition(TransitionBase* T);
    /// check against circular references
    void circle_check(unsigned int n, set<unsigned int> pnts = set<unsigned int>()) const;
    
    BindingEnergyLibrary const&  BEL;           ///< electron binding energy info
    double tcut;
    vector<NucLevel> levels;                    ///< levels, enumerated
    map<std::string,unsigned int> levelIndex;   ///< energy levels by name
    PSelector lStart;                           ///< selector for starting level (for breaking up long decays)
    vector<PSelector> levelDecays;              ///< probabilities for transitions from each level
    map<unsigned int, DecayAtom*> atoms;        ///< atom information
    vector<TransitionBase*> transitions;        ///< transitions, enumerated
    vector< vector<TransitionBase*> > transIn;  ///< transitions into each level
    vector< vector<TransitionBase*> > transOut; ///< transitions out of each level
};

/// manager for loading decay event generators
class NucDecayLibrary {
public:
    /// constructor
    NucDecayLibrary(const std::string& datp, double t = DBL_MAX);
    /// destructor
    ~NucDecayLibrary();
    /// check if generator is available
    bool hasGenerator(const std::string& n);
    /// get decay generator by name
    NucDecaySystem& getGenerator(const std::string& n);
    
    string datpath;             ///< path to data folder
    double tcut;                ///< event generator default cutoff time
    BindingEnergyLibrary  BEL;  ///< electron binding energy info
    
protected:
    map<std::string,NucDecaySystem*> NDs;  ///< loaded decay systems
    std::set<string> cantdothis;           ///< list of decay systems that can't be loaded
};

/// class for throwing from large list of gammas
class GammaForest {
public:
    /// constructor
    GammaForest(const std::string& fname, double E2keV = 1000);
    /// get total cross section
    double getCrossSection() const { return gammaProb.getCumProb(); }
    /// generate cluster of gamma decays
    void genDecays(vector<NucDecayEvent>& v, double n = 1.0);
protected:
    vector<double> gammaE;      ///< gamma energies
    PSelector gammaProb;        ///< gamma probabilities selector
};

#endif
