#include "ResultFontSystem.h"

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
#include "component/ResultUI/ResultFontStatus.h"
#include "component/ResultUI/ResultUIParentStatus.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>

ResultFontSystem::ResultFontSystem()
    : ISystem(SystemType::Movement) {}

ResultFontSystem::~ResultFontSystem() {}

void ResultFontSystem::Initialize() {
    time_ = 0.0f;
}

void ResultFontSystem::Finalize() {}

void ResultFontSystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* levelUIParent                = ecsManager->getUniqueEntity("ResultUIParent");

    if (!levelUIParent) { // Entityが存在しない場合の早期リターン
        return;
    }

    // get timer component
    ResultFontStatus* resultFont         = getComponent<ResultFontStatus>(_entity);
    SpriteRenderer* sprite               = getComponent<SpriteRenderer>(_entity);
    ResultUIParentStatus* resultUIParent = getComponent<ResultUIParentStatus>(levelUIParent);
    float deltaTIme                      = Engine::getInstance()->getDeltaTime();

    if (!resultFont || !resultUIParent || !sprite) {
        return;
    }

    if (resultFont->GetAnimationStep() == ResultFontStep::NONE) {
        time_ = 0.0f;

        // アニメーションリセット
        resultFont->Reset();
        resultFont->SetAnimationStep(ResultFontStep::FIRSTWAIT);
    }

    switch (resultFont->GetAnimationStep()) {
    case ResultFontStep::NONE:

        break;
        ///----------------------------------------------------------------
        /// Move Animation
        ///----------------------------------------------------------------
    case ResultFontStep::FIRSTWAIT:
        time_ += deltaTIme;

        if (time_ < resultFont->GetFirstWaitTime()) {
            break;
        }
        time_ = 0.0f;
        resultFont->SetAnimationStep(ResultFontStep::ZOOM);

        break;
        ///----------------------------------------------------------------
        /// Scroll Wait
        ///----------------------------------------------------------------
    case ResultFontStep::ZOOM:
        resultFont->ZoomAnimation(deltaTIme);

        break;
        ///----------------------------------------------------------------
        /// Scroll Animation
        ///----------------------------------------------------------------
    case ResultFontStep::MOVEWAIT:
        time_ += deltaTIme;

        if (time_ < resultFont->GetMoveWaitTime()) {
            break;
        }
        time_ = 0.0f;
        resultFont->SetAnimationStep(ResultFontStep::MOVE);
        break;
        ///----------------------------------------------------------------
        /// Scaling Animation
        ///----------------------------------------------------------------
    case ResultFontStep::MOVE:
        resultFont->MoveAnimation(deltaTIme, *resultUIParent);
        break;
        ///----------------------------------------------------------------
        /// Reverse Wait
        ///----------------------------------------------------------------
    case ResultFontStep::END:
       /* resultUIParent->SetIsAnimation(true);*/
        break;

    default:
        break;
    }

    ///* ------------------------------calucration------------------------------

    Vec3f basePos  = resultFont->GetBasePos();
    Vec2f baseSize = sprite->getTextureSize() * resultFont->GetBaseScale();
    /*  float uvPos    = resultUIParent->GetCurrentLevelUV() * 0.1f;*/

    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setTranslate(Vec2f(basePos[X], basePos[Y]));
    sprite->setSize(baseSize);

}

void ResultFontSystem::ComboReset() {
}
