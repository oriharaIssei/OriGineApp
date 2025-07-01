#include "FloorUISystem.h"

/// Engine
#define ENGINE_INCLUDE
#define ENGINE_INPUT
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// component
#include "component/FloorUI/FloorUIController.h"
#include "component/FloorUI/FloorUIStatus.h"
#include "component/Menu/TutorialMenuParentStatus.h"
#include"component/GameStart/GameStartStatus.h"

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
    GameEntity* gameStartEntity              = ecsManager->getUniqueEntity("GameStartRendition");


    if (!FloorUIControllerEntity || !gameStartEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    FloorUIStatus* floorUIStatus         = getComponent<FloorUIStatus>(entity);
    Transform* transform                 = getComponent<Transform>(entity);
    FloorUIController* floorUIController = getComponent<FloorUIController>(FloorUIControllerEntity);
    GameStartStatus* gameStartStatus     = getComponent<GameStartStatus>(gameStartEntity);


    if (!floorUIStatus || !transform || !floorUIController) {
        return;
    }

    if (!gameStartStatus->GetIsStart()) {
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
        floorUIStatus->SetFloorUIStep(FloorUIStep::APPER);

        break;

        ///-----------------------------------------------------------
        /// 出現
        ///-----------------------------------------------------------
    case FloorUIStep::APPER:
        floorUIStatus->SavingScale();
        floorUIStatus->moveEasing(deltaTime);
        floorUIStatus->RotateMoving(deltaTime);
        floorUIStatus->ChangeAnimationEndForFlag();
        floorUIStatus->OpenEasing(deltaTime);

        break;

        ///-----------------------------------------------------------
        /// UI強調
        ///-----------------------------------------------------------
    case FloorUIStep::MOVING:
        floorUIStatus->SavingScale();
        floorUIStatus->ChangeAnimationEndForFlag();
        floorUIStatus->RotateMoving(deltaTime);
        floorUIStatus->moveEasing(deltaTime);
        break;

        ///-----------------------------------------------------------
        /// 閉じる
        ///-----------------------------------------------------------
    case FloorUIStep::CLOSE:
        floorUIStatus->moveEasing(deltaTime);
        floorUIStatus->CloseEasing(deltaTime);
        break;

        ///-----------------------------------------------------------
        /// 終わり
        ///-----------------------------------------------------------
    case FloorUIStep::END:
        floorUIStatus->Reset();
        floorUIStatus->SetFloorUIStep(FloorUIStep::NONE);

        break;

    default:
        break;
    }

    ///* ------------------------------calucration------------------------------

    Vec3f basePos   = floorUIStatus->GetBasePosition() + floorUIStatus->GetPosition();
    Vec3f baseScale = floorUIStatus->GetBaseScale();
    float rotateZ   = floorUIStatus->GetRotate();
   
    ///* ------------------------------adapt------------------------------

    // pos
    transform->translate = basePos;
    transform->scale     = baseScale;

     // Quaternionに変換
    Quaternion q = Quaternion::FromEulerAngles(0.0f, 0.0f, rotateZ);
    // クォータニオンで回転をセット
    transform->rotate    = q;
}

void FloorUISystem::ComboReset() {
}
