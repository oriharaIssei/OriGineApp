#include "ScrapGotDeleteSystem.h"

#define ENGINE_INCLUDE
#define ENGINE_COMPONENTS

#include "component/Player/PlayerStates.h"
#include "engine/EngineInclude.h"



ScrapToPlayerCollisionSystem::ScrapToPlayerCollisionSystem()
    : ISystem(SystemType::Collision) {}

ScrapToPlayerCollisionSystem::~ScrapToPlayerCollisionSystem() {}

void ScrapToPlayerCollisionSystem::Initialize() {}
void ScrapToPlayerCollisionSystem::Finalize() {}


void ScrapToPlayerCollisionSystem::UpdateEntity(GameEntity* _entity) {
    
}

