#pragma once
#include "component/IComponent.h"

class PlayerStatus
    : public IComponent {
public:
    PlayerStatus() : IComponent() {}
    ~PlayerStatus() {}

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;
    void Finalize() override;

public:
    enum class MoveState {
        IDLE,
        MOVE,
        // DASH // 常にダッシュ状態なので Moveと統合 して省略
        JUMP
    };

private:
    /// <summary>
    /// プレイヤーの 移動状態 を表す列挙体
    /// </summary>

    MoveState moveState_ = MoveState::IDLE;
    bool isJump_         = false; // ジャンプ中かどうか
    bool canJump_        = false; // ジャンプ可能かどうか

    /// <summary>
    /// ダッシュ状態を表す構造体
    /// </summary>
    struct DashState {
        int32_t gearLevel_    = 0; // ギアレベル
        float gearUpCoolTime_ = 0.0f; // ギアレベルが上がるまでの時間

        // 基本速度 最終速度は gearLevel_ に応じて変化する
        float baseSpeed_ = 0.0f;
    };

    DashState dashState_;

    float directionInterpolateRate_ = 0.1f;

public:
    const MoveState& getMoveState() const {
        return moveState_;
    }
    void setMoveState(const MoveState& _state) {
        moveState_ = _state;
    }

    float getDirectionInterpolateRate() const {
        return directionInterpolateRate_;
    }

    bool isJump() const {
        return isJump_;
    }
    void setJump(bool _isJump) {
        isJump_ = _isJump;
    }

    bool canJump() const {
        return canJump_;
    }
    void setCanJump(bool _canJump) {
        canJump_ = _canJump;
    }

    const DashState& getDashState() const {
        return dashState_;
    }

    float getBaseSpeed() const {
        return dashState_.baseSpeed_;
    }

    int32_t getGearLevel() const {
        return dashState_.gearLevel_;
    }
    void setGearLevel(int32_t _gearLevel) {
        dashState_.gearLevel_ = _gearLevel;
    }

    float getGearUpCoolTime() const {
        return dashState_.gearUpCoolTime_;
    }
    void setGearUpCoolTime(float _gearUpCoolTime) {
        dashState_.gearUpCoolTime_ = _gearUpCoolTime;
    }
    void minusGearUpCoolTime(float _deltaTime) {
        dashState_.gearUpCoolTime_ -= _deltaTime;
    }
};
