#include "LevelUIAdaptSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/LevelUPUI/LevelUIParentStatus.h"
#include "component/LevelUPUI/LevelUIStatus.h"
#include "component/Timer/TimerStatus.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>
#include <Vector.h>

LevelUIAdaptSystem::LevelUIAdaptSystem()
    : ISystem(SystemType::Movement) {}

LevelUIAdaptSystem::~LevelUIAdaptSystem() {}

void LevelUIAdaptSystem::Initialize() {
}

void LevelUIAdaptSystem::Finalize() {}

void LevelUIAdaptSystem::UpdateEntity(GameEntity* _entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* levelUIParent                = ecsManager->getUniqueEntity("LevelUIParent");

    if (!levelUIParent) { // Entityが存在しない場合の早期リターン
        return;
    }

    // get timer component
    LevelUIStatus* levelUIStatus             = getComponent<LevelUIStatus>(_entity);
    SpriteRenderer* sprite                   = getComponent<SpriteRenderer>(_entity);
    LevelUIParentStatus* levelUIParentStatus = getComponent<LevelUIParentStatus>(levelUIParent);

    if (!levelUIStatus || !levelUIParentStatus || !sprite) {
        return;
    }

     ///* ------------------------------calucration------------------------------

    Vec3f basePos   =  Vec3f(0.0f, 0.0f, 0.0f);
    Vec2f baseSize = levelUIStatus->GetTextureSize() * levelUIParentStatus->GetBaseScale();
    float uvPos     = levelUIParentStatus->GetCurrentLevelUV() * 0.1f;

    switch (levelUIStatus->GetLevelUIDigit()) {
    case LevelUIDigit::ICON:
         basePos = levelUIParentStatus->GetBasePos() + levelUIStatus->GetOffsetPos();
        break;
    default:
         basePos = levelUIParentStatus->GetBasePos() + levelUIStatus->GetOffsetPos()+levelUIParentStatus->GetCurrentMoveOffset();
        break;
    }

    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setTranslate(Vec2f(basePos[X], basePos[Y]));
    // scale
    sprite->setSize(baseSize);
    // uv
    if (levelUIStatus->GetLevelUIDigit() != LevelUIDigit::ICON) {
        sprite->setUVTranslate(Vec2f(uvPos, 0.0f));
    }
}

void LevelUIAdaptSystem::ComboReset() {
}
