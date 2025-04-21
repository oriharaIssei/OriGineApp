#include "BlockSpawnSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
#include "myRandom/MyRandom.h"
// lib
#include "input/Input.h"
// include
#include <Quaternion.h>
#include <Vector3.h>
// component

#include "component/Block/BlockStatus.h"
#include "component/Piller/FloatingFloorStatus.h"
// #include "component/Piller/PillerStates.h"
//  system
#include "system/Block/BlockExBomCollision.h"
#include "system/Block/BlockMoveSystem.h"
#include "system/Block/BreakBlockSystem.h"
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

    blockSpawner_ = getComponent<BlockManager>(_entity);

    if (!blockSpawner_) {
        return;
    }

    blockSpawner_->CostReset(); // コストリセット

    float blockWidth_  = blockSpawner_->GetBlockSize()[X] * 2.0f;
    float nextPosition = blockSpawner_->GetNextCreatePositionX();

    if (!isInited_) { // 初回の生成

        for (int32_t i = 0; i < blockSpawner_->GetColumNumMax(); ++i) {
            CreateBlocks(i, blockSpawner_->GetStartPositionX());
        }
        // 　行数カウンターを一応初期化
        for (int i = 0; i < static_cast<int32_t>(BlockType::COUNT); ++i) {
            blockSpawner_->ResetLineCounter(static_cast<BlockType>(i));
        }

    } else {

        // 2回目移行の生成
        if (lastTransform_ && lastTransform_->translate[X] < nextPosition) {
            for (int32_t i = 0; i < blockSpawner_->GetColumNumMax(); ++i) {
                float newX = lastTransform_->translate[X] + blockWidth_;
                CreateBlocks(i, newX);
            }
        }
    }

    isInited_ = true;
}

void BlockSpawnSystem::CreateBlocks(const int32_t& columIndex, const float& xPos) {

    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* block = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer, BlockStatus>(
        "Block", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer(), BlockStatus());

    // ================================= Componentを初期化 ================================= //

    //* Transform
    Transform* transform = getComponent<Transform>(block); // 柱
    float sizeY          = blockSpawner_->GetBlockSize()[Y] * 2.0f;

    transform->translate = Vec3f{xPos, blockSpawner_->GetBasePosY() + (sizeY * columIndex), blockSpawner_->GetStartPositionZ()};
    transform->scale     = Vec3f(blockSpawner_->GetBlockSize()[X], blockSpawner_->GetBlockSize()[Y], 1.0f);

    //* Collider
    SphereCollider* collider              = getComponent<SphereCollider>(block);
    collider->getLocalShapePtr()->radius_ = blockSpawner_->GetCollisionRadius();

    // /// States

    // row,columNum
    BlockStatus* blockStatus = getComponent<BlockStatus>(block);
    blockStatus->SetColum(columIndex);
    blockStatus->SetBlockType(BlockType::NORMAL); // まずはノーマルにセット

    /// blockTypeCreater
    BlockTypeSetting(blockStatus, BlockType::SKULL); // どくろの生成
    BlockTypeSetting(blockStatus, BlockType::ADVANTAGE); // アドバンテージブロックの生成

    //* MeshRenderer
    ModelMeshRenderer* blockRender = getComponent<ModelMeshRenderer>(block);

    //// Model から MeshRenderer を作成
    ModelSetForBlockType(blockRender, block, blockStatus->GetBlockType());

    // hp
    blockStatus->SetcurrentHP(blockSpawner_->GetHpMax());

    // 一番上のブロックを監視対象に設定
    if (columIndex == blockSpawner_->GetColumNumMax() - 1) {
        lastTransform_ = transform;
    }

    // ================================= System ================================= //
    ECSManager* ecs = ECSManager::getInstance();

    //------------------ Input
    // None

    //------------------ StateTransition
    ecs->getSystem<DeleteBlockSystem>()->addEntity(block);
    ecs->getSystem<BreakBlockSystem>()->addEntity(block);
    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(block);
    ecs->getSystem<BlockMoveSystem>()->addEntity(block);

    //------------------ Collision
    ecs->getSystem<CollisionCheckSystem>()->addEntity(block);
    ecs->getSystem<BlockExBomCollision>()->addEntity(block);
    //------------------ Physics
    // None

    //------------------ Render
    /* ecs->getSystem<ColliderRenderingSystem>()->addEntity(block);*/
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(block);
}

void BlockSpawnSystem::CostInit() {
}

void BlockSpawnSystem::BlockTypeSetting(BlockStatus* status, BlockType blocktype) {

    blockSpawner_->SetIncrementLineCounter(blocktype);

    // 指定された行間隔に達していなければスキップ
    if (blockSpawner_->GetLineCounter(blocktype) % blockSpawner_->GetGenerateInterval(blocktype) != 0) {
        return;
    }
    // コストチェック
    if (blockSpawner_->GetCurrentCost(blocktype) >= blockSpawner_->GetRandomPar(blocktype)) {
        return;
    }

    // 指定の割合で別ブロックに変身
    MyRandom::Int Random(0, 100);
    if (Random.get() <= blockSpawner_->GetRandomPar(blocktype)) {
        status->SetBlockType(blocktype);
        blockSpawner_->SetCurrentCostIncrement(blocktype);
    }
}

void BlockSpawnSystem::ModelSetForBlockType(ModelMeshRenderer* render, GameEntity* entity, BlockType type) {
    switch (type) {
    case BlockType::NORMAL:
        CreateModelMeshRenderer(render, entity, kApplicationResourceDirectory + "/Models/Block", "Block.obj");
        break;
    case BlockType::SKULL:
        CreateModelMeshRenderer(render, entity, kApplicationResourceDirectory + "/Models/SkullBlock", "SkullBlock.obj");
        break;
    case BlockType::ADVANTAGE:
        CreateModelMeshRenderer(render, entity, kApplicationResourceDirectory + "/Models/AdvantageBlock", "AdvantageBlock.obj");
        break;
    case BlockType::COUNT:
        break;
    default:
        break;
    }
}
