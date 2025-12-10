#pragma once
#include "system/ISystem.h"

/// <summary>
/// Playerの移動を制御するシステム
/// </summary>
class PlayerMoveSystem
    : public OriGine::ISystem {
public:
    PlayerMoveSystem() : ISystem(OriGine::SystemCategory::Movement) {}
    ~PlayerMoveSystem() {}

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

private:
    void UpdateEntity(OriGine::Entity* _entity) override;
};
