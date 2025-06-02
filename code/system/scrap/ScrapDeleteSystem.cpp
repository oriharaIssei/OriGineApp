#include "ScrapDeleteSystem.h"

#define ENGINE_INCLUDE
#define ENGINE_COMPONENTS

#include "component/Player/PlayerStates.h"
#include"component/Scrap/ScrapStatus.h"
#include "engine/EngineInclude.h"



ScrapDeleteSystem::ScrapDeleteSystem()
    : ISystem(SystemType::StateTransition) {}

ScrapDeleteSystem::~ScrapDeleteSystem() {}

void ScrapDeleteSystem::Initialize() {}
void ScrapDeleteSystem::Finalize() {}


void ScrapDeleteSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    ScrapStatus* scrapStatus = getComponent<ScrapStatus>(_entity);
    if (!scrapStatus) {
        return;
    }

    if (scrapStatus->GetIsDestroy()) {
        DestroyEntity(_entity);
    }


}

