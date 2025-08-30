#pragma once
#include "system/ISystem.h"

/// ECS
// component
struct Transform;

/// util
#include "util/DiffValue.h"

/// math
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
    void UpdateEntity(GameEntity* _entity) override;

    void UpdateShockWaveRing(GameEntity* _entity, Transform* _playerTransform);

protected:
    const Vec3f emitterOffset_ = {0.0f, 0.0f, 8.f};

    struct AnimationState {
        AnimationState(bool _playState, float _currentTime, float _maxTime)
            : playState_(_playState), currentTime(_currentTime), maxTime(_maxTime) {}
        DiffValue<bool> playState_ = false; // 再生中かどうか
        float currentTime          = 0.f;
        float maxTime              = 1.f;
    };
    /// ==========================================
    // ShockWave Ring
    /// ==========================================
    AnimationState shockWaveState_ = AnimationState(false, 0.f, 0.6f);

    const Vec3f shockWaveOffset_ = {0.f, 0.f, 33.f}; // 衝撃波のオフセット位置
    const float minInnerRadius_  = 4.2f; // 衝撃波の中半径
    const float maxInnerRadius_  = 14.3f; // 衝撃波の中半径
    const float minOuterRadius_  = 5.1f; // 衝撃波の外半径
    const float maxOuterRadius_  = 16.0f; // 衝撃波の外半径
};
