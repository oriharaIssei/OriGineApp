#pragma once
#include "system/ISystem.h"

/// <summary>
/// Playerの衝突反応システム
/// </summary>
class PlayerOnCollision
    : public ISystem {
public:
    PlayerOnCollision(int32_t _priority = 0)
        : ISystem(SystemCategory::Collision, _priority) {};
    void Initialize() override;
    // void Update() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;
};
