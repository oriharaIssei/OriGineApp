#pragma once
#include "system/ISystem.h"

/// <summary>
/// Playerの移動を制御するシステム
/// </summary>
class PlayerMoveSystem
    : public ISystem {
public:
    PlayerMoveSystem() : ISystem(SystemCategory::Movement) {}
    ~PlayerMoveSystem() {}

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
