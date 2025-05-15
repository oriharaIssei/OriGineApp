#include "TimerUIScrollSystem.h"

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
#include "component/Timer/TimerStatus.h"
#include "component/Timer/TimerUIStatus.h"

#define RESOURCE_DIRECTORY
#include "engine/EngineInclude.h"
#include <cstdint>

TimerUIScrollSystem::TimerUIScrollSystem()
    : ISystem(SystemType::Movement) {}

TimerUIScrollSystem::~TimerUIScrollSystem() {}

void TimerUIScrollSystem::Initialize() {
}

void TimerUIScrollSystem::Finalize() {}

void TimerUIScrollSystem::UpdateEntity(GameEntity* _entity) {

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
    TimerStatus* timerStatus     = getComponent<TimerStatus>(TimerEntity);
    TimerUIStatus* timerUIStatus = getComponent<TimerUIStatus>(_entity);
    SpriteRenderer* spriteRender = getComponent<SpriteRenderer>(_entity);
  
    if (!timerStatus || !timerUIStatus || !spriteRender) { // Componentが存在しない場合の早期リターン
        return;
    }

   
    // 現在タイムの取得
    float currentTime  = timerStatus->GetCurrentTimer();
    int32_t timerDigit = timerUIStatus->GetValueForDigit(currentTime);

   // UV座標を設定
    spriteRender->setUVTranslate(Vec2f(float(timerDigit * 0.1f), 0.0f));
}
