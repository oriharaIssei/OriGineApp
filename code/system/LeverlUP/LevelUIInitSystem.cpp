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
#include"component/LevelUPUI/LevelUIStatus.h"



LevelUIInitSystem::LevelUIInitSystem()
    : ISystem(SystemType::Initialize) {}

LevelUIInitSystem::~LevelUIInitSystem() {}

void LevelUIInitSystem::Initialize() {
}

void LevelUIInitSystem::Finalize() {}

void LevelUIInitSystem::UpdateEntity(GameEntity* _entity) {

    _entity;
  /*  SpriteRenderer* sprite = getComponent<SpriteRenderer>(_entity);
    LevelUIStatus* levelUIStatus = getComponent<LevelUIStatus>(_entity);

    if (!sprite || !levelUIStatus) {
        return;
    }*/

    // savePos
   /* levelUIStatus->SetTextureSize(sprite->getTextureSize());*/


   
}

