#pragma once
#include "system/ISystem.h"

/// <summary>
/// Playerの衝突反応システム
/// </summary>
class PlayerOnCollision
    : public OriGine::ISystem {
public:
    PlayerOnCollision(int32_t _priority = 0)
        : ISystem(OriGine::SystemCategory::Collision, _priority) {};
    void Initialize() override;
    // void Update() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::EntityHandle _handle) override;
};
