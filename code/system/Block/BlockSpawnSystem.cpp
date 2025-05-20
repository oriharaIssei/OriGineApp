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

    //// Particle達
    //for (int32_t i = 0; i < emitters_.size(); ++i) {
    //    emitters_[i] = getComponent<Emitter>(_entity, i);
    //}

    float blockWidth   = blockSpawner_->GetBlockSize()[X] * 2.0f;
    float nextPosition = blockSpawner_->GetNextCreatePositionX();

    if (blockSpawner_->GetIsMove()) {
        return;
    }
    if (!isInited_) { // 初回の生成

        blockSpawner_->ApplyLevelParams(blockSpawner_->GetCurrentLevel());

        for (int32_t i = blockSpawner_->GetRowMaxNum(); i > 0; --i) {
            // 列インデックスを作成しシャッフル
            std::vector<int32_t> columns(blockSpawner_->GetColumnMaxNum());
            std::iota(columns.begin(), columns.end(), 0);
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(columns.begin(), columns.end(), g);

            for (int32_t j : columns) {
                CreateBlocks(i, j, blockSpawner_->GetStartPositionX() + (blockWidth * (blockSpawner_->GetRowMaxNum() - i)));
            }
            blockSpawner_->CostReset(); // コストリセット
            blockSpawner_->LineIncrement();
        }

    } else {

        // 2回目移行の生成
        if (lastTransform_ && lastTransform_->translate[X] < nextPosition) {
            // 列インデックスを作成しシャッフル
            std::vector<int32_t> columns(blockSpawner_->GetColumnMaxNum());
            std::iota(columns.begin(), columns.end(), 0);
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(columns.begin(), columns.end(), g);

            float newX = lastTransform_->translate[X] + blockWidth;
            for (int32_t i : columns) {
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
   /* ComponentArray<Emitter>* emitterArray = ECSManager::getInstance()->getComponentArray<Emitter>();*/
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

    // アドバンテージのcolum位置を保存
    if (blockStatus->GetBlockType() == BlockType::ADVANTAGE) {
        // Advance生成時にcolumを予約
        if (std::find(reservedSkullColumns_.begin(), reservedSkullColumns_.end(), columIndex) == reservedSkullColumns_.end()) {
            reservedSkullColumns_.push_back(columIndex);
        }
    }

    // アドバンテージの隣にskullを生成する
    BlockTypeSettingBySameColum(blockStatus, BlockType::SKULL, columIndex);
    BlockTypeSettingBySameColum(blockStatus, BlockType::ADVANTAGE, columIndex);

    // ブロックタイプにより得られるスコアを設定
    blockStatus->SetBaseScoreValue(blockSpawner_->GetScoreValue(blockStatus->GetBlockType()));

    //* MeshRenderer
    ModelMeshRenderer* blockRender = getComponent<ModelMeshRenderer>(block);

    //// Model から MeshRenderer を作成
    ModelSetForBlockType(blockRender, block, blockStatus->GetBlockType());

    ////* Emitter
    //Emitter* blockEmitterLayerOne = getComponent<Emitter>(block);
    //Emitter* blockEmitterLayerTwo = getComponent<Emitter>(block,1);

    //// blockTypeによってEmitterを変更する
    //EmitterSetForBlockType(blockEmitterLayerOne, blockEmitterLayerTwo, blockStatus->GetBlockType());

    // hp
    blockStatus->SetcurrentHP(blockSpawner_->GetHpMax());

    // 一番上のブロックを監視対象に設定
    if (columIndex == blockSpawner_->GetColumnMaxNum() - 1) {
        lastTransform_ = transform;
    }

    // ブロックステータスを更新
    blockCombinationStatus_->AddBlockStatus(blockStatus);

    // 列カウンターインクリメント

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
   /* ecs->getSystem<EmitterWorkSystem>()->addEntity(block);*/

    //------------------ Collision
    ecs->getSystem<CollisionCheckSystem>()->addEntity(block);
    ecs->getSystem<BlockExBomCollision>()->addEntity(block);
    ecs->getSystem<BlockFloorCollision>()->addEntity(block);
    ecs->getSystem<BlockColorChangeSystem>()->addEntity(block);
    //------------------ Physics
    // None

    //------------------ Render
   /* ecs->getSystem<ParticleRenderSystem>()->addEntity(block);*/
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
    if ((blockSpawner_->GetLineCounter(blocktype) % blockSpawner_->GetGenerateInterval(blocktype) != 0)) {

        //// 指定の割合で別ブロックに変身
        // if (Random.get() <= blockSpawner_->GetRandomParConstant(blocktype)) {
        //     status->SetBlockType(blocktype);
        // }

        return;
    }
    if (blockSpawner_->GetCurrentCost(blocktype) >= blockSpawner_->GetCost(blocktype)) {
        return;
    }

    // 指定の割合で別ブロックに変身
    if (Random.get() <= blockSpawner_->GetRandomPar(blocktype)) {
        status->SetBlockType(blocktype);
        blockSpawner_->SetCurrentCostIncrement(blocktype);
    }
}

void BlockSpawnSystem::BlockTypeSettingBySameColum(BlockStatus* status, BlockType type, const int32_t& columnNum) {

    if (status->GetBlockType() != BlockType::NORMAL) {
        return;
    }

    // アドバンテージブロックと同じ列に生成する
    auto it = std::find(reservedSkullColumns_.begin(), reservedSkullColumns_.end(), columnNum);
    if (it != reservedSkullColumns_.end()) {

        int32_t baseProb = blockSpawner_->GetRandomParRightOfAdvance(type);
        int32_t upValue  = blockSpawner_->GetRandomParUPValue(type);

        int32_t& currentProb = skullProbabilities_[columnNum];
        if (currentProb == 0) {
            currentProb = baseProb;
        }

        MyRandom::Int rand(0, 100);

        int32_t judgeProb = (type == BlockType::SKULL) ? currentProb : baseProb; // skullブロックのみ確率上昇をする

        if (rand.get() < judgeProb) {
            status->SetBlockType(type);

            // 生成できたので予約解除＆確率リセット
            reservedSkullColumns_.erase(it);
            skullProbabilities_.erase(columnNum);
        } else if (type == BlockType::SKULL) {
            // 失敗したら確率をUP（Skullのみ）
            currentProb += upValue;
            if (currentProb > 100)
                currentProb = 100;
        }
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


//void BlockSpawnSystem::EmitterSetForBlockType(Emitter* emitter1, Emitter* emitter2, BlockType type) {
//
//   /* switch (type) {
//    case BlockType::NORMAL:
//        *emitter1 = *emitters_[0];
//        *emitter2 = *emitters_[1];
//            break;
//    
//    case BlockType::ADVANTAGE:
//        emitter1 = emitters_[2];
//        emitter2 = emitters_[3];
//        break;
//    case BlockType::SKULL:
//        emitter1 = emitters_[4];
//        emitter2 = emitters_[5];
//        break;
//    case BlockType::COUNT:
//        break;
//    default:
//        break;
//    }*/
//}
