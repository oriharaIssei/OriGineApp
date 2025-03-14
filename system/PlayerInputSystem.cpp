#include "PlayerInputSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
#include "input/Input.h"
// include
#include "EngineInclude.h"

/// Application
// component
#include "component/BulletSpawner.h"

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

    Vec2f moveDir2d = {
        static_cast<float>(input_->isPressKey(DIK_D) - input_->isPressKey(DIK_A)),
        static_cast<float>(input_->isPressKey(DIK_W) - input_->isPressKey(DIK_S))};

    entityRigidbody->setVelocity(Vec3f(moveDir2d, 0.f));

    ///============================================================
    // 弾 発射 入力
    ///============================================================
    BulletSpawner* entityBulletSpawner = getComponent<BulletSpawner>(_entity);
    if (!entityBulletSpawner) {
        return;
    }

    float currentCoolTime = entityBulletSpawner->getFireCoolTime();
    entityBulletSpawner->setIsFire(false);

    // coolTimeが0以下になったら発射
    if (currentCoolTime <= 0.f) {
        entityBulletSpawner->setIsFire(input_->isPressKey(DIK_SPACE));
    } else {
        // coolTimeを減らす
        currentCoolTime -= Engine::getInstance()->getDeltaTime();
        entityBulletSpawner->setFireCoolTime((std::min)(currentCoolTime, 0.f));
    }
}
