//#include "CreateFloorAndPillerSystem.h"
//
///// engine
//#define ENGINE_INCLUDE
//
//#define RESOURCE_DIRECTORY // Resource の Directory
//// ECS
//#define ENGINE_ECS
//#include "engine/EngineInclude.h"
//
///// app
//// component
//#include "component/Bom/ExplotionCollision.h"
//#include "component/Floor/FloorStates.h"
//#include "component/FloorAndPillerColum/FloorAndPillerrStatus.h"
//#include "component/FloorAndPillerColum/FloorAndPillerSpawner.h"
//#include "component/Piller/PillerStates.h"
//
//// system
//#include "system/Piller/DeletePillerSystem.h"
//#include "system/Piller/PillerDamageSystem.h"
//
//CreateFloorAndPillerSystem::CreateFloorAndPillerSystem() : ISystem(SystemType::Movement) {}
//CreateFloorAndPillerSystem::~CreateFloorAndPillerSystem() {}
//
//void CreateFloorAndPillerSystem::Initialize() {
//}
//
//void CreateFloorAndPillerSystem::Finalize() {
//    /*  entities_.clear();*/
//}
//
//void CreateFloorAndPillerSystem::UpdateEntity(GameEntity* _entity) {
//
//    FloorAndPillerSpawner* entitySpawner = getComponent<FloorAndPillerSpawner>(_entity);
//    PillerStates* pillerStates           = getComponent<PillerStates>(_entity);
//    FloorStates* floorStates             = getComponent<FloorStates>(_entity);
//
//    if (!entitySpawner || !pillerStates || !floorStates) {
//        return;
//    }
//    if (entitySpawner->GetIsCreated()) {
//        return;
//    }
//
//
//
//    // 建物を生成
//    CreateFandP(_entity, entitySpawner, floorStates, pillerStates);
//
//    entitySpawner->SetIsCreated(true);
//}
//
//void CreateFloorAndPillerSystem::CreateFandP(
//    GameEntity* _entity, FloorAndPillerSpawner* fAndP,
//    FloorStates* floorStates, PillerStates* pillerStates) {
//
//    for (int32_t i = 0; i < fAndP->GetColumNumMax(); ++i) {
//
//        // ================================= Bullet Entityを 生成 ================================= //
//        GameEntity* piller         = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer, FloorStates>("Piller", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer(), FloorStates());
//        GameEntity* floor          = CreateEntity<Transform, SphereCollider, Rigidbody, ModelMeshRenderer, PillerStates>("Floor", Transform(), SphereCollider(), Rigidbody(), ModelMeshRenderer(), PillerStates());
//        GameEntity* floorAndPiller = CreateEntity<Transform, Rigidbody, FloorAndPillerrStatus>("FAndP", Transform(), Rigidbody(), FloorAndPillerrStatus());
//
//        // ================================= Componentを初期化 ================================= //
//
//        // Transform
//        Transform* hostPivotTransform = getComponent<Transform>(_entity, 1); // 設置元
//        Transform* hostTransform      = getComponent<Transform>(_entity, 0); // 設置元
//
//        Transform* fAndPTransform  = getComponent<Transform>(floorAndPiller);
//        Transform* pillerTransform = getComponent<Transform>(piller); // 柱
//        Transform* floorTransform  = getComponent<Transform>(floor); // 床
//
//        // parent
//        fAndPTransform          = hostPivotTransform;
//        pillerTransform->parent = fAndPTransform;
//        floorTransform->parent  = fAndPTransform;
//
//        // 　親のY位置を設定
//        if (i == 0) {
//            fAndPTransform->translate = Vec3f(0.0f, fAndP->GetFirstPillerOffset(), 0.0f);
//
//        } else {
//            fAndPTransform->translate = Vec3f(0.0f, fAndP->GetFirstPillerOffset() + (fAndP->GetPillerSpace() * float(i)), 0.0f);
//        }
//
//        fAndPTransform->Update();
//
//        //子の
//        pillerTransform->translate = Vec3f(hostTransform->translate) + Vec3f(0.0f, fAndP->GetFirstPillerOffset(), 0.0f);
//        floorTransform->translate  = Vec3f(hostTransform->translate) + Vec3f(0.0f, fAndP->GetFloorSpace(), 0.0f);
//
//        // Collider
//        SphereCollider* collider           = getComponent<SphereCollider>(piller);
//        collider->getLocalShape()->radius_ = pillerStates->GetCollisionSize();
//
//        // MeshRenderer
//        ModelMeshRenderer* pillerRender = getComponent<ModelMeshRenderer>(piller);
//        ModelMeshRenderer* floorRender  = getComponent<ModelMeshRenderer>(floor);
//
//        // Model から MeshRenderer を作成
//        CreateModelMeshRenderer(pillerRender, piller, kApplicationResourceDirectory + "/Models/Piller", "Piller.obj");
//        CreateModelMeshRenderer(floorRender, floor, kApplicationResourceDirectory + "/Models/whiteFloor", "whiteFloor.obj");
//
//        // ↓ここのパラメータコピーがエディターで設定した値にならない(初期化設定した値は問題なし)
//
//        /// States
//        FloorStates* statusFloor = getComponent<FloorStates>(floor);
//        statusFloor              = floorStates;
//
//        PillerStates* statusPiller = getComponent<PillerStates>(piller);
//        statusPiller               = pillerStates;
//        statusPiller->SetCurrentHp(pillerStates->GetHpMax());
//
//        // row,columNum
//        FloorAndPillerrStatus* statusFandP = getComponent<FloorAndPillerrStatus>(floorAndPiller);
//        statusFandP->SetColumAndRow(i, fAndP->GetRowNumber());
//       
//        // ================================= System ================================= //
//        ECSManager* ecs = ECSManager::getInstance();
//        //------------------ Input
//        // None
//
//        //------------------ StateTransition
//        ecs->getSystem<CanageStateFallSystem>()->addEntity(piller);
//
//        //------------------ Movement
//        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(piller);
//        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(floor);
//        ecs->getSystem<MoveSystemByRigidBody>()->addEntity(floorAndPiller);
//
//        //------------------ Collision
//        /*  ecs->getSystem<CharacterOnCollision>()->addEntity(bom);*/
//        ecs->getSystem<CollisionCheckSystem>()->addEntity(piller);
//        ecs->getSystem<CollisionCheckSystem>()->addEntity(floor);
//        ecs->getSystem<PillerDamageSystem>()->addEntity(piller);
//        //------------------ Physics
//        // None
//
//        //------------------ Render
//        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(piller);
//        ecs->getSystem<TexturedMeshRenderSystem>()->addEntity(floor);
//    }
//}
