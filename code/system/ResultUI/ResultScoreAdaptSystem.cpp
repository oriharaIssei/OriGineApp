#include "ResultScoreAdaptSystem.h"

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
#include "component/ResultUI/ResultUIScoreStatus.h"

#include "engine/EngineInclude.h"
#include <Vector.h>
#include <Vector2.h>
#include <cstdint>

ResultScoreAdaptSystem::ResultScoreAdaptSystem()
    : ISystem(SystemType::Movement) {}

ResultScoreAdaptSystem::~ResultScoreAdaptSystem() {}

void ResultScoreAdaptSystem::Initialize() {
}

void ResultScoreAdaptSystem::Finalize() {}

void ResultScoreAdaptSystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* levelUIParent                = ecsManager->getUniqueEntity("ResultUIParent");

    if (!levelUIParent) { // Entityが存在しない場合の早期リターン
        return;
    }

    // get timer component
    ResultUIScoreStatus* resultScoreUI       = getComponent<ResultUIScoreStatus>(_entity);
    SpriteRenderer* sprite                   = getComponent<SpriteRenderer>(_entity);
    ResultUIParentStatus* resultUIParent = getComponent<ResultUIParentStatus>(levelUIParent);

    if (!resultScoreUI || !resultUIParent || !sprite) {
        return;
    }

    ///* ------------------------------calucration------------------------------

    Vec3f basePos  = resultUIParent->GetBasePos() + resultScoreUI->GetOffsetPos();
    Vec2f baseSize = resultScoreUI->GetTextureSize() * resultUIParent->GetScoreScale();
  /*  float uvPos    = resultUIParent->GetCurrentLevelUV() * 0.1f;*/

    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setTranslate(Vec2f(basePos[X], basePos[Y]));
    // scale
    sprite->setSize(baseSize);
    //alpha
    sprite->setColor(Vec4f(1.0f, 1.0f, 1.0f, resultUIParent->GetAlpha()));
    // uv
     // 現在タイムの取得
    float curerntScore = resultUIParent->GetCurrentScore();
    int32_t scoreDigit = resultScoreUI->GetValueForDigit(int32_t(curerntScore));

    // UV座標を設定
    sprite->setUVTranslate(Vec2f(float(scoreDigit * 0.1f), 0.0f));
}

void ResultScoreAdaptSystem::ComboReset() {
}
