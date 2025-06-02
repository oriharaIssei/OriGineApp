#include "ResultRankSystem.h"

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
#include "component/ResultUI/ResultUIParentStatus.h"
#include "component/ResultUI/ResultUIRankStatus.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>

ResultRankSystem::ResultRankSystem()
    : ISystem(SystemType::Movement) {}

ResultRankSystem::~ResultRankSystem() {}

void ResultRankSystem::Initialize() {
    time_ = 0.0f;
}

void ResultRankSystem::Finalize() {}

void ResultRankSystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* levelUIParent                = ecsManager->getUniqueEntity("ResultUIParent");

    if (!levelUIParent) { // Entityが存在しない場合の早期リターン
        return;
    }

    // get timer component
    ResultUIRankStatus* resultRank       = getComponent<ResultUIRankStatus>(_entity);
    SpriteRenderer* sprite               = getComponent<SpriteRenderer>(_entity);
    Audio* rankUpSystem                  = getComponent<Audio>(_entity);
    ResultUIParentStatus* resultUIParent = getComponent<ResultUIParentStatus>(levelUIParent);
    float deltaTIme                      = Engine::getInstance()->getDeltaTime();

    if (!resultRank || !resultUIParent || !sprite) {
        return;
    }

    switch (resultRank->GetAnimationStep()) {
    case RankAnimationStep::NONE:
        if (resultUIParent->GetAnimationStep() != ResultStep::END) {
            return;
        }
        time_ = 0.0f;

        // アニメーションリセット
        resultRank->Reset();
        rankUpSystem->Play();
        resultRank->SetAnimationStep(RankAnimationStep::SCALING);
        break;
        ///----------------------------------------------------------------
        /// Move Animation
        ///----------------------------------------------------------------
    case RankAnimationStep::SCALING:

        resultRank->ScalingEasing(deltaTIme);
        resultRank->RotateEasing(deltaTIme);

        break;
        ///----------------------------------------------------------------
        /// Scroll Wait
        ///----------------------------------------------------------------
    case RankAnimationStep::END:
        resultRank->RotateEasing(deltaTIme);
        break;
        ///----------------------------------------------------------------
        /// Scroll Animation
        ///----------------------------------------------------------------

    default:
        break;
    }

    ///* ------------------------------calucration------------------------------

    Vec3f basePos  = resultRank->GetPosition();
    Vec2f baseSize = resultRank->GetTextureSize() * resultRank->GetScale();
    resultRank->SetRankForScore(resultUIParent->GetCurrentScore());

    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setTranslate(Vec2f(basePos[X], basePos[Y]));
    sprite->setRotate(resultRank->GetRotate());
    sprite->setSize(baseSize);
    sprite->setUVTranslate(Vec2f(0.0f, resultRank->GetUVPos()));
}

void ResultRankSystem::ComboReset() {
}
