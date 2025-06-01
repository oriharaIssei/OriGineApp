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
    BlockCombinationStatus* combinationStatus = getComponent<BlockCombinationStatus>(combiEntity);
  /*  Transform* transform                      = getComponent<Transform>(_entity);*/
    Emitter* emitterAura                      = getComponent<Emitter>(_entity);

    if (!blockStatus || !combinationStatus) {
        return;
    }

    // Skull限定
    if (blockStatus->GetBlockType() != BlockType::SKULL) {
        return;
    }

    // 左にAdvantageがあるか判定
    bool leftBlocks = combinationStatus->JudgeIsAdvantageToLeft(blockStatus->GetRowNum(), blockStatus->GetColumnNum());
    blockStatus->SetIsSinister(leftBlocks);
    blockStatus->SinisterShake(); // シェイク

    if (!leftBlocks) {
        return;
    }
    //particle
    if (emitterAura) {
        // particleダウ
      /*  emitterAura->setOriginePos(transform->worldMat[3]);*/
        emitterAura->PlayStart();
    }
}
