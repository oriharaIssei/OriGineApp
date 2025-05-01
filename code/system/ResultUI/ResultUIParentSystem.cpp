#include "ResultUIParentSystem.h"

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
#include "component/ResultUI/ResultUIParentStatus.h"


#include "engine/EngineInclude.h"

ResultUIParentSystem::ResultUIParentSystem()
    : ISystem(SystemType::Movement) {}

ResultUIParentSystem::~ResultUIParentSystem() {}

void ResultUIParentSystem::Initialize() {
    time_ = 0.0f;
}

void ResultUIParentSystem::Finalize() {}

void ResultUIParentSystem::UpdateEntity(GameEntity* _entity) {

    // get timer component
    ResultUIParentStatus* resultUIParent = getComponent<ResultUIParentStatus>(_entity);
    float deltaTIme      = Engine::getInstance()->getDeltaTime();

    if (!resultUIParent) {
        return;
    }

    if (resultUIParent->GetIsAnimation() && resultUIParent->GetAnimationStep() == ResultStep::NONE) {
        time_ = 0.0f;
        // アニメーションリセット
        resultUIParent->Reset();
        resultUIParent->SetAnimationStep(ResultStep::ALPHA);
    }

    switch (resultUIParent->GetAnimationStep()) {
    case ResultStep::NONE:

        break;
        ///----------------------------------------------------------------
        /// Move Animation
        ///---------------------------------------------------------------- 
    case ResultStep::ALPHA:
        resultUIParent->MoveAnimation(deltaTIme);
        resultUIParent->AlphaAnimation(deltaTIme);
        break;
        ///----------------------------------------------------------------
        /// Scroll Wait
        ///---------------------------------------------------------------- 
    case ResultStep::SCOREUPWAIT:
        resultUIParent->AlphaAnimation(deltaTIme);
        time_ += deltaTIme;

        if (time_ < resultUIParent->GetScoreWaitTime()) {
            break;
        }
        time_ = 0.0f;
        resultUIParent->SetAnimationStep(ResultStep::SCOREUP);

        break;
        ///----------------------------------------------------------------
        /// Scroll Animation
        ///---------------------------------------------------------------- 
    case ResultStep::SCOREUP:
        resultUIParent->ScoreUP(deltaTIme);
        break;
        ///----------------------------------------------------------------
        /// Scaling Animation
        ///---------------------------------------------------------------- 
    case ResultStep::SCOREUPSCALING:
        resultUIParent->ScoreScalingAnimation(deltaTIme);
        break;
        ///----------------------------------------------------------------
        /// Reverse Wait
        ///---------------------------------------------------------------- 
    case ResultStep::END:
      
        break;
    
    default:
        break;
    }
}

void ResultUIParentSystem::ComboReset() {
}
