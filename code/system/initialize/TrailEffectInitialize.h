#pragma once
#include "system/ISystem.h"

class TrailEffectInitialize
    : public OriGine::ISystem {
public:
    TrailEffectInitialize();
    ~TrailEffectInitialize();

    void Initialize();
    void Finalize();

private:
    void UpdateEntity(OriGine::Entity* _entity);
};
