#include "PlayerInput.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
#include "input/Input.h"
// include
#include "component/Field/FieldStates.h"
#include "component/Player/PlayerStates.h"
#include "engine/EngineInclude.h"

PlayerInputSystem::PlayerInputSystem()
    : ISystem(SystemType::Input) {}

PlayerInputSystem::~PlayerInputSystem() {}

void PlayerInputSystem::Initialize() {
    input_ = Input::getInstance();
}

void PlayerInputSystem::Finalize() {}

void PlayerInputSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    ///============================================================
    // 移動入力
    ///============================================================
    Rigidbody* entityRigidbody = getComponent<Rigidbody>(_entity);
    if (!entityRigidbody) {
        return;
    }
    PlayerStates* entityPlayerStates = getComponent<PlayerStates>(_entity);
    if (!entityPlayerStates) {
        return;
    }

    float angularSpeed = 0.0f;

    if (input_->isPressKey(DIK_D)) {
        angularSpeed += entityPlayerStates->GetMoveSpeed(); // 反時計回りに加速
    }

    else if (input_->isPressKey(DIK_A)) {
        angularSpeed -= entityPlayerStates->GetMoveSpeed(); // 時計回りに加速
    }

    entityPlayerStates->SetIncrementTheta(angularSpeed * Engine::getInstance()->getDeltaTime());

    FieldStates* entityFieldStates = getComponent<FieldStates>(_entity);
    if (!entityFieldStates) {
        return;
    }

    Vec3f moveDir3d = {
        entityFieldStates->GetFieldRadius() * std::cos(entityPlayerStates->GetTheta()),
        0.0f,
        entityFieldStates->GetFieldRadius() * std::sin(entityPlayerStates->GetTheta())};


    entityRigidbody->setVelocity(moveDir3d);

    ///============================================================
    // 弾 発射 入力
    ///============================================================
    // BulletSpawner* entityBulletSpawner = getComponent<BulletSpawner>(_entity);
    // if (!entityBulletSpawner) {
    //     return;
    // }

    // float currentCoolTime = entityBulletSpawner->getFireCoolTime();
    // entityBulletSpawner->setIsFire(false);

    //// coolTimeが0以下になったら発射
    // if (currentCoolTime <= 0.f) {
    //     entityBulletSpawner->setIsFire(input_->isPressKey(DIK_SPACE));
    // } else {
    //     // coolTimeを減らす
    //     currentCoolTime -= Engine::getInstance()->getDeltaTime();
    //     entityBulletSpawner->setFireCoolTime((std::min)(currentCoolTime, 0.f));
    // }
}
