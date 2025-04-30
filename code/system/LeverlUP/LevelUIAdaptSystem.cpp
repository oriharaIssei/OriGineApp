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

    Vec3f basePos   = levelUIParentStatus->GetBasePos() + levelUIStatus->GetOffsetPos();
    Vec2f baseSize = sprite->getTextureSize() *levelUIParentStatus->GetBaseScale();
    float uvPos     = levelUIParentStatus->GetCurrentLevelUV() * 0.1f;

    // pos
    sprite->setTranslate(Vec2f(basePos[X] + levelUIStatus->GetOffsetPos()[X], basePos[Y] + levelUIStatus->GetOffsetPos()[Y]));
    // scale
    sprite->setSize(baseSize);
    // uv
    if (levelUIStatus->GetLevelUIDigit() != LevelUIDigit::ICON) {
        sprite->setUVTranslate(Vec2f(uvPos, 0.0f));
    }
}

void LevelUIAdaptSystem::ComboReset() {
}
