#include "FloatingFloorAnimationSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
// ECS
#define ENGINE_ECS
// include

// component
#include "component/Piller/FloatingFloorAnimationStatus.h"
#include "component/Piller/FloatingFloorStatus.h"

#include "engine/EngineInclude.h"

FloatingFloorAnimationSystem::FloatingFloorAnimationSystem()
    : ISystem(SystemType::Movement) {}

FloatingFloorAnimationSystem::~FloatingFloorAnimationSystem() {}

void FloatingFloorAnimationSystem::Initialize() {
    time_ = 0.0f;
}

void FloatingFloorAnimationSystem::Finalize() {}

void FloatingFloorAnimationSystem::UpdateEntity(GameEntity* _entity) {

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* animationEntity              = ecsManager->getUniqueEntity("FloatingFloorAnimation");

    if (!animationEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    animationStatus_ = getComponent<FloatingFloorAnimationStatus>(animationEntity);
    FloatingFloorStatus* floatingFloorStatus = getComponent<FloatingFloorStatus>(_entity);

    if (!animationStatus_ || !floatingFloorStatus) {
        return;
    }

    float deltaTime = Engine::getInstance()->getDeltaTime();

    /* if (scoreStatus_->GetIsScoreChange()) {
         scoreStatus_->SetScoreUIStep(ScoreUIStep::INIT);
    }*/

    switch (floatingFloorStatus->GetReactionStep()) {
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case ReactionStep::NONE:
        time_ = 0.0f;
      
        break;

        ///---------------------------------------------------
        /// スコアアップ加算
        ///---------------------------------------------------
    case ReactionStep::DAMAGESHAKE:
        time_ = 0.0f;
        floatingFloorStatus->DamageShake(animationStatus_,deltaTime);

        break;

        ///---------------------------------------------------
        /// 待機
        ///---------------------------------------------------
    case ReactionStep::CONSTANTSHAKE:
       /* time_ += deltaTime;

        if (time_ < animationStatus_->GetWaitTime()) {
            return;
        }

        animationStatus_->SetScoreUIStep(ScoreUIStep::ADAPT);*/

        break;
      
        ///---------------------------------------------------
        /// 終わり
        ///---------------------------------------------------
    case ReactionStep::END:

        break;
    default:
        break;
    }

    //// 実際にタイム加算
    // scoreStatus_->TimeIncrementAnimation(deltaTime);
    // scoreStatus_->BaseScoreUPAmplitudeScaling(deltaTime);

    // if (scoreStatus_->GetCurrentScore() <= 0.0f) {
    //     scoreStatus_->SetCurrentScore(0.0f);
    // }

    // if (scoreStatus_->GetCurrentScore() < scoreStatus_->GetScoreMax()) {
    //     return;
    // }

    // scoreStatus_->SetCurrentScore(scoreStatus_->GetScoreMax());
}

void FloatingFloorAnimationSystem::ComboReset() {
}
