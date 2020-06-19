#include "MyActionInitialization.h"
#include "MyPrimaryGenerator.h"

void MyActionInitialization::Build() const {
    SetUserAction( new MyPrimaryGenerator );
};