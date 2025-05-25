#include "AdaptReadySystem.h"

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

AdaptReadySystem::AdaptReadySystem()
    : ISystem(SystemType::Movement) {}

AdaptReadySystem::~AdaptReadySystem() {}

void AdaptReadySystem::Initialize() {
    time_ = 0.0f;
}

void AdaptReadySystem::Finalize() {}

void AdaptReadySystem::UpdateEntity(GameEntity* _entity) {

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* blockManagerEntity           = ecsManager->getUniqueEntity("GameStartRendition");

    if (!blockManagerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    GameStartStatus* gameStartStatus = getComponent<GameStartStatus>(_entity);
    SpriteRenderer* spriteRenderer   = getComponent<SpriteRenderer>(_entity);

    if (!spriteRenderer || !gameStartStatus) {
        return;
    }

    Vec2f pos = gameStartStatus->GetReadyPos();
    /*Vec2f size = spriteRenderer->getTextureSize() * gameStartStatus->GetGoScale();*/
   /* float rotate = gameStartStatus->GetGoRotate();*/

    spriteRenderer->setTranslate(pos);
   /* spriteRenderer->setSize(size);
    spriteRenderer->setRotate(rotate);*/
}

void AdaptReadySystem::ComboReset() {
}
