#include "MyEventAction.h"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"

void MyEventAction::BeginOfEventAction(const G4Event* anEvent){

    G4cout << "\t Starting event " << anEvent->GetEventID() << G4endl;    

}

void MyEventAction::EndOfEventAction(const G4Event* anEvent){

    G4cout << "\t End of event.\n";
}
