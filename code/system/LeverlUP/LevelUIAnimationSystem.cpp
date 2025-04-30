#include "LevelUIAnimationSystem.h"

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
#include "component/LevelUPUI/LevelUIParentStatus.h"
#include "component/LevelUPUI/LevelUIStatus.h"
#include "component/Timer/TimerStatus.h"

#include "engine/EngineInclude.h"

LevelUIAnimationSystem::LevelUIAnimationSystem()
    : ISystem(SystemType::Movement) {}

LevelUIAnimationSystem::~LevelUIAnimationSystem() {}

void LevelUIAnimationSystem::Initialize() {
    time_ = 0.0f;
}

void LevelUIAnimationSystem::Finalize() {}

void LevelUIAnimationSystem::UpdateEntity(GameEntity* _entity) {

    // get timer component
    levelUIParentStatus_ = getComponent<LevelUIParentStatus>(_entity);
    float deltaTIme      = Engine::getInstance()->getDeltaTime();

    if (!levelUIParentStatus_) {
        return;
    }

    if (levelUIParentStatus_->GetIsLevelChange() && levelUIParentStatus_->GetAnimationStep() == AnimationStep::NONE) {
        time_ = 0.0f;
        // アニメーションリセット
        levelUIParentStatus_->Reset();
        levelUIParentStatus_->SetAnimationStep(AnimationStep::MOVE);
    }

    switch (levelUIParentStatus_->GetAnimationStep()) {
    case AnimationStep::NONE:

        break;
        ///----------------------------------------------------------------
        /// Move Animation
        ///---------------------------------------------------------------- 
    case AnimationStep::MOVE:
        levelUIParentStatus_->MoveAnimation(deltaTIme);
        break;
        ///----------------------------------------------------------------
        /// Scroll Wait
        ///---------------------------------------------------------------- 
    case AnimationStep::SCROLLWAIT:
        time_ += deltaTIme;

        if (time_ < levelUIParentStatus_->GetScrollWaitTime()) {
            break;
        }
        time_ = 0.0f;
        levelUIParentStatus_->SetAnimationStep(AnimationStep::SCROLL);

        break;
        ///----------------------------------------------------------------
        /// Scroll Animation
        ///---------------------------------------------------------------- 
    case AnimationStep::SCROLL:
        levelUIParentStatus_->ScrollAnimation(deltaTIme);
        break;
        ///----------------------------------------------------------------
        /// Scaling Animation
        ///---------------------------------------------------------------- 
    case AnimationStep::SCALING:
        levelUIParentStatus_->ScalingAnimation(deltaTIme);
        break;
        ///----------------------------------------------------------------
        /// Reverse Wait
        ///---------------------------------------------------------------- 
    case AnimationStep::REVERSEWAIT:
        time_ += deltaTIme;

        if (time_ < levelUIParentStatus_->GetReverseWaitTime()) {
            break;
        }
        time_ = 0.0f;
        levelUIParentStatus_->SetAnimationStep(AnimationStep::REVERSE);
        break;
        ///----------------------------------------------------------------
        /// Reverse Animation
        ///---------------------------------------------------------------- 
    case AnimationStep::REVERSE:
        levelUIParentStatus_->ReverseAnimation(deltaTIme);
        break;
    default:
        break;
    }
}

void LevelUIAnimationSystem::ComboReset() {
}
