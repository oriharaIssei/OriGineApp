#pragma once

#include "component/Block/BlockManager.h"
#include "component/EffectByBlock/EffectByBlockUIStatus.h"
#include "system/ISystem.h"

 class BlockStatus;

class FrontPlaneDeleteSystem
    : public ISystem {
public:
    FrontPlaneDeleteSystem();
    ~FrontPlaneDeleteSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

    void BlockReaction(GameEntity* _entity, BlockType blocktype);

    private:
    BlockStatus* blockStatus_;
        EffectType effectType_;
        float tempValue_;
   
};
