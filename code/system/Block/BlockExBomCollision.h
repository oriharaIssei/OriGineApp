// PillarSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// PillerPhysicsSystem
///====================================================================
class BlockExBomCollision : public ISystem {
public:
    BlockExBomCollision();
    ~BlockExBomCollision();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
