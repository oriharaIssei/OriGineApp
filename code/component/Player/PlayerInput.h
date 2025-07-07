#pragma once
/// parent
#include "component/IComponent.h"

/// stl
#include <vector>

/// engine
// lib
#include "input/Input.h"

/// math
#include "math/Vector2.h"
#include "math/Vector3.h"

/// <summary>
/// プレイヤーの入力を管理するクラス
/// </summary>
class PlayerInput
    : public IComponent {
    friend void to_json(nlohmann::json& j, const PlayerInput& _playerInput);
    friend void from_json(const nlohmann::json& j, PlayerInput& _playerInput);

public:
    PlayerInput() : IComponent() {}
    ~PlayerInput() {}

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Finalize() override;

private:
    Vec2f inputDirection_ = {0.0f, 0.0f};

    bool isJumpInput_    = false; // ジャンプ入力があったかどうか
    float jumpInputTime_ = 0.0f; // ジャンプ入力の時間 (これによってジャンプ力が変わる)
    float maxJumpTime_   = 0.1f; // ジャンプ入力の最大時間 (秒単位)

    /// <summary>
    /// 移動 キー (ゲームパッドは スティックだから必要なし)
    /// </summary>
    std::vector<Key> moveFrontKeys_ = {
        Key::W,
        Key::UP};
    std::vector<Key> moveBackKeys_ = {
        Key::S,
        Key::DOWN};
    std::vector<Key> moveLeftKeys_ = {
        Key::A,
        Key::LEFT};
    std::vector<Key> moveRightKeys_ = {
        Key::D,
        Key::RIGHT};

    std::vector<Key> jumpKeys_ = {
        Key::SPACE,
        Key::NUMPAD0};
    std::vector<PadButton> jumpButton_ = {
        PadButton::A,
        PadButton::B};
    std::vector<Key> slideKeys_ = {
        Key::L_SHIFT,
        Key::L_CTRL};
    std::vector<PadButton> slideButton_ = {
        PadButton::L_SHOULDER,
        PadButton::R_SHOULDER};

public:
    const std::vector<Key>& getMoveFrontKeys() const {
        return moveFrontKeys_;
    }
    const std::vector<Key>& getMoveBackKeys() const {
        return moveBackKeys_;
    }
    const std::vector<Key>& getMoveLeftKeys() const {
        return moveLeftKeys_;
    }
    const std::vector<Key>& getMoveRightKeys() const {
        return moveRightKeys_;
    }

    const std::vector<Key>& getJumpKeys() const {
        return jumpKeys_;
    }

    const std::vector<PadButton>& getJumpButton() const {
        return jumpButton_;
    }

    const std::vector<Key>& getSlideKeys() const {
        return slideKeys_;
    }
    const std::vector<PadButton>& getSlideButton() const {
        return slideButton_;
    }

    const Vec2f& getInputDirection() const {
        return inputDirection_;
    }
    void setInputDirection(const Vec2f& _inputDirection) {
        inputDirection_ = _inputDirection;
    }

    bool isJumpInput() const {
        return isJumpInput_;
    }
    void setJumpInput(bool _isJumpInput) {
        isJumpInput_ = _isJumpInput;
    }

    const float getMaxJumpTime() const {
        return maxJumpTime_;
    }
    float getJumpInputTime() const {
        return jumpInputTime_;
    }
    void setJumpInputTime(float _jumpInputTime) {
        jumpInputTime_ = _jumpInputTime;
    }
};
