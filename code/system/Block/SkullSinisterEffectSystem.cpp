#include "SkullSinisterEffectSystem.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

#include "component/BigBom/BigBomStatus.h"
#include "component/Block/BlockCombinationStatus.h"
#include "component/Block/BlockManager.h"
#include "component/Block/BlockStatus.h"
#include "component/Bom/BomStatus.h"

SkullSinisterEffectSystem::SkullSinisterEffectSystem() : ISystem(SystemType::Movement) {}
SkullSinisterEffectSystem::~SkullSinisterEffectSystem() {}

void SkullSinisterEffectSystem::Initialize() {
}

void SkullSinisterEffectSystem::Finalize() {
}

void SkullSinisterEffectSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

    // CharacterStatusを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* combiEntity                  = ecsManager->getUniqueEntity("BlockCombination");

    BlockStatus* blockStatus                  = getComponent<BlockStatus>(_entity);
  /*  ModelMeshRenderer* modelMesh              = getComponent<ModelMeshRenderer>(_entity);*/
    BlockCombinationStatus* combinationStatus = getComponent<BlockCombinationStatus>(combiEntity);

    if (!blockStatus || !combinationStatus) {
        return;
    }

    // Skull限定
    if (blockStatus->GetBlockType() != BlockType::SKULL) {
        return;
    }

    // 左にAdvantageがあるか判定
    bool leftBlocks = combinationStatus->JudgeIsAdvantageToLeft(blockStatus->GetRowNum(), blockStatus->GetColumnNum());
    /* blockStatus->SetIsCollisionReaction(leftBlocks);*/ leftBlocks;
}
