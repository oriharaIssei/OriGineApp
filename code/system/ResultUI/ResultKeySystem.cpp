#include "ResultKeySystem.h"

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
#include "component/ResultUI/ResultUIkeyStatus.h"
#include "component/ResultUI/ResultUIRankStatus.h"
#include "sceneManager/SceneManager.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>

ResultKeySystem::ResultKeySystem()
    : ISystem(SystemType::Movement) {}

ResultKeySystem::~ResultKeySystem() {}

void ResultKeySystem::Initialize() {
    time_ = 0.0f;
    isBackTitle_ = false;
    input_       = Input::getInstance();
    sceneManager_ = SceneManager::getInstance();
  
}

void ResultKeySystem::Finalize() {}

void ResultKeySystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* rankEntity                = ecsManager->getUniqueEntity("ResultRank");

    if (!rankEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    if (!isBackTitle_) {
        if (input_->isTriggerKey(DIK_T)) {
            isBackTitle_ = true;
            sceneManager_->changeScene("TITLE");
        }
    }

    // get timer component
    ResultUIkeyStatus* resultRank        = getComponent<ResultUIkeyStatus>(_entity);
    SpriteRenderer* sprite               = getComponent<SpriteRenderer>(_entity);
    ResultUIRankStatus* resultUIParent   = getComponent<ResultUIRankStatus>(rankEntity);
    float deltaTIme                      = Engine::getInstance()->getDeltaTime();

    if (!resultRank || !resultUIParent || !sprite) {
        return;
    }

    if (resultRank->GetAnimationStep() == KeyResultStep::NONE&& resultUIParent->GetAnimationStep()==RankAnimationStep::END) {
        time_ = 0.0f;

        // アニメーションリセット
        resultRank->Reset();
        resultRank->SetAnimationStep(KeyResultStep::ALPHA);
    }

    switch (resultRank->GetAnimationStep()) {
    case KeyResultStep::NONE:

        break;
        ///----------------------------------------------------------------
        /// Move Animation
        ///----------------------------------------------------------------
    case KeyResultStep::ALPHA:
      
        resultRank->AlphaAnimation(deltaTIme);
        resultRank->MoveAnimation(deltaTIme);

        break;
        ///----------------------------------------------------------------
        /// Scroll Wait
        ///----------------------------------------------------------------
    case KeyResultStep::END:
        resultRank->AlphaAnimation(deltaTIme);
        break;
        ///----------------------------------------------------------------
        /// Scroll Animation
        ///----------------------------------------------------------------
   
    default:
        break;
    }

    ///* ------------------------------calucration------------------------------

    Vec3f basePos  = resultRank->GetBasePos();
    Vec2f baseSize = sprite->getTextureSize(); /** resultRank->gets();*/
  /*  resultRank->SetRankForScore(resultUIParent->GetCurrentScore());*/

    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setTranslate(Vec2f(basePos[X], basePos[Y]));
    sprite->setSize(baseSize);
    sprite->setColor(Vec4f(1.0f, 1.0, 1.0f, resultRank->GetAlpha()));

}

void ResultKeySystem::ComboReset() {
}
