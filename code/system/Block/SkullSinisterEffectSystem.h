// PillarSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// PillerPhysicsSystem
///====================================================================
class SkullSinisterEffectSystem : public ISystem {
public:
    SkullSinisterEffectSystem();
    ~SkullSinisterEffectSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
