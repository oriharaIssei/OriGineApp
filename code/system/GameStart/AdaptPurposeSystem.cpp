#include "AdaptPurposeSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
// ECS
#define ENGINE_ECS
// include

// component
#include "component/GameStart/GameStartStatus.h"

#include "engine/EngineInclude.h"

AdaptPurposeSystem::AdaptPurposeSystem()
    : ISystem(SystemType::Movement) {}

AdaptPurposeSystem::~AdaptPurposeSystem() {}

void AdaptPurposeSystem::Initialize() {
    time_ = 0.0f;
}

void AdaptPurposeSystem::Finalize() {}

void AdaptPurposeSystem::UpdateEntity(GameEntity* _entity) {

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* gameStartEntity              = ecsManager->getUniqueEntity("GameStartRendition");

    if (!gameStartEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    GameStartStatus* gameStartStatus = getComponent<GameStartStatus>(gameStartEntity);
    SpriteRenderer* spriteRenderer   = getComponent<SpriteRenderer>(_entity);

    if (!spriteRenderer || !gameStartStatus) {
        return;
    }

    Vec2f basePos = gameStartStatus->GetPurposePos();

    spriteRenderer->setTranslate(basePos);
}

void AdaptPurposeSystem::ComboReset() {
}
