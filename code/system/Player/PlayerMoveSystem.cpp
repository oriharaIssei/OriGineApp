#include "PlayerMoveSystem.h"

#define ENGINE_INCLUDE
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"

#include "component/Field/FieldStatus.h"
#include "component/Player/PlayerStates.h"

#include <cmath>
#include <numbers>
#include <Quaternion.h>
#include <Vector3.h>

PlayerMoveSystem::PlayerMoveSystem()
    : ISystem(SystemType::Movement) {}

PlayerMoveSystem::~PlayerMoveSystem() {}

void PlayerMoveSystem::Initialize() {}
void PlayerMoveSystem::Finalize() {}

void PlayerMoveSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity)
        return;

    PlayerStates* playerStates = getComponent<PlayerStates>(_entity);
    if (!playerStates)
        return;

    Transform* pivotTransform = getComponent<Transform>(_entity, 1);
    Transform* transform      = getComponent<Transform>(_entity, 0);
    transform->parent         = pivotTransform;

     // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* fieldEntity                  = ecsManager->getUniqueEntity("Field");

    if (!fieldEntity) { 
        return;
    }

    /// component取得
    FieldStatus* fieldStatus = getComponent<FieldStatus>(fieldEntity);

    if (!fieldStatus) {
        return;
    }

    float direction = playerStates->GetDirection();
    float speed     = playerStates->GetMoveSpeed();
    float deltaTime = Engine::getInstance()->getDeltaTime();

    if (direction == 0.0f) {
        return;
    }

    /// 移動ベクトル（左右）
    Vec3f velocity = {direction, 0.0f, 0.0f};

    /// 位置更新
    pivotTransform->translate += velocity.normalize() * speed * deltaTime;

   

    pivotTransform->translate[X] = std::clamp(pivotTransform->translate[X], fieldStatus->GetFieldLeftMax(), fieldStatus->GetFieldRightMax());

    /// 回転：進行方向に向ける
    float targetAngle  = std::atan2(-velocity[X], -velocity[Z]);
    float currentAngle = transform->rotate.ToEulerAngles()[Y];
    float newAngle     = LerpShortAngle(currentAngle, targetAngle, 0.5f);

    transform->rotate = Quaternion::RotateAxisAngle({0.0f, 1.0f, 0.0f}, newAngle);

    pivotTransform->Update();
    transform->Update();
}

/// ===================================================
/// 角度補間（[-π, π]の範囲で補間）
/// ===================================================
float PlayerMoveSystem::LerpShortAngle(float a, float b, float t) {
    // 角度差分を求める
    float diff = b - a;
    float pi   = std::numbers::pi_v<float>;
    // 角度を[-2PI,+2PI]に補正する
    diff = std::fmodf(diff, 2.0f * pi);
    // 角度を[-PI,PI]に補正する
    if (diff > pi) {
        diff -= 2.0f * pi;
    } else if (diff < -pi) {
        diff += 2.0f * pi;
    }
    return a + diff * t;
}
