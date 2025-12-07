#pragma once
#include "system/ISystem.h"

/// <summary>
/// Playerの速度を 3dUIに適応するクラス
/// </summary>
class PlayerSpeedFor3dUI
    : public ISystem {
public:
    PlayerSpeedFor3dUI();
    ~PlayerSpeedFor3dUI() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
