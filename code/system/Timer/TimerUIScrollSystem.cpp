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
#include "component/Timer/TimerStatus.h"
#include "component/Timer/TimerUIStatus.h"

#define RESOURCE_DIRECTORY
#include "engine/EngineInclude.h"
#include <cstdint>

TimerUIScrollSystem::TimerUIScrollSystem()
    : ISystem(SystemType::Movement) {}

TimerUIScrollSystem::~TimerUIScrollSystem() {}

void TimerUIScrollSystem::Initialize() {
    //// TextureNameの初期化
    // for (int32_t i = 0; i < 10; ++i) {
    //     textureName_[i] = "Texture/Combo/ComboNumber" + std::to_string(i) + ".png";
    // };
}

void TimerUIScrollSystem::Finalize() {}

void TimerUIScrollSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* TimerEntity                  = ecsManager->getUniqueEntity("Timer");

    if (!TimerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    /// component取得
    TimerStatus* comboStatus     = getComponent<TimerStatus>(TimerEntity);
    TimerUIStatus* comboUIStatus = getComponent<TimerUIStatus>(_entity);
    SpriteRenderer* spriteRender = getComponent<SpriteRenderer>(_entity);

    if (!comboStatus || !comboUIStatus || !spriteRender) { // Componentが存在しない場合の早期リターン
        return;
    }

    // 現在タイムの取得
    float currentTime  = comboStatus->GetCurrentTimer();
    int32_t timerDigit = comboUIStatus->GetValueForDigit(currentTime);

    //UV座標を設定
    spriteRender->setUVTranslate(Vec2f(float(timerDigit * 0.1f), 0.0f));
}
