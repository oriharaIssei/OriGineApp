#pragma once
#include "component/IComponent.h"

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

public:
    /// <summary>
    /// プレイヤーの 移動状態 を表す列挙体
    /// </summary>
    enum class MoveState {
        IDLE, // 待機 (動いていない)
        DASH, // ダッシュ(基本移動)
        JUMP, // ジャンプ
        SLIDE // スライディング
    };

private:
    MoveState moveState_     = MoveState::IDLE;
    MoveState prevMoveState_ = MoveState::IDLE; // 前の移動状態

    bool onGround_ = true; // 地面にいるかどうか

    int32_t gearLevel_        = 0; // ギアレベル
    float baseGearupCoolTime_ = 1.0f; // ギアアップの基本クールタイム (秒単位)
    float gearUpCoolTime_     = 0.0f; // ギアレベルが上がるまでの時間

    // 基本速度 最終速度は gearLevel_ に応じて変化する
    float baseSpeed_ = 0.0f;
    float currentSpeed_;

    bool isGearUp_ = false; // ギアアップ中かどうか

    float directionInterpolateRate_ = 0.1f;

public:
    const MoveState& getState() const {
        return moveState_;
    }
    void setState(const MoveState& _state) {
        moveState_ = _state;
    }

    const MoveState& getPrevState() const {
        return prevMoveState_;
    }
    void setPrevState(const MoveState& _prevState) {
        prevMoveState_ = _prevState;
    }

    float getDirectionInterpolateRate() const {
        return directionInterpolateRate_;
    }

    bool isGearUp() const {
        return isGearUp_;
    }
    void setGearUp(bool _isGearUp) {
        isGearUp_ = _isGearUp;
    }

    float getBaseSpeed() const {
        return baseSpeed_;
    }

    float getCurrentSpeed() const {
        return currentSpeed_;
    }
    void setCurrentSpeed(float _currentSpeed) {
        currentSpeed_ = _currentSpeed;
    }

    int32_t getGearLevel() const {
        return gearLevel_;
    }
    void setGearLevel(int32_t _gearLevel) {
        gearLevel_ = _gearLevel;
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
