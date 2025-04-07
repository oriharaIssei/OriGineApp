#include "PlayerFollowCameraSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
#include "input/Input.h"
// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/Bom/BomSpawner.h"

#include "engine/EngineInclude.h"
#include <cmath>
#include <numbers>

PlayerFollowCameraSystem::PlayerFollowCameraSystem()
    : ISystem(SystemType::Movement) {}

PlayerFollowCameraSystem::~PlayerFollowCameraSystem() {}

void PlayerFollowCameraSystem::Initialize() {
   
}

void PlayerFollowCameraSystem::Finalize() {}

void PlayerFollowCameraSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    
}
