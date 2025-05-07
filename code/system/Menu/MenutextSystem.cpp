#include "MenutextSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// component
#include "component/Menu/MenuStatus.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>

MenutextSystem::MenutextSystem()
    : ISystem(SystemType::Movement) {}

MenutextSystem::~MenutextSystem() {}

void MenutextSystem::Initialize() {
    time_  = 0.0f;
    input_ = Input::getInstance();
}

void MenutextSystem::Finalize() {}

void MenutextSystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* menuEntity                   = ecsManager->getUniqueEntity("Menu");

    if (!menuEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    // get timer component

    SpriteRenderer* sprite = getComponent<SpriteRenderer>(_entity);
    MenuStatus* menuStatus = getComponent<MenuStatus>(menuEntity);
    /*  float deltaTIme = Engine::getInstance()->getDeltaTime();*/

    if (!sprite || !menuStatus) {
        return;
    }

    ///* ------------------------------calucration------------------------------

    Vec2f baseSize = sprite->getTextureSize() * menuStatus->GetBaseScale();
    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setSize(baseSize);
}

void MenutextSystem::ComboReset() {
}
