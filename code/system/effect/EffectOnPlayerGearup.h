#pragma once
#include "system/ISystem.h"

/// math
#include <Vector3.h>

/// <summary>
/// PlayerがGearupしたとき, 衝撃波もどきを発生させる
/// </summary>
class EffectOnPlayerGearup
    : public ISystem {
public:
    EffectOnPlayerGearup();
    ~EffectOnPlayerGearup();

    void Initialize() override;
    void Finalize() override;

protected:
    bool prePlayerIsGearup_ = false; // 前フレームにPlayerがGearupしているかどうか
    bool isPlay_            = false; // 衝撃波を発生させるかどうか

    /// ==========================================
    // ShockWave Ring
    /// ==========================================
    const Vec3f shockWaveOffset_ = {0.f, 0.f, 20.f}; // 衝撃波のオフセット位置

    const float minInnerRadius_ = 1.15f; // 衝撃波の中半径
    const float minouterRadius_ = 1.46f; // 衝撃波の外半径
    const float maxInnerRadius_ = 24.0f; // 衝撃波の中半径
    const float maxouterRadius_ = 28.0f; // 衝撃波の外半径

    const float defaultFovY_ = 45.0f * 3.141592654f / 180.0f;
    const float maxFovY_     = 98.f * 3.141592654f / 180.0f;

    float currentTime_   = 0.f;
    const float maxTime_ = 3.f;

    void UpdateEntity(GameEntity* _entity) override;
};
