#include "LevelUIInitSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
// lib

// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/LevelUPUI/LevelUIParentStatus.h"
#include "component/LevelUPUI/LevelUIStatus.h"

LevelUIInitSystem::LevelUIInitSystem()
    : ISystem(SystemType::Initialize) {}

LevelUIInitSystem::~LevelUIInitSystem() {}

void LevelUIInitSystem::Initialize() {
}

void LevelUIInitSystem::Finalize() {}

void LevelUIInitSystem::UpdateEntity(GameEntity* _entity) {

    LevelUIStatus* levelUIStatus = getComponent<LevelUIStatus>(_entity);
    SpriteRenderer* sprite       = getComponent<SpriteRenderer>(_entity);

    if (!levelUIStatus) {
        return;
    }

    switch (levelUIStatus->GetLevelUIDigit()) {
    case LevelUIDigit::ICON:
        levelUIStatus->SetTextureSize(sprite->getTextureSize());
        break;   
    default:
        levelUIStatus->SetTextureSize(Vec2f(
           128.0f,
           128.0f));
       /* levelUIStatus->SetTextureSize(Vec2f(
            sprite->getTextureSize()[X] / 10.0f,
            sprite->getTextureSize()[Y])
        );*/
        break;
    }

  

    // savePos
    /* levelUIStatus->SetTextureSize(sprite->getTextureSize());*/
}
