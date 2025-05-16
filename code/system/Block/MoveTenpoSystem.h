#pragma once

#include "system/ISystem.h"
#include <cstdint>

class BlockManager;
class MoveTenpoSystem
    : public ISystem {
public:
    MoveTenpoSystem();
    ~MoveTenpoSystem();

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

    void OneTenpoMoveMethod(GameEntity* _entity);

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

private:
    BlockManager* blockManager_;

    float timer_;

    bool isOneTenpoEnd_     = false;
    int32_t curentTempoNum_ = 0;
    float curerntTenpoTime_ = 0.0f;
    
};
