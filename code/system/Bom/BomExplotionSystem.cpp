#include "BomExplotionSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

/// app
// component
#include "component/Bom/BomStatus.h"

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

    float deltaTime      = Engine::getInstance()->getDeltaTime();
    BomStatus* bomStates = getComponent<BomStatus>(_entity);
 
    if (!bomStates) {
        return;
    }

    ///============================================================
    // 爆弾タイム加算
    ///============================================================

    bomStates->CurrentTimeIncrement(deltaTime);


   
   
}


