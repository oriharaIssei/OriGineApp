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
#include"component/LevelUPUI/LevelUIParentStatus.h"



LevelUIInitSystem::LevelUIInitSystem()
    : ISystem(SystemType::Initialize) {}

LevelUIInitSystem::~LevelUIInitSystem() {}

void LevelUIInitSystem::Initialize() {
}

void LevelUIInitSystem::Finalize() {}

void LevelUIInitSystem::UpdateEntity(GameEntity* _entity) {


    SpriteRenderer* sprite = getComponent<SpriteRenderer>(_entity);
    LevelUIParentStatus* parentUIStatus = getComponent<LevelUIParentStatus>(_entity);

    if (!sprite || !parentUIStatus) {
        return;
    }

    // savePos
    parentUIStatus->SetbasePos(sprite->getTranslate());
    parentUIStatus->SetInitPos(sprite->getTranslate());


   
}

