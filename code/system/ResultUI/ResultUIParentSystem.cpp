#include "ResultUIParentSystem.h"

/// Engine
#define ENGINE_INCLUDE
#define ENGINE_INPUT
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/ResultUI/ResultUIParentStatus.h"
#include "component/Score/ScoreStatus.h"

#include "engine/EngineInclude.h"

ResultUIParentSystem::ResultUIParentSystem()
    : ISystem(SystemType::Movement) {}

ResultUIParentSystem::~ResultUIParentSystem() {}

void ResultUIParentSystem::Initialize() {
    time_  = 0.0f;
    input_ = Input::getInstance();
}

void ResultUIParentSystem::Finalize() {}

void ResultUIParentSystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* scoreEntity                  = ecsManager->getUniqueEntity("Score");
    if (!scoreEntity) {
        return;
    }

    ScoreStatus* scoreStatus = getComponent<ScoreStatus>(scoreEntity);

    // get timer component
    ResultUIParentStatus* resultUIParent = getComponent<ResultUIParentStatus>(_entity);
    SpriteRenderer* sprite               = getComponent<SpriteRenderer>(_entity);
    Audio* paperApearAudio                    = getComponent<Audio>(_entity);
    Audio* scoreDesideAudio              = getComponent<Audio>(_entity,1);
    float deltaTIme                      = Engine::getInstance()->getDeltaTime();

    if (!resultUIParent) {
        return;
    }

 

    switch (resultUIParent->GetAnimationStep()) {
    case ResultStep::NONE:

        if (!resultUIParent->GetIsAnimation()) {
            resultUIParent->Reset();
            break;
        }

        time_ = 0.0f;
        resultUIParent->SetResultScore(scoreStatus->GetCurrentScore());
        paperApearAudio->Play();
        // アニメーションリセット
        resultUIParent->Reset();
        resultUIParent->SetAnimationStep(ResultStep::ALPHA);
        
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
        if (input_->isTriggerKey(DIK_SPACE)) {
            resultUIParent->SetIsScoreSkip(true);
        }
        resultUIParent->ScoreUP(deltaTIme, scoreDesideAudio);
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

    ///* ------------------------------calucration------------------------------

    Vec3f basePos  = resultUIParent->GetBasePos();
    Vec2f baseSize = sprite->getTextureSize() * resultUIParent->GetBaseScale();
    /*  float uvPos    = resultUIParent->GetCurrentLevelUV() * 0.1f;*/

    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setTranslate(Vec2f(basePos[X], basePos[Y]));
    // scale
    sprite->setSize(baseSize);
    // alpha
    sprite->setColor(Vec4f(1.0f, 1.0f, 1.0f, resultUIParent->GetAlpha()));
}

void ResultUIParentSystem::ComboReset() {
}
