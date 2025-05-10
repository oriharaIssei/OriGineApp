#include "SceneTransitionSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include

// component
#include "component/GameEnd/SceneChangerStatus.h"
#include "component/SceneTransition/SceneTransition.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>

SceneTransitionSystem::SceneTransitionSystem()
    : ISystem(SystemType::Movement) {}

SceneTransitionSystem::~SceneTransitionSystem() {}

void SceneTransitionSystem::Initialize() {
    time_  = 0.0f;
    input_ = Input::getInstance();
}

void SceneTransitionSystem::Finalize() {}

void SceneTransitionSystem::UpdateEntity(GameEntity* entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* endEntity                    = ecsManager->getUniqueEntity("GameEnd");

    if (!endEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    SpriteRenderer* sprite            = getComponent<SpriteRenderer>(entity);
    SceneTransition* sceneTransition = getComponent<SceneTransition>(entity);
    SceneChangerStatus* gameend       = getComponent<SceneChangerStatus>(endEntity);

   
    if (!gameend || !sprite || !sceneTransition) {
        return;
    }

    float deltaTime = Engine::getInstance()->getDeltaTime();

    //ボタンによるシーン切り替え

    //ステップに基づく動き
    switch (sceneTransition->GetTransitionStep()) {
    case TransitonStep::NONE:
        if (!sceneTransition->IsAbleAnimationStart()||!sceneTransition->IsAbleTitleOutAnimationStart()) {
            sceneTransition->TransitionInit();
            break;
        }
        sceneTransition->Reset();
        sceneTransition->SetTransitionStep(TransitonStep::MOVE);

        break;

    case TransitonStep::MOVE:

       sceneTransition->UpdateTransition(deltaTime);

        break;

    case TransitonStep::WAIT:
        sceneTransition->WaitUpdate(deltaTime);
        break;

    case TransitonStep::END:
        sceneTransition->GoToNextScene(gameend);
        break;

    default:
        break;
    }

    ///* ------------------------------calucration------------------------------

    Vec2f basePos  = Vec2f(sceneTransition->TransitionPosX(),0.0f);
    Vec2f baseSize = sprite->getTextureSize() * sceneTransition->GetScale();
    /*  resultRank->SetRankForScore(resultUIParent->GetCurrentScore());*/

    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setTranslate(basePos);
    sprite->setSize(baseSize);
    /* sprite->setUVScale(Vec2f(tutorialMenu->GetUVScale(),1.0f));*/
}

void SceneTransitionSystem::ComboReset() {
}
