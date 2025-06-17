#include "PlayerStatus.h"

/// engine
#include "ECSManager.h"
// component
#include "component/physics/Rigidbody.h"
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"

/// application
// component
#include "component/Player/PlayerInput.h"

/// externals
#ifdef _DEBUG
#include "myGui/MyGui.h"
#include <imgui/imgui.h>
#endif // _DEBUG

/// math
#include <math/Quaternion.h>

#pragma region "Player State"
IPlayerMoveState::IPlayerMoveState(int32_t _playerEntityID, PlayerMoveState _state) : playerEntityID_(_playerEntityID), state_(_state) {}
IPlayerMoveState::~IPlayerMoveState() {}

/// ====================================================================================
// IDLE
/// ====================================================================================

void PlayerIdleState::Initialize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto playerStatus  = getComponent<PlayerStatus>(playerEntity);
    playerStatus->setCurrentSpeed(0.0f);
    playerStatus->setGearUpCoolTime(playerStatus->getBaseGearupCoolTime());
    playerStatus->setGearLevel(0);
}

void PlayerIdleState::Update(float /*_deltaTime*/) {
}

void PlayerIdleState::Finalize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto playerStatus  = getComponent<PlayerStatus>(playerEntity);
    playerStatus->setCurrentSpeed(playerStatus->getBaseSpeed());
}

PlayerMoveState PlayerIdleState::TransitionState() const {
    auto* playerEntity = getEntity(playerEntityID_);
    auto playerStatus  = getComponent<PlayerStatus>(playerEntity);
    auto playerInput   = getComponent<PlayerInput>(playerEntity);

    if (playerStatus->isOnGround()) {
        if (playerInput->isJumpInput()) {
            return PlayerMoveState::JUMP;
        }
    }

    if (playerInput->getInputDirection().lengthSq() > 0.f) {
        return PlayerMoveState::DASH;
    }

    return PlayerMoveState::IDLE;
}

/// ====================================================================================

/// ====================================================================================
// DASH
/// ====================================================================================

void PlayerDashState::Initialize() {
}

void PlayerDashState::Update(float _deltaTime) {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);
    auto* playerInput  = getComponent<PlayerInput>(playerEntity);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    auto* transform    = getComponent<Transform>(playerEntity);

    /// ---------------------------------------------------------------------
    // gearLevel の更新
    playerStatus->minusGearUpCoolTime(_deltaTime);

    if (playerStatus->getGearUpCoolTime() <= 0.f) {
        playerStatus->setGearUp(true);

        playerStatus->setGearLevel(playerStatus->getGearLevel() + 1);
        playerStatus->setGearUpCoolTime(playerStatus->getBaseGearupCoolTime() + (playerStatus->getGearLevel() * 1.3f));
        playerStatus->setCurrentSpeed(playerStatus->getBaseSpeed() + playerStatus->getBaseSpeed() * (playerStatus->getGearLevel() * 1.6f));
    }

    // 入力方向を取得
    Vec2f inputDirection = playerInput->getInputDirection();

    // カメラの回転を取得
    GameEntity* gameCamera           = getUniqueEntity("GameCamera");
    const Quaternion& cameraRotation = getComponent<CameraTransform>(gameCamera)->rotate;

    // カメラの回転からヨー（y軸回転）だけを抽出
    float cameraYaw              = cameraRotation.ToEulerAngles()[Y]; // y成分のみ
    Quaternion cameraYawRotation = Quaternion::RotateAxisAngle(Vec3f(0.f, 1.f, 0.f), cameraYaw);

    // 入力方向の回転
    float inputAngle         = std::atan2(inputDirection[X], inputDirection[Y]);
    Quaternion inputRotation = Quaternion::RotateAxisAngle(Vec3f(0.f, 1.f, 0.f), inputAngle);

    // y軸のみの回転合成
    Quaternion targetRotation = Quaternion::Normalize(cameraYawRotation * inputRotation);

    // プレイヤーの回転を補間して設定
    transform->rotate = Slerp(transform->rotate, targetRotation, playerStatus->getDirectionInterpolateRate());

    // 移動方向を回転
    Vec3f movementDirection = Vec3f(0.f, 0.f, 1.f) * MakeMatrix::RotateQuaternion(transform->rotate);
    Vec3f velo              = (playerStatus->getCurrentSpeed() * _deltaTime) * movementDirection;
    if (playerStatus->isOnGround()) {
        // 地面にいる場合はY軸の速度を0にする
        velo[Y] = 0.f;
    } else {
        // 空中にいる場合はY軸の速度を保持
        velo[Y] = rigidbody->getVelocity()[Y];
    }

    rigidbody->setVelocity(velo);
}

void PlayerDashState::Finalize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    rigidbody->setVelocity(Vec3f(0.f, 0.f, 0.f)); // ダッシュ終了時に速度をリセット
}

PlayerMoveState PlayerDashState::TransitionState() const {
    auto* playerEntity = getEntity(playerEntityID_);
    auto playerStatus  = getComponent<PlayerStatus>(playerEntity);
    auto playerInput   = getComponent<PlayerInput>(playerEntity);

    if (playerInput->getInputDirection().lengthSq() <= 0.f) {
        return PlayerMoveState::IDLE;
    }

    if (playerStatus->isOnGround()) {
        if (playerInput->isJumpInput()) {
            return PlayerMoveState::JUMP;
        }
    } else {
        // 空中にいる場合はジャンプ状態に遷移
        return PlayerMoveState::JUMP;
    }

    return PlayerMoveState::DASH;
}

/// ====================================================================================

/// ====================================================================================
// JUMP
/// ====================================================================================

void PlayerJumpState::Initialize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);

    Vec3f velocity = rigidbody->getVelocity(); // ジャンプ中はY軸の速度を保持
    velocity[Y]    = playerStatus->getJumpPower(); // ジャンプパワーをY軸に設定
    rigidbody->setVelocity(velocity);
}

void PlayerJumpState::Update(float _deltaTime) {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);
    auto* playerInput  = getComponent<PlayerInput>(playerEntity);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    auto* transform    = getComponent<Transform>(playerEntity);

    // 入力方向を取得
    Vec2f inputDirection = playerInput->getInputDirection();

    // カメラの回転を取得
    GameEntity* gameCamera           = getUniqueEntity("GameCamera");
    const Quaternion& cameraRotation = getComponent<CameraTransform>(gameCamera)->rotate;

    // カメラの回転からヨー（y軸回転）だけを抽出
    float cameraYaw              = cameraRotation.ToEulerAngles()[Y]; // y成分のみ
    Quaternion cameraYawRotation = Quaternion::RotateAxisAngle(Vec3f(0.f, 1.f, 0.f), cameraYaw);

    // 入力方向の回転
    float inputAngle         = std::atan2(inputDirection[X], inputDirection[Y]);
    Quaternion inputRotation = Quaternion::RotateAxisAngle(Vec3f(0.f, 1.f, 0.f), inputAngle);

    // y軸のみの回転合成
    Quaternion targetRotation = Quaternion::Normalize(cameraYawRotation * inputRotation);

    // プレイヤーの回転を補間して設定
    transform->rotate = Slerp(transform->rotate, targetRotation, playerStatus->getDirectionInterpolateRate());

    // 移動方向を回転
    Vec3f movementDirection = Vec3f(0.f, 0.f, 1.f) * MakeMatrix::RotateQuaternion(transform->rotate);
    Vec3f velocity          = (playerStatus->getCurrentSpeed() * _deltaTime) * movementDirection;
    velocity[Y]             = rigidbody->getVelocity()[Y]; // ジャンプ中はY軸の速度を保持
    rigidbody->setVelocity(velocity);
}

void PlayerJumpState::Finalize() {}

PlayerMoveState PlayerJumpState::TransitionState() const {
    auto* playerEntity = getEntity(playerEntityID_);
    auto playerStatus  = getComponent<PlayerStatus>(playerEntity);
    auto playerInput   = getComponent<PlayerInput>(playerEntity);

    if (playerStatus->isOnGround()) {
        if (playerInput->getInputDirection().lengthSq() > 0.f) {
            return PlayerMoveState::DASH;
        }
        return PlayerMoveState::IDLE;
    }

    return PlayerMoveState::JUMP;
}

/// ====================================================================================

#pragma endregion

#pragma region "Player Status"

void PlayerStatus::Initialize(GameEntity* _entity) {
    playerMoveState_ = std::make_shared<PlayerIdleState>(_entity->getID());
}

bool PlayerStatus::Edit() {
#ifdef _DEBUG

    bool isChange = false;

    isChange |= DragGuiCommand("baseSpeed", baseSpeed_);
    isChange |= DragGuiCommand("jumpPower", jumpPower_);
    isChange |= DragGuiCommand("gearUpCoolTime", baseGearupCoolTime_);
    isChange |= DragGuiCommand("directionInterpolateRate", directionInterpolateRate_);

    return isChange;
#else
    return false;
#endif // _DEBUG
}

void PlayerStatus::Debug() {
#ifdef _DEBUG

    static std::map<PlayerMoveState, const char*> moveStateName = {
        {PlayerMoveState::IDLE, "IDLE"},
        {PlayerMoveState::DASH, "DASH"},
        {PlayerMoveState::JUMP, "JUMP"}
        // {PlayerMoveState::SLIDE, "SLIDE"}
    };

    ImGui::Text("MoveState: %s", moveStateName[moveState_.toEnum()]);
    ImGui::Text("Gear Level: %d", gearLevel_);
    ImGui::Spacing();
    ImGui::Text("Base Gear Up Cool Time: %.2f", baseGearupCoolTime_);
    ImGui::Text("Gear Up Cool Time: %.2f", gearUpCoolTime_);
    ImGui::Spacing();
    ImGui::Text("Base Speed: %.2f", baseSpeed_);
    ImGui::Text("Current Speed: %.2f", currentSpeed_);
    ImGui::Text("Direction Interpolate Rate: %.2f", directionInterpolateRate_);

#endif
}

void PlayerStatus::Finalize() {}

void to_json(nlohmann::json& j, const PlayerStatus& _playerStatus) {
    j["baseSpeed"]                = _playerStatus.baseSpeed_;
    j["jumpPower"]                = _playerStatus.jumpPower_;
    j["gearUpCoolTime"]           = _playerStatus.baseGearupCoolTime_;
    j["directionInterpolateRate"] = _playerStatus.directionInterpolateRate_;
}
void from_json(const nlohmann::json& j, PlayerStatus& _playerStatus) {
    j.at("baseSpeed").get_to(_playerStatus.baseSpeed_);
    if (j.find("jumpPower") != j.end()) {
        j.at("jumpPower").get_to(_playerStatus.jumpPower_);
    }
    j.at("gearUpCoolTime").get_to(_playerStatus.baseGearupCoolTime_);
    j.at("directionInterpolateRate").get_to(_playerStatus.directionInterpolateRate_);
}
#pragma endregion
