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
    : public IComponent {
    friend void to_json(nlohmann::json& j, const PlayerEffectControlParam& _p);
    friend void from_json(const nlohmann::json& j, PlayerEffectControlParam& _p);

public:
    PlayerEffectControlParam();
    ~PlayerEffectControlParam() override;
    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
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
    float CalculateWheelTiltAngle(const Vec3f& _inputV, const Vec3f& _direction) const;

private:
    std::vector<Vector4f> trailColorByGearLevel_ = std::vector<Vector4f>(kMaxPlayerGearLevel, Vector4f(1.f, 1.f, 1.f, 1.f));
    float rotateOffsetOnWallRun_                 = 0.0f;
    float maxWheelSpinSpeed_                     = 0.001f;
    float wheelTiltAngleRate_                    = 0.0f;
    float preWheelTiltAngle_                     = 0.0f;
    // 最大傾き角度の加速度
    float wheelTiltAngleMaxAccel_ = 0.0f;

public:
    Vector4f getTrailColorByGearLevel(int32_t _level) const {
        return trailColorByGearLevel_[_level];
    }
    float getRotateOffsetOnWallRun() const {
        return rotateOffsetOnWallRun_;
    }
    float getMaxWheelSpinSpeedRate() const {
        return maxWheelSpinSpeed_;
    };
    float getWheelTiltAngleRate() const {
        return wheelTiltAngleRate_;
    }
    float getWheelTiltAngleMaxAccel() const {
        return wheelTiltAngleMaxAccel_;
    }

    float getPreWheelTiltAngle() const {
        return preWheelTiltAngle_;
    }
    void setPreWheelTiltAngle(float _angle) {
        preWheelTiltAngle_ = _angle;
    }
};
