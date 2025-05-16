#pragma once

#include "component/Block/BlockManager.h"
#include "component/EffectByBlock/EffectByBlockUIStatus.h"
#include "system/ISystem.h"
#include <vector>

class BlockStatus;
class BreakBlockSystem
    : public ISystem {
public:
    BreakBlockSystem();
    ~BreakBlockSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    void BlockReaction(GameEntity* _entity,BlockType blocktype);
    void ScrapSpawn(GameEntity* _entity);
  /*  void EffectUISpawn(GameEntity* _entity);*/

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

private:
    std::vector<BlockStatus*> rightBlocks_;
    BlockStatus* blockStatus_;
    EffectType effectType_;
    float tempValue_;
};
