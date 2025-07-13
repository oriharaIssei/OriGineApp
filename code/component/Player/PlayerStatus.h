#pragma once
#include "component/IComponent.h"

/// util
#include "util/EnumBitMask.h"

/// math
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

#pragma region "Playser State"
class PlayerStatus; // 前方宣言
class IPlayerMoveState {
public:
    IPlayerMoveState(int32_t _playerEntityID, PlayerMoveState _state);
    virtual ~IPlayerMoveState();

    virtual void Initialize()             = 0;
    virtual void Update(float _deltaTime) = 0;
    virtual void Finalize()               = 0;

    virtual PlayerMoveState TransitionState() const = 0;

protected:
    int32_t playerEntityID_ = -1; // プレイヤーのエンティティID
private:
    // このクラスが表す移動状態
    PlayerMoveState state_ = PlayerMoveState::IDLE;

public:
    const PlayerMoveState& getState() const {
        return state_;
    }
    void setState(const PlayerMoveState& _state) {
        state_ = _state;
    }
};

class PlayerIdleState : public IPlayerMoveState {
public:
    PlayerIdleState(int32_t _playerEntityID) : IPlayerMoveState(_playerEntityID, PlayerMoveState::IDLE) {}
    ~PlayerIdleState() override {};

    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;
    PlayerMoveState TransitionState() const override;
};

class PlayerDashState
    : public IPlayerMoveState {
public:
    PlayerDashState(int32_t _playerEntityID) : IPlayerMoveState(_playerEntityID, PlayerMoveState::DASH) {}
    ~PlayerDashState() override {};

    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;
    PlayerMoveState TransitionState() const override;
};

class PlayerJumpState
    : public IPlayerMoveState {
public:
    PlayerJumpState(int32_t _playerEntityID) : IPlayerMoveState(_playerEntityID, PlayerMoveState::JUMP) {}
    ~PlayerJumpState() override {};
    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;
    PlayerMoveState TransitionState() const override;

private:
    bool isAnimatedJumpUp_ = false; // アニメーションのジャンプアップを行うかどうか
};

class PlayerFallDownState
    : public IPlayerMoveState {
public:
    PlayerFallDownState(int32_t _playerEntityID) : IPlayerMoveState(_playerEntityID, PlayerMoveState::FALL_DOWN) {}
    ~PlayerFallDownState() override {};
    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;
    PlayerMoveState TransitionState() const override;

private:
};

class PlayerWallRunState
    : public IPlayerMoveState {
public:
    PlayerWallRunState(int32_t _playerEntityID) : IPlayerMoveState(_playerEntityID, PlayerMoveState::DASH) {}
    ~PlayerWallRunState() override {};

    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;

    PlayerMoveState TransitionState() const override;

protected:
    Vec3f prevVelo_            = Vec3f(0.0f, 0.0f, 0.0f); // 前の速度 壁走り前の速度を保存
    float separationGraceTime_ = 0.04f; // オブジェクトが離れていると判定するまでの猶予時間
    float separationdLeftTime_ = 0.0f; // 壁との衝突判定の残り時間
};

class PlayerWallJumpState
    : public IPlayerMoveState {
public:
    PlayerWallJumpState(int32_t _playerEntityID) : IPlayerMoveState(_playerEntityID, PlayerMoveState::WALL_JUMP) {}
    ~PlayerWallJumpState() override {};

    void Initialize() override;
    void Update(float _deltaTime) override;
    void Finalize() override;

    PlayerMoveState TransitionState() const override;

protected:
    Vec3f prevVelo_       = Vec3f(0.0f, 0.0f, 0.0f); // 前の速度 着地後に戻す
    Vec3f velo_           = Vec3f(0.0f, 0.0f, 0.0f); // 壁ジャンプの方向
    float forcedJumpTime_ = 0.6f;
    float leftTime_       = 0.0f; // 壁ジャンプの残り時間
};

// class PlayerSlideState
//     : public IPlayerMoveState {
// public:
//     PlayerSlideState(int32_t _playerEntityID) : IPlayerMoveState(_playerEntityID, PlayerMoveState::SLIDE) {}
//     ~PlayerSlideState() override {};
//     void Initialize() override;
//     void Update(float _deltaTime) override;
//     void Finalize() override;
//     PlayerMoveState TransitionState() const override;
// };
#pragma endregion

class PlayerStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const PlayerStatus& _playerStatus);
    friend void from_json(const nlohmann::json& j, PlayerStatus& _playerStatus);

public:
    PlayerStatus() : IComponent() {}
    ~PlayerStatus() {}

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Debug() override;
    void Finalize() override;

    float CalculateSpeedByGearLevel(int32_t _gearLevel) const;
    float CalculateCoolTimeByGearLevel(int32_t _gearLevel) const;

private:
    EnumBitmask<PlayerMoveState> moveState_           = PlayerMoveState::IDLE; // 現在の移動状態
    EnumBitmask<PlayerMoveState> prevPlayerMoveState_ = PlayerMoveState::IDLE; // 前の移動状態

    std::shared_ptr<IPlayerMoveState> playerMoveState_ = nullptr; // 現在の移動状態を管理するクラス

    /// ==========================================
    // プレイヤーの状態を表す変数群
    bool onGround_ = false; // 地面にいるかどうか

    bool collisionWithWall_    = false; // 壁に衝突しているかどうか
    Vec3f wallCollisionNormal_ = Vec3f(0.0f, 0.0f, 0.0f); // 壁との衝突時の法線ベクトル

    bool isGearUp_        = false; // ギアアップ中かどうか
    int32_t gearLevel_    = 0; // ギアレベル
    int32_t maxGearLevel_ = 6; // 最大ギアレベル

    bool isGoal_ = false; // ゴールに到達したかどうか

    /// ==========================================
    // 能力値
    float baseGearupCoolTime_        = 1.0f; // ギアアップの基本クールタイム (秒単位)
    float gearUpCoolTime_            = 0.0f; // ギアレベルが上がるまでの時間
    float coolTimeAddRateBase_       = 1.0f;
    float coolTimeAddRateCommonRate_ = 1.f;

    // 基本速度
    float baseSpeed_             = 0.0f; // 基本速度 (ギアレベル0の時の速度)
    float speedUpRateBase_       = 1.0f; // ギアアップ時の速度上昇率の基本値
    float speedUpRateCommonRate_ = 1.f; // ギアアップ時の速度上昇率の共通値
    float wallRunRate_           = 0.0f; // 壁走りの速度
    Vec3f wallJumpDirection_     = Vec3f(0.0f, 0.0f, 0.0f); // 壁ジャンプの方向

    // 現在の速度は gearLevel_ に応じて変化する
    float currentSpeed_ = 0.0f; // 現在の速度

    float jumpPower_ = 0.0f; // ジャンプのパワー

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

    float getCurrentSpeed() const {
        return currentSpeed_;
    }
    void setCurrentSpeed(float _currentSpeed) {
        currentSpeed_ = _currentSpeed;
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
