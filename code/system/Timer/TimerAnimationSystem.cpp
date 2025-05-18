#include "TimerAnimationSystem.h"

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

#include "engine/EngineInclude.h"
#include <Vector2.h>

TimerAnimationSystem::TimerAnimationSystem()
    : ISystem(SystemType::Movement) {}

TimerAnimationSystem::~TimerAnimationSystem() {}

void TimerAnimationSystem::Initialize() {
    time_           = 0.0f;
    scalingElapsed_ = 0.0f;
}

void TimerAnimationSystem::Finalize() {}

void TimerAnimationSystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* timerEntity                  = ecsManager->getUniqueEntity("Timer");

    if (!timerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    // get timer component
    TimerAnimationStatus* timerAnimationStatus = getComponent<TimerAnimationStatus>(_entity);
    TimerStatus* timerStatus                   = getComponent<TimerStatus>(timerEntity);
    float deltaTIme                            = Engine::getInstance()->getDeltaTime();

    if (!timerAnimationStatus || !timerStatus) {
        return;
    }

    switch (timerAnimationStatus->GetAnimationStep()) {
    case TimerAnimationStep::NONE:

        if (timerStatus->GetCurrentTimer() > timerStatus->GetPromiseTime()) {
            timerAnimationStatus->Reset();
            return;
        }

        // アニメーションリセット
        timerAnimationStatus->Reset();
        timerAnimationStatus->SetAnimationStep(TimerAnimationStep::SCALING);

        break;
        ///----------------------------------------------------------------
        /// Move Animation
        ///----------------------------------------------------------------
    case TimerAnimationStep::SCALING:

        timerAnimationStatus->ScalingAnimation(deltaTIme);
        timerAnimationStatus->ColorChangeEasing(deltaTIme);
        timerAnimationStatus->BackAlphaEasing(deltaTIme);
        timerAnimationStatus->BackApearScaling(deltaTIme);
        timerAnimationStatus->BackRotateEasing(deltaTIme);
        break;
        ///----------------------------------------------------------------
        /// Scroll Wait
        ///----------------------------------------------------------------
    case TimerAnimationStep::END:

        if (!timerStatus->IsChangeSecond()) {
            return;
        }

        timerAnimationStatus->SetAnimationStep(TimerAnimationStep::NONE);
        break;
        ///----------------------------------------------------------------
        /// Scroll Animation
        ///----------------------------------------------------------------

    default:
        break;
    }

    
}
