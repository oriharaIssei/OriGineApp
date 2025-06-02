#pragma once

#include "component/Block/BlockManager.h"
#include "component/EffectByBlock/EffectByBlockUIStatus.h"
#include "system/ISystem.h"
#include <array>

class BlockStatus;
class EffectByBlockSpawner;
struct Transform;
class Emitter;
class DeleteBlockForAdvantageSystem
    : public ISystem {
public:
    DeleteBlockForAdvantageSystem();
    ~DeleteBlockForAdvantageSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

    void BlockBreakParticleShot(GameEntity* _entity, BlockType blocktype);
    void ApearResultScoreUI();

    void SpawnBlockEffect(BlockType type);
    void SpawnBlockEffectN(BlockType type);

private:
    EffectByBlockSpawner* SpawnerStatus_;
    Transform* blockTransform_; 
    BlockStatus* blockStatus_;
    EffectType effectType_;

    EffectType timeEffectType_;
    EffectType scoreEffectType_;

    std::array<Emitter*, 3> breakBlockEffects_;

    std::array<Emitter*, 3> breakBlockEffectsN_;
      std::array<Emitter*, 3> breakEffect_;
    
    float tempValue_;
};
