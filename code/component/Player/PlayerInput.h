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
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& j, const PlayerInput& _playerInput);
    friend void from_json(const nlohmann::json& j, PlayerInput& _playerInput);

public:
    PlayerInput() : IComponent() {}
    ~PlayerInput() {}

    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;
    void Finalize() override;

    bool IsInputDisabled() const {
        return inputDisableTimer_ < inputDisableDuration_;
    }

private:
    OriGine::Vec2f inputDirection_ = {0.0f, 0.0f};
    // effectに使う
    OriGine::Vec3f worldInputDirection_ = {0.0f, 0.0f, 0.0f};

    bool isJumpInput_     = false; // ジャンプ入力があったかどうか
    bool isWallJumpInput_ = false; // 壁ジャンプ入力があったかどうか
    float jumpInputTime_  = 0.0f; // ジャンプ入力の時間 (これによってジャンプ力が変わる)
    float maxJumpTime_    = 0.1f; // ジャンプ入力の最大時間 (秒単位)

    float inputDisableDuration_ = 0.0f; // 入力無効時間 (秒単位)
    float inputDisableTimer_    = 0.0f; // 入力無効タイマー (秒単位)

    /// <summary>
    /// 移動 キー (ゲームパッドは スティックだから必要なし)
    /// </summary>
    std::vector<OriGine::Key> moveFrontKeys_ = {
        OriGine::Key::W,
        OriGine::Key::UP};
    std::vector<OriGine::Key> moveBackKeys_ = {
        OriGine::Key::S,
        OriGine::Key::DOWN};
    std::vector<OriGine::Key> moveLeftKeys_ = {
        OriGine::Key::A,
        OriGine::Key::LEFT};
    std::vector<OriGine::Key> moveRightKeys_ = {
        OriGine::Key::D,
        OriGine::Key::RIGHT};

    std::vector<OriGine::Key> jumpKeys_ = {
        OriGine::Key::SPACE,
        OriGine::Key::NUMPAD0};
    std::vector<OriGine::PadButton> jumpButton_ = {
        OriGine::PadButton::A,
        OriGine::PadButton::B};

public:
    const std::vector<OriGine::Key>& GetMoveFrontKeys() const {
        return moveFrontKeys_;
    }
    const std::vector<OriGine::Key>& GetMoveBackKeys() const {
        return moveBackKeys_;
    }
    const std::vector<OriGine::Key>& GetMoveLeftKeys() const {
        return moveLeftKeys_;
    }
    const std::vector<OriGine::Key>& GetMoveRightKeys() const {
        return moveRightKeys_;
    }

    const std::vector<OriGine::Key>& GetJumpKeys() const {
        return jumpKeys_;
    }

    const std::vector<OriGine::PadButton>& GetJumpButton() const {
        return jumpButton_;
    }

    const OriGine::Vec2f& GetInputDirection() const {
        return inputDirection_;
    }
    void SetInputDirection(const OriGine::Vec2f& _inputDirection) {
        inputDirection_ = _inputDirection;
    }

    const OriGine::Vec3f& GetWorldInputDirection() const {
        return worldInputDirection_;
    }
    void SetWorldInputDirection(const OriGine::Vec3f& _worldInputDirection) {
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

    float GetInputDisableDuration() const {
        return inputDisableDuration_;
    }
    float GetInputDisableTimer() const {
        return inputDisableTimer_;
    }
    void SetInputDisableDuration(float _inputDisableDuration) {
        inputDisableDuration_ = _inputDisableDuration;
    }
    void SetInputDisableTimer(float _inputDisableTimer) {
        inputDisableTimer_ = _inputDisableTimer;
    }
    /// <summary>
    /// 入力無効タイマーを減算
    /// </summary>
    /// <param name="_deltaTime"></param>
    void SubInputDisableTimer(float _deltaTime) {
        inputDisableTimer_ -= _deltaTime;
    }
};
