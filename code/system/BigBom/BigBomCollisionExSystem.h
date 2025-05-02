// PillarSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// PillerPhysicsSystem
///====================================================================
class BigBomCollisionExSystem : public ISystem {
public:
    BigBomCollisionExSystem();
    ~BigBomCollisionExSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
