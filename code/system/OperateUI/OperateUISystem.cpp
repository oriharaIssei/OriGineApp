#include "OperateUISystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Vector3.h>
// component
#include "component/OperateUI/OperateUIStatus.h"
#include "component/Timer/TimerStatus.h"

#define RESOURCE_DIRECTORY
#include "engine/EngineInclude.h"
#include <cstdint>

OperateUISystem::OperateUISystem()
    : ISystem(SystemType::Movement) {}

OperateUISystem::~OperateUISystem() {}

void OperateUISystem::Initialize() {
    //// TextureNameの初期化
    // for (int32_t i = 0; i < 10; ++i) {
    //     textureName_[i] = "Texture/Combo/ComboNumber" + std::to_string(i) + ".png";
    // };
}

void OperateUISystem::Finalize() {}

void OperateUISystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    //   EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    //   GameEntity* blockManagerEntity           = ecsManager->getUniqueEntity("BlockManager");

    //   if (!blockManagerEntity) {
    //       return;
    //   }
    ///*   BlockManager* blockManager = getComponent<BlockManager>(blockManagerEntity);*/

    SpriteRenderer* sprite   = getComponent<SpriteRenderer>(_entity);
    OperateUIStatus* operate = getComponent<OperateUIStatus>(_entity);

    if (!sprite || !operate) {
        return;
    }

    switch (operate->GetOperateMode()) {
    case OperateMode::LAUNCH:
        sprite->setUVTranslate(Vec2f(0.0f, 0.0f));
        break;
    case OperateMode::EXPLOTION:
        sprite->setUVTranslate(Vec2f(0.5f, 0.0f));
        break;
    default:
        break;
    }

  

    // 移動
    operate->ScalingEaseUpdate(Engine::getInstance()->getDeltaTime());

      // スケール代入
    sprite->setSize(operate->GetREsultSize());
}
