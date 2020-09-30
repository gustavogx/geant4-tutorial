#include "MyRunAction.h"
#include "G4Run.hh"

void MyRunAction::BeginOfRunAction(const G4Run* aRun){

    G4cout << "=== Starting run === " << aRun->GetRunID() << " with " << aRun->GetNumberOfEventToBeProcessed() << " events to be processed." << G4endl;
}

void MyRunAction::EndOfRunAction(const G4Run* aRun){

    G4cout << "=== End of Run ===" << G4endl;

}
