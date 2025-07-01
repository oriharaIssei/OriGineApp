#include "MenuArrowSystem.h"

/// Engine
#define ENGINE_INCLUDE
#define ENGINE_INPUT
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// component
#include "component/Menu/MenuStatus.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>

MenuArrowSystem::MenuArrowSystem()
    : ISystem(SystemType::Movement) {}

MenuArrowSystem::~MenuArrowSystem() {}

void MenuArrowSystem::Initialize() {
    time_  = 0.0f;
    input_ = Input::getInstance();
}

void MenuArrowSystem::Finalize() {}

void MenuArrowSystem::UpdateEntity(GameEntity* _entity) {

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

    Vec2f basePos  = Vec2f(menuStatus->GetArrowPos()[X] + menuStatus->GetArrowOffset(), menuStatus->GetArrowPos()[Y]);
    Vec2f baseSize = sprite->getTextureSize() * menuStatus->GetBaseScale();
    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setTranslate(basePos);
    sprite->setSize(baseSize);
}

void MenuArrowSystem::ComboReset() {
}
