#pragma once
#include "component/IComponent.h"
#include "PlayerStatus.h"

/// stl
#include <array>

/// component
#include "state/PlayerState.h"

/// math
#include "math/Vector4.h"

/// <summary>
/// Playerのエフェクト制御用パラメータコンポーネント
/// </summary>
class PlayerEffectControlParam
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& j, const PlayerEffectControlParam& _p);
    friend void from_json(const nlohmann::json& j, PlayerEffectControlParam& _p);

public:
    PlayerEffectControlParam();
    ~PlayerEffectControlParam() override;
    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;
    void Finalize() override;

    /// <summary>
    /// ギアレベルに応じたホイールの回転速度を計算する
    /// </summary>
    float CalculateWheelSpinSpeedBySpeed(float _currentSpeed, float _maxSpeed) const;

    /// <summary>
    /// 進行方向と入力方向の角度差に応じたホイールの傾き角度を計算する
    /// </summary>
    /// <param name="_inputV">入力ベクトル</param>
    /// <param name="_direction">進行方向ベクトル</param>
    float CalculateWheelTiltAngle(const OriGine::Vec3f& _inputV, const OriGine::Vec3f& _direction) const;

private:
    std::vector<OriGine::Vec4f> trailColorByGearLevel_ = std::vector<OriGine::Vec4f>(kMaxPlayerGearLevel, OriGine::Vec4f(1.f, 1.f, 1.f, 1.f));
    float rotateOffsetOnWallRun_                       = 0.0f;
    float maxWheelSpinSpeed_                           = 0.001f;
    float wheelTiltAngleRate_                          = 0.0f;
    float preWheelTiltAngle_                           = 0.0f;
    // 最大傾き角度の加速度
    float wheelTiltAngleMaxAccel_ = 0.0f;

    int32_t tireTrailSplineEntityId_ = -1;

public:
    const OriGine::Vec4f& GetTrailColorByGearLevel(int32_t _level) const {
        return trailColorByGearLevel_[_level];
    }
    float GetRotateOffsetOnWallRun() const {
        return rotateOffsetOnWallRun_;
    }
    float GetMaxWheelSpinSpeedRate() const {
        return maxWheelSpinSpeed_;
    };
    float GetWheelTiltAngleRate() const {
        return wheelTiltAngleRate_;
    }
    float GetWheelTiltAngleMaxAccel() const {
        return wheelTiltAngleMaxAccel_;
    }

    float GetPreWheelTiltAngle() const {
        return preWheelTiltAngle_;
    }
    void SetPreWheelTiltAngle(float _angle) {
        preWheelTiltAngle_ = _angle;
    }

    int32_t GetTireTrailSplineEntityId() const {
        return tireTrailSplineEntityId_;
    }
    void SetTireTrailSplineEntityId(int32_t _id) {
        tireTrailSplineEntityId_ = _id;
    }
};
