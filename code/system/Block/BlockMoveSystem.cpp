#include "BlockMoveSystem.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// component
#include "component/Block/BlockManager.h"
#include "component/Block/BlockStatus.h"

BlockMoveSystem::BlockMoveSystem()
    : ISystem(SystemType::Movement) {}

BlockMoveSystem::~BlockMoveSystem() {}

void BlockMoveSystem::Initialize() {}
void BlockMoveSystem::Finalize() {}

void BlockMoveSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    BlockStatus* blockStatus = getComponent<BlockStatus>(_entity);
    if (!blockStatus) {
        return;
    }
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* blockManagerEntity           = ecsManager->getUniqueEntity("BlockManager");

    if (!blockManagerEntity) {
        return;
    }

    Transform* transform       = getComponent<Transform>(_entity);
    BlockManager* blockManager = getComponent<BlockManager>(blockManagerEntity);

    if (!transform || !blockManager) {
        return;
    }

    // スケール代入
    transform->scale = blockManager->GetResultScalle();

    if (blockManager->GetIsMove()) {
        blockStatus->SetIsMove(true);
        blockStatus->SetPreMovePos(transform->translate);
        blockStatus->TimeInit();
    }
        // 移動
        blockStatus->MoveUpdate(Engine::getInstance()->getDeltaTime(), transform, blockManager->GetBlockSize()[X]);
    
}
