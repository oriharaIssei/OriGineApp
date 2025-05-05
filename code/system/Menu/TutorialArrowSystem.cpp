#include "TutorialArrowSystem.h"

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
#include "component/Menu/TutorialArrowStatus.h"

#include "engine/EngineInclude.h"
#include <cstdint>
#include <Vector.h>
#include <Vector2.h>

TutorialArrowSystem::TutorialArrowSystem()
    : ISystem(SystemType::Movement) {}

TutorialArrowSystem::~TutorialArrowSystem() {}

void TutorialArrowSystem::Initialize() {
}

void TutorialArrowSystem::Finalize() {}

void TutorialArrowSystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* tutorialParentEntity         = ecsManager->getUniqueEntity("TutorialMenuParent");

    if (!tutorialParentEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    deltaTime_ = Engine::getInstance()->getDeltaTime();

    // get timer component
    TutorialArrowStatus* arrowStatus          = getComponent<TutorialArrowStatus>(_entity);
    SpriteRenderer* sprite                     = getComponent<SpriteRenderer>(_entity);
    TutorialMenuParentStatus* tutorialUIParent = getComponent<TutorialMenuParentStatus>(tutorialParentEntity);

    if (!arrowStatus || !tutorialUIParent || !sprite) {
        return;
    }

    if (tutorialUIParent->GetAnimationStep() == ScrollStep::NONE) {
        arrowStatus->Reset();
        return;
    }

    //イージング
    arrowStatus->ArrowMoveEasing(deltaTime_);

    ///* ------------------------------calucration------------------------------

    Vec2f basePos  = arrowStatus->GetBasePos();
    Vec2f baseSize = Vec2f(
        sprite->getTextureSize()[X] ,
        sprite->getTextureSize()[Y] * tutorialUIParent->GetScaleY()); 
    /*  resultRank->SetRankForScore(resultUIParent->GetCurrentScore());*/

    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setTranslate(basePos);
    sprite->setSize(baseSize);
}

void TutorialArrowSystem::ComboReset() {
}
