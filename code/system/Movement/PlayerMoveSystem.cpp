#include "PlayerMoveSystem.h"

/// engine
#define ENGINE_COMPONENTS
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/Player/PlayerInput.h"
#include "component/Player/PlayerStatus.h"

void PlayerMoveSystem::Initialize() {
}

void PlayerMoveSystem::Finalize() {
}

void PlayerMoveSystem::UpdateEntity(GameEntity* _entity) {
    PlayerInput* playerInput   = getComponent<PlayerInput>(_entity);
    PlayerStatus* playerStatus = getComponent<PlayerStatus>(_entity);
    Rigidbody* rigidbody       = getComponent<Rigidbody>(_entity);
    Transform* transform       = getComponent<Transform>(_entity);

    if (playerInput->getInputDirection().lengthSq() <= 0.f) {
        rigidbody->setVelocity({0.f, 0.f, 0.f});
        return;
    }
    const float deltaTime = getMainDeltaTime();

    // プレイヤーの向きを入力方向に向ける
    Vector3f forward = Vec3f(playerInput->getInputDirection()[X], 0.f, playerInput->getInputDirection()[Y]);
    forward.normalize();

    // gearLevel の更新
    playerStatus->minusGearUpCoolTime(deltaTime);
    if (playerStatus->getGearUpCoolTime() <= 0.f) {
        playerStatus->setGearLevel(playerStatus->getGearLevel() + 1);
        playerStatus->setGearUpCoolTime(1.f * playerStatus->getGearLevel());
    }

    // 移動速度の更新
    float playerSpeed = playerStatus->getBaseSpeed();
    rigidbody->setVelocity(forward * playerSpeed * deltaTime);

    /// --------------------------------------- 位置の更新 --------------------------------------- ///
    transform->translate += rigidbody->getVelocity() * deltaTime;
    // worldMatの更新
    transform->Update();
}
