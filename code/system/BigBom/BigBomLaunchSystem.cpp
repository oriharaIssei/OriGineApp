#include "BigBomLaunchSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include "component/BigBom/BigBomStatus.h"
#include "component/Menu/MenuStatus.h"
#include "component/Player/PlayerStates.h"

#include <Vector3.h>

BigBomLaunchSystem::BigBomLaunchSystem()
    : ISystem(SystemType::Movement) {}

BigBomLaunchSystem::~BigBomLaunchSystem() {}

void BigBomLaunchSystem::Initialize() {
    input_ = Input::getInstance();
}
void BigBomLaunchSystem::Finalize() {}

void BigBomLaunchSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity)
        return;

    // ポーズ中は通さない
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* menuEntity                   = ecsManager->getUniqueEntity("Menu");
  /*  GameEntity* playerEntity                   = ecsManager->getUniqueEntity("Player");*/

    if (!menuEntity /*|| !playerEntity*/) {
        return;
    }

    MenuStatus* menu = getComponent<MenuStatus>(menuEntity);

    if (!menu) {
        return;
    }

    if (menu->GetIsPose()) {
        return;
    }

    Transform* transform       = getComponent<Transform>(_entity);
    BigBomStatus* bigBomStatus = getComponent<BigBomStatus>(_entity);
   
    if (!transform || !bigBomStatus)
        return;

    if (!bigBomStatus->GetIsLaunch())
        return;

    float deltaTime = Engine::getInstance()->getDeltaTime();

    // LaunchDirection に向かって進行
    Vec3f direction = bigBomStatus->GetLaunchDirection(); // 正規化済みを想定
    float speed     = bigBomStatus->GetLaunghSpeed();

    transform->translate += direction * speed * deltaTime;

  
   
}
