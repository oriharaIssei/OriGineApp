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
#include "component/Combo/ComboUIStatus.h"

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

    // get timer component 
   timerStauts_ = getComponent<TimerStatus>(_entity);

   if (!timerStauts_) {
       return;
   }

   //タイマー減算
   timerStauts_->DecrementTimer();

   
}

void TimerDecrementSystem::ComboReset() {

 }
