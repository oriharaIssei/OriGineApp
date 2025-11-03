#include "PlayerUpdateOnTitle.h"

/// engine
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/physics/Rigidbody.h"
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"
#include "component/Player/State/PlayerDashState.h"
#include "component/Player/State/PlayerState.h"
#include "component/transform/Transform.h"

/// math
#include "math/Interpolation.h"
#include "math/mathEnv.h"

PlayerUpdateOnTitle::PlayerUpdateOnTitle() : ISystem(SystemCategory::Movement) {}
PlayerUpdateOnTitle::~PlayerUpdateOnTitle() {}

void PlayerUpdateOnTitle::Initialize() {
    // 特に初期化処理は不要
}
void PlayerUpdateOnTitle::Finalize() {
    // 特に終了処理は不要
}

void PlayerUpdateOnTitle::UpdateEntity(Entity* _entity) {
    auto* playerInput = getComponent<PlayerInput>(_entity);
    if (playerInput == nullptr) {
        LOG_ERROR("PlayerInput component is missing.");
        return;
    }
    auto* playerState = getComponent<PlayerState>(_entity);
    if (playerState == nullptr) {
        LOG_ERROR("PlayerState component is missing.");
        return;
    }
    auto* playerStatus = getComponent<PlayerStatus>(_entity);
    if (playerStatus == nullptr) {
        LOG_ERROR("PlayerStatus component is missing.");
        return;
    }

    // ジャンプ入力を無効化
    playerInput->setJumpInput(false);

    // タイトル画面では常に地面に接地している状態にする
    playerState->OnCollisionGround(-1);

    // 更新処理 ほぼ PlayerDashState と同じ
    float deltaTime = getMainDeltaTime();
    auto* transform = getComponent<Transform>(_entity);
    auto* rigidbody = getComponent<Rigidbody>(_entity);

    playerStatus->minusGearUpCoolTime(deltaTime);

    int32_t gearLevel = playerState->getGearLevel();
    // ギアレベルが最大に達していない場合、
    if (gearLevel < kMaxPlayerGearLevel - 1) {
        // クールタイムが0以下になったらギアレベルを上げる
        if (playerStatus->getGearUpCoolTime() <= 0.f) {
            playerState->setGearUp(true);

            ++gearLevel;
            playerState->setGearLevel(gearLevel);

            playerStatus->setGearUpCoolTime(playerStatus->CalculateCoolTimeByGearLevel(gearLevel));

            playerStatus->setCurrentMaxSpeed(playerStatus->CalculateSpeedByGearLevel(gearLevel));
            rigidbody->setMaxXZSpeed(playerStatus->getCurrentMaxSpeed());
        }
    }

    // 速度更新
    playerStatus->UpdateAccel(deltaTime, playerInput, transform, rigidbody, Quaternion::Identity());
    if (playerInput->getInputDirection().length() >= kEpsilon) {
        Vec3f newVelo = rigidbody->getVelocity() + rigidbody->getAcceleration() * deltaTime;
        if (newVelo.lengthSq() >= rigidbody->getMaxXZSpeed()) {
            newVelo = newVelo.normalize() * rigidbody->getMaxXZSpeed();
        }
        rigidbody->setVelocity(newVelo);
    } else {
        constexpr float kDecelerationRate = 1.6f;
        /// 初期化処理
        // 入力方向をリセット
        playerInput->setWorldInputDirection({0.f, 0.f, 0.f});
        // 速度をゼロにする
        rigidbody->setAcceleration({0.f, 0.0f, 0.0f});
        playerStatus->setCurrentMaxSpeed(0.0f);
        // ギアアップのクールタイム&ギアレベルをリセット
        playerStatus->setGearUpCoolTime(playerStatus->getBaseGearupCoolTime());
        playerState->setGearLevel(kDefaultPlayerGearLevel);

        // 速度を減衰させる
        rigidbody->setVelocity(LerpByDeltaTime(rigidbody->getVelocity(), Vec3f(), deltaTime, kDecelerationRate));
    }
    transform->UpdateMatrix();
}
