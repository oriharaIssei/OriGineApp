// PillarSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// PillerPhysicsSystem
///====================================================================
class BomCollisionExSystem : public ISystem {
public:
    BomCollisionExSystem();
    ~BomCollisionExSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
