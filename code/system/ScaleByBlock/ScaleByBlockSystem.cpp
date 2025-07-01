#include "ScaleByBlockSystem.h"

/// Engine
#define ENGINE_INCLUDE
#define ENGINE_INPUT
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include

// component
#include "component/Block/BlockManager.h"
#include "component/effect/TextureEffectParam.h"
#include "component/ScaleByBlockStatus/ScaleByBlockStatus.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>

ScaleByBlockSystem::ScaleByBlockSystem()
    : ISystem(SystemType::Movement) {}

ScaleByBlockSystem::~ScaleByBlockSystem() {}

void ScaleByBlockSystem::Initialize() {
    time_  = 0.0f;
    input_ = Input::getInstance();
}

void ScaleByBlockSystem::Finalize() {}

void ScaleByBlockSystem::UpdateEntity(GameEntity* entity) {

    // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* blockManagerEntity           = ecsManager->getUniqueEntity("BlockManager");

    if (!blockManagerEntity) { // Entityが存在しない場合の早期リターン
        return;
    }
    float deltaTime = Engine::getInstance()->getDeltaTime();

    ScaleByBlockStatus* scaleByBlockStatus = getComponent<ScaleByBlockStatus>(entity);
    Transform* transform                   = getComponent<Transform>(entity);
    ModelMeshRenderer* modelMesh           = getComponent<ModelMeshRenderer>(entity);
    BlockManager* blockManagerStatus       = getComponent<BlockManager>(blockManagerEntity);

    if (!blockManagerStatus || !scaleByBlockStatus || !transform || !modelMesh) {
        return;
    }

    scaleByBlockStatus->UVScrollUPdate(deltaTime);

    transform->scale = blockManagerStatus->GetResultScale() - scaleByBlockStatus->GetScaleByBlockStatus();

    if (scaleByBlockStatus->GetIsUVScroll()) {
        modelMesh->getMaterialBuff(0).openData_.uvTransform_.translate_ = scaleByBlockStatus->GetUVPos();
    }
}

void ScaleByBlockSystem::ComboReset() {
}
