#include "AdaptFrontPlaneSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
// ECS
#define ENGINE_ECS
// include

// component
#include "component/Block/BlockFrontPlaneStatus.h"
#include "component/GameStart/GameStartStatus.h"

#include "engine/EngineInclude.h"

AdaptFrontPlaneSystem::AdaptFrontPlaneSystem()
    : ISystem(SystemType::Movement) {}

AdaptFrontPlaneSystem::~AdaptFrontPlaneSystem() {}

void AdaptFrontPlaneSystem::Initialize() {
    time_ = 0.0f;
}

void AdaptFrontPlaneSystem::Finalize() {}

void AdaptFrontPlaneSystem::UpdateEntity(GameEntity* _entity) {

    /*EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* gameStartEntity              = ecsManager->getUniqueEntity("GameStartRendition");*/

    // if (!gameStartEntity) { // Entityが存在しない場合の早期リターン
    //     return;
    // }

    Transform* transform                         = getComponent<Transform>(_entity);
    BlockFrontPlaneStatus* blockFrontPlaneStatus = getComponent<BlockFrontPlaneStatus>(_entity);

    if (!blockFrontPlaneStatus || !transform) {
        return;
    }

    Vec3f scale  = blockFrontPlaneStatus->GetScale();
    float rotate = blockFrontPlaneStatus->GetRotate();

    // Y軸90度回転とZ軸回転を組み合わせる
    Quaternion yRotation = Quaternion::FromEulerAngles(0.0f, std::numbers::pi_v<float>, 0.0f); // Y軸90度
    Quaternion zRotation = Quaternion::FromEulerAngles(0.0f, 0.0f, rotate); // Z軸回転

    // 回転を組み合わせる（順番が重要）
    // Z軸回転 -> Y軸90度回転 の順に適用する場合
    Quaternion animationRotation = zRotation * yRotation;
    // Y軸90度回転 -> Z軸回転 の順に適用する場合
    // Quaternion animationRotation = yRotation * zRotation;

    transform->translate = Vec3f{0.0f, 0.0f, -1.0f}; // 位置は固定
    transform->rotate    = animationRotation;
    transform->scale     = scale;
}

void AdaptFrontPlaneSystem::ComboReset() {
}
