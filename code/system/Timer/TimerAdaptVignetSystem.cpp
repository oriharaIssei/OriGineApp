#include "TimerAdaptVignetSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Vector3.h>
// component
#include "component/Timer/TimerAnimationStatus.h"


#define RESOURCE_DIRECTORY
#include "engine/EngineInclude.h"
#include <cstdint>

TimerAdaptVignetSystem::TimerAdaptVignetSystem()
    : ISystem(SystemType::Movement) {}

TimerAdaptVignetSystem::~TimerAdaptVignetSystem() {}

void TimerAdaptVignetSystem::Initialize() {
}

void TimerAdaptVignetSystem::Finalize() {}

void TimerAdaptVignetSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
 
    GameEntity* TimerAnimationEntity         = ecsManager->getUniqueEntity("TimerAnimation");

    if (!TimerAnimationEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    /// component取得
    VignetteParam* vignetterParm                = getComponent<VignetteParam>(_entity);
    TimerAnimationStatus* timerAnimationStatus = getComponent<TimerAnimationStatus>(TimerAnimationEntity);

    if (!vignetterParm || !timerAnimationStatus) { // Componentが存在しない場合の早期リターン
        return;
    }

    ///* ------------------------------calucration------------------------------
 /*   Vec4f clolor   = timerAnimationStatus->GetBaseClolor();*/

    ///* ------------------------------adapt------------------------------
    vignetterParm->getVignetteBuffer().openData_.vignettePow = timerAnimationStatus->GetVignetteValue();
    vignetterParm->setVignetteParam(vignetterParm->getVignetteParam());

}
