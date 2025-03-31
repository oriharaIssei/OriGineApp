#include "CreateFloorAndPillerSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

/// app
// component
#include "component/Floor/FloorStates.h"
#include "component/FloorAndPillerColum/FloorAndPillerSpawner.h"
#include "component/Piller/PillerStates.h"

// system

CreateFloorAndPillerSystem::CreateFloorAndPillerSystem() : ISystem(SystemType::Movement) {}
CreateFloorAndPillerSystem::~CreateFloorAndPillerSystem() {}

void CreateFloorAndPillerSystem::Initialize() {
   
}

void CreateFloorAndPillerSystem::Finalize() {
    /*  entities_.clear();*/
}

void CreateFloorAndPillerSystem::UpdateEntity(GameEntity* _entity) {

    FloorAndPillerSpawner* entitySpawner = getComponent<FloorAndPillerSpawner>(_entity);
 /*   BottomFloorStates* bottomFloorStates = getComponent<BottomFloorStates>(_entity);*/
    PillerStates* pillerStates           = getComponent<PillerStates>(_entity);
    FloorStates* floorStates             = getComponent<FloorStates>(_entity);

    if (!entitySpawner   || !pillerStates || !floorStates) {
        return;
    }
    if (entitySpawner->GetIsCreated()) {
        return;
    }

    // 建物を生成
    CreateFandP(_entity, entitySpawner, floorStates, pillerStates);

    entitySpawner->SetIsCreated(true);
   
}

void CreateFloorAndPillerSystem::CreateFandP(
    GameEntity* _entity,  FloorAndPillerSpawner* fAndP,
    FloorStates* floorStates, PillerStates* pillerStates) {

    for (size_t i = 0; i < size_t(fAndP->GetColumNum()); ++i) {

        // ================================= Bullet Entityを 生成 ================================= //
        GameEntity* piller = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer>("Piller", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer());
        GameEntity* floor  = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer>("Floor", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer());

        // ================================= Componentを初期化 ================================= //

        // Transform
        Transform* hostPivotTransform = getComponent<Transform>(_entity,1); // 設置元
        Transform* hostTransform      = getComponent<Transform>(_entity, 0); // 設置元
        Transform* pillerTransform    = getComponent<Transform>(piller); // 柱
        Transform* floorTransform     = getComponent<Transform>(floor); // 床

        // parent
        pillerTransform->parent = hostPivotTransform;
        floorTransform->parent  = hostPivotTransform;

        // 　位置
        if (i == 0) {
            pillerTransform->translate = Vec3f(hostTransform->translate) + Vec3f(0.0f, fAndP->GetFirstPillerOffset(), 0.0f);
        } else {

            pillerTransform->translate = Vec3f(hostTransform->translate) + Vec3f(0.0f, fAndP->GetFirstPillerOffset() +(fAndP->GetPillerSpace() * float(i)), 0.0f);
        }

        floorTransform->translate  = Vec3f(hostTransform->translate) + Vec3f(0.0f, fAndP->GetFloorSpace() * float(i), 0.0f);
   
        // Collider
        SphereCollider* collider           = getComponent<SphereCollider>(piller);
        collider->getLocalShape()->radius_ = pillerStates->GetCollisionSize();

        // MeshRenderer
        ModelMeshRenderer* pillerRender = getComponent<ModelMeshRenderer>(piller);
        ModelMeshRenderer* floorRender  = getComponent<ModelMeshRenderer>(floor);

        // Model から MeshRenderer を作成
        CreateModelMeshRenderer(pillerRender, piller, kApplicationResourceDirectory + "/Models/Piller", "Piller.obj");
        CreateModelMeshRenderer(floorRender, floor, kApplicationResourceDirectory + "/Models/whiteFloor", "whiteFloor.obj");

        /// States
        FloorStates* StatesFloor = getComponent<FloorStates>(floor);
        StatesFloor              = floorStates;

        PillerStates* StatesPiller = getComponent<PillerStates>(floor);
        StatesPiller               = pillerStates;

        // ================================= System ================================= //
        ECSManager* ecs = ECSManager::getInstance();
        //------------------ Input
        // None

        //------------------ StateTransition
        /* ecs->getSystem<DeleteCharacterEntitySystem>()->addEntity(bullet);*/

        //------------------ Movement
        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(piller);
        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(floor);

        //------------------ Collision
        /*  ecs->getSystem<CharacterOnCollision>()->addEntity(bom);*/
        ecs->getSystem<CollisionCheckSystem>()->addEntity(piller);
        //------------------ Physics
        // None

        //------------------ Render
        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(piller);
        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(floor);
    }
}
