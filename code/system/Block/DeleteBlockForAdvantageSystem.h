#pragma once

#include "component/Block/BlockManager.h"
#include "component/EffectByBlock/EffectByBlockUIStatus.h"
#include "system/ISystem.h"

class BlockStatus;
class EffectByBlockSpawner;
struct Transform;
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

    void BlockReaction(GameEntity* _entity, BlockType blocktype);
    void ApearResultScoreUI();
    

private:
    EffectByBlockSpawner* SpawnerStatus_;
    Transform* blockTransform_; 
    BlockStatus* blockStatus_;
    EffectType effectType_;
    float tempValue_;
};
