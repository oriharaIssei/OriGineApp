#include "PlayerMoveSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/Player/PlayerStates.h"

#include "engine/EngineInclude.h"
#include <cmath>

PlayerMoveSystem::PlayerMoveSystem()
    : ISystem(SystemType::Movement) {}

PlayerMoveSystem::~PlayerMoveSystem() {}

void PlayerMoveSystem::Initialize() {
}

void PlayerMoveSystem::Finalize() {}

void PlayerMoveSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }
    PlayerStates* entityPlayerStates = getComponent<PlayerStates>(_entity);

    if (!entityPlayerStates) {
        return;
    }

    Transform* pivotTransform = getComponent<Transform>(_entity, 1);
    Transform* transform      = getComponent<Transform>(_entity, 0);

    float moveTime  = entityPlayerStates->GetMoveSpeed();
    float deltaTime = Engine::getInstance()->getDeltaTime();

    // 移動方向をセット
    if (entityPlayerStates->GetDirection() == 0.0f) {
        return;
    }

    ///============================================================
    // Y軸回転のQuaternionを作成
    ///============================================================
    Quaternion rotateAxisY = Quaternion::RotateAxisAngle(Vec3f(0.0f, 1.0f, 0.0f),
        entityPlayerStates->GetDirection() * moveTime * deltaTime);

    ///============================================================
    // 変換後の位置を計算
    ///============================================================

    // 位置を適用
    pivotTransform->rotate *= rotateAxisY;

    // 進行方向よ
    transform->rotate = Quaternion::RotateAxisAngle({0.0f, 1.0f, 0.0f},
        std::atan2(-entityPlayerStates->GetDirection(), 0.0f));

    /// 更新
    pivotTransform->Update();
    transform->Update();
}
