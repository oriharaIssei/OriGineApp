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
#include"component/Bom/BomSpawner.h"
#include"component/Menu/MenuStatus.h"

// system
// #include "system/CharacterOnCollision.h"
// #include "system/DeleteCharacterEntitySystem.h"

void BomExplotionSystem::Initialize() {
    input_ = Input::getInstance();
}

void BomExplotionSystem::Finalize() {
   /* entities_.clear();*/
}

void BomExplotionSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }
  
    // ポーズ中は通さない
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* menuEntity                   = ecsManager->getUniqueEntity("Menu");

    if (!menuEntity) {
        return;
    }

    MenuStatus* menu = getComponent<MenuStatus>(menuEntity);

    if (!menu) {
        return;
    }

    if (menu->GetIsPose()) {
        return;
    }

    

    // PlayerEntityを取得
     GameEntity* playerEntity                 = ecsManager->getUniqueEntity("Player");

    /*  float deltaTime      = Engine::getInstance()->getDeltaTime();*/
    bomStates_ = getComponent<BomStatus>(_entity);
    BomSpawner* bomSpawner = getComponent<BomSpawner>(playerEntity);

    if (!bomStates_ || !playerEntity || !bomSpawner) {
        return;
    }

    // 打ち上げ処理
    LaunchMethod(_entity);

  /*  PlayerStates* playerStates = getComponent<PlayerStates>(playerEntity);*/

    ///============================================================
    // 爆発していい爆弾なら起爆
    ///============================================================
    if (input_->isTriggerKey(DIK_SPACE)) {
        bomStates_->SetIsExplotion(true);
       
    }
    /*  bomStates->CurrentTimeIncrement(deltaTime);*/
}

void BomExplotionSystem::LaunchMethod(GameEntity* _entity) {
    float deltaTime      = Engine::getInstance()->getDeltaTime();
    Transform* transform = getComponent<Transform>(_entity);
    bomStates_->Rotating(deltaTime);

   
    transform->translate[Y] += bomStates_->GetLaunghSpeed() * Engine::getInstance()->getDeltaTime();
    transform->translate[Z] = -1.0f;

        // Quaternionに変換
    Quaternion q = Quaternion::FromEulerAngles(0.0f, bomStates_->GetRotateY(), 0.0f);
    // クォータニオンで回転をセット
    transform->rotate = q;
}
