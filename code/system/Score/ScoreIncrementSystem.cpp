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
    time_ = 0.0f;
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
        time_ = 0.0f;
        scoreStatus_->ResetEaseTime();
        scoreStatus_->SetScoreUIStep(ScoreUIStep::SCOREUP);
        break;

        ///---------------------------------------------------
        /// スコアアップ加算
        ///---------------------------------------------------
    case ScoreUIStep::SCOREUP:
        time_ = 0.0f;
        scoreStatus_->PlusScorePlusAnimation(deltaTime);
        scoreStatus_->ScoreUPAmplitudeScaling(deltaTime);

        break;

        ///---------------------------------------------------
        /// 待機
        ///---------------------------------------------------
    case ScoreUIStep::WAIT:
        time_ += deltaTime;

        if (time_ < scoreStatus_->GetWaitTime()) {
            return;
        }

        scoreStatus_->SetScoreUIStep(ScoreUIStep::ADAPT);

        break;
        ///---------------------------------------------------
        /// 適応
        ///---------------------------------------------------
    case ScoreUIStep::ADAPT:
        scoreStatus_->ResetEaseTime();
        scoreStatus_->ResetPlusScore();
        scoreStatus_->SetIsChanging(true);
        scoreStatus_->SetTotalScoreValue();
        scoreStatus_->SetScoreUIStep(ScoreUIStep::END);
        break;
        ///---------------------------------------------------
        /// 終わり
        ///---------------------------------------------------
    case ScoreUIStep::END:
       
        break;
    default:
        break;
    }

    // 実際にタイム加算
    scoreStatus_->TimeIncrementAnimation(deltaTime);
    scoreStatus_->BaseScoreUPAmplitudeScaling(deltaTime);

    if (scoreStatus_->GetCurrentScore() <= 0.0f) {
        scoreStatus_->SetCurrentScore(0.0f);
    }

    if (scoreStatus_->GetCurrentScore() < scoreStatus_->GetScoreMax()) {
        return;
    }

    scoreStatus_->SetCurrentScore(scoreStatus_->GetScoreMax());
}

void ScoreIncrementSystem::ComboReset() {
}
