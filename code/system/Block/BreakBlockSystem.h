#pragma once

#include "component/Block/BlockManager.h"
#include "system/ISystem.h"

class BreakBlockSystem
    : public ISystem {
public:
    BreakBlockSystem();
    ~BreakBlockSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    void BlockReaction(BlockType blocktype);
    void ScrapSpawn(GameEntity* _entity);

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
};
