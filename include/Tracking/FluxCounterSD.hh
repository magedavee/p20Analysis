/// \file FluxCounterSD.hh \brief Sensitive detector for tallying particles entering/leaving a volume.
#ifndef FLUXCOUNTERSD_HH
#define FLUXCOUNTERSD_HH

#include <G4VSensitiveDetector.hh>
#include <set>

/// Sensitive detector for recording particles entering/leaving a volume
class FluxCounterSD: public G4VSensitiveDetector {
public:
    /// Constructor
    FluxCounterSD(G4String name);
    
    /// Initializes detector at start of event
    void Initialize(G4HCofThisEvent*) { }
    /// Called each step to determine what qualifies as a "hit"
    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* H);
    /// Some processing to be done when an event is finished, as new event will erase old event data
    void EndOfEvent(G4HCofThisEvent*) { }

    std::set<G4int> record_PIDs;        ///< Particle IDs of fluxes to record; leave empty for all.
};

#endif