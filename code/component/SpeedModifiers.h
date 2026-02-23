#pragma once

#include "component/IComponent.h"

/// ECS
// component
#include "component/ComponentHandle.h"

/// math
#include "math/Vector3.h"
#include <math/MyEasing.h>

namespace OriGine {
class Rigidbody;
}

/// <summary>
/// 速度補正コンポーネント
/// </summary>
struct SpeedModifiers
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& _j, const SpeedModifiers& _c);
    friend void from_json(const nlohmann::json& _j, SpeedModifiers& _c);

public:
    SpeedModifiers();
    ~SpeedModifiers();

    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    void Finalize() override;

    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;

    void Reset();

    /// <summary>
    /// 加算効果の開始
    /// </summary>
    /// <param name="_target"></param>
    /// <param name="_lerpDuration"></param>
    /// <param name="_effectDuration"></param>
    /// <param name="_easeType"></param>
    /// <param name="_beforeSpeed"></param>
    void StartAdditiveEffect(float _target, float _lerpDuration, float _effectDuration, OriGine::EaseType _easeType, float _beforeSpeed, float _restoreSpeed);
    /// <summary>
    /// 乗算効果の開始
    /// </summary>
    /// <param name="_target"></param>
    /// <param name="_lerpDuration"></param>
    /// <param name="_effectDuration"></param>
    /// <param name="_easeType"></param>
    /// <param name="_beforeSpeed"></param>
    void StartMultiplierEffect(float _target, float _lerpDuration, float _effectDuration, OriGine::EaseType _easeType, float _beforeSpeed, float _restoreSpeed);

    bool IsAdditiveEffectActive() const {
        return additiveTimer < additiveDuration;
    }
    bool IsMultiplierEffectActive() const {
        return multiplierTimer < multiplierDuration;
    }

public:
    bool isAutoDestroyed = true; // 効果時間終了後に自動で削除されるかどうか

    OriGine::ComponentHandle targetRigidbodyHandle{};

    // Additive
    float additiveTarget                   = 0.f;
    float additiveLerpTimer                = 0.0f;
    float additiveLerpDuration             = 0.0f;
    float additiveDuration                 = 0.0f;
    float additiveTimer                    = 0.0f;
    OriGine::EaseType additiveLerpEaseType = OriGine::EaseType::Linear;

    // Multiplier
    float multiplierTarget                   = 1.f;
    float multiplierLerpTimer                = 0.0f;
    float multiplierLerpDuration             = 0.0f;
    float multiplierDuration                 = 0.0f;
    float multiplierTimer                    = 0.0f;
    OriGine::EaseType multiplierLerpEaseType = OriGine::EaseType::Linear;

    float beforeSpeed  = 0.0f;
    float restoreSpeed = 0.0f;
};
