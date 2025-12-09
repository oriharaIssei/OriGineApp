#pragma once
/// parent
#include "component/IComponent.h"

/// stl
#include <vector>

/// engine
// lib
#include "input/InputManager.h"

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

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

private:
    Vec2f inputDirection_ = {0.0f, 0.0f};
    // effectに使う
    Vec3f worldInputDirection_ = {0.0f, 0.0f, 0.0f};

    bool isJumpInput_     = false; // ジャンプ入力があったかどうか
    bool isWallJumpInput_ = false; // 壁ジャンプ入力があったかどうか
    float jumpInputTime_  = 0.0f; // ジャンプ入力の時間 (これによってジャンプ力が変わる)
    float maxJumpTime_    = 0.1f; // ジャンプ入力の最大時間 (秒単位)

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
    const std::vector<Key>& GetMoveFrontKeys() const {
        return moveFrontKeys_;
    }
    const std::vector<Key>& GetMoveBackKeys() const {
        return moveBackKeys_;
    }
    const std::vector<Key>& GetMoveLeftKeys() const {
        return moveLeftKeys_;
    }
    const std::vector<Key>& GetMoveRightKeys() const {
        return moveRightKeys_;
    }

    const std::vector<Key>& GetJumpKeys() const {
        return jumpKeys_;
    }

    const std::vector<PadButton>& GetJumpButton() const {
        return jumpButton_;
    }

    const std::vector<Key>& GetSlideKeys() const {
        return slideKeys_;
    }
    const std::vector<PadButton>& GetSlideButton() const {
        return slideButton_;
    }

    const Vec2f& GetInputDirection() const {
        return inputDirection_;
    }
    void SetInputDirection(const Vec2f& _inputDirection) {
        inputDirection_ = _inputDirection;
    }

    const Vec3f& GetWorldInputDirection() const {
        return worldInputDirection_;
    }
    void SetWorldInputDirection(const Vec3f& _worldInputDirection) {
        worldInputDirection_ = _worldInputDirection;
    }

    bool IsJumpInput() const {
        return isJumpInput_;
    }
    void SetJumpInput(bool _IsJumpInput) {
        isJumpInput_ = _IsJumpInput;
    }

    bool IsWallJumpInput() const {
        return isWallJumpInput_;
    }
    void SetWallJumpInput(bool _isWallJumpInput) {
        isWallJumpInput_ = _isWallJumpInput;
    }

    const float GetMaxJumpTime() const {
        return maxJumpTime_;
    }
    float GetJumpInputTime() const {
        return jumpInputTime_;
    }
    void SetJumpInputTime(float _jumpInputTime) {
        jumpInputTime_ = _jumpInputTime;
    }
};

