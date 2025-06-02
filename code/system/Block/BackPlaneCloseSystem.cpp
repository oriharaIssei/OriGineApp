#include "BackPlaneCloseSystem.h"

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

BackPlaneCloseSystem::BackPlaneCloseSystem()
    : ISystem(SystemType::Movement) {}

BackPlaneCloseSystem::~BackPlaneCloseSystem() {}

void BackPlaneCloseSystem::Initialize() {
    time_ = 0.0f;
}

void BackPlaneCloseSystem::Finalize() {}

void BackPlaneCloseSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    ModelMeshRenderer* modelRenderer             = getComponent<ModelMeshRenderer>(_entity);
    BlockFrontPlaneStatus* blockFrontPlaneStatus = getComponent<BlockFrontPlaneStatus>(_entity);
    Transform* transform                         = getComponent<Transform>(_entity);

    float deltaTime = Engine::getInstance()->getDeltaTime();

    if (!modelRenderer || !blockFrontPlaneStatus || !transform) {
        return;
    }

    switch (blockFrontPlaneStatus->GetPlaneStep()) {
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case BlockFrontPlaneStatus::PlaneStep::NONE:
        if (!*blockFrontPlaneStatus->GetIsFrontPlaneClose()) {
            blockFrontPlaneStatus->Reset();
            break;
        }

        blockFrontPlaneStatus->SetPlaneStep(BlockFrontPlaneStatus::PlaneStep::CLOSE);
        break;
        ///---------------------------------------------------
        /// 適応
        ///---------------------------------------------------

    case BlockFrontPlaneStatus::PlaneStep::CLOSE:
        blockFrontPlaneStatus->CloseEasing(deltaTime);
        break;
        ///---------------------------------------------------
        /// 終わり
        ///---------------------------------------------------
    case BlockFrontPlaneStatus::PlaneStep::END:
        /*color_ = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);*/
        break;
    default:
        break;
    }

    Vec3f scale  = blockFrontPlaneStatus->GetScale();
    float rotate = blockFrontPlaneStatus->GetRotate();

    // Y軸90度回転とZ軸回転を組み合わせる
    Quaternion yRotation = Quaternion::FromEulerAngles(0.0f, std::numbers::pi_v<float>, 0.0f); // Y軸90度
    Quaternion zRotation = Quaternion::FromEulerAngles(0.0f, 0.0f, rotate); // Z軸回転

    Quaternion animationRotation = zRotation * yRotation;

    transform->translate = Vec3f{0.0f, 0.0f, -1.0f}; // 位置は固定
    transform->rotate    = animationRotation;
    transform->scale     = scale;

    if (transform->parent->scale == Vec3f(0.0f, 0.0f, 0.0f)) {
        modelRenderer->setIsRender(false);
    }
}

void BackPlaneCloseSystem::ComboReset() {
}
