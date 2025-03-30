#include "CreateFloorAndPillerSystem.h"

/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"

/// app
// component
#include "component/FloorAndPillerColum/FloorAndPillerSpawner.h"
#include"component/Floor/BottomFloorStates.h"
#include "component/Floor/FloorStates.h"
#include "component/Piller/PillerStates.h"



// system

CreateFloorAndPillerSystem::CreateFloorAndPillerSystem() : ISystem(SystemType::Movement) {}
CreateFloorAndPillerSystem::~CreateFloorAndPillerSystem() {}

void CreateFloorAndPillerSystem::Initialize() {
    isCreated_ = false;
}

void CreateFloorAndPillerSystem::Finalize() {
  /*  entities_.clear();*/
}

void CreateFloorAndPillerSystem::UpdateEntity(GameEntity* _entity) {
   
    FloorAndPillerSpawner* entitySpawner = getComponent<FloorAndPillerSpawner>(_entity);
    BottomFloorStates* bottomFloorStates = getComponent<BottomFloorStates>(_entity);
    PillerStates* pillerStates           = getComponent<PillerStates>(_entity);
    FloorStates* floorStates             = getComponent<FloorStates>(_entity);

    if (entitySpawner == nullptr || bottomFloorStates==nullptr||
        pillerStates == nullptr || floorStates==nullptr) {
        return;
    }
    if (isCreated_) {
        return;
    }

    // 建物を生成
    CreateFandP(_entity, *entitySpawner,floorStates,pillerStates);

    isCreated_ = true;
}

void CreateFloorAndPillerSystem::CreateFandP(
    GameEntity* _entity,const FloorAndPillerSpawner& fAndP,
     FloorStates* floorStates,  PillerStates* pillerStates) {

    for (size_t i = 0; i < fAndP.GetColumNum(); ++i) {

        // ================================= Bullet Entityを 生成 ================================= //
        GameEntity* piller = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer>("Piller", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer());
        GameEntity* floor  = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer>("Floor", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer());

        // ================================= Componentを初期化 ================================= //

        // Transform
        Transform* hostTransform = getComponent<Transform>(_entity); // 設置元
        Transform* pillerTransform  = getComponent<Transform>(piller); // 柱
        Transform* floorTransform = getComponent<Transform>(floor); // 床

        //　位置
        pillerTransform->translate = Vec3f(hostTransform->worldMat[3]) + Vec3f(0.0f, fAndP.GetPillerSpace() * float(i+1), 0.0f);
        floorTransform->translate  = Vec3f(hostTransform->worldMat[3]) + Vec3f(0.0f, fAndP.GetFloorSpace() * float(i + 1), 0.0f);

        //回転
        pillerTransform->rotate = hostTransform->rotate;
        floorTransform->rotate  = hostTransform->rotate;

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
        StatesPiller                = pillerStates;

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


//void ShotBulletSystem::SpawnBullet(GameEntity* _entity, BulletSpawner* _spawner) {
//    // ================================= Bullet Entityを 生成 ================================= //
//    GameEntity* bullet = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer>("Bullet", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer());
//
//    // ================================= Componentを初期化 ================================= //
//    // Transform
//    Transform* hostTransfomr = getComponent<Transform>(_entity);
//    Transform* transform     = getComponent<Transform>(bullet);
//    transform->translate     = Vec3f(hostTransfomr->worldMat[3]) + _spawner->getBulletOffset();
//
//    // Collider
//    SphereCollider* collider           = getComponent<SphereCollider>(bullet);
//    collider->getLocalShape()->radius_ = _spawner->getBulletSize();
//
//    // Rigidbody
//    Rigidbody* rigidbody = getComponent<Rigidbody>(bullet);
//    rigidbody->setVelocity(Vec3f(0.0f, _spawner->getBulletSpeed(), 0.0f));
//
//    // MeshRenderer
//    ModelMeshRenderer* renderer = getComponent<ModelMeshRenderer>(bullet);
//    // Model から MeshRenderer を作成
//    CreateModelMeshRenderer(renderer, bullet, kApplicationResourceDirectory + "/Models", "sphere.obj");
//
//    // Status
//     CharacterStatus* status = getComponent<CharacterStatus>(bullet);
//     status->setHP(1);
//     status->setAttack(1);
//
//    // ================================= System ================================= //
//    ECSManager* ecs = ECSManager::getInstance();
//
//    ////------------------ Input
//    //// None
//
//    ////------------------ StateTransition
//    ecs->getSystem<DeleteCharacterEntitySystem>()->addEntity(bullet);
//
//    ////------------------ Movement
//    ecs->getSystem<MoveSystemByRigidBody>()->addEntity(bullet);
//
//    ////------------------ Collision
//    ecs->getSystem<CharacterOnCollision>()->addEntity(bullet);
//    ecs->getSystem<CollisionCheckSystem>()->addEntity(bullet);
//
//    ////------------------ Physics
//    //// None
//
//    ////------------------ Render
//    ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(bullet);
//}
