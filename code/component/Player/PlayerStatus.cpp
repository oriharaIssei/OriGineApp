#include "PlayerStatus.h"

/// engine
#include "ECSManager.h"
// component
#include "component/collider/CollisionPushBackInfo.h"
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
#include <math/Sequence.h>

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

    auto* rigidbody = getComponent<Rigidbody>(playerEntity);
    rigidbody->setVelocity(Vec3f(0.f, 0.f, 0.f)); // ダッシュ終了時に速度をリセット
}

void PlayerIdleState::Update(float /*_deltaTime*/) {}

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

void PlayerDashState::Initialize() {}

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

        int32_t addedGearLevel = playerStatus->getGearLevel() + 1;
        playerStatus->setGearLevel(addedGearLevel);

        playerStatus->setGearUpCoolTime(
            GeometricSequence<float>(
                playerStatus->getBaseGearupCoolTime(),
                ArithmeticSequence<float>(playerStatus->getCoolTimeUpRateBase(), playerStatus->getCoolTimeUpRateCommonRate(), addedGearLevel - 1),
                addedGearLevel));

        playerStatus->setCurrentSpeed(
            ArithmeticSequence<float>(
                playerStatus->getBaseSpeed(),
                ArithmeticSequence<float>(playerStatus->getSpeedUpRateBase(), playerStatus->getSpeedUpRateCommonRate(), addedGearLevel - 1),
                addedGearLevel));
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
            //! TODO : JumpInit に書く
            auto* rigidbody = getComponent<Rigidbody>(playerEntity);

            Vec3f velocity = rigidbody->getVelocity(); // ジャンプ中はY軸の速度を保持
            velocity[Y]    = playerStatus->getJumpPower(); // ジャンプパワーをY軸に設定
            rigidbody->setVelocity(velocity);
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

void PlayerJumpState::Initialize() {}

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
    // ジャンプ中は速度が落ちない,止まらない
    Vec3f movementDirection = Vec3f(0.f, 0.f, 1.f) * MakeMatrix::RotateQuaternion(transform->rotate);

    Vec3f oldVelo   = rigidbody->getVelocity();
    Vec2f oldXZVelo = {oldVelo[X], oldVelo[Z]};

    Vec3f velocity = {0.f, 0.f, 0.f};
    if (oldXZVelo.length() > 0.f) {
        velocity = (playerStatus->getCurrentSpeed() * _deltaTime) * movementDirection;
    }
    velocity[Y] = oldVelo[Y]; // ジャンプ中はY軸の速度を保持
    rigidbody->setVelocity(velocity);
}

void PlayerJumpState::Finalize() {}

PlayerMoveState PlayerJumpState::TransitionState() const {
    auto* playerEntity = getEntity(playerEntityID_);
    auto playerStatus  = getComponent<PlayerStatus>(playerEntity);
    auto playerInput   = getComponent<PlayerInput>(playerEntity);

    if (playerStatus->isOnGround()) {
        if (playerStatus->isCollisionWithWall()) {
            return PlayerMoveState::WALL_RUN;
        } else {
            if (playerInput->getInputDirection().lengthSq() > 0.f) {
                return PlayerMoveState::DASH;
            }
        }

        return PlayerMoveState::IDLE;
    }

    return PlayerMoveState::JUMP;
}

/// ====================================================================================

/// ====================================================================================
// WALL RUN
/// ====================================================================================

void PlayerWallRunState::Initialize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    prevVelo_          = rigidbody->getVelocity(); // 壁ジャンプ前の速度を保存

    rigidbody->setUseGravity(false);

    separationdLeftTime_ = separationGraceTime_; // 壁との衝突判定の残り時間を初期化
}

void PlayerWallRunState::Update(float _deltaTime) {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    auto* transform    = getComponent<Transform>(playerEntity);

    // 移動方向を回転
    Vec3f movementDirection = axisZ * MakeMatrix::RotateQuaternion(transform->rotate);
    Vec3f velo              = (playerStatus->getCurrentSpeed() * playerStatus->getWallRunRate() * _deltaTime) * movementDirection;
    Vec3f normal            = playerStatus->getWallCollisionNormal();
    velo -= normal * 0.02f;

    rigidbody->setVelocity(velo);

    if (playerStatus->isCollisionWithWall()) {
        separationdLeftTime_ = separationGraceTime_;
    } else {
        separationdLeftTime_ -= _deltaTime; // 壁との衝突判定の残り時間を減少
    }
}

void PlayerWallRunState::Finalize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    // 壁走行終了時に速度をリセット
    rigidbody->setVelocity(prevVelo_); // 壁走行終了時に速度をリセット

    auto playerStatus = getComponent<PlayerStatus>(playerEntity);
    playerStatus->setCollisionWithWall(false); // 壁走行終了時に壁との衝突をリセット
}

PlayerMoveState PlayerWallRunState::TransitionState() const {
    auto* playerEntity = getEntity(playerEntityID_);

    if (separationdLeftTime_ <= 0.0f) {
        return PlayerMoveState::DASH;
    }

    auto playerInput = getComponent<PlayerInput>(playerEntity);
    if (playerInput->isJumpInput()) {
        return PlayerMoveState::WALL_JUMP;
    }

    return PlayerMoveState::WALL_RUN;
}

/// ====================================================================================

/// ====================================================================================
// WALL JUMP
/// ====================================================================================

void PlayerWallJumpState::Initialize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);

    rigidbody->setAcceleration({0.0f, 0.0f, 0.0f}); // 壁ジャンプ時は加速度をリセット
    rigidbody->setUseGravity(false); // 無効
    prevVelo_ = rigidbody->getVelocity(); // 壁ジャンプ前の速度を保存

    // ジャンプ方向  基本 +z にしか進まないので 一旦固定
    Vec3f wallJumpDirection = playerStatus->getWallJumpDirection();
    wallJumpDirection       = wallJumpDirection.normalize();

    velo_ = wallJumpDirection * (playerStatus->getCurrentSpeed() * playerStatus->getWallRunRate());

    leftTime_ = forcedJumpTime_; // 壁ジャンプの残り時間を初期化
}

void PlayerWallJumpState::Update(float _deltaTime) {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);

    rigidbody->setVelocity(velo_ * _deltaTime); // 壁ジャンプの方向に速度を設定

    leftTime_ -= _deltaTime; // 壁ジャンプの残り時間を減少
}

void PlayerWallJumpState::Finalize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);

    rigidbody->setUseGravity(true); // 重力を有効
    rigidbody->setVelocity(prevVelo_); // 壁ジャンプ終了時に速度をリセット
}

PlayerMoveState PlayerWallJumpState::TransitionState() const {
    auto* playerEntity = getEntity(playerEntityID_);
    auto playerStatus  = getComponent<PlayerStatus>(playerEntity);

    if (playerStatus->isOnGround()) {
        return PlayerMoveState::DASH;
    }
    if (leftTime_ <= 0.0f) {
        return PlayerMoveState::DASH;
    }

    return this->getState();
}

/// ====================================================================================
#pragma endregion

#pragma region "Player Status"

void PlayerStatus::Initialize(GameEntity* _entity) {
    playerMoveState_     = std::make_shared<PlayerIdleState>(_entity->getID());
    moveState_           = PlayerMoveState::IDLE; // 初期状態は IDLE
    prevPlayerMoveState_ = PlayerMoveState::IDLE; // 前の移動状態を初期化

    gearUpCoolTime_ = baseGearupCoolTime_; // ギアアップのクールタイムを初期化
    gearLevel_      = 0; // ギアレベルを初期化
    isGearUp_       = false; // ギアアップ状態を初期化

    currentSpeed_ = baseSpeed_; // 現在の速度を初期化

    onGround_            = true; // 初期状態では地面にいる
    collisionWithWall_   = false; // 初期状態では壁に衝突していない
    wallCollisionNormal_ = Vec3f(0.0f, 0.0f, 0.0f); // 初期状態では壁との衝突がない
}

bool PlayerStatus::Edit() {
#ifdef _DEBUG

    bool isChange = false;

    isChange |= DragGuiCommand("baseSpeed", baseSpeed_);
    isChange |= DragGuiCommand("wallRunRate", wallRunRate_);
    if (DragGuiVectorCommand<3, float>("wallJumpDirection",
            wallJumpDirection_,
            0.01f,
            -1.f, 1.f,
            "%.4f",
            [](Vector<3, float>* _v) {
                *_v = Vec3f(*_v).normalize();
            })) {
        wallJumpDirection_ = wallJumpDirection_.normalize();
    }

    isChange |= DragGuiCommand("speedUpRateBase", speedUpRateBase_);
    isChange |= DragGuiCommand("speedUpRateCommonRate", speedUpRateCommonRate_);
    isChange |= DragGuiCommand("coolTimeAddRateBase", coolTimeAddRateBase_);
    isChange |= DragGuiCommand("coolTimeAddRateCommonRate", coolTimeAddRateCommonRate_);

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
        {PlayerMoveState::JUMP, "JUMP"},
        {PlayerMoveState::WALL_RUN, "WALL_RUN"},
        {PlayerMoveState::WALL_JUMP, "WALL_JUMP"}
        // {PlayerMoveState::SLIDE, "SLIDE"}
    };

    ImGui::Text("MoveState  :  %s", moveStateName[moveState_.toEnum()]);
    ImGui::Text("Gear Level : %d", gearLevel_);
    ImGui::Spacing();
    ImGui::Text("Base Gear Up Cool Time : %.2f", baseGearupCoolTime_);
    ImGui::Text("Gear Up Cool Time      : %.2f", gearUpCoolTime_);
    ImGui::Spacing();
    ImGui::Text("Base Speed          : %.2f", baseSpeed_);
    ImGui::Text("Current Speed       : %.2f", currentSpeed_);
    ImGui::Spacing();
    ImGui::Text("Speed Up Rate Base : %.2f", speedUpRateBase_);
    ImGui::Text("Speed Up Rate Common Rate : %.2f", speedUpRateCommonRate_);
    ImGui::Spacing();
    ImGui::Text("Cool Time Up Rate Base : %.2f", coolTimeAddRateBase_);
    ImGui::Text("Cool Time Up Rate Common Rate : %.2f", coolTimeAddRateCommonRate_);
    ImGui::Spacing();
    ImGui::Text("Wall Run Rate      : %.2f", wallRunRate_);
    ImGui::Text("Wall Jump Direction : (%.2f, %.2f, %.2f)", wallJumpDirection_[X], wallJumpDirection_[Y], wallJumpDirection_[Z]);
    ImGui::Text("Direction Interpolate Rate: %.2f", directionInterpolateRate_);

#endif
}

void PlayerStatus::Finalize() {}

void to_json(nlohmann::json& j, const PlayerStatus& _playerStatus) {
    j["baseSpeed"]                = _playerStatus.baseSpeed_;
    j["wallRunRate"]              = _playerStatus.wallRunRate_;
    j["wallJumpDirection"]        = _playerStatus.wallJumpDirection_;
    j["jumpPower"]                = _playerStatus.jumpPower_;
    j["gearUpCoolTime"]           = _playerStatus.baseGearupCoolTime_;
    j["directionInterpolateRate"] = _playerStatus.directionInterpolateRate_;

    j["speedUpRateBase"]           = _playerStatus.speedUpRateBase_;
    j["speedUpRateCommonRate"]     = _playerStatus.speedUpRateCommonRate_;
    j["coolTimeAddRateBase"]       = _playerStatus.coolTimeAddRateBase_;
    j["coolTimeAddRateCommonRate"] = _playerStatus.coolTimeAddRateCommonRate_;
}
void from_json(const nlohmann::json& j, PlayerStatus& _playerStatus) {
    j.at("baseSpeed").get_to(_playerStatus.baseSpeed_);
    j.at("jumpPower").get_to(_playerStatus.jumpPower_);
    j.at("wallRunRate").get_to(_playerStatus.wallRunRate_);
    j.at("wallJumpDirection").get_to(_playerStatus.wallJumpDirection_);
    j.at("gearUpCoolTime").get_to(_playerStatus.baseGearupCoolTime_);
    j.at("directionInterpolateRate").get_to(_playerStatus.directionInterpolateRate_);

    j.at("speedUpRateBase").get_to(_playerStatus.speedUpRateBase_);
    j.at("speedUpRateCommonRate").get_to(_playerStatus.speedUpRateCommonRate_);
    j.at("coolTimeAddRateBase").get_to(_playerStatus.coolTimeAddRateBase_);
    j.at("coolTimeAddRateCommonRate").get_to(_playerStatus.coolTimeAddRateCommonRate_);
}
#pragma endregion
