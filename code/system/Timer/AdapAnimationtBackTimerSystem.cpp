#include "AdapAnimationtBackTimerSystem.h"

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

AdapAnimationtBackTimerSystem::AdapAnimationtBackTimerSystem()
    : ISystem(SystemType::Movement) {}

AdapAnimationtBackTimerSystem::~AdapAnimationtBackTimerSystem() {}

void AdapAnimationtBackTimerSystem::Initialize() {
}

void AdapAnimationtBackTimerSystem::Finalize() {}

void AdapAnimationtBackTimerSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* TimerAnimationEntity         = ecsManager->getUniqueEntity("TimerAnimation");
    GameEntity* TimerEntity                  = ecsManager->getUniqueEntity("Timer");

    if (!TimerAnimationEntity || !TimerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    /// component取得
    SpriteRenderer* spriteRender               = getComponent<SpriteRenderer>(_entity);
    TimerAnimationStatus* timerAnimationStatus = getComponent<TimerAnimationStatus>(TimerAnimationEntity);
    /// component取得
    TimerStatus* timerStatus     = getComponent<TimerStatus>(TimerEntity);
    TimerUIStatus* timerUIStatus = getComponent<TimerUIStatus>(_entity);

    if (!spriteRender || !timerAnimationStatus) { // Componentが存在しない場合の早期リターン
        return;
    }
    timerUIStatus->SetDigit(TimerUIStatus::TimeDigit::ONE);

    ///* ------------------------------calucration------------------------------

    Vec2f baseSize = timerAnimationStatus->GetTextureSize() * timerAnimationStatus->GetBackBaseScale();
    float alpha    = timerAnimationStatus->GetBackAlpha();
    // 現在タイムの取得
    float currentTime  = timerStatus->GetCurrentTimer();
    int32_t timerDigit = timerUIStatus->GetValueForDigit(currentTime+1.0f);
    ///* ------------------------------adapt------------------------------

    spriteRender->setSize(baseSize);
    spriteRender->setColor(Vec4f(0.7f, 0.0f, 0.0f, alpha));
    // UV座標を設定
    spriteRender->setUVTranslate(Vec2f(float(timerDigit * 0.1f), 0.0f));
}
