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
}

void LevelUIAnimationSystem::Finalize() {}

void LevelUIAnimationSystem::UpdateEntity(GameEntity* _entity) {

    // get timer component
    levelUIParentStatus_ = getComponent<LevelUIParentStatus>(_entity);

    if (!levelUIParentStatus_) {
        return;
    }

    if (levelUIParentStatus_->GetIsLevelChange() && levelUIParentStatus_->GetAnimationStep() == AnimationStep::NONE) {

        // アニメーションリセット
        levelUIParentStatus_->Reset();

        levelUIParentStatus_->SetAnimationStep(AnimationStep::MOVE);
    }

    switch (levelUIParentStatus_->GetAnimationStep()) {
    case AnimationStep::NONE:

        break;

    case AnimationStep::MOVE:
        levelUIParentStatus_->MoveAnimation(Engine::getInstance()->getDeltaTime());
        break;
    case AnimationStep::SCROLL:
        levelUIParentStatus_->ScrollAnimation(Engine::getInstance()->getDeltaTime());
        break;
    case AnimationStep::SCALING:
        levelUIParentStatus_->ScalingAnimation(Engine::getInstance()->getDeltaTime());
        break;
    case AnimationStep::REVERSE:
        levelUIParentStatus_->ReverseAnimation(Engine::getInstance()->getDeltaTime());
        break;
    default:
        break;
    }
}

void LevelUIAnimationSystem::ComboReset() {
}
