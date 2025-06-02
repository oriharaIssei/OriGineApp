#include "CameraShakeSystem.h"

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
#include "component/GameCamera/CameraRenditionStatus.h"
#include "component/Piller/FloatingFloorStatus.h"

#include "engine/EngineInclude.h"

CameraShakeSystem::CameraShakeSystem()
    : ISystem(SystemType::Movement) {}

CameraShakeSystem::~CameraShakeSystem() {}

void CameraShakeSystem::Initialize() {
   
}

void CameraShakeSystem::Finalize() {}

void CameraShakeSystem::UpdateEntity(GameEntity* _entity) {

   /* EntityComponentSystemManager* ecsManager = ECSManager::getInstance();*/
  /*  GameEntity* animationEntity              = ecsManager->getUniqueEntity("FloatingFloorAnimation");*/

    //if (!animationEntity) { // Entityが存在しない場合の早期リターン
    //    return;
    //}

    CameraRenditionStatus* cameraStatus  = getComponent<CameraRenditionStatus>(_entity);
 

    if (!cameraStatus) {
        return;
    }

    float deltaTime = Engine::getInstance()->getDeltaTime();

    switch (cameraStatus->GetRenditionStep()) {
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case CameraRenditionStep::NONE:

        if (!cameraStatus->GetIsShaking()) {
            return;
        }
        cameraStatus->ShakeReset();

        cameraStatus->SetRenditionStep(CameraRenditionStep::SHAKING);
        break;

        ///---------------------------------------------------
        /// ダメージシェイク
        ///---------------------------------------------------
    case CameraRenditionStep::SHAKING:

        cameraStatus->CameraShake(deltaTime);
        break;

        ///---------------------------------------------------
        /// 常に揺れる
        ///---------------------------------------------------
    case CameraRenditionStep::END:
        cameraStatus->ShakeReset();
        cameraStatus->SetRenditionStep(CameraRenditionStep::NONE);
        break;

        ///---------------------------------------------------
        /// 終わり
        ///---------------------------------------------------
   
    default:
        break;
    }

   
}

void CameraShakeSystem::ComboReset() {
}
