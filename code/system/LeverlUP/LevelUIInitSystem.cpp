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

   
    LevelUIParentStatus* levelUIStatus = getComponent<LevelUIParentStatus>(_entity);

    if ( !levelUIStatus) {
        return;
    }

    levelUIStatus->Init();

    // savePos
   /* levelUIStatus->SetTextureSize(sprite->getTextureSize());*/


   
}

