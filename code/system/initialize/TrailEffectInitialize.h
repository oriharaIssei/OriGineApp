#pragma once
#include "system/ISystem.h"

class TrailEffectInitialize
    : public ISystem {
public:
    TrailEffectInitialize();
    ~TrailEffectInitialize();

    void Initialize();
    void Finalize();

private:
    void UpdateEntity(Entity* _entity);
};
