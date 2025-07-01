#include "TutorialMenuParentSystem.h"

/// Engine
#define ENGINE_INCLUDE
#define ENGINE_INPUT
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
// component
#include "component/Menu/TutorialMenuParentStatus.h"
#include"component/Menu/MenuStatus.h"

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
    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* menuEntity                   = ecsManager->getUniqueEntity("Menu");

    if (!menuEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    // get timer component
    TutorialMenuParentStatus* tutorialMenu = getComponent<TutorialMenuParentStatus>(_entity);
    MenuStatus* menu = getComponent<MenuStatus>(menuEntity);

     // Audio
    Audio* audioOpenTutorial = getComponent<Audio>(_entity);
    Audio* selectPauge       = getComponent<Audio>(_entity, 1);
    Audio* closeTutorial     = getComponent<Audio>(_entity, 2);
   
    if (!tutorialMenu || !menu || !audioOpenTutorial || !selectPauge || !closeTutorial) {
        return;
    }

   

     float deltaTIme = Engine::getInstance()->getDeltaTime();


    switch (tutorialMenu->GetAnimationStep()) {
    case ScrollStep::NONE:
        if (!tutorialMenu->GetIsAnimation()) {
            break;
        }
            time_ = 0.0f;
            // アニメーションリセット
            tutorialMenu->Reset();
            audioOpenTutorial->Play();
            tutorialMenu->SetAnimationStep(ScrollStep::FIRSTUVSCROLL);
        
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
            selectPauge->Play();
            tutorialMenu->ScrollTimeReset();
            tutorialMenu->SetAnimationStep(ScrollStep::PAUGEUP);

            // pauge down
        } else if (input_->isTriggerKey(DIK_A) || input_->isTriggerKey(DIK_LEFT)) {
            selectPauge->Play();
            tutorialMenu->ScrollTimeReset();
            tutorialMenu->SetAnimationStep(ScrollStep::PAUGEDOWN);
        }

        //back
        else if (input_->isTriggerKey(DIK_ESCAPE) || input_->isTriggerKey(DIK_SPACE)) {
            closeTutorial->Play();
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
        menu->SetMenuMode(MenuMode::MENUSELECT);
        tutorialMenu->SetAnimationStep(ScrollStep::NONE);
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
