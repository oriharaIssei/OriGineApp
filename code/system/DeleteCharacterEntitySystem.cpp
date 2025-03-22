#include "DeleteCharacterEntitySystem.h"

/// ECS
#define ENGINE_ECS
// component
#include "component/CharacterStatus.h"

#include "engine/EngineInclude.h"

DeleteCharacterEntitySystem::DeleteCharacterEntitySystem() : ISystem(SystemType::StateTransition) {}

DeleteCharacterEntitySystem::~DeleteCharacterEntitySystem() {}

void DeleteCharacterEntitySystem::UpdateEntity(GameEntity* _entity) {
    auto status = getComponent<CharacterStatus>(_entity);
    if (status->getHP() <= 0) {
        DestroyEntity(_entity);
    }
}
