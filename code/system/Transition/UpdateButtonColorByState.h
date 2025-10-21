#pragma once

#include "system/ISystem.h"
class UpdateButtonColorByState
    : public ISystem {
public:
    UpdateButtonColorByState() : ISystem(SystemCategory::StateTransition) {}
    ~UpdateButtonColorByState() {}

    virtual void Initialize();
    virtual void Finalize();

protected:
    virtual void UpdateEntity(Entity* _entity);
};
