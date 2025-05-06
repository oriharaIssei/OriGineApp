// PillarSystem.h
#pragma once
#include "system/ISystem.h"

///====================================================================
// PillerPhysicsSystem
///====================================================================
class BlockColorChangeSystem : public ISystem {
public:
    BlockColorChangeSystem();
    ~BlockColorChangeSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
