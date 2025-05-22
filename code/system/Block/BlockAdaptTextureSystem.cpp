#include "BlockAdaptTextureSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib
// ECS
#define ENGINE_ECS
// include

// component
#include "component/Block/BlockCombinationStatus.h"
#include "component/Block/BlockStatus.h"

#include "engine/EngineInclude.h"
#include <Vector4.h>

BlockAdaptTextureSystem::BlockAdaptTextureSystem()
    : ISystem(SystemType::Movement) {}

BlockAdaptTextureSystem::~BlockAdaptTextureSystem() {}

void BlockAdaptTextureSystem::Initialize() {
    time_ = 0.0f;
}

void BlockAdaptTextureSystem::Finalize() {}

void BlockAdaptTextureSystem::UpdateEntity(GameEntity* _entity) {

    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* blockCombiEntity             = ecsManager->getUniqueEntity("BlockCombination");

    if (!blockCombiEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    BlockStatus* blockStatus                 = getComponent<BlockStatus>(_entity);
    ModelMeshRenderer* modelRenderer         = getComponent<ModelMeshRenderer>(_entity);
    BlockCombinationStatus* blockCombiStatus = getComponent<BlockCombinationStatus>(blockCombiEntity);
    Vec4f color_;

    if (!blockStatus || !modelRenderer || !blockCombiStatus) {
        return;
    }

    switch (blockStatus->GetAdaptTextureStep()) {
        ///---------------------------------------------------
        /// NONE
        ///---------------------------------------------------
    case BlockStatus::AdaptTextureStep::NONE:
        if (blockStatus->GetRowNum() < blockCombiStatus->GetConbinationMax()) {
            /*  color_ = Vec4f(1.0f, 0.0f, 0.4f, 1.0f);*/
            break;
        }

        blockStatus->SetAdaptTextureStep(BlockStatus::AdaptTextureStep::ADAPT);
        break;
        ///---------------------------------------------------
        /// 適応
        ///---------------------------------------------------

    case BlockStatus::AdaptTextureStep::ADAPT:
        /*  color_ = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);*/
        // ドロンのようなパーティクル入れたい
        modelRenderer->setTexture(0, blockStatus->GetAdaptTexture());
        blockStatus->SetAdaptTextureStep(BlockStatus::AdaptTextureStep::END);
        break;
        ///---------------------------------------------------
        /// 終わり
        ///---------------------------------------------------
    case BlockStatus::AdaptTextureStep::END:
        /*    color_ = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);*/
        break;
    default:
        break;
    }
    /* modelRenderer->getMaterialBuff(0)->color_ = color_;*/
}

void BlockAdaptTextureSystem::ComboReset() {
}
