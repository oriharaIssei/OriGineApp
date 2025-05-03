#include "TutorialMenuParentSystem.h"

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
#include "component/Menu/TutorialMenuParentStatus.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>

TutorialMenuParentSystem::TutorialMenuParentSystem()
    : ISystem(SystemType::Movement) {}

TutorialMenuParentSystem::~TutorialMenuParentSystem() {}

void TutorialMenuParentSystem::Initialize() {
    time_  = 0.0f;
    input_ = Input::getInstance();
}

void TutorialMenuParentSystem::Finalize() {}

void TutorialMenuParentSystem::UpdateEntity(GameEntity* _entity) {

    //// ComboEntityを取得
    //EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    //GameEntity* rankEntity                   = ecsManager->getUniqueEntity("ResultRank");

    //if (!rankEntity) { // Entityが存在しない場合の早期リターン
    //    return;
    //}

    // get timer component
    TutorialMenuParentStatus* tutorialMenu = getComponent<TutorialMenuParentStatus>(_entity);
  /*  SpriteRenderer* sprite                 = getComponent<SpriteRenderer>(_entity);
  */  /*  ResultUIRankStatus* resultUIParent   = getComponent<ResultUIRankStatus>(rankEntity);*/
    float deltaTIme = Engine::getInstance()->getDeltaTime();

    if (!tutorialMenu) {
        return;
    }

    if (tutorialMenu->GetIsAnimation()&&tutorialMenu->GetAnimationStep() == ScrollStep::NONE) {
        time_ = 0.0f;
        // アニメーションリセット
        tutorialMenu->Reset();
        tutorialMenu->SetAnimationStep(ScrollStep::FIRSTUVSCROLL);
    }

    switch (tutorialMenu->GetAnimationStep()) {
    case ScrollStep::NONE:

        break;
        ///----------------------------------------------------------------
        /// Move Animation
        ///----------------------------------------------------------------
    case ScrollStep::FIRSTUVSCROLL:

        tutorialMenu->FirstMoveAnimation(deltaTIme);

        break;
        ///----------------------------------------------------------------
        /// Scroll Wait
        ///----------------------------------------------------------------
    case ScrollStep::PAUGESELECTION:
        //pauge up
        if (input_->isTriggerKey(DIK_D) || input_->isTriggerKey(DIK_RIGHT)) {
            tutorialMenu->ScrollTimeReset();
            tutorialMenu->SetAnimationStep(ScrollStep::PAUGEUP);

            // pauge down
        } else if (input_->isTriggerKey(DIK_A) || input_->isTriggerKey(DIK_LEFT)) {
            tutorialMenu->ScrollTimeReset();
            tutorialMenu->SetAnimationStep(ScrollStep::PAUGEDOWN);
        }

        //back
        else if (input_->isTriggerKey(DIK_ESCAPE)) {
            tutorialMenu->SetAnimationStep(ScrollStep::BACK);
        }
        break;
        ///----------------------------------------------------------------
        /// UP Animation
        ///----------------------------------------------------------------
    case ScrollStep::PAUGEUP:

        tutorialMenu->MoveAnimation(deltaTIme);

        break;
        ///----------------------------------------------------------------
        /// Down Animation
        ///----------------------------------------------------------------
    case ScrollStep::PAUGEDOWN:

        tutorialMenu->MoveAnimation(deltaTIme);

        break;
        ///----------------------------------------------------------------
        /// Down Animation
        ///----------------------------------------------------------------
    case ScrollStep::BACK:

        tutorialMenu->BackSizeAnimation(deltaTIme);

        break;
    case ScrollStep::END:
        tutorialMenu->SetIsAnimation(false);
        tutorialMenu->Reset();
       
        break;
    default:
        break;
    }

    /////* ------------------------------calucration------------------------------

    //Vec2f basePos  = tutorialMenu->GetBasePos();
    //Vec2f baseSize = sprite->getTextureSize(); /** resultRank->gets();*/
    ///*  resultRank->SetRankForScore(resultUIParent->GetCurrentScore());*/

    /////* ------------------------------adapt------------------------------

    //// pos
    //sprite->setTranslate(basePos);
    //sprite->setSize(baseSize);
    //sprite->setUVScale(Vec2f(tutorialMenu->GetUVScale(),1.0f));
}

void TutorialMenuParentSystem::ComboReset() {
}
