#pragma once

#include "component/Block/BlockManager.h"
#include "component/EffectByBlock/EffectByBlockUIStatus.h"
#include "system/ISystem.h"
#include"component/transform/Transform.h"
#include <vector>

class BlockStatus;
class Emitter;
class BreakBlockSystem
    : public ISystem {
public:
    BreakBlockSystem();
    ~BreakBlockSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    void BlockReaction(GameEntity* _entity, BlockType blocktype);
    void ScrapSpawn(GameEntity* _entity);

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

    void SpawnBlockEffect(BlockType type);
    void SpawnBlockEffectN(BlockType type);
    void SpawnBlockEffectS(BlockType type);

private:
    std::vector<BlockStatus*> rightBlocks_;
    BlockStatus* blockStatus_;
    EffectType effectType_;
    float tempValue_;
    Transform* transform;

    std::array<Emitter*, 3> breakBlockEffects_;
    std::array<Emitter*, 3> breakBlockEffectsS_;
    std::array<Emitter*, 3> breakBlockEffectsN_;
};
