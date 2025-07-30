#pragma once
#include "system/ISystem.h"

class PlayerOnCollision
    : public ISystem {
public:
    PlayerOnCollision(int32_t _priority = 0)
        : ISystem(SystemCategory::Collision, _priority) {};
    void Initialize() override;
    // void Update() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
