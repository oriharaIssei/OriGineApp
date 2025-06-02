#include "ResourceAllLoadSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
#include "engine/EngineInclude.h"

bool ResourceAllLoadSystem::isLoaded_ = false;

ResourceAllLoadSystem::ResourceAllLoadSystem()
    : ISystem(SystemType::Initialize) {}

ResourceAllLoadSystem::~ResourceAllLoadSystem() {}

void ResourceAllLoadSystem::Initialize() {
}

void ResourceAllLoadSystem::Finalize() {}

void ResourceAllLoadSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    if (isLoaded_) {
        return;
    }

    TextureManager::LoadTexture();

    isLoaded_ = true;
}
