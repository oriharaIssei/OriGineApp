#include "ScoreIncrementSystem.h"

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
#include "component/Combo/ComboUIStatus.h"
#include "component/Score/ScoreStatus.h"

#include "engine/EngineInclude.h"
#include <cmath>
#include <cstdint>

ScoreIncrementSystem::ScoreIncrementSystem()
    : ISystem(SystemType::Movement) {}

ScoreIncrementSystem::~ScoreIncrementSystem() {}

void ScoreIncrementSystem::Initialize() {
}

void ScoreIncrementSystem::Finalize() {}

void ScoreIncrementSystem::UpdateEntity(GameEntity* _entity) {

    // get timer component
    scoreStatus_ = getComponent<ScoreStatus>(_entity);

    if (!scoreStatus_) {
        return;
    }

    scoreStatus_->TimeIncrement(Engine::getInstance()->getDeltaTime());
    scoreStatus_->SetCurrentScore(Lerp(scoreStatus_->GetCurrentScore(), scoreStatus_->GetPulusScore(), scoreStatus_->GetScoreChangeTIme()));
}

void ScoreIncrementSystem::ComboReset() {
}

float ScoreIncrementSystem::Lerp(const float& start, const float& end, float t) {
    return (1.0f - t) * start + end * t;
}
