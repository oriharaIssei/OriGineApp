#include "BlockSpawnSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
#include "myRandom/MyRandom.h"

#include <Vector3.h>
// component

#include "component/Block/BlockCombinationStatus.h"
#include "component/Block/BlockStatus.h"
#include "component/Menu/MenuStatus.h"
#include "component/Piller/FloatingFloorStatus.h"
// #include "component/Piller/PillerStates.h"
//  system
#include "system/Block/BlockColorChangeSystem.h"
#include "system/Block/BlockExBomCollision.h"
#include "system/Block/BlockFloorCollision.h"
#include "system/Block/BlockMoveSystem.h"
#include "system/Block/BreakBlockSystem.h"
#include "system/Block/DeleteBlockForAdvantageSystem.h"
#include "system/Block/DeleteBlockSystem.h"

BlockSpawnSystem::BlockSpawnSystem() : ISystem(SystemType::Movement) {}
BlockSpawnSystem::~BlockSpawnSystem() {}

void BlockSpawnSystem::Initialize() {
    isInited_        = false;
    nextSpecialLine_ = 0;
}

void BlockSpawnSystem::Finalize() {
}

void BlockSpawnSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    // ポーズ中は通さない
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* menuEntity                   = ecsManager->getUniqueEntity("Menu");
    GameEntity* bockCombiEntity              = ecsManager->getUniqueEntity("BlockCombination");

    if (!menuEntity || !bockCombiEntity) {
        return;
    }

    MenuStatus* menu        = getComponent<MenuStatus>(menuEntity);
    blockCombinationStatus_ = getComponent<BlockCombinationStatus>(bockCombiEntity);
    if (!menu || !blockCombinationStatus_) {
        return;
    }

    if (menu->GetIsPose()) {
        return;
    }

    blockSpawner_ = getComponent<BlockManager>(_entity);

    if (!blockSpawner_) {
        return;
    }

    float blockWidth   = blockSpawner_->GetBlockSize()[X] * 2.0f;
    float nextPosition = blockSpawner_->GetNextCreatePositionX();

    if (blockSpawner_->GetIsMove()) {
        return;
    }

    if (!isInited_) { // 初回の生成

        for (int32_t i = blockSpawner_->GetRowMaxNum(); i > 0; --i) {//列を
            for (int32_t j = 0; j < blockSpawner_->GetColumnMaxNum(); ++j) {//上に立ててく
                CreateBlocks(i, j, blockSpawner_->GetStartPositionX() + (blockWidth * (blockSpawner_->GetRowMaxNum() - i)));
            }
            blockSpawner_->CostReset(); // コストリセット
            blockSpawner_->LineIncrement();
        }

    } else {

        // 2回目移行の生成
        if (lastTransform_ && lastTransform_->translate[X] < nextPosition) {
            for (int32_t i = 0; i < blockSpawner_->GetColumnMaxNum(); ++i) {
                float newX = lastTransform_->translate[X] + blockWidth;
                CreateBlocks(0, i, newX);
            }
            blockSpawner_->CostReset(); // コストリセット
            blockSpawner_->LineIncrement();
        }
    }

    isInited_ = true;
}

void BlockSpawnSystem::CreateBlocks(const int32_t& rowIndex, const int32_t& columIndex, const float& xPos) {

    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* block = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer, BlockStatus>(
        "Block", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer(), BlockStatus());

    // ================================= Componentを初期化 ================================= //

    //* Transform
    Transform* transform = getComponent<Transform>(block); // 柱
    float sizeY          = blockSpawner_->GetBlockSize()[Y] * 2.0f;

    transform->translate = Vec3f{xPos, blockSpawner_->GetBasePosY() + (sizeY * columIndex), blockSpawner_->GetStartPositionZ()};
    transform->scale     = blockSpawner_->GetBlockSize();

    //* Collider
    SphereCollider* collider              = getComponent<SphereCollider>(block);
    collider->getLocalShapePtr()->radius_ = blockSpawner_->GetCollisionRadius();

    // /// States

    // row,columNum
    BlockStatus* blockStatus = getComponent<BlockStatus>(block);
    blockStatus->SetColumnNum(columIndex);
    blockStatus->SetRowNum(rowIndex);
    blockStatus->SetBlockType(BlockType::NORMAL); // まずはノーマルにセット
    blockStatus->SetEaseTimeMax(blockSpawner_->GetMoveTime());

    /// blockTypeCreater

    BlockTypeSetting(blockStatus, BlockType::ADVANTAGE); // アドバンテージブロックの生成

    //アドバンテージのcolum位置を保存
    if (blockStatus->GetBlockType() == BlockType::ADVANTAGE) {
        // Advance生成時にcolumを予約
        if (std::find(reservedSkullColumns_.begin(), reservedSkullColumns_.end(), columIndex) == reservedSkullColumns_.end()) {
            reservedSkullColumns_.push_back(columIndex);
        }
    }

    // アドバンテージの隣にskullを生成する
    BlockTypeSettingBySameColum(blockStatus, columIndex);

    // ブロックタイプにより得られるスコアを設定
    blockStatus->SetBaseScoreValue(blockSpawner_->GetScoreValue(blockStatus->GetBlockType()));

    //* MeshRenderer
    ModelMeshRenderer* blockRender = getComponent<ModelMeshRenderer>(block);

    //// Model から MeshRenderer を作成
    ModelSetForBlockType(blockRender, block, blockStatus->GetBlockType());

    // hp
    blockStatus->SetcurrentHP(blockSpawner_->GetHpMax());

    // 一番上のブロックを監視対象に設定
    if (columIndex == blockSpawner_->GetColumnMaxNum() - 1) {
        lastTransform_ = transform;
    }

    // ブロックステータスを更新
    blockCombinationStatus_->AddBlockStatus(blockStatus);

    // 列カウンターインクリメント

    /*blockSpawner_->SetIncrementLineCounter(BlockType::NORMAL);
    blockSpawner_->SetIncrementLineCounter(BlockType::ADVANTAGE);
    blockSpawner_->SetIncrementLineCounter(BlockType::SKULL);*/
    // ================================= System ================================= //
    ECSManager* ecs = ECSManager::getInstance();

    //------------------ Input
    // None

    //------------------ StateTransition
    ecs->getSystem<DeleteBlockSystem>()->addEntity(block);
    ecs->getSystem<BreakBlockSystem>()->addEntity(block);
    ecs->getSystem<DeleteBlockForAdvantageSystem>()->addEntity(block);
    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(block);
    ecs->getSystem<BlockMoveSystem>()->addEntity(block);

    //------------------ Collision
    ecs->getSystem<CollisionCheckSystem>()->addEntity(block);
    ecs->getSystem<BlockExBomCollision>()->addEntity(block);
    ecs->getSystem<BlockFloorCollision>()->addEntity(block);
    ecs->getSystem<BlockColorChangeSystem>()->addEntity(block);
    //------------------ Physics
    // None

    //------------------ Render
    /* ecs->getSystem<ColliderRenderingSystem>()->addEntity(block);*/
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(block);
}

void BlockSpawnSystem::CostInit() {
    // 　行数カウンターを一応初期化
}

void BlockSpawnSystem::BlockTypeSetting(BlockStatus* status, BlockType blocktype) {

    if (status->GetBlockType() != BlockType::NORMAL) {
        return;
    }

    MyRandom::Int Random(0, 100);

    // 指定された行間隔に達していなければスキップ
    if ((blockSpawner_->GetLineCounter(blocktype) % blockSpawner_->GetGenerateInterval(blocktype) != 0) || blockSpawner_->GetCurrentCost(blocktype) >= blockSpawner_->GetCost(blocktype)) {

        // 指定の割合で別ブロックに変身
        if (Random.get() <= blockSpawner_->GetRandomParConstant(blocktype)) {
            status->SetBlockType(blocktype);
        }

        return;
    }

    // 指定の割合で別ブロックに変身
    if (Random.get() <= blockSpawner_->GetRandomPar(blocktype)) {
        status->SetBlockType(blocktype);
        blockSpawner_->SetCurrentCostIncrement(blocktype);
    }
}

void BlockSpawnSystem::BlockTypeSettingBySameColum(BlockStatus* status, const int32_t& columnNum) {
    // --- reservedSkullColumns_に含まれるcolumならSkull生成判定 ---
    auto it = std::find(reservedSkullColumns_.begin(), reservedSkullColumns_.end(), columnNum);
    if (it != reservedSkullColumns_.end()) {
        // 確率でSkull生成
        MyRandom::Int rand(0, 100);
        int32_t skullProb = blockSpawner_->GetRandomPar(BlockType::SKULL); // 例: 30なら30%
        if (rand.get() < skullProb) {
            status->SetBlockType(BlockType::SKULL);
            // 生成できたので予約解除
            reservedSkullColumns_.erase(it);
        }
        // 外れた場合は予約を残す（次回また判定）
    }
}

void BlockSpawnSystem::ModelSetForBlockType(ModelMeshRenderer* render, GameEntity* entity, BlockType type) {
    switch (type) {
    case BlockType::NORMAL:
        CreateModelMeshRenderer(render, entity, kApplicationResourceDirectory + "/Models/Block", "Block.gltf");
        break;
    case BlockType::SKULL:
        CreateModelMeshRenderer(render, entity, kApplicationResourceDirectory + "/Models/SkullBlock", "SkullBlock.gltf");
        break;
    case BlockType::ADVANTAGE:
        CreateModelMeshRenderer(render, entity, kApplicationResourceDirectory + "/Models/AdvantageBlock", "AdvantageBlock.gltf");
        break;
    case BlockType::COUNT:
        break;
    default:
        break;
    }
}
