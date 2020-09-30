#pragma once

#include "G4VUserActionInitialization.hh"


class MyActionInitialization : public G4VUserActionInitialization{
public:
    virtual void Build() const override;
};
