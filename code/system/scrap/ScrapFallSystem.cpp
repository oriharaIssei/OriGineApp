#include "ScrapFallSystem.h"

#define ENGINE_INCLUDE
#define ENGINE_COMPONENTS

#include "component/Player/PlayerStates.h"
#include"component/Scrap/ScrapStatus.h"
#include "engine/EngineInclude.h"



ScrapFallSystem::ScrapFallSystem()
    : ISystem(SystemType::Movement) {}

ScrapFallSystem::~ScrapFallSystem() {}

void ScrapFallSystem::Initialize() {}
void ScrapFallSystem::Finalize() {}

void ScrapFallSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity)
        return;

    ScrapStatus* scrapSystem = getComponent<ScrapStatus>(_entity);
    if (!scrapSystem) {
        return;
    }


  
}
