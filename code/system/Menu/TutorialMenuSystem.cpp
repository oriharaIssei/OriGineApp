#include "TutorialMenuSystem.h"

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
#include "component/Menu/TutorialMenuStatus.h"

#include "engine/EngineInclude.h"
#include <cstdint>
#include <Vector.h>
#include <Vector2.h>

TutorialMenuSystem::TutorialMenuSystem()
    : ISystem(SystemType::Movement) {}

TutorialMenuSystem::~TutorialMenuSystem() {}

void TutorialMenuSystem::Initialize() {
}

void TutorialMenuSystem::Finalize() {}

void TutorialMenuSystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* tutorialParentEntity         = ecsManager->getUniqueEntity("TutorialMenuParent");

    if (!tutorialParentEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    // get timer component
    TutorialMenuStatus* tutorialMenu           = getComponent<TutorialMenuStatus>(_entity);
    SpriteRenderer* sprite                     = getComponent<SpriteRenderer>(_entity);
    TutorialMenuParentStatus* tutorialUIParent = getComponent<TutorialMenuParentStatus>(tutorialParentEntity);

    if (!tutorialMenu || !tutorialUIParent || !sprite) {
        return;
    }
    ///* ------------------------------calucration------------------------------

    Vec2f basePos  = tutorialUIParent->GetBasePos() + tutorialMenu->GetOffsetPos();
    Vec2f baseSize = sprite->getTextureSize(); /** resultRank->gets();*/
    /*  resultRank->SetRankForScore(resultUIParent->GetCurrentScore());*/

    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setTranslate(basePos);
    sprite->setSize(baseSize);
    sprite->setUVScale(Vec2f(tutorialUIParent->GetUVScale(), 1.0f));
}

void TutorialMenuSystem::ComboReset() {
}
