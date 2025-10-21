#pragma once

#ifndef _RELEASE

#include "system/ISystem.h"

class StageReloadSystem
    : public ISystem {
public:
    StageReloadSystem() : ISystem(SystemCategory::StateTransition) {}
    ~StageReloadSystem() override = default;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity);
};

#endif // _RELEASE
