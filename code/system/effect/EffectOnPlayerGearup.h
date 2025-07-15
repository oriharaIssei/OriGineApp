#pragma once
#include "system/ISystem.h"

#include <math/Vector3.h>

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
    /// ==========================================
    // ShockWave Ring
    /// ==========================================
    bool isEffectPlaying_ = false; // 衝撃波の再生中かどうか

    const Vec3f shockWaveOffset_ = {0.f, 0.f, 33.f}; // 衝撃波のオフセット位置
    const Vec3f emitterOffset_   = {0.0f, 0.0f, 8.f};

    const float minInnerRadius_ = 4.2f; // 衝撃波の中半径
    const float minouterRadius_ = 5.1f; // 衝撃波の外半径
    const float maxInnerRadius_ = 14.3f; // 衝撃波の中半径
    const float maxouterRadius_ = 19.2f; // 衝撃波の外半径

    const float minRadialBlurWidth_ = 0.f; // 衝撃波のレイディアルブラーの幅
    const float maxRadialBlurWidth_ = -0.04f; // 衝撃波のレイディアルブラーの幅

    float currentTime_   = 0.f;
    const float maxTime_ = 0.6f;

    void UpdateEntity(GameEntity* _entity) override;
};
