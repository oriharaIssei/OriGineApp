#include "FloorUISystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// component
#include "component/FloorUI/FloorUIController.h"
#include "component/FloorUI/FloorUIStatus.h"
#include "component/Menu/TutorialMenuParentStatus.h"

#include "engine/EngineInclude.h"

FloorUISystem::FloorUISystem()
    : ISystem(SystemType::Movement) {}

FloorUISystem::~FloorUISystem() {}

void FloorUISystem::Initialize() {
    time_  = 0.0f;
    input_ = Input::getInstance();
}

void FloorUISystem::Finalize() {}

void FloorUISystem::UpdateEntity(GameEntity* entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* FloorUIControllerEntity      = ecsManager->getUniqueEntity("FloorUIController");

    if (!FloorUIControllerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    FloorUIStatus* floorUIStatus         = getComponent<FloorUIStatus>(entity);
    Transform* transform                 = getComponent<Transform>(entity);
    FloorUIController* floorUIController = getComponent<FloorUIController>(FloorUIControllerEntity);

    if (!floorUIStatus || !transform || !floorUIController) {
        return;
    }

    float deltaTime = Engine::getInstance()->getDeltaTime();

    switch (floorUIStatus->GetFloorUIStep()) {
        ///-----------------------------------------------------------
        /// 停止中
        ///-----------------------------------------------------------
    case FloorUIStep::NONE:

        if (!floorUIStatus->GetIsAnimationPtr()) {
            return;
        }
        floorUIStatus->Reset();

        break;

        ///-----------------------------------------------------------
        /// メニューセレクト中
        ///-----------------------------------------------------------
    case FloorUIStep::APPER:
        floorUIStatus->SavingScale();
        floorUIStatus->ChangeAnimationEndForFlag();
        floorUIStatus->OpenEasing(deltaTime);

        break;

        ///-----------------------------------------------------------
        /// チュートリアル
        ///-----------------------------------------------------------
    case FloorUIStep::MOVING:
        floorUIStatus->SavingScale();
        floorUIStatus->ChangeAnimationEndForFlag();
        floorUIStatus->moveEasing(deltaTime);
        break;

        ///-----------------------------------------------------------
        /// 閉じる前の初期化*
        ///-----------------------------------------------------------
    case FloorUIStep::CLOSE:
        floorUIStatus->CloseEasing(deltaTime);
        break;

        ///-----------------------------------------------------------
        /// 終わり
        ///-----------------------------------------------------------
    case FloorUIStep::END:
        floorUIStatus->SetFloorUIStep(FloorUIStep::NONE);

        break;

    default:
        break;
    }

    ///* ------------------------------calucration------------------------------

    Vec3f basePos   = floorUIStatus->GetPosition();
    Vec3f baseScale = floorUIStatus->GetBaseScale();

    ///* ------------------------------adapt------------------------------

    // pos
    transform->translate = basePos;
    transform->scale     = baseScale;
}

void FloorUISystem::ComboReset() {
}
