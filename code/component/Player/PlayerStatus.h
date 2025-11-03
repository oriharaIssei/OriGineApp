#pragma once
#include "component/IComponent.h"

/// engine
#include "scene/Scene.h"

/// component
class Rigidbody;
struct Transform;
class PlayerInput;

/// math
#include <math/Quaternion.h>
#include <math/Vector3.h>

constexpr float kPlayerHeight = 2.0f;

/// <summary>
/// Playerのステータスを管理するコンポーネント
/// </summary>
class PlayerStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const PlayerStatus& _playerStatus);
    friend void from_json(const nlohmann::json& j, PlayerStatus& _playerStatus);

public:
    PlayerStatus();
    ~PlayerStatus();

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Debug(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

    /// <summary>
    /// ギアレベルに応じた速度を計算する
    /// </summary>
    /// <param name="_gearLevel"></param>
    /// <returns></returns>
    float CalculateSpeedByGearLevel(int32_t _gearLevel) const;
    /// <summary>
    /// ギアレベルに応じたクールタイムを計算する
    /// </summary>
    float CalculateCoolTimeByGearLevel(int32_t _gearLevel) const;

    void UpdateAccel(float _deltaTime, PlayerInput* _input, Transform* _transform, Rigidbody* _rigidbody, const Quaternion& _cameraRotation);

private:
    /// ==========================================
    // 能力値
    float baseGearupCoolTime_        = 1.0f; // ギアアップの基本クールタイム (秒単位)
    float gearUpCoolTime_            = 0.0f; // ギアレベルが上がるまでの時間
    float coolTimeAddRateBase_       = 1.0f;
    float coolTimeAddRateCommonRate_ = 1.f;

    // 速度
    float baseSpeed_             = 0.0f; // 基本速度 (ギアレベル0の時の速度)
    float speedUpRateBase_       = 1.0f; // ギアアップ時の速度上昇率の基本値
    float speedUpRateCommonRate_ = 1.f; // ギアアップ時の速度上昇率の共通値

    // 壁系
    float wallRunRate_    = 0.0f; // 壁走りの速度倍率
    Vec3f wallJumpOffset_ = {0.0f, 1.0f, 0.0f};
    float wallJumpRate_   = 0.0f; // 壁ジャンプ(壁から地面に行くとき)の速度倍率

    // currentMaxSpeed は gearLevel に応じて変化する
    float currentMaxSpeed_ = 0.0f; // 現在の最大速度

    float jumpPower_ = 0.0f; // ジャンプのパワー
    float fallPower_ = 0.0f; // 落下のパワー

    float directionInterpolateRate_ = 0.1f;

public:
    float getDirectionInterpolateRate() const {
        return directionInterpolateRate_;
    }

    float getBaseSpeed() const {
        return baseSpeed_;
    }
    float getJumpPower() const { return jumpPower_; }
    float getFallPower() const { return fallPower_; }

    float getCurrentMaxSpeed() const {
        return currentMaxSpeed_;
    }
    void setCurrentMaxSpeed(float _currentMaxSpeed) {
        currentMaxSpeed_ = _currentMaxSpeed;
    }

    float getWallRunRate() const {
        return wallRunRate_;
    }
    void setWallRunRate(float _wallRunRate) {
        wallRunRate_ = _wallRunRate;
    }

    const Vec3f& getWallJumpOffset() const {
        return wallJumpOffset_;
    }
    void setWallJumpOffset(const Vec3f& _wallJumpOffset) {
        wallJumpOffset_ = _wallJumpOffset;
    }

    float getBaseGearupCoolTime() const {
        return baseGearupCoolTime_;
    }
    float getGearUpCoolTime() const {
        return gearUpCoolTime_;
    }
    void setGearUpCoolTime(float _gearUpCoolTime) {
        gearUpCoolTime_ = _gearUpCoolTime;
    }
    void minusGearUpCoolTime(float _deltaTime) {
        gearUpCoolTime_ -= _deltaTime;
    }

    float getCoolTimeUpRateBase() const {
        return coolTimeAddRateBase_;
    }
    float getCoolTimeUpRateCommonRate() const {
        return coolTimeAddRateCommonRate_;
    }
    float getSpeedUpRateBase() const {
        return speedUpRateBase_;
    }
    float getSpeedUpRateCommonRate() const {
        return speedUpRateCommonRate_;
    }
};
