#pragma once
#include "component/IComponent.h"

/// engine
#include "scene/Scene.h"

/// component
class Rigidbody;
struct Transform;
class PlayerInput;
class IPlayerMoveState;

/// util
#include "util/EnumBitMask.h"

/// math
#include <math/Quaternion.h>
#include <math/Vector3.h>

/// <summary>
/// プレイヤーの 移動状態 を表す列挙体
/// </summary>
enum class PlayerMoveState {
    IDLE      = 1 << 0, // 待機 (動いていない)
    DASH      = 1 << 1, // ダッシュ(基本移動)
    FALL_DOWN = 1 << 2, // 落下中 (ジャンプ ではない.)
    JUMP      = 1 << 3, // ジャンプ
    WALL_RUN  = 1 << 4, // 壁走り
    WALL_JUMP = 1 << 5, // 壁ジャンプ
    // SLIDE    = 1 << 4, // スライド

    Count = 6 // 5
};

constexpr int32_t kDefaultPlayerGearLevel = 1; // デフォルトのギアレベル

class PlayerStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const PlayerStatus& _playerStatus);
    friend void from_json(const nlohmann::json& j, PlayerStatus& _playerStatus);

public:
    PlayerStatus();
    ~PlayerStatus();

    void Initialize(GameEntity* _entity) override;
    void Edit(Scene* _scene, GameEntity* _entity, const std::string& _parentLabel) override;
    void Debug(Scene* _scene, GameEntity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

    float CalculateSpeedByGearLevel(int32_t _gearLevel) const;
    float CalculateCoolTimeByGearLevel(int32_t _gearLevel) const;

    void UpdateAccel(PlayerInput* _input, Transform* _transform, Rigidbody* _rigidbody, const Quaternion& _cameraRotation);

private:
    EnumBitmask<PlayerMoveState> moveState_           = PlayerMoveState::IDLE; // 現在の移動状態
    EnumBitmask<PlayerMoveState> prevPlayerMoveState_ = PlayerMoveState::IDLE; // 前の移動状態

    std::shared_ptr<IPlayerMoveState> playerMoveState_ = nullptr; // 現在の移動状態を管理するクラス

    /// ==========================================
    // プレイヤーの状態を表す変数群
    bool onGround_          = false; // 地面にいるかどうか
    bool isGearUp_          = false; // ギアアップ中かどうか
    bool collisionWithWall_ = false; // 壁に衝突しているかどうか
    bool isGoal_            = false; // ゴールに到達したかどうか

    Vec3f wallCollisionNormal_ = Vec3f(0.0f, 0.0f, 0.0f); // 壁との衝突時の法線ベクトル

    int32_t gearLevel_    = 0; // ギアレベル
    int32_t maxGearLevel_ = 6; // 最大ギアレベル

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
    float wallRunRate_           = 0.0f; // 壁走りの速度
    Vec3f wallJumpDirection_     = Vec3f(0.0f, 0.0f, 0.0f); // 壁ジャンプの方向

    // currentMaxSpeed は gearLevel に応じて変化する
    float currentMaxSpeed_     = 0.0f; // 現在の最大速度

    float jumpPower_ = 0.0f; // ジャンプのパワー
    float fallPower_ = 0.0f; // 落下のパワー

    float directionInterpolateRate_ = 0.1f;

public:
    PlayerMoveState getState() const {
        return moveState_.toEnum();
    }
    void setState(const PlayerMoveState& _state) {
        moveState_ = _state;
    }

    PlayerMoveState getPrevState() const {
        return prevPlayerMoveState_.toEnum();
    }
    void setPrevState(const PlayerMoveState& _prevState) {
        prevPlayerMoveState_ = _prevState;
    }

    std::shared_ptr<IPlayerMoveState> getPlayerMoveState() const {
        return playerMoveState_;
    }
    void setPlayerMoveState(std::shared_ptr<IPlayerMoveState> _playerMoveState) {
        playerMoveState_ = _playerMoveState;
    }

    bool isGoal() const {
        return isGoal_;
    }
    void setGoal(bool _isGoal) {
        isGoal_ = _isGoal;
    }

    bool isOnGround() const {
        return onGround_;
    }
    void setOnGround(bool _onGround) {
        onGround_ = _onGround;
    }

    bool isCollisionWithWall() const {
        return collisionWithWall_;
    }
    void setCollisionWithWall(bool _collisionWithWall, const Vec3f& _wallCollisionNormal) {
        collisionWithWall_   = _collisionWithWall;
        wallCollisionNormal_ = _wallCollisionNormal;
    }
    void setCollisionWithWall(bool _collisionWithWall) {
        collisionWithWall_ = _collisionWithWall;
    }
    const Vec3f& getWallCollisionNormal() const {
        return wallCollisionNormal_;
    }

    bool isGearUp() const {
        return isGearUp_;
    }
    void setGearUp(bool _isGearUp) {
        isGearUp_ = _isGearUp;
    }

    int32_t getGearLevel() const {
        return gearLevel_;
    }
    void setGearLevel(int32_t _gearLevel) {
        gearLevel_ = _gearLevel;
    }
    int32_t getMaxGearLevel() const {
        return maxGearLevel_;
    }

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

    const Vec3f& getWallJumpDirection() const {
        return wallJumpDirection_;
    }
    void setWallJumpDirection(const Vec3f& _wallJumpDirection) {
        wallJumpDirection_ = _wallJumpDirection;
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
