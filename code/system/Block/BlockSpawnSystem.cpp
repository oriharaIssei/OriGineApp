#include "BlockSpawnSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
// lib
#include "input/Input.h"
// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/Block/BlockSpawner.h"
#include "component/Block/BlockStatus.h"
#include "component/Piller/FloatingFloorStatus.h"
// #include "component/Piller/PillerStates.h"
//  system
//  #include "system/FloorAndPillerColum/CreateFloorAndPillerSystem.h"
#include "system/FloatingFloor/CanageStateFallSystem.h"

BlockSpawnSystem::BlockSpawnSystem() : ISystem(SystemType::Initialize) {}
BlockSpawnSystem::~BlockSpawnSystem() {}

void BlockSpawnSystem::Initialize() {
    isInited_ = false;
}

void BlockSpawnSystem::Finalize() {
}

void BlockSpawnSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    blockSpawner_ = getComponent<BlockSpawner>(_entity);

    if (!blockSpawner_) {
        return;
    }

    float blockWidth_ = blockSpawner_->GetBlockSize()[X];
    float nextPosition = blockSpawner_->GetNextCreatePositionX();

    if (!isInited_) {// 初回の生成
       
        for (int32_t i = 0; i < blockSpawner_->GetColumNumMax(); ++i) {
            CreateBlocks(_entity, i, blockSpawner_->GetStartPositionX());
        }
        
    } else {

        // 2回目移行の生成
        if (lastTransform_ && lastTransform_->translate[X] < nextPosition) {
            for (int32_t i = 0; i < blockSpawner_->GetColumNumMax(); ++i) {
                float newX = lastTransform_->translate[X] + blockWidth_;
                CreateBlocks(_entity, i, newX);
            }
        }
    }

   isInited_ = true;
}

void BlockSpawnSystem::CreateBlocks(GameEntity* _entity, const int32_t& columIndex, const float& xPos) {
    // ================================= Bullet Entityを 生成 ================================= //
    GameEntity* block = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer, BlockStatus>(
        "Floor", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer(), BlockStatus());
    // ================================= Componentを初期化 ================================= //

    Transform* transform = getComponent<Transform>(block); // 柱

    // ランダムで

    //* Transform
    transform->translate = Vec3f{xPos, blockSpawner_->GetBlockSize()[Y] * columIndex, 0.0f};

    //* Collider
    SphereCollider* collider              = getComponent<SphereCollider>(block);
    collider->getLocalShapePtr()->radius_ = blockSpawner_->GetCollisionRadius();

    // MeshRenderer
    ModelMeshRenderer* blockRender = getComponent<ModelMeshRenderer>(block);

    //// Model から MeshRenderer を作成
    CreateModelMeshRenderer(blockRender, block, kApplicationResourceDirectory + "/Models/Block", "Block.obj");

    // /// States

    // row,columNum
    BlockStatus* blockStatus = getComponent<BlockStatus>(block);
    blockStatus->SetColum(columIndex);

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
    ecs->getSystem<CanageStateFallSystem>()->addEntity(block);

    //------------------ Movement
    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(block);

    //------------------ Collision
    /*  ecs->getSystem<CharacterOnCollision>()->addEntity(bom);*/
    ecs->getSystem<CollisionCheckSystem>()->addEntity(block);

    //------------------ Physics
    // None

    //------------------ Render
    ecs->getSystem<ColliderRenderingSystem>()->addEntity(block);
    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(block);
    /*ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(floor);*/
}

void BlockSpawnSystem::CostInit() {
}
