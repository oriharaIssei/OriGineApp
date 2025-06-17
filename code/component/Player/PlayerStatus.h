#pragma once
#include "component/IComponent.h"

/// util
#include "util/EnumBitMask.h"

/// <summary>
/// プレイヤーの 移動状態 を表す列挙体
/// </summary>
enum class PlayerMoveState {
    IDLE = 1 << 0, // 待機 (動いていない)
    DASH = 1 << 1, // ダッシュ(基本移動)
    JUMP = 1 << 2, // ジャンプ
    // SLIDE    = 1 << 3, // スライド
    // WALL_RUN = 1 << 4, // ウォールラン

    Count = 3 // 5
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

private:
    EnumBitmask<PlayerMoveState> moveState_           = PlayerMoveState::IDLE; // 現在の移動状態
    EnumBitmask<PlayerMoveState> prevPlayerMoveState_ = PlayerMoveState::IDLE; // 前の移動状態

    std::shared_ptr<IPlayerMoveState> playerMoveState_ = nullptr; // 現在の移動状態を管理するクラス

    /// ==========================================
    // プレイヤーの状態を表す変数群
    bool onGround_ = false; // 地面にいるかどうか

    bool canJump_  = false; // ジャンプ可能かどうか
    bool canSlide_ = false; // スライド可能かどうか

    bool isGearUp_     = false; // ギアアップ中かどうか
    int32_t gearLevel_ = 0; // ギアレベル

    /// ==========================================
    // 能力値
    float baseGearupCoolTime_ = 1.0f; // ギアアップの基本クールタイム (秒単位)
    float gearUpCoolTime_     = 0.0f; // ギアレベルが上がるまでの時間

    // 基本速度
    float baseSpeed_ = 0.0f;
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

    bool isOnGround() const {
        return onGround_;
    }
    void setOnGround(bool _onGround) {
        onGround_ = _onGround;
    }

    bool canJump() const {
        return canJump_;
    }
    void setCanJump(bool _canJump) {
        canJump_ = _canJump;
    }

    bool canSlide() const {
        return canSlide_;
    }
    void setCanSlide(bool _canSlide) {
        canSlide_ = _canSlide;
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
};
