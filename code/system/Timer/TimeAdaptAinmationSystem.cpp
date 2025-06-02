#include "TimeAdaptAinmationSystem.h"

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

TimeAdaptAinmationSystem::TimeAdaptAinmationSystem()
    : ISystem(SystemType::Movement) {}

TimeAdaptAinmationSystem::~TimeAdaptAinmationSystem() {}

void TimeAdaptAinmationSystem::Initialize() {
}

void TimeAdaptAinmationSystem::Finalize() {}

void TimeAdaptAinmationSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* TimerEntity                  = ecsManager->getUniqueEntity("Timer");
    GameEntity* TimerAnimationEntity         = ecsManager->getUniqueEntity("TimerAnimation");

    if (!TimerEntity || !TimerAnimationEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    /// component取得
    SpriteRenderer* spriteRender = getComponent<SpriteRenderer>(_entity);
    TimerAnimationStatus* timerAnimationStatus = getComponent<TimerAnimationStatus>(TimerAnimationEntity);

    if (!spriteRender || !timerAnimationStatus) { // Componentが存在しない場合の早期リターン
        return;
    }

    ///* ------------------------------calucration------------------------------

    Vec2f baseSize = timerAnimationStatus->GetTextureSize() * timerAnimationStatus->GetBaseScale();
    Vec4f clolor   = timerAnimationStatus->GetBaseClolor();

    ///* ------------------------------adapt------------------------------

    spriteRender->setSize(baseSize);
    spriteRender->setColor(clolor);
}
