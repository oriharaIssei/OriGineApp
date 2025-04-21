#include "BreakBlockSystem.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
/// ECS
#define ENGINE_ECS
// component
#include "component/Block/BlockStatus.h"
#include "component/Scrap/ScrapSpawner.h"
#include "component/Scrap/ScrapStatus.h"
#include "engine/EngineInclude.h"
#include <cstdint>
#include <Vector.h>

BreakBlockSystem::BreakBlockSystem() : ISystem(SystemType::StateTransition) {}

void BreakBlockSystem::Initialize() {
}

void BreakBlockSystem::Finalize() {
    /*entities_.clear();*/
}

BreakBlockSystem::~BreakBlockSystem() {}

void BreakBlockSystem::UpdateEntity(GameEntity* _entity) {
    if (!_entity) {
        return;
    }

    BlockStatus* blockStatus = getComponent<BlockStatus>(_entity);

    if (!blockStatus) {
        return;
    }

    if (blockStatus->GetIsBreak()) {
        ScrapSpawn(_entity);
        BlockReaction(blockStatus->GetBlockType());
        DestroyEntity(_entity);
    }
}

void BreakBlockSystem::BlockReaction(BlockType blocktype) {
    switch (blocktype) {
        ///---------------------------------------------
        /// Normal
        ///---------------------------------------------
    case BlockType::NORMAL:

        break;

        ///---------------------------------------------
        /// Skull
        ///---------------------------------------------
    case BlockType::SKULL:

        break;

        ///---------------------------------------------
        /// Advance
        ///---------------------------------------------
    case BlockType::ADVANTAGE:

        break;

    default:
        break;
    }
}

void BreakBlockSystem::ScrapSpawn(GameEntity* _entity) {
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* scrapEntity                  = ecsManager->getUniqueEntity("ScrapSpawner");

    if (!scrapEntity) {
        return;
    }

    ScrapSpawner* scrapSpawner = getComponent<ScrapSpawner>(scrapEntity);

    for (int32_t i = 0; i < scrapSpawner->GetCreateNum(); ++i) { // num分

        GameEntity* scrap = CreateEntity<Transform, Rigidbody, SphereCollider, ModelMeshRenderer, ScrapStatus>("Scrap", Transform(), Rigidbody(), SphereCollider(), ModelMeshRenderer(), ScrapStatus());

        // ================================= Componentを初期化 ================================= //

        //*Status
        ScrapStatus* status = getComponent<ScrapStatus>(scrap);
        float blowValueX    = 0.0f;
        float blowValueY    = scrapSpawner->GetBlowValue()[Y];

        // 吹っ飛び向きを決める
        if (i % 2 == 0) {
            status->SetBlowDirection(BlowDirection::LEFT);
            blowValueX = -scrapSpawner->GetBlowValue()[X];
        } else {
            status->SetBlowDirection(BlowDirection::RIGHT);
            blowValueX = scrapSpawner->GetBlowValue()[X];
        }

        //* transform
        Transform* transform = getComponent<Transform>(scrap);
        Transform* baseTransform = getComponent<Transform>(_entity);
        transform->translate     = baseTransform->translate;

        //* rigitBody
        // 初速、重力、massの設定

        Rigidbody* rigitBody = getComponent<Rigidbody>(scrap);
        rigitBody->setMass(scrapSpawner->GetMass());
        rigitBody->setVelocity(Vec3f(blowValueX, blowValueY, 0.0f));
        rigitBody->setUseGravity(true);

        //* collider
        SphereCollider* collider              = getComponent<SphereCollider>(scrap);
        collider->getLocalShapePtr()->radius_ = scrapSpawner->GetColliderRadius();

        //*model
        ModelMeshRenderer* modelMesh = getComponent<ModelMeshRenderer>(scrap);
        CreateModelMeshRenderer(modelMesh, scrap, kApplicationResourceDirectory + "/Models/Block", "Block.obj");

        // ================================= System ================================= //
        ECSManager* ecs = ECSManager::getInstance();

        //------------------ Input
        // None

        //------------------ StateTransition

        //------------------ Movement
        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(scrap);

        //------------------ Collision
        ecs->getSystem<CollisionCheckSystem>()->addEntity(scrap);
        //------------------ Physics
        // None

        //------------------ Render
        /* ecs->getSystem<ColliderRenderingSystem>()->addEntity(block);*/
        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(scrap);
    }
}
