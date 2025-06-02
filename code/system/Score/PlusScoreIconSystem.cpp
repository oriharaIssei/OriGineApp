#include "PlusScoreIconSystem.h"

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
#include <cmath>
#include <cstdint>

PlusScoreIconSystem::PlusScoreIconSystem()
    : ISystem(SystemType::Movement) {}

PlusScoreIconSystem::~PlusScoreIconSystem() {}

void PlusScoreIconSystem::Initialize() {
    //// TextureNameの初期化
    // for (int32_t i = 0; i < 10; ++i) {
    //     textureName_[i] = "Texture/Combo/ComboNumber" + std::to_string(i) + ".png";
    // };
}

void PlusScoreIconSystem::Finalize() {}

void PlusScoreIconSystem::UpdateEntity(GameEntity* _entity) {

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

    Vec2f size = scoreStatus->GetBasePlusScoreTextureSize() * scoreStatus->GetPlusScoreScale();
    spriteRender->setSize(size);

    spriteRender->setUVScale(Vec2f(0.5f, 1.0f));

    // UV座標を設定
    if (scoreStatus->GetPlusScoreValue() < 0.0f) {
        spriteRender->setUVTranslate(Vec2f(float(0.5f), 0.0f));
    } else {
        spriteRender->setUVTranslate(Vec2f(float(0.0f), 0.0f));
    }

    if (scoreStatus->GetPlusScoreValue() == 0.0f) {
        spriteRender->setColor(Vec4f(1.0f, 1.0f, 1.0f, 0.0f));
    } else {
        spriteRender->setColor(Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    }
}
