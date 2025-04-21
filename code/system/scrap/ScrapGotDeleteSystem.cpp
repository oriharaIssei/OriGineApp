#include "ScrapGotDeleteSystem.h"

#define ENGINE_INCLUDE
#define ENGINE_COMPONENTS

#include "component/Player/PlayerStates.h"
#include "engine/EngineInclude.h"



ScrapGotDeleteSystem::ScrapGotDeleteSystem()
    : ISystem(SystemType::Collision) {}

ScrapGotDeleteSystem::~ScrapGotDeleteSystem() {}

void ScrapGotDeleteSystem::Initialize() {}
void ScrapGotDeleteSystem::Finalize() {}


void ScrapGotDeleteSystem::UpdateEntity(GameEntity* _entity) {
    
}

