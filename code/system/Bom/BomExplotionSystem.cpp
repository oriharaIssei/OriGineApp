#include "BomExplotionSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
#include"input/Input.h"

/// app
// component
#include "component/Bom/BomStatus.h"
#include"component/Player/PlayerStates.h"

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
    BomStatus* bomStates = getComponent<BomStatus>(_entity);
    PlayerStates* playerStates = getComponent<PlayerStates>(playerEntity);
 
    if (!bomStates || !playerEntity) {
        return;
    }

    ///============================================================
    // 爆発していい爆弾なら起爆
    ///============================================================
    if (input_->isTriggerKey(DIK_B)) {

        if (playerStates->GetBomExplotionNum() >= playerStates->GetBomExplotionNum()) {
            playerStates->SetincrementBomExplotionNum();
            bomStates->SetIsExplotion(true);
        }
    }
  /*  bomStates->CurrentTimeIncrement(deltaTime);*/

    
   
   
}


