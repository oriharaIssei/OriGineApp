#include "TimerDecrementSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/Timer/TimerStatus.h"
#include"component/Menu/MenuStatus.h"
#include"component/GameStart/GameStartStatus.h"

#include "engine/EngineInclude.h"
#include <cmath>
#include <cstdint>

TimerDecrementSystem::TimerDecrementSystem()
    : ISystem(SystemType::Movement) {}

TimerDecrementSystem::~TimerDecrementSystem() {}

void TimerDecrementSystem::Initialize() {
}

void TimerDecrementSystem::Finalize() {}

void TimerDecrementSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    // ポーズ中は通さない
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* menuEntity                   = ecsManager->getUniqueEntity("Menu");
    GameEntity* gameStartEntity              = ecsManager->getUniqueEntity("GameStartRendition");

    if (!menuEntity || !gameStartEntity) {
        return;
    }

    MenuStatus* menu = getComponent<MenuStatus>(menuEntity);
    GameStartStatus* gameStartStatus = getComponent<GameStartStatus>(gameStartEntity);


    if (!menu) {
        return;
    }

    if (menu->GetIsPose()) {
        return;
    }

    if (!gameStartStatus->GetIsStart()) {
        return;
    }


    // get timer component 
   timerStauts_ = getComponent<TimerStatus>(_entity);

   if (!timerStauts_) {
       return;
   }

   //タイマー減算
   timerStauts_->DecrementTimer();

  //タイマー多すぎた場合の処理
   if (timerStauts_->GetCurrentTimer() >= 60) {
       timerStauts_->SetCurrentTimer(60);
   }
   
}

void TimerDecrementSystem::ComboReset() {

 }
