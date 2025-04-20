#include "BomExplotionSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
#include "input/Input.h"

/// app
// component
#include "component/Bom/BomStatus.h"
#include "component/Player/PlayerStates.h"

// system
// #include "system/CharacterOnCollision.h"
// #include "system/DeleteCharacterEntitySystem.h"

void BomExplotionSystem::Initialize() {}

void BomExplotionSystem::Finalize() {
    entities_.clear();
}

void BomExplotionSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }
    input_ = Input::getInstance();

    // PlayerEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* playerEntity                 = ecsManager->getUniqueEntity("Player");

    /*  float deltaTime      = Engine::getInstance()->getDeltaTime();*/
     bomStates_ = getComponent<BomStatus>(_entity);

    if (!bomStates_ || !playerEntity) {
        return;
    }

    // 打ち上げ処理
    LaunchMethod(_entity);

    PlayerStates* playerStates = getComponent<PlayerStates>(playerEntity);

    ///============================================================
    // 爆発していい爆弾なら起爆
    ///============================================================
    if (input_->isTriggerKey(DIK_J)) {

        if (playerStates->GetBomExplotionNum() >= bomStates_->GetBomNumber()) {

            bomStates_->SetIsExplotion(true);
        }
    }
    /*  bomStates->CurrentTimeIncrement(deltaTime);*/
}

void BomExplotionSystem::LaunchMethod(GameEntity* _entity) {

    Transform* transform = getComponent<Transform>(_entity);

    transform->translate[Y] += bomStates_->GetLaunghSpeed() * Engine::getInstance()->getDeltaTime();
    /*transform->rotate[Z] += bomStates_->GetLaunghSpeed() * Engine::getInstance()->getDeltaTime();*/


 }
