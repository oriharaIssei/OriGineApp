#include "GameStartBackGroundSystem.h"

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

GameStartBackGroundSystem::GameStartBackGroundSystem()
    : ISystem(SystemType::Movement) {}

GameStartBackGroundSystem::~GameStartBackGroundSystem() {}

void GameStartBackGroundSystem::Initialize() {
    time_ = 0.0f;
}

void GameStartBackGroundSystem::Finalize() {}

void GameStartBackGroundSystem::UpdateEntity(GameEntity* _entity) {

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* gameStartEntity           = ecsManager->getUniqueEntity("GameStartRendition");

    if (!gameStartEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    GameStartStatus* gameStartStatus = getComponent<GameStartStatus>(gameStartEntity);
    SpriteRenderer* spriteRenderer   = getComponent<SpriteRenderer>(_entity);

    if (!spriteRenderer || !gameStartStatus) {
        return;
    }

    if (gameStartStatus->GetIsBackApear() == true) {
        spriteRenderer->setColor(Vec4f(1.0f,1.0f,1.0f,0.4f));
    } else {
        spriteRenderer->setColor(Vec4f(1.0f, 1.0f, 1.0f, 0.0f));
    }
   /* Vec2f size = spriteRenderer->getTextureSize() * gameStartStatus->GetGoScale();
    float rotate = gameStartStatus->GetGoRotate();

    spriteRenderer->setSize(size);
    spriteRenderer->setRotate(rotate);*/
}

void GameStartBackGroundSystem::ComboReset() {
}
