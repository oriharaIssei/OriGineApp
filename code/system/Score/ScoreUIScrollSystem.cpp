#include "ScoreUIScrollSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Vector3.h>
// component
#include "component/Score/ScoreStatus.h"
#include "component/Score/ScoreUIStatus.h"

#define RESOURCE_DIRECTORY
#include "engine/EngineInclude.h"
#include <cstdint>

ScoreUIScrollSystem::ScoreUIScrollSystem()
    : ISystem(SystemType::Movement) {}

ScoreUIScrollSystem::~ScoreUIScrollSystem() {}

void ScoreUIScrollSystem::Initialize() {
    //// TextureNameの初期化
    // for (int32_t i = 0; i < 10; ++i) {
    //     textureName_[i] = "Texture/Combo/ComboNumber" + std::to_string(i) + ".png";
    // };
}

void ScoreUIScrollSystem::Finalize() {}

void ScoreUIScrollSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* scoreEntity                  = ecsManager->getUniqueEntity("Score");

    if (!scoreEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    /// component取得
    ScoreStatus* scoreStatus     = getComponent<ScoreStatus>(scoreEntity);
    ScoreUIStatus* scoreUIStatus = getComponent<ScoreUIStatus>(_entity);
    SpriteRenderer* spriteRender = getComponent<SpriteRenderer>(_entity);

    if (!scoreStatus || !scoreUIStatus || !spriteRender) { // Componentが存在しない場合の早期リターン
        return;
    }

    Vec2f size = scoreStatus->GetBaseTextureSize() * scoreStatus->GetBaseScoreScale();
    spriteRender->setSize(size);

    // 現在タイムの取得
    float curerntScore = scoreStatus->GetCurrentScore();
    int32_t scoreDigit = scoreUIStatus->GetValueForDigit(curerntScore);

    // UV座標を設定
    spriteRender->setUVTranslate(Vec2f(float(scoreDigit * 0.1f), 0.0f));
}
