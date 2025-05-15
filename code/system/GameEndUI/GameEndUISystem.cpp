#include "GameEndUISystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Vector3.h>
// component
#include "component/GameEndUI/GameEndUIStatus.h"
#include "component/Timer/TimerStatus.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>

GameEndUISystem::GameEndUISystem()
    : ISystem(SystemType::Movement) {}

GameEndUISystem::~GameEndUISystem() {}

void GameEndUISystem::Initialize() {
    time_ = 0.0f;
}

void GameEndUISystem::Finalize() {}

void GameEndUISystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");

    if (!timerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    // get timer component
    GameEndUIStatus* gameEndUIStatus = getComponent<GameEndUIStatus>(_entity);
    SpriteRenderer* sprite           = getComponent<SpriteRenderer>(_entity);
    TimerStatus* timerStatus         = getComponent<TimerStatus>(timerEntity);
    float deltaTIme                  = Engine::getInstance()->getDeltaTime();

    if (!gameEndUIStatus || !timerStatus || !sprite) {
        return;
    }

    switch (gameEndUIStatus->GetAnimationStep()) {
    case GameEndUIStep::NONE:
  
        if (timerStatus->GetCurrentTimer() > 0.0f) {
            gameEndUIStatus->SetBaseScale(Vec2f(0.0f, 0.0f));
            break;
        }
        // アニメーションリセット
        time_ = 0.0f;
        gameEndUIStatus->Reset();
        gameEndUIStatus->SetAnimationStep(GameEndUIStep::APEER);
        break;
        ///----------------------------------------------------------------
        /// Move Animation
        ///----------------------------------------------------------------
    case GameEndUIStep::APEER:
       
        gameEndUIStatus->ApeerUIAnimation(deltaTIme);
        gameEndUIStatus->AlphaEaseAnimation(deltaTIme);
        gameEndUIStatus->CheckAbleNextStep();

        break;
        ///----------------------------------------------------------------
        /// Scroll Wait
        ///----------------------------------------------------------------
    case GameEndUIStep::WAIT:
        time_ += deltaTIme;

        if (time_ < gameEndUIStatus->GetWaitTimeAfterApear()) {
            break;
        }
        time_ = 0.0f;
        gameEndUIStatus->SetAnimationStep(GameEndUIStep::END);
        break;
        ///----------------------------------------------------------------
        /// Scroll Animation
        ///----------------------------------------------------------------
    case GameEndUIStep::END:

        break;

    default:
        break;
    }

    ///* ------------------------------calucration------------------------------

    Vec2f baseSize = sprite->getTextureSize() * gameEndUIStatus->GetBaseScale();
 
    ///* ------------------------------adapt------------------------------

    sprite->setColor(Vec4f(1.0f,1.0f,1.0f,gameEndUIStatus->GetAlpha()));
    sprite->setSize(baseSize);
}
