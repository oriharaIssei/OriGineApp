#include "PlayerMoveSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
#include "input/Input.h"
// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/Player/PlayerStates.h"

#include "engine/EngineInclude.h"
#include <cmath>
#include <numbers>

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

    entityPlayerStates_ = getComponent<PlayerStates>(_entity);
    if (!entityPlayerStates_) {
        return;
    }

    float moveTime  = entityPlayerStates_->GetMoveSpeed();
    float deltaTime = Engine::getInstance()->getDeltaTime();
   GetTransformForPlayer(_entity);

    // 移動方向をセット
   if (entityPlayerStates_->GetDirection() == 0.0f) {
       return;
   }

    ///============================================================
    // Y軸回転のQuaternionを作成
    ///============================================================
    Quaternion rotateAxisY = Quaternion::RotateAxisAngle(Vec3f(0.0f, 1.0f, 0.0f),
        entityPlayerStates_->GetDirection() * moveTime * deltaTime);

    ///============================================================
    // 変換後の位置を計算
    ///============================================================

    // 位置を適用
    pivotTransform_->rotate *= rotateAxisY;

    // 進行方向よ
    transform_->rotate = Quaternion::RotateAxisAngle({0.0f, 1.0f, 0.0f},
            std::atan2(-entityPlayerStates_->GetDirection(), 0.0f));

    /// 更新
    pivotTransform_->Update();
    transform_->Update();
}

void PlayerMoveSystem::GetTransformForPlayer(GameEntity* _entity) {
    /// TransformをSet
    _entity;
    if (isInited_) {
        return;
    }

    if (!entityPlayerStates_->GetTransform()) {
        return;
    }
    if (!entityPlayerStates_->GetPivotTransform()) {
        return;
    }

    transform_      = entityPlayerStates_->GetTransform();
    pivotTransform_ = entityPlayerStates_->GetPivotTransform();

   isInited_ = true;
 }
