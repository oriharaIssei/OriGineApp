#include "PlayerStatus.h"

/// engine
#include "ECSManager.h"
// component
#include "component/animation/SkinningAnimationComponent.h"
#include "component/collider/CollisionPushBackInfo.h"
#include "component/effect/particle/emitter/Emitter.h"
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
    playerStatus->setGearLevel(kDefaultPlayerGearLevel);

    int32_t emitterSize = ECSManager::getInstance()->getComponentArray<Emitter>()->getComponentSize(playerEntity);

    //! TODO: 速度でパーティクルの量を変える
    for (int32_t i = 0; i < emitterSize; i++) {
        auto* emitter = getComponent<Emitter>(playerEntity, i);
        emitter->PlayStop();
    }
}

void PlayerIdleState::Update(float /*_deltaTime*/) {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    // 減速
    rigidbody->setVelocity(rigidbody->getVelocity() - rigidbody->getVelocity() * 0.8f);
}

void PlayerIdleState::Finalize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto playerStatus  = getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    playerStatus->setCurrentSpeed(playerStatus->getBaseSpeed());
    // Jump がおかしくなるため しっかりと ゼロ にする
    rigidbody->setVelocity({0.0f, 0.0f, 0.0f});
}

PlayerMoveState PlayerIdleState::TransitionState() const {
    auto* playerEntity = getEntity(playerEntityID_);
    auto playerStatus  = getComponent<PlayerStatus>(playerEntity);
    auto playerInput   = getComponent<PlayerInput>(playerEntity);

    if (playerStatus->isOnGround()) {
        if (playerInput->isJumpInput()) {
            return PlayerMoveState::JUMP;
        }
        if (playerInput->getInputDirection().lengthSq() > 0.f) {
            return PlayerMoveState::DASH;
        }

        return PlayerMoveState::IDLE;
    } else {
        // 空中にいる場合はジャンプ状態に遷移
        return PlayerMoveState::FALL_DOWN;
    }
}

/// ====================================================================================

/// ====================================================================================
// DASH
/// ====================================================================================

void PlayerDashState::Initialize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);

    auto* skinningAnim = getComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->setPlaybackSpeed(1, 1.f + float(playerStatus->getGearLevel()));
    }
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

    if (playerStatus->getGearLevel() < playerStatus->getMaxGearLevel()) {
        if (playerStatus->getGearUpCoolTime() <= 0.f) {
            playerStatus->setGearUp(true);

            int32_t addedGearLevel = playerStatus->getGearLevel() + 1;
            playerStatus->setGearLevel(addedGearLevel);

            playerStatus->setGearUpCoolTime(playerStatus->CalculateCoolTimeByGearLevel(addedGearLevel));

            playerStatus->setCurrentSpeed(playerStatus->CalculateSpeedByGearLevel(addedGearLevel));

            auto* skinningAnim = getComponent<SkinningAnimationComponent>(playerEntity);
            if (skinningAnim) {
                skinningAnim->setPlaybackSpeed(1, 1.f + float(playerStatus->getGearLevel()));
            }
        }
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

    Vec3f movementDirection = Vec3f(0.f, 0.f, 1.f) * MakeMatrix::RotateQuaternion(transform->rotate);
    rigidbody->setVelocity(movementDirection * playerStatus->getCurrentSpeed());
}

void PlayerDashState::Finalize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);

    Vec3f velo = rigidbody->getVelocity();

    float limitSpeed = playerStatus->getCurrentSpeed(); // このフレームに追加される速度を引く
    if (velo.lengthSq() >= limitSpeed * limitSpeed) {
        // 速度が速すぎる場合は制限
        velo = velo.normalize() * limitSpeed;
        rigidbody->setVelocity(velo);
    }
    auto* skinningAnim = getComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->setPlaybackSpeed(1, 1.f);
    }
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
        return PlayerMoveState::FALL_DOWN;
    }

    return PlayerMoveState::DASH;
}

/// ====================================================================================

/// ====================================================================================
// FALL DOWN
/// ====================================================================================

void PlayerFallDownState::Initialize() {}

void PlayerFallDownState::Update(float /*_deltaTime*/) {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);
    auto* playerInput  = getComponent<PlayerInput>(playerEntity);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    auto* transform    = getComponent<Transform>(playerEntity);

    // 入力方向を取得
    Vec2f inputDirection = playerInput->getInputDirection();

    if (inputDirection.lengthSq() > 0.f) {
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
    }

    // 移動方向を回転
    // 落下中は 速度が落ちない,止まらない

    Vec3f oldVelo   = rigidbody->getVelocity();
    Vec2f oldXZVelo = {oldVelo[X], oldVelo[Z]};

    Vec3f movementDirection = Vec3f(0.f, 0.f, 1.f) * MakeMatrix::RotateQuaternion(transform->rotate);
    Vec3f velocity          = {0.f, 0.f, 0.f};
    if (oldXZVelo.lengthSq() != 0.f) {
        // 移動方向を回転
        velocity = movementDirection * oldXZVelo.length();
    }

    velocity[Y] = oldVelo[Y]; // 落下中はY軸の速度を保持
    rigidbody->setVelocity(velocity);
}

void PlayerFallDownState::Finalize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);

    rigidbody->setAcceleration({0.0f, 0.0f, 0.0f}); // ダッシュ終了時に加速度をリセット
    Vec3f velo = rigidbody->getVelocity();

    float limitSpeed = playerStatus->getCurrentSpeed(); // このフレームに追加される速度を引く
    if (velo.lengthSq() >= limitSpeed * limitSpeed) {
        // 速度が速すぎる場合は制限
        velo = velo.normalize() * limitSpeed;
        rigidbody->setVelocity(velo);
    }

    auto* skinningAnim = getComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->Play(4); // 差し替え
    }
}

PlayerMoveState PlayerFallDownState::TransitionState() const {
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

    return PlayerMoveState::FALL_DOWN;
}

/// ====================================================================================

/// ====================================================================================
// JUMP
/// ====================================================================================

void PlayerJumpState::Initialize() {
    releaseJumpPower_ = 0.f;

    auto* playerEntity = getEntity(playerEntityID_);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);

    rigidbody->setUseGravity(false);

    rigidbody->setVelocity(Y, playerStatus->getJumpPower()); // ジャンプパワーをY軸に設定

    auto* skinningAnim = getComponent<SkinningAnimationComponent>(playerEntity);
    if (skinningAnim) {
        skinningAnim->endTransition();
        skinningAnim->Play(2);
    }
}

void PlayerJumpState::Update(float /*_deltaTime*/) {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* playerInput  = getComponent<PlayerInput>(playerEntity);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    auto* transform    = getComponent<Transform>(playerEntity);

    // 入力方向を取得
    Vec2f inputDirection = playerInput->getInputDirection();

    if (inputDirection.lengthSq() > 0.f) {
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
    }

    // 移動方向を回転
    // ジャンプ中は速度が落ちない,止まらない

    Vec3f oldVelo   = rigidbody->getVelocity();
    Vec2f oldXZVelo = {oldVelo[X], oldVelo[Z]};

    Vec3f movementDirection = Vec3f(0.f, 0.f, 1.f) * MakeMatrix::RotateQuaternion(transform->rotate);
    Vec3f velocity          = {0.f, 0.f, 0.f};
    if (oldXZVelo.lengthSq() != 0.f) {
        velocity = movementDirection * playerStatus->getCurrentSpeed(); // 移動方向を回転
    }
    velocity[Y] = oldVelo[Y]; // ジャンプ中はY軸の速度を保持

    rigidbody->setVelocity(velocity);

    releaseJumpPower_ += playerStatus->getFallPower();
}

void PlayerJumpState::Finalize() {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    auto* skinningAnim = getComponent<SkinningAnimationComponent>(playerEntity);

    rigidbody->setUseGravity(true);

    rigidbody->setVelocity(Y, releaseJumpPower_);

    if (skinningAnim) {
        skinningAnim->Play(4); // 差し替え
    }
}

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
    } else {
        if (!playerInput->isJumpInput()) {
            // ジャンプ入力がない場合は落下状態に遷移
            return PlayerMoveState::FALL_DOWN;
        }
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

    separationdLeftTime_ = separationGraceTime_; // 壁との衝突判定の残り時間を初期化
}

void PlayerWallRunState::Update(float _deltaTime) {
    auto* playerEntity = getEntity(playerEntityID_);
    auto* playerStatus = getComponent<PlayerStatus>(playerEntity);
    auto* rigidbody    = getComponent<Rigidbody>(playerEntity);
    auto* transform    = getComponent<Transform>(playerEntity);

    // 移動方向を回転
    Vec3f movementDirection = axisZ * MakeMatrix::RotateQuaternion(transform->rotate);
    Vec3f velo              = (playerStatus->getCurrentSpeed() * playerStatus->getWallRunRate()) * movementDirection;
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
        return PlayerMoveState::FALL_DOWN;
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

    rigidbody->setVelocity(velo_); // 壁ジャンプの方向に速度を設定

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
    gearLevel_      = kDefaultPlayerGearLevel; // ギアレベルを初期化
    isGearUp_       = false; // ギアアップ状態を初期化

    currentSpeed_ = baseSpeed_; // 現在の速度を初期化

    onGround_            = true; // 初期状態では地面にいる
    collisionWithWall_   = false; // 初期状態では壁に衝突していない
    wallCollisionNormal_ = Vec3f(0.0f, 0.0f, 0.0f); // 初期状態では壁との衝突がない
}

bool PlayerStatus::Edit() {
#ifdef _DEBUG

    bool isChange = false;

    if (ImGui::TreeNode("Speed")) {
        isChange |= DragGuiCommand("baseSpeed", baseSpeed_);
        isChange |= DragGuiCommand("speedUpRateBase", speedUpRateBase_);
        isChange |= DragGuiCommand("speedUpRateCommonRate", speedUpRateCommonRate_);
        if (ImGui::BeginTable("SpeedByGearLevel", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Gear Level");
            ImGui::TableSetupColumn("Speed");
            ImGui::TableHeadersRow();
            for (int level = 1; level <= maxGearLevel_; ++level) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", level);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f", CalculateSpeedByGearLevel(level));
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("CollTime")) {
        isChange |= DragGuiCommand("gearUpCoolTime", baseGearupCoolTime_);
        isChange |= DragGuiCommand("coolTimeAddRateBase", coolTimeAddRateBase_);
        isChange |= DragGuiCommand("coolTimeAddRateCommonRate", coolTimeAddRateCommonRate_);
        if (ImGui::BeginTable("CoolTimeByGearLevel", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Gear Level");
            ImGui::TableSetupColumn("CoolTime");
            ImGui::TableHeadersRow();
            for (int level = 1; level <= maxGearLevel_; ++level) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", level);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f", CalculateCoolTimeByGearLevel(level));
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    isChange |= DragGuiCommand("directionInterpolateRate", directionInterpolateRate_);
    isChange |= DragGuiCommand("jumpPower", jumpPower_);
    isChange |= DragGuiCommand("fallPower", fallPower_);
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

    ImGui::Text("MoveState  : %s", moveStateName[moveState_.toEnum()]);
    ImGui::Text("Gear Level : %d", gearLevel_);
    ImGui::Spacing();
    ImGui::Text("Base Gear Up Cool Time : %.2f", baseGearupCoolTime_);
    ImGui::Text("Gear Up Cool Time      : %.2f", gearUpCoolTime_);
    ImGui::Spacing();
    ImGui::Text("Base Speed          : %.2f", baseSpeed_);
    ImGui::Text("Current Speed       : %.2f", currentSpeed_);
    ImGui::Text("Jump Power          : %.2f", jumpPower_);
    ImGui::Text("Fall Power          : %.2f", fallPower_);

    ImGui::Text("Is Gear Up          : %s", isGearUp_ ? "true" : "false");
    ImGui::Spacing();
    ImGui::Text("Speed Up Rate Base        : %.2f", speedUpRateBase_);
    ImGui::Text("Speed Up Rate Common Rate : %.2f", speedUpRateCommonRate_);
    ImGui::Spacing();
    ImGui::Text("Cool Time Up Rate Base        : %.2f", coolTimeAddRateBase_);
    ImGui::Text("Cool Time Up Rate Common Rate : %.2f", coolTimeAddRateCommonRate_);
    ImGui::Spacing();
    ImGui::Text("Wall Run Rate             : %.2f", wallRunRate_);
    ImGui::Text("Wall Jump Direction       : (%.2f, %.2f, %.2f)", wallJumpDirection_[X], wallJumpDirection_[Y], wallJumpDirection_[Z]);
    ImGui::Text("Direction Interpolate Rate: %.2f", directionInterpolateRate_);

#endif
}

void PlayerStatus::Finalize() {}

float PlayerStatus::CalculateSpeedByGearLevel(int32_t _gearLevel) const {
    return ArithmeticSequence<float>(
        baseSpeed_,
        ArithmeticSequence<float>(speedUpRateBase_, speedUpRateCommonRate_, _gearLevel - 1),
        _gearLevel);
}

float PlayerStatus::CalculateCoolTimeByGearLevel(int32_t _gearLevel) const {
    return GeometricSequence<float>(
        baseGearupCoolTime_,
        ArithmeticSequence<float>(coolTimeAddRateBase_, coolTimeAddRateCommonRate_, _gearLevel - 1),
        _gearLevel);
}

void to_json(nlohmann::json& j, const PlayerStatus& _playerStatus) {
    j["baseSpeed"]                = _playerStatus.baseSpeed_;
    j["wallRunRate"]              = _playerStatus.wallRunRate_;
    j["wallJumpDirection"]        = _playerStatus.wallJumpDirection_;
    j["jumpPower"]                = _playerStatus.jumpPower_;
    j["fallPower"]                = _playerStatus.fallPower_;
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
    j.at("fallPower").get_to(_playerStatus.fallPower_);
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
