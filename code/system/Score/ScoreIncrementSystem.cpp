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

    float deltaTime = Engine::getInstance()->getDeltaTime();

   /* if (scoreStatus_->GetIsScoreChange()) {
        scoreStatus_->SetScoreUIStep(ScoreUIStep::INIT);
    }*/

    switch (scoreStatus_->GetScoreUIStep()) {
    case ScoreUIStep::INIT:

        scoreStatus_->Reset();
        scoreStatus_->SetScoreUIStep(ScoreUIStep::SCOREUP);
        break;

    case ScoreUIStep::SCOREUP:

        scoreStatus_->TimeIncrementAnimation(deltaTime);
       
        break;
    default:
        break;
    }

   /* scoreStatus_->TimeIncrementAnimation(deltaTime);*/
    /* scoreStatus_->SetCurrentScore(Lerp(scoreStatus_->GetCurrentScore(), scoreStatus_->GetPulusScore(), scoreStatus_->GetScoreChangeTIme()));*/

    if (scoreStatus_->GetCurrentScore() < scoreStatus_->GetScoreMax()) {
        return;
    }

    scoreStatus_->SetCurrentScore(scoreStatus_->GetScoreMax());
}

void ScoreIncrementSystem::ComboReset() {
}
