#include "RunAction.hh"

#include "FileIO.hh"
#include "RunMessenger.hh"
#include "EventAction.hh"
#include "StackingAction.hh"
#include "ProcessInfo.hh"
#include "CommandInfo.hh"
#include "strutils.hh"

#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4ios.hh>


RunAction::RunAction(PrimaryGeneratorAction* gn, DetectorConstruction* d, PhysicsList* p):
XMLProvider("Run"), gen(gn), det(d), phys(p) {
    run_messenger = new RunMessenger(this);
}

RunAction::~RunAction() {
   delete run_messenger;
}

void RunAction::SetRunNumber(G4int rnum) {
    nRunNumber = rnum;
    G4RunManager::GetRunManager()->SetRunIDCounter(nRunNumber);
}

void RunAction::BeginOfRunAction(const G4Run* aRun) {
    start_time = time(NULL);
    G4cout << "~~~~~ Run Number " << aRun->GetRunID() << " Initiated ~~~~~\n" << G4endl;
    CLHEP::HepRandom::setTheSeed(aRun->GetRunID()); // set unique random seed for run
}

void RunAction::fillNode(TXMLEngine& E) {
    addAttr(E, "t_start",to_str(start_time));
    addAttr(E, "t_end",to_str(end_time));
    addAttrI(E, "num", nRunNumber);
}

void RunAction::EndOfRunAction(const G4Run* aRun) {
    end_time = time(NULL);
    G4int numOfEvents = aRun->GetNumberOfEvent();
    if (numOfEvents == 0) return;
    
    FileIO* R = FileIO::GetInstance();
    
    // set up XML output
    children.clear();
    ProcessInfo PI;
    addChild(&PI);
    CommandInfo CI;
    addChild(&CI);
    addChild(det);
    addChild(gen);
    addChild(phys);
    writeToFile(R->GetFileName()+".xml");
    children.clear();
}

void RunAction::SetPrimariesOnly() {
    StackingAction* stacking_action = new StackingAction();
    stacking_action->noTracks = true;
    G4RunManager::GetRunManager()->SetUserAction(stacking_action);
    SetRecordLevel(3);
}
